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

int Player::left_border() const
{
	return x;
}


int Player::right_border() const
{
	return left_border() + Player::SIZE;
}

int Player::top_border() const
{
	return y;
}

int Player::bottom_border() const
{
	return top_border() + Player::SIZE;
}


