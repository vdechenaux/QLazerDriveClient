#include "qlazerdrivepacket.h"

QLazerDrivePacket::QLazerDrivePacket(const QByteArray &packet)
{
    setByteOrder(QLazerDrivePacket::LittleEndian);

    m_pPacketData = new QByteArray(packet);
    m_pBuffer = new QBuffer(m_pPacketData);
    m_pBuffer->open(QIODevice::ReadWrite);
    setDevice(m_pBuffer);
    m_hasSizeField = !packet.isEmpty();
}

QLazerDrivePacket::~QLazerDrivePacket()
{
    setDevice(0);
    m_pBuffer->close();
    delete m_pBuffer;
    delete m_pPacketData;
}

qint64 QLazerDrivePacket::sendTo(QWebSocket *socket)
{
    if (!m_hasSizeField) {
        quint16 size = m_pPacketData->size();
        m_pPacketData->insert(0, "  ", 2); // push 2 bytes at the front
        m_pBuffer->reset();
        *this << size; // overwrite the 2 bytes with the size
        m_hasSizeField = true;
    }

    qint64 len = socket->sendBinaryMessage(*m_pPacketData);
    socket->flush();
    return len;
}

void QLazerDrivePacket::clear()
{
    m_pPacketData->clear();
    m_pBuffer->reset();
    m_hasSizeField = false;
}

QString QLazerDrivePacket::readString()
{
    quint16 size;
    *this >> size;

    quint16 *buffer = new quint16[size];

    for (int i=0; i<size; i++) {
        *this >> buffer[i];
    }

    QString str = QString::fromUtf16(buffer, size);
    delete[] buffer;
    return str;
}

void QLazerDrivePacket::writeString(QString str)
{
    *this << (quint16)str.length();

    const quint16 *tmp = str.utf16();
    for (int i=0, j=str.length(); i<j; i++)
        *this << tmp[i];
}
