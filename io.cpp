#include <unistd.h>
#include <ncurses.h>
#include <cctype>
#include <cstdlib>
#include <climits>

#include "io.h"
#include "character.h"
#include "poke327.h"

typedef struct io_message {
  /* Will print " --more-- " at end of line when another message follows. *
   * Leave 10 extra spaces for that.                                      */
  char msg[71];
  struct io_message *next;
} io_message_t;

static io_message_t *io_head, *io_tail;

void io_init_terminal(void)
{
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

void io_reset_terminal(void)
{
  endwin();

  while (io_head) {
    io_tail = io_head;
    io_head = io_head->next;
    free(io_tail);
  }
  io_tail = NULL;
}

int io_starter_screen(void) {
  int input, validInput;

  clear();
  
  do {
    mvprintw(5, 25, "Choose your starter Pokemon");
    mvprintw(6, 31, "Input 1, 2, or 3");
    mvprintw(7, 20, "Squirtle  |  Bulbasaur  |  Charmander");
    mvprintw(8, 20, "  [1]           [2]           [3]    ");
    refresh();
    switch(input = getch()) {
      case '1':
        validInput = 1;
        break;
      case '2':
        validInput = 1;
        break;
      case '3':
        validInput = 1;
        break;
      default:
        mvprintw(11, 7, "Invalid input, please input: 1, 2, or 3 for the respective starter! ");
        refresh();
        validInput = 0;
        break;
    }
  } while(!validInput);

  return input;
  clear();
}

void io_queue_message(const char *format, ...)
{
  io_message_t *tmp;
  va_list ap;

  if (!(tmp = (io_message_t *) malloc(sizeof (*tmp)))) {
    perror("malloc");
    exit(1);
  }

  tmp->next = NULL;

  va_start(ap, format);

  vsnprintf(tmp->msg, sizeof (tmp->msg), format, ap);

  va_end(ap);

  if (!io_head) {
    io_head = io_tail = tmp;
  } else {
    io_tail->next = tmp;
    io_tail = tmp;
  }
}

static void io_print_message_queue(uint32_t y, uint32_t x)
{
  while (io_head) {
    io_tail = io_head;
    attron(COLOR_PAIR(COLOR_CYAN));
    mvprintw(y, x, "%-80s", io_head->msg);
    attroff(COLOR_PAIR(COLOR_CYAN));
    io_head = io_head->next;
    if (io_head) {
      attron(COLOR_PAIR(COLOR_CYAN));
      mvprintw(y, x + 70, "%10s", " --more-- ");
      attroff(COLOR_PAIR(COLOR_CYAN));
      refresh();
      getch();
    }
    free(io_tail);
  }
  io_tail = NULL;
}

/**************************************************************************
 * Compares trainer distances from the PC according to the rival distance *
 * map.  This gives the approximate distance that the PC must travel to   *
 * get to the trainer (doesn't account for crossing buildings).  This is  *
 * not the distance from the NPC to the PC unless the NPC is a rival.     *
 *                                                                        *
 * Not a bug.                                                             *
 **************************************************************************/
static int compare_trainer_distance(const void *v1, const void *v2)
{
  const character *const *c1 = (const character * const *) v1;
  const character *const *c2 = (const character * const *) v2;

  return (world.rival_dist[(*c1)->pos[dim_y]][(*c1)->pos[dim_x]] -
          world.rival_dist[(*c2)->pos[dim_y]][(*c2)->pos[dim_x]]);
}

static character *io_nearest_visible_trainer()
{
  character **c, *n;
  uint32_t x, y, count;

  c = (character **) malloc(world.cur_map->num_trainers * sizeof (*c));

  /* Get a linear list of trainers */
  for (count = 0, y = 1; y < MAP_Y - 1; y++) {
    for (x = 1; x < MAP_X - 1; x++) {
      if (world.cur_map->cmap[y][x] && world.cur_map->cmap[y][x] !=
          &world.pc) {
        c[count++] = world.cur_map->cmap[y][x];
      }
    }
  }

  /* Sort it by distance from PC */
  qsort(c, count, sizeof (*c), compare_trainer_distance);

  n = c[0];

  free(c);

  return n;
}

void io_display()
{
  uint32_t y, x;
  character *c;

  clear();
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      if (world.cur_map->cmap[y][x]) {
        mvaddch(y + 1, x, world.cur_map->cmap[y][x]->symbol);
      } else {
        switch (world.cur_map->map[y][x]) {
        case ter_boulder:
          attron(COLOR_PAIR(COLOR_MAGENTA));
          mvaddch(y + 1, x, BOULDER_SYMBOL);
          attroff(COLOR_PAIR(COLOR_MAGENTA));
          break;
        case ter_mountain:
          attron(COLOR_PAIR(COLOR_MAGENTA));
          mvaddch(y + 1, x, MOUNTAIN_SYMBOL);
          attroff(COLOR_PAIR(COLOR_MAGENTA));
          break;
        case ter_tree:
          attron(COLOR_PAIR(COLOR_GREEN));
          mvaddch(y + 1, x, TREE_SYMBOL);
          attroff(COLOR_PAIR(COLOR_GREEN));
          break;
        case ter_forest:
          attron(COLOR_PAIR(COLOR_GREEN));
          mvaddch(y + 1, x, FOREST_SYMBOL);
          attroff(COLOR_PAIR(COLOR_GREEN));
          break;
        case ter_path:
          attron(COLOR_PAIR(COLOR_YELLOW));
          mvaddch(y + 1, x, PATH_SYMBOL);
          attroff(COLOR_PAIR(COLOR_YELLOW));
          break;
        case ter_gate:
          attron(COLOR_PAIR(COLOR_YELLOW));
          mvaddch(y + 1, x, GATE_SYMBOL);
          attroff(COLOR_PAIR(COLOR_YELLOW));
          break;
        case ter_bailey:
          attron(COLOR_PAIR(COLOR_YELLOW));
          mvaddch(y + 1, x, BAILEY_SYMBOL);
          attroff(COLOR_PAIR(COLOR_YELLOW));
          break;
        case ter_mart:
          attron(COLOR_PAIR(COLOR_BLUE));
          mvaddch(y + 1, x, POKEMART_SYMBOL);
          attroff(COLOR_PAIR(COLOR_BLUE));
          break;
        case ter_center:
          attron(COLOR_PAIR(COLOR_RED));
          mvaddch(y + 1, x, POKEMON_CENTER_SYMBOL);
          attroff(COLOR_PAIR(COLOR_RED));
          break;
        case ter_grass:
          attron(COLOR_PAIR(COLOR_GREEN));
          mvaddch(y + 1, x, TALL_GRASS_SYMBOL);
          attroff(COLOR_PAIR(COLOR_GREEN));
          break;
        case ter_clearing:
          attron(COLOR_PAIR(COLOR_GREEN));
          mvaddch(y + 1, x, SHORT_GRASS_SYMBOL);
          attroff(COLOR_PAIR(COLOR_GREEN));
          break;
        case ter_water:
          attron(COLOR_PAIR(COLOR_CYAN));
          mvaddch(y + 1, x, WATER_SYMBOL);
          attroff(COLOR_PAIR(COLOR_CYAN));
          break;
        default:
          attron(COLOR_PAIR(COLOR_CYAN));
          mvaddch(y + 1, x, ERROR_SYMBOL);
          attroff(COLOR_PAIR(COLOR_CYAN)); 
       }
      }
    }
  }

  mvprintw(23, 1, "PC position is (%2d,%2d) on map %d%cx%d%c.",
           world.pc.pos[dim_x],
           world.pc.pos[dim_y],
           abs(world.cur_idx[dim_x] - (WORLD_SIZE / 2)),
           world.cur_idx[dim_x] - (WORLD_SIZE / 2) >= 0 ? 'E' : 'W',
           abs(world.cur_idx[dim_y] - (WORLD_SIZE / 2)),
           world.cur_idx[dim_y] - (WORLD_SIZE / 2) <= 0 ? 'N' : 'S');
  mvprintw(22, 1, "%d known %s.", world.cur_map->num_trainers,
           world.cur_map->num_trainers > 1 ? "trainers" : "trainer");
  mvprintw(22, 30, "Nearest visible trainer: ");
  if ((c = io_nearest_visible_trainer())) {
    attron(COLOR_PAIR(COLOR_RED));
    mvprintw(22, 55, "%c at vector %d%cx%d%c.",
             c->symbol,
             abs(c->pos[dim_y] - world.pc.pos[dim_y]),
             ((c->pos[dim_y] - world.pc.pos[dim_y]) <= 0 ?
              'N' : 'S'),
             abs(c->pos[dim_x] - world.pc.pos[dim_x]),
             ((c->pos[dim_x] - world.pc.pos[dim_x]) <= 0 ?
              'W' : 'E'));
    attroff(COLOR_PAIR(COLOR_RED));
  } else {
    attron(COLOR_PAIR(COLOR_BLUE));
    mvprintw(22, 55, "NONE.");
    attroff(COLOR_PAIR(COLOR_BLUE));
  }

  io_print_message_queue(0, 0);

  refresh();
}

