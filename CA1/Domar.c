#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

#pragma warning(disable:4996)

//colors
#define RED 12
#define BLUE 3
#define GREEN 10
#define YELLOW 14
#define GRAY 8
#define PINK 13
#define WHITE 15
#define WAIT_TIME_MILI_SEC 100
//directions
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
// general
#define BOARD_SIZE 40
#define INITIAL_SNAKE_LENGTH 3
#define MINIMUM_SNAKE_LENGTH 2
#define MAX_LEN_SNAKES 30
#define NUMBER_OF_MOUSES 20
//board_characters
#define EMPTY '0'
#define MOUSE 'm'
#define PLAYER1_SNAKE_HEAD '1'
#define PLAYER2_SNAKE_HEAD '2'
#define PLAYER1_SNAKE_BODY 'a'
#define PLAYER2_SNAKE_BODY 'b'
//Bernard, Poison and golden star
#define BERNARD_CLOCK 'c' //on the board character
#define GOLDEN_STAR '*' //on the board character
#define POISON 'x' //on the board character
#define NUMBER_OF_POISONS 5
#define NUMBER_OF_GOLDEN_STARS 3
#define BERNARD_CLOCK_APPEARANCE_CHANCE_PERCENT 20
#define BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD_MILI_SEC 2000
#define BERNARD_CLOCK_FROZEN_TIME_MILI_SEC 4000
CONSOLE_FONT_INFOEX former_cfi;
CONSOLE_CURSOR_INFO former_info;
COORD former_screen_size;

void reset_console() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleDisplayMode(consoleHandle, CONSOLE_WINDOWED_MODE, &former_screen_size);
    SetCurrentConsoleFontEx(consoleHandle, FALSE, &former_cfi);
    SetConsoleCursorInfo(consoleHandle, &former_info);
}

void hidecursor(HANDLE consoleHandle)
{
    GetConsoleCursorInfo(consoleHandle, &former_info);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void set_console_font_and_font_size(HANDLE consoleHandle) {
    former_cfi.cbSize = sizeof(former_cfi);
    GetCurrentConsoleFontEx(consoleHandle, FALSE, &former_cfi);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 20;
    cfi.dwFontSize.Y = 20;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Courier");
    SetCurrentConsoleFontEx(consoleHandle, FALSE, &cfi);
}

void set_full_screen_mode(HANDLE consoleHandle) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    former_screen_size.X = csbi.dwSize.X; former_screen_size.Y = csbi.dwSize.Y;
    COORD coord;
    SetConsoleDisplayMode(consoleHandle, CONSOLE_FULLSCREEN_MODE, &coord);
}

void init_screen()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    set_full_screen_mode(consoleHandle);
    hidecursor(consoleHandle);
    set_console_font_and_font_size(consoleHandle);

}

void wait_and_get_direction(int* player1_snake_direction, int* player2_snake_direction) {
    DWORD64 start_time, check_time;
    start_time = GetTickCount64();
    check_time = start_time + WAIT_TIME_MILI_SEC; //GetTickCount returns time in miliseconds
    char key = 0;
    char player1_key_hit = 0;
    char player2_key_hit = 0;

    while (check_time > GetTickCount64()) {
        if (_kbhit()) {
            key = _getch();
            if (key == 0)
                key = _getch();
            if (key == 'w' || key == 'a' || key == 's' || key == 'd')
                player1_key_hit = key;
            if (key == 'i' || key == 'j' || key == 'k' || key == 'l')
                player2_key_hit = key;
        }
    }

    switch (player1_key_hit) {
    case 'w': if (*player1_snake_direction != DOWN) *player1_snake_direction = UP; break;
    case 'a': if (*player1_snake_direction != RIGHT) *player1_snake_direction = LEFT; break;
    case 's': if (*player1_snake_direction != UP) *player1_snake_direction = DOWN; break;
    case 'd': if (*player1_snake_direction != LEFT) *player1_snake_direction = RIGHT; break;
    default: break;
    }

    switch (player2_key_hit) {
    case 'i': if (*player2_snake_direction != DOWN) *player2_snake_direction = UP; break;
    case 'j': if (*player2_snake_direction != RIGHT) *player2_snake_direction = LEFT; break;
    case 'k': if (*player2_snake_direction != UP) *player2_snake_direction = DOWN; break;
    case 'l': if (*player2_snake_direction != LEFT) *player2_snake_direction = RIGHT; break;
    default: break;
    }
}

