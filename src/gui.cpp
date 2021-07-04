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

/**
 * An initialization function for setting cells randomly on at start.
 * */
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

/**
 * Returns the state of a cell if its on or not for given coordinate.
 * */
int Visualizer::cellState(uint x, uint y) {
    unsigned char *cell_ptr = cells + (y * w) + x;
    return *cell_ptr & 0x01;
}


/**
 * The main function to generate the next generation
 * */
void Visualizer::nextGen() {
    unsigned int x, y, live_neighbours;
    unsigned char *cell_ptr;
    memcpy(temp_cells, cells, length);

    cell_ptr = temp_cells;

    for (int y = 0; y < h; y++) {
        x = 0;
        do {
            // skipping the dead cells
            while (*cell_ptr == 0) {
                cell_ptr++;
                if (++x >= w) goto NextRow;
            }
            live_neighbours = *cell_ptr >> 1;
            // If cell is on
            if (*cell_ptr & 0x01) {
                // If it will stay on
                if ((live_neighbours != 2) && (live_neighbours != 3)) {
                    clearCells(x, y);
                    Visualizer::drawCells(x, y, 0xFF, 0xFF, 0xFF);
                }
            } else {
                // cell will stay on in next generation
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


/**
 * @brief A function utility that draws the cells at x and y position for given color.
 * */
void Visualizer::drawCells(uint x, uint y, uint red, uint green, uint blue) {
    Uint8 *pixel_ptr = (Uint8 *) surface->pixels + (y * CELL_SIZE * SCREEN_WIDTH + x * CELL_SIZE) * 4;
    for (unsigned int i = 0; i < CELL_SIZE; i++) {
        for (unsigned int j = 0; j < CELL_SIZE; j++) {
            // Setting colors
            *(pixel_ptr + j * 4 + 0) = red;
            *(pixel_ptr + j * 4 + 1) = green;
            *(pixel_ptr + j * 4 + 2) = blue;
        }
        pixel_ptr += SCREEN_WIDTH * 4;  // Moving to next row of the pixel
    }
}

void Visualizer::setCells(uint x, uint y) {
    u_char *cell_ptr = cells + (y * w) + x;
    int xleft, xright, yabove, ybelow;

    *(cell_ptr) |= 0x01;  // Setting the first bit as 1, means setting the cell state as 1.
    if (x == 0)
        // If we are at the left border
        xleft = w - 1;
    else
        xleft = -1;

    if (x == (w - 1))
        // If we are at the right border
        xright = -(w - 1);
    else
        xright = +1;

    if (y == 0)
        // If we are at the top edge of the screen
        yabove = length - w;
    else
        yabove = -w;

    if (y == (h - 1))
        // If we are at the bottom edge of the screen
        ybelow = -(length - w);
    else
        ybelow = w;

    // Incrementing neighbour counts
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
    int xleft, xright, yabove, ybelow;

    *(cell_ptr) &= ~0x01; // Setting first bit as 0 to set the cell state as off.
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

    // Decrement neighbour count
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
    putlog.info("Initializing GUI manager...");
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
            "Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );
    surface = SDL_GetWindowSurface(window);
    putlog.info("GUI manager initialized successfully.");
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

void GuiManager::setFps(int fps) {
    if (fps > 120){
        putlog.warning("FPS must be less than or equal to 120");
        return;
    }
    FPS = fps;
}

void GuiManager::runMainLoop() {
    bool running = true;
    uint32_t current_framerate;

    // Setting visualizer
    putlog.debug("Setting up visualizer for algorithm.");
    Visualizer v = setVisualizer(surface, CELL_MAP_WIDTH, CELL_MAP_HEIGHT);
    putlog.debug("Initializing visualizer...");
    putlog.info("Current fps is set to : ", false);
    putlog.info(FPS, false, true);
    v.init();
    putlog.info("Running Mainloop for UI...");
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
    putlog.warning("Destroying UI window...");
    SDL_DestroyWindow(window);
    SDL_Quit();
    putlog.info("Program exits gracefully. :)");
}

















