// #include <stdio.h>

// // Function to print the game board

// Function to check if a player has won

// int main() {
//     char board[3][3] = {
//         {' ', ' ', ' '},
//         {' ', ' ', ' '},
//         {' ', ' ', ' '}
//     };

//     int currentPlayer = 1;
//     int moves = 0;

//     while (1) {
//         // Print the board
//         printBoard(board);

//         // Get the current player's move
//         char crow, ccol;
//         printf("Player %d, enter your move (row column): ", currentPlayer);
//         scanf(" %c %c", &crow, &ccol);

// printf("row = %c, col = %c\n", crow, ccol);
//         // Check if the move is valid
//         if (crow < '0' ) {
//             printf("crow < 0 \n");
//         }

//         if (crow >= '3') {
//             printf("crow >= 3 \n");
//         }

//         if (ccol < '0') {
//             printf("ccol < 0 \n");
//         }

//         if (ccol >= '3') {
//             printf("ccol >= 3 \n");
//         }

//         if (crow < '0' || crow >= '3' || ccol < '0' || ccol >= '3') {
//             printf("Invalid move. Try again.\n");
//             continue;
//         }

//         int row = crow - '0';
//         int col = ccol - '0';

//         if (board[row][col] != ' ') {
//             printf("Cell already taken. Try again.\n");
//             continue;
//         }

//         // Make the move
//         board[row][col] = (currentPlayer == 1) ? 'X' : 'O';
//         moves++;

//         // Check if the current player has won
//         if (checkWin(board, (currentPlayer == 1) ? 'X' : 'O')) {
//             printf("Player %d wins!\n", currentPlayer);
//             break;
//         }

//         // Check if it's a draw
//         if (moves == 9) {
//             printf("It's a draw!\n");
//             break;
//         }

//         // Switch to the other player
//         currentPlayer = (currentPlayer == 1) ? 2 : 1;
//     }