void draw_point(char point_content) {
    switch (point_content) {
    case PLAYER1_SNAKE_HEAD: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED); printf("@"); break;
    case PLAYER2_SNAKE_HEAD: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);  printf("@"); break;
    case PLAYER1_SNAKE_BODY: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);  printf("o"); break;
    case PLAYER2_SNAKE_BODY: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);  printf("o"); break;
    case MOUSE: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GRAY); printf("m"); break;
    case GOLDEN_STAR: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW); printf("*"); break;
    case POISON: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN); printf("x"); break;
    case BERNARD_CLOCK: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PINK); printf("c"); break;
    default: printf(" ");
    }
}

void draw_horizonatal_walls() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
    for (int i = 0; i < BOARD_SIZE + 2; ++i)
        printf("-");
    printf("\n");
}

void draw_board(char board_content[BOARD_SIZE][BOARD_SIZE]) {
    system("cls");
    draw_horizonatal_walls();
    for (int i = 0; i < BOARD_SIZE; i++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        printf("|"); // vertical wall 
        for (int j = 0; j < BOARD_SIZE; j++)
            draw_point(board_content[i][j]);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        printf("|\n"); // vertical wall
    }
    draw_horizonatal_walls();
}

// prototypes
void init_screen();
void reset_console();
void wait_and_get_direction(int* player1_snake_direction, int* player2_snake_direction);
void draw_board(char board_content[BOARD_SIZE][BOARD_SIZE]);
void completing_mouses(int mouse_position[NUMBER_OF_MOUSES][2], char board_content[BOARD_SIZE][BOARD_SIZE], int i);
void completing_golden_stars(int golden_star_position[NUMBER_OF_GOLDEN_STARS][2], char board_content[BOARD_SIZE][BOARD_SIZE], int i);

