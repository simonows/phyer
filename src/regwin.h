#ifndef __REGWIN_H__
#define __REGWIN_H__

#include <vector>

#include <QDialog>
#include <QComboBox>
#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include <phyer/qtableviewmodel.h>
#include <phyer/registerflag.h>

QT_BEGIN_NAMESPACE
namespace Ui { class RegWindow; }
QT_END_NAMESPACE


class RegWindow: public QDialog
{
    Q_OBJECT

public:
    RegWindow( QWidget* parent = 0 );
    ~RegWindow();

    unsigned long getAddr() const;
    QString getDesc() const;
    QString getName() const;
    QString getMode() const;

private:
    QLineEdit* m_edit;
    Ui::RegWindow *ui;

private slots:
    void on_ok_clicked(void);
    void on_cancel_clicked(void);
};

#endif // __REGWIN_H__

