#include "DesktopSoccer.h"
#include "SFML\System\Vector2.hpp"
#pragma once

class Player : DynamicCircle
{
public:
	Player();
	~Player();
	Player(bool team);
	bool getTeam();
private:
	bool team;
};

