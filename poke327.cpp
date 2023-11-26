#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <cassert>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <cctype>

#include "heap.h"
#include "poke327.h"
#include "character.h"
#include "io.h"

typedef struct queue_node {
  int x, y;
  struct queue_node *next;
} queue_node_t;

/* Even unallocated, a WORLD_SIZE x WORLD_SIZE array of pointers is a very *
 * large thing to put on the stack.  To avoid that, world is a global.     */
class world world;

pair_t all_dirs[8] = {
  { -1, -1 },
  { -1,  0 },
  { -1,  1 },
  {  0, -1 },
  {  0,  1 },
  {  1, -1 },
  {  1,  0 },
  {  1,  1 },
};

Pokemon* starter_pokemon;
std::vector<CSV*> pokemon, pokemon_moves, pokemon_species, pokemon_stats, pokemon_types, stats, moves, experience, type_names;


static int32_t path_cmp(const void *key, const void *with) {
  return ((path_t *) key)->cost - ((path_t *) with)->cost;
}

static int32_t edge_penalty(int8_t x, int8_t y)
{
  return (x == 1 || y == 1 || x == MAP_X - 2 || y == MAP_Y - 2) ? 2 : 1;
}

static void dijkstra_path(map *m, pair_t from, pair_t to)
{
  static path_t path[MAP_Y][MAP_X], *p;
  static uint32_t initialized = 0;
  heap_t h;
  int32_t x, y;

  if (!initialized) {
    for (y = 0; y < MAP_Y; y++) {
      for (x = 0; x < MAP_X; x++) {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }
    }
    initialized = 1;
  }
  
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      path[y][x].cost = INT_MAX;
    }
  }

  path[from[dim_y]][from[dim_x]].cost = 0;

  heap_init(&h, path_cmp, NULL);

  for (y = 1; y < MAP_Y - 1; y++) {
    for (x = 1; x < MAP_X - 1; x++) {
      path[y][x].hn = heap_insert(&h, &path[y][x]);
    }
  }

  while ((p = (path_t *) heap_remove_min(&h))) {
    p->hn = NULL;

    if ((p->pos[dim_y] == to[dim_y]) && p->pos[dim_x] == to[dim_x]) {
      for (x = to[dim_x], y = to[dim_y];
           (x != from[dim_x]) || (y != from[dim_y]);
           p = &path[y][x], x = p->from[dim_x], y = p->from[dim_y]) {
        /* Don't overwrite the gate */
        if (x != to[dim_x] || y != to[dim_y]) {
          mapxy(x, y) = ter_path;
          heightxy(x, y) = 0;
        }
      }
      heap_delete(&h);
      return;
    }

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost >
         ((p->cost + heightpair(p->pos)) *
          edge_penalty(p->pos[dim_x], p->pos[dim_y] - 1)))) {
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost =
        ((p->cost + heightpair(p->pos)) *
         edge_penalty(p->pos[dim_x], p->pos[dim_y] - 1));
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x]    ].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost >
         ((p->cost + heightpair(p->pos)) *
          edge_penalty(p->pos[dim_x] - 1, p->pos[dim_y])))) {
      path[p->pos[dim_y]][p->pos[dim_x] - 1].cost =
        ((p->cost + heightpair(p->pos)) *
         edge_penalty(p->pos[dim_x] - 1, p->pos[dim_y]));
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost >
         ((p->cost + heightpair(p->pos)) *
          edge_penalty(p->pos[dim_x] + 1, p->pos[dim_y])))) {
      path[p->pos[dim_y]][p->pos[dim_x] + 1].cost =
        ((p->cost + heightpair(p->pos)) *
         edge_penalty(p->pos[dim_x] + 1, p->pos[dim_y]));
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost >
         ((p->cost + heightpair(p->pos)) *
          edge_penalty(p->pos[dim_x], p->pos[dim_y] + 1)))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost =
        ((p->cost + heightpair(p->pos)) *
         edge_penalty(p->pos[dim_x], p->pos[dim_y] + 1));
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x]    ].hn);
    }
  }
}

static int build_paths(map *m)
{
  pair_t from, to;

  /*  printf("%d %d %d %d\n", m->n, m->s, m->e, m->w);*/

  if (m->e != -1 && m->w != -1) {
    from[dim_x] = 1;
    to[dim_x] = MAP_X - 2;
    from[dim_y] = m->w;
    to[dim_y] = m->e;

    dijkstra_path(m, from, to);
  }

  if (m->n != -1 && m->s != -1) {
    from[dim_y] = 1;
    to[dim_y] = MAP_Y - 2;
    from[dim_x] = m->n;
    to[dim_x] = m->s;

    dijkstra_path(m, from, to);
  }

  if (m->e == -1) {
    if (m->s == -1) {
      from[dim_x] = 1;
      from[dim_y] = m->w;
      to[dim_x] = m->n;
      to[dim_y] = 1;
    } else {
      from[dim_x] = 1;
      from[dim_y] = m->w;
      to[dim_x] = m->s;
      to[dim_y] = MAP_Y - 2;
    }

    dijkstra_path(m, from, to);
  }

  if (m->w == -1) {
    if (m->s == -1) {
      from[dim_x] = MAP_X - 2;
      from[dim_y] = m->e;
      to[dim_x] = m->n;
      to[dim_y] = 1;
    } else {
      from[dim_x] = MAP_X - 2;
      from[dim_y] = m->e;
      to[dim_x] = m->s;
      to[dim_y] = MAP_Y - 2;
    }

    dijkstra_path(m, from, to);
  }

  if (m->n == -1) {
    if (m->e == -1) {
      from[dim_x] = 1;
      from[dim_y] = m->w;
      to[dim_x] = m->s;
      to[dim_y] = MAP_Y - 2;
    } else {
      from[dim_x] = MAP_X - 2;
      from[dim_y] = m->e;
      to[dim_x] = m->s;
      to[dim_y] = MAP_Y - 2;
    }

    dijkstra_path(m, from, to);
  }

  if (m->s == -1) {
    if (m->e == -1) {
      from[dim_x] = 1;
      from[dim_y] = m->w;
      to[dim_x] = m->n;
      to[dim_y] = 1;
    } else {
      from[dim_x] = MAP_X - 2;
      from[dim_y] = m->e;
      to[dim_x] = m->n;
      to[dim_y] = 1;
    }

    dijkstra_path(m, from, to);
  }

  return 0;
}

static int gaussian[5][5] = {
  {  1,  4,  7,  4,  1 },
  {  4, 16, 26, 16,  4 },
  {  7, 26, 41, 26,  7 },
  {  4, 16, 26, 16,  4 },
  {  1,  4,  7,  4,  1 }
};

