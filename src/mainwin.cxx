#include "mainwin.h"
#include "../res/ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    values = new QList<RegisterFlag>;

    values->append(RegisterFlag(0,"Control", "", ""));
    values->append(RegisterFlag(1, "Status", "", ""));
    values->append(RegisterFlag(2, "PHY Identifier", "", ""));
    values->append(RegisterFlag(3, "PHY Identifier", "", ""));
    values->append(RegisterFlag(4, "Auto-Negotiation Advertisement", "", ""));
    values->append(RegisterFlag(5, "Auto-Negotiation Link Partner Base Page Ability", "", ""));
    values->append(RegisterFlag(6, "Auto-Negotiation Expansion", "", ""));
    values->append(RegisterFlag(7, "Auto-Negotiation Next Page Transmit", "", ""));
    values->append(RegisterFlag(8, "Auto-Negotiation Link Partner Received Next Page", "", ""));
    values->append(RegisterFlag(9, "MASTER-SLAVE Control Register", "", ""));
    values->append(RegisterFlag(10, "MASTER-SLAVE Status Register", "", ""));
    values->append(RegisterFlag(11, "PSE Control register", "", ""));
    values->append(RegisterFlag(12, "PSE/PD Status register", "", ""));
    values->append(RegisterFlag(13, "Reserved", "", ""));
    values->append(RegisterFlag(14, "Reserved", "", ""));
    values->append(RegisterFlag(15, "Extended Status", "", ""));

    model = new QTableViewModel();
    model->populate(values);
    this->ui->tableView->setModel(model);

    newidx = 100;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    newidx++;
    QString strIdx = QString().number(newidx);

    model->append(RegisterFlag(newidx, "Optional", "", ""));
}

void MainWindow::on_pushButton_2_clicked()
{
    model->update(ui->tableView->currentIndex().row(), RegisterFlag(16, "Optional", "", ""));
}

void MainWindow::on_pushButton_3_clicked()
{
    model->deleteRow(0);
}

void MainWindow::on_pushButton_4_clicked()
{
    model->insertAt(0, RegisterFlag(16, "Optional", "", ""));
}

