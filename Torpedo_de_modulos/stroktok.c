#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include "debugmalloc.h"
#include "stroktok.h"

Maptile** mallocMap(int size) {
    Maptile** mappp;
    mappp = (Maptile**)calloc(size, sizeof(Maptile*));
    for (int i = 0; i < size; i++)
    {
        mappp[i] = (Maptile*)calloc(size, sizeof(Maptile));
    }
    return mappp;
}
void freeMap(Maptile** mappp, int size) {
    for (int i = 0; i < size; i++)
    {
        free(mappp[i]);
    }
    free(mappp);
}
void set_color(int mod, int* palette) {        //mód az hogy víz/meglõtt víz/hajó/sérült hajó/elsüllyesztett hajó
    HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);
    //printf("clr: m:%d p:%d\n", mod, *palette);
    //printf(".%d", mod);
    switch (mod)
    {
    case -1:
        SetConsoleTextAttribute(hc, 0x07);
        break;
    case 0:         //víz
        switch (*palette)
        {
        case 0:     //kék víz
            SetConsoleTextAttribute(hc, 0x19);
            break;
        case 1:     //zöld víz
            SetConsoleTextAttribute(hc, 0x2a);
            break;
        case 2:     //cyan víz
            SetConsoleTextAttribute(hc, 0x3b);
            break;
        case 3:     //vörös víz
            SetConsoleTextAttribute(hc, 0x4c);
            break;
        case 4:     //lila víz
            SetConsoleTextAttribute(hc, 0x5d);
            break;
        case 5:     //sárga víz
            SetConsoleTextAttribute(hc, 0x6e);
            break;
        default:
            break;
        }
        break;
    case 1:         //meglõtt víz
        switch (*palette)
        {
        case 0:     //kék víz
            SetConsoleTextAttribute(hc, 0x18);
            break;
        case 1:     //zöld víz
            SetConsoleTextAttribute(hc, 0x28);
            break;
        case 2:     //cyan víz
            SetConsoleTextAttribute(hc, 0x38);
            break;
        case 3:     //vörös víz
            SetConsoleTextAttribute(hc, 0x48);
            break;
        case 4:     //lila víz
            SetConsoleTextAttribute(hc, 0x58);
            break;
        case 5:     //sárga víz
            SetConsoleTextAttribute(hc, 0x68);
            break;
        default:
            break;
        }
        break;
    case 2:         //hajó
        switch (*palette)
        {
        case 0:     //kék víz
            SetConsoleTextAttribute(hc, 0x1f);
            break;
        case 1:     //zöld víz
            SetConsoleTextAttribute(hc, 0x2f);
            break;
        case 2:     //cyan víz
            SetConsoleTextAttribute(hc, 0x3f);
            break;
        case 3:     //vörös víz
            SetConsoleTextAttribute(hc, 0x4f);
            break;
        case 4:     //lila víz
            SetConsoleTextAttribute(hc, 0x5f);
            break;
        case 5:     //sárga víz
            SetConsoleTextAttribute(hc, 0x6f);
            break;
        default:
            break;
        }
        break;
    case 3:         //meglõtt hajó
        switch (*palette)
        {
        case 0:     //kék víz
            SetConsoleTextAttribute(hc, 0x18);
            break;
        case 1:     //zöld víz
            SetConsoleTextAttribute(hc, 0x28);
            break;
        case 2:     //cyan víz
            SetConsoleTextAttribute(hc, 0x38);
            break;
        case 3:     //vörös víz
            SetConsoleTextAttribute(hc, 0x48);
            break;
        case 4:     //lila víz
            SetConsoleTextAttribute(hc, 0x58);
            break;
        case 5:     //sárga víz
            SetConsoleTextAttribute(hc, 0x68);
            break;
        default:
            break;
        }
        break;
    case 4:         //elsüllyedt hajó
        switch (*palette)
        {
        case 0:     //kék víz
            SetConsoleTextAttribute(hc, 0x10);
            break;
        case 1:     //zöld víz
            SetConsoleTextAttribute(hc, 0x20);
            break;
        case 2:     //cyan víz
            SetConsoleTextAttribute(hc, 0x30);
            break;
        case 3:     //vörös víz
            SetConsoleTextAttribute(hc, 0x40);
            break;
        case 4:     //lila víz
            SetConsoleTextAttribute(hc, 0x50);
            break;
        case 5:     //sárga víz
            SetConsoleTextAttribute(hc, 0x60);
            break;
        default:
            break;
        }
        break;/**/
    default:
        break;
    }
}
void save_game(Game* g, char* name) {
    FILE* fp; /* fájl mutató (file pointer/handle) */
    int len_of_navy = set_navy_size(g->size);
    //char fajlnev[] = "last_save.txt";
    fp = fopen(name, "w"); /* megnyitás */
    if (fp == NULL) {
        perror("Fájl megnyitása sikertelen");
        return;    /* nem folytathatjuk! */
    }
    fprintf(fp, "%d %d %d\n", g->gm, g->diff, g->size);
    for (int i = 0; i < g->size; i++)
    {
        for (int j = 0; j < g->size; j++)
        {
            fprintf(fp, "%c %d", g->s.p1[j][i].value, g->s.p1[j][i].hit);
        }
        fprintf(fp, "\n");
    }
    for (int i = 0; i < len_of_navy; i++)
    {
        fprintf(fp, "%d %d %d %d %d\n", g->s.p1navy[i].b.x, g->s.p1navy[i].b.y, g->s.p1navy[i].e.x, g->s.p1navy[i].e.y, g->s.p1navy[i].intact_tiles);
    }
    for (int i = 0; i < g->size; i++)
    {
        for (int j = 0; j < g->size; j++)
        {
            fprintf(fp, "%c %d", g->s.cp2[j][i].value, g->s.cp2[j][i].hit);
        }
        fprintf(fp, "\n");
    }
    for (int i = 0; i < len_of_navy; i++)
    {
        fprintf(fp, "%d %d %d %d %d\n", g->s.cp2navy[i].b.x, g->s.cp2navy[i].b.y, g->s.cp2navy[i].e.x, g->s.cp2navy[i].e.y, g->s.cp2navy[i].intact_tiles);
    }

    fclose(fp);
}
int get_ship_by_coords(Maptile** map, Hajo* h, int size, int x, int y) {       //visszaadja a sorszámát annak a hajónak a listából amelyiknek az egyik mezõje a megadott (x,y)
    for (int i = 0; i < set_navy_size(size); i++)     //végig megyünk az összes mezõjén a nézendõ hajónak    //a placablebõl módosítva
    {
        int this_ship_len = (h[i].e.x - h[i].b.x) + (h[i].e.y - h[i].b.y);
        for (int j = 0; j < this_ship_len + 1; j++)     //végignézzük az összes elemét a hajónak
        {
            if (h[i].b.x == h[i].e.x) //ha függõleges
            {
                if (h[i].b.x == x && h[i].b.y + j == y) return i;
            }
            else
            {
                if (h[i].b.x + j == x && h[i].b.y == y) return i;
            }
        }
    }
}
int set_navy_size(int map_size) {
    switch (map_size)
    {
    case 10:
        return 7;
    case 15:
        return 11;
    case 20:
        return 23;
    case 26:
        return 41;
    default:
        return 0;
    }
}
