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
    return 1;
}
