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
        SendHello                   = 20,

        ReceivePlayerInitExisting   = 1,
        ReceivePlayerInitCoords     = 2,
        ReceiveAliasLeftTheGame     = 6,
        ReceivePlayerTookBonus      = 8,
        ReceivePlayerLeftTheGame    = 9,
        ReceiveEraser               = 15,
        ReceivePLayerPressKey       = 18,
        ReceiveHello                = 19,
        ReceivePlayerMovement       = 21,
        ReceiveLeaderBoardInit      = 25,
        ReceiveLeaderBoardScore     = 26,
        ReceiveLeaderBoardRank      = 27,
        ReceiveAliasEnterTheGame    = 28,
        ReceivePlayerSpeed          = 30,
        ReceivePlayerThickness      = 31,
        ReceivePlayerDead           = 32,
        ReceivePlayerPrintChanged   = 35,
        ReceivePlayerImuneChanged   = 36,
        ReceivePlayerReversed       = 37,
        ReceiveBonusAppeared        = 39,
        ReceiveOtherEnterTheGame    = 40,
        ReceiveMyselfEnterTheGame   = 41,
    };
};

#endif // QLAZERDRIVEPACKET_H