uint32_t io_teleport_pc(pair_t dest)
{
  /* Just for fun. And debugging.  Mostly debugging. */

  do {
    dest[dim_x] = rand_range(1, MAP_X - 2);
    dest[dim_y] = rand_range(1, MAP_Y - 2);
  } while (world.cur_map->cmap[dest[dim_y]][dest[dim_x]]                  ||
           move_cost[char_pc][world.cur_map->map[dest[dim_y]]
                                                [dest[dim_x]]] ==
             DIJKSTRA_PATH_MAX                                            ||
           world.rival_dist[dest[dim_y]][dest[dim_x]] < 0);

  return 0;
}

static void io_scroll_trainer_list(char (*s)[40], uint32_t count)
{
  uint32_t offset;
  uint32_t i;

  offset = 0;

  while (1) {
    for (i = 0; i < 13; i++) {
      mvprintw(i + 6, 19, " %-40s ", s[i + offset]);
    }
    switch (getch()) {
    case KEY_UP:
      if (offset) {
        offset--;
      }
      break;
    case KEY_DOWN:
      if (offset < (count - 13)) {
        offset++;
      }
      break;
    case 27:
      return;
    }

  }
}

static void io_list_trainers_display(npc **c, uint32_t count)
{
  uint32_t i;
  char (*s)[40]; /* pointer to array of 40 char */

  s = (char (*)[40]) malloc(count * sizeof (*s));

  mvprintw(3, 19, " %-40s ", "");
  /* Borrow the first element of our array for this string: */
  snprintf(s[0], 40, "You know of %d trainers:", count);
  mvprintw(4, 19, " %-40s ", *s);
  mvprintw(5, 19, " %-40s ", "");

  for (i = 0; i < count; i++) {
    snprintf(s[i], 40, "%16s %c: %2d %s by %2d %s",
             char_type_name[c[i]->ctype],
             c[i]->symbol,
             abs(c[i]->pos[dim_y] - world.pc.pos[dim_y]),
             ((c[i]->pos[dim_y] - world.pc.pos[dim_y]) <= 0 ?
              "North" : "South"),
             abs(c[i]->pos[dim_x] - world.pc.pos[dim_x]),
             ((c[i]->pos[dim_x] - world.pc.pos[dim_x]) <= 0 ?
              "West" : "East"));
    if (count <= 13) {
      /* Handle the non-scrolling case right here. *
       * Scrolling in another function.            */
      mvprintw(i + 6, 19, " %-40s ", s[i]);
    }
  }

  if (count <= 13) {
    mvprintw(count + 6, 19, " %-40s ", "");
    mvprintw(count + 7, 19, " %-40s ", "Hit escape to continue.");
    while (getch() != 27 /* escape */)
      ;
  } else {
    mvprintw(19, 19, " %-40s ", "");
    mvprintw(20, 19, " %-40s ",
             "Arrows to scroll, escape to continue.");
    io_scroll_trainer_list(s, count);
  }

  free(s);
}

