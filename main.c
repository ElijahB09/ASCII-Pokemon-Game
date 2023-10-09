#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "buildings.h"
#include "worldgen.h"
#include "handleTurns.h"
#define X_BOUND 80
#define Y_BOUND 21

int main(int argc, char *argv[]) {
    PokeMap* world[401][401];
    PlayerCharacter *player;
    int current_x, current_y, i, j, /*fly_x, fly_y,*/ num_npcs;
    //char userInput;
    TurnOrder *turnOrder;
    num_npcs = 10;

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

    srand(time(NULL));

    current_x = 200;
    current_y = 200;
    *world[current_y][current_x] = *generateMap(world[current_y][current_x], world, current_x, current_y, num_npcs, player, npcs);
    // Comment out unused values for this week
    //userInput = 'x';
    //fly_x = fly_y = -999;
    turnOrder = createTurnPriority(num_npcs, npcs, player, world[current_y][current_x]);
    printf("%d\n\n", turnOrder->size);

    for (i = 0; i < Y_BOUND; i++) {
        for (j = 0; j < X_BOUND; j++) {
            if (world[current_y][current_x]->arr[i][j].character_present) {
                printf("%c", *world[current_y][current_x]->arr[i][j].present_character);
            } else {
                printf("%c", world[current_y][current_x]->arr[i][j].terrainPiece);
            }
        }
        printf("\n");
    }

    takeTurn(turnOrder, world[current_y][current_x]);

    for (i = 0; i < Y_BOUND; i++) {
        for (j = 0; j < X_BOUND; j++) {
            if (world[current_y][current_x]->arr[i][j].present_character) {
                printf("%c", *world[current_y][current_x]->arr[i][j].present_character);
            } else {
                printf("%c", world[current_y][current_x]->arr[i][j].terrainPiece);
            }
        }
        printf("\n");
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

    for(i = 0; i < 401; i++) {
        for(j = 0; j < 401; j++) {
            if(world[i][j]){
                free(world[i][j]);
                world[i][j] = NULL;
            }
        }
    }
    free(player);
    for (i = 0; i < num_npcs; i++) {
        free(npcs[i]);
    }
    free(turnOrder->arr);
    free(turnOrder);

    return 0;
}