#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "files.h"
#include "game.h"
#include "bundle.h"

#define MAX(a,b) (a > b ? a : b)

FILE *in_file = NULL;
FILE *out_file = NULL;

//INNER FUNCTIONS
void initFile(const char *file) {
    int size = strlen(file);
    char *file_name = NULL;

    if(strcmp(".camp0", file+size-6) != 0) {
        exit(0);
    }
    in_file = fopen(file, "r");
    checkNull(in_file);

    file_name = (char *) calloc((strlen(basename((char *)file))+2) , sizeof(char)) ;
    checkNull(file_name);
    strncpy(file_name, basename((char *)file), strlen(basename((char *)file))-6);

    out_file = fopen(strcat(file_name,".tents0"), "w");
    checkNull(out_file);

    free(file_name);
}

int readRowsAndColumns() {
    int linhas = 0, colunas = 0;

    //get number of rows and columns
    if(fscanf(in_file, "%d %d", &linhas , &colunas) != 2) {
        return 0;
    }
    setBoardRowsNColumns(linhas, colunas);
    return 1;
}

int readMode() {
    char mode = '\0';
    int x = 0, y = 0;

    //get test mode
    while(mode < 'A' || mode > 'Z'){
        if(fscanf(in_file, "%c", &mode) != 1 ) {
            return 0;
        }
    }

    setBoardMode(mode);

    if(mode != 'A' && mode != 'B' && mode != 'C') {
        setBoardAnswer(-1);
    }

    //get coordinates for test mode 'B'
    else if(mode == 'B') {
        if(fscanf(in_file, "%d %d", &x , &y) != 2) {
            return 0;
        }
        setBoardCoordinates(x, y);
        if(x >= getBoardRows() || y >= getBoardColumns() || x < 0 || y < 0) {
            setBoardAnswer(-1);
        }
    }

    return 1;
}

int readElRowsAndColumns() {
    int rows = getBoardRows(), columns = getBoardColumns(), sum_tents_row = 0,
        sum_tents_column = 0, el = 0, *el_linha = getBoardAllElRow(),
        *el_coluna = getBoardAllElColumn();
    char mode = getBoardMode();

    //get number of elements in each row
    for(int i = 0; i < rows; i++) {
        if(fscanf(in_file, "%d", &el) != 1 ) {
            return 0;
        }
        el_linha[i] = el;
        sum_tents_row += el;
    }

    //get number of elemets in each column
    for(int i = 0; i < columns; i++) {
        if(fscanf(in_file, "%d", &el) != 1 ) {
            return 0;
        }
        el_coluna[i] = el;
        sum_tents_column += el;
    }

    if(sum_tents_row != sum_tents_column && (mode == 'A' || mode == 'C')) {
        setBoardAnswer(2);
        return 1;
    }

    setBoardSum(sum_tents_row);

    return 1;
}

char readChar() {
    char c = '\0';
    while(c != 'A' && c != 'T' && c != '.'){
        if(fscanf(in_file, "%c", &c) != 1) {
            return '\0';
        }
    }

    return c;
}

void finishLayout() {
    int rows = getBoardRows(), columns = getBoardColumns();
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            readChar();
        }
    }
}

void maxSize() {
    int max = 0, linhas = 0, colunas = 0, mux = 0, c = 0,
        x = 0, aux = 0, max_row = 0, max_column = 0,
        *el_linha = NULL, *el_coluna = NULL;

    char mode = '\0', *tabuleiro = NULL;

    while(checkEOF()) {
        aux = fscanf(in_file, "%d %d", &linhas , &colunas);
        if(aux != 2){
            break;
        }

        while(mode < 'A' || mode > 'Z'){
            aux = fscanf(in_file, "%c", &mode);
        }

        if(mode == 'B') {
            aux = fscanf(in_file, "%d %d", &x, &x);
        }

        for(int i = 0; i < linhas; i++){
            aux = fscanf(in_file, "%d", &x);
        }
        for(int i = 0; i < colunas; i++){
            aux = fscanf(in_file, "%d", &x);
        }

        for(int i = 0; i < linhas; i++){
            for(int j = 0; j < colunas; j++){
                readChar();
            }
        }

        if(mode == 'C') {
            c = 1;
            mux = linhas * colunas;
            max = MAX(max,mux);
        }
        max_row = MAX(max_row, linhas);
        max_column = MAX(max_column, colunas);
        mode = '\0';
    }

    if(c == 1){
        tabuleiro = (char *) malloc(max * sizeof(char));
        checkNull(tabuleiro);
    }

    el_linha = (int *) malloc(max_row * sizeof(int));
    checkNull(el_linha);
    el_coluna = (int *) malloc(max_column * sizeof(int));
    checkNull(el_coluna);
    setBoardArrays(tabuleiro, el_linha, el_coluna);

}

int readLayout() {
    int sum_tents = 0, trees = 0;
    char *tabuleiro = getBoardLayout();
    char c = '\0';
    int linha_atual = 0, coluna_atual = 0,
        rows = getBoardRows(), columns = getBoardColumns(),
        tents_row = 0, *tents_column = NULL;

    if(getBoardMode() == 'C') {
        //get summation of tents in rows
        if(getBoardAnswer() == 2){
            return 1;
        }

        sum_tents = getBoardSum();

        //save the actual number of tents in each column
        tents_column = (int *) calloc((unsigned int)columns , sizeof(int));
        checkNull(tents_column);

        while(linha_atual != rows) {
            if(fscanf(in_file, "%c", &c) != 1) {
                return 0;
            }

            if(c == 'T' || c == 'A' || c == '.'){
                if (c == 'A') {
                    trees++;
                }
                else if(c == 'T') {
                    tents_column[coluna_atual]++;
                    tents_row++;
                }

                tabuleiro[linha_atual*columns+coluna_atual] = c;

                coluna_atual++;
                if(coluna_atual == columns) {
                    if(tents_row > getBoardElRow(linha_atual)) {
                        setBoardAnswer(2);
                    }
                    tents_row = 0;
                    linha_atual++;
                    coluna_atual = 0;
                }
            }
        }

        for(int j = 0; j < columns; j++) {
            if(tents_column[j] > getBoardElColumn(j)) {
                setBoardAnswer(2);
                break;
            }
        }
        if(sum_tents > trees) {
            setBoardAnswer(2);
            return 1;
        }
        free(tents_column);

    }

    return 1;
}



//OUTER FUNCTIONS
int readFile() {
    if(!readRowsAndColumns()) {
        return 0;
    }

    if(!readMode()) {
        return 0;
    }

    if(!readElRowsAndColumns()) {
        return 0;
    }

    if(!readLayout()){
        return 0;
    }

    return 1;
}

void writeFile () {
    char mode = getBoardMode();

    if(fprintf(out_file, "%d %d %c ", getBoardRows(), getBoardColumns(), mode) < 0) {
        exit(0);
    }
    if(mode == 'B') {
        if(fprintf(out_file, "%d %d ", getBoardCoordinateX(), getBoardCoordinateY()) < 0) {
            exit(0);
        }
    }
    if(fprintf(out_file, "%d\n\n", getBoardAnswer()) < 0) {
        exit(0);
    }

}

void begining(){
    fseek(in_file, 0, SEEK_SET) ;
}

int checkEOF(){
    char aux = '\0';
    int end = 0;
    while(fscanf(in_file,"%c",&aux) == 1){
        end = feof(in_file);
        if(end){
            return !end;
        }

        if(aux != '\n') {
            fseek(in_file, -1, SEEK_CUR);
            return 1;
        }
    }
    return 0;
}

void terminateFile() {
    if(fclose(in_file) != 0) {
        exit(0);
    }

    if(fclose(out_file) != 0) {
        exit(0);
    }

    freeBoard();
}