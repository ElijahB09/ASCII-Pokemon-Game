#include <iostream>
#include <ctime>
#include <cstring>
#include <chrono>
#include <thread>
#include "handleTurns.h"

#define X_BOUND 80
#define Y_BOUND 21

void io_init_terminal()
{
	initscr();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	nodelay(stdscr, FALSE);
}

int main(int argc, char *argv[])
{
	PokeMap *world[401][401];
	PlayerCharacter *player = new PlayerCharacter;
	int current_x, current_y, i, j, k, fly_x, fly_y, num_npcs, found, tempx, tempy;
	num_npcs = 10;
	int user_input, gameRunning;
	char charToPrint;

	if (argc == 2)
	{
		if (strncmp(argv[1], "--numtrainers", 13) == 0)
		{
			char *num_npcs_str = argv[1] + 14;
			char *endptr;
			num_npcs = static_cast<int>(strtol(num_npcs_str, &endptr, 10));

			if (*endptr != '\0' || num_npcs <= 0)
			{
				std::cout << "ERROR: use --numtrainers=x where x >= 0, instead passed: " << num_npcs_str << std::endl;
				return 1;
			}
		}
	}
	else if (argc > 2)
	{
		std::cout << "ERROR: usage ./<assignment_name> --numtrainers=x where x >= 0" << std::endl;
		return 1;
	}

	// Init characters
	player->symbol = '@';

	for (i = 0; i < 401; i++)
	{
		for (j = 0; j < 401; j++)
		{
			world[i][j] = nullptr;
		}
	}

	io_init_terminal();
	srand(static_cast<unsigned>(time(nullptr)));

	current_x = 200;
	current_y = 200;
	*world[current_y][current_x] = *generateMap(world[current_y][current_x], world, current_x, current_y, num_npcs);

	fly_x = current_x;
	fly_y = current_y;

	found = 0;
	for (i = 0; i < Y_BOUND; i++)
	{
		for (j = 0; j < X_BOUND; j++)
		{
			if (world[current_y][current_x]->arr[i][j].terrainPiece == '#' && i > 0 && j > 0 && found == 0 && world[current_y][current_x]->arr[i][j].character_present == 0)
			{
				player->x_coord = j;
				player->y_coord = i;
				world[current_y][current_x]->arr[i][j].player = player;
				world[current_y][current_x]->arr[i][j].character_present = 1;
				tempx = j;
				tempy = i;
				found = 1;
			}
		}
	}
	// Get cost maps
	DijkstraTrainers(X_BOUND, Y_BOUND, world[current_y][current_x], world[current_y][current_x]->arr[tempy][tempx]);
	initNPCS(world[current_y][current_x], num_npcs, world[current_y][current_x]->npcs);

	world[current_y][current_x]->order = createTurnPriority(num_npcs, world[current_y][current_x]->npcs, player, world[current_y][current_x]);

	clear();
	printw("Starting PokeGame!\n");
	refresh();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	gameRunning = 1;

	clear();
	printw("Beginning");
	for (i = 0; i < Y_BOUND; i++)
	{
		for (j = 0; j < X_BOUND; j++)
		{
			if (world[current_y][current_x]->arr[i][j].character_present == 1)
			{
				charToPrint = world[current_y][current_x]->arr[i][j].npc != nullptr ? world[current_y][current_x]->arr[i][j].npc->symbol : world[current_y][current_x]->arr[i][j].player->symbol;
				mvaddch(i + 1, j, charToPrint);
			}
			else
			{
				charToPrint = world[current_y][current_x]->arr[i][j].terrainPiece;
				mvaddch(i + 1, j, charToPrint);
			}
		}
	}
	refresh();
	while (gameRunning)
	{
		clear();
		for (i = 0; i < 21; i++)
		{
			for (j = 0; j < 80; j++)
			{
				if (world[current_y][current_x]->arr[i][j].character_present == 1)
				{
					charToPrint = world[current_y][current_x]->arr[i][j].npc != nullptr ? world[current_y][current_x]->arr[i][j].npc->symbol : world[current_y][current_x]->arr[i][j].player->symbol;
					mvaddch(i + 1, j, charToPrint);
				}
				else
				{
					charToPrint = world[current_y][current_x]->arr[i][j].terrainPiece;
					mvaddch(i + 1, j, charToPrint);
				}
			}
		}
		refresh();

		for (i = 0; i < num_npcs; i++)
		{
			user_input = takeTurn(world[current_y][current_x]->order, world[current_y][current_x], num_npcs, world[current_y][current_x]->npcs);

			if (user_input == 'Q')
			{
				gameRunning = 0;
			}
			else if (user_input == 'L')
			{
				current_x--;
				if (current_x >= 0)
				{
					*world[current_y][current_x] = *generateMap(world[current_y][current_x], world, current_x, current_y, num_npcs);
					world[current_y][current_x]->arr[world[current_y][current_x + 1]->left_exit][78].player = player;
					world[current_y][current_x]->arr[world[current_y][current_x + 1]->left_exit][78].character_present = 1;
					player->y_coord = world[current_y][current_x + 1]->left_exit;
					player->x_coord = 78;
					if (world[current_y][current_x]->order == nullptr)
					{
						DijkstraTrainers(X_BOUND, Y_BOUND, world[current_y][current_x], world[current_y][current_x]->arr[player->y_coord][player->x_coord]);
						initNPCS(world[current_y][current_x], num_npcs, world[current_y][current_x]->npcs);
						world[current_y][current_x]->order = createTurnPriority(num_npcs, world[current_y][current_x]->npcs, player, world[current_y][current_x]);
					}
				}
				else
				{
					current_x++;
					clear();
					printw("Error cannot go beyond world limits\n");
					refresh();
				}
				break;
			}
			else if (user_input == 'R')
			{
				current_x++;
				if (current_x < 401)
				{
					*world[current_y][current_x] = *generateMap(world[current_y][current_x], world, current_x, current_y, num_npcs);
					world[current_y][current_x]->arr[world[current_y][current_x - 1]->right_exit][1].player = player;
					world[current_y][current_x]->arr[world[current_y][current_x - 1]->right_exit][1].character_present = 1;
					player->y_coord = world[current_y][current_x - 1]->right_exit;
					player->x_coord = 1;
					if (world[current_y][current_x]->order == nullptr)
					{
						DijkstraTrainers(X_BOUND, Y_BOUND, world[current_y][current_x], world[current_y][current_x]->arr[player->y_coord][player->x_coord]);
						initNPCS(world[current_y][current_x], num_npcs, world[current_y][current_x]->npcs);
						world[current_y][current_x]->order = createTurnPriority(num_npcs, world[current_y][current_x]->npcs, player, world[current_y][current_x]);
					}
				}
				else
				{
					current_x--;
					clear();
					printw("Error cannot go beyond world limits\n");
					refresh();
				}
				break;
			}
			else if (user_input == 'U')
			{
				current_y--;
				if (current_y >= 0)
				{
					*world[current_y][current_x] = *generateMap(world[current_y][current_x], world, current_x, current_y, num_npcs);
					world[current_y][current_x]->arr[19][world[current_y + 1][current_x]->up_exit].player = player;
					world[current_y][current_x]->arr[19][world[current_y + 1][current_x]->up_exit].character_present = 1;
					player->y_coord = 19;
					player->x_coord = world[current_y + 1][current_x]->up_exit;
					if (world[current_y][current_x]->order == nullptr)
					{
						DijkstraTrainers(X_BOUND, Y_BOUND, world[current_y][current_x], world[current_y][current_x]->arr[player->y_coord][player->x_coord]);
						initNPCS(world[current_y][current_x], num_npcs, world[current_y][current_x]->npcs);
						world[current_y][current_x]->order = createTurnPriority(num_npcs, world[current_y][current_x]->npcs, player, world[current_y][current_x]);
					}
				}
				else
				{
					current_y++;
					clear();
					printw("Error cannot go beyond world limits\n");
					refresh();
				}
				break;
			}
			else if (user_input == 'D')
			{
				current_y++;
				if (current_y < 401)
				{
					*world[current_y][current_x] = *generateMap(world[current_y][current_x], world, current_x, current_y, num_npcs);
					world[current_y][current_x]->arr[1][world[current_y - 1][current_x]->down_exit].player = player;
					world[current_y][current_x]->arr[1][world[current_y - 1][current_x]->down_exit].character_present = 1;
					player->y_coord = 1;
					player->x_coord = world[current_y - 1][current_x]->down_exit;
					if (world[current_y][current_x]->order == nullptr)
					{
						DijkstraTrainers(X_BOUND, Y_BOUND, world[current_y][current_x], world[current_y][current_x]->arr[player->y_coord][player->x_coord]);
						initNPCS(world[current_y][current_x], num_npcs, world[current_y][current_x]->npcs);
						world[current_y][current_x]->order = createTurnPriority(num_npcs, world[current_y][current_x]->npcs, player, world[current_y][current_x]);
					}
				}
				else
				{
					current_y--;
					clear();
					printw("Error cannot go beyond world limits\n");
					refresh();
				}
				break;
			}
			else if (user_input == 'f')
			{
				clear();
				printw("Enter in the desired fly coords, x first from -200 to 200 inclusive, then y with the same bounds.");
				refresh();

				noraw();
				curs_set(1);
				echo();
				move(1, 0);

				do
				{
					scanw("%d", &fly_x);
				} while (fly_x < -200 || fly_x > 200);

				do
				{
					scanw("%d", &fly_y);
				} while (fly_y < -200 || fly_y > 200);

				raw();
				curs_set(0);
				noecho();

				tempx = current_x;
				tempy = current_y;
				current_x = fly_x + 200;
				current_y = fly_y + 200;

				*world[current_y][current_x] = *generateMap(world[current_y][current_x], world, current_x, current_y, num_npcs);

				if (tempx < current_x)
				{
					tempx = 78;
					tempy = world[current_y][current_x]->right_exit;
				}
				else
				{
					tempx = 1;
					tempy = world[current_y][current_x]->left_exit;
				}

				if (tempy < current_y)
				{
					tempy = 19;
					tempx = world[current_y][current_x]->down_exit;
				}
				else
				{
					tempy = 1;
					tempx = world[current_y][current_x]->up_exit;
				}

				world[current_y][current_x]->arr[tempy][tempx].player = player;
				world[current_y][current_x]->arr[tempy][tempx].character_present = 1;
				player->y_coord = tempy;
				player->x_coord = tempx;
				if (world[current_y][current_x]->order == nullptr)
				{
					DijkstraTrainers(X_BOUND, Y_BOUND, world[current_y][current_x], world[current_y][current_x]->arr[player->y_coord][player->x_coord]);
					initNPCS(world[current_y][current_x], num_npcs, world[current_y][current_x]->npcs);
					world[current_y][current_x]->order = createTurnPriority(num_npcs, world[current_y][current_x]->npcs, player, world[current_y][current_x]);
				}
				break;
			}
		}
	}
	clear();
	endwin();

	for (i = 0; i < 401; i++)
	{
		for (j = 0; j < 401; j++)
		{
			if (world[i][j])
			{
				delete[] world[i][j]->order->arr;
				world[i][j]->order->arr = nullptr;
				delete world[i][j]->order;
				world[i][j]->order = nullptr;
				for (k = 0; k < num_npcs; k++)
				{
					delete world[i][j]->npcs[k];
					world[i][j]->npcs[k] = nullptr;
				}
				delete world[i][j]->npcs;
				world[i][j]->npcs = nullptr;
				delete world[i][j];
				world[i][j] = nullptr;
			}
		}
	}

	delete player;

	return 0;
}
