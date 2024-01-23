#pragma once
#include "Player.hpp"

#define PLATFORM_SPEED 3

enum PlatformType {DEFAULT, MOVING, FRAGILE, SLOWING};

class Platform
{
private:
	static PlatformType random_type();
	int x, y;
	PlatformType type;
public:
	bool player_on_platform(const Player& player, double player_speed) const;
	static const int WIDTH = 200;
	static const int HEIGHT = 20;
	int speed;
	Platform(int x, int y);
	~Platform();
	bool operator==(const Platform& other) const;
	PlatformType get_type() const;
	int left_border() const;
	int right_border() const;
	int top_border() const;
	int bottom_border() const;
	void move(int dx, int dy);
};

