#include "Level.h"
#include "GameState.h"
#include "Player.h"
#include"vector"
#include <util.h>
#include "graphics.h"



void Level::update(float dt)
{
    if (m_state->getPlayer()->isActive()) {
        m_state->getPlayer()->update(dt);
    }

    checkBug(); 
    spawnBug(); 

    for (auto bug : bugs) {
        bug->update(dt);
    }

    CheckEnemyCollision();
    checkCollisions();

    // Check if player has reached the checkpoint
    if (!checkpointReached &&
        m_state->getPlayer()->getX() >= checkpointX && m_state->getPlayer()->getY() <= checkpointY) {
        checkpointReached = true;
        m_state->goToNextLevel(); // This assumes GameState has a method to go to the next level
    }

    GameObject::update(dt);
}

void Level::init()
{

     checkpointX = 16.0f;
     checkpointY = 4.0f;

     for (auto obj : m_staticObjects) {
        delete obj;
    }
    m_staticObjects.clear();

    // Ground floor
    for (int x = 0; x < 20; ++x) {
        m_staticObjects.push_back(new StaticObj(x, 9, 1, 1, "block4.png"));
    }
    m_staticObjects.push_back(new StaticObj(18, 6, 1, 1, "block4.png"));
    m_staticObjects.push_back(new StaticObj(19, 7, 1, 1, "block4.png")); 

    m_staticObjects.push_back(new StaticObj(4, 6, 1, 1, "block4.png"));

      for (int x = 9; x < 16; ++x) {
        m_staticObjects.push_back(new StaticObj(x, 7, 1, 1, "block4.png"));
    }

    m_staticObjects.push_back(new StaticObj(7, 7, 1, 1, "block4.png"));
    m_staticObjects.push_back(new StaticObj(16, 5, 1, 1, "block4.png"));
    m_staticObjects.push_back(new StaticObj(15, 5, 1, 1, "block4.png"));


    for (int x = 2; x < 5; ++x) {
        m_staticObjects.push_back(new StaticObj(x, 6, 1, 1, "block4.png"));
    }
    int steppingStonesY = 4;
    m_staticObjects.push_back(new StaticObj(6, steppingStonesY, 1, 1, "block4.png"));
    m_staticObjects.push_back(new StaticObj(8, steppingStonesY - 1, 1, 1, "block4.png"));
    m_staticObjects.push_back(new StaticObj(10, steppingStonesY - 2, 1, 1, "block4.png"));
    m_staticObjects.push_back(new StaticObj(12, steppingStonesY - 1, 1, 1, "block4.png"));
    m_staticObjects.push_back(new StaticObj(14, steppingStonesY, 1, 1, "block4.png"));

    for (int x = -2; x <= 8; x+=2) {
        m_staticObjects.push_back(new StaticObj(18, x, 2, 2, "glitch3.png"));
    }

    for (int x = -2; x <= 8; x += 2) {
        m_staticObjects.push_back(new StaticObj(2, x, 2, 2, "glitch3.png"));
    }

    for (auto p_gob : m_staticObjects) {
        if (p_gob) p_gob->init();
    }

}

