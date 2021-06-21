#include "imports.h"

int main(int argc, char* argv[]) {

    GuiManager guiManager;
    guiManager.setFps(30);
    guiManager.runMainLoop();
    return 0;
}

