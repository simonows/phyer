#include "mainwin.h"
#include "../res/ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    values.append(RegisterFlag(0,"Control", "", ""));
    values.append(RegisterFlag(1, "Status", "", ""));
    values.append(RegisterFlag(2, "PHY Identifier", "", ""));
    values.append(RegisterFlag(3, "PHY Identifier", "", ""));
    values.append(RegisterFlag(4, "Auto-Negotiation Advertisement", "", ""));
    values.append(RegisterFlag(5, "Auto-Negotiation Link Partner Base Page Ability", "", ""));
    values.append(RegisterFlag(6, "Auto-Negotiation Expansion", "", ""));
    values.append(RegisterFlag(7, "Auto-Negotiation Next Page Transmit", "", ""));
    values.append(RegisterFlag(8, "Auto-Negotiation Link Partner Received Next Page", "", ""));
    values.append(RegisterFlag(9, "MASTER-SLAVE Control Register", "", ""));
    values.append(RegisterFlag(10, "MASTER-SLAVE Status Register", "", ""));
    values.append(RegisterFlag(11, "PSE Control register", "", ""));
    values.append(RegisterFlag(12, "PSE/PD Status register", "", ""));
    values.append(RegisterFlag(13, "Reserved", "", ""));
    values.append(RegisterFlag(14, "Reserved", "", ""));
    values.append(RegisterFlag(15, "Extended Status", "", ""));

    model = new QTableViewModel();
    model_optional = new QTableViewModel();
    model->populate(values);
    this->ui->tableView->setModel(model);
    this->ui->tableView->setColumnWidth(0, 50);
    this->ui->tableView->setColumnWidth(1, 260);

    this->ui->tableView2->setModel(model_optional);
    this->ui->tableView2->setColumnWidth(0, 50);
    this->ui->tableView2->setColumnWidth(1, 260);

    this->ui->interfaceComboBox->addItems(hrd.getItems());

    newidx = 100;

    this->ui->pushButton->setEnabled(false);
    this->ui->editButton->setEnabled(false);
    this->ui->pushButton_3->setEnabled(false);
    this->ui->pushButton_4->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(void)
{
    newidx++;
    QString strIdx = QString().number(newidx);

    model->append(RegisterFlag(newidx, "Optional", "", ""));
}

void MainWindow::on_editButton_clicked(void)
{
    hrd.setRegisterValue(
        model->dataAddr(ui->tableView->currentIndex())
      , static_cast<unsigned short>(
            this->ui->lineEdit->text().toUShort(nullptr, 16)
        )
    );

    QList<RegisterFlag> temp = hrd.getRegisterSet();
    if (temp.count() > 0)
    {
        for (size_t i = values.count(); i > 0 ; i--)
        {
            model->deleteRow(i - 1);
        }
        model->populate(temp);
    }
}

void MainWindow::on_pushButton_3_clicked(void)
{
    model->deleteRow(0);
}

void MainWindow::on_pushButton_4_clicked(void)
{
    model->insertAt(0, RegisterFlag(16, "Optional", "", ""));
}

void MainWindow::on_selectButton_clicked(void)
{
    hrd.setItem(this->ui->interfaceComboBox->currentText());

    QList<RegisterFlag> temp = hrd.getRegisterSet();

    if (temp.count() <= 0)
    {
        this->ui->infoLabel->setText("Not enough roots for read interface");
        this->ui->pushButton->setEnabled(false);
        this->ui->editButton->setEnabled(false);
        this->ui->pushButton_3->setEnabled(false);
        this->ui->pushButton_4->setEnabled(false);
        return;
    }

    this->ui->infoLabel->setText(this->ui->interfaceComboBox->currentText() + " selected");
    this->ui->pushButton->setEnabled(true);
    this->ui->editButton->setEnabled(true);
    this->ui->pushButton_3->setEnabled(true);
    this->ui->pushButton_4->setEnabled(true);

    for (size_t i = values.count(); i > 0 ; i--)
    {
        model->deleteRow(i - 1);
    }

    model->populate(temp);
}

