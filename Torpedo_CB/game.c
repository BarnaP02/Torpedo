#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include "debugmalloc.h"
#include "stroktok.h"
#include "loves.h"
#include "game.h"

void put_navy_on_map(Maptile** mappp, int map_size, Hajo* navy, int navy_size) {
    //printf("cso\n%d\n",navy_size);
    char c = 219;
    for (int i = 0; i < navy_size; i++)
    {
        int len = ((navy[i].e.x - navy[i].b.x) + (navy[i].e.y - navy[i].b.y)) % 256;
        for (int j = 0; j < len; j++)
        {
            //printf("cso1\n");
            if (navy[i].b.x == navy[i].e.x) //ha függõleges
            {
                mappp[navy[i].b.x][navy[i].b.y + j].value = '1';
            }
            else
            {
                mappp[navy[i].b.x + j][navy[i].b.y].value = '1';
            }
            mappp[navy[i].e.x][navy[i].e.y].value = '1';
            //printf("%c", mappp[navy[i].b.x][navy[i].b.y].value);
        }
        if (navy[i].intact_tiles == 1) {
            //printf(".(%d:%d)\n", navy[i].b.x, navy[i].b.y);
            mappp[navy[i].b.x][navy[i].b.y].value = '1';
        }
        //mappp[navy[len].e.x][navy[len].e.y].value = c;        
    }
    /**/
    for (int i = 0; i < map_size; i++)
    {
        for (int j = 0; j < map_size; j++)
        {
            if (mappp[j][i].value != '1')
            {
                mappp[j][i].value = '2';
            }
            printf(".%c", mappp[j][i].value);
            mappp[i][j].hit = false;
        }
        printf("\n");
    }
    printf("\n");
}
void place_all_ships(Game* g, Maptile** mappp, int map_size, Hajo* new_navy, bool is_cp_game, int* palette) {
    int navy[5] = { 0 };    //hogy tudjuk hogy hány milyen hajó van
    switch (map_size)
    {
    case 10:
        navy[0] = 2;    //sub        (1x1)
        navy[1] = 2;    //destroyer  (2x1)
        navy[2] = 1;    //cruiser    (3x1)
        navy[3] = 1;    //battleship (4x1)
        navy[4] = 1;    //carrier    (5x1)
        break;
    case 15:
        navy[0] = 3;    //sub        (1x1)
        navy[1] = 3;    //destroyer  (2x1)
        navy[2] = 2;    //cruiser    (3x1)
        navy[3] = 2;    //battleship (4x1)
        navy[4] = 1;    //carrier    (5x1)
        break;
    case 20:
        navy[0] = 7;    //sub        (1x1)
        navy[1] = 5;    //destroyer  (2x1)
        navy[2] = 5;    //cruiser    (3x1)
        navy[3] = 4;    //battleship (4x1)
        navy[4] = 2;    //carrier    (5x1)
        break;
    case 26:
        navy[0] = 10;   //sub        (1x1)(azért csak tíz, mert már így is lehet egy teljes órát játszani, mivel hogy 676 mezõbõl áll egy oldal)
        navy[1] = 11;   //destroyer  (2x1)
        navy[2] = 9;    //cruiser    (3x1)
        navy[3] = 7;    //battleship (4x1)
        navy[4] = 4;    //carrier    (5x1)
        break;
    default:
        break;
    }
    int index = 4;
    int navy_size = 0;
    for (int i = 4; i >= 0; i--)
    {
        while (navy[i] != 0) {
            navy_size++;
            if (is_cp_game) cp_place_ship(navy, i, new_navy, map_size, navy_size);
            else place_ship(navy, i, new_navy, map_size, navy_size);
            put_navy_on_map(mappp, map_size, new_navy, set_navy_size(map_size));
            navy[i]--;
            if (is_cp_game) show_pov(g, 2, palette);
            else show_pov(g, 1, palette);         //1:játékos oldala            
        }
    }
}

void clear_navy(Hajo* navy, int len) {
    for (int i = 0; i < len; i++)
    {
        navy[i].b.x = -1;
        navy[i].b.y = -1;
        navy[i].e.x = -1;
        navy[i].e.y = -1;
        navy[i].intact_tiles = -1;
    }
}

