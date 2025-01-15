#pragma once
#include "GameObject.h"
#include "Box.h"
#include "graphics.h" 

class StaticObj : public Box, public GameObject, public Collidable {
public:

    float m_blocks_size = 1.0f;
    graphics::Brush m_blocks_brush;
    graphics::Brush m_blocks_brush_debug;

    std::vector<Box> m_blocks;
    std::vector<std::string> m_blocks_names;

    // Constructor
    StaticObj(float x, float y, float width, float height, const std::string& textureName);

    // Override GameObject methods
    void init() override;
    void update(float dt) override;
    void draw() override;
    void drawBlock(int i);
    Box getCollisionHull() const override;

    // Destructor
    ~StaticObj() override;

private:
    std::string m_textureName; 
};
