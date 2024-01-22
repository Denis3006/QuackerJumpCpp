#include "Player.hpp"

Player::Player() : Player(0, 0)
{
}

Player::Player(int starting_x, int starting_y) : x(starting_x), y(starting_y), active_buff(Buff::NO_BUFF)
{

}

void Player::move(int dx, int dy)
{
	x += dx;
	y += dy;
}

void Player::set_buff(Buff new_buff)
{
	active_buff = new_buff;
}

int Player::get_x() const
{
	return x;
}

int Player::get_y() const
{
	return y;
}


