#include "Platform.hpp"
#include <map>
#include <iostream>
#include <random>

PlatformType Platform::random_type()
{
    std::map<PlatformType, int> type_weights = {
        {PlatformType::DEFAULT, 60},
        {PlatformType::MOVING, 15},
        {PlatformType::FRAGILE, 15},
        {PlatformType::SLOWING, 10}
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 100);
    auto rand_number = distr(gen);

    for (auto it = type_weights.begin(); it != type_weights.end(); ++it) {
        auto weight = it->second;
        if (rand_number < weight) {
            return it->first;
        }
        else {
            rand_number -= weight;
        }
    }

    return PlatformType::DEFAULT;
}

bool Platform::player_on_platform(const Player& player, double player_speed) const
{
    bool horisontal_intersection = left_border() <= player.right_border() && player.left_border() <= right_border();
    bool vertical_intersection = top_border() <= player.bottom_border() && player.bottom_border() <= bottom_border();
    bool vertical_intesection_between_frames = player.bottom_border() < top_border() && top_border() < player.bottom_border() - player_speed;

    return horisontal_intersection && (vertical_intersection || vertical_intesection_between_frames);
}

Platform::Platform(int x, int y) : x(x), y(y), type(random_type())
{   
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 1);
    auto rand_number = distr(gen);

    if (type == PlatformType::MOVING) {
        speed = rand_number == 0 ? -PLATFORM_SPEED : PLATFORM_SPEED;
    }
}

Platform::~Platform()
{
}

bool Platform::operator==(const Platform& other) const
{
    return (x == other.x && y == other.y && type == other.type);
}

PlatformType Platform::get_type() const
{
    return type;
}

int Platform::get_speed() const
{
    return speed;
}

void Platform::swap_movement_direction()
{
    speed *= -1;
}

int Platform::left_border() const
{
    return x;
}

int Platform::right_border() const
{
    return left_border() + WIDTH;
}


int Platform::top_border() const
{
    return y;
}

int Platform::bottom_border() const
{
    return top_border() + HEIGHT;
}

void Platform::move(int dx, int dy)
{
    x += dx;
    y += dy;
}
