#include <stdlib.h>
#include <stdio.h>

#include "files.h"
#include "game.h"


int main(int argc, char const *argv[]) {
    int valid = 0;

    //check if two arguments are passed
    if (argc != 2) {
        exit(0);
    }

    initFile(argv[1]);
    initBoard();

    while(checkEOF()) {
        valid = readFile();
        if(valid) {
            selMode();
            writeFile();
        }
        freeBoard();
    }

    terminateFile();

    return 0;
}
