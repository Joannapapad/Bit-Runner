#pragma once
#include "Level.h"

class Level2 : public Level {
public:
    Level2(const std::string& name = "Level2");
	bool checkpointReached = false; // Example checkpoint position

    ~Level2() override;

    void draw() override;
	void init() override;
	void update(float dt) override;



};
