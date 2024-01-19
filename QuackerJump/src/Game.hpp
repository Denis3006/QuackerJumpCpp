#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Window/Event.hpp>
#include "Player.hpp"
#include "Platform.hpp"
#include <vector>
class Game
{
public:
	Game();
	void draw_game();
	bool running();
	bool poll_event(sf::Event& event);
	void get_user_input();
	void update_game_state();
private:
	bool moving_right = false;
	bool moving_left = false;
	bool jumping = false;
	sf::RenderWindow window;
	Player player;
	std::vector<Platform> platforms;
	const int gravity = 5;
};

