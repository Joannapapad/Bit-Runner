#include "Player.h"
#include "graphics.h"
#include "GameState.h"
#include <util.h>
#include"math.h"
#include "box.h"

bool isMoving = false;


float Player::getX()
{
    return m_pos_x;
}
float Player::getY()
{
    return m_pos_y;
}

void Player::update(float dt)
{
    float delta_time = dt / 1000.0f;
    const float v = 9.0f;

    isMoving = (m_vx != 0.0f);

    movePlayer(dt);
    animationTimer += delta_time;


    m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
    m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

    GameObject::update(dt);
}

float Player::getSpeed() const
{
    return fabs(m_vx);
}

void Player::init()
{
    m_pos_x = 4.0f;
    m_pos_y = 7.0f;


    m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
    m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

    m_brush_player.fill_opacity = 1.0f;
    m_brush_player.outline_opacity = 0.0f;
    m_width = 0.5f;

    m_brush_player.texture = m_state->getFullAssetPath("idle-1.png");

    m_player_right_animation.push_back(m_state->getFullAssetPath("run-1.png"));
    m_player_right_animation.push_back(m_state->getFullAssetPath("run-2.png"));
    m_player_right_animation.push_back(m_state->getFullAssetPath("run-3.png"));
    m_player_right_animation.push_back(m_state->getFullAssetPath("run-4.png"));
    m_player_right_animation.push_back(m_state->getFullAssetPath("run-5.png"));
    m_player_right_animation.push_back(m_state->getFullAssetPath("run-6.png"));
    m_player_right_animation.push_back(m_state->getFullAssetPath("run-7.png"));
    m_player_right_animation.push_back(m_state->getFullAssetPath("run-8.png"));

    m_player_idle_animation.push_back(m_state->getFullAssetPath("idle-1.png"));
    m_player_idle_animation.push_back(m_state->getFullAssetPath("idle-2.png"));
    m_player_idle_animation.push_back(m_state->getFullAssetPath("idle-3.png"));
    m_player_idle_animation.push_back(m_state->getFullAssetPath("idle-4.png"));


    m_player_left_animation.push_back(m_state->getFullAssetPath("run-1-left.png"));
    m_player_left_animation.push_back(m_state->getFullAssetPath("run-2-left.png"));
    m_player_left_animation.push_back(m_state->getFullAssetPath("run-3-left.png"));
    m_player_left_animation.push_back(m_state->getFullAssetPath("run-4-left.png"));
    m_player_left_animation.push_back(m_state->getFullAssetPath("run-5-left.png"));
    m_player_left_animation.push_back(m_state->getFullAssetPath("run-6-left.png"));
    m_player_left_animation.push_back(m_state->getFullAssetPath("run-7-left.png"));
    m_player_left_animation.push_back(m_state->getFullAssetPath("run-8-left.png"));


}

void Player::draw()
{
    float animation_speed = 0.6f;  \

    // Choose animation based on movement state or button press
    int animationIndex;


    if (isMoving) {
        if (graphics::getKeyState(graphics::SCANCODE_LEFT)) {
            animationIndex = (int)fmod(100.0f - m_pos_x * 9.0f * animation_speed, m_player_left_animation.size());
        }
        else if (graphics::getKeyState(graphics::SCANCODE_RIGHT)) {
            animationIndex = (int)fmod(100.0f - m_pos_x * 9.0f * animation_speed, m_player_right_animation.size());
        }
        else if (graphics::getKeyState(graphics::SCANCODE_UP)) {
            animationIndex = (int)fmod(animationTimer * 0.6f, m_player_idle_animation.size());
        }
    }
    else {
        animationIndex = (int)fmod(animationTimer * 3.0f, m_player_idle_animation.size());
    }


    if (isMoving) {
        if (graphics::getKeyState(graphics::SCANCODE_LEFT)) {
            m_brush_player.texture = m_player_left_animation[animationIndex];
        }
        else if (graphics::getKeyState(graphics::SCANCODE_RIGHT)) {
            m_brush_player.texture = m_player_right_animation[animationIndex];
        }
        else if (graphics::getKeyState(graphics::SCANCODE_UP)) {
            m_brush_player.texture = m_player_idle_animation[animationIndex];
        }
    }
    else {
        m_brush_player.texture = m_player_idle_animation[animationIndex];
    }


    graphics::drawRect(m_state-> getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f,
        1.0f, 1.0f , m_brush_player);
    if (m_state->m_debugging)
        debugDraw();
}


void Player::debugDraw()
{
    graphics::Brush debug_brush;
    SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
    SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
    debug_brush.fill_opacity = 0.1f;
    debug_brush.outline_opacity = 1.0f;
    graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, m_width, m_height, debug_brush);

    char s[20];
    sprintf_s(s, "(%5.2f, %5.2f)", m_pos_x, m_pos_y);
    SETCOLOR(debug_brush.fill_color, 1, 0, 0);
    debug_brush.fill_opacity = 1.0f;
    graphics::drawText(m_state->getCanvasWidth() * 0.5f - 0.4f, m_state->getCanvasHeight() * 0.5f - 0.6f, 0.15f, s, debug_brush);
}

void Player::movePlayer(float dt)
{
    float delta_time = dt / 1000.0f;

    float move = 0.0f;

    if (graphics::getKeyState(graphics::SCANCODE_LEFT))
        move -= 1.0f;
    if (graphics::getKeyState(graphics::SCANCODE_RIGHT))
        move += 1.0f;

    m_vx = std::min<float>(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);
    m_vx = std::max<float>(-m_max_velocity, m_vx);
    

    m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx));

    if (fabs(m_vx) < 0.01f)
        m_vx = 0.0f;

    m_pos_x += m_vx * delta_time;

    if (m_vy == 0.0f)
        m_vy -= (graphics::getKeyState(graphics::SCANCODE_UP) ? m_accel_vertical : 0.0f) * 0.02f;// not delta_time!! Burst 

    // add gravity
    m_vy += delta_time * m_gravity;

    // adjust vertical position
    m_pos_y += m_vy * delta_time;


}

Box Player::getCollisionHull() const
{
    Box box;
    box.m_pos_x = m_pos_x;
    box.m_pos_y = m_pos_y;
    box.m_height = 0.8f;
    box.m_width = 0.4f;

    return box;
}

