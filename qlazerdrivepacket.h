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
        SendHello               = 20,

        ReceiveHello            = 19,
        ReceiveLeaderBoardInit  = 25,
        ReceiveLeaderBoardScore = 26,
        ReceiveLeaderBoardRank  = 27
    };
};

#endif // QLAZERDRIVEPACKET_H