#ifndef POKE327_H
# define POKE327_H

# include <cstdlib>
# include <cstring>
# include <string>
# include <cassert>
# include <vector>
# include <iostream>
# include <climits>

# include "heap.h"
# include "character.h"
# include "pair.h"

#define malloc(size) ({                 \
  char *_tmp;                           \
  assert(_tmp = (char *) malloc(size)); \
  _tmp;                                 \
})

/* Returns true if random float in [0,1] is less than *
 * numerator/denominator.  Uses only integer math.    */
# define rand_under(numerator, denominator) \
  (rand() < ((RAND_MAX / denominator) * numerator))

/* Returns random integer in [min, max]. */
# define rand_range(min, max) ((rand() % (((max) + 1) - (min))) + (min))

# define UNUSED(f) ((void) f)

#define MAP_X              80
#define MAP_Y              21
#define MIN_TREES          10
#define MIN_BOULDERS       10
#define TREE_PROB          95
#define BOULDER_PROB       95
#define WORLD_SIZE         401

#define MIN_TRAINERS     7
#define ADD_TRAINER_PROB 60

#define MOUNTAIN_SYMBOL       '%'
#define BOULDER_SYMBOL        '0'
#define TREE_SYMBOL           '4'
#define FOREST_SYMBOL         '^'
#define GATE_SYMBOL           '#'
#define PATH_SYMBOL           '#'
#define BAILEY_SYMBOL         '#'
#define POKEMART_SYMBOL       'M'
#define POKEMON_CENTER_SYMBOL 'C'
#define TALL_GRASS_SYMBOL     ':'
#define SHORT_GRASS_SYMBOL    '.'
#define WATER_SYMBOL          '~'
#define ERROR_SYMBOL          '&'

#define PC_SYMBOL       '@'
#define HIKER_SYMBOL    'h'
#define RIVAL_SYMBOL    'r'
#define EXPLORER_SYMBOL 'e'
#define SENTRY_SYMBOL   's'
#define PACER_SYMBOL    'p'
#define SWIMMER_SYMBOL  'm'
#define WANDERER_SYMBOL 'w'

#define mappair(pair) (m->map[pair[dim_y]][pair[dim_x]])
#define mapxy(x, y) (m->map[y][x])
#define heightpair(pair) (m->height[pair[dim_y]][pair[dim_x]])
#define heightxy(x, y) (m->height[y][x])

typedef enum __attribute__ ((__packed__)) terrain_type {
  ter_boulder,
  ter_tree,
  ter_path,
  ter_mart,
  ter_center,
  ter_grass,
  ter_clearing,
  ter_mountain,
  ter_forest,
  ter_water,
  ter_gate,
  ter_bailey,
  num_terrain_types,
  ter_debug
} terrain_type_t;

extern int32_t move_cost[num_character_types][num_terrain_types];

class CSV {
public:
  virtual ~CSV() {}
  virtual void print() = 0; // Pure virtual function
};

class Pokemon : public CSV {
  public:
    int id, species_id, height, weight, base_experience, order, is_default;
    std::string identifier;

    Pokemon(int id, int species_id, int height, int weight, int base_experience, int order, int is_default, std::string identifier) : id(id), species_id(species_id), height(height), weight(weight), base_experience(base_experience), order(order), is_default(is_default), identifier(identifier) {}

    void print() override {
      std::cout << "ID: " << (this->id == INT_MAX ? " " : std::to_string(this->id)) << std::endl;
      std::cout << "Identifier: " << this->identifier << std::endl;
      std::cout << "Species_id: " << (this->species_id == INT_MAX ? " " : std::to_string(this->species_id)) << std::endl;
      std::cout << "Height: " << (this->height == INT_MAX ? " " : std::to_string(this->height)) << std::endl;
      std::cout << "Weight: " << (this->weight == INT_MAX ? " " : std::to_string(this->weight)) << std::endl;
      std::cout << "Base_experience: " << (this->base_experience == INT_MAX ? " " : std::to_string(this->base_experience)) << std::endl;
      std::cout << "Order: " << (this->order == INT_MAX ? " " : std::to_string(this->order)) << std::endl;
      std::cout << "Is_default: " << (this->is_default == INT_MAX ? " " : std::to_string(this->is_default)) << std::endl;
    }
};

class Stat : public CSV {
  public:
    int id, damage_class_id, is_battle_only, game_index;
    std::string identifier;

