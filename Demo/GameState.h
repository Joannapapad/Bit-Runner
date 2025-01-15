#pragma once
#include <string>
#include <graphics.h>

class GameState
{
private:
    std::string m_asset_path = "assets\\";

    bool m_game_started;

    const float m_canvas_width = 6.0f;
    const float m_canvas_height = 6.0f;

    static GameState* m_unique_instance;

    GameState();

    class Player* m_player = 0;
    class Level* m_current_level = 0;

    std::vector<Level*> levels;
    int currentLevelIndex = -1;

    bool gameOver = false;

public:

    float m_global_offset_x = 0.0f;
    float m_global_offset_y = 0.0f;
    bool m_debugging = false;

    void init();
    void draw();
    void update(float dt);
    void checkGameOver();
    void drawGameOverScreen();

    void setCurrentLevel(int index);

    static GameState* getInstance();

    void goToNextLevel();

    ~GameState();

    float getCanvasWidth() { return m_canvas_width; }
    float getCanvasHeight() { return m_canvas_height; }

    void drawStartScreen();
    std::string getAssetDir();
    std::string getFullAssetPath(const std::string& asset);

    class Player* getPlayer() { return m_player; }
};