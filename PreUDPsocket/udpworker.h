#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );
    void HomeSendDatagram(QString data);


private slots:
    void readPendingDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;
    QUdpSocket* sendSocket;
    QHostAddress Addres = QHostAddress("127.0.0.1 ");
    qint16 Port = 5432;

signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_readyData(qint16 Data, const QHostAddress Addres);

};

#endif // UDPWORKER_H
