#include "qlazerdrivebonus.h"

QLazerDriveBonus::QLazerDriveBonus(const uint &id, const uint &x, const uint &y, const Type &type)
{
    m_id = id;
    m_x = x;
    m_y = y;
    m_type = type;
}

uint QLazerDriveBonus::id() const
{
    return m_id;
}

void QLazerDriveBonus::setId(const uint &id)
{
    m_id = id;
}

uint QLazerDriveBonus::x() const
{
    return m_x;
}

void QLazerDriveBonus::setX(const uint &x)
{
    m_x = x;
}

uint QLazerDriveBonus::y() const
{
    return m_y;
}

void QLazerDriveBonus::setY(const uint &y)
{
    m_y = y;
}

QLazerDriveBonus::Type QLazerDriveBonus::type() const
{
    return m_type;
}

void QLazerDriveBonus::setType(const Type &type)
{
    m_type = type;
}