//     return 0;
// }

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void printBoard(int board[9])
{
    printf("-------------\n");
    for (int i = 0; i < 9; i++)
    {
        printf("| %i ", board[i]);
        if (i % 3 == 2)
        {
            printf("|\n");
            printf("-------------\n");
        }
    }
}
int checkWin(int board[9], int player)
{
    // Check rows

    if ((board[0] == player && board[1] == player && board[2] == player) ||
        (board[3] == player && board[4] == player && board[5] == player) ||
        (board[6] == player && board[7] == player && board[8] == player))
    {
        return 1;
    }

    // Check columns

    if ((board[0] == player && board[3] == player && board[6] == player) ||
        (board[1] == player && board[4] == player && board[7] == player) ||
        (board[2] == player && board[5] == player && board[8] == player))
    {
        return 1;
    }

    // Check diagonals
    if (board[0] == player && board[4] == player && board[8] == player)
    {
        return 1;
    }
    if (board[2] == player && board[4] == player && board[6] == player)
    {
        return 1;
    }
    return 0;
}

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer)
{
    SDL_Surface *tmp = NULL;
    SDL_Texture *texture = NULL;
    tmp = IMG_Load(path);
    if (NULL == tmp)
    {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if (NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;
}
int checkfill(SDL_bool filledRect[9])
{
    if (filledRect[0] && filledRect[1] && filledRect[2] && filledRect[3] && filledRect[4] && filledRect[5] && filledRect[6] && filledRect[7] && filledRect[8])
    {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int statut = EXIT_FAILURE;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    int x, y, i;
    int w = 700, h = 700;
    FILE *error = fopen("error.txt", "w");
    if (0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        fprintf(error, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        return -1;
    }
    window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (NULL == window)
    {
        fprintf(error, "Erreur de creation de la fenetre : %s\n", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (NULL == renderer)
    {
        fprintf(error, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET, 200, 200);
    if (NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTexture : %s", SDL_GetError());
        goto Quit;
    }
    if (0 != SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255))
    {
        fprintf(error, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }

    if (0 != SDL_RenderClear(renderer))
    {
        fprintf(error, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Rect rect[9];
    SDL_Rect title;
    SDL_Rect winner;
    for (i = 0; i < 9; i++)
    {
        rect[i].w = 150;
        rect[i].h = 150;
        rect[i].x = ((w - rect[i].w) / 4) + 150 * (i % 3);
        rect[i].y = ((h - rect[i].h) / 4) + 150 * (i / 3);
    }
    if (SDL_RenderDrawRects(renderer, rect, 9) != 0)
    {
        fprintf(error, "Erreur SDL_RenderDrawRects : %s", SDL_GetError());
        goto Quit;
    }
    SDL_RenderPresent(renderer);

    title.w = 70;
    title.h = 70;
    title.x = 50;
    title.y = 50;
    winner.w = 90;
    winner.h = 90;
    winner.x = 120;
    winner.y = 50;
    SDL_Color grey = {100, 100, 100, 20};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Delay(100);
    SDL_bool filledRect[9] = {SDL_FALSE};
    int board[9] = {0};
    SDL_bool turn = SDL_TRUE;
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    while (!quit)
    {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
        {
            quit = SDL_TRUE;
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            SDL_GetMouseState(&x, &y);
            for (i = 0; i < 9; i++)
            {
                if (x > rect[i].x && x < rect[i].x + rect[i].w && y > rect[i].y && y < rect[i].y + rect[i].h)
                {
                    if (filledRect[i])
                    {
                        break;
                    }
                    const char *path = turn ? "kitty.jpg" : "kuro.jpg";
                    SDL_Texture *img = loadImage(path, renderer);
                    SDL_RenderCopy(renderer, img, NULL, &rect[i]);
                    board[i] = turn ? 1 : 2;
                    turn = !turn;
                    filledRect[i] = SDL_TRUE;
                    SDL_DestroyTexture(img);
                    SDL_RenderPresent(renderer);
                    break;
                }
            }
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&x, &y);
            for (i = 0; i < 9; i++)
            {
                if (x >= rect[i].x && x <= rect[i].x + rect[i].w && y >= rect[i].y && y <= rect[i].y + rect[i].h)
                {
                    SDL_SetRenderDrawColor(renderer, grey.r, grey.g, grey.b, grey.a);
                    SDL_RenderDrawRect(renderer, &rect[i]);
                    SDL_RenderFillRect(renderer, &rect[i]);
                }
                else
                {
                    if (!filledRect[i])
                    {
                        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
                        SDL_RenderDrawRect(renderer, &rect[i]);
                        SDL_RenderFillRect(renderer, &rect[i]);
                    }
                    else
                    {
                        const char *path = (board[i] == 1) ? "kitty.jpg" : "kuro.jpg";
                        SDL_Texture *img = loadImage(path, renderer);
                        SDL_RenderCopy(renderer, img, NULL, &rect[i]);
                        SDL_DestroyTexture(img);
                    }
                }
            }
        }
        printBoard(board);
        if (checkWin(board, 1) == 1)
        {
            SDL_Texture *win = loadImage("winner.jpeg", renderer);
            SDL_Texture *img = loadImage("kitty.jpg", renderer);
            SDL_RenderCopy(renderer, win, NULL, &title);
            SDL_RenderCopy(renderer, img, NULL, &winner);
            SDL_DestroyTexture(win);
            SDL_DestroyTexture(img);
            SDL_RenderPresent(renderer);
            SDL_Delay(5000);
            goto Quit;
        }
        else if (checkWin(board, 2) == 1)
        {
            SDL_Texture *win = loadImage("winner.jpeg", renderer);
            SDL_Texture *img = loadImage("kuro.jpg", renderer);
            SDL_RenderCopy(renderer, win, NULL, &title);
            SDL_RenderCopy(renderer, img, NULL, &winner);
            SDL_DestroyTexture(win);
            SDL_DestroyTexture(img);
            SDL_RenderPresent(renderer);
            SDL_Delay(5000);
            goto Quit;
        }
        else if (checkfill(filledRect))
        {
            SDL_Texture *win = loadImage("draw.jpeg", renderer);
            SDL_RenderCopy(renderer, win, NULL, &title);
            SDL_DestroyTexture(win);
            SDL_RenderPresent(renderer);
            SDL_Delay(5000);
            goto Quit;
        }
        else
        {
            const char *turnp = turn ? "kitty.jpg" : "kuro.jpg";
            SDL_Texture *img = loadImage(turnp, renderer);
            SDL_RenderCopy(renderer, img, NULL, &title);
            SDL_DestroyTexture(img);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            for (int i = 0; i < 9; i++)
            {
                rect[i].w = 150;
                rect[i].h = 150;
                rect[i].x = ((w - rect[i].w) / 4) + 150 * (i % 3);
                rect[i].y = ((h - rect[i].h) / 4) + 150 * (i / 3);
            }
            if (SDL_RenderDrawRects(renderer, rect, 9) != 0)
            {
                fprintf(error, "Erreur SDL_RenderDrawRects : %s", SDL_GetError());
                goto Quit;
            }
            SDL_RenderPresent(renderer);
        }
        SDL_Delay(5);
    }
Quit:
    if (NULL != texture)
        SDL_DestroyTexture(texture);
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}
