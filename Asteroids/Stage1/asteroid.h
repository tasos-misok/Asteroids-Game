#pragma once
#include "gameobject.h"
#include <sgg/graphics.h>
#include "box.h"

enum class AsteroidSize
{
	SMALL,
	MEDIUM,
	LARGE
};

class Asteroid : public GameObject, public Box
{
	graphics::Brush ast_brush_asteroid;
	int ast_hp = 1;
	float ast_speed = 10.0f;
	float ast_vx = 0.0f;
	float ast_vy = 0.0f;
	AsteroidSize ast_size;
	bool ast_exploding = false;
	float ast_explode_timer = 0.0f;
	float ast_explode_duration = 300.0f;
	
	void asteroidExplosion();
	bool checkCollision();
	

public:


	void update(float dt) override;
	void draw() override;
	void init() override;

	Asteroid( std::string name, AsteroidSize size) : GameObject(name), ast_size(size) {}
	void asteroidShot();
protected:
	void debugAstDraw();
	
};