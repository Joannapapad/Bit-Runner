#include "graphics.h"
#include <string>
#include "util.h"
#include "GameState.h"


void init()
{
    GameState::getInstance()->init();

}


void update(float dt) {
    GameState::getInstance()->update(dt);

}

void draw()
{
    GameState::getInstance()->draw();

}


int main(int argc, char** argv)
{   graphics::createWindow(800, 800, "Bit Runner");
    
    GameState::getInstance()->init();

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    graphics::setCanvasSize(GameState::getInstance()-> getCanvasWidth(), GameState::getInstance()->getCanvasHeight());

    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    graphics::startMessageLoop();

    return 0;
}
