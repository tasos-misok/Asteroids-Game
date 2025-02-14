#pragma once
#include "gameobject.h"
#include <sgg/graphics.h>
#include <list>

class Level : public GameObject
{
	graphics::Brush m_brush_background;
	graphics::Brush m_brush_ui;

	std::vector<GameObject*> m_static_objects;
	std::list<GameObject*> m_dynamic_objects;


	float m_center_x = 5.0f;
	float m_center_y = 5.0f;



public:
	void update(float dt) override;
	void draw() override;
	void init() override;

	std::list<GameObject*>& getDynamicObjects() { return m_dynamic_objects; }

	Level(const std::string& name = "Level0");
	~Level();

};