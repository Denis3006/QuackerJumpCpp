#include "Game.hpp"
#include <SFML/Window/Event.hpp>
#include "SFML/Graphics/RectangleShape.hpp"

Game::Game() : window(sf::VideoMode(1920, 1080), "Quacker Jump") 
{
	window.setFramerateLimit(60);
	for (int i = 0; i < 5; i++) {
		platforms.push_back(Platform((i + 1) * 200, i * 100));
	}
}

void Game::draw_game()
{
	window.clear(sf::Color::Black);
	// draw player
	sf::RectangleShape player_model(sf::Vector2f(100, 100));
	player_model.setFillColor(sf::Color::Magenta);
	player_model.setPosition(player.get_x(), player.get_y());
	window.draw(player_model);
	
	// draw platforms
	for (auto platform : platforms) {
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
				jumping = true;
			}
		}
		if (event.type == sf::Event::KeyReleased) {
			if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
				moving_left = false;
			}
			if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
				moving_right = false;
			}
			if (event.key.code == sf::Keyboard::Space) {
				jumping = false;
			}
		}
	}
	bool moving_sideways = (moving_left != moving_right);
	if (moving_sideways && jumping) {
		if (moving_left) {
			player.move(-10); 
			player.jump(20);
		}
		else {
			player.move(10);
			player.jump(20);
		}
	}
	else if (moving_sideways && !jumping) {
		if (moving_left) {
			player.move(-10);
 			player.fall(gravity);
		}
		else {
			player.move(10);
			player.fall(gravity);
		}
	}
	else if (jumping) {
		player.jump(100);
	}
	else {
		player.fall(gravity);
	}
}

void Game::update_game_state()
{
}
