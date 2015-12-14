#include "qlazerdriveplayer.h"

QLazerDrivePlayer::QLazerDrivePlayer(const uint &id, const QString &name, const uint &r, const uint &g, const uint &b, const uint &score)
{
    m_id = id;
    m_name = name;
    m_r = r;
    m_g = g;
    m_b = b;
    m_score = score;
}

uint QLazerDrivePlayer::r() const
{
    return m_r;
}

void QLazerDrivePlayer::setR(const uint &r)
{
    m_r = r;
}

uint QLazerDrivePlayer::g() const
{
    return m_g;
}

void QLazerDrivePlayer::setG(const uint &g)
{
    m_g = g;
}

uint QLazerDrivePlayer::b() const
{
    return m_b;
}

void QLazerDrivePlayer::setB(const uint &b)
{
    m_b = b;
}
