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
            QString name = packet.readString();

            QLazerDrivePlayer player(playerId, name, r, g, b);
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
            quint16 playerId, score, rank, tmp;
            packet >> playerId;
            packet >> score >> rank;
            packet >> tmp >> tmp >> tmp; // 3 x 2 bytes unknown, maybe R G B
            QString name = packet.readString();

            emit leaderBoardLineReceived(playerId, score, rank, name);
            break;
        }
        case QLazerDrivePacket::ReceivePlayerMovement: {
            quint16 playerId, x, y, angle;
            packet >> playerId >> x >> y >> angle;
            qreal angleDegrees = qMin(angle / (2000 * M_PI) * 360, 360.0);

            emit playerMoved(playerId, x, y, angleDegrees);
            break;
        }
        case QLazerDrivePacket::ReceivePlayerDead: {
            quint16 playerId, x, y, angle, killerId, isOldData;
            packet >> playerId >> x >> y >> angle >> killerId >> isOldData;

            if (killerId) {
                if (killerId == playerId) {
                    emit playerDead(playerId, killerId, QLazerDrivePlayer::Suicide, x, y, angle);
                } else if (isOldData) {
                    emit playerDead(playerId, killerId, QLazerDrivePlayer::Crash, x, y, angle);
                } else {
                    emit playerDead(playerId, killerId, QLazerDrivePlayer::Kill, x, y, angle);
                }
            } else {
                emit playerDead(playerId, killerId, QLazerDrivePlayer::Wall, x, y, angle);
            }

            break;
        }
    }
}
