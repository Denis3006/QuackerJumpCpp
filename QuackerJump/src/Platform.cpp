#include "Platform.hpp"
#include <map>
#include <iostream>
#include <random>

PlatformType Platform::random_type()
{
    std::map<int, PlatformType> type_weights = {
        {60, PlatformType::DEFAULT},
        {15, PlatformType::MOVING},
        {15, PlatformType::FRAGILE},
        {10, PlatformType::SLOWING}
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 100);
    auto rand_number = distr(gen);

    for (auto it = type_weights.begin(); it != type_weights.end(); ++it) {
        auto weight = it->first;
        if (rand_number < weight) {
            return it->second;
        }
        else {
            rand_number -= weight;
        }
    }

    return PlatformType::DEFAULT;
}

Platform::Platform(int x, int y) : x(x), y(y), type(random_type())
{   
    if (type == PlatformType::MOVING) {
        speed = PLATFORM_SPEED;
    }
    std::cout << "Created platform at: " << x << ", " << y << std::endl;
}

Platform::~Platform()
{
}
