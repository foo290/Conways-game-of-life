//
// Created by linuxpanda on 19/06/21.
//

#include <SDL2/SDL.h>
#include <ctime>
#include <memory>
#include <unistd.h>


// SDL window and surface
SDL_Window *window = NULL;
SDL_Surface *surface = NULL;

// width and the height of your cell in pixels
unsigned int CELL_SIZE = 6;
unsigned int CELL_MAP_WIDTH = 350;
unsigned int CELL_MAP_HEIGHT = 180;


unsigned int SCREEN_WIDTH = CELL_MAP_WIDTH * CELL_SIZE;
unsigned int SCREEN_HEIGHT = CELL_MAP_HEIGHT * CELL_SIZE;


void drawCells(unsigned int x, unsigned int y, unsigned int colour) {
    Uint8 *pixel_ptr = (Uint8 *) surface->pixels + (y * CELL_SIZE * SCREEN_WIDTH + x * CELL_SIZE) * 4;
    for (unsigned int i = 0; i < CELL_SIZE; i++) {
        for (unsigned int j = 0; j < CELL_SIZE; j++) {
            // Setting colors
            *(pixel_ptr + j * 4 + 0) = colour;
            *(pixel_ptr + j * 4 + 1) = colour;
            *(pixel_ptr + j * 4 + 2) = colour;
        }
        pixel_ptr += SCREEN_WIDTH * 4;
    }
}

class CellMap {
private:
    unsigned char *cells;
    unsigned char *temp_cells;
    unsigned int w, h;
    unsigned int length;
public:
    CellMap(unsigned int width, unsigned int height);

    ~CellMap();

    int cellState(unsigned int x, unsigned int y) {
        unsigned char *cell_ptr = cells + (y * w) + x;
        return *cell_ptr & 0x01;
    }

    void init() {
        unsigned int seed = (unsigned) time(nullptr);
        srand(seed);
        unsigned int x, y;
        for (int i = 0; i < length * 0.5; i++) {
            x = rand() % (w - 1);
            y = rand() % (h - 1);

            if (!cellState(x, y))
                setCells(x, y);
        }
    }

    void nextGen() {
        unsigned int x, y, live_neighbours;
        unsigned char *cell_ptr;
        memcpy(temp_cells, cells, length);

        cell_ptr = temp_cells;
        for (int y = 0; y < h; y++) {
            x = 0;
            do {
                // skipping
                while (*cell_ptr == 0) {
                    cell_ptr++;
                    if (++x >= w) goto NextRow;
                }
                live_neighbours = *cell_ptr >> 1;
                if (*cell_ptr & 0x01){
                    if ((live_neighbours !=2) && (live_neighbours!=3)){
                        clearCells(x, y);
                        drawCells(x, y, 0x00);
                    }
                }
                else{
                    if (live_neighbours==3){
                        setCells(x,y);
                        drawCells(x,y, 0xFF);
                    }
                }

                cell_ptr++;

            } while (++x < w);
            NextRow:;
        }
    }

    void setCells(unsigned int x, unsigned int y) {
        unsigned char *cell_ptr = cells + (y * w) + x;
        int xleft, xright, yabove, ybelow;

        *(cell_ptr) |= 0x01;
        if (x == 0)
            xleft = w - 1;
        else
            xleft = -1;

        if (x == (w - 1))
            xright = -(w - 1);
        else
            xright = +1;

        if (y == 0)
            yabove = length - w;
        else
            yabove = -w;

        if (y == (h - 1))
            ybelow = -(length - w);
        else
            ybelow = w;

        *(cell_ptr + yabove + xleft) += 0x02;
        *(cell_ptr + yabove) += 0x02;
        *(cell_ptr + yabove + xright) += 0x02;
        *(cell_ptr + xleft) += 0x02;
        *(cell_ptr + xright) += 0x02;
        *(cell_ptr + ybelow + xleft) += 0x02;
        *(cell_ptr + ybelow) += 0x02;
        *(cell_ptr + ybelow + xright) += 0x02;
    }

    void clearCells(unsigned int x, unsigned int y) {
        unsigned char *cell_ptr = cells + (y * w) + x;
        int xleft, xright, yabove, ybelow;

        *(cell_ptr) &= ~0x01;
        if (x == 0)
            xleft = w - 1;
        else
            xleft = -1;

        if (x == (w - 1))
            xright = -(w - 1);
        else
            xright = +1;

        if (y == 0)
            yabove = length - w;
        else
            yabove = -w;

        if (y == (h - 1))
            ybelow = -(length - w);
        else
            ybelow = w;

        *(cell_ptr + yabove + xleft) -= 0x02;
        *(cell_ptr + yabove) -= 0x02;
        *(cell_ptr + yabove + xright) -= 0x02;
        *(cell_ptr + xleft) -= 0x02;
        *(cell_ptr + xright) -= 0x02;
        *(cell_ptr + ybelow + xleft) -= 0x02;
        *(cell_ptr + ybelow) -= 0x02;
        *(cell_ptr + ybelow + xright) -= 0x02;
    }

};

CellMap::CellMap(unsigned int width, unsigned int height) : w(width), h(height) {
    length = w * h;
    cells = new unsigned char[length];
    temp_cells = new unsigned char[length];
    memset(cells, 0, length);
}

CellMap::~CellMap() {
    delete[] cells;
    delete[] temp_cells;
}



int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
            "Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    surface = SDL_GetWindowSurface(window);

    // SDL Event handler
    SDL_Event e;

    CellMap map(CELL_MAP_WIDTH, CELL_MAP_HEIGHT);
    map.init();

    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0)
            if (e.type == SDL_QUIT) quit = true;
        usleep(5000);
        map.nextGen();
        // Update frame buffer.
        SDL_UpdateWindowSurface(window);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

}