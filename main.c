#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#define TITLE "Welcome to the Game of Life"

#define HEIGHT 20
#define WIDTH  40
#define DELAY 100000

void init_grid(int grid[HEIGHT][WIDTH])
{
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            grid[y][x] = rand() % 2;
        }
    }
}

void render_grid(int grid[HEIGHT][WIDTH])
{
    clear();
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[y][x] == 1) {
                mvprintw(y, x, "^");    // live cell
            } else {
                mvprintw(y, x, " ");    // dead cell
            }
        }
    }
    refresh();
}

int count_neighbors(int grid[HEIGHT][WIDTH], int y, int x)
{
    int count = 0;
    // Check all eight neighbors
    for (int dy = -1; dy < 2; dy++) {
        for (int dx = -1; dx < 2; dx++) {
            if (dx == 0 && dy == 0) continue; // exclude current cell to the count

            int ny = y + dy;
            int nx = x + dx;

            // wrap arounds
            if (nx < 0) nx = WIDTH - 1;
            if (ny < 0) ny = HEIGHT - 1;
            if (nx >= WIDTH) nx = 0;
            if (ny >= HEIGHT) ny = 0;
            count += grid[nx][ny];

            // no wrap arounds
            //if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) count += grid[nx][ny];
        }
    }
    return count;
}

void update_grid(int grid[HEIGHT][WIDTH])
{
    int temp_grid[HEIGHT][WIDTH] = {0};

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int neighbors = count_neighbors(grid, y, x);
            if (grid[y][x] == 1) { // cell is alive
                if (neighbors < 2 || neighbors > 3) { // cell dies
                    temp_grid[y][x] = 0;
                } else { // cell survives
                    temp_grid[y][x] = 1;
                }
            } else { // cell is dead
                if (neighbors == 3) { // cell becomes alive
                    temp_grid[y][x] = 1;
                }
            }
        }
    }

    // copy back to original grid
    memcpy(grid, temp_grid, sizeof(temp_grid));
}


int main() 
{
    bool running = true;
    int grid[HEIGHT][WIDTH] = {0};
    int screen_height;
    int screen_width;

    // Init window
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    timeout(FALSE);

    getmaxyx(stdscr, screen_height, screen_width);
    int c_row = screen_height / 2;
    int c_col = (screen_width - strlen(TITLE)) / 2;
    mvprintw(c_row, c_col, TITLE);
    refresh();

    sleep(1);
    // Initialize grid with random values
    srand(time(NULL));
    init_grid(grid);
    
    while (running) {
        render_grid(grid);

        update_grid(grid);
        usleep(DELAY);
    }

    getch();
    // Close window
    endwin(); 

    return 0;
}
