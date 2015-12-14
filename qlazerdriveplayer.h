#ifndef QLAZERDRIVEPLAYER_H
#define QLAZERDRIVEPLAYER_H

#include "qlazerdriveclient_global.h"
#include <QString>

class QLAZERDRIVECLIENTSHARED_EXPORT QLazerDrivePlayer
{
    uint m_id;
    uint m_score;
    QString m_name;
    uint m_r, m_g, m_b;
public:
    QLazerDrivePlayer(const uint &id = 0, const QString &name = QString(), const uint &r = 0, const uint &g = 0, const uint &b = 0, const uint &score = 0);
    uint id() const;
    void setId(const uint &id);
    uint score() const;
    void setScore(const uint &score);
    QString name() const;
    void setName(const QString &name);
    uint r() const;
    void setR(const uint &r);
    uint g() const;
    void setG(const uint &g);
    uint b() const;
    void setB(const uint &b);
};

#endif // QLAZERDRIVEPLAYER_H
