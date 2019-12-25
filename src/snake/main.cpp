#include <SFML/Graphics.hpp>
#include <algorithm>
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
	static_assert(
		std::is_floating_point_v<T> || std::is_integral_v<T>,
		"T must be an integer or floating point number.");

	static std::default_random_engine eng{std::random_device{}()};

	if constexpr (std::is_floating_point_v<T>) {
		std::uniform_real_distribution<T> dist(min, max);
		return dist(eng);
	}
	else if constexpr (std::is_integral_v<T>) {
		std::uniform_int_distribution<T> dist(min, max);
		return dist(eng);
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
	if (snake.size() > 1) {
		snake.back().setPosition(std::prev(snake.end(), 2)->getPosition());
	}
}

void reset(
	sf::RectangleShape& apple, std::list<sf::RectangleShape>& snake,
	direction& snake_move_direction, unsigned& score, sf::Text& score_text,
	sf::Vector2u bounds)
{
	apple.setPosition(random_cell_position(bounds));

	snake.clear();
	append_snake_unit(snake);
	snake.front().setPosition(
		bounds.x / 2.f, bounds.y / 2.f + cell_size_real * 5.f);
	snake_move_direction = direction::left;

	score = 0;
	score_text.setString("Score: " + std::to_string(score));
}

bool check_gameover(std::list<sf::RectangleShape>& snake, sf::Vector2u bounds)
{
	auto head_pos{snake.front().getPosition()};
	if (head_pos.x < 0.f || head_pos.y < 0.f ||
		head_pos.x > bounds.x - cell_size_int ||
		head_pos.y > bounds.y - cell_size_int) {
		return true;
	}

	// The snake can't collide with itself unless it has at least 5 cells
	if (snake.size() > 4) {
		// std::next skips the cells that the head cannot collide with
		return std::any_of(
			std::next(snake.begin(), 3), snake.end(),
			[&head_pos](sf::RectangleShape& cell) {
				return cell.getPosition() == head_pos;
			});
	}

	return false;
}

int main()
{
	sf::RenderWindow window{{640, 480}, "Snake", sf::Style::Close};
	window.setFramerateLimit(15);

	assert(cell_size_int > 0);
	assert(cell_size_int == cell_size_real);
	assert(window.getSize().x % cell_size_int == 0);
	assert(window.getSize().y % cell_size_int == 0);

	sf::RectangleShape apple{{cell_size_real, cell_size_real}};
	apple.setFillColor(sf::Color::Red);

	std::list<sf::RectangleShape> snake{};

	direction snake_move_direction;

	sf::Font font;
	font.loadFromFile("fonts/PressStart2P/PressStart2P-Regular.ttf");

	unsigned score{0};
	sf::Text score_text{"", font};
	score_text.setCharacterSize(12);

	sf::Text press_any_key_text{"Press any key to begin", font};
	press_any_key_text.setCharacterSize(16);
	press_any_key_text.setOrigin(
		press_any_key_text.getGlobalBounds().width / 2.f,
		press_any_key_text.getGlobalBounds().height / 2.f);
	press_any_key_text.setPosition(
		window.getSize().x / 2.f, window.getSize().y / 2.f);

	bool game_running{false};

	reset(
		apple, snake, snake_move_direction, score, score_text,
		window.getSize());

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
				else {
					if (!game_running) {
						game_running = true;
						reset(
							apple, snake, snake_move_direction, score,
							score_text, window.getSize());
					}
				}
				break;
			default:
				break;
			}
		}

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

		if (game_running) {
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
				apple.setPosition(random_cell_position(window.getSize()));
				score += 10;
				score_text.setString("Score: " + std::to_string(score));
			}

			game_running = !check_gameover(snake, window.getSize());
		}

		window.clear();
		window.draw(apple);
		for (auto& i : snake) {
			window.draw(i);
		}
		window.draw(score_text);
		if (!game_running) {
			window.draw(press_any_key_text);
		}
		window.display();
	}
}
