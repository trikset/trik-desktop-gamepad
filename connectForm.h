#ifndef CONNECTFORM_H
#define CONNECTFORM_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTcpSocket>

namespace Ui {
class ConnectForm;
}

class ConnectForm : public QDialog
{
    Q_OBJECT

public:
    ConnectForm(QTcpSocket *socket, QWidget *parent = 0);
    ~ConnectForm();

private slots:
    void on_connectButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ConnectForm *ui;
    QDialogButtonBox *buttonBox;
    QPushButton *connectButton;
    QTcpSocket *socket_;
    QTcpSocket *socket;

signals:
    void connectionChanged(const QString&);

};

#endif // CONNECT_H
