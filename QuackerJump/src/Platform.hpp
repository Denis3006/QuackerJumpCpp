#pragma once
#include "Player.hpp"

#define PLATFORM_SPEED 3

enum PlatformType {DEFAULT, MOVING, FRAGILE, SLOWING};

class Platform
{
private:
	static PlatformType random_type();
public:
	PlatformType type;
	int x, y;
	bool player_on_platform(const Player& player) const;
	static const int WIDTH = 200;
	static const int HEIGHT = 20;
	int speed;
	Platform(int x, int y);
	~Platform();
	bool operator==(const Platform& other) const;
};

