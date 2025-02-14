#include "asteroid.h"
#include "player.h"
#include "gamestate.h"
#include "util.h"
#include <sgg/graphics.h>
#include <cmath>

void Asteroid::asteroidExplosion()
{
}

bool Asteroid::checkCollision()
{
	const Player* player = m_state->getPlayer();
	if (!player)
	{
		printf("Player not found\n");
		return false;
	}

	Player tempPlayer = *player;
	bool isColliding = this->intersect(tempPlayer);
	if (isColliding)
	{
		printf("Collision Detected: Asteroid at (%.2f, %.2f), Player at (%.2f, %.2f)\n",
			m_pos_x, m_pos_y, player->m_pos_x, player->m_pos_y);
	}
	return isColliding;
}

void Asteroid::asteroidShot()
{
	ast_hp--;
	printf("Asteroid took  damage, remaining HP=%d\n", ast_hp);

	if (ast_hp <= 0)
	{
		graphics::playMusic(m_state->getFullAssetPath("asteroidBoom.mp3"), 0.5f, false);
		ast_brush_asteroid.texture = m_state->getFullAssetPath("asteroidSmoke.png");
		ast_exploding = true;

		if (m_state)
		{
			switch (ast_size)
			{
			case AsteroidSize::SMALL:
				m_state->addScore(20);
				break;
			case AsteroidSize::MEDIUM:
				m_state->addScore(50);
				break;
			case AsteroidSize::LARGE:
				m_state->addScore(100);
				break;
			}
		}
		printf("Asteroid destroyed\n");
	}
}

void Asteroid::update(float dt)
{
	float delta_time = dt / 1000.0f;
	m_pos_x += ast_vx * delta_time;
	m_pos_y += ast_vy * delta_time;

	if (ast_exploding)
	{
		ast_explode_timer += dt;
		if (ast_explode_timer > ast_explode_duration)
		{
			setActive(false);
		}
		return;
	}

	if (checkCollision())
	{
		graphics::playMusic(m_state->getFullAssetPath("Hit.mp3"), 0.5f, false);
		m_state->getPlayer()->lifeCounter();
		/*printf("Hit by asteroid\n"); */// debug message to see if player got hit
		setActive(false);
	}

	if (m_pos_x < -500.0f || m_pos_x > m_state->getCanvasWidth() + 500.0f ||
		m_pos_y < -500.0f || m_pos_y > m_state->getCanvasHeight() + 500.0f)
	{
		printf("Asteroid out of bounds\n");
		setActive(false);
	}

	GameObject::update(dt);



}

void Asteroid::draw()
{
	float screen_x = m_pos_x + m_state->m_global_offset_x;
	float screen_y = m_pos_y + m_state->m_global_offset_y;

	float scaleFactor = 1.0f;
	if(ast_size == AsteroidSize::LARGE)
		scaleFactor = 3.0f;
	else if (ast_size == AsteroidSize::MEDIUM)
		scaleFactor = 2.0f;

	ast_brush_asteroid.outline_opacity = 0.0f;
	ast_brush_asteroid.fill_opacity = 1.0f;
	graphics::drawRect(screen_x, screen_y, scaleFactor , scaleFactor , ast_brush_asteroid);

	if (m_state->m_debugging)
		debugAstDraw();
}

void Asteroid::init()
{

	const Player* player = m_state->getPlayer();
	if (!player)
	{
		printf("Player not found\n");
		return;
	}


	float player_x = player->m_pos_x;
	float player_y = player->m_pos_y;

	float angle = float(rand() % 360);
	float radius = angle * 3.14159f / 180.0f;

	float canvasWidth = m_state->getCanvasWidth();
	float canvasHeight = m_state->getCanvasHeight();

	float halfDiagonal = sqrt(canvasWidth * canvasWidth + canvasHeight * canvasHeight) / 2.0f;
	float spawnRadius = halfDiagonal + 50.0f;

	m_pos_x = player_x + spawnRadius * cosf(radius);
	m_pos_y = player_y + spawnRadius * sinf(radius);

	switch (ast_size)
	{	
	case AsteroidSize::SMALL:
		m_width = 0.8f;
		m_height = 0.8f;
		ast_speed = 10.0f;
		break;
	case AsteroidSize::MEDIUM:
		m_width = 1.5f;
		m_height = 1.5f;
		ast_hp = 2;
		ast_speed = 8.0f;
		break;
	case AsteroidSize::LARGE:
		m_width = 2.2f;
		m_height = 2.2f;
		ast_hp = 4;
		ast_speed = 5.0f;
		break;
	}

	float dx = player_x - m_pos_x;
	float dy = player_y - m_pos_y;
	float distance = sqrt(dx * dx + dy * dy);
	if (distance > 0.0f) {
		ast_vx = ast_speed * (dx / distance);
		ast_vy = ast_speed * (dy / distance);
	}
	else {
		ast_vx = ast_speed;
		ast_vy = 0.0f;
	}
	
	ast_brush_asteroid.texture = m_state->getFullAssetPath("asteroid.png");
	SETCOLOR(ast_brush_asteroid.fill_color, 1.0f, 0.0f, 0.0f);
}

void Asteroid::debugAstDraw()
{
	graphics::Brush debug_ast_brush;
	debug_ast_brush.outline_opacity = 1.0f;
	debug_ast_brush.fill_opacity = 0.1f;
	SETCOLOR(debug_ast_brush.fill_color, 1.0f, 0.3f, 0.0f);
	SETCOLOR(debug_ast_brush.outline_color, 1.0f, 0.1f, 0.0f);

	float screen_x = m_pos_x + m_state->m_global_offset_x;
	float screen_y = m_pos_y + m_state->m_global_offset_y;

	graphics::drawRect(screen_x, screen_y, m_width, m_height, debug_ast_brush);

	char debug_ast_text[60];
	sprintf_s(debug_ast_text, "Pos: (%.2f, %.2f), Vel: (%.2f, %.2f)", m_pos_x, m_pos_y, ast_vx, ast_vy);
	graphics::drawText(screen_x, screen_y - 10.0f, 0.15f, debug_ast_text, debug_ast_brush);
}
