#pragma once

enum Buff {
	NO_BUFF
};

class Player
{
private:
	int x, y;
	int jump_frames = 0;
	int jumping_speed;
	bool jumping;
	Buff active_buff;
public:
	Player(int jumping_speed);
	void move(int dx,int dy);
	void set_buff(Buff new_buff);
	int get_x() const;
	int get_y() const;
};

