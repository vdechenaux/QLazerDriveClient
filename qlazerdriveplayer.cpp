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

uint QLazerDrivePlayer::id() const
{
    return m_id;
}

void QLazerDrivePlayer::setId(const uint &id)
{
    m_id = id;
}

uint QLazerDrivePlayer::score() const
{
    return m_score;
}

void QLazerDrivePlayer::setScore(const uint &score)
{
    m_score = score;
}

QString QLazerDrivePlayer::name() const
{
    return m_name;
}

void QLazerDrivePlayer::setName(const QString &name)
{
    m_name = name;
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