void create_state(State* s, Maptile** pmap1, Maptile** pmap2, Hajo* navy1, Hajo* navy2) {
    s->p1 = pmap1;
    s->cp2 = pmap2;
    s->p1navy = navy1;
    s->cp2navy = navy2;
}
void create_game(Game* g, State s, char diff, int size, int gm) {
    g->gm = gm;
    switch (diff)
    {
    case 'e':
        g->diff = 0;
    case 'm':
        g->diff = 1;
    case 'h':
        g->diff = 2;
    case 'x':
        g->diff = 3;
    default:
        break;
    }
    g->size = size;
    g->s = s;
}
void salvo(Game* g, int* p1intact, int* cp2intact, int* p1miss, int* cp2miss, int pov, bool* if_hit, int* damaged_ship, int* last_col, int* last_row, int* first_hit_col, int* first_hit_row, int* align, int* palette) {
    printf("pov%d\n", pov);
    if (pov == 1)
    {
        system("cls");
        show_pov(g, pov, palette);         //1:játékos oldala
        fire(g, if_hit, pov, palette);
        Sleep(2);
        while (*if_hit)
        {
            system("cls");
            --* cp2intact;
            show_pov(g, pov, palette);         //1:játékos oldala
            if (*cp2intact > 0) fire(g, if_hit, pov, palette);
            else *if_hit = false;
            Sleep(2);
        }
        printf("fire--\n");
        system("cls");
        show_pov(g, pov, palette);         //1:játékos oldala
        Sleep(2);
        //getch();
    }
    else
    {
        printf("gm:%d\n", g->gm);
        if (g->gm == 1)
        {
            cpfire(g, p1intact, cp2intact, p1miss, cp2miss, if_hit, damaged_ship, last_col, last_row, first_hit_col, first_hit_row, align);  //a gép mindig ugyanazon az oldalon van
            while (*if_hit)
            {
                system("cls");
                --* p1intact;
                printf("loves utan:\n");
                show_pov(g, 1, palette);         //1:játékos oldala
                Sleep(1200);
                if (*p1intact >= 0) cpfire(g, p1intact, cp2intact, p1miss, cp2miss, if_hit, damaged_ship, last_col, last_row, first_hit_col, first_hit_row, align);
                else *if_hit = false;

            }
            printf("[%d,%d]\n", *last_row, *last_col);
        }
        else
        {
            system("cls");
            show_pov(g, pov, palette);
            fire(g, if_hit, pov, palette);      //itt a pov 2
            Sleep(2);
            while (*if_hit)
            {
                system("cls");
                --* p1intact;
                show_pov(g, pov, palette);
                if (*p1intact >= 0) fire(g, if_hit, pov, palette);
                else *if_hit = false;
            }
            system("cls");
            show_pov(g, pov, palette);
        }
    }
}


void init_game(Game* g, int diff, int size, int gm) {
    Maptile** p1map = mallocMap(g->size), ** cpmap = mallocMap(g->size);
    int len_of_navy = set_navy_size(g->size);
    Hajo* p1navy = (Hajo*)malloc(len_of_navy * sizeof(Hajo));
    if (p1navy == NULL) printf("itt a hiba\n");
    Hajo* cpnavy = (Hajo*)malloc(len_of_navy * sizeof(Hajo));
    if (cpnavy == NULL) printf("itt a hiba\n");
    State s;
    create_state(&s, p1map, cpmap, p1navy, cpnavy);
    create_game(g, s, g->diff, g->size, g->gm);       //1: CPvP
}
void load_game(char* name, Game* g) {
    printf("%s\n", name);
    FILE* fp = fopen(name, "r"); /* megnyitás */
    if (fp == NULL) {
        perror("Fajl megnyitasa sikertelen");
        get_new_file(g);
        return;    /* nem folytathatjuk! */
    }
    g->gm = get_next_int(fp);
    g->diff = get_next_int(fp);
    g->size = get_next_int(fp);
    char shift;
    int index;
    bool was_space = false;

    init_game(g, g->diff, g->size, g->gm);

    for (int i = 0; i < g->size; i++)
    {
        shift = fgetc(fp);
        index = 0;
        while (shift != '\n')
        {
            if (was_space) {
                g->s.p1[index][i].hit = shift - '0';
                was_space = false;
                index++;                        //azért van itt mert a következõ karakter már másik mezõhöz tartozik
            }
            else {
                g->s.p1[index][i].value = shift;
                printf("%c ", g->s.p1[index][i].value);
            }
            shift = fgetc(fp);
            if (shift == ' ') {
                was_space = true;
                shift = fgetc(fp);
            }
        }
        printf("\n");
    }
    for (int i = 0; i < set_navy_size(g->size); i++)
    {
        g->s.p1navy[i].b.x = get_next_int(fp);
        g->s.p1navy[i].b.y = get_next_int(fp);
        g->s.p1navy[i].e.x = get_next_int(fp);
        g->s.p1navy[i].e.y = get_next_int(fp);
        g->s.p1navy[i].intact_tiles = get_next_int(fp);
    }
    for (int i = 0; i < g->size; i++)
    {
        shift = fgetc(fp);
        index = 0;
        while (shift != '\n')
        {
            if (was_space) {
                g->s.cp2[index][i].hit = shift - '0';
                was_space = false;
                index++;                        //azért van itt mert a következõ karakter már másik mezõhöz tartozik
            }
            else {
                g->s.cp2[index][i].value = shift;
                printf("%c ", g->s.cp2[index][i].value);
            }
            shift = fgetc(fp);
            if (shift == ' ') {
                was_space = true;
                shift = fgetc(fp);
            }
        }
        printf("\n");
    }
    for (int i = 0; i < set_navy_size(g->size); i++)
    {
        g->s.cp2navy[i].b.x = get_next_int(fp);
        g->s.cp2navy[i].b.y = get_next_int(fp);
        g->s.cp2navy[i].e.x = get_next_int(fp);
        g->s.cp2navy[i].e.y = get_next_int(fp);
        g->s.cp2navy[i].intact_tiles = get_next_int(fp);
    }
    printf("%d\n", g->s.cp2navy[0].intact_tiles);

    fclose(fp);
}
int get_next_int(FILE* fp) {
    char nextc;
    int nexti = 0;
    nextc = fgetc(fp);
    while (nextc != ' ' && nextc != '\n')
    {
        nexti = nexti * 10 + (nextc - '0');
        nextc = fgetc(fp);
    }
    return nexti;
}
void get_new_file(Game* g) {
    char input_name[251];
    char name[255];
    for (int i = 0; i < 21; i++)
    {
        name[i] = '\0';
    }
    gets(input_name);
    strcat(name, input_name);
    strcat(name, ".txt");
    load_game(name, g);
}