static int smooth_height(map *m)
{
  int32_t i, x, y;
  int32_t s, t, p, q;
  queue_node_t *head, *tail, *tmp;
  /*  FILE *out;*/
  uint8_t height[MAP_Y][MAP_X];

  memset(&height, 0, sizeof (height));

  /* Seed with some values */
  for (i = 1; i < 255; i += 20) {
    do {
      x = rand() % MAP_X;
      y = rand() % MAP_Y;
    } while (height[y][x]);
    height[y][x] = i;
    if (i == 1) {
      head = tail = (queue_node_t *) malloc(sizeof (*tail));
    } else {
      tail->next = (queue_node_t *) malloc(sizeof (*tail));
      tail = tail->next;
    }
    tail->next = NULL;
    tail->x = x;
    tail->y = y;
  }

  /*
  out = fopen("seeded.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&height, sizeof (height), 1, out);
  fclose(out);
  */
  
  /* Diffuse the vaules to fill the space */
  while (head) {
    x = head->x;
    y = head->y;
    i = height[y][x];

    if (x - 1 >= 0 && y - 1 >= 0 && !height[y - 1][x - 1]) {
      height[y - 1][x - 1] = i;
      tail->next = (queue_node_t *) malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x - 1;
      tail->y = y - 1;
    }
    if (x - 1 >= 0 && !height[y][x - 1]) {
      height[y][x - 1] = i;
      tail->next = (queue_node_t *) malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x - 1;
      tail->y = y;
    }
    if (x - 1 >= 0 && y + 1 < MAP_Y && !height[y + 1][x - 1]) {
      height[y + 1][x - 1] = i;
      tail->next = (queue_node_t *) malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x - 1;
      tail->y = y + 1;
    }
    if (y - 1 >= 0 && !height[y - 1][x]) {
      height[y - 1][x] = i;
      tail->next = (queue_node_t *) malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x;
      tail->y = y - 1;
    }
    if (y + 1 < MAP_Y && !height[y + 1][x]) {
      height[y + 1][x] = i;
      tail->next = (queue_node_t *) malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x;
      tail->y = y + 1;
    }
    if (x + 1 < MAP_X && y - 1 >= 0 && !height[y - 1][x + 1]) {
      height[y - 1][x + 1] = i;
      tail->next = (queue_node_t *) malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x + 1;
      tail->y = y - 1;
    }
    if (x + 1 < MAP_X && !height[y][x + 1]) {
      height[y][x + 1] = i;
      tail->next = (queue_node_t *) malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x + 1;
      tail->y = y;
    }
    if (x + 1 < MAP_X && y + 1 < MAP_Y && !height[y + 1][x + 1]) {
      height[y + 1][x + 1] = i;
      tail->next = (queue_node_t *) malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x + 1;
      tail->y = y + 1;
    }

    tmp = head;
    head = head->next;
    free(tmp);
  }

  /* And smooth it a bit with a gaussian convolution */
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      for (s = t = p = 0; p < 5; p++) {
        for (q = 0; q < 5; q++) {
          if (y + (p - 2) >= 0 && y + (p - 2) < MAP_Y &&
              x + (q - 2) >= 0 && x + (q - 2) < MAP_X) {
            s += gaussian[p][q];
            t += height[y + (p - 2)][x + (q - 2)] * gaussian[p][q];
          }
        }
      }
      m->height[y][x] = t / s;
    }
  }
  /* Let's do it again, until it's smooth like Kenny G. */
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      for (s = t = p = 0; p < 5; p++) {
        for (q = 0; q < 5; q++) {
          if (y + (p - 2) >= 0 && y + (p - 2) < MAP_Y &&
              x + (q - 2) >= 0 && x + (q - 2) < MAP_X) {
            s += gaussian[p][q];
            t += height[y + (p - 2)][x + (q - 2)] * gaussian[p][q];
          }
        }
      }
      m->height[y][x] = t / s;
    }
  }

  /*
  out = fopen("diffused.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&height, sizeof (height), 1, out);
  fclose(out);

  out = fopen("smoothed.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&m->height, sizeof (m->height), 1, out);
  fclose(out);
  */

  return 0;
}

static void find_building_location(map *m, pair_t p)
{
  do {
    p[dim_x] = rand() % (MAP_X - 3) + 1;
    p[dim_y] = rand() % (MAP_Y - 3) + 1;

    if ((((mapxy(p[dim_x] - 1, p[dim_y]    ) == ter_path)     &&
          (mapxy(p[dim_x] - 1, p[dim_y] + 1) == ter_path))    ||
         ((mapxy(p[dim_x] + 2, p[dim_y]    ) == ter_path)     &&
          (mapxy(p[dim_x] + 2, p[dim_y] + 1) == ter_path))    ||
         ((mapxy(p[dim_x]    , p[dim_y] - 1) == ter_path)     &&
          (mapxy(p[dim_x] + 1, p[dim_y] - 1) == ter_path))    ||
         ((mapxy(p[dim_x]    , p[dim_y] + 2) == ter_path)     &&
          (mapxy(p[dim_x] + 1, p[dim_y] + 2) == ter_path)))   &&
        (((mapxy(p[dim_x]    , p[dim_y]    ) != ter_mart)     &&
          (mapxy(p[dim_x]    , p[dim_y]    ) != ter_center)   &&
          (mapxy(p[dim_x] + 1, p[dim_y]    ) != ter_mart)     &&
          (mapxy(p[dim_x] + 1, p[dim_y]    ) != ter_center)   &&
          (mapxy(p[dim_x]    , p[dim_y] + 1) != ter_mart)     &&
          (mapxy(p[dim_x]    , p[dim_y] + 1) != ter_center)   &&
          (mapxy(p[dim_x] + 1, p[dim_y] + 1) != ter_mart)     &&
          (mapxy(p[dim_x] + 1, p[dim_y] + 1) != ter_center))) &&
        (((mapxy(p[dim_x]    , p[dim_y]    ) != ter_path)     &&
          (mapxy(p[dim_x] + 1, p[dim_y]    ) != ter_path)     &&
          (mapxy(p[dim_x]    , p[dim_y] + 1) != ter_path)     &&
          (mapxy(p[dim_x] + 1, p[dim_y] + 1) != ter_path)))) {
          break;
    }
  } while (1);
}

static int place_pokemart(map *m)
{
  pair_t p;

  find_building_location(m, p);

  mapxy(p[dim_x]    , p[dim_y]    ) = ter_mart;
  mapxy(p[dim_x] + 1, p[dim_y]    ) = ter_mart;
  mapxy(p[dim_x]    , p[dim_y] + 1) = ter_mart;
  mapxy(p[dim_x] + 1, p[dim_y] + 1) = ter_mart;

  return 0;
}

static int place_center(map *m)
{  pair_t p;

  find_building_location(m, p);

  mapxy(p[dim_x]    , p[dim_y]    ) = ter_center;
  mapxy(p[dim_x] + 1, p[dim_y]    ) = ter_center;
  mapxy(p[dim_x]    , p[dim_y] + 1) = ter_center;
  mapxy(p[dim_x] + 1, p[dim_y] + 1) = ter_center;

  return 0;
}

