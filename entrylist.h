#ifndef ENTRYLIST_H
#define ENTRYLIST_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QStringList>
#include "databaseoperations.h"

namespace Ui {
class EntryList;
}

class Entry;
class EntryList : public QDialog
{
    Q_OBJECT

public:
    explicit EntryList(QWidget *parent = 0);
    ~EntryList();

    void Add(int id, QString title, QString timestamp);
    int SelectedRow();
    int SelectedId();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EntryList *ui;

    QList<Entry> List;

    int m_Count = 0;
    int m_Selected = -1;
    int m_SelectedId = -1;
};

class Entry
{
public:
    int Id;
    QString Title;
    QString Timestamp;
};

#endif // ENTRYLIST_H
