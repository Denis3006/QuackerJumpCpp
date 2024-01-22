#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Window/Event.hpp>
#include "Player.hpp"
#include "Platform.hpp"
#include <vector>
#include <iterator>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1300


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
	const int FRAMES_PER_JUMP = 50;
	const int DEFAULT_JUMPING_SPEED = 8;
	const int DEFAULT_JUMPING_HEIGHT = DEFAULT_JUMPING_SPEED * FRAMES_PER_JUMP;
	int jump_frames_left;
	int jumping_speed;
	int score;
	Player player;
	std::vector<Platform> platforms;
	bool new_platforms_needed() const;
	void handle_moving_platforms();
	void scroll_platforms(int dy);
	void create_random_platform();
	const Platform* player_on_platform() const;
	void delete_platform(const Platform* platform);
	int jumping_height() const;
	const int GRAVITY = 8;
};

