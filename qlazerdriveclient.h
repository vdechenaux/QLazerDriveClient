#ifndef QLAZERDRIVECLIENT_H
#define QLAZERDRIVECLIENT_H

#include "qlazerdriveclient_global.h"
#include <QObject>
#include <QWebSocket>
#include <QtMath>
#include "qlazerdrivepacket.h"
#include "qlazerdriveplayer.h"

class QLAZERDRIVECLIENTSHARED_EXPORT QLazerDriveClient : public QObject
{
    Q_OBJECT
    QWebSocket *m_pSocket;
    QString m_desiredUsername, m_assignedUsername;
    void handlePacket(QLazerDrivePacket &packet);
    qreal decodeAngle(uint angle);
public:
    explicit QLazerDriveClient(QObject *parent = 0);
    ~QLazerDriveClient();
    void connectToServer(const QString &host, const QString &username = QString());

signals:
    void connected(QLazerDrivePlayer player);
    void leaderBoardRankChanged(uint playerId, uint rank);
    void leaderBoardScoreChanged(uint playerId, uint score);
    void leaderBoardLineReceived(QLazerDrivePlayer player, uint rank);
    void playerMoved(uint playerId, uint x, uint y, qreal angle);
    void playerDead(uint playerId, uint killerId, QLazerDrivePlayer::DeathTypes type, uint x, uint y, uint angle);
    void playerEnteredTheGame(QLazerDrivePlayer player, bool isMyself, bool isAlias);
    void playerLeftTheGame(QLazerDrivePlayer player, bool isAlias);
    void existingPlayerInitialized(QLazerDrivePlayer player, uint x, uint y);
    void playerCoordsInitialized(uint playerId, uint x, uint y, qreal angle);
    void playerPressedKey(uint playerId, QLazerDrivePlayer::KeyFlags key);
    void playerPrintChanged(uint playerId, bool isPrinting);
    void playerImuneChanged(uint playerId, bool isImune);
    void playerReversed(uint playerId, bool isReversed);
    void mapErased();
    void playerSpeedChanged(uint playerId, uint speed);
    void playerThicknessChanged(uint playerId, uint thickness);
private slots:
    void socketConnected();
    void socketBinaryMessageReceived(const QByteArray &packetData);
};

#endif // QLAZERDRIVECLIENT_H
