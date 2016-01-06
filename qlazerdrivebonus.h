#ifndef QLAZERDRIVEBONUS_H
#define QLAZERDRIVEBONUS_H

#include "qlazerdriveclient_global.h"

class QLAZERDRIVECLIENTSHARED_EXPORT QLazerDriveBonus
{
public:
    enum Type {
        UNDEFINED = -1,
        RED_THICKNESS,
        RED_HIGH_SPEED,
        RED_REVERSE,
        RED_LOW_SPEED,
        ERASER,
        GREEN_HIGH_SPEED,
        GREEN_IMUNE,
        GREEN_LOW_SPEED,
        GREEN_THICKNESS
    };
private:
    uint m_id, m_x, m_y;
    Type m_type;
public:
    QLazerDriveBonus(const uint &id = 0, const uint &x = 0, const uint &y = 0, const Type &type = UNDEFINED);

    uint id() const;
    void setId(const uint &id);
    uint x() const;
    void setX(const uint &x);
    uint y() const;
    void setY(const uint &y);
    Type type() const;
    void setType(const Type &type);
};

#endif // QLAZERDRIVEBONUS_H
