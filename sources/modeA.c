#include "../headers/modeA.h"
#include "../headers/game.h"
#include "../headers/files.h"

void modeA() {
    int tents_row = 0, tents_column = 0, trees = 0;

    //get summation of tents in rows
    for(int i=0; i<getBoardRows(); i++) {
        tents_row += getBoardElRow(i);
    }
    //get summation of tents in columns
    for(int j=0; j<getBoardColumns(); j++) {
        tents_column += getBoardElColumn(j);
    }
    //summation of tents in rows must be
    //equal to summation of tents in columns
    if(tents_row != tents_column) {
        finishLayout();
        return;
    }

    for(int i=0;i<getBoardRows();i++){
        for(int j=0;j<getBoardColumns();j++){
            if(readChar() == 'A') {
                trees++;
            }
        }
    }
    //number of trees must be greater or equal
    //to summation of tents in rows/columns
    if(tents_row > trees) {
        return;
    }

    //if all correct then answer is 1
    setBoardAnswer(1);
}
