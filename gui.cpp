//
// Created by linuxpanda on 20/06/21.
//

#include "imports.h"

Visualizer::Visualizer(SDL_Surface *surface, uint width, uint height) : surface(surface), w(width), h(height) {
    length = w * h;
    cells = new unsigned char[length];
    temp_cells = new unsigned char[length];
    memset(cells, 0, length);
}

Visualizer::~Visualizer() {
    delete[] cells;
    delete[] temp_cells;
}

void Visualizer::init() {
    uint seed = (unsigned) time(nullptr);
    srand(seed);
    unsigned int x, y;
    for (int i = 0; i < length * 0.5; i++) {
        x = rand() % (w - 1);
        y = rand() % (h - 1);

        if (!cellState(x, y)) {
            setCells(x, y);
        }

    }
}

int Visualizer::cellState(uint x, uint y) {
    unsigned char *cell_ptr = cells + (y * w) + x;
    return *cell_ptr & 0x01;
}

void Visualizer::nextGen() {
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
            if (*cell_ptr & 0x01) {
                if ((live_neighbours != 2) && (live_neighbours != 3)) {
                    clearCells(x, y);
                    Visualizer::drawCells(x, y, 0xFF, 0xFF, 0xFF);
                }
            } else {
                if (live_neighbours == 3) {
                    setCells(x, y);
                    Visualizer::drawCells(x, y, 0x0, 0x0, 0x0);
                }
            }
            cell_ptr++;
        } while (++x < w);
        NextRow:;
    }
}

void Visualizer::drawCells(uint x, uint y, uint red, uint green, uint blue) {
    Uint8 *pixel_ptr = (Uint8 *) surface->pixels + (y * CELL_SIZE * SCREEN_WIDTH + x * CELL_SIZE) * 4;
    for (unsigned int i = 0; i < CELL_SIZE; i++) {
        for (unsigned int j = 0; j < CELL_SIZE; j++) {
            // Setting colors
            *(pixel_ptr + j * 4 + 0) = red;
            *(pixel_ptr + j * 4 + 1) = green;
            *(pixel_ptr + j * 4 + 2) = blue;
        }
        pixel_ptr += SCREEN_WIDTH * 4;
    }
}

void Visualizer::setCells(uint x, uint y) {
    u_char *cell_ptr = cells + (y * w) + x;
    uint xleft, xright, yabove, ybelow;

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

void Visualizer::clearCells(uint x, uint y) {
    u_char *cell_ptr = cells + (y * w) + x;
    uint xleft, xright, yabove, ybelow;

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


GuiManager::GuiManager() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
            "Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );
    surface = SDL_GetWindowSurface(window);
}

void GuiManager::capFrameRate(uint32_t currentFrameRate, uint fps) {
    if ((1000 / fps) > SDL_GetTicks() - currentFrameRate) {
        SDL_Delay(1000 / fps - (SDL_GetTicks() - currentFrameRate));
    }
}

SDL_Surface *GuiManager::getSurface() {
    return surface;
}

SDL_Window *GuiManager::getWindow() {
    return window;
}

Visualizer GuiManager::setVisualizer(SDL_Surface *s, uint mapWidth, uint mapHeight) {
    Visualizer v(s, mapWidth, mapHeight);
    return v;
}

void GuiManager::runMainLoop() {
    bool running = true;
    uint32_t current_framerate;

    // Setting visualizer
    Visualizer v = setVisualizer(surface, CELL_MAP_WIDTH, CELL_MAP_HEIGHT);
    v.init();

    while (running) {
        // Main window loop
        current_framerate = SDL_GetTicks();

        while (SDL_PollEvent(&event) != 0) {
            //event Monitor Loop
            if (event.type == SDL_QUIT) running = false;
        }
        v.nextGen();  // Generating next generation

        capFrameRate(current_framerate, FPS);  // Frame rate cap
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}

