/* Chooses tree or boulder for border cell.  Choice is biased by dominance *
 * of neighboring cells.                                                   */
static terrain_type_t border_type(map *m, int32_t x, int32_t y)
{
  int32_t p, q;
  int32_t r, t;
  int32_t miny, minx, maxy, maxx;
  
  r = t = 0;
  
  miny = y - 1 >= 0 ? y - 1 : 0;
  maxy = y + 1 <= MAP_Y ? y + 1: MAP_Y;
  minx = x - 1 >= 0 ? x - 1 : 0;
  maxx = x + 1 <= MAP_X ? x + 1: MAP_X;

  for (q = miny; q < maxy; q++) {
    for (p = minx; p < maxx; p++) {
      if (q != y || p != x) {
        if (m->map[q][p] == ter_mountain ||
            m->map[q][p] == ter_boulder) {
          r++;
        } else if (m->map[q][p] == ter_forest ||
                   m->map[q][p] == ter_tree) {
          t++;
        }
      }
    }
  }
  
  if (t == r) {
    return rand() & 1 ? ter_boulder : ter_tree;
  } else if (t > r) {
    if (rand() % 10) {
      return ter_tree;
    } else {
      return ter_boulder;
    }
  } else {
    if (rand() % 10) {
      return ter_boulder;
    } else {
      return ter_tree;
    }
  }
}

static int map_terrain(map *m, int8_t n, int8_t s, int8_t e, int8_t w)
{
  int32_t i, x, y;
  queue_node_t *head, *tail, *tmp;
  //  FILE *out;
  int num_grass, num_clearing, num_mountain, num_forest, num_water, num_total;
  terrain_type_t type;
  int added_current = 0;
  
  num_grass = rand() % 4 + 2;
  num_clearing = rand() % 4 + 2;
  num_mountain = rand() % 2 + 1;
  num_forest = rand() % 2 + 1;
  num_water = rand() % 2 + 1;
  num_total = num_grass + num_clearing + num_mountain + num_forest + num_water;

  memset(&m->map, 0, sizeof (m->map));

  /* Seed with some values */
  for (i = 0; i < num_total; i++) {
    do {
      x = rand() % MAP_X;
      y = rand() % MAP_Y;
    } while (m->map[y][x]);
    if (i == 0) {
      type = ter_grass;
    } else if (i == num_grass) {
      type = ter_clearing;
    } else if (i == num_grass + num_clearing) {
      type = ter_mountain;
    } else if (i == num_grass + num_clearing + num_mountain) {
      type = ter_forest;
    } else if (i == num_grass + num_clearing + num_mountain + num_forest) {
      type = ter_water;
    }
    m->map[y][x] = type;
    if (i == 0) {
      head = tail = (queue_node_t *) malloc(sizeof (*tail));
    } else {
      tail->next = (queue_node_t *) malloc(sizeof (*tail));
      tail = tail->next;
    }
    tail->next = NULL;
    tail->x = x;
    tail->y = y;
  }

  /*
  out = fopen("seeded.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&m->map, sizeof (m->map), 1, out);
  fclose(out);
  */

  /* Diffuse the vaules to fill the space */
  while (head) {
    x = head->x;
    y = head->y;
    type = m->map[y][x];
    
    if (x - 1 >= 0 && !m->map[y][x - 1]) {
      if ((rand() % 100) < 80) {
        m->map[y][x - 1] = type;
        tail->next = (queue_node_t *) malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x - 1;
        tail->y = y;
      } else if (!added_current) {
        added_current = 1;
        m->map[y][x] = type;
        tail->next = (queue_node_t *) malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y;
      }
    }

    if (y - 1 >= 0 && !m->map[y - 1][x]) {
      if ((rand() % 100) < 20) {
        m->map[y - 1][x] = type;
        tail->next = (queue_node_t *) malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y - 1;
      } else if (!added_current) {
        added_current = 1;
        m->map[y][x] = type;
        tail->next = (queue_node_t *) malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y;
      }
    }

    if (y + 1 < MAP_Y && !m->map[y + 1][x]) {
      if ((rand() % 100) < 20) {
        m->map[y + 1][x] = type;
        tail->next = (queue_node_t *) malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y + 1;
      } else if (!added_current) {
        added_current = 1;
        m->map[y][x] = type;
        tail->next = (queue_node_t *) malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y;
      }
    }

    if (x + 1 < MAP_X && !m->map[y][x + 1]) {
      if ((rand() % 100) < 80) {
        m->map[y][x + 1] = type;
        tail->next = (queue_node_t *) malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x + 1;
        tail->y = y;
      } else if (!added_current) {
        added_current = 1;
        m->map[y][x] = type;
        tail->next = (queue_node_t *) malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y;
      }
    }

    added_current = 0;
    tmp = head;
    head = head->next;
    free(tmp);
  }

  /*
  out = fopen("diffused.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&m->map, sizeof (m->map), 1, out);
  fclose(out);
  */
  
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      if (y == 0 || y == MAP_Y - 1 ||
          x == 0 || x == MAP_X - 1) {
        mapxy(x, y) = border_type(m, x, y);
      }
    }
  }

  m->n = n;
  m->s = s;
  m->e = e;
  m->w = w;

  if (n != -1) {
    mapxy(n,         0        ) = ter_gate;
    mapxy(n,         1        ) = ter_bailey;
  }
  if (s != -1) {
    mapxy(s,         MAP_Y - 1) = ter_gate;
    mapxy(s,         MAP_Y - 2) = ter_bailey;
  }
  if (w != -1) {
    mapxy(0,         w        ) = ter_gate;
    mapxy(1,         w        ) = ter_bailey;
  }
  if (e != -1) {
    mapxy(MAP_X - 1, e        ) = ter_gate;
    mapxy(MAP_X - 2, e        ) = ter_bailey;
  }

  return 0;
}

static int place_boulders(map *m)
{
  int i;
  int x, y;

  for (i = 0; i < MIN_BOULDERS || rand() % 100 < BOULDER_PROB; i++) {
    y = rand() % (MAP_Y - 2) + 1;
    x = rand() % (MAP_X - 2) + 1;
    if (m->map[y][x] != ter_forest &&
        m->map[y][x] != ter_path   &&
        m->map[y][x] != ter_gate) {
      m->map[y][x] = ter_boulder;
    }
  }

  return 0;
}

static int place_trees(map *m)
{
  int i;
  int x, y;
  
  for (i = 0; i < MIN_TREES || rand() % 100 < TREE_PROB; i++) {
    y = rand() % (MAP_Y - 2) + 1;
    x = rand() % (MAP_X - 2) + 1;
    if (m->map[y][x] != ter_mountain &&
        m->map[y][x] != ter_path     &&
        m->map[y][x] != ter_water    &&
        m->map[y][x] != ter_gate) {
      m->map[y][x] = ter_tree;
    }
  }

  return 0;
}

