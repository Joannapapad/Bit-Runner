#pragma once
#include "GameObject.h"
#include "box.h"
#include "graphics.h"
#include "Enemy.h"

class Player : public GameObject, public Collidable, public Box
{
	graphics::Brush m_brush_player;

	std::vector<std::string> m_player_right_animation;
	std::vector<std::string> m_player_idle_animation;
	std::vector<std::string> m_player_left_animation;
	std::vector<std::string> m_player_up_animation;
	std::vector<std::string> m_player_down_animation;

	float animationTimer = 0.0f;

	float player_life = 1.0f;

	const float m_accel_horizontal = 70.0f;
	const float m_accel_vertical = 300.0f;
	const float m_max_velocity = 4.0f;
	const float m_gravity = 10.0f;
public:
	float m_vx = 0.0f;
	float m_vy = 0.0f;

public:

	Player(std::string name) : GameObject(name) {}
	float getX();
	float getY();

	void update(float dt) override;
	float getSpeed() const;
	void init()  override;
	void draw()  override;
	float getRemainingLife() const { return player_life; }
	Box getCollisionHull() const override;
	void drainLife(float amount) { player_life = std::max<float>(0.0f, player_life - amount); }
protected:
	void debugDraw();

	// dynamic motion control
	void movePlayer(float dt);


};