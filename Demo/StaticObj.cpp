#include "StaticObj.h"
#include "graphics.h" 
#include "Level.h"
#include "GameState.h"
#include <util.h>
// Constructor implementation
StaticObj::StaticObj(float x, float y, float width, float height, const std::string& textureName)
    : Box(x, y, width, height), m_textureName(textureName) {
    m_blocks.push_back(Box(x, y, width, height));
    m_blocks_names.push_back(textureName);

}

// init method implementation
void StaticObj::init() {
    
}

// update method implementation
void StaticObj::update(float dt) {
   
}

void StaticObj::draw() {

}

void StaticObj::drawBlock(int i)
{
    Box& box = m_blocks[i];
    float x = box.m_pos_x + m_state->m_global_offset_x;
    float y = box.m_pos_y + m_state->m_global_offset_y;
    SETCOLOR(m_blocks_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);
    SETCOLOR(m_blocks_brush_debug.fill_color, 0.3f, 1.0f, 0.2f);

    m_blocks_brush.texture = m_state->getFullAssetPath(m_blocks_names[i]);
    m_blocks_brush.outline_opacity = 0.0f;

    graphics::drawRect(x, y, box.m_width, box.m_height, m_blocks_brush);
    if (m_state->m_debugging)
        graphics::drawRect(x, y, m_blocks_size, m_blocks_size, m_blocks_brush_debug);

}

Box StaticObj::getCollisionHull() const
{
    Box box;
    box.m_pos_x = m_pos_x;
    box.m_pos_y = m_pos_y;
    box.m_height = 1.0f;
    box.m_width = 1.0f;

    return box;
}

// Destructor implementation
StaticObj::~StaticObj() = default;