    Stat(int id, int damage_class_id, int is_battle_only, int game_index, std::string identifier) : id(id), damage_class_id(damage_class_id), is_battle_only(is_battle_only), game_index(game_index), identifier(identifier) {}

    void print() override {
      std::cout << "ID: " << (this->id == INT_MAX ? " " : std::to_string(this->id)) << std::endl;
      std::cout << "Damage_class_id: " << (this->damage_class_id == INT_MAX ? " " : std::to_string(this->damage_class_id)) << std::endl;
      std::cout << "Identifier: " << this->identifier << std::endl;
      std::cout << "Is_battle_only: " << (this->is_battle_only == INT_MAX ? " " : std::to_string(this->is_battle_only)) << std::endl;
      std::cout << "Game_index: " << (this->game_index == INT_MAX ? " " : std::to_string(this->game_index)) << std::endl;
    }
};

class Pokemon_Move : public CSV {
  public:
    int pokemon_id, version_group_id, move_id, pokemon_move_method_id, level, order;

    Pokemon_Move(int pokemon_id, int version_group_id, int move_id, int pokemon_move_method_id, int level, int order) : pokemon_id(pokemon_id), version_group_id(version_group_id), move_id(move_id), pokemon_move_method_id(pokemon_move_method_id), level(level), order(order) {}

    void print() override {
      std::cout << "Pokemon_id: " << (this->pokemon_id == INT_MAX ? " " : std::to_string(this->pokemon_id)) << std::endl;
      std::cout << "Version_group_id: " << (this->version_group_id == INT_MAX ? " " : std::to_string(this->version_group_id)) << std::endl;
      std::cout << "Move_id: " << (this->move_id == INT_MAX ? " " : std::to_string(this->move_id)) << std::endl;
      std::cout << "Pokemon_move_method_id: " << (this->pokemon_move_method_id == INT_MAX ? " " : std::to_string(this->pokemon_move_method_id)) << std::endl;
      std::cout << "Level: " << (this->level == INT_MAX ? " " : std::to_string(this->level)) << std::endl;
      std::cout << "Order: " << (this->order == INT_MAX ? " " : std::to_string(this->order)) << std::endl;
    }
};

class Pokemon_Species : public CSV {
  public:
    int id, generation_id, evolves_from_species_id, evolution_chain_id, color_id, shape_id, habitat_id, gender_rate, capture_rate, base_happiness, is_baby, hatch_counter, has_gender_differences, growth_rate_id, forms_switchable, is_legendary, is_mythical, order, conquest_order;
    std::string identifier;

    Pokemon_Species(int id, int generation_id, int evolves_from_species_id, int evolution_chain_id, int color_id, int shape_id, int habitat_id, int gender_rate, int capture_rate, int base_happiness, int is_baby, int hatch_counter, int has_gender_differences, int growth_rate_id, int forms_switchable, int is_legendary, int is_mythical, int order, int conquest_order, std::string identifier) : id(id), generation_id(generation_id), evolves_from_species_id(evolves_from_species_id), evolution_chain_id(evolution_chain_id), color_id(color_id), shape_id(shape_id), habitat_id(habitat_id), gender_rate(gender_rate), capture_rate(capture_rate), base_happiness(base_happiness), is_baby(is_baby), hatch_counter(hatch_counter), has_gender_differences(has_gender_differences), growth_rate_id(growth_rate_id), forms_switchable(forms_switchable), is_legendary(is_legendary), is_mythical(is_mythical), order(order), conquest_order(conquest_order), identifier(identifier) {}

