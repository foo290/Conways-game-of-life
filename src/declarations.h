//
// Created by linuxpanda on 20/06/21.
//
#include "Logger.h"

class Configs {
public:
    uint FPS = 30;  // This represents the spreed of the algorithm, default is 30 FPS

    // The three variables below will determine the screen size
    uint CELL_SIZE = 5;  // This represents the width and the height of a cell in pixels
    uint CELL_MAP_WIDTH = 300;
    uint CELL_MAP_HEIGHT = 150;

    // window width and height
    uint SCREEN_WIDTH = CELL_MAP_WIDTH * CELL_SIZE;
    uint SCREEN_HEIGHT = CELL_MAP_HEIGHT * CELL_SIZE;

    Logger putlog;  // For logging the necessary stuffs on console.
};

class Visualizer : public Configs {
private:
    u_char *cells;
    u_char *temp_cells;
    uint w, h;
    uint length;

    SDL_Surface *surface;

public:
    ~Visualizer();

    Visualizer(SDL_Surface *surface, uint width, uint height);

    void init();

    void nextGen();

    void setCells(uint x, uint y);

    void clearCells(uint x, uint y);

    int cellState(uint x, uint y);

    void drawCells(uint x, uint y, uint red, uint green, uint blue);
};


class GuiManager : public Configs {
private:
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Event event;
public:
    GuiManager();

    static void capFrameRate(uint32_t currentFrameRate, uint fps);

    SDL_Window *getWindow();

    SDL_Surface *getSurface();

    void setFps(int fps);

    void runMainLoop();

    static Visualizer setVisualizer(SDL_Surface *s, uint mapWidth, uint mapHeight);

};















