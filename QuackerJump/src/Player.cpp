#include "Player.hpp"

Player::Player() : x(0), y(0)
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


