#include "Player.hpp"

Player::Player() : x(0), y(0), jumping(false)
{
}

void Player::jump(int dy)
{
	y -= dy;
	jumping = true;
}

void Player::fall(int dy)
{
	y += dy;
	jumping = false;
}

void Player::move(int dx)
{
	x += dx;
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


