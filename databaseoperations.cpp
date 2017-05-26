#include "databaseoperations.h"

DatabaseOperations::DatabaseOperations()
{
}

bool DatabaseOperations::Connect(QString DosyaAdi)
{
    if (!QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
        QMessageBox(QMessageBox::Critical, "Error", QObject::tr("SQLite driver is not loaded"), QMessageBox::Ok).exec();
        return false;
    }

    DB = QSqlDatabase::addDatabase("QSQLITE", "Baglanti");
    QDir Dir(QDir::homePath() +"/.diary/");
    if (!Dir.exists())
        Dir.mkdir(Dir.path());
    Dir.setCurrent(Dir.path());
    DB.setDatabaseName(DosyaAdi);

    if (!DB.open())
    {
        QString Hata = DB.lastError().text();
        QMessageBox(QMessageBox::Critical, "Error", "Cannot create database: "+Hata, QMessageBox::Ok).exec();
        return false;
    }

    if (!RunQuery("Create Table If Not Exists Diary ("
                  "Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "Timestamp DATE DEFAULT (datetime('now','localtime')), "
                  "Title varchar(100), "
                  "EncKey varbinary(32) DEFAULT (randomblob(32)), "
                  "Entry BLOB)"))
    {
        QString Hata = DB.lastError().text();
        QMessageBox(QMessageBox::Critical, "Error", "Diary table cannot be created: "+Hata, QMessageBox::Ok).exec();
        return false;
    }

    if (!RunQuery("Create Table If Not Exists Preferences ("
                  "UseEncryption bit DEFAULT (0), "
                  "PasswordHash varbinary(64))"))
    {
        QString Hata = DB.lastError().text();
        QMessageBox(QMessageBox::Critical, "Error", "Preferences table cannot be created: "+Hata, QMessageBox::Ok).exec();
        return false;
    }
    return true;
}

int DatabaseOperations::AddNewEntryToDatabase(QString title)
{
    QSqlQuery Q("Insert Into Diary (Title) Values (:Title);", DB);
    Q.bindValue(0, title);
    if (!Q.exec())
    {
        QString Hata = DB.lastError().text();
        QMessageBox(QMessageBox::Critical, "Error", "Cannot add new entry to the database: "+Hata, QMessageBox::Ok).exec();
        return -1;
    }
    return Q.lastInsertId().toInt();
}

QString DatabaseOperations::RetrieveEntryFromDatabase(int rowId)
{
    if (rowId == -1)
        throw "Invalid Entry Id specified";

    QSqlQuery Q("Select Id, Timestamp, Title, EncKey, Entry From Diary Where Id = :Id;", DB);
    Q.bindValue(0, rowId);
    if (!Q.exec())
    {
        QString Hata = DB.lastError().text();
        QMessageBox(QMessageBox::Critical, "Error", "Cannot retrieve entry from the database: "+Hata, QMessageBox::Ok).exec();
        return "";
    }
    Q.next();

    m_CurrentKey = Q.value("EncKey").toByteArray();
    QByteArray arr = Q.value("Entry").toByteArray();
    m_Entry = DecryptString(arr, m_CurrentKey);
    m_Title = Q.value("Title").toString();
    m_Id = rowId;
    m_TimeStamp = Q.value("Timestamp").toString();
    return m_Entry;
}

bool DatabaseOperations::DeleteEntryFromDatabase(int rowId)
{
    QSqlQuery Q("Delete From Diary Where Id = :Id;", DB);
    Q.bindValue(0, rowId);
    return Q.exec();
}

bool DatabaseOperations::SaveEntryToDatabase(QString entry)
{
    m_Entry = entry;
    QByteArray arr = EncryptString(entry, m_CurrentKey);
    QSqlQuery Q("Update Diary Set Entry = :Entry Where Id = :Id;", DB);
    Q.bindValue(0, arr);
    Q.bindValue(1, m_Id);
    return Q.exec();
}

QList<EntryMetadataEntity> DatabaseOperations::RetrieveEntryList(QDateTime fromWhen)
{
    QList<EntryMetadataEntity> result;
    QSqlQuery Q("Select Id, Timestamp, Title From Diary Where Timestamp >= :Timestamp Order By Timestamp DESC;", DB);
    Q.bindValue(0, fromWhen);
    if (!Q.exec())
    {
        throw "Cannot get entry list";
    }

    while (Q.next())
    {
        EntryMetadataEntity row;
        row.Id = Q.value("Id").toInt();
        row.TimeStamp = Q.value("Timestamp").toDateTime();
        row.Title = Q.value("Title").toString();
        result.append(row);
    }

    return result;
}

// reduces 512 bits hash to 128 bits key because our AES library is 128 bits
QByteArray ReduceKey(QByteArray LongKey)
{
    QByteArray result;
    result.fill(0xCC, 16);
    for (int i = 0; i < LongKey.count(); ++i)
    {
        result[i % 16] = result[i % 16] ^ LongKey[i];
    }
    return result;
}

QByteArray DatabaseOperations::EncryptString(QString input, QByteArray EncKey)
{
    QCryptographicHash hasher(QCryptographicHash::Sha512);
    hasher.addData(m_MasterPasswordHash);
    hasher.addData(EncKey);
    QByteArray Key = hasher.result();

    AesClass enc;
    return enc.Encrypt(input.toUtf8(), ReduceKey(Key));
}

QString DatabaseOperations::DecryptString(QByteArray input, QByteArray DecKey)
{
    QCryptographicHash hasher(QCryptographicHash::Sha512);
    hasher.addData(m_MasterPasswordHash);
    hasher.addData(DecKey);
    QByteArray Key = hasher.result();

    AesClass dec;
    return QString::fromUtf8(dec.Decrypt(input, ReduceKey(Key)));
}

bool DatabaseOperations::RunQuery(QString Query)
{
    QSqlQuery Q(Query, DB);
    return Q.exec();
}

int DatabaseOperations::EntryCount()
{
    QSqlQuery Q("Select Count(*) As NumRows From Diary;", DB);
    if (!Q.exec())
    {
        throw "Cannot get row count";
    }

    Q.next();
    return Q.value("NumRows").toInt();
}

bool DatabaseOperations::SetEncryption(bool useEncryption, QString password)
{
    QByteArray hash = HashString(password);

    if (useEncryption)
    {
        QSqlQuery Q("Replace Into Preferences (UseEncryption, PasswordHash) Values (:useEncryption, :hash);", DB);
        Q.bindValue(0, useEncryption);
        Q.bindValue(1, hash);
        return Q.exec();
    }
    return false;
}

bool DatabaseOperations::IsPreferencesSet()
{
    QSqlQuery Q("Select Count(*) As NumRows From Preferences;", DB);
    if (!Q.exec())
    {
        throw "Cannot get row count";
    }

    Q.next();
    return (Q.value("NumRows").toInt() > 0);
}

bool DatabaseOperations::IsPasswordValid(QString password)
{
    QSqlQuery Q("Select PasswordHash From Preferences;", DB);
    if (!Q.exec())
    {
        throw "Cannot read preferences";
    }

    Q.next();
    QByteArray Original = Q.value("PasswordHash").toByteArray();
    if (Original.count() != 64)
    {
        throw "Configuration error. Password hash must be 512 bits";
    }
    m_MasterPasswordHash = HashString(password);
    return (Original == m_MasterPasswordHash);
}

QByteArray DatabaseOperations::HashString(QString password)
{
    QCryptographicHash hasher(QCryptographicHash::Sha512);
    QByteArray arr = password.toUtf8();
    hasher.addData(arr);
    return hasher.result();
}

