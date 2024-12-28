#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{

    sendSocket = new QUdpSocket(this);

}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);

}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();


    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendTimeToGUI(dateTime);
}
/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}

void UDPworker::HomeSendDatagram(QString data){
    QByteArray Data_to_send = data.toUtf8();
    QNetworkDatagram datagram(Data_to_send, Addres, Port);
    qint16 sendData = sendSocket->writeDatagram(datagram);
    emit sig_readyData(sendData, Addres);
}
