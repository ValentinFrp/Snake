#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 20;
const int GAME_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int GAME_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

enum Direction { Up, Down, Left, Right };

struct SnakeSegment {
    int x, y;
    SnakeSegment(int x, int y) : x(x), y(y) {}
};

class Snake {
public:
    Snake() {
        segments.push_back(SnakeSegment(GAME_WIDTH / 2, GAME_HEIGHT / 2));
        direction = Direction::Right;
    }

    void move() {
        SnakeSegment newHead = segments.front();
        switch (direction) {
            case Direction::Up: newHead.y -= 1; break;
            case Direction::Down: newHead.y += 1; break;
            case Direction::Left: newHead.x -= 1; break;
            case Direction::Right: newHead.x += 1; break;
        }
        segments.insert(segments.begin(), newHead);
        segments.pop_back();
    }

    void grow() {
        SnakeSegment newHead = segments.front();
        switch (direction) {
            case Direction::Up: newHead.y -= 1; break;
            case Direction::Down: newHead.y += 1; break;
            case Direction::Left: newHead.x -= 1; break;
            case Direction::Right: newHead.x += 1; break;
        }
        segments.insert(segments.begin(), newHead);
    }

    void setDirection(Direction dir) {
        if ((direction == Direction::Up && dir != Direction::Down) ||
            (direction == Direction::Down && dir != Direction::Up) ||
            (direction == Direction::Left && dir != Direction::Right) ||
            (direction == Direction::Right && dir != Direction::Left)) {
            direction = dir;
        }
    }

    std::vector<SnakeSegment> segments;
    Direction direction;
};

class Game {
public:
    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake Game") {
        window.setFramerateLimit(10);
        srand(time(0));
        spawnFood();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Up: snake.setDirection(Direction::Up); break;
                    case sf::Keyboard::Down: snake.setDirection(Direction::Down); break;
                    case sf::Keyboard::Left: snake.setDirection(Direction::Left); break;
                    case sf::Keyboard::Right: snake.setDirection(Direction::Right); break;
                }
            }
        }
    }

    void update() {
        snake.move();
        if (snake.segments.front().x == food.x && snake.segments.front().y == food.y) {
            snake.grow();
            spawnFood();
        }
    }

    void render() {
        window.clear();
        for (auto &segment : snake.segments) {
            sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            rectangle.setFillColor(sf::Color::Green);
            rectangle.setPosition(segment.x * CELL_SIZE, segment.y * CELL_SIZE);
            window.draw(rectangle);
        }
        sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        rectangle.setFillColor(sf::Color::Red);
        rectangle.setPosition(food.x * CELL_SIZE, food.y * CELL_SIZE);
        window.draw(rectangle);
        window.display();
    }

    void spawnFood() {
        food.x = rand() % GAME_WIDTH;
        food.y = rand() % GAME_HEIGHT;
    }

    sf::RenderWindow window;
    Snake snake;
    sf::Vector2i food;
};

int main() {
    Game game;
    game.run();
    return 0;
}
