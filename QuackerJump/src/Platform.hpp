#pragma once

enum PlatformType {DEFAULT, MOVING, FRAGILE, SLOWING};

class Platform
{
private:
	static PlatformType random_type();
public:
	PlatformType type;
	int x, y;
	static const int width = 200;
	static const int height = 20;
	Platform(int x, int y);
	~Platform();
};

