#ifndef QLAZERDRIVECLIENT_H
#define QLAZERDRIVECLIENT_H

#include "qlazerdriveclient_global.h"
#include <QObject>
#include <QWebSocket>
#include "qlazerdrivepacket.h"
#include "qlazerdriveplayer.h"

class QLAZERDRIVECLIENTSHARED_EXPORT QLazerDriveClient : public QObject
{
    Q_OBJECT
    QWebSocket *m_pSocket;
    QString m_desiredUsername;
    void handlePacket(QLazerDrivePacket &packet);
public:
    explicit QLazerDriveClient(QObject *parent = 0);
    ~QLazerDriveClient();
    void connectToServer(const QString &host, const QString &username = QString());

signals:
    void connected(QLazerDrivePlayer player);
private slots:
    void socketConnected();
    void socketBinaryMessageReceived(const QByteArray &packetData);
};

#endif // QLAZERDRIVECLIENT_H