static void io_list_trainers()
{
  npc **c;
  uint32_t x, y, count;

  c = (npc **) malloc(world.cur_map->num_trainers * sizeof (*c));

  /* Get a linear list of trainers */
  for (count = 0, y = 1; y < MAP_Y - 1; y++) {
    for (x = 1; x < MAP_X - 1; x++) {
      if (world.cur_map->cmap[y][x] && world.cur_map->cmap[y][x] !=
          &world.pc) {
        c[count++] = dynamic_cast<npc *> (world.cur_map->cmap[y][x]);
      }
    }
  }

  /* Sort it by distance from PC */
  qsort(c, count, sizeof (*c), compare_trainer_distance);

  /* Display it */
  io_list_trainers_display(c, count);
  free(c);

  /* And redraw the map */
  io_display();
}

void io_pokemart()
{
  world.pc.potions = world.pc.potions < 5 ? 5 : world.pc.potions;
  world.pc.revives = world.pc.revives < 3 ? 3 : world.pc.revives;
  world.pc.pokeballs = world.pc.pokeballs < 10 ? 10 : world.pc.pokeballs;
  mvprintw(0, 0, "Welcome to the Pokemart. Your items have been replenished");
  refresh();
  getch();
}

void io_pokemon_center()
{
  long unsigned int i, j;

  for (i = 0 ; i < world.pc.pokemon.size(); i++) {
    world.pc.pokemon[i]->stats[0] = world.pc.pokemon[i]->max_health;
    for (j = 0; j < world.pc.pokemon[i]->current_pps.size(); j++) {
      world.pc.pokemon[i]->current_pps[j] = world.pc.pokemon[i]->max_pps[j];
    }
    world.pc.pokemon[i]->knocked_out = 0;
  }

  mvprintw(0, 0, "Welcome to the Pokemon Center. Your pokemon have been healed");
  refresh();
  getch();
}

