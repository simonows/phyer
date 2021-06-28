#include <QStyle>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QSettings>
#include "mainwin.h"
#include "../ui_mainwindow.h"
#include "regwin.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    values.append(RegisterFlag(0,"Control", "", "", ""));
    values.append(RegisterFlag(1, "Status", "", "", ""));
    values.append(RegisterFlag(2, "PHY Identifier", "", "", ""));
    values.append(RegisterFlag(3, "PHY Identifier", "", "", ""));
    values.append(RegisterFlag(4, "Auto-Negotiation Advertisement", "", "", ""));
    values.append(RegisterFlag(5, "Auto-Negotiation Link Partner Base Page Ability", "", "", ""));
    values.append(RegisterFlag(6, "Auto-Negotiation Expansion", "", "", ""));
    values.append(RegisterFlag(7, "Auto-Negotiation Next Page Transmit", "", "", ""));
    values.append(RegisterFlag(8, "Auto-Negotiation Link Partner Received Next Page", "", "", ""));
    values.append(RegisterFlag(9, "MASTER-SLAVE Control Register", "", "", ""));
    values.append(RegisterFlag(10, "MASTER-SLAVE Status Register", "", "", ""));
    values.append(RegisterFlag(11, "PSE Control register", "", "", ""));
    values.append(RegisterFlag(12, "PSE/PD Status register", "", "", ""));
    values.append(RegisterFlag(13, "Reserved", "", "", ""));
    values.append(RegisterFlag(14, "Reserved", "", "", ""));
    values.append(RegisterFlag(15, "Extended Status", "", "", ""));

    model = new QTableViewModel(&hrd);
    model->populate(values);
    this->ui->tableView->setModel(model);
    this->ui->tableView->setColumnWidth(0, 50);
    this->ui->tableView->setColumnWidth(1, 310);
    this->ui->tableView->setColumnWidth(3, 50);

    connect(this->ui->tableView,
        SIGNAL(clicked(const QModelIndex &))
      , this
      , SLOT(handleOnTableClicked(const QModelIndex &))
    );
    connect(this->ui->pageComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index)
        {
            if (!is_selected)
            {
                return;
            }
            for (size_t i = values.count(); i > 0 ; i--)
            {
                model->deleteRow(i - 1);
            }

            QList<RegisterFlag> temp = hrd.getRegisterSet(this->ui->interfaceComboBox->currentIndex(), index);
            model->populate(temp);
            model->page = index;
            values = temp;
        }
    );

    QStringList pages;
    for (int i = 0; i < 8; i++)
    {
        if (i == 2 || i == 3 || i == 6)
        {
            continue;
        }
        pages << QString::asprintf("%i", i);
    }
    this->ui->pageComboBox->addItems(pages);
    this->ui->interfaceComboBox->addItems(hrd.getItems());

    newidx = 100;

    this->ui->pushButton->setEnabled(false);
    this->ui->deleteButton->setEnabled(false);
    this->ui->saveButton->setEnabled(false);

    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(this);
    QPoint center = rect.center();
    int x = center.x() - (width()/2);
    int y = center.y() - (height()/2);
    center.setX(x);
    center.setY(y);
    move(center);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    is_selected = false;
}

void currentIndexChanged(int index)
{
}

void MainWindow::handleOnTableClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    this->ui->descEdit->clear();
    this->ui->descEdit->insertPlainText(model->values->at(index.row()).getDesc());
}

void MainWindow::on_saveButton_clicked(void)
{
    if (this->ui->tableView->currentIndex().isValid())
    {
        (*model->values)[this->ui->tableView->currentIndex().row()].setDesc(
            this->ui->descEdit->toPlainText()
        );
        hrd.updatePage(this->ui->pageComboBox->currentIndex(), *(this->model->values));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(void)
{
    QMessageBox box;
    RegWindow dlg(this);
    unsigned long temp = 0;

    if (dlg.exec() != QDialog::Accepted)
    {
        return;
    }

    temp = dlg.getAddr();

    if (temp < Hardware::getStdRegCount() || temp > Hardware::MAX_ADDR)
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

    model->append(
        RegisterFlag(
            temp
          , dlg.getName()
          , QString::asprintf("0x%04hX", hrd.getRegisterValue(temp))
          , dlg.getMode()
          , dlg.getDesc()
        )
    );
    hrd.updatePage(this->ui->pageComboBox->currentIndex(), *(this->model->values));
}


void MainWindow::on_deleteButton_clicked(void)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    if (ui->tableView->currentIndex().row() == -1)
    {
        QMessageBox msgBox;
        msgBox.setText("You should choose a row before");
        msgBox.exec();
        return;
    }
    /*if (ui->tableView->currentIndex().row() < Hardware::getStdRegCount())
    {
        QMessageBox msgBox;
        msgBox.setText("You can't delete standard set of registers");
        msgBox.exec();
        return;
    }*/
    settings.remove(QString::asprintf("%s_%d.db/%d",
this->ui->interfaceComboBox->currentText().toLocal8Bit().data(), model->page, ui->tableView->currentIndex().row() + 1));
    set_optional.removeOne(model->dataAddr(ui->tableView->currentIndex()));
    model->deleteRow(ui->tableView->currentIndex().row());
}


void MainWindow::on_selectButton_clicked(void)
{
    hrd.setItem(this->ui->interfaceComboBox->currentText());

    QList<RegisterFlag> temp = hrd.getRegisterSet(0, 0);

    if (temp.count() <= 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Not enough roots for read interface");
        msgBox.exec();
        this->ui->infoLabel->setText("Not enough roots for read interface");
        this->ui->pushButton->setEnabled(false);
        this->ui->deleteButton->setEnabled(false);
        this->ui->saveButton->setEnabled(false);
        return;
    }

    this->ui->infoLabel->setText(this->ui->interfaceComboBox->currentText() + " selected");
    this->ui->pushButton->setEnabled(true);
    this->ui->deleteButton->setEnabled(true);
    this->ui->saveButton->setEnabled(true);

    for (size_t i = values.count(); i > 0 ; i--)
    {
        model->deleteRow(i - 1);
    }

    model->populate(temp);
    is_selected = true;
}

