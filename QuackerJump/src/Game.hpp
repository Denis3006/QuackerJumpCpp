#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Window/Event.hpp>
#include "Player.hpp"
#include "Platform.hpp"
#include <vector>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1300
#define PLAYER_SIZE 100


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
	sf::RenderWindow window;
	const int frames_per_jump = 50;
	int jump_frames_left;
	int jumping_speed;
	int platform_speed = PLATFORM_SPEED;
	Player player;
	std::vector<Platform> platforms;
	void move_platforms(int dy);
	void create_random_platforms(int n_platforms, int x_max, int y_max);
	bool on_platform();
	const int gravity = 2;
};