void rand_pos(pair_t pos)
{
  pos[dim_x] = (rand() % (MAP_X - 2)) + 1;
  pos[dim_y] = (rand() % (MAP_Y - 2)) + 1;
}

void new_hiker()
{
  pair_t pos;
  npc *c;

  do {
    rand_pos(pos);
  } while (world.hiker_dist[pos[dim_y]][pos[dim_x]] == DIJKSTRA_PATH_MAX ||
           world.cur_map->cmap[pos[dim_y]][pos[dim_x]]                   ||
           pos[dim_x] < 3 || pos[dim_x] > MAP_X - 4                      ||
           pos[dim_y] < 3 || pos[dim_y] > MAP_Y - 4);

  world.cur_map->cmap[pos[dim_y]][pos[dim_x]] = c = new npc;
  c->pos[dim_y] = pos[dim_y];
  c->pos[dim_x] = pos[dim_x];
  c->ctype = char_hiker;
  c->mtype = move_hiker;
  c->dir[dim_x] = 0;
  c->dir[dim_y] = 0;
  c->defeated = 0;
  c->symbol = HIKER_SYMBOL;
  c->next_turn = 0;
  c->seq_num = world.char_seq_num++;

  do {
    c->pokemon.push_back(create_pokemon(0));
  } while (c->pokemon.size() < 6 && ((std::rand() % 10) > 3));

  heap_insert(&world.cur_map->turn, c);
}

void new_rival()
{
  pair_t pos;
  npc *c;

  do {
    rand_pos(pos);
  } while (world.rival_dist[pos[dim_y]][pos[dim_x]] == DIJKSTRA_PATH_MAX ||
           world.rival_dist[pos[dim_y]][pos[dim_x]] < 0                  ||
           world.cur_map->cmap[pos[dim_y]][pos[dim_x]]                   ||
           pos[dim_x] < 3 || pos[dim_x] > MAP_X - 4                      ||
           pos[dim_y] < 3 || pos[dim_y] > MAP_Y - 4);

  world.cur_map->cmap[pos[dim_y]][pos[dim_x]] = c = new npc;
  c->pos[dim_y] = pos[dim_y];
  c->pos[dim_x] = pos[dim_x];
  c->ctype = char_rival;
  c->mtype = move_rival;
  c->dir[dim_x] = 0;
  c->dir[dim_y] = 0;
  c->defeated = 0;
  c->symbol = RIVAL_SYMBOL;
  c->next_turn = 0;
  c->seq_num = world.char_seq_num++;

  do {
    c->pokemon.push_back(create_pokemon(0));
  } while (c->pokemon.size() < 6 && ((std::rand() % 10) > 3));

  heap_insert(&world.cur_map->turn, c);
}

void new_swimmer()
{
  pair_t pos;
  npc *c;

  do {
    rand_pos(pos);
  } while (world.cur_map->map[pos[dim_y]][pos[dim_x]] != ter_water ||
           world.cur_map->cmap[pos[dim_y]][pos[dim_x]]);

  world.cur_map->cmap[pos[dim_y]][pos[dim_x]] = c = new npc;
  c->pos[dim_y] = pos[dim_y];
  c->pos[dim_x] = pos[dim_x];
  c->ctype = char_swimmer;
  c->mtype = move_swim;
  rand_dir(c->dir);
  c->defeated = 0;
  c->symbol = SWIMMER_SYMBOL;
  c->next_turn = 0;
  c->seq_num = world.char_seq_num++;

  do {
    c->pokemon.push_back(create_pokemon(0));
  } while (c->pokemon.size() < 6 && ((std::rand() % 10) > 3));

  heap_insert(&world.cur_map->turn, c);
}

void new_char_other()
{
  pair_t pos;
  npc *c;

  do {
    rand_pos(pos);
  } while (world.rival_dist[pos[dim_y]][pos[dim_x]] == DIJKSTRA_PATH_MAX ||
           world.rival_dist[pos[dim_y]][pos[dim_x]] < 0                  ||
           world.cur_map->cmap[pos[dim_y]][pos[dim_x]]                   ||
           pos[dim_x] < 3 || pos[dim_x] > MAP_X - 4                      ||
           pos[dim_y] < 3 || pos[dim_y] > MAP_Y - 4);

  world.cur_map->cmap[pos[dim_y]][pos[dim_x]] = c = new npc;
  c->pos[dim_y] = pos[dim_y];
  c->pos[dim_x] = pos[dim_x];
  c->ctype = char_other;
  switch (rand() % 4) {
  case 0:
    c->mtype = move_pace;
    c->symbol = PACER_SYMBOL;
    break;
  case 1:
    c->mtype = move_wander;
    c->symbol = WANDERER_SYMBOL;
    break;
  case 2:
    c->mtype = move_sentry;
    c->symbol = SENTRY_SYMBOL;
    break;
  case 3:
    c->mtype = move_explore;
    c->symbol = EXPLORER_SYMBOL;
    break;
  }
  rand_dir(c->dir);
  c->defeated = 0;
  c->next_turn = 0;
  c->seq_num = world.char_seq_num++;

  do {
    c->pokemon.push_back(create_pokemon(0));
  } while (c->pokemon.size() < 6 && ((std::rand() % 10) > 3));

  heap_insert(&world.cur_map->turn, c);
}

void place_characters()
{
  world.cur_map->num_trainers = 3;

  //Always place a hiker and a rival, then place a random number of others
  new_hiker();
  new_rival();
  new_swimmer();
  do {
    //higher probability of non- hikers and rivals
    switch(rand() % 10) {
    case 0:
      new_hiker();
      break;
    case 1:
      new_rival();
      break;
    case 2:
      new_swimmer();
      break;
    default:
      new_char_other();
      break;
    }
    /* Game attempts to continue to place trainers until the probability *
     * roll fails, but if the map is full (or almost full), it's         *
     * impossible (or very difficult) to continue to add, so we abort if *
     * we've tried MAX_TRAINER_TRIES times.                              */
  } while (++world.cur_map->num_trainers < MIN_TRAINERS ||
           ((rand() % 100) < ADD_TRAINER_PROB));
}

void generate_pokemon_IVs(Pokemon* pokemon) {
  int i;

  for (i = 0; i < 6; i++) {
    // Should give a random stat distribution from [0, 15]
    pokemon->stat_ivs.push_back(std::rand() % 16);
  }
}

