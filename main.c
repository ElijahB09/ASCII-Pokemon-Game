#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "buildings.h"
#include "worldgen.h"
#include "handleTurns.h"
#define X_BOUND 80
#define Y_BOUND 21

void io_init_terminal(void)
{
    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, FALSE);
}

int main(int argc, char *argv[]) {
    PokeMap* world[401][401];
    PlayerCharacter *player;
    int current_x, current_y, i, j, k,/*fly_x, fly_y,*/ num_npcs, found, tempx, tempy;
    //char userInput;
    num_npcs = 10;
    int user_input, gameRunning;
    char charToPrint;

    if (argc == 2) {
        if (strncmp(argv[1], "--numtrainers", 13) == 0) {
            char *num_npcs_str = argv[1] + 14;
            char *endptr;
            num_npcs = (int)strtol(num_npcs_str, &endptr, 10);

            if (*endptr != '\0' || num_npcs <= 0) {
                printf("ERROR: use --numtrainers=x where x >= 0, instead passed: %s\n", num_npcs_str);
                return 1;
            }
        }
    } else if (argc > 2) {
        printf("ERROR: usage ./<assignment_name> --numtrainers=x where x >= 0");
    }

    // Init characters
    NPC *npcs[num_npcs];
    // This malloc is here cause valgrind was angry otherwise
    player = malloc(sizeof (PlayerCharacter));
    player->symbol = '@';
    for (i = 0; i < num_npcs; i++) {
        npcs[i] = NULL;
    }

    for (i = 0; i < 401; i++) {
        for (j = 0; j < 401; j++) {
            world[i][j] = NULL;
        }
    }

    io_init_terminal();
    srand(time(NULL));

    current_x = 200;
    current_y = 200;
    *world[current_y][current_x] = *generateMap(world[current_y][current_x], world, current_x, current_y, num_npcs, npcs);
    // Comment out unused values for this week
    //userInput = 'x';
    //fly_x = fly_y = -999;

    // place player, this may have to move later due to the nature of the player moving between maps
    // For now its okay
    found = 0;
    for (i = 0; i < Y_BOUND; i++) {
        for (j = 0; j < X_BOUND; j++) {
            if (world[current_y][current_x]->arr[i][j].terrainPiece == '#' && i > 0 && j > 0 && found == 0 && world[current_y][current_x]->arr[i][j].character_present == 0) {
                player->x_coord = j; player->y_coord = i;
                world[current_y][current_x]->arr[i][j].player = player;
                world[current_y][current_x]->arr[i][j].character_present = 1;
                tempx = j;
                tempy = i;
                found = 1;
            }
        }
    }
    // Get cost maps
    DijkstraTrainers(X_BOUND, Y_BOUND, world[current_y][current_x]->arr, world[current_y][current_x]->arr[tempy][tempx]);

    // Bottom of what may have to move

    world[current_y][current_x]->order = createTurnPriority(num_npcs, npcs, player, world[current_y][current_x]);
    
    clear();
    printw("Starting PokeGame!\n");
    refresh();
    sleep(2);
    gameRunning = 1;
    
    clear();
    printw("Beginning");
    for (i = 0; i < Y_BOUND; i++) {
        for (j = 0; j < X_BOUND; j++) {
            if (world[current_y][current_x]->arr[i][j].character_present == 1) {
		charToPrint = world[current_y][current_x]->arr[i][j].npc != NULL ? world[current_y][current_x]->arr[i][j].npc->symbol : world[current_y][current_x]->arr[i][j].player->symbol;
                mvaddch(i + 1, j, charToPrint);
            } else {
		charToPrint = world[current_y][current_x]->arr[i][j].terrainPiece;
                mvaddch(i + 1, j, charToPrint);
            }
        }
    }
    refresh();

    while (gameRunning) {
	k = 0;
        for (i = 0; i < Y_BOUND; i++) {
            for (j = 0; j < X_BOUND; j++) {
		if (world[current_y][current_x]->arr[i][j].npc != NULL) {
		    npcs[k] = world[current_y][current_x]->arr[i][j].npc;
		    k++;
		}
	    }
	}
	for (i = 0; i < num_npcs; i++) {
	    user_input = takeTurn(world[current_y][current_x]->order, world[current_y][current_x], num_npcs, npcs);
	    if (user_input == 'Q') {
		gameRunning = 0;
	    }
	}
    }
    clear();
    endwin();

    for(i = 0; i < 401; i++) {
        for(j = 0; j < 401; j++) {
            if(world[i][j]){
		free(world[i][j]->order->arr);
		world[i][j]->order->arr = NULL;
		free(world[i][j]->order);
		world[i][j]->order = NULL;
                free(world[i][j]);
                world[i][j] = NULL;
            }
        }
    }
    free(player);
    for (i = 0; i < num_npcs; i++) {
        free(npcs[i]);
    }

    return 0;
}

// Comment out functionality for moving between maps
//    while (userInput != 'q') {
//        for (i = 0; i < Y_BOUND; i++) {
//            for (j = 0; j < X_BOUND; j++) {
//                if (world[current_y][current_x]->arr[i][j].present_character) {
//                    printf("%c", *world[current_y][current_x]->arr[i][j].present_character);
//                } else {
//                    printf("%c", world[current_y][current_x]->arr[i][j].terrainPiece);
//                }
//            }
//            printf("\n");
//        }
//
//        printf("You are currently at position (%d, %d). Input command:", world[current_y][current_x]->world_x - 200, world[current_y][current_x]->world_y - 200);
//
//        if (scanf(" %c", &userInput) == 1) {
//            printf("\n");
//            switch (userInput) {
//                case 'w':
//                    current_x--;
//                    if (current_x >= 0) {
//                        *world[current_y][current_x] = *generateMap(world[current_y][current_x],world,current_x, current_y, num_npcs, player, npcs);
//                    } else {
//                        current_x++;
//                        printf("Error cannot go beyond world limits\n");
//                    }
//                    break;
//                case 'e':
//                    current_x++;
//                    if (current_x < 401) {
//                        *world[current_y][current_x] = *generateMap(world[current_y][current_x],world,current_x, current_y, num_npcs, player, npcs);
//                    } else {
//                        current_x--;
//                        printf("Error cannot go beyond world limits\n");
//                    }
//                    break;
//                case 'n':
//                    current_y--;
//                    if (current_y >= 0) {
//                        *world[current_y][current_x] = *generateMap(world[current_y][current_x],world,current_x, current_y, num_npcs, player, npcs);
//                    } else {
//                        current_y++;
//                        printf("Error cannot go beyond world limits\n");
//                    }
//                    break;
//                case 's':
//                    current_y++;
//                    if (current_y < 401) {
//                        *world[current_y][current_x] = *generateMap(world[current_y][current_x],world,current_x, current_y, num_npcs, player, npcs);
//                    } else {
//                        current_y--;
//                        printf("Error cannot go beyond world limits\n");
//                    }
//                    break;
//                case 'q':
//                    printf("Now quitting\n");
//                    break;
//                case 'f':
//                    if (scanf("%d %d", &fly_x, &fly_y) == 2) {
//                        if ((-200 <= fly_x && fly_x < 201) && (-200 <= fly_y && fly_y < 201)) {
//                            current_x = fly_x + 200;
//                            current_y = fly_y + 200;
//                            *world[current_y][current_x] = *generateMap(world[current_y][current_x], world, current_x, current_y, num_npcs, player, npcs);
//                        } else {
//                            printf("Invalid coordinates, must be between -200 and 200 inclusive for both x and y");
//                        }
//                    }
//                    break;
//                default:
//                    printf("Bad input, commands are n, s, e, w, 'f x y', and q\n");
//                    break;
//            }
//        } else {
//            printf("Bad input, commands are n, s, e, w, 'f x y', and q\n");
//        }
//    }
