#define TERM_ROWS 24
#define TERM_COLS 80
#define NO_CHAR_RX_UART	-1

void TermInit();

void TermClear();

char TermGetChar();
char TermGetCharNotBlocking();

void TermPrint(char* str);

void TermMoveUp();
void TermMoveDown();
void TermMoveRight();
void TermMoveLeft();

void TermNewLine();

void TermMove(char row, char col);

void TermBell();