void io_battle_options(int curr_pokemon_index) {
  int input;
  int unsigned long i;

  printw("Fight (f)\nBag (b)\nRun (r)\nPokemon (p)\n");
  refresh();
  input = getch();
  switch (input) {
    case 'f':
      clear();
      for (i = 0; i < world.pc.pokemon[curr_pokemon_index]->moves.size(); i++) {
        printw("%s  %d\n", get_pokemon_move_name(world.pc.pokemon[curr_pokemon_index]->moves[i]).c_str(), world.pc.pokemon[curr_pokemon_index]->current_pps[i]);
      }
      getch();
      break;
    case 'b':
      break;
    case 'r':
      break;
    case 'p':
      break;
    default:
      break;
  }
}

void io_battle(character *aggressor, character *defender)
{
  npc *n = (npc *) ((aggressor == &world.pc) ? defender : aggressor);
  int cycle, input, break_loop;
  long unsigned int i;

  break_loop = 0;
  cycle = 0;
  do {
    clear();
    printw("Use the keys 4 and 6 on a num pad to cycle through the pokemon a trainer has. Hit q to exit\n");
    printw("This trainer has %d pokemon\n", n->pokemon.size());
    printw("Pokemon: %s\n", (n->pokemon[cycle]->identifier).c_str());
    printw("Pokemon level: %d\n", n->pokemon[cycle]->pokemon_level);
    printw("Health: %d, Attack: %d, Defence: %d, Special-Attack: %d, Special-Defence: %d, Speed: %d\n", n->pokemon[cycle]->stats[0], n->pokemon[cycle]->stats[1], n->pokemon[cycle]->stats[2], n->pokemon[cycle]->stats[3], n->pokemon[cycle]->stats[4], n->pokemon[cycle]->stats[5]);
    for (i = 0; i < n->pokemon[cycle]->moves.size(); i++) {
      printw("Move %d: %s\n", i + 1, get_pokemon_move_name(n->pokemon[cycle]->moves[i]).c_str());
    }
    printw("Gender: %s\n", n->pokemon[cycle]->gender == 1 ? "Female" : "Male");
    printw("Shiny: %s\n", n->pokemon[cycle]->is_shiny == 1 ? "Yes" : "No");
    printw("Press q to exit\n");
    refresh();
    switch (input = getch()) {
      case '4':
        cycle = cycle == 0 ? n->pokemon.size() - 1 : (cycle - 1) % n->pokemon.size();
        break;
      case '6':
        cycle = (cycle + 1) % n->pokemon.size();
        break;
      case 'q':
        break_loop = 1;
        break;
      default:
        break;
    }
  } while (!break_loop);

  io_display();
  mvprintw(0, 0, "Aww, how'd you get so strong?  You and your pokemon must share a special bond!");
  refresh();
  getch();

  n->defeated = 1;
  if (n->ctype == char_hiker || n->ctype == char_rival) {
    n->mtype = move_wander;
  }
}