void set_pokemon_stats(Pokemon* pokemon) {
  int count;
  int stat_val;

  generate_pokemon_IVs(pokemon);
  count = 0;
  for (CSV* pokemon_stat : pokemon_stats) {
    Pokemon_Stats* temp_stat = dynamic_cast<Pokemon_Stats*>(pokemon_stat);
    if (temp_stat->pokemon_id == pokemon->id) {
      if (count == 0) {
        stat_val = ((((temp_stat->base_stat + pokemon->stat_ivs[count]) * 2) * pokemon->pokemon_level) / 100) + pokemon->pokemon_level + 10;
        pokemon->max_health = stat_val;
      } else {
        stat_val = ((((temp_stat->base_stat + pokemon->stat_ivs[count]) * 2) * pokemon->pokemon_level) / 100) + 5;
      }
      pokemon->stats.push_back(stat_val);
      pokemon->stat_efforts.push_back(temp_stat->effort);
      count++;
    }
  }

  pokemon->is_shiny = (rand() % 8192 == 0);
}

void set_pokemon_types(Pokemon *pokemon) {
  for (CSV* pokemon_type : pokemon_types) {
    Pokemon_Type* temp_type = dynamic_cast<Pokemon_Type*>(pokemon_type);
    if (temp_type->pokemon_id == pokemon->id) {
      pokemon->type_ids.push_back(temp_type->type_id);
    }
  }
}

std::string get_pokemon_move_name(Pokemon_Move* poke_move) {
  std::string move_name = "";
  for (CSV* item : moves) {
    Move* temp_move = dynamic_cast<Move*>(item);
    if (temp_move->id == poke_move->move_id) {
      move_name = temp_move->identifier;
    }
  }
  return move_name;
}

int get_pokemon_move_power(Pokemon_Move* poke_move) {
  int move_power;
  for (CSV* item : moves) {
    Move* temp_move = dynamic_cast<Move*>(item);
    if (temp_move->id == poke_move->move_id) {
      move_power = temp_move->power;
    }
  }
  return move_power;
}

int get_pokemon_move_type_id(Pokemon_Move* poke_move) {
  int type_id;
  for (CSV* item : moves) {
    Move* temp_move = dynamic_cast<Move*>(item);
    if (temp_move->id == poke_move->move_id) {
      type_id = temp_move->type_id;
    }
  }
  return type_id;
}

int get_pokemon_move_accuracy(Pokemon_Move* poke_move) {
  int move_accuracy;
  for (CSV* item : moves) {
    Move* temp_move = dynamic_cast<Move*>(item);
    if (temp_move->id == poke_move->move_id) {
      move_accuracy = temp_move->accuracy;
    }
  }
  return move_accuracy;
}

int get_pokemon_move_pp(Pokemon_Move* poke_move) {
  int move_pp;
  for (CSV* item : moves) {
    Move* temp_move = dynamic_cast<Move*>(item);
    if (temp_move->id == poke_move->move_id) {
      move_pp = temp_move->pp;
    }
  }
  return move_pp;
}

int get_pokemon_move_priority(Pokemon_Move* poke_move) {
  int move_priority;
  for (CSV* item : moves) {
    Move* temp_move = dynamic_cast<Move*>(item);
    if (temp_move->id == poke_move->move_id) {
      move_priority = temp_move->priority;
    }
  }
  return move_priority;
}

std::vector<Pokemon_Move*> get_valid_pokemon_moves(Pokemon* pokemon) {
  // This grabs valid pokemon moves
  std::vector<Pokemon_Move*> valid_moves;
  int already_added = 0;
  for (CSV* pocket_monster_move : pokemon_moves) {
    Pokemon_Move* temp_poke_move = dynamic_cast<Pokemon_Move*>(pocket_monster_move);
    if (pokemon->species_id == temp_poke_move->pokemon_id && temp_poke_move->pokemon_move_method_id == 1 && temp_poke_move->level <= pokemon->pokemon_level) {
      for (Pokemon_Move* valid_move : valid_moves) {
        if (valid_move->move_id == temp_poke_move->move_id) {
          already_added = 1;
        }
      }
      if (!already_added) {
        valid_moves.push_back(temp_poke_move);
      }
      already_added = 0;
    }
  }

  return valid_moves;
}

Pokemon* create_pokemon(int random) {
  int rand_pokemon_id, poke_level, distance, min_level, max_level, move_limit, i, index;
  Pokemon* rand_pokemon;
  std::vector<Pokemon_Move*> valid_moves;

  rand_pokemon_id = (std::rand() % pokemon.size()) + 1;
  
  rand_pokemon = new Pokemon(*dynamic_cast<Pokemon*>(pokemon[rand_pokemon_id]));
  // generate level
  distance = (abs(world.cur_idx[dim_x] - (WORLD_SIZE / 2)) + abs(world.cur_idx[dim_y] - (WORLD_SIZE / 2)));
  min_level = distance > 200 ? ((distance - 200)/2) : 1;
  max_level = distance > 200 ? 100 : (distance/2) + 1;

  poke_level = (std::rand() % ((max_level - min_level) + 1)) + min_level;
  rand_pokemon->pokemon_level = poke_level - 1;
  // generate moves
  move_limit = random == 1 ? 2 : 4;
  // Ensure there are moves for the pokemon
  do {
    rand_pokemon->pokemon_level++;
    valid_moves = get_valid_pokemon_moves(rand_pokemon);
  } while (valid_moves.size() == 0);

  for (i = 0; i < move_limit; i++) {
    if (valid_moves.size() >= 1) {
      index = (std::rand() % valid_moves.size());
      rand_pokemon->moves.push_back(valid_moves[index]);
      rand_pokemon->max_pps.push_back(get_pokemon_move_pp(valid_moves[index]));
      rand_pokemon->current_pps.push_back(get_pokemon_move_pp(valid_moves[index]));
      // Remove the move to prevent rand giving the same move back
      valid_moves.erase(valid_moves.begin() + index);
    }
  }
  // Generate stats
  set_pokemon_stats(rand_pokemon);
  // Generate gender
  rand_pokemon->gender = std::rand() % 2;
  // Generate types
  set_pokemon_types(rand_pokemon);
  return rand_pokemon;
}

