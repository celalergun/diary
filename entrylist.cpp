#include "entrylist.h"
#include "ui_entrylist.h"

EntryList::EntryList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EntryList)
{
    ui->setupUi(this);
}

EntryList::~EntryList()
{
    delete ui;
}