void io_pokemon_battle(Pokemon *random_pokemon) {
  // clear();
  // printw("Encountered Pokemon\n");
  // printw("Pokemon: %s\n", (random_pokemon->identifier).c_str());
  // printw("Pokemon level: %d\n", random_pokemon->pokemon_level);
  // printw("Health: %d, Attack: %d, Defence: %d, Special-Attack: %d, Special-Defence: %d, Speed: %d\n", random_pokemon->stats[0], random_pokemon->stats[1], random_pokemon->stats[2], random_pokemon->stats[3], random_pokemon->stats[4], random_pokemon->stats[5]);
  // if (random_pokemon->moves.size() == 1) {
  //   printw("Move 1: %s\n", get_pokemon_move_name(random_pokemon->moves[0]).c_str());
  // } else if (random_pokemon->moves.size() == 2) {
  //   printw("Move 1: %s, Move 2: %s\n", get_pokemon_move_name(random_pokemon->moves[0]).c_str(), get_pokemon_move_name(random_pokemon->moves[1]).c_str());
  // }
  // printw("Gender: %s\n", random_pokemon->gender == 1 ? "Female" : "Male");
  // printw("Shiny: %s\n", random_pokemon->is_shiny == 1 ? "Yes" : "No");
  // printw("Press any key to exit\n");
  // refresh();
  // getch();
  // io_display();
  // refresh();

  int curr_pokemon_index;
  int unsigned long i;

  for (i = 0; i < world.pc.pokemon.size(); i++) {
    if (world.pc.pokemon[i]->knocked_out == 0) {
      curr_pokemon_index = i;
      break;
    }
  }
  clear();
  printw("Encountered a wild %s\n\n", (random_pokemon->identifier).c_str());
  io_battle_options(curr_pokemon_index);

}

uint32_t move_pc_dir(uint32_t input, pair_t dest)
{
  dest[dim_y] = world.pc.pos[dim_y];
  dest[dim_x] = world.pc.pos[dim_x];

  switch (input) {
  case 1:
  case 2:
  case 3:
    dest[dim_y]++;
    break;
  case 4:
  case 5:
  case 6:
    break;
  case 7:
  case 8:
  case 9:
    dest[dim_y]--;
    break;
  }
  switch (input) {
  case 1:
  case 4:
  case 7:
    dest[dim_x]--;
    break;
  case 2:
  case 5:
  case 8:
    break;
  case 3:
  case 6:
  case 9:
    dest[dim_x]++;
    break;
  case '>':
    if (world.cur_map->map[world.pc.pos[dim_y]][world.pc.pos[dim_x]] ==
        ter_mart) {
      io_pokemart();
    }
    if (world.cur_map->map[world.pc.pos[dim_y]][world.pc.pos[dim_x]] ==
        ter_center) {
      io_pokemon_center();
    }
    break;
  }

  if (world.cur_map->cmap[dest[dim_y]][dest[dim_x]]) {
    if (dynamic_cast<npc *> (world.cur_map->cmap[dest[dim_y]][dest[dim_x]]) &&
        ((npc *) world.cur_map->cmap[dest[dim_y]][dest[dim_x]])->defeated) {
      // Some kind of greeting here would be nice
      return 1;
    } else if ((dynamic_cast<npc *> (world.cur_map->cmap[dest[dim_y]][dest[dim_x]]))) {
      if (world.pc.all_knocked_out == 0) {
        io_battle(&world.pc, world.cur_map->cmap[dest[dim_y]][dest[dim_x]]);
      }
      // Not actually moving, so set dest back to PC position
      dest[dim_x] = world.pc.pos[dim_x];
      dest[dim_y] = world.pc.pos[dim_y];
    }
  }

  // Encounter pokemon here
  if (world.cur_map->map[dest[dim_y]][dest[dim_x]] == ter_grass) {
    int encounter;
    encounter = std::rand() % 10 == 0 ? 1 : 0;
    if (encounter) {
      // One cause this is a random encounter, limits moves to 2
      Pokemon* random_pokemon;
      random_pokemon = create_pokemon(1);
      io_pokemon_battle(random_pokemon);
    }
  }
  
  if (move_cost[char_pc][world.cur_map->map[dest[dim_y]][dest[dim_x]]] ==
      DIJKSTRA_PATH_MAX) {
    return 1;
  }

  if (world.cur_map->map[dest[dim_y]][dest[dim_x]] == ter_gate &&
      dest[dim_y] != world.pc.pos[dim_y]                       &&
      dest[dim_x] != world.pc.pos[dim_x]) {
    return 1;
  }

  return 0;
}

