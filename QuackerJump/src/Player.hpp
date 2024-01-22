#pragma once

enum Buff {
	NO_BUFF
};

class Player
{
private:
	int x, y;
	Buff active_buff;
public:
	static const int SIZE = 100;
	Player();
	Player(int starting_x, int starting_y);
	void move(int dx,int dy);
	void set_buff(Buff new_buff);
	int get_x() const;
	int get_y() const;
};

