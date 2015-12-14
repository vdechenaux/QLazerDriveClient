#ifndef QLAZERDRIVEPACKET_H
#define QLAZERDRIVEPACKET_H

#include "qlazerdriveclient_global.h"
#include <QDataStream>
#include <QWebSocket>
#include <QBuffer>

class QLAZERDRIVECLIENTSHARED_EXPORT QLazerDrivePacket : public QDataStream
{
    QByteArray *m_pPacketData;
    QBuffer *m_pBuffer;
    bool m_hasSizeField;
public:
    QLazerDrivePacket(const QByteArray &packet = QByteArray());
    ~QLazerDrivePacket();
    qint64 sendTo(QWebSocket *socket);
    void clear();
    QString readString();
    void writeString(QString str);

    enum Opcode
    {
        ReceiveHello            = 19,
        SendHello               = 20
    };
};

#endif // QLAZERDRIVEPACKET_H
