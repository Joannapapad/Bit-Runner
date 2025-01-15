#include "GameState.h"
#include "level.h"
#include "Player.h"
#include <util.h>
#include "Level2.h"
#include <iostream>

GameState::GameState() : m_game_started(false)
{

}

void GameState::init()
{
	levels.push_back(new Level("Level1"));
	levels.push_back(new Level2("Level2")); 

	setCurrentLevel(0);

	graphics::setFont(m_asset_path + "OpenSans-Regular.ttf");

}

void GameState::draw()
{
	if (!m_game_started) {

		drawStartScreen();

		return;
	}

	if (!m_current_level)
		return;
	m_current_level->draw();

	if (gameOver) { // Check if the game is over
		graphics::playSound(getFullAssetPath("opening.wav"), 0.5f);

		drawGameOverScreen(); // Draw the game over screen
		return;
	}
}

void GameState::update(float dt)
{
	if (dt > 500)
		return;


	if (!m_game_started) {

		if (graphics::getKeyState(graphics::SCANCODE_RETURN)) {
			m_game_started = true; // Start the game
		}
		return; 
	}

	checkGameOver();

	if (!m_current_level) return;


	m_current_level->update(dt);

	m_debugging = graphics::getKeyState(graphics::SCANCODE_0);

}

void GameState::checkGameOver()
{
		if (m_player->getRemainingLife() <= 0) {
			gameOver = true;
		}
	
}

void GameState::drawGameOverScreen()
{
	graphics::Brush br;

	br.texture = getFullAssetPath("GameOver.png"); 
	br.outline_opacity = 0.0f;
	graphics::drawRect(getCanvasWidth() / 2, getCanvasHeight() / 2, getCanvasWidth(), getCanvasHeight(), br);
}

GameState* GameState::getInstance()
{
	if (m_unique_instance == nullptr)
		m_unique_instance = new GameState();
	return m_unique_instance;
}

GameState::~GameState()
{
	if (m_player)
		delete m_player;

	for (auto& level : levels) {
		delete level;
	}
	levels.clear();
}

void GameState::drawStartScreen()
{

	graphics::Brush br;
	graphics::Brush br_txt;

	char info[40];
	sprintf_s(info, "Press ENTER to start");
	graphics::drawText(getCanvasWidth()*0.3f, getCanvasHeight()*0.7f, 0.3f, info, br_txt);
	br.texture = m_unique_instance->getFullAssetPath("computer1.png");
	br.outline_opacity = 0.0f;
	graphics::drawRect(getCanvasWidth() / 2, getCanvasHeight() / 2, getCanvasWidth(), getCanvasHeight(), br);


}

std::string GameState::getAssetDir()
{
	return m_asset_path;
}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}

void GameState::setCurrentLevel(int index) {
	if (index >= 0 && index < levels.size()) {
		if (m_current_level) {
			delete m_current_level; // Properly delete the current level
		}
		currentLevelIndex = index;
		m_current_level = levels[index]; // Set the new level

		m_player = new Player("Player");
		m_player->init();
		m_current_level->init(); // Initialize the new level
	}
}

void GameState::goToNextLevel() {
	int nextLevelIndex = currentLevelIndex + 1;
	if (nextLevelIndex < levels.size()) {
		setCurrentLevel(nextLevelIndex);
	}
	else {
		gameOver = true;

	}
}

GameState* GameState::m_unique_instance = nullptr;
