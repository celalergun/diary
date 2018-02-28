#include "entrylist.h"
#include "ui_entrylist.h"

EntryList::EntryList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EntryList)
{
    ui->setupUi(this);
    List = QList<Entry>();
    ui->tableWidget->setColumnCount(3);
    QStringList labels;
    labels << "Id" << "Title" << "Date";
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->tableWidget->setFocus();
}

EntryList::~EntryList()
{
    delete ui;
}

void EntryList::Add(int id, QString title, QString timestamp)
{
    Entry result;
    result.Id = id;
    result.Title = title;
    result.Timestamp = timestamp;
    List.append(result);

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(m_Count, 0, new QTableWidgetItem(QString::number(id)));

    QTableWidgetItem *Title = new QTableWidgetItem(title);
    ui->tableWidget->setItem(m_Count, 1, Title);

    QTableWidgetItem *TimeStamp = new QTableWidgetItem(timestamp);
    ui->tableWidget->setItem(m_Count, 2, TimeStamp);
    m_Count++;
}

int EntryList::SelectedId()
{
    return m_SelectedId;
}

void EntryList::on_buttonBox_accepted()
{
    auto sel = ui->tableWidget->selectedItems();
    if (sel.count() == 0)
        return;

    m_Selected = sel[0]->row();
    m_SelectedId = List[m_Selected].Id;
}

int EntryList::SelectedRow()
{
    return m_Selected;
}
