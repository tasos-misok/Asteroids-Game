#include "bullet.h"
#include "gamestate.h"
#include "asteroid.h" 
#include "level.h"
#include <sgg/graphics.h>
#include <cmath>
#include <cstdio>
#include "util.h"

void Bullet::init()
{
    m_width = 0.2f;
    m_height = 0.2f;
    m_brush_bullet.outline_opacity = 0.0f;
    m_brush_bullet.fill_opacity = 1.0f;
    m_brush_bullet.texture = m_state->getFullAssetPath("bullet.png");
    SETCOLOR(m_brush_bullet.fill_color, 1.0f, 1.0f, 0.0f);

}

void Bullet::setStartAndTarget(float start_x, float start_y, float target_x, float target_y)
{
    m_pos_x = start_x;
    m_pos_y = start_y;

    float dx = target_x - start_x;
    float dy = target_y - start_y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist > 0.001f)
    {
        m_dir_x = dx / dist;
        m_dir_y = dy / dist;
    }
    else
    {
       
        m_dir_x = 1.0f;
        m_dir_y = 0.0f;
    }
}

void Bullet::update(float dt)
{
    float delta_time = dt / 1000.0f; 
  
    m_pos_x += m_dir_x * m_speed * delta_time;
    m_pos_y += m_dir_y * m_speed * delta_time;

    float w = m_state->getCanvasWidth();
    float h = m_state->getCanvasHeight();
    if (m_pos_x < -500.0f || m_pos_x >(w + 500.0f) ||
        m_pos_y < -500.0f || m_pos_y >(h + 500.0f))
    {
        setActive(false);
        return;
    }


    auto* level = m_state->getLevel();
    if(level){
        auto& objects = level->getDynamicObjects();
        for (auto pObj : objects)
        {
            if (!pObj || !pObj->isActive())
                continue;

            Asteroid* ast = dynamic_cast<Asteroid*>(pObj);
            if (ast)
            {
                    if (this->intersect(*ast))
                    {
						ast->asteroidShot();
                        this->setActive(false);
                        break;
                    }
            }
        }
    
	}
    GameObject::update(dt);
}

void Bullet::draw()
{
    float screen_x = m_pos_x + m_state->m_global_offset_x;
    float screen_y = m_pos_y + m_state->m_global_offset_y;

    graphics::drawRect(screen_x, screen_y, m_width * 4, m_height * 4, m_brush_bullet);

    if (m_state->m_debugging)
        debugDraw();
}

void Bullet::debugDraw()
{
    graphics::Brush debug_brush;
    debug_brush.outline_opacity = 1.0f;
    debug_brush.fill_opacity = 0.1f;
    SETCOLOR(debug_brush.fill_color, 0.0f, 1.0f, 0.0f);
    SETCOLOR(debug_brush.outline_color, 0.0f, 1.0f, 0.0f);

    float screen_x = m_pos_x + m_state->m_global_offset_x;
    float screen_y = m_pos_y + m_state->m_global_offset_y;
    graphics::drawRect(screen_x, screen_y, m_width, m_height, debug_brush);

    char s[60];
    sprintf_s(s, "B(%.2f, %.2f)", m_pos_x, m_pos_y);
    debug_brush.fill_opacity = 1.0f;
    graphics::drawText(screen_x - 0.2f, screen_y - 0.6f, 0.15f, s, debug_brush);
}
