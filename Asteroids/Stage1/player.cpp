#include "player.h"
#include <sgg/graphics.h>
#include "gamestate.h"
#include "util.h"
#include "bullet.h"
#include "level.h"

void Player::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;

	float move_x = 0.0f;
	float move_y = 0.0f;


	//X axis movement
	if (graphics::getKeyState(graphics::SCANCODE_A))   //player controls
		move_x -= 1.0f;
	if (graphics::getKeyState(graphics::SCANCODE_D))
		move_x += 1.0f;

	m_vx = std::min(m_max_velocity, m_vx + delta_time * move_x * m_acceleration); //acceleration
	m_vx = std::max(-m_max_velocity, m_vx); //max velocity
	m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx)); //friction
	if (fabs(m_vx) < 0.01f)
		m_vx = 0.0f;
	m_pos_x += delta_time * m_vx; //update the position

	//Y axis movement
	if ((graphics::getKeyState(graphics::SCANCODE_W)))
		move_y -= 1.0f;
	if (graphics::getKeyState(graphics::SCANCODE_S))
		move_y += 1.0f;

	m_vy = std::min(m_max_velocity, m_vy + delta_time * move_y * m_acceleration);
	m_vy = std::max(-m_max_velocity, m_vy);
	m_vy -= 0.2f * m_vy / (0.1f + fabs(m_vy));
	if (fabs(m_vy) < 0.01f)
		m_vy = 0.0f;
	m_pos_y += delta_time * m_vy;
}

void Player::fireBullet()
{
	graphics::MouseState ms;
	graphics::getMouseState(ms);

	if (ms.button_left_pressed)
	{
		graphics::playMusic(m_state->getFullAssetPath("laser.mp3"), 0.5f, false);

		
		float mouse_canvas_x = graphics::windowToCanvasX(ms.cur_pos_x);
		float mouse_canvas_y = graphics::windowToCanvasY(ms.cur_pos_y);

		float mouse_world_x = mouse_canvas_x - m_state->m_global_offset_x;
		float mouse_world_y = mouse_canvas_y - m_state->m_global_offset_y;

		Bullet* bullet = new Bullet("Bullet");
		bullet->init();

		bullet->setStartAndTarget(m_pos_x,m_pos_y,mouse_world_x,mouse_world_y);

		if (m_state && m_state->getLevel())
		{
			m_state->getLevel()->getDynamicObjects().push_back(bullet);
		}
	}
}

void Player::update(float dt)
{
	if (m_is_exploding)
	{
		m_explode_timer += dt;
		if (m_explode_timer > m_explode_duration)
		{
			setActive(false);
			m_state->setGameOver(true);
		}
		return;
	}
	movePlayer(dt);
	fireBullet();

	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x; //update the centering of the player
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	GameObject::update(dt);
}

void Player::draw()
{

	float screen_x = m_pos_x + m_state->m_global_offset_x; //update the position of the player
	float screen_y = m_pos_y + m_state->m_global_offset_y;
	


	graphics::drawRect(screen_x,screen_y, 1.0f , 1.0f, m_brush_player); //draw the player

	if (m_state->m_debugging)
		debugDraw();
}

void Player::init()
{
	m_pos_x = 0.0f; //starting position
	m_pos_y = 0.0f;
	m_width /= 1.1f; 
	m_height /= 2.0f;

	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x; //center the player
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	m_brush_player.outline_opacity = 0.0f;
	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.texture = m_state->getFullAssetPath("spaceship.png");

	graphics::setFont(m_state->getFullAssetPath("OpenSans-Bold.ttf"));

}

void Player::lifeCounter()
{
	m_lives--;
	printf("Player got hit, lives left: %d\n", m_lives);

	if (m_lives == 0 && !m_is_exploding)
	{
		graphics::playMusic(m_state->getFullAssetPath("Death.mp3"), 0.3f, false);
		m_is_exploding = true;
		m_explode_timer = 0.0f;
		m_brush_player.texture = m_state->getFullAssetPath("explosion.png");
		
		printf("Player is dead\n");
	}
}

void Player::debugDraw()
{
	graphics::Brush debug_brush;
	debug_brush.outline_opacity = 1.0f;
	debug_brush.fill_opacity = 0.1f;
	SETCOLOR(debug_brush.fill_color, 1.0f, 0.3f, 0.0f);
	SETCOLOR(debug_brush.outline_color, 1.0f, 0.1f, 0.0f);

	float screen_x = m_pos_x + m_state->m_global_offset_x;
	float screen_y = m_pos_y + m_state->m_global_offset_y;

	graphics::drawRect(screen_x,screen_y,m_width,m_height,debug_brush);

	char s[20];
	sprintf_s(s, "(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 1.0f, 0.0f, 0.0f);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(screen_x - 0.4f, screen_y - 0.6f, 0.15f, s, debug_brush);
}