void io_teleport_world(pair_t dest)
{
  /* mvscanw documentation is unclear about return values.  I believe *
   * that the return value works the same way as scanf, but instead   *
   * of counting on that, we'll initialize x and y to out of bounds   *
   * values and accept their updates only if in range.                */
  int x = INT_MAX, y = INT_MAX;
  
  world.cur_map->cmap[world.pc.pos[dim_y]][world.pc.pos[dim_x]] = NULL;

  echo();
  curs_set(1);
  do {
    mvprintw(0, 0, "Enter x [-200, 200]:           ");
    refresh();
    mvscanw(0, 21, "%d", &x);
  } while (x < -200 || x > 200);
  do {
    mvprintw(0, 0, "Enter y [-200, 200]:          ");
    refresh();
    mvscanw(0, 21, "%d", &y);
  } while (y < -200 || y > 200);

  refresh();
  noecho();
  curs_set(0);

  x += 200;
  y += 200;

  world.cur_idx[dim_x] = x;
  world.cur_idx[dim_y] = y;

  new_map(1);
  io_teleport_pc(dest);
}

void io_open_bag() {
  int cycle, break_loop, input;
  long unsigned int i;

  break_loop = 0;
  cycle = 0;
  do {
    clear();
    printw("Your Pokemon, use 4 and 6 to scroll between them\n");
    printw("Pokemon: %s\n", (world.pc.pokemon[cycle]->identifier).c_str());
    printw("Pokemon level: %d\n", world.pc.pokemon[cycle]->pokemon_level);
    printw("Health: %d/%d, Attack: %d, Defence: %d, Special-Attack: %d, Special-Defence: %d, Speed: %d\n", world.pc.pokemon[cycle]->stats[0], world.pc.pokemon[cycle]->max_health, world.pc.pokemon[cycle]->stats[1], world.pc.pokemon[cycle]->stats[2], world.pc.pokemon[cycle]->stats[3], world.pc.pokemon[cycle]->stats[4], world.pc.pokemon[cycle]->stats[5]);
    for (i = 0; i < world.pc.pokemon[cycle]->moves.size(); i++) {
      printw("Move %d: %s\n", i + 1, get_pokemon_move_name(world.pc.pokemon[cycle]->moves[i]).c_str());
    }
    printw("Gender: %s\n", world.pc.pokemon[cycle]->gender == 1 ? "Female" : "Male");
    printw("Shiny: %s\n", world.pc.pokemon[cycle]->is_shiny == 1 ? "Yes" : "No");
    printw("Knocked out: %s\n", world.pc.pokemon[cycle]->knocked_out == 1 ? "Yes" : "No");
    printw("Your items\n");
    printw("Potions: %d\n", world.pc.potions);
    printw("Revives: %d\n", world.pc.revives);
    printw("Pokeballs: %d\n", world.pc.pokeballs);
    printw("Press h to heal the selected pokemon, press r to revive the selected pokemon, press q to exit\n");
    refresh();
    switch (input = getch()) {
      case 'h':
        if (world.pc.pokemon[cycle]->stats[0] < world.pc.pokemon[cycle]->max_health && world.pc.pokemon[cycle]->max_health != 0 && world.pc.pokemon[cycle]->knocked_out == 0 && world.pc.potions > 0) {
          world.pc.pokemon[cycle]->stats[0] = ((world.pc.pokemon[cycle]->stats[0] + 20) > world.pc.pokemon[cycle]->max_health) ? world.pc.pokemon[cycle]->max_health : world.pc.pokemon[cycle]->stats[0] += 20;
          world.pc.potions--;
        } else {
          clear();
          printw("Your pokemon is %s\n", world.pc.pokemon[cycle]->knocked_out == 1 ? "knocked out, you can't revive it with a potion." : "already at max health!");
          refresh();
          getch();
        }
        break;
      case 'r':
        if (world.pc.pokemon[cycle]->stats[0] == 0 && world.pc.pokemon[cycle]->knocked_out == 1 && world.pc.revives > 0) {
          world.pc.pokemon[cycle]->knocked_out = 0;
          world.pc.pokemon[cycle]->stats[0] = world.pc.pokemon[cycle]->max_health / 2;
          world.pc.revives--;
        } else {
          clear();
          printw("Your pokemon is not knocked out!\n");
          refresh();
          getch();
        }
        break;
      case '4':
        cycle = cycle == 0 ? world.pc.pokemon.size() - 1 : (cycle - 1) % world.pc.pokemon.size();
        break;
      case '6':
        cycle = (cycle + 1) % world.pc.pokemon.size();
        break;
      case 'q':
        break_loop = 1;
        break;
      default:
        break;
    }
  } while (!break_loop);
  io_display();
}

