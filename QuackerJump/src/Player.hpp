#pragma once

enum Buff {
	NO_BUFF
};

class Player
{
private:
	int x, y;
	bool jumping;
	Buff active_buff;
public:
	Player();
	void jump(int dy);
	void fall(int dy);
	void move(int dx);
	void set_buff(Buff new_buff);
	int get_x() const;
	int get_y() const;
};

