#ifndef LOVES_H_INCLUDED
#define LOVES_H_INCLUDED

#include "debugmalloc.h"
#include "stroktok.h"
/*
bool placable(int rown, int col, char align, int index, int map_size, Hajo* new_navy, int navy_size);
void get_ship_coords(char* coln, int* row, char* align, int index, int map_size);
void get_cp_ship_coords(int* coln, int* row, bool* align, int index, int map_size);
void place_ship(int* navy, int index, Hajo* new_navy, int map_size, int navy_size);
void cp_place_ship(int* navy, int index, Hajo* new_navy, int map_size, int navy_size);
void show_pov(Game* g, int pov, int* palette);*/

void menu(Game* g, int* palette, int* global);
void get_target(Game* g, int* coln, int* row, int* palette, int* global);
void ship_sunk(Maptile** map, Hajo* h, int size);
bool can_shoot_left(int this_x, int this_y, Maptile** map);
bool can_shoot_up(int this_x, int this_y, Maptile** map);
bool can_shoot_right(int this_x, int this_y, Maptile** map, int size);
bool can_shoot_down(int this_x, int this_y, Maptile** map, int size);
void reset_cpfire(int* first_hit_col, int* first_hit_row, int* damaged_ship, int* align);
void cpfire(Game* g, int* p1intact, int* cp2intact, int* cpmiss, bool* if_hit, bool* ship_ended, int* damaged_ship, int* last_col, int* last_row, int* first_hit_col, int* first_hit_row, int* align);
void fire(Game* g, bool* if_hit, int pov, int* palette, int* global)

#endif //LOVES_H_INCLUDED