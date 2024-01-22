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

bool Platform::player_on_platform(const Player& player) const
{
    return (x <= player.get_x() + Player::SIZE &&
        player.get_x() <= x + WIDTH &&
        y <= player.get_y() + Player::SIZE &&
        player.get_y() + Player::SIZE <= y + HEIGHT);
}

Platform::Platform(int x, int y) : x(x), y(y), type(random_type())
{   
    if (type == PlatformType::MOVING) {
        speed = PLATFORM_SPEED;
    }
}

Platform::~Platform()
{
}

bool Platform::operator==(const Platform& other) const
{
    return (x == other.x && y == other.y && type == other.type);
}
