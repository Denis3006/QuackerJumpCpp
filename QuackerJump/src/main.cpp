#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include <SFML/Window/Event.hpp>
#include "Game.hpp"

int main() {
	Game game;

	sf::RectangleShape rect(sf::Vector2f(100, 100));
	rect.setFillColor(sf::Color::Cyan);
	int pos_x = 0, pos_y = 1000;
	rect.setPosition(pos_x, pos_y);
	while (game.running()) {
		game.get_user_input();
		game.update_game_state();
		game.draw_game();
	}

	return 0;
}