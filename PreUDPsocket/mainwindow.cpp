#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);

    });
    connect(udpWorker, &UDPworker::sig_readyData, this, [&](qint16 sendData, const QHostAddress Addres){
        ui->te_result->append("Принято сообщение от " + Addres.toString() + ", размер сообщения(байт) " + QString::number(sendData));
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}


void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));


}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}


void MainWindow::on_pb_send_clicked()
{
    udpWorker->HomeSendDatagram(ui->te_send->toPlainText());
}