    void print() override {
      std::cout << "ID: " << (this->id == INT_MAX ? " " : std::to_string(this->id)) << std::endl;
      std::cout << "Identifier: " << this->identifier << std::endl;
      std::cout << "Generation_id: " << (this->generation_id == INT_MAX ? " " : std::to_string(this->generation_id)) << std::endl;
      std::cout << "Evolves_from_species_id: " << (this->evolves_from_species_id == INT_MAX ? " " : std::to_string(this->evolves_from_species_id)) << std::endl;
      std::cout << "Evolution_chain_id: " << (this->evolution_chain_id == INT_MAX ? " " : std::to_string(this->evolution_chain_id)) << std::endl;
      std::cout << "Color_id: " << (this->color_id == INT_MAX ? " " : std::to_string(this->color_id)) << std::endl;
      std::cout << "Shape_id: " << (this->shape_id == INT_MAX ? " " : std::to_string(this->shape_id)) << std::endl;
      std::cout << "Habitat_id: " << (this->habitat_id == INT_MAX ? " " : std::to_string(this->habitat_id)) << std::endl;
      std::cout << "Gender_rate: " << (this->gender_rate == INT_MAX ? " " : std::to_string(this->gender_rate)) << std::endl;
      std::cout << "Capture_rate: " << (this->capture_rate == INT_MAX ? " " : std::to_string(this->capture_rate)) << std::endl;
      std::cout << "Base_happiness: " << (this->base_happiness == INT_MAX ? " " : std::to_string(this->base_happiness)) << std::endl;
      std::cout << "Is_baby: " << (this->is_baby == INT_MAX ? " " : std::to_string(this->is_baby)) << std::endl;
      std::cout << "Hatch_counter: " << (this->hatch_counter == INT_MAX ? " " : std::to_string(this->hatch_counter)) << std::endl;
      std::cout << "Has_gender_differences: " << (this->has_gender_differences == INT_MAX ? " " : std::to_string(this->has_gender_differences)) << std::endl;
      std::cout << "Growth_rate_id: " << (this->growth_rate_id == INT_MAX ? " " : std::to_string(this->growth_rate_id)) << std::endl;
      std::cout << "Forms_switchable: " << (this->forms_switchable == INT_MAX ? " " : std::to_string(this->forms_switchable)) << std::endl;
      std::cout << "Is_legendary: " << (this->is_legendary == INT_MAX ? " " : std::to_string(this->is_legendary)) << std::endl;
      std::cout << "Is_mythical: " << (this->is_mythical == INT_MAX ? " " : std::to_string(this->is_mythical)) << std::endl;
      std::cout << "Order: " << (this->order == INT_MAX ? " " : std::to_string(this->order)) << std::endl;
      std::cout << "Conquest_order: " << (this->conquest_order == INT_MAX ? " " : std::to_string(this->conquest_order)) << std::endl;
    }
};

class Pokemon_Stats : public CSV {
  public:
    int pokemon_id, stat_id, base_stat, effort;

    Pokemon_Stats(int pokemon_id, int stat_id, int base_stat, int effort) : pokemon_id(pokemon_id), stat_id(stat_id), base_stat(base_stat), effort(effort) {}

    void print() override {
      std::cout << "Pokemon_id: " << (this->pokemon_id == INT_MAX ? " " : std::to_string(this->pokemon_id)) << std::endl;
      std::cout << "Stat_id: " << (this->stat_id == INT_MAX ? " " : std::to_string(this->stat_id)) << std::endl;
      std::cout << "Base_stat: " << (this->base_stat == INT_MAX ? " " : std::to_string(this->base_stat)) << std::endl;
      std::cout << "Effort: " << (this->effort == INT_MAX ? " " : std::to_string(this->effort)) << std::endl;
    }
};

class Pokemon_Type : public CSV {
  public:
    int pokemon_id, type_id, slot;

    Pokemon_Type(int pokemon_id, int type_id, int slot) : pokemon_id(pokemon_id), type_id(type_id), slot(slot) {}

    void print() override {
      std::cout << "Pokemon_id: " << (this->pokemon_id == INT_MAX ? " " : std::to_string(this->pokemon_id)) << std::endl;
      std::cout << "Type_id: " << (this->type_id == INT_MAX ? " " : std::to_string(this->type_id)) << std::endl;
      std::cout << "Slot: " << (this->slot == INT_MAX ? " " : std::to_string(this->slot)) << std::endl;
    }
};

class Move : public CSV {
  public:
    int id, generation_id, type_id, power, pp, accuracy, priority, target_id, damage_class_id, effect_id, effect_chance, contest_type_id, contest_effect_id, super_contest_effect_id;
    std::string identifier;

    Move(int id, int generation_id, int type_id, int power, int pp, int accuracy, int priority, int target_id, int damage_class_id, int effect_id, int effect_chance, int contest_type_id, int contest_effect_id, int super_contest_effect_id, std::string identifier) : id(id), generation_id(generation_id), type_id(type_id), power(power), pp(pp), accuracy(accuracy), priority(priority), target_id(target_id), damage_class_id(damage_class_id), effect_id(effect_id), effect_chance(effect_chance), contest_type_id(contest_type_id), contest_effect_id(contest_effect_id), super_contest_effect_id(super_contest_effect_id), identifier(identifier) {}