void Level::draw()
{
    float w = m_state->getCanvasWidth();
    float h = m_state->getCanvasHeight();

    float offset_x = m_state->m_global_offset_x / 2.0f + w / 2.0f;
    float offset_y = m_state->m_global_offset_y / 2.0f + h / 2.0f;

    graphics::drawRect(offset_x, offset_y, 4.0f * w, 2.0f * w, m_brush_backround);

    graphics::Brush br;

    if (m_state->getPlayer()->isActive())
        m_state->getPlayer()->draw();

    for (auto& staticObj : m_staticObjects) {
        for (int i = 0; i < staticObj->m_blocks.size(); ++i) {
            staticObj->drawBlock(i);
        }
    }
    
    graphics::Brush br_portal;

    float x = checkpointX + m_state->m_global_offset_x;
    float y = checkpointY + m_state->m_global_offset_y;

    br_portal.texture = m_state->getFullAssetPath("portal1.png");
    br_portal.outline_opacity = 0.0f;
    graphics::drawRect(x,y , 1.0f, 1.0f, br_portal);



    float player_life = m_state->getPlayer()->isActive() ? m_state->getPlayer()->getRemainingLife() : 0.0f;

    br.outline_opacity = 0.0f;
    SETCOLOR(br.fill_color, 1.0f, 0.2f, 0.2f);
    SETCOLOR(br.fill_secondary_color, 1.0f * (1.0f - player_life) + player_life * 0.2f, 0.2f, 0.2 * (1.0f - player_life) + player_life * 1.0f);

    br.gradient = true;
    br.gradient_dir_u = 1.0f;
    br.gradient_dir_v = 0.0f;
    graphics::drawRect(m_state->getCanvasWidth() - 0.8f - player_life * 1.2f / 2, m_state->getCanvasHeight() * 0.15f, player_life * 1.2f, 0.3f, br);


    br.gradient = false;
    br.outline_opacity = 1.0f;
    br.fill_opacity = 0.0f;
    graphics::drawRect(m_state->getCanvasWidth() - 0.8f - 1.2f / 2, m_state->getCanvasHeight() * 0.15f, 1.2f, 0.3f, br);

    for (auto bug : bugs) {
        bug->draw();
    }
}

Level::Level(const std::string& name)
    : GameObject(name)
{
    m_brush_backround.outline_opacity = 0.0f;
    m_brush_backround.texture = m_state->getFullAssetPath("green.png");

}


void Level::spawnBug()
{

    const size_t MAX_BUGS = 8; // Maximum number of bugs at any time
    if (bugs.size() < MAX_BUGS) {
        Enemy* newBug = new Enemy();
        newBug->init();
        bugs.push_back(newBug);
    }
}

void Level::checkBug()
{
    for (auto it = bugs.begin(); it != bugs.end();) {
        if (!(*it)->isActive()) {
            delete* it; // Free the memory
            it = bugs.erase(it); 
        }
        else {
            ++it; 
        }
    }
}

void Level::CheckEnemyCollision()
{

    auto it = bugs.begin();
    while (it != bugs.end()) {
        Box playerHull = m_state->getPlayer()->getCollisionHull();
        Box bugHull = (*it)->getCollisionHull();

        if (playerHull.intersectSideways(bugHull) || playerHull.intersectDown(bugHull)) {
            delete* it; 
            it = bugs.erase(it); 

            m_state->getPlayer()->drainLife(0.1f); 

        }
        else {
            ++it; 
        }
    }
}

void Level::checkCollisions()
{

    for (auto& staticObj : m_staticObjects) {
        {
            float offset = 0.0f;
            if (offset = m_state->getPlayer()->intersectSideways(*staticObj))
            {
                m_state->getPlayer()->m_pos_x += offset;

                m_state->getPlayer()->m_vx = 0.0f;
                break;
            }
        }

    }

    for (auto& staticObj : m_staticObjects) {
        {
            float offset = 0.0f;
            if (offset = m_state->getPlayer()->intersectDown(*staticObj))
            {
                m_state->getPlayer()->m_pos_y += offset;

                if (m_state->getPlayer()->m_vy > 1.0f)
                    graphics::playSound(m_state->getFullAssetPath("boxcollision.wav"), 1.0f);

                m_state->getPlayer()->m_vy = 0.0f;

                break;
            }
        }
    }

    for (auto& staticObj : m_staticObjects) {
        Box playerHull = m_state->getPlayer()->getCollisionHull();
        Box objectHull = staticObj->getCollisionHull();

        if (playerHull.intersect(objectHull) && m_state->getPlayer()->m_vy < 0) {

            m_state->getPlayer()->m_pos_y = objectHull.m_pos_y + objectHull.m_height / 2 + playerHull.m_height / 2;
            m_state->getPlayer()->m_vy = 0;


            break;
        }
    }


}


Level::~Level()
{
    for (auto p_go : m_staticObjects)
        delete p_go;

    for (auto bug : bugs) {
        delete bug;
    }
    bugs.clear();
}
