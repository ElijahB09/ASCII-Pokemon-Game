#include "handleBattle.h"

int battle(PokeMap* map, NPC* opponent) {
    int inputFromUser;

    inputFromUser = 0;
	
    clear();
    printw("Holder Battle Screen");
    refresh();
    while(inputFromUser != 27) {
	inputFromUser = getch();
    }
    // Return 1 for a defeated trainer
    return 1;
}
