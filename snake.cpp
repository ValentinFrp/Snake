#include <SFML/Graphics.hpp>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include "snake.h"

class Snake {
public:
    Snake() {
        reset();
    }

    void reset() {
        segments.clear();
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

    bool checkCollision() {
        SnakeSegment head = segments.front();
        if (head.x < 0 || head.x >= GAME_WIDTH || head.y < 0 || head.y >= GAME_HEIGHT)
            return true;
        for (size_t i = 1; i < segments.size(); ++i) {
            if (head.x == segments[i].x && head.y == segments[i].y)
                return true;
        }
        return false;
    }

    std::vector<SnakeSegment> segments;
    Direction direction;
};

class Game {
public:
    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake Game"), score(0), gameOver(false), frameRate(10) {
        window.setFramerateLimit(frameRate);
        srand(time(0));

        if (!font.loadFromFile("arial.ttf")) {
            printf("Error loading font\n");
        }
        if (!snakeTexture.loadFromFile("snake.png")) {
            printf("Error loading snake texture\n");
        }
        if (!foodTexture.loadFromFile("food.png")) {
            printf("Error loading food texture\n");
        }
        spawnFood();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            if (!gameOver) {
                update();
            }
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
                    case sf::Keyboard::R:
                        if (gameOver) {
                            gameOver = false;
                            score = 0;
                            frameRate = 10;
                            snake.reset();
                            spawnFood();
                            window.setFramerateLimit(frameRate);
                        }
                        break;
                    default: break;
                }
            }
        }
    }

    void update() {
        snake.move();
        if (snake.segments.front().x == food.x && snake.segments.front().y == food.y) {
            snake.grow();
            spawnFood();
            score++;
            frameRate++;
            window.setFramerateLimit(frameRate);
        }
        if (snake.checkCollision()) {
            gameOver = true;
        }
    }

    void render() {
        window.clear();
        for (size_t i = 0; i < snake.segments.size(); ++i) {
            sf::Sprite sprite;
            sprite.setTexture(snakeTexture);
            sprite.setPosition(snake.segments[i].x * CELL_SIZE, snake.segments[i].y * CELL_SIZE);
            window.draw(sprite);
        }
        sf::Sprite foodSprite;
        foodSprite.setTexture(foodTexture);
        foodSprite.setPosition(food.x * CELL_SIZE, food.y * CELL_SIZE);
        window.draw(foodSprite);

        if (gameOver) {
            sf::Text text("Game Over! Press 'R' to Restart", font, 20);
            text.setFillColor(sf::Color::White);
            text.setPosition(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2);
            window.draw(text);
        } else {
            sf::Text text("Score: " + std::to_string(score), font, 20);
            text.setFillColor(sf::Color::White);
            text.setPosition(10, 10);
            window.draw(text);
        }

        window.display();
    }

    void spawnFood() {
        food.x = rand() % GAME_WIDTH;
        food.y = rand() % GAME_HEIGHT;
    }

    sf::RenderWindow window;
    Snake snake;
    sf::Vector2i food;
    sf::Font font;
    sf::Texture snakeTexture;
    sf::Texture foodTexture;
    int score;
    bool gameOver;
    unsigned int frameRate;
};

int main() {
    Game game;
    game.run();
    return 0;
}
