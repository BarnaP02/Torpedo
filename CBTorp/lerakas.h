#ifndef LERAKAS_H_INCLUDED
#define LERAKAS_H_INCLUDED

#include "debugmalloc.h"
#include "stroktok.h"

bool placable(int rown, int col, char align, int index, int map_size, Hajo* new_navy, int navy_size);
void get_ship_coords(char* coln, int* row, char* align, int index, int map_size);
void get_cp_ship_coords(int* coln, int* row, bool* align, int index, int map_size);
void place_ship(int* navy, int index, Hajo* new_navy, int map_size, int navy_size);
void cp_place_ship(int* navy, int index, Hajo* new_navy, int map_size, int navy_size);
void show_pov(Game* g, int pov, int* palette);

#endif //LERAKAS_H_INCLUDED