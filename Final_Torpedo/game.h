#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "debugmalloc.h"
#include "stroktok.h"
#include "loves.h"

void put_navy_on_map(Maptile** mappp, int map_size, Hajo* navy, int navy_size);
void place_all_ships(Game* g, Maptile** mappp, int map_size, Hajo* new_navy, bool is_cp_game, int* palette);
void clear_navy(Hajo* navy, int len);
int set_navy_size(int map_size);
void create_state(State* s, Maptile** pmap1, Maptile** pmap2, Hajo* navy1, Hajo* navy2);
void create_game(Game* g, State s, char diff, int size, int gm);
void salvo(Game* g, int* p1intact, int* cp2intact, int* cpmiss, int pov, bool* if_hit, int* damaged_ship, int* last_col, int* last_row, int* first_hit_col, int* first_hit_row, int* align, int* palette, int* global);
void init_game(Game* g, int diff, int size, int gm);
void load_game(Game* g);
int get_next_int(FILE* fp);
void get_new_file(char* name);

#endif //GAME_H_INCLUDED