#ifndef ENTRYLIST_H
#define ENTRYLIST_H

#include <QDialog>

namespace Ui {
class EntryList;
}

class EntryList : public QDialog
{
    Q_OBJECT

public:
    explicit EntryList(QWidget *parent = 0);
    ~EntryList();

private:
    Ui::EntryList *ui;
};

#endif // ENTRYLIST_H