void init_pc()
{
  int x, y;

  do {
    x = rand() % (MAP_X - 2) + 1;
    y = rand() % (MAP_Y - 2) + 1;
  } while (world.cur_map->map[y][x] != ter_path);

  world.pc.pos[dim_x] = x;
  world.pc.pos[dim_y] = y;
  world.pc.symbol = PC_SYMBOL;

  world.cur_map->cmap[y][x] = &world.pc;
  world.pc.next_turn = 0;

  if (starter_pokemon) {
    std::vector<Pokemon_Move*> valid_moves;
    world.pc.pokemon.push_back(starter_pokemon);

    // Sets pokemon level
    world.pc.pokemon[0]->pokemon_level = 1;
    // Sets pokemon moves
    valid_moves = get_valid_pokemon_moves(world.pc.pokemon[0]);
    for (Pokemon_Move* valid_move : valid_moves) {
      if (world.pc.pokemon[0]->moves.size() < 4) {
        world.pc.pokemon[0]->moves.push_back(valid_move);
        world.pc.pokemon[0]->max_pps.push_back(get_pokemon_move_pp(valid_move));
        world.pc.pokemon[0]->current_pps.push_back(get_pokemon_move_pp(valid_move));
      }
    }
    // Sets pokemon stats
    set_pokemon_stats(world.pc.pokemon[0]);
    // Set pokemon gender
    world.pc.pokemon[0]->gender = std::rand() % 2;
    world.pc.pokemon[0]->knocked_out = 0;
    // Set pokemon types
    set_pokemon_types(world.pc.pokemon[0]);
  } else {
    io_reset_terminal();
    std::cerr << "Somehow the starter pokemon was not chosen" << std::endl;
    exit(1);
  }
  world.pc.potions = 5;
  world.pc.revives = 3;
  world.pc.pokeballs = 10;
  world.pc.all_knocked_out = 1;
  world.pc.seq_num = world.char_seq_num++;

  heap_insert(&world.cur_map->turn, &world.pc);
}

void place_pc()
{
  character *c;

  if (world.pc.pos[dim_x] == 1) {
    world.pc.pos[dim_x] = MAP_X - 2;
  } else if (world.pc.pos[dim_x] == MAP_X - 2) {
    world.pc.pos[dim_x] = 1;
  } else if (world.pc.pos[dim_y] == 1) {
    world.pc.pos[dim_y] = MAP_Y - 2;
  } else if (world.pc.pos[dim_y] == MAP_Y - 2) {
    world.pc.pos[dim_y] = 1;
  }

  world.cur_map->cmap[world.pc.pos[dim_y]][world.pc.pos[dim_x]] = &world.pc;

  if ((c = (character *) heap_peek_min(&world.cur_map->turn))) {
    world.pc.next_turn = c->next_turn;
  } else {
    world.pc.next_turn = 0;
  }
}

// New map expects cur_idx to refer to the index to be generated.  If that
// map has already been generated then the only thing this does is set
// cur_map.
int new_map(int teleport)
{
  int d, p;
  int e, w, n, s;
  int x, y;
  
  if (world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x]]) {
    world.cur_map = world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x]];
    place_pc();

    return 0;
  }

  world.cur_map = new map;
  world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x]] = world.cur_map;
    

  smooth_height(world.cur_map);
  
  if (!world.cur_idx[dim_y]) {
    n = -1;
  } else if (world.world[world.cur_idx[dim_y] - 1][world.cur_idx[dim_x]]) {
    n = world.world[world.cur_idx[dim_y] - 1][world.cur_idx[dim_x]]->s;
  } else {
    n = 3 + rand() % (MAP_X - 6);
  }
  if (world.cur_idx[dim_y] == WORLD_SIZE - 1) {
    s = -1;
  } else if (world.world[world.cur_idx[dim_y] + 1][world.cur_idx[dim_x]]) {
    s = world.world[world.cur_idx[dim_y] + 1][world.cur_idx[dim_x]]->n;
  } else  {
    s = 3 + rand() % (MAP_X - 6);
  }
  if (!world.cur_idx[dim_x]) {
    w = -1;
  } else if (world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x] - 1]) {
    w = world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x] - 1]->e;
  } else {
    w = 3 + rand() % (MAP_Y - 6);
  }
  if (world.cur_idx[dim_x] == WORLD_SIZE - 1) {
    e = -1;
  } else if (world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x] + 1]) {
    e = world.world[world.cur_idx[dim_y]][world.cur_idx[dim_x] + 1]->w;
  } else {
    e = 3 + rand() % (MAP_Y - 6);
  }
  
  map_terrain(world.cur_map, n, s, e, w);
     
  place_boulders(world.cur_map);
  place_trees(world.cur_map);
  build_paths(world.cur_map);
  d = (abs(world.cur_idx[dim_x] - (WORLD_SIZE / 2)) +
       abs(world.cur_idx[dim_y] - (WORLD_SIZE / 2)));
  p = d > 200 ? 5 : (50 - ((45 * d) / 200));
  //  printf("d=%d, p=%d\n", d, p);
  if ((rand() % 100) < p || !d) {
    place_pokemart(world.cur_map);
  }
  if ((rand() % 100) < p || !d) {
    place_center(world.cur_map);
  }

  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      world.cur_map->cmap[y][x] = NULL;
    }
  }

  heap_init(&world.cur_map->turn, cmp_char_turns, delete_character);

  if ((world.cur_idx[dim_x] == WORLD_SIZE / 2) &&
      (world.cur_idx[dim_y] == WORLD_SIZE / 2)) {
    init_pc();
  } else {
    place_pc();
  }

  pathfind(world.cur_map);
  if (teleport) {
    do {
      world.cur_map->cmap[world.pc.pos[dim_y]][world.pc.pos[dim_x]] = NULL;
      world.pc.pos[dim_x] = rand_range(1, MAP_X - 2);
      world.pc.pos[dim_y] = rand_range(1, MAP_Y - 2);
    } while (world.cur_map->cmap[world.pc.pos[dim_y]][world.pc.pos[dim_x]] ||
             (move_cost[char_pc][world.cur_map->map[world.pc.pos[dim_y]]
                                                   [world.pc.pos[dim_x]]] ==
              DIJKSTRA_PATH_MAX)                                           ||
             world.rival_dist[world.pc.pos[dim_y]][world.pc.pos[dim_x]] < 0);
    world.cur_map->cmap[world.pc.pos[dim_y]][world.pc.pos[dim_x]] = &world.pc;
    pathfind(world.cur_map);
  }
  
  place_characters();

  return 0;
}

// The world is global because of its size, so init_world is parameterless
void init_world()
{
  world.quit = 0;
  world.cur_idx[dim_x] = world.cur_idx[dim_y] = WORLD_SIZE / 2;
  world.char_seq_num = 0;
  new_map(0);
}

void delete_pokemon() {
  int x, y, i, j;
  long unsigned int z;
  character *n;

  for (y = 0; y < WORLD_SIZE; y++) {
    for (x = 0; x < WORLD_SIZE; x++) {
      if (world.world[y][x]) {
        for (i = 0; i < MAP_Y; i++) {
          for (j = 0; j < MAP_X; j++) {
            // if Character exists
            if (dynamic_cast<character *> (world.cur_map->cmap[i][j])) {
              n = (character *) (world.cur_map->cmap[i][j]);
              for (z = 0; z < n->pokemon.size(); z++) {
                delete (n->pokemon[z]);
              }
            }
          }
        }
      }
    }
  }
}

void delete_world()
{
  int x, y;

  delete_pokemon();

  for (y = 0; y < WORLD_SIZE; y++) {
    for (x = 0; x < WORLD_SIZE; x++) {
      if (world.world[y][x]) {
        heap_delete(&world.world[y][x]->turn);
        delete world.world[y][x];
        world.world[y][x] = NULL;
      }
    }
  }

}

