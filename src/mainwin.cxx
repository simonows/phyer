#include <QStyle>
#include <QDesktopWidget>
#include <QMessageBox>
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
    this->ui->deleteButton->setEnabled(false);

    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(this);
    QPoint center = rect.center();
    int x = center.x() - (width()/2);
    int y = center.y() - (height()/2);
    center.setX(x);
    center.setY(y);
    move(center);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(void)
{
    unsigned long temp = this->ui->lineEdit_2->text().toULong(nullptr, 10);
    QMessageBox box;

    if (temp < 16 || temp > 31)
    {
        box.setText(QString::asprintf("Wrong address %lu value", temp));
        box.exec();
        return;
    }

    if (set_optional.indexOf(temp) != -1)
    {
        box.setText(QString::asprintf("Address %lu already exist", temp));
        box.exec();
        return;
    }

    set_optional.push_back(temp);
    model_optional->append(
        RegisterFlag(
            temp
          , this->ui->lineEdit_3->text()
          , QString::asprintf("0x%04hX"
          , hrd.getRegisterValue(temp))
          , ""
        )
    );
}

void MainWindow::on_editButton_clicked(void)
{
    if (ui->tableView->currentIndex().row() == -1)
    {
        QMessageBox msgBox;
        msgBox.setText("You should choose a row before");
        msgBox.exec();
        return;
    }

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

void MainWindow::on_deleteButton_clicked(void)
{
    if (ui->tableView2->currentIndex().row() == -1)
    {
        QMessageBox msgBox;
        msgBox.setText("You should choose a row before");
        msgBox.exec();
        return;
    }
    model_optional->deleteRow(ui->tableView2->currentIndex().row());
}


void MainWindow::on_selectButton_clicked(void)
{
    hrd.setItem(this->ui->interfaceComboBox->currentText());

    QList<RegisterFlag> temp = hrd.getRegisterSet();

    if (temp.count() <= 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Not enough roots for read interface");
        msgBox.exec();
        this->ui->infoLabel->setText("Not enough roots for read interface");
        this->ui->pushButton->setEnabled(false);
        this->ui->editButton->setEnabled(false);
        this->ui->deleteButton->setEnabled(false);
        return;
    }

    this->ui->infoLabel->setText(this->ui->interfaceComboBox->currentText() + " selected");
    this->ui->pushButton->setEnabled(true);
    this->ui->editButton->setEnabled(true);
    this->ui->deleteButton->setEnabled(true);

    for (size_t i = values.count(); i > 0 ; i--)
    {
        model->deleteRow(i - 1);
    }

    model->populate(temp);
}

