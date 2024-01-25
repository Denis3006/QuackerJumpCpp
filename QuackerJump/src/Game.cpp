#include "Game.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include <cmath>
#include <random>
#include <iostream>

Game::Game() : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Quacker Jump"), 
			   player(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 3 * Player::SIZE),
			   score(0)
{

	if (!score_font.loadFromFile("Arial.ttf"))
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

	if (game_over()) {
		// draw game over text
		sf::Text game_over_text;
		game_over_text.setFont(score_font);
		game_over_text.setCharacterSize(120);
		game_over_text.setFillColor(sf::Color::Cyan);
		game_over_text.setStyle(sf::Text::Bold);
		game_over_text.setString("Game Over!");
		//center text
		sf::FloatRect textRect = game_over_text.getLocalBounds();
		game_over_text.setOrigin(textRect.left + textRect.width / 2.0f,
			textRect.top + textRect.height / 2.0f);
		game_over_text.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
		window.draw(game_over_text);
	}
	
	// draw score text
	sf::Text score_text;
	score_text.setFont(score_font);
	score_text.setCharacterSize(24);
	score_text.setFillColor(sf::Color::Red);
	score_text.setStyle(sf::Text::Bold);
	score_text.setString("Score: " + std::to_string(score));
	window.draw(score_text);

	// draw player
	sf::RectangleShape player_model(sf::Vector2f(Player::SIZE, Player::SIZE));
	player_model.setFillColor(sf::Color::Magenta);
	player_model.setPosition(player.left_border(), player.top_border());
	window.draw(player_model);
	
	// draw platforms
	for (const auto& platform : platforms) {
		sf::RectangleShape plaftorm_model(sf::Vector2f(platform.WIDTH, platform.HEIGHT));
		switch (platform.get_type()) {
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
		plaftorm_model.setPosition(platform.left_border(), platform.top_border());
		window.draw(plaftorm_model);
	}
	window.display();
}

bool Game::game_over()
{
	return player.top_border() > SCREEN_HEIGHT;
}

bool Game::running()
{
	return window.isOpen();
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
				v = V_0;
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
	if (!game_over()) {
		int dx = 0;
		auto player_platform = player_on_platform();

		if (moving_left != moving_right) {
			dx = moving_right ? V_HORIZONTAL : -V_HORIZONTAL;
		}
		if (player_platform != nullptr && v <= 0) {  // on platform and not already jumping - start the jump
			v = V_0;
			sum = 0;
			n = 0;
			if (player_platform->get_type() == PlatformType::SLOWING) {
				v = v0_slowed();
			}
			if (player_platform->get_type() == PlatformType::FRAGILE) {
				delete_platform(player_platform);
			}
		}
		else {
			v -= GRAVITY;
		}

		if (player.top_border() < int(SCREEN_HEIGHT / 2)) {
			double delta = int(SCREEN_HEIGHT / 2) - player.top_border();
			score += delta / 10;
			scroll_platforms(delta);
			player.move(0, delta);
		}
		// limit player movement to stay inbound 
		if (player.left_border() + dx < 0) {
			dx = player.left_border();
		}
		if (player.right_border() + dx > SCREEN_WIDTH) {
			dx = SCREEN_WIDTH - player.right_border();
		}
		player.move(dx, int(-v));
		handle_moving_platforms();
		while (new_platforms_needed()) {
			create_random_platform();
		}
		sum += v;
		n++;
		std::cout << n << ", " << v << ", " << sum << ", " << jumping_height(v) << std::endl;
	}
}

const Platform* Game::player_on_platform() const
{
	for (const auto& platform : platforms) {
		if (platform.player_on_platform(player, v)) {
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

double Game::jumping_height(double v_0) const
{
	double n = v_0 / GRAVITY;
	return n / 2 * (2 * v_0 - GRAVITY * (n - 1));
}

double Game::jumping_distance(double v_0) const
{
	return int(v_0 / GRAVITY) * V_HORIZONTAL;
}

double Game::v0_slowed() const
{
	// find v0_slowed such that jumping_height(V0_slowed) = jumping_height(V_0) / 2 = h
	// h = jumping_height(V0_slowed) = V0_slowed / (GRAVITY * 2) * (2 * V0_slowed - GRAVITY * (V0_slowed / GRAVITY - 1)) 
	// h = V0_slowed / (GRAVITY * 2) * (V0_slowed - GRAVITY)
	// h * 2 * GRAVITY = V0_slowed * V0_slowed - GRAVITY * V0_slowed
	// (V0_slowed)^2 - GRAVITY * V0_slowed - h * 2 * G = 0
	// discriminant = G^2 + 4 * h * 2 * GRAVITY =  G^2 + 8 * h * GRAVITY
	// V = (GRAVITY + sqrt(discriminant)) / 2

	double h = jumping_height(V_0) / 2;
	double discriminant = GRAVITY * GRAVITY + 8 * h * GRAVITY;
	return (GRAVITY + sqrt(discriminant)) / 2;
}

bool Game::new_platforms_needed() const
{
	if (platforms.empty()) {
		return true;
	}
	else {
		return platforms.back().bottom_border() > 0;
	}
}

void Game::handle_moving_platforms() {
	for (int i = 0; i < platforms.size(); i++) {
		if (platforms[i].get_type() == PlatformType::MOVING) {
			if (platforms[i].right_border() + platforms[i].get_speed() > SCREEN_WIDTH ||
				platforms[i].left_border() + platforms[i].get_speed() < 0) {
				platforms[i].swap_movement_direction();
			}
			platforms[i].move(platforms[i].get_speed(), 0);
		}
	}
}

void Game::scroll_platforms(int dy)
{
	for (int i = 0; i < platforms.size(); i++) {
		platforms[i].move(0, dy);
	}
	platforms.erase(std::remove_if(platforms.begin(), platforms.end(),
		[](Platform platform) { return platform.top_border() > SCREEN_HEIGHT; }), platforms.end());
}

void Game::create_random_platform()
{
	int y_end = SCREEN_HEIGHT;
	int y_start = y_end - floor(jumping_height(V_0));
	int x_start = 0;
	int x_end = SCREEN_WIDTH - Platform::WIDTH;
	if (!platforms.empty()) {
		const auto& last_created_platform = platforms.back();
		double v = last_created_platform.get_type() == PlatformType::SLOWING ? v0_slowed() : V_0;
		int y_distance = floor(jumping_height(v));
		y_start = platforms.back().top_border() - y_distance;
		y_end = last_created_platform.top_border() - 2 * Platform::HEIGHT;
		if (last_created_platform.get_type() != PlatformType::MOVING) {
			int x_distance = floor(jumping_distance(v));
			x_start = std::max(platforms.back().left_border() - x_distance, 0);
			x_end = std::min(platforms.back().right_border() + x_distance, SCREEN_WIDTH - Platform::WIDTH);
		}
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> x_distr(x_start, x_end);
	std::uniform_int_distribution<> y_distr(y_start, y_end);

	platforms.push_back(Platform(x_distr(gen), y_distr(gen)));
}