int main() {



    int player1_snake_position[MAX_LEN_SNAKES][2];                           //to save different parts of the first snake's body's position
    int player2_snake_position[MAX_LEN_SNAKES][2];                           //to save different parts of the second snake's body's position
    int mouse_position[NUMBER_OF_MOUSES][2];                                 //to save the mouses' position
    int golden_star_position[NUMBER_OF_GOLDEN_STARS][2];                     //to save the golden stars' position
    int player1_snake_length = INITIAL_SNAKE_LENGTH;
    int player2_snake_length = INITIAL_SNAKE_LENGTH;
    char board_content[BOARD_SIZE][BOARD_SIZE];
    int player1_snake_direction = DOWN; int player2_snake_direction = UP;    //their directions when we start the game

    init_screen();


    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++)
            board_content[x][y] = EMPTY;
    }

    /****************************************************************************************************************************************************************************************************************************************************************************************************************/

    for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++) {
        player1_snake_position[i][1] = 0;
        player1_snake_position[i][0] = (INITIAL_SNAKE_LENGTH - 1) - i;
        player2_snake_position[i][1] = BOARD_SIZE - 1;
        player2_snake_position[i][0] = BOARD_SIZE - (INITIAL_SNAKE_LENGTH - i);
        //first coordinates of the snakes
        if (i == 0)
            board_content[player1_snake_position[i][0]][player1_snake_position[i][1]] = PLAYER1_SNAKE_HEAD;
        else
            board_content[player1_snake_position[i][0]][player1_snake_position[i][1]] = PLAYER1_SNAKE_BODY;
        if (i == 0)
            board_content[player2_snake_position[i][0]][player2_snake_position[i][1]] = PLAYER2_SNAKE_HEAD;
        else
            board_content[player2_snake_position[i][0]][player2_snake_position[i][1]] = PLAYER2_SNAKE_BODY;
    }

    //first size of their bodies(whithout calculating heads)
    int SNAKE1_BODY_LEN = INITIAL_SNAKE_LENGTH - 1;
    int SNAKE2_BODY_LEN = INITIAL_SNAKE_LENGTH - 1;

    //creating mouses with random numbers
    srand(time(NULL));
    int i = 0;
    while (i < NUMBER_OF_MOUSES) {
        mouse_position[i][0] = rand() % BOARD_SIZE;
        mouse_position[i][1] = rand() % BOARD_SIZE;
        if (board_content[mouse_position[i][0]][mouse_position[i][1]] == EMPTY) {
            board_content[mouse_position[i][0]][mouse_position[i][1]] = MOUSE;
            i++;
        }
        else continue;
    }

    //creating golden stars with random numbers
    int j = 0;
    while (j < NUMBER_OF_GOLDEN_STARS) {
        golden_star_position[j][0] = rand() % BOARD_SIZE;
        golden_star_position[j][1] = rand() % BOARD_SIZE;
        if (board_content[golden_star_position[j][0]][golden_star_position[j][1]] == EMPTY) {
            board_content[golden_star_position[j][0]][golden_star_position[j][1]] = GOLDEN_STAR;
            j++;
        }
        else continue;
    }


    /****************************************************************************************************************************************************************************************************************************************************************************************************************/
    while (TRUE) {

        int END_GAME = 1;




        draw_board(board_content);
        wait_and_get_direction(&player1_snake_direction, &player2_snake_direction);

        int i_snake_tail1 = player1_snake_position[SNAKE1_BODY_LEN][1];       //to save snake's tails
        int j_snake_tail1 = player1_snake_position[SNAKE1_BODY_LEN][0];
        int i_snake_tail2 = player2_snake_position[SNAKE2_BODY_LEN][1];
        int j_snake_tail2 = player2_snake_position[SNAKE2_BODY_LEN][0];
        int body1, body2;
        /****************************************************************************************************************************************************************************************************************************************************************************************************************/

        for (int i = 0; i < NUMBER_OF_GOLDEN_STARS; i++) {
            if (player1_snake_position[0][0] == golden_star_position[i][0] && player1_snake_position[0][1] == golden_star_position[i][1]) {             //when first player's snake eats a golden star
                completing_golden_stars(golden_star_position, board_content, i);
                board_content[player2_snake_position[SNAKE2_BODY_LEN][0]][player2_snake_position[SNAKE2_BODY_LEN][1]] = EMPTY;
                SNAKE2_BODY_LEN -= 1;
            }

        }

        for (int i = 0; i < NUMBER_OF_GOLDEN_STARS; i++) {
            if (player2_snake_position[0][0] == golden_star_position[i][0] && player2_snake_position[0][1] == golden_star_position[i][1]) {             //when second player's snake eats a golden star
                completing_golden_stars(golden_star_position, board_content, i);
                board_content[player1_snake_position[SNAKE1_BODY_LEN][0]][player1_snake_position[SNAKE1_BODY_LEN][1]] = EMPTY;
                SNAKE1_BODY_LEN -= 1;
            }

        }

        /****************************************************************************************************************************************************************************************************************************************************************************************************************/
        //first player's snake's move
        if (player1_snake_direction == DOWN) {

            for (body1 = SNAKE1_BODY_LEN; body1 >= 1; body1--) {
                if (body1 == SNAKE1_BODY_LEN) {
                    board_content[player1_snake_position[body1][0]][player1_snake_position[body1][1]] = EMPTY;
                }

                player1_snake_position[body1][0] = player1_snake_position[body1 - 1][0];    //moving snake's body
                player1_snake_position[body1][1] = player1_snake_position[body1 - 1][1];

            }
            for (body1 = SNAKE1_BODY_LEN; body1 >= 1; body1--)
                board_content[player1_snake_position[body1][0]][player1_snake_position[body1][1]] = PLAYER1_SNAKE_BODY;

            player1_snake_position[0][0] += 1;                                              //moving snake's head
            if (player1_snake_position[0][0] >= BOARD_SIZE) {
                player1_snake_position[0][0] = 0;
            }

            board_content[player1_snake_position[0][0]][player1_snake_position[0][1]] = PLAYER1_SNAKE_HEAD;


        }

        if (player1_snake_direction == UP) {

            for (body1 = SNAKE1_BODY_LEN; body1 >= 1; body1--) {
                if (body1 == SNAKE1_BODY_LEN) {
                    board_content[player1_snake_position[body1][0]][player1_snake_position[body1][1]] = EMPTY;
                }

                player1_snake_position[body1][0] = player1_snake_position[body1 - 1][0];
                player1_snake_position[body1][1] = player1_snake_position[body1 - 1][1];

            }
            for (body1 = SNAKE1_BODY_LEN; body1 >= 1; body1--)
                board_content[player1_snake_position[body1][0]][player1_snake_position[body1][1]] = PLAYER1_SNAKE_BODY;

            player1_snake_position[0][0] -= 1;
            if (player1_snake_position[0][0] < 0) {
                player1_snake_position[0][0] = BOARD_SIZE - 1;
            }
            board_content[player1_snake_position[0][0]][player1_snake_position[0][1]] = PLAYER1_SNAKE_HEAD;

        }

        if (player1_snake_direction == LEFT) {
            for (body1 = SNAKE1_BODY_LEN; body1 >= 1; body1--) {
                if (body1 == SNAKE1_BODY_LEN) {
                    board_content[player1_snake_position[body1][0]][player1_snake_position[body1][1]] = EMPTY;
                }

                player1_snake_position[body1][0] = player1_snake_position[body1 - 1][0];
                player1_snake_position[body1][1] = player1_snake_position[body1 - 1][1];

            }
            for (body1 = SNAKE1_BODY_LEN; body1 >= 1; body1--)
                board_content[player1_snake_position[body1][0]][player1_snake_position[body1][1]] = PLAYER1_SNAKE_BODY;

            player1_snake_position[0][1] -= 1;
            if (player1_snake_position[0][1] < 0) {
                player1_snake_position[0][1] = BOARD_SIZE - 1;
            }
            board_content[player1_snake_position[0][0]][player1_snake_position[0][1]] = PLAYER1_SNAKE_HEAD;


        }

        if (player1_snake_direction == RIGHT) {
            for (body1 = SNAKE1_BODY_LEN; body1 >= 1; body1--) {
                if (body1 == SNAKE1_BODY_LEN) {
                    board_content[player1_snake_position[body1][0]][player1_snake_position[body1][1]] = EMPTY;
                }
                player1_snake_position[body1][0] = player1_snake_position[body1 - 1][0];
                player1_snake_position[body1][1] = player1_snake_position[body1 - 1][1];

            }
            for (body1 = SNAKE1_BODY_LEN; body1 >= 1; body1--)
                board_content[player1_snake_position[body1][0]][player1_snake_position[body1][1]] = PLAYER1_SNAKE_BODY;

            player1_snake_position[0][1] += 1;
            if (player1_snake_position[0][1] >= BOARD_SIZE) {
                player1_snake_position[0][1] = 0;
            }
            board_content[player1_snake_position[0][0]][player1_snake_position[0][1]] = PLAYER1_SNAKE_HEAD;
        }

        /****************************************************************************************************************************************************************************************************************************************************************************************************************/
        for (int i = 0; i < NUMBER_OF_MOUSES; i++) {
            if (player1_snake_position[0][0] == mouse_position[i][0] && player1_snake_position[0][1] == mouse_position[i][1]) {             //when first player's snake eats a mouse
                completing_mouses(mouse_position, board_content, i);
                if (SNAKE1_BODY_LEN < MAX_LEN_SNAKES - 1) {
                    SNAKE1_BODY_LEN += 1;
                    player1_snake_position[SNAKE1_BODY_LEN][0] = j_snake_tail1;
                    player1_snake_position[SNAKE1_BODY_LEN][1] = i_snake_tail1;
                }
                else break;
            }

        }

        /****************************************************************************************************************************************************************************************************************************************************************************************************************/
        //second player's snake's move
        if (player2_snake_direction == DOWN) {

            for (body2 = SNAKE2_BODY_LEN; body2 >= 1; body2--) {
                if (body2 == SNAKE2_BODY_LEN) {
                    board_content[player2_snake_position[body2][0]][player2_snake_position[body2][1]] = EMPTY;
                }

                player2_snake_position[body2][0] = player2_snake_position[body2 - 1][0];
                player2_snake_position[body2][1] = player2_snake_position[body2 - 1][1];

            }
            for (body2 = SNAKE2_BODY_LEN; body2 >= 1; body2--)
                board_content[player2_snake_position[body2][0]][player2_snake_position[body2][1]] = PLAYER2_SNAKE_BODY;

            player2_snake_position[0][0] += 1;
            if (player2_snake_position[0][0] >= BOARD_SIZE) {
                player2_snake_position[0][0] = 0;
            }
            board_content[player2_snake_position[0][0]][player2_snake_position[0][1]] = PLAYER2_SNAKE_HEAD;

        }

        if (player2_snake_direction == UP) {

            for (body2 = SNAKE2_BODY_LEN; body2 >= 1; body2--) {
                if (body2 == SNAKE2_BODY_LEN) {
                    board_content[player2_snake_position[body2][0]][player2_snake_position[body2][1]] = EMPTY;
                }

                player2_snake_position[body2][0] = player2_snake_position[body2 - 1][0];
                player2_snake_position[body2][1] = player2_snake_position[body2 - 1][1];

            }
            for (body2 = SNAKE2_BODY_LEN; body2 >= 1; body2--)
                board_content[player2_snake_position[body2][0]][player2_snake_position[body2][1]] = PLAYER2_SNAKE_BODY;

            player2_snake_position[0][0] -= 1;
            if (player2_snake_position[0][0] < 0) {
                player2_snake_position[0][0] = BOARD_SIZE - 1;
            }
            board_content[player2_snake_position[0][0]][player2_snake_position[0][1]] = PLAYER2_SNAKE_HEAD;

        }

        if (player2_snake_direction == LEFT) {
            for (body2 = SNAKE2_BODY_LEN; body2 >= 1; body2--) {
                if (body2 == SNAKE2_BODY_LEN) {
                    board_content[player2_snake_position[body2][0]][player2_snake_position[body2][1]] = EMPTY;
                }

                player2_snake_position[body2][0] = player2_snake_position[body2 - 1][0];
                player2_snake_position[body2][1] = player2_snake_position[body2 - 1][1];

            }
            for (body2 = SNAKE2_BODY_LEN; body2 >= 1; body2--)
                board_content[player2_snake_position[body2][0]][player2_snake_position[body2][1]] = PLAYER2_SNAKE_BODY;

            player2_snake_position[0][1] -= 1;
            if (player2_snake_position[0][1] < 0) {
                player2_snake_position[0][1] = BOARD_SIZE - 1;
            }
            board_content[player2_snake_position[0][0]][player2_snake_position[0][1]] = PLAYER2_SNAKE_HEAD;


        }

        if (player2_snake_direction == RIGHT) {
            for (body2 = SNAKE2_BODY_LEN; body2 >= 1; body2--) {
                if (body2 == SNAKE2_BODY_LEN) {
                    board_content[player2_snake_position[body2][0]][player2_snake_position[body2][1]] = EMPTY;
                }
                player2_snake_position[body2][0] = player2_snake_position[body2 - 1][0];
                player2_snake_position[body2][1] = player2_snake_position[body2 - 1][1];

            }
            for (body2 = SNAKE2_BODY_LEN; body2 >= 1; body2--)
                board_content[player2_snake_position[body2][0]][player2_snake_position[body2][1]] = PLAYER2_SNAKE_BODY;

            player2_snake_position[0][1] += 1;
            if (player2_snake_position[0][1] >= BOARD_SIZE) {
                player2_snake_position[0][1] = 0;
            }
            board_content[player2_snake_position[0][0]][player2_snake_position[0][1]] = PLAYER2_SNAKE_HEAD;
        }

        /****************************************************************************************************************************************************************************************************************************************************************************************************************/
        for (int i = 0; i < NUMBER_OF_MOUSES; i++) {
            if (player2_snake_position[0][0] == mouse_position[i][0] && player2_snake_position[0][1] == mouse_position[i][1]) {            //when second player's snake eats a mouse
                completing_mouses(mouse_position, board_content, i);
                if (SNAKE2_BODY_LEN < MAX_LEN_SNAKES - 1) {
                    SNAKE2_BODY_LEN += 1;
                    player2_snake_position[SNAKE2_BODY_LEN][0] = j_snake_tail2;
                    player2_snake_position[SNAKE2_BODY_LEN][1] = i_snake_tail2;
                }
                else break;
            }

        }

        /****************************************************************************************************************************************************************************************************************************************************************************************************************/

         //when their heads collide
        if (player1_snake_position[0][0] == player2_snake_position[0][0] && player1_snake_position[0][1] == player2_snake_position[0][1]) {
            if (SNAKE2_BODY_LEN > SNAKE1_BODY_LEN)
                printf("Player 2 (blue) has won!!\n\a");
            else if (SNAKE2_BODY_LEN < SNAKE1_BODY_LEN)
                printf("Player 1 (red) has won!!\n\a");
            else if (SNAKE2_BODY_LEN == SNAKE1_BODY_LEN)
                printf("The game has no winners!!\n\a");
            break;
        }
        /****************************************************************************************************************************************************************************************************************************************************************************************************************/
           //when first player's snake's head collides to its body
        for (body1 = 1; body1 <= SNAKE1_BODY_LEN; body1++) {
            if (player1_snake_position[0][0] == player1_snake_position[body1][0] && player1_snake_position[0][1] == player1_snake_position[body1][1]) {
                printf("Player 2 (blue) has won!!\n\a");
                END_GAME -= 1;
            }
        }


        //when second player's snake's head collides to its body
        for (body2 = 1; body2 <= SNAKE2_BODY_LEN; body2++) {
            if (player2_snake_position[0][0] == player2_snake_position[body2][0] && player2_snake_position[0][1] == player2_snake_position[body2][1]) {
                printf("Player 1 (red) has won!!\n\a");
                END_GAME -= 1;
            }
        }

        /****************************************************************************************************************************************************************************************************************************************************************************************************************/
           //when first player's snake's head collides to second snake's body
        for (body2 = 1; body2 <= SNAKE2_BODY_LEN; body2++) {
            if (player1_snake_position[0][0] == player2_snake_position[body2][0] && player1_snake_position[0][1] == player2_snake_position[body2][1]) {
                printf("Player 2 (blue) has won!!\n\a");
                END_GAME -= 1;
            }
        }


        //when second player's snake's head collides to first snake's body
        for (body1 = 1; body1 <= SNAKE1_BODY_LEN; body1++) {
            if (player2_snake_position[0][0] == player1_snake_position[body1][0] && player2_snake_position[0][1] == player1_snake_position[body1][1]) {
                printf("Player 1 (red) has won!!\n\a");
                END_GAME -= 1;
            }
        }
        if (END_GAME == 0) break;

        /****************************************************************************************************************************************************************************************************************************************************************************************************************/
            //when first player's body's length becomes 1
        if (SNAKE1_BODY_LEN < MINIMUM_SNAKE_LENGTH - 1) {
            printf("Player 2 (blue) has won!!\n\a");
            break;
        }

        //when second player's body's length becomes 1
        if (SNAKE2_BODY_LEN < MINIMUM_SNAKE_LENGTH - 1) {
            printf("Player 1 (red) has won!!\n\a");
            break;
        }
    }

    return 0;
}

