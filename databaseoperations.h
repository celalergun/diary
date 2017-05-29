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
#include <aesclass.h>

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

    bool DeleteEntryFromDatabase(int rowId);

    bool SaveEntryToDatabase(QString entry);

    QList<EntryMetadataEntity> RetrieveEntryList(QDateTime fromWhen);

    QByteArray StretchKey(QByteArray EncKey);

    QByteArray EncryptString(QString input, QByteArray EncKey);

    QString DecryptString(QByteArray input, QByteArray DecKey);

    bool RunQuery(QString Query);

    int EntryCount();

    bool SetEncryption(bool useEncryption, QString password, QStringList fileList);

    bool IsPreferencesSet();

    bool IsPasswordValid(QString password, QStringList fileList);

    void ReadPreferences();

    bool IsPasswordEnabled();

private:
    QSqlDatabase DB;

    QByteArray m_MasterPasswordHash;

    QByteArray CalculateHash(QString password, QStringList fileList);
    QString m_Title;
    QString m_TimeStamp;
    bool m_UseEncryption;

public:
    QByteArray m_CurrentKey;
    QString m_Entry;
    int m_Id;
    QString CurrentTitle()
    {
        return m_Title;
    }
    QString CurrentTimestamp()
    {
        return m_TimeStamp;
    }
};

#endif // DATABASEOPERATIONS_H
