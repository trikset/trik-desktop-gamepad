#include "connectForm.h"
#include "ui_connectForm.h"
#include <QGridLayout>
#include <QMessageBox>
#include <gamepadForm.h>

ConnectForm::ConnectForm(QTcpSocket *socket, QWidget *parent) :
    QDialog(parent),
    socket_(socket),
    ui(new Ui::ConnectForm)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

ConnectForm::~ConnectForm()
{
    delete ui;
}

void ConnectForm::on_connectButton_clicked()
{
    const auto ip = ui->robotIpLineEdit->text();

    // Disable "connect" button and address input field to indicate that we are trying to connect.
    // Allow form to redraw disabled buttons.
    QApplication::processEvents();

    // Connecting. 4444 is hardcoded here since it is default gamepad port on TRIK.
    socket_->connectToHost(ip, 4444);
    // Waiting for opened connection and checking that connection is actually established.
    if (!socket_->waitForConnected(3000)) {
        // If not, warn user.
        QMessageBox::warning(this, "Connection failed", "Failed to connect to robot");
    } else {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void ConnectForm::on_buttonBox_accepted()
{
    this->accept();
}

void ConnectForm::on_buttonBox_rejected()
{
    this->reject();
}
