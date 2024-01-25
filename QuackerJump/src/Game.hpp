#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Text.hpp"
#include "Player.hpp"
#include "Platform.hpp"
#include "SFML/Graphics/Font.hpp"
#include <vector>
#include <iterator>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1200


class Game
{
public:
	Game();
	void draw_game();
	bool running();
	bool game_over();
	bool poll_event(sf::Event& event);
	void get_user_input();
	void update_game_state();
private:
	bool moving_right = false;
	bool moving_left = false;
	sf::RenderWindow window;
	sf::Font score_font;
	const double V_0 = 30;
	const double V_HORIZONTAL = 10;
	double GRAVITY = 1.25;
	double v = V_0 + GRAVITY; 
	int score;
	Player player;
	std::vector<Platform> platforms;

	bool new_platforms_needed() const;
	void handle_moving_platforms();
	void scroll_platforms(int dy);
	void create_random_platform();
	const Platform* player_on_platform() const;
	void delete_platform(const Platform* platform);
	double jumping_height(double v_0) const;
	double jumping_distance(double v_0) const;
	double v0_slowed() const;


	// debug variables
	double sum = 0;
	int n = 0;
};