void completing_mouses(int mouse_position[NUMBER_OF_MOUSES][2], char board_content[BOARD_SIZE][BOARD_SIZE], int i) {                                 //keeping the number of mouses 20
    int j = 0;
    while (j < 1) {
        mouse_position[i][0] = rand() % BOARD_SIZE;
        mouse_position[i][1] = rand() % BOARD_SIZE;
        if (board_content[mouse_position[i][0]][mouse_position[i][1]] == EMPTY) {
            board_content[mouse_position[i][0]][mouse_position[i][1]] = MOUSE;
            j++;
        }
        else continue;
    }

}
/****************************************************************************************************************************************************************************************************************************************************************************************************************/
void completing_golden_stars(int golden_star_position[NUMBER_OF_GOLDEN_STARS][2], char board_content[BOARD_SIZE][BOARD_SIZE], int i) {               //keeping the number of golden stars 3
    int j = 0;
    while (j < 1) {
        golden_star_position[i][0] = rand() % BOARD_SIZE;
        golden_star_position[i][1] = rand() % BOARD_SIZE;
        if (board_content[golden_star_position[i][0]][golden_star_position[i][1]] == EMPTY) {
            board_content[golden_star_position[i][0]][golden_star_position[i][1]] = GOLDEN_STAR;
            j++;
        }
        else continue;
    }

}