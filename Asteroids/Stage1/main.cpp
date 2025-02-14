#include "sgg/graphics.h"
#include "stdio.h"
#include <string>
#include "util.h"
#include "gamestate.h"



void draw()
{
	GameState::get_instance()->draw();
}

void update(float dt)
{
	GameState::get_instance()->update(dt);
}


void init() 
{
	GameState::get_instance()->init();
}





int main(int argc, char ** argv)
{
	graphics::createWindow(800, 800, "Asteroids");
	init();

	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);

	graphics::setCanvasSize(GameState::get_instance()->getCanvasWidth(),
		GameState::get_instance()->getCanvasHeight());
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

	graphics::startMessageLoop();
	return 0;
}