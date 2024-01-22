#include "Game.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include <random>
#include <iostream>

Game::Game() : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Quacker Jump"), 
			   jumping_speed(DEFAULT_JUMPING_SPEED), 
			   player(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 3 * Player::SIZE),
			   score(0)
{
	if (!font.loadFromFile("Arial.ttf"))
	{
		std::cout << "Error loading font" << std::endl;
	}
	window.setFramerateLimit(60);
	while (new_platforms_needed()) {
		create_random_platform();
	}
}

void Game::draw_game()
{
	window.clear(sf::Color::Black);
	sf::Text text;

	text.setFont(font); // font is a sf::Font
	text.setString("Score: " + std::to_string(score));
	text.setCharacterSize(24); // in pixels, not points!
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	window.draw(text);
	// draw player
	sf::RectangleShape player_model(sf::Vector2f(Player::SIZE, Player::SIZE));
	player_model.setFillColor(sf::Color::Magenta);
	player_model.setPosition(player.get_x(), player.get_y());
	window.draw(player_model);
	
	// draw platforms
	for (const auto& platform : platforms) {
		sf::RectangleShape plaftorm_model(sf::Vector2f(platform.WIDTH, platform.HEIGHT));
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
				jump_frames_left = FRAMES_PER_JUMP;
				jumping_speed = 3 * DEFAULT_JUMPING_SPEED;
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
	int dy = 0;

	auto player_platform = player_on_platform();

	if (moving_sideways) {
		dx = moving_right ? 10 : -10;
	}
	if (jump_frames_left > 0) {  // currently jumping
		jump_frames_left--;
		dy = -jumping_speed;
	}
	else if (player_platform != nullptr) {  // on platform and not already jumping - start the jump
		jump_frames_left = FRAMES_PER_JUMP;
		if (player_platform->type == PlatformType::SLOWING) {
			jumping_speed = DEFAULT_JUMPING_SPEED / 2;
		}
		else {
			jumping_speed = DEFAULT_JUMPING_SPEED;
		}
		if (player_platform->type == PlatformType::FRAGILE) {
			delete_platform(player_platform);
		}
		score += jumping_height() / 10;
	}
	else { // not on platform and not jumping - fall
		dy = GRAVITY;
	}

	if (player.get_y() < int(SCREEN_HEIGHT / 2)) {
		scroll_platforms(jumping_speed);
		player.move(0, jumping_speed);
	}
	handle_moving_platforms();
	while (new_platforms_needed()) {
		create_random_platform();
	}
	player.move(dx, dy);
}

const Platform* Game::player_on_platform() const
{
	for (const auto& platform : platforms) {
		if (platform.player_on_platform(player)) {
			return &platform;
		}
	}
	return nullptr;
}

void Game::delete_platform(const Platform* platform)
{
	platforms.erase(std::remove_if(platforms.begin(), platforms.end(),
		[platform](Platform p) { return p == *platform; }), platforms.end());

}

int Game::jumping_height() const
{
	return jumping_speed * FRAMES_PER_JUMP;
}

bool Game::new_platforms_needed() const
{
	if (platforms.empty()) {
		return true;
	}
	else {
		return platforms.back().y > 0;
	}
}

void Game::handle_moving_platforms() {
	for (int i = 0; i < platforms.size(); i++) {
		if (platforms[i].type == PlatformType::MOVING) {
			if (platforms[i].x + platforms[i].speed + Platform::WIDTH > SCREEN_WIDTH ||
				platforms[i].x + platforms[i].speed < 0) {
				platforms[i].speed *= -1;
			}
			platforms[i].x += platforms[i].speed;
		}
	}
}

void Game::scroll_platforms(int dy)
{
	for (int i = 0; i < platforms.size(); i++) {
		platforms[i].y += dy;
	}
	platforms.erase(std::remove_if(platforms.begin(), platforms.end(),
		[](Platform platform) { return platform.y > SCREEN_HEIGHT; }), platforms.end());
}


void Game::create_random_platform()
{
	int y_start = SCREEN_HEIGHT;
	int y_end = y_start - DEFAULT_JUMPING_HEIGHT;
	if (!platforms.empty()) {
		y_start = platforms.back().y - 2 * Platform::HEIGHT;
		int max_distance = DEFAULT_JUMPING_HEIGHT;
		if (platforms.back().type == PlatformType::SLOWING) {
			max_distance = DEFAULT_JUMPING_HEIGHT / 2;
		}
		y_end = platforms.back().y - max_distance;
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> x_distr(0, SCREEN_WIDTH - Platform::WIDTH);
	std::uniform_int_distribution<> y_distr(y_end, y_start);

	platforms.push_back(Platform(x_distr(gen), y_distr(gen)));
}
