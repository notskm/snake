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

enum class direction { up, down, left, right };

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

void append_snake_unit(std::list<sf::RectangleShape>& snake)
{
	snake.emplace_back(sf::Vector2f{cell_size_real, cell_size_real});
	snake.back().setFillColor(sf::Color::Green);
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
	append_snake_unit(snake);

	direction snake_move_direction{direction::right};

	sf::Clock clock;
	sf::Time time_since_update{sf::Time::Zero};
	const sf::Time tick{sf::seconds(1.f / 15.f)};

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

		time_since_update += clock.restart();

		if (time_since_update >= tick) {
			time_since_update = sf::Time::Zero;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				if (snake_move_direction != direction::down) {
					snake_move_direction = direction::up;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				if (snake_move_direction != direction::up) {
					snake_move_direction = direction::down;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				if (snake_move_direction != direction::right) {
					snake_move_direction = direction::left;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				if (snake_move_direction != direction::left) {
					snake_move_direction = direction::right;
				}
			}

			snake.back().setPosition(snake.front().getPosition());
			snake.splice(snake.begin(), snake, std::prev(snake.end()));

			switch (snake_move_direction) {
			case direction::up:
				snake.front().move(0.f, -cell_size_real);
				break;
			case direction::down:
				snake.front().move(0.f, cell_size_real);
				break;
			case direction::left:
				snake.front().move(-cell_size_real, 0.f);
				break;
			case direction::right:
				snake.front().move(cell_size_real, 0.f);
				break;
			default:
				break;
			}

			if (snake.front().getPosition() == apple.getPosition()) {
				append_snake_unit(snake);
				snake.back().setPosition(snake.front().getPosition());
				apple.setPosition(random_cell_position(window.getSize()));
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