void print_hiker_dist()
{
  int x, y;

  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      if (world.hiker_dist[y][x] == DIJKSTRA_PATH_MAX) {
        printf("   ");
      } else {
        printf(" %02d", world.hiker_dist[y][x] % 100);
      }
    }
    printf("\n");
  }
}

void print_rival_dist()
{
  int x, y;

  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      if (world.rival_dist[y][x] == DIJKSTRA_PATH_MAX ||
          world.rival_dist[y][x] < 0) {
        printf("   ");
      } else {
        printf(" %02d", world.rival_dist[y][x] % 100);
      }
    }
    printf("\n");
  }
}

void leave_map(pair_t d)
{
  if (d[dim_x] == 0) {
    world.cur_idx[dim_x]--;
  } else if (d[dim_y] == 0) {
    world.cur_idx[dim_y]--;
  } else if (d[dim_x] == MAP_X - 1) {
    world.cur_idx[dim_x]++;
  } else {
    world.cur_idx[dim_y]++;
  }
  new_map(0);
}

void choose_starter(std::vector<CSV*> pokemon) {
  int starter;
  CSV* choice;

  //Appearance in global array Bulbasaur = 1, charmandar = 4, squirtle = 7
  switch (starter = io_starter_screen()) {
    case '1':
      choice = pokemon[6];
      starter_pokemon = new Pokemon(*dynamic_cast<Pokemon*>(choice));
      break;
    case '2':
      choice = pokemon[0];
      starter_pokemon = new Pokemon(*dynamic_cast<Pokemon*>(choice));
      break;
    case '3':
      choice = pokemon[3];
      starter_pokemon = new Pokemon(*dynamic_cast<Pokemon*>(choice));
      break;
    default:
      break;
  }
}

void game_loop()
{
  character *c;
  npc *n;
  pc *p;
  pair_t d;
  
  while (!world.quit) {
    c = (character *) heap_remove_min(&world.cur_map->turn);
    n = dynamic_cast<npc *> (c);
    p = dynamic_cast<pc *> (c);

    move_func[n ? n->mtype : move_pc](c, d);

    world.cur_map->cmap[c->pos[dim_y]][c->pos[dim_x]] = NULL;
    if (p && (d[dim_x] == 0 || d[dim_x] == MAP_X - 1 ||
              d[dim_y] == 0 || d[dim_y] == MAP_Y - 1)) {
      leave_map(d);
      d[dim_x] = c->pos[dim_x];
      d[dim_y] = c->pos[dim_y];
    }
    world.cur_map->cmap[d[dim_y]][d[dim_x]] = c;

    if (p) {
      pathfind(world.cur_map);
    }

    c->next_turn += move_cost[n ? n->ctype : char_pc]
                             [world.cur_map->map[d[dim_y]][d[dim_x]]];

    c->pos[dim_y] = d[dim_y];
    c->pos[dim_x] = d[dim_x];

    heap_insert(&world.cur_map->turn, c);
  }
}

