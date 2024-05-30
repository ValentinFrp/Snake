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
