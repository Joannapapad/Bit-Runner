#include "Enemy.h"
#include "graphics.h"
#include "GameState.h"
#include <random>
#include <iostream>

void Enemy::update(float dt)
{
	float delta_time = dt / 1000.0f;
	enemy_pos_x -= speed * dt;
	animationTimer += delta_time;

	if (enemy_pos_x < -size)
	{
		active = false;
	}
}

void Enemy::draw()
{
	int animationIndex;

	animationIndex = (int)fmod(animationTimer * 5.0f, m_enemy_animation.size());

	m_brush_enemy.texture = m_enemy_animation[animationIndex];
	m_brush_enemy.outline_opacity = 0.0f;
	graphics::drawRect(enemy_pos_x + m_state->m_global_offset_x, enemy_pos_y + m_state->m_global_offset_y , size, size, m_brush_enemy);

}

void Enemy::init()
{
	speed = 0.003f;
	enemy_pos_x = m_state->getCanvasWidth() + 1.2f* rand()/(float)RAND_MAX + 14.0f;
	enemy_pos_y = m_state->getCanvasHeight() *1.2f* rand()/(float)RAND_MAX;
	size = 0.7f;

	m_enemy_animation.push_back(m_state->getFullAssetPath("drone-1.png"));
	m_enemy_animation.push_back(m_state->getFullAssetPath("drone-2.png"));
	m_enemy_animation.push_back(m_state->getFullAssetPath("drone-3.png"));
	m_enemy_animation.push_back(m_state->getFullAssetPath("drone-4.png"));

}



Enemy::Enemy() {
	init();
}



Box Enemy::getCollisionHull() const
{
	Box box;
	box.m_pos_x = enemy_pos_x;
	box.m_pos_y = enemy_pos_y;
	box.m_height = 0.6f;
	box.m_width = 0.6f;

	return box;
}

Enemy::~Enemy(){}