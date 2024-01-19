#include "Game.hpp"
#include <SFML/Window/Event.hpp>
#include "SFML/Graphics/RectangleShape.hpp"
#include <random>
#include <iostream>

Game::Game() : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Quacker Jump"), jumping_speed(5)
{
	window.setFramerateLimit(60);
	create_random_platforms(5, SCREEN_WIDTH - Platform::width, SCREEN_HEIGHT - Platform::height);
}

void Game::draw_game()
{
	window.clear(sf::Color::Black);
	// draw player
	sf::RectangleShape player_model(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
	player_model.setFillColor(sf::Color::Magenta);
	player_model.setPosition(player.get_x(), player.get_y());
	window.draw(player_model);
	
	// draw platforms
	for (const auto& platform : platforms) {
		sf::RectangleShape plaftorm_model(sf::Vector2f(platform.width, platform.height));
		switch (platform.type) {
		case PlatformType::DEFAULT:
			plaftorm_model.setFillColor(sf::Color::Red);
			break;
		case PlatformType::MOVING:
			plaftorm_model.setFillColor(sf::Color::Green);
			break;
		case PlatformType::FRAGILE:
			plaftorm_model.setFillColor(sf::Color::Yellow);
			break;
		case PlatformType::SLOWING:
			plaftorm_model.setFillColor(sf::Color::Blue);
			break;
		}
		plaftorm_model.setPosition(platform.x, platform.y);
		window.draw(plaftorm_model);
	}
	window.display();
}


bool Game::running() {
	return true;
}

bool Game::poll_event(sf::Event& event)
{
	return window.pollEvent(event);
}

void Game::get_user_input()
{
	sf::Event event;
	while (poll_event(event))
	{
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
				moving_left = true;
			}
			if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
				moving_right = true;
			}
			if (event.key.code == sf::Keyboard::Space) {
				jump_frames_left = frames_per_jump;
			}
		}
		if (event.type == sf::Event::KeyReleased) {
			if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
				moving_left = false;
			}
			if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
				moving_right = false;
			}
		}
	}
}

void Game::update_game_state()
{
	bool moving_sideways = (moving_left != moving_right);
	int dx = 0;
	int dy = 10;
	if (moving_sideways) {
		dx = moving_right ? 10 : -10;
	}
	if (jump_frames_left > 0) {
		jump_frames_left--;
		dy = -jumping_speed;
		move_platforms(jumping_speed);
		// TODO: spawn new platforms above the screen every time player jumps (min_y = -Y_LIMIT, max_y = Platform::height)
	}
	else if (!on_platform()) {
		dy = gravity;
		move_platforms(0);
	}
	else {
		jump_frames_left = frames_per_jump;
		move_platforms(0);
	}
	player.move(dx, dy);
}

bool Game::on_platform()
{
	for (const auto& platform : platforms) {
		if (platform.x <= player.get_x() + PLAYER_SIZE && player.get_x() <= platform.x + platform.width && platform.y <= player.get_y() + PLAYER_SIZE && player.get_y() + PLAYER_SIZE <= platform.y + platform.height) {
			return true;
		}
	}
	return false;
}

void Game::move_platforms(int dy) {
	int n_platforms_removed = 0;
	for (int i = 0; i < platforms.size(); i++) {
		platforms[i].y += dy;
		if (platforms[i].type == PlatformType::MOVING) {
			if (platforms[i].x + platforms[i].speed + Platform::width > SCREEN_WIDTH || platforms[i].x + platforms[i].speed < 0) {
				platforms[i].speed *= -1;
			}
			platforms[i].x += platforms[i].speed;
		}
		if (platforms[i].y > SCREEN_HEIGHT) {  // platform is outside the screen -> erase
			std::cout << platforms[i].y << std::endl;
			n_platforms_removed++;
		}
	}
	platforms.erase(std::remove_if(platforms.begin(),
		platforms.end(),
		[](Platform platform) { return platform.y > SCREEN_HEIGHT; }), platforms.end());
	create_random_platforms(n_platforms_removed, SCREEN_WIDTH - Platform::width, int(SCREEN_HEIGHT / 2));

}

void Game::create_random_platforms(int n_platforms, int x_max, int y_max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> x_distr(0, x_max);
	std::uniform_int_distribution<> y_distr(0, y_max);

	for (int i = 0; i < n_platforms; i++) {
		platforms.push_back(Platform(x_distr(gen), y_distr(gen)));
	}
}
