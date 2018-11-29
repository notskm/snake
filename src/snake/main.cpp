/*
	Copyright (c) 2018, Notskm
	BSD 3-Clause License
	https://opensource.org/licenses/BSD-3-Clause
*/

#include <SFML/Graphics.hpp>
#include <cassert>
#include <gsl/gsl>

constexpr float cell_size{8.f};

int main()
{
	sf::RenderWindow window{{640, 480}, "Snake", sf::Style::Close};
	window.setVerticalSyncEnabled(true);

	assert(window.getSize().x % gsl::narrow<int>(cell_size) == 0);
	assert(window.getSize().y % gsl::narrow<int>(cell_size) == 0);

	sf::RectangleShape apple{{cell_size, cell_size}};
	apple.setFillColor(sf::Color::Red);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				break;
			default:
				break;
			}
		}

		window.clear();
		window.draw(apple);
		window.display();
	}
}