    void print() override {
      std::cout << "ID: " << (this->id == INT_MAX ? " " : std::to_string(this->id)) << std::endl;
      std::cout << "Identifier: " << this->identifier << std::endl;
      std::cout << "Generation_id: " << (this->generation_id == INT_MAX ? " " : std::to_string(this->generation_id)) << std::endl;
      std::cout << "Type_id: " << (this->type_id == INT_MAX ? " " : std::to_string(this->type_id)) << std::endl;
      std::cout << "Power: " << (this->power == INT_MAX ? " " : std::to_string(this->power)) << std::endl;
      std::cout << "pp: " << (this->pp == INT_MAX ? " " : std::to_string(this->pp)) << std::endl;
      std::cout << "Accuracy: " << (this->accuracy == INT_MAX ? " " : std::to_string(this->accuracy)) << std::endl;
      std::cout << "Priority: " << (this->priority == INT_MAX ? " " : std::to_string(this->priority)) << std::endl;
      std::cout << "Target_id: " << (this->target_id == INT_MAX ? " " : std::to_string(this->target_id)) << std::endl;
      std::cout << "Damage_class_id: " << (this->damage_class_id == INT_MAX ? " " : std::to_string(this->damage_class_id)) << std::endl;
      std::cout << "Effect_id: " << (this->effect_id == INT_MAX ? " " : std::to_string(this->effect_id)) << std::endl;
      std::cout << "Effect_chance: " << (this->effect_chance == INT_MAX ? " " : std::to_string(this->effect_chance)) << std::endl;
      std::cout << "Contest_type_id: " << (this->contest_type_id == INT_MAX ? " " : std::to_string(this->contest_type_id)) << std::endl;
      std::cout << "Contest_effect_id: " << (this->contest_effect_id == INT_MAX ? " " : std::to_string(this->contest_effect_id)) << std::endl;
      std::cout << "Super_contest_effect_id: " << (this->super_contest_effect_id == INT_MAX ? " " : std::to_string(this->super_contest_effect_id)) << std::endl;
    }
};

class Experience : public CSV {
  public:
    int growth_rate_id, level, experience;

    Experience(int growth_rate_id, int level, int experience) : growth_rate_id(growth_rate_id), level(level), experience(experience) {}

    void print() override {
      std::cout << "Growth_rate_id: " << (this->growth_rate_id == INT_MAX ? " " : std::to_string(this->growth_rate_id)) << std::endl;
      std::cout << "Level: " << (this->level == INT_MAX ? " " : std::to_string(this->level)) << std::endl;
      std::cout << "Experience: " << (this->experience == INT_MAX ? " " : std::to_string(this->experience)) << std::endl;
    }
};

class Type_Name : public CSV {
  public:
    int type_id, local_language_id;
    std::string name;

    Type_Name(int type_id, int local_language_id, std::string name) : type_id(type_id), local_language_id(local_language_id), name(name) {}

    void  print() override {
      std::cout << "Type_id: " << (this->type_id == INT_MAX ? " " : std::to_string(this->type_id)) << std::endl;
      std::cout << "Local_language_id: " << (this->local_language_id == INT_MAX ? " " : std::to_string(this->local_language_id)) << std::endl;
      std::cout << "Name: " << this->name << std::endl;
    }
};

class map {
 public:
  terrain_type_t map[MAP_Y][MAP_X];
  uint8_t height[MAP_Y][MAP_X];
  character *cmap[MAP_Y][MAP_X];
  heap_t turn;
  int32_t num_trainers;
  int8_t n, s, e, w;
};

class world {
 public:
  map *world[WORLD_SIZE][WORLD_SIZE];
  pair_t cur_idx;
  map *cur_map;
  /* Please distance maps in world, not map, since *
   * we only need one pair at any given time.      */
  int hiker_dist[MAP_Y][MAP_X];
  int rival_dist[MAP_Y][MAP_X];
  class pc pc;
  int quit;
  int add_trainer_prob;
  int char_seq_num;
};

/* Even unallocated, a WORLD_SIZE x WORLD_SIZE array of pointers is a very *
 * large thing to put on the stack.  To avoid that, world is a global.     */
extern class world world;

extern pair_t all_dirs[8];

#define rand_dir(dir) {     \
  int _i = rand() & 0x7;    \
  dir[0] = all_dirs[_i][0]; \
  dir[1] = all_dirs[_i][1]; \
}

typedef struct path {
  heap_node_t *hn;
  uint8_t pos[2];
  uint8_t from[2];
  int32_t cost;
} path_t;

int new_map(int teleport);
void pathfind(map *m);

#endif
