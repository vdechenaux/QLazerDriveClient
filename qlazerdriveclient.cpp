#include "qlazerdriveclient.h"


QLazerDriveClient::QLazerDriveClient(QObject *parent) : QObject(parent)
{
    m_pSocket = new QWebSocket();
    connect(m_pSocket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(m_pSocket, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(socketBinaryMessageReceived(QByteArray)));
}

QLazerDriveClient::~QLazerDriveClient()
{
    m_pSocket->close();
    delete m_pSocket;
}

void QLazerDriveClient::connectToServer(const QString &host, const QString &username)
{
    QUrl url;
    url.setScheme("ws");
    url.setHost(host);

    m_desiredUsername = username;

    m_pSocket->open(url);
}

void QLazerDriveClient::socketConnected()
{
    QLazerDrivePacket packet;

    // Username packet
    packet << (quint16)QLazerDrivePacket::SendHello;
    packet << (quint16)0x00;
    packet << (quint16)0x00; // r
    packet << (quint16)0x00; // g
    packet << (quint16)0x00; // b
    packet.writeString(m_desiredUsername);
    packet.sendTo(m_pSocket);
}

void QLazerDriveClient::socketBinaryMessageReceived(const QByteArray & packetData)
{
    QLazerDrivePacket packet(packetData);

    quint16 size;
    while (!packet.atEnd()) {
        packet >> size;

        QByteArray subPacketData = packetData.mid(packet.device()->pos(), size);
        packet.skipRawData(size);
        QLazerDrivePacket subPacket(subPacketData);

        handlePacket(subPacket);
    }
}

void QLazerDriveClient::handlePacket(QLazerDrivePacket &packet)
{
    quint16 opcode;
    packet >> opcode;

    switch (opcode) {
        case QLazerDrivePacket::ReceiveHello: {
            quint16 playerId, r, g, b;
            packet >> playerId >> r >> g >> b;
            m_assignedUsername = packet.readString();

            QLazerDrivePlayer player(playerId, m_assignedUsername, r, g, b);
            emit connected(player);

            break;
        }
        case QLazerDrivePacket::ReceiveLeaderBoardRank: {
            quint16 playerId, rank;
            packet >> playerId >> rank;

            emit leaderBoardRankChanged(playerId, rank);
            break;
        }
        case QLazerDrivePacket::ReceiveLeaderBoardScore: {
            quint16 playerId, score;
            packet >> playerId >> score;

            emit leaderBoardScoreChanged(playerId, score);
            break;
        }
        case QLazerDrivePacket::ReceiveLeaderBoardInit: {
            quint16 playerId, score, rank, r, g, b;
            packet >> playerId;
            packet >> score >> rank;
            packet >> r >> g >> b;
            QString name = packet.readString();
            QLazerDrivePlayer player(playerId, name, r, g, b, score);

            emit leaderBoardLineReceived(player, rank);
            break;
        }
        case QLazerDrivePacket::ReceivePlayerMovement: {
            quint16 playerId, x, y, angle;
            packet >> playerId >> x >> y >> angle;

            emit playerMoved(playerId, x, y, decodeAngle(angle));
            break;
        }
        case QLazerDrivePacket::ReceivePlayerDead: {
            quint16 playerId, x, y, angle, killerId, isOldData;
            packet >> playerId >> x >> y >> angle >> killerId >> isOldData;

            if (killerId) {
                if (killerId == playerId) {
                    emit playerDead(playerId, killerId, QLazerDrivePlayer::Suicide, x, y, decodeAngle(angle));
                } else if (isOldData) {
                    emit playerDead(playerId, killerId, QLazerDrivePlayer::Crash, x, y, decodeAngle(angle));
                } else {
                    emit playerDead(playerId, killerId, QLazerDrivePlayer::Kill, x, y, decodeAngle(angle));
                }
            } else {
                emit playerDead(playerId, killerId, QLazerDrivePlayer::Wall, x, y, decodeAngle(angle));
            }

            break;
        }
        case QLazerDrivePacket::ReceiveMyselfEnterTheGame:
        case QLazerDrivePacket::ReceiveOtherEnterTheGame:
        case QLazerDrivePacket::ReceiveAliasEnterTheGame: {
            quint16 playerId, r, g, b;
            packet >> playerId >> r >> g >> b;
            QString name = packet.readString();

            QLazerDrivePlayer player(playerId, name, r, g, b);
            bool isMyself = name == m_assignedUsername;
            bool isAlias = opcode == QLazerDrivePacket::ReceiveAliasEnterTheGame;
            emit playerEnteredTheGame(player, isMyself, isAlias);

            break;
        }
        case QLazerDrivePacket::ReceivePlayerLeftTheGame:
        case QLazerDrivePacket::ReceiveAliasLeftTheGame: {
            quint16 playerId;
            packet >> playerId;
            QString name = packet.readString();

            QLazerDrivePlayer player(playerId, name);
            bool isAlias = opcode == QLazerDrivePacket::ReceiveAliasLeftTheGame;
            emit playerLeftTheGame(player, isAlias);

            break;
        }
        case QLazerDrivePacket::ReceivePlayerInitExisting: {
            // Sent only when the client (me) is entering the room, not while playing
            // lastX and lastY are the last known coordinates of the player (If dead => death coords, If alive => current coords)
            // Equivalent to ReceiveOtherEnterTheGame + ReceivePlayerInitCoords
            quint16 playerId, r, g, b, lastX, lastY, tmp;
            packet >> playerId >> lastX >> lastY >> r >> g >> b;
            packet >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp; // unknown
            QString name = packet.readString();

            QLazerDrivePlayer player(playerId, name, r, g, b);
            emit existingPlayerInitialized(player, lastX, lastY);
            break;
        }
        case QLazerDrivePacket::ReceivePlayerInitCoords: {
            // Not sent if ReceivePlayerInit was received for this playerId
            // But if he dies and comes back, this packet will be sent, but not ReceivePlayerInit
            quint16 playerId, x, y, angle;
            packet >> playerId >> x >> y >> angle;

            emit playerCoordsInitialized(playerId, x, y, decodeAngle(angle));
            break;
        }
        case QLazerDrivePacket::ReceivePLayerPressKey: {
            quint16 playerId, keyFlag;
            packet >> playerId >> keyFlag;
            emit playerPressedKey(playerId, (QLazerDrivePlayer::KeyFlags)keyFlag);
            break;
        }
        case QLazerDrivePacket::ReceivePlayerPrintChanged: {
            quint16 playerId, isPrinting;
            packet >> playerId >> isPrinting;
            emit playerPrintChanged(playerId, isPrinting);
            break;
        }
        case QLazerDrivePacket::ReceivePlayerImuneChanged: {
            quint16 playerId, isImune;
            packet >> playerId >> isImune;
            emit playerImuneChanged(playerId, isImune);
            break;
        }
        case QLazerDrivePacket::ReceivePlayerReversed: {
            quint16 playerId, isReversed;
            packet >> playerId >> isReversed;
            emit playerReversed(playerId, isReversed);
            break;
        }
        case QLazerDrivePacket::ReceiveEraser: {
            emit mapErased();
        }
        case QLazerDrivePacket::ReceivePlayerSpeed: {
            quint16 playerId, speed;
            packet >> playerId >> speed;
            emit playerSpeedChanged(playerId, speed);
            break;
        }
        case QLazerDrivePacket::ReceivePlayerThickness: {
            quint16 playerId, thickness;
            packet >> playerId >> thickness;
            emit playerThicknessChanged(playerId, thickness);
            break;
        }
        case QLazerDrivePacket::ReceivePlayerTookBonus: {
            quint16 bonusId;
            packet >> bonusId;
            emit playerTookBonus(bonusId);
            break;
        }
        case QLazerDrivePacket::ReceiveBonusAppeared: {
        quint16 bonusId, type, x, y;
            packet >> bonusId >> type >> x >> y;
            emit bonusAppeared(QLazerDriveBonus(bonusId, x, y, (QLazerDriveBonus::Type)type));
            break;
        }
        case QLazerDrivePacket::ReceivePing: {
            quint16 latency;
            packet >> latency;
            emit ping(latency);
            break;
        }
        case QLazerDrivePacket::ReceivePlayerTrace: {
            quint16 x, y, angle, thickness, r, g, b, playerId;
            packet >> x >> y >> angle >> thickness >> r >> g >> b >> playerId;
            emit playerTraceInitialized(playerId, x, y, decodeAngle(angle*10), thickness, r, g, b);
            break;
        }
    }
}

qreal QLazerDriveClient::decodeAngle(uint angle)
{
    return qMin(angle / (2000 * M_PI) * 360, 360.0);
}
