#pragma once
#include "GameObject.h"
#include "graphics.h"
#include <list>
#include "StaticObj.h"
#include "Enemy.h"

class Level : public GameObject
{
protected:
	std::vector<Enemy*> bugs; // Holds multiple bugs

	graphics::Brush m_brush_backround;

	float m_center_x = 5.0f;
	float m_center_y = 5.0f;


	float checkpointX; 
	float checkpointY;
	float animationTimer = 0.0f;

	bool checkpointReached = false; // Example checkpoint position

	std::vector<StaticObj*> m_staticObjects; 
	graphics::Brush m_blocks_brush;
	graphics::Brush m_blocks_brush_debug;
	void spawnBug();
	void checkBug();

	void CheckEnemyCollision();
	void checkCollisions();


public:
	virtual void update(float dt);
	virtual void init()  override;
	virtual void draw()  override;
	Level(const std::string& name = "Level0");
	~Level() override;

};