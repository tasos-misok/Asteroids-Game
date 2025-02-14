#include "level.h"
#include "gamestate.h"
#include "player.h"
#include "asteroid.h"
#include "util.h"
#include <sgg/graphics.h>

void Level::update(float dt)
{
	//update the player if active
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);
	

	//update all the dynamic objects
	for (auto p_gob = m_dynamic_objects.begin(); p_gob != m_dynamic_objects.end(); )
	{
		if (*p_gob) {
			(*p_gob)->update(dt);

			// remove inactive objects
			if (!(*p_gob)->isActive())
			{
				delete* p_gob;
				p_gob = m_dynamic_objects.erase(p_gob);
				continue;
			}
		}
		++p_gob;
	}

	static float asteroid_spawn_timer = 0.0f;
	static const int max_asteroids = 50;
	asteroid_spawn_timer += dt;


	// spawn a new asteroid every second
	if (asteroid_spawn_timer > 1000.0f /*&& m_dynamic_objects.size() < max_asteroids*/) 
	{
		AsteroidSize size = static_cast<AsteroidSize>(rand() % 3);
		Asteroid* new_asteroid = new Asteroid("Asteroid", size); 
		new_asteroid->init(); 
		m_dynamic_objects.push_back(new_asteroid);
		printf("Spawned asteroid. Total: %zu\n", m_dynamic_objects.size());
		asteroid_spawn_timer = 0.0f;
	}


	GameObject::update(dt);
}

void Level::draw()
{
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	float tileW = 16.0f;
	float tileH = 16.0f;

	int tiles_x = 3;
	int tiles_y = 3;

	float center_x = m_state->m_global_offset_x + w * 0.5f;
	float center_y = m_state->m_global_offset_y + h * 0.5f;

	// draw big background
	for (int i = -tiles_x; i <= tiles_x; i++)
	{
		for (int j = -tiles_y; j <= tiles_y; j++)
		{
			float tile_x = center_x + i * tileW;
			float tile_y = center_y + j * tileH;

			graphics::drawRect(tile_x, tile_y, tileW, tileH, m_brush_background);
		}
	}

	if(m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();

	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->draw();

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) {
			p_gob->draw(); 
		}

	// Draw the ui
	if (m_state->getPlayer()->isActive())
	{
		char score_text[64];
		sprintf_s(score_text, "Score: %d", m_state->getScore());
		float score_size = 0.5f;
		float score_x = w - 2.5f;
		float score_y = 2.0f;
		graphics::drawText(score_x, score_y, score_size, score_text, m_brush_ui);

		int lives = m_state->getPlayer()->getLives();
		char lives_text[64];
		sprintf_s(lives_text, "Lives: %d", lives);
		float lives_size = 0.5f;
		float lives_x = 0.5f;
		float lives_y = 2.0f;
		graphics::drawText(lives_x, lives_y, lives_size, lives_text, m_brush_ui);
	}

	//Draw game over
	if (m_state->isGameOver()) 
	{
		char game_over_text[64];
		sprintf_s(game_over_text, "Game Over!    Final Score: %d", m_state->getScore());
		float game_over_size = 1.0f;
		float game_over_x = m_state->getCanvasWidth() / 2.0f - 6.0f;
		float game_over_y = m_state->getCanvasHeight() / 2.0f;
		graphics::drawText(game_over_x, game_over_y, game_over_size, game_over_text, m_brush_ui);
	}

}

void Level::init()
{
	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->init();
	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->init();

}

Level::Level(const std::string& name)
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("bg8.png");

	m_brush_ui.outline_opacity = 0.0f;
	SETCOLOR(m_brush_ui.fill_color, 1.0f, 1.0f, 1.0f);
}

Level::~Level()
{
	for (auto p_gob : m_static_objects)
		if (p_gob) delete p_gob;
	m_static_objects.clear();
	for (auto p_gob : m_dynamic_objects)
		if (p_gob) delete p_gob;
	m_dynamic_objects.clear();
}
