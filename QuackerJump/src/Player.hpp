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
	Player();
	void move(int dx,int dy);
	void set_buff(Buff new_buff);
	int get_x() const;
	int get_y() const;
};