void io_handle_input(pair_t dest)
{
  uint32_t turn_not_consumed;
  int key;

  do {
    switch (key = getch()) {
    case '7':
    case 'y':
    case KEY_HOME:
      turn_not_consumed = move_pc_dir(7, dest);
      break;
    case '8':
    case 'k':
    case KEY_UP:
      turn_not_consumed = move_pc_dir(8, dest);
      break;
    case '9':
    case 'u':
    case KEY_PPAGE:
      turn_not_consumed = move_pc_dir(9, dest);
      break;
    case '6':
    case 'l':
    case KEY_RIGHT:
      turn_not_consumed = move_pc_dir(6, dest);
      break;
    case '3':
    case 'n':
    case KEY_NPAGE:
      turn_not_consumed = move_pc_dir(3, dest);
      break;
    case '2':
    case 'j':
    case KEY_DOWN:
      turn_not_consumed = move_pc_dir(2, dest);
      break;
    case '1':
    case 'b':
    case KEY_END:
      turn_not_consumed = move_pc_dir(1, dest);
      break;
    case '4':
    case 'h':
    case KEY_LEFT:
      turn_not_consumed = move_pc_dir(4, dest);
      break;
    case '5':
    case ' ':
    case '.':
    case KEY_B2:
      dest[dim_y] = world.pc.pos[dim_y];
      dest[dim_x] = world.pc.pos[dim_x];
      turn_not_consumed = 0;
      break;
    case '>':
      turn_not_consumed = move_pc_dir('>', dest);
      break;
    case 'Q':
      dest[dim_y] = world.pc.pos[dim_y];
      dest[dim_x] = world.pc.pos[dim_x];
      world.quit = 1;
      turn_not_consumed = 0;
      break;
      break;
    case 't':
      io_list_trainers();
      turn_not_consumed = 1;
      break;
    case 'p':
      /* Teleport the PC to a random place in the map.              */
      io_teleport_pc(dest);
      turn_not_consumed = 0;
      break;
    case 'f':
      /* Fly to any map in the world.                                */
      io_teleport_world(dest);
      turn_not_consumed = 0;
      break;    
    case 'q':
      /* Demonstrate use of the message queue.  You can use this for *
       * printf()-style debugging (though gdb is probably a better   *
       * option.  Not that it matters, but using this command will   *
       * waste a turn.  Set turn_not_consumed to 1 and you should be *
       * able to figure out why I did it that way.                   */
      io_queue_message("This is the first message.");
      io_queue_message("Since there are multiple messages, "
                       "you will see \"more\" prompts.");
      io_queue_message("You can use any key to advance through messages.");
      io_queue_message("Normal gameplay will not resume until the queue "
                       "is empty.");
      io_queue_message("Long lines will be truncated, not wrapped.");
      io_queue_message("io_queue_message() is variadic and handles "
                       "all printf() conversion specifiers.");
      io_queue_message("Did you see %s?", "what I did there");
      io_queue_message("When the last message is displayed, there will "
                       "be no \"more\" prompt.");
      io_queue_message("Have fun!  And happy printing!");
      io_queue_message("Oh!  And use 'Q' to quit!");

      dest[dim_y] = world.pc.pos[dim_y];
      dest[dim_x] = world.pc.pos[dim_x];
      turn_not_consumed = 0;
      break;
    case 'B':
      io_open_bag();
      turn_not_consumed = 1;
      break;
    default:
      /* Also not in the spec.  It's not always easy to figure out what *
       * key code corresponds with a given keystroke.  Print out any    *
       * unhandled key here.  Not only does it give a visual error      *
       * indicator, but it also gives an integer value that can be used *
       * for that key in this (or other) switch statements.  Printed in *
       * octal, with the leading zero, because ncurses.h lists codes in *
       * octal, thus allowing us to do reverse lookups.  If a key has a *
       * name defined in the header, you can use the name here, else    *
       * you can directly use the octal value.                          */
      mvprintw(0, 0, "Unbound key: %#o ", key);
      turn_not_consumed = 1;
    }
    refresh();
  } while (turn_not_consumed);
}