bool isInteger(const std::string& s) {
    if (s.empty()) {
        return false;
    }
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

std::vector<CSV*> getCSVFile(std::string csvFile, int switchFile) {
  std::vector<CSV*> v;
  char *fileTry2;

  const char* extension = ".csv";
  size_t csvFileLength = csvFile.length();
  size_t extensionLength = strlen(extension);
  char* fullFile = new char[csvFileLength + extensionLength + 1];
  strcpy(fullFile, csvFile.c_str());
  strcat(fullFile, extension);

  const char *dbpath = "/pokedex/pokedex/data/csv/";

  const char *universalPath = "/share/cs327";
  char *home = getenv("HOME"); //takes env variable 
  char *filename =  fullFile;
  int len = strlen(universalPath) + strlen(dbpath) + strlen(filename) + 1;

  char *file = malloc(len+1);
  strcpy(file, universalPath);
  strcat(file, dbpath);
  strcat(file, filename);

  std::ifstream i(file);
  if (!i) {
    i.close();
    std::cout << "Could not find file in /share/cs327/..." << std::endl;
    len = strlen(home) + strlen(dbpath) + strlen(filename) + 1;
    fileTry2 = malloc(len+1);
    strcpy(fileTry2, home);
    strcat(fileTry2, dbpath);
    strcat(fileTry2, filename);

    std::ifstream i(fileTry2);
    if (!i) {
      std::cout << "universalFile: " << fileTry2 << std::endl;
      std::cerr << "No valid file locations" << std::endl;
      exit(1);
    }
  }

  std::string line;
  std::getline(i, line);
  std::string word;

  while (std::getline(i, line)) {
    std::vector<std::string> values;
    std::istringstream lineStream(line);
    while (std::getline(lineStream, word, ',')) {
      if (word == "") {
        std::string newWord = "2147483647";
        values.push_back(newWord);
      } else {
        values.push_back(word);
      }
    }
    CSV* item = nullptr;
    switch (switchFile) {
      case 0:
      {
        if (values.size() < 5) {
          std::string fixEnd = "2147483647";
          values.push_back(fixEnd);
        }
        item = new Stat(std::stoi(values[0]), std::stoi(values[1]), std::stoi(values[3]), std::stoi(values[4]), values[2]);
        v.push_back(item);
      }
        break;
      case 1:
      {
        if (values.size() < 8) {
          std::string fixEnd = "2147483647";
          values.push_back(fixEnd);
        }
        item = new Pokemon(std::stoi(values[0]), std::stoi(values[2]), std::stoi(values[3]), std::stoi(values[4]), std::stoi(values[5]), std::stoi(values[6]), std::stoi(values[7]), values[1]);
        v.push_back(item);
      }
        break;
      case 2:
      {
        if (values.size() < 6) {
          std::string fixEnd = "2147483647";
          values.push_back(fixEnd);
        }
        item = new Pokemon_Move(std::stoi(values[0]), std::stoi(values[1]), std::stoi(values[2]), std::stoi(values[3]), std::stoi(values[4]), std::stoi(values[5]));
        v.push_back(item);
      }
        break;
      case 3:
      {
        if (values.size() < 20) {
          std::string fixEnd = "2147483647";
          values.push_back(fixEnd);
        }
        item = new Pokemon_Species(std::stoi(values[0]), std::stoi(values[2]), std::stoi(values[3]), std::stoi(values[4]), std::stoi(values[5]), std::stoi(values[6]), std::stoi(values[7]), std::stoi(values[8]), std::stoi(values[9]), std::stoi(values[10]), std::stoi(values[11]), std::stoi(values[12]), std::stoi(values[13]), std::stoi(values[14]), std::stoi(values[15]), std::stoi(values[16]), std::stoi(values[17]), std::stoi(values[18]), std::stoi(values[19]), values[1]);
        v.push_back(item);
      } 
        break;
      case 4:
      {
        if (values.size() < 4) {
          std::string fixEnd = "2147483647";
          values.push_back(fixEnd);
        }
        item = new Pokemon_Stats(std::stoi(values[0]), std::stoi(values[1]), std::stoi(values[2]), std::stoi(values[3]));
        v.push_back(item);
      }  
        break;
      case 5:
      {
        if (values.size() < 3) {
          std::string fixEnd = "2147483647";
          values.push_back(fixEnd);
        }
        item = new Pokemon_Type(std::stoi(values[0]), std::stoi(values[1]), std::stoi(values[2]));
        v.push_back(item);
      }  
        break;
      case 6:
      {
        if (values.size() < 15) {
          std::string fixEnd = "2147483647";
          values.push_back(fixEnd);
        }
        item = new Move(std::stoi(values[0]), std::stoi(values[2]), std::stoi(values[3]), std::stoi(values[4]), std::stoi(values[5]), std::stoi(values[6]), std::stoi(values[7]), std::stoi(values[8]), std::stoi(values[9]), std::stoi(values[10]), std::stoi(values[11]), std::stoi(values[12]), std::stoi(values[13]), std::stoi(values[14]), values[1]);
        v.push_back(item);
      }  
        break;
      case 7:
      {
        if (values.size() < 3) {
          std::string fixEnd = "2147483647";
          values.push_back(fixEnd);
        }
        item = new Experience(std::stoi(values[0]), std::stoi(values[1]), std::stoi(values[2]));
        v.push_back(item);
      }  
        break;
      case 8:
      {
        if (values.size() < 3) {
          std::string fixEnd = "";
          values.push_back(fixEnd);
        }
        item = new Type_Name(std::stoi(values[0]), std::stoi(values[1]), values[2]);
        v.push_back(item);
      }
    }
  }

  i.close();
  delete[] fullFile;
  free(file);

  return v;
}

void usage(char *s)
{
  fprintf(stderr, "Usage: %s [-s|--seed <seed>]\n", s);

  exit(1);
}

int main(int argc, char *argv[])
{
  struct timeval tv;
  uint32_t seed;
  int long_arg;
  int do_seed;
  //  char c;
  //  int x, y;
  int i;

  do_seed = 1;
  
  if (argc > 1) {
    for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0) {
      if (argv[i][0] == '-') { /* All switches start with a dash */
        if (argv[i][1] == '-') {
          argv[i]++;    /* Make the argument have a single dash so we can */
          long_arg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1]) {
        case 's':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-seed")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%u", &seed) /* Argument is not an integer */) {
            usage(argv[0]);
          }
          do_seed = 0;
          break;
        default:
          usage(argv[0]);
        }
      } else { /* No dash */
        usage(argv[0]);
      }
    }
  }

  // Grab all csv files
  stats = getCSVFile("stats", 0);
  pokemon = getCSVFile("pokemon", 1);
  pokemon_moves = getCSVFile("pokemon_moves", 2);
  pokemon_species = getCSVFile("pokemon_species", 3);
  pokemon_stats = getCSVFile("pokemon_stats", 4);
  pokemon_types = getCSVFile("pokemon_types", 5);
  moves = getCSVFile("moves", 6);
  experience = getCSVFile("experience", 7);
  type_names = getCSVFile("type_names", 8);

  if (do_seed) {
    /* Allows me to start the game more than once *
     * per second, as opposed to time().          */
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  printf("Using seed: %u\n", seed);
  srand(seed);

  io_init_terminal();

  choose_starter(pokemon);
  
  init_world();

  /* print_hiker_dist(); */
  
  /*
  do {
    print_map();  
    printf("Current position is %d%cx%d%c (%d,%d).  "
           "Enter command: ",
           abs(world.cur_idx[dim_x] - (WORLD_SIZE / 2)),
           world.cur_idx[dim_x] - (WORLD_SIZE / 2) >= 0 ? 'E' : 'W',
           abs(world.cur_idx[dim_y] - (WORLD_SIZE / 2)),
           world.cur_idx[dim_y] - (WORLD_SIZE / 2) <= 0 ? 'N' : 'S',
           world.cur_idx[dim_x] - (WORLD_SIZE / 2),
           world.cur_idx[dim_y] - (WORLD_SIZE / 2));
    scanf(" %c", &c);
    switch (c) {
    case 'n':
      if (world.cur_idx[dim_y]) {
        world.cur_idx[dim_y]--;
        new_map();
      }
      break;
    case 's':
      if (world.cur_idx[dim_y] < WORLD_SIZE - 1) {
        world.cur_idx[dim_y]++;
        new_map();
      }
      break;
    case 'e':
      if (world.cur_idx[dim_x] < WORLD_SIZE - 1) {
        world.cur_idx[dim_x]++;
        new_map();
      }
      break;
    case 'w':
      if (world.cur_idx[dim_x]) {
        world.cur_idx[dim_x]--;
        new_map();
      }
      break;
     case 'q':
      break;
    case 'f':
      scanf(" %d %d", &x, &y);
      if (x >= -(WORLD_SIZE / 2) && x <= WORLD_SIZE / 2 &&
          y >= -(WORLD_SIZE / 2) && y <= WORLD_SIZE / 2) {
        world.cur_idx[dim_x] = x + (WORLD_SIZE / 2);
        world.cur_idx[dim_y] = y + (WORLD_SIZE / 2);
        new_map();
      }
      break;
    case '?':
    case 'h':
      printf("Move with 'e'ast, 'w'est, 'n'orth, 's'outh or 'f'ly x y.\n"
             "Quit with 'q'.  '?' and 'h' print this help message.\n");
      break;
    default:
      fprintf(stderr, "%c: Invalid input.  Enter '?' for help.\n", c);
      break;
    }
  } while (c != 'q');

  */

  game_loop();
  
  delete_world();

  io_reset_terminal();

  // For each csv file
  for (CSV* ptr : stats) {
    delete ptr;
  }
  stats.clear();
  for (CSV* ptr : pokemon) {
    delete ptr;
  }
  pokemon.clear();
  for (CSV* ptr : pokemon_moves) {
    delete ptr;
  }
  pokemon_moves.clear();
  for (CSV* ptr : pokemon_species) {
    delete ptr;
  }
  pokemon_species.clear();
  for (CSV* ptr : pokemon_stats) {
    delete ptr;
  }
  pokemon_stats.clear();
  for (CSV* ptr : pokemon_types) {
    delete ptr;
  }
  pokemon_types.clear();
  for (CSV* ptr : moves) {
    delete ptr;
  }
  moves.clear();
  for (CSV* ptr : experience) {
    delete ptr;
  }
  experience.clear();
  for (CSV* ptr : type_names) {
    delete ptr;
  }
  type_names.clear();
  
  return 0;
}
