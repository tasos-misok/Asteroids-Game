#pragma once
#include <string>
#include <sgg/graphics.h>

enum class GameScreen
{
	MENU,
	HELP,
	PLAY
};

class GameState
{
private:
	graphics::Brush m_brush_menu;
	graphics::Brush m_brush_help;
	std::string m_asset_path = "assets\\";  // path to my assets folder
	float m_canvas_width = 16.0f;
	float m_canvas_height = 16.0f;
	int m_score = 0;
	bool m_game_over = false;
	GameScreen m_current_screen = GameScreen::MENU;

	static GameState* m_unique_instance;

	GameState();

	class Player* m_player = 0;
	class Level* m_level = 0;

public:
	float m_global_offset_x = 0.0f;
	float m_global_offset_y = 0.0f;
	bool m_debugging = false;


	void init();
	void draw();
	void update(float dt);

	static GameState* get_instance();
	~GameState();

	float getCanvasWidth() { return m_canvas_width; }
	float getCanvasHeight() { return m_canvas_height; }
	void addScore(int score) { m_score += score; }
	int getScore() { return m_score; }
	bool isGameOver() { return m_game_over; }
	void setGameOver(bool over) { m_game_over = over; }
	GameScreen getCurrentScreen() { return m_current_screen; }
	void setCurrentScreen(GameScreen screen) { m_current_screen = screen; }

	std::string getAssetDir();
	std::string getFullAssetPath(const std::string& asset);

	class Player* getPlayer() { return m_player; }
	class Level* getLevel() { return m_level; }
};