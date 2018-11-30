/*
	Copyright (c) 2018, Notskm
	BSD 3-Clause License
	https://opensource.org/licenses/BSD-3-Clause
*/

#include <SFML/Graphics.hpp>
#include <cassert>
#include <gsl/gsl>
#include <list>
#include <random>
#include <type_traits>

constexpr int cell_size_int{8};
constexpr float cell_size_real{cell_size_int};

template <typename T>
[[nodiscard]] T random_number(T min, T max)
{
	static std::default_random_engine eng{std::random_device{}()};

	if constexpr (std::is_floating_point_v<T>) {
		std::uniform_real_distribution<T> dist(min, max);
		return dist(eng);
	}
	else if constexpr (std::is_integral_v<T>) {
		std::uniform_int_distribution<T> dist(min, max);
		return dist(eng);
	}
	else {
		static_assert(false, "T must be an integer or floating point number");
	}
}

[[nodiscard]] sf::Vector2f random_cell_position(sf::Vector2i bounds)
{
	return {random_number(0, bounds.x / cell_size_int - 1) * cell_size_real,
			random_number(0, bounds.y / cell_size_int - 1) * cell_size_real};
}

[[nodiscard]] sf::Vector2f random_cell_position(sf::Vector2u bounds)
{
	sf::Vector2i vec{gsl::narrow<int>(bounds.x), gsl::narrow<int>(bounds.y)};
	return random_cell_position(vec);
}

int main()
{
	sf::RenderWindow window{{640, 480}, "Snake", sf::Style::Close};
	window.setVerticalSyncEnabled(true);

	assert(cell_size_int > 0);
	assert(cell_size_int == cell_size_real);
	assert(window.getSize().x % cell_size_int == 0);
	assert(window.getSize().y % cell_size_int == 0);

	sf::RectangleShape apple{{cell_size_real, cell_size_real}};
	apple.setFillColor(sf::Color::Red);

	apple.setPosition(random_cell_position(window.getSize()));

	std::list<sf::RectangleShape> snake{};
	snake.emplace_back(sf::Vector2f{cell_size_real, cell_size_real});
	snake.back().setFillColor(sf::Color::Green);

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
		for (auto& i : snake) {
			window.draw(i);
		}
		window.display();
	}
}
