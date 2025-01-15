#include "Level2.h"
#include "Level.h"
#include "GameState.h"
#include "Player.h"
#include"vector"
#include <util.h>
#include "graphics.h"

Level2::Level2(const std::string& name) : Level(name) 
{
    checkpointX = 3.0f;
    checkpointY = 1.0f; 
    m_brush_backround.outline_opacity = 0.0f;
    m_brush_backround.texture = m_state->getFullAssetPath("level1.png");
}

Level2::~Level2()
{
    for (auto p_go : m_staticObjects)
        delete p_go;

    for (auto bug : bugs) {
        delete bug;
    }
    bugs.clear();
}

void Level2::init()
{

    // Clear existing static objects
    for (auto obj : m_staticObjects) {
        delete obj;
    }
    m_staticObjects.clear();

    // Ground floor
    for (int x = 0; x < 20; ++x) {
        m_staticObjects.push_back(new StaticObj(x, 9, 1, 1, "glitch.png"));
    }


    for (int x = 9; x < 16; ++x) {
        m_staticObjects.push_back(new StaticObj(x, 7, 1, 1, "glitch.png"));
    }

    m_staticObjects.push_back(new StaticObj(7, 7, 1, 1, "glitchtxt2.png"));
    m_staticObjects.push_back(new StaticObj(15, 4, 1, 1, "glitchtxt2.png"));
    m_staticObjects.push_back(new StaticObj(16, 4, 1, 1, "glitchtxt2.png"));
    m_staticObjects.push_back(new StaticObj(17, 4, 1, 1, "glitchtxt2.png"));


    for (int x = 2; x < 5; ++x) {
        m_staticObjects.push_back(new StaticObj(x, 6, 1, 1, "glitch.png"));
    }

    int steppingStonesY = 4;
    m_staticObjects.push_back(new StaticObj(6, steppingStonesY, 1, 1, "glitchtxt2.png"));
    m_staticObjects.push_back(new StaticObj(8, steppingStonesY - 1, 1, 1, "glitch.png"));
    m_staticObjects.push_back(new StaticObj(10, steppingStonesY - 2, 1, 1, "glitch.png"));
    m_staticObjects.push_back(new StaticObj(12, steppingStonesY - 1, 1, 1, "glitch.png"));
    m_staticObjects.push_back(new StaticObj(14, steppingStonesY, 1, 1, "glitch.png"));

    for (int x = -2; x <= 8; x += 2) {
        m_staticObjects.push_back(new StaticObj(18, x, 2, 2, "glitchtxt3.png"));
    }

    for (int x = -2; x <= 8; x += 2) {
        m_staticObjects.push_back(new StaticObj(1, x, 2, 2, "glitch3.png"));
    }
    m_staticObjects.push_back(new StaticObj(3, 2, 1, 1, "glitch.png"));


    for (int x = 12; x < 15; ++x) {
        for (int y = 2; y < 5; ++y) {
            if (!(x == 13 && y == 3)) { 
                m_staticObjects.push_back(new StaticObj(x, y, 1, 1, "glitch.png"));
            }
        }
    }

    m_staticObjects.push_back(new StaticObj(1, 3, 1, 1, "glitch.png"));
    m_staticObjects.push_back(new StaticObj(3, 2, 1, 1, "glitchtxt3.png"));
    m_staticObjects.push_back(new StaticObj(5, 1, 1, 1, "glitchtxt3.png"));

    for (int y = 0; y < 4; ++y) {
        m_staticObjects.push_back(new StaticObj(4, y, 1, 1, "glitch.png"));
    }

    // Initialize each static object
    for (auto p_gob : m_staticObjects) {
        if (p_gob) p_gob->init();
    }
}

void Level2::update(float dt)
{
    if (m_state->getPlayer()->isActive()) {
        m_state->getPlayer()->update(dt);
    }

    checkBug(); 
    spawnBug(); 


    for (auto bug : bugs) {
        bug->update(dt);
    }

    CheckEnemyCollision(); // Check and handle collisions with all bugs

    checkCollisions();

    // Check if player has reached the checkpoint
    if (!checkpointReached &&
        m_state->getPlayer()->getX() <= checkpointX && m_state->getPlayer()->getY() <= checkpointY) {
        checkpointReached = true;
        m_state->goToNextLevel(); // This assumes GameState has a method to go to the next level
    }

    GameObject::update(dt);

}

void Level2::draw()
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

    //Portal Texture

    float x = checkpointX + m_state->m_global_offset_x;
    float y = checkpointY + m_state->m_global_offset_y;


    br_portal.texture = m_state->getFullAssetPath("portal1.png");
    br_portal.outline_opacity = 0.0f;
    graphics::drawRect(x, y, 1.0f, 1.0f, br_portal);


    //HealthBar
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

