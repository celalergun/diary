#ifndef DATABASEOPERATIONS_H
#define DATABASEOPERATIONS_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QSqlError>
#include <QInputDialog>
#include <QDateTime>
#include <QObject>
#include <QCryptographicHash>

class EntryMetadataEntity
{
public:
    int Id;
    QString Title;
    QDateTime TimeStamp;
};

class EntryEntity
{
public:
    EntryMetadataEntity MetaData;
    QString Entry;
    QByteArray EncKey;
};

class DatabaseOperations
{
public:
    DatabaseOperations();

    bool Connect(QString DosyaAdi);

    int AddNewEntryToDatabase(QString title);

    QString RetrieveEntryFromDatabase(int rowId);

    bool DeleteFromDatabase(int rowId);

    bool SaveToDatabase(QString entry);

    QList<EntryMetadataEntity> RetrieveList(QDateTime fromWhen);

    static QByteArray EncryptString(QString input, QByteArray EncKey);

    static QString DecryptString(QByteArray data, QByteArray DecKey);

    bool RunQuery(QString Query);

    int EntryCount();

    bool SetEncryption(bool useEncryption, QString password);

    bool IsPreferencesSet();

private:
    QSqlDatabase DB;

public:
    QByteArray m_CurrentKey;
    QString m_Title;
    QString m_Entry;
    int m_Id;
    QString m_TimeStamp;
};

#endif // DATABASEOPERATIONS_H
