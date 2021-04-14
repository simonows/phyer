#ifndef __MAINWIN_H__
#define __MAINWIN_H__

#include <registerflag.h>
#include <qtableviewmodel.h>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow: public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    QList<RegisterFlag> *values;
    QTableViewModel *model;
    int newidx;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();

};
#endif // __MAINWIN_H__

