#include "gamestate.h"
#include "level.h"
#include "player.h"
#include <thread>
#include <chrono>
#include <ctime>
#include <sgg/graphics.h>

GameState::GameState()
{
}

void GameState::init()
{
	graphics::playSound(getFullAssetPath("Density & Time - MAZE  NO COPYRIGHT 8-bit Music.mp3"), 0.03f, true);

	m_player = new Player("Player");
	m_player->init();

	m_level = new Level();
	m_level->init();

	

	graphics::preloadBitmaps(getAssetDir()); // preload all bitmaps to avoid loading them during gameplay
	graphics::setFont(getFullAssetPath("OpenSans-Bold.ttf"));
}

void GameState::draw()
{
	//Menu screen
	if (m_current_screen == GameScreen::MENU)
	{

		m_brush_menu.outline_opacity = 0.0f;
		m_brush_menu.fill_opacity = 1.0f;
		m_brush_menu.texture = getFullAssetPath("Menu.png");
		graphics::drawRect(getCanvasWidth() / 2.0f,getCanvasHeight() / 2.0f,getCanvasWidth(),getCanvasHeight(),m_brush_menu);
	}

	//Help screen
	else if (m_current_screen == GameScreen::HELP)
	{
		m_brush_menu.outline_opacity = 0.0f;
		m_brush_menu.fill_opacity = 1.0f;
		m_brush_menu.texture = getFullAssetPath("Help.png");
		graphics::drawRect(getCanvasWidth() / 2.0f, getCanvasHeight() / 2.0f, getCanvasWidth(), getCanvasHeight(), m_brush_menu);
	}
	//Gam screen
	else if (m_current_screen == GameScreen::PLAY)
	{
		if (m_level) {
			m_level->draw();
		}
	}
}

void GameState::update(float dt)
{

	if (dt > 500)
		return; // if long delay is detected skip update to avoid messing up the collisions

	float sleep_time = std::max(0.0f, 17.0f - dt); // sleep for the remaining time to make the total time 17ms
	std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));  

	if (m_game_over)
		return;

	if (m_current_screen == GameScreen::MENU) 
	{
		if (graphics::getKeyState(graphics::SCANCODE_SPACE))
			setCurrentScreen(GameScreen::PLAY);
		else if (graphics::getKeyState(graphics::SCANCODE_H))
			setCurrentScreen(GameScreen::HELP);
	}
	else if (m_current_screen == GameScreen::HELP)
	{
		if (graphics::getKeyState(graphics::SCANCODE_SPACE))
			setCurrentScreen(GameScreen::PLAY);
	}
	else if (m_current_screen == GameScreen::PLAY)
	{
		if (m_level)
			m_level->update(dt);
	}
	m_debugging = graphics::getKeyState(graphics::SCANCODE_0);
}

GameState* GameState::get_instance()
{
	if(m_unique_instance == nullptr)
		m_unique_instance = new GameState();

	return m_unique_instance;
}

GameState::~GameState()
{
	if(m_player)
		delete m_player;
	if (m_level)
		delete m_level;
}

std::string GameState::getAssetDir()
{
	return m_asset_path;
}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}

GameState* GameState::m_unique_instance = nullptr;