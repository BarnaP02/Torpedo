#ifndef STROKTOK_H_INCLUDED
#define STROKTOK_H_INCLUDED

#include "debugmalloc.h"

typedef struct Pont {
    int x;
    int y;
}Pont;
typedef struct Hajo {
    Pont b;
    Pont e;
    int intact_tiles;   /*elején a hajó hossza, találatkor --, ha 0, akkor a hajó elsüllyedt*/
}Hajo;
typedef struct Maptile {
    bool hit;
    char value;
}Maptile;
typedef struct State {
    Maptile** p1;
    Maptile** cp2;
    Hajo* p1navy;
    Hajo* cp2navy;
}State;
typedef struct Game {
    int diff;
    int size;
    int gm;
    State s;
}Game;
Maptile** mallocMap(int size);
void freeMap(Maptile** mappp, int size);
void save_game(Game* g, char* name);
void set_color(int mod, int* palette);
int get_ship_by_coords(Maptile** map, Hajo* h, int size, int x, int y);
int set_navy_size(int map_size);

#endif //STROKTOK_H_INCLUDED