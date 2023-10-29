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
    int current_x, current_y, i, j, k, fly_x, fly_y, num_npcs, found, tempx, tempy;
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
    fly_x = current_x;
    fly_y = current_y;

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
	    } else if (user_input == 'L') {
	        current_x--;
                if (current_x >= 0) {
                    *world[current_y][current_x] = *generateMap(world[current_y][current_x],world,current_x, current_y, num_npcs, npcs);
		    world[current_y][current_x]->arr[world[current_y][current_x + 1]->left_exit][78].player = player;
		    world[current_y][current_x]->arr[world[current_y][current_x + 1]->left_exit][78].character_present = 1;
		    player->y_coord = world[current_y][current_x + 1]->left_exit;
		    player->x_coord = 78;
		    if (world[current_y][current_x]->order == NULL) {
                        world[current_y][current_x]->order = createTurnPriority(num_npcs, npcs, player, world[current_y][current_x]);
		    }
                } else {
                    current_x++;
		    clear();
                    printw("Error cannot go beyond world limits\n");
		    refresh();
                }
		break;
	    } else if (user_input == 'R') {
	        current_x++;
                if (current_x < 401) {
                    *world[current_y][current_x] = *generateMap(world[current_y][current_x],world,current_x, current_y, num_npcs, npcs);
		    world[current_y][current_x]->arr[world[current_y][current_x - 1]->right_exit][1].player = player;
		    world[current_y][current_x]->arr[world[current_y][current_x - 1]->right_exit][1].character_present = 1;
		    player->y_coord = world[current_y][current_x - 1]->right_exit;
		    player->x_coord = 1;
		    if (world[current_y][current_x]->order == NULL) {
                        world[current_y][current_x]->order = createTurnPriority(num_npcs, npcs, player, world[current_y][current_x]);
		    }
		} else {
                    current_x--;
		    clear();
                    printw("Error cannot go beyond world limits\n");
		    refresh();
                }
		break;
	    } else if (user_input == 'U') {
	        current_y--;
                if (current_y >= 0) {
                    *world[current_y][current_x] = *generateMap(world[current_y][current_x],world,current_x, current_y, num_npcs, npcs); 
		    world[current_y][current_x]->arr[19][world[current_y + 1][current_x]->up_exit].player = player;
		    world[current_y][current_x]->arr[19][world[current_y + 1][current_x]->up_exit].character_present = 1;
		    player->y_coord = 19;
		    player->x_coord = world[current_y + 1][current_x]->up_exit;
		    if (world[current_y][current_x]->order == NULL) {
		        world[current_y][current_x]->order = createTurnPriority(num_npcs, npcs, player, world[current_y][current_x]);
                    }
		} else {
                    current_y++;
		    clear();
                    printw("Error cannot go beyond world limits\n");
		    refresh();
                }
		break;
	    } else if (user_input == 'D') {
	        current_y++;
                if (current_y < 401) {
                    *world[current_y][current_x] = *generateMap(world[current_y][current_x],world,current_x, current_y, num_npcs, npcs);
		    world[current_y][current_x]->arr[1][world[current_y - 1][current_x]->down_exit].player = player;
		    world[current_y][current_x]->arr[1][world[current_y - 1][current_x]->down_exit].character_present = 1;
		    player->y_coord = 1;
		    player->x_coord = world[current_y - 1][current_x]->down_exit;
		    if (world[current_y][current_x]->order == NULL) {
		        world[current_y][current_x]->order = createTurnPriority(num_npcs, npcs, player, world[current_y][current_x]);
		    }
                } else {
                    current_y--;
		    clear();
                    printw("Error cannot go beyond world limits\n");
		    refresh();
                }
		break;
	    } else if (user_input == 'f') {
	        clear();
		printw("Enter in the desired fly coords, x first from -200 to 200 inclusive, then y with the same bounds.");
		refresh();

		noraw();
		curs_set(1);
		echo();
		move(1, 0);

		do {
		    scanw("%d", &fly_x);
		} while (fly_x < -200 || fly_x > 200);
		
		do {
		    scanw("%d", &fly_y);
		} while (fly_y < -200 || fly_y > 200);

		raw();
		curs_set(0);
		noecho();

		tempx = current_x;
		tempy = current_y;
		current_x = fly_x + 200;
		current_y = fly_y + 200;

                *world[current_y][current_x] = *generateMap(world[current_y][current_x], world, current_x, current_y, num_npcs, npcs);

		if (tempx < current_x) {
		    tempx = 78;
		    tempy = world[current_y][current_x]->right_exit;
		} else {
		    tempx = 1;
		    tempy = world[current_y][current_x]->left_exit;
		}

		if (tempy < current_y) {
		    tempy = 19;
		    tempx = world[current_y][current_x]->down_exit;
		} else {
		    tempy = 1;
		    tempx = world[current_y][current_x]->up_exit;
		}

		world[current_y][current_x]->arr[tempy][tempx].player = player;
		world[current_y][current_x]->arr[tempy][tempx].character_present = 1;
		player->y_coord = tempy;
		player->x_coord = tempx;
		if (world[current_y][current_x]->order == NULL) {
		    world[current_y][current_x]->order = createTurnPriority(num_npcs, npcs, player, world[current_y][current_x]);
		}
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
