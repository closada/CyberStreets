#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

class Enemy : public Entity
{
public:
    Enemy(float x, float y);
    void update() override;
    void takeHit();

};

#endif

