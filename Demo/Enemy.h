#pragma once
#include "GameObject.h"
#include "Box.h"
#include "graphics.h"
#include <string>
#include <vector>

class Enemy : public GameObject, public Collidable, public Box
{

    graphics::Brush m_brush_enemy;
    float enemy_pos_x,enemy_pos_y;
    float size;
    float speed;
    float animationTimer = 0.0f;
    bool active = true;

    std::vector<std::string> m_enemy_animation;

public:
    Enemy();
    ~Enemy();
    void update(float dt) override;
    void draw() override;
    void init() override;

    bool isActive() { return active; }

    Box getCollisionHull() const override;

protected:
    void debugDraw();
};
