#pragma once
#include "gameobject.h"
#include "box.h"
#include <sgg/graphics.h>

class Player : public GameObject, public Box
{
	graphics::Brush m_brush_player;

	const float m_acceleration = 40.0f;
	const float m_max_velocity = 10.0f;
	int m_lives = 3;
	bool m_is_exploding = false;
	float m_explode_timer = 0.0f;
	float m_explode_duration = 500.0f;

	void movePlayer(float dt);
	void fireBullet();

public:
	float m_vx = 0.0f;
	float m_vy = 0.0f;
	

	void update(float dt) override;
	void draw() override;
	void init() override;	

	Player(std::string name) : GameObject(name) {}
	int getLives() { return m_lives; }
	void lifeCounter();

protected:
	void debugDraw();

};