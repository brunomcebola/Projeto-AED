#ifndef game_included
#define game_included

void initBoard(void);

void setBoardSum(int);
void setBoardRows(int);
void setBoardColumns(int);
void setBoardMode(char);
void setBoardCoordinates(int, int);
void setBoardElRows(int *);
void setBoardElColumns(int *);
void setBoardLayout(char *);
void setBoardAnswer(int);

int getBoardSum(void);
int getBoardRows(void);
int getBoardColumns(void);
char getBoardMode(void);
int getBoardElRow(int);
int getBoardElColumn(int);
char getBoardLayoutElement(int, int);
char *getBoardLayout(void);
int getBoardAnswer(void);
int getBoardCoordinateX(void);
int getBoardCoordinateY(void);

void selMode(void);

void freeBoard(void);
void freeC(void);

#endif
