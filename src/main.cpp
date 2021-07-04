/**
 * This is a very efficient implementation of the algorithm game of life. This assumes you already have knowledge of c++
 * and familiar with conway's game of life.
 *
 * Instead of naively counting the no. of neighbouring cells for each cell in every generation,
 * each cell stores the no. of its neighbours.
 * For dead cells this number will be zero and for alive one's this will be 2.
 *
 * So whenever we set the cell as alive, we do two things :
 *  1. Set the cell as alive.
 *  2. Increase the living neighbour count of it's 8 neighbours.
 *
 * Then during calculating the next generation, we will iterate through each cell that either has neighbour or is alive
 * and check how many neighbours it has. This step doesnt requires calculation as each cell stores the number of their neighbours.
 * And because most of the cells will be dead initially, we dont even need to iterate over them, and as we go through
 * next generations, this no. will increase and we will be skipping more cells as they will be dead.
 * This will boost the program's speed drastically.
 *
 *
 *
 *                  -- THE DATA STRUCTURE FOR THE GRID --
 *
 * We know that we need to store two things for each cell :
 *  1. Is the cell alive or dead? (true / false)
 *  2. How many live neighbours (0-8)
 *
 * The state of a cell is just a boolean type, either 1 or 0, so we can store this in a single bit of memory.
 * The neighbour cells is an integer that can have a maximum value of 8, which needs 4 bits.
 * So we practically needs just 5 bits of the memory.
 *
 * In c++ we have char data type which is 8 bytes in memory and since char can store numbers, we can use some bitwise
 * manipulation to store our two variables inside one unsigned char. This way we are wasting only 3 bits of memory.
 *
 * Explanation :
 *
 *      In an unsigned char :
 *              indexes : |         0                 |  1     2     3     4    |   5 6 7     |
 *              values  : | The zeroth bit will store | The next four bits will | Unused bits |
 *                        | our cell state            | will store the no. of   |
 *                                                    | living neighbours       |
 *
 * */

#include "imports.h"

int main(int argc, char* argv[]) {

    GuiManager guiManager;
    guiManager.setFps(30);
    guiManager.runMainLoop();
    return 0;
}

