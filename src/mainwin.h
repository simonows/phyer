#ifndef __MAINWIN_H__
#define __MAINWIN_H__

#include <vector>

#include <QMainWindow>
#include <QComboBox>

#include <phyer/hardware.h>
#include <phyer/qtableviewmodel.h>
#include <phyer/registerflag.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow: public QMainWindow
{
    Q_OBJECT

    Hardware hrd;
    Ui::MainWindow *ui;
    QList<RegisterFlag> values;
    QTableViewModel *model;
    QTableViewModel *model_optional;
    QList<unsigned long> set_optional;
    QComboBox *interface_model;
    int newidx;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked(void);
    void on_deleteButton_clicked(void);
    void on_editButton_clicked(void);
    void on_selectButton_clicked(void);

};
#endif // __MAINWIN_H__

