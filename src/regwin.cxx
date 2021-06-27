#include "regwin.h"
#include "../ui_regwindow.h"

RegWindow::RegWindow(QWidget* parent) : QDialog(parent), ui(new Ui::RegWindow)
{
    ui->setupUi(this);
    this->ui->addrValue->setPlaceholderText(" 0-31");
    this->ui->nameValue->setPlaceholderText(" Control register ...");
    this->ui->descValue->setPlaceholderText(" This is register witch ...");
}

void RegWindow::on_ok_clicked(void)
{
    accept();
}

void RegWindow::on_cancel_clicked(void)
{
    reject();
}

RegWindow::~RegWindow()
{
    delete ui;
}

unsigned long RegWindow::getAddr() const
{
    return this->ui->addrValue->text().toULong(nullptr, 10);
}

QString RegWindow::getDesc() const
{
    return this->ui->descValue->toPlainText();
}

QString RegWindow::getName() const
{
    return this->ui->nameValue->text();
}

QString RegWindow::getMode() const
{
    return this->ui->modeValue->currentText();
}

