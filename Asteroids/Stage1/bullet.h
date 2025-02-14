#pragma once
#include "gameobject.h"
#include "box.h"
#include <sgg/graphics.h>

class Bullet : public GameObject, public Box
{
    graphics::Brush m_brush_bullet;

    float m_speed = 20.0f;
    float m_dir_x = 0.0f;
    float m_dir_y = 0.0f;

public:
    Bullet(std::string name) : GameObject(name) {}

    void update(float dt) override;
    void draw() override;
    void init() override;

    void setStartAndTarget(float start_x, float start_y, float target_x, float target_y);

protected:
    void debugDraw();
};
