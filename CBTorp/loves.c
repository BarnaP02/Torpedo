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
#include "lerakas.h"

void menu(Game* g, int* palette, int* global) {
    printf("Menu:\tpress:\n(s)\tto save (and exit)\n(e)\tto exit\n(c)\tto save and continue\n(b)\tto just go back\n");
    char input;
    char input_name[21];
    char name[25];// = (char*)malloc(25 * sizeof(char));
    for (int i = 0; i < 21; i++)
    {
        name[i] = '\0';
    }
    input = tolower(_getch());
    switch (input)
    {
    case 's':
        printf("How do you want to name the save file? (max length: 20)\n");
        gets(input_name);
        strcat(name, input_name);
        strcat(name, ".txt");
        save_game(g, name);
        *global = 7;
        break;
    case 'e':
        *global = 7;
        break;
    case 'c':
        printf("How do you want to name the save file? (max length: 20)\n");
        gets(input_name);
        strcat(name, input_name);
        strcat(name, ".txt");
        save_game(g, name);
        break;
    case 'b':
        break;
    case '+':
        *palette = 0;
        break;
    case '!':
        *palette = 1;
        break;
    case '%':
        *palette = 2;
        break;
    case '/':
        *palette = 3;
        break;
    case '=':
        *palette = 4;
        break;
    case '(':
        *palette = 5;
        break;
    default:
        break;
    }
}
void get_target(Game* g, int* coln, int* row, int* palette, int* global) {
    if (*global == 7) return;
    printf("Enter firing coordinates! (ect.: 'A8') (menu: '.')\n");
    char col = _getch();
    //int row = 0;
    if (col == '.')
    {
        menu(g, palette, global);
        //col = _getch();
    }
    if (*global == 7) return;
    col = tolower(col);
    *coln = col - 'a';
    printf("%c", col + ('A' - 'a'));
    char tempcol[3];
    gets(tempcol);
    tempcol[2] = '\0';
    if (strlen(tempcol) < 2)
    {
        *row = tempcol[0] - '0' - 1;
    }
    else
    {
        *row = (tempcol[0] - '0') * 10 + (tempcol[1] - '0') - 1;
    }
    while (*coln < 0 || *coln > g->size - 1 || *row < 0 || *row > g->size - 1)
    {
        printf("invalid input\nEnter firing coordinates! (ect.: 'A8') (menu: '.')\n");
        //printf("rossz input\nrown:%d\ntempc:%s   coln:%d\nstrlen:%d\n", *coln, tempcol, *row, strlen(tempcol));
        col = _getch();
        if (col == '.')
        {
            menu(g, palette, global);
        }
        if (*global == 7) return;
        col = tolower(col);
        *coln = col - 'a';
        printf("%c", col + ('A' - 'a'));
        gets(tempcol);
        if (strlen(tempcol) < 2)
        {
            *row = tempcol[0] - '0' - 1;
        }
        else
        {
            *row = (tempcol[0] - '0') * 10 + (tempcol[1] - '0') - 1;
        }
    }
}
void ship_sunk(Maptile** map, Hajo* h, int size) {
    for (int i = 0; i < set_navy_size(size); i++)     //végig megyünk az összes mezőjén a nézendő hajónak    //a placableből módosítva
    {
        int this_ship_len = (h->e.x - h->b.x) + (h->e.y - h->b.y);
        for (int j = 0; j < this_ship_len + 1; j++)     //végignézzük az összes elemét a hajónak
        {
            if (h->b.x == h->e.x) //ha függőleges
            {
                if (h->b.x == 0 && h->b.y + j == 0)            //bal felső sarok
                {
                    map[h->b.x][h->b.y + j + 1].hit = true;        //le
                    map[h->b.x + 1][h->b.y + j + 1].hit = true;    //átlósan jobbra le
                    map[h->b.x + 1][h->b.y + j].hit = true;        //jobbra
                }
                else if (h->b.x == 0 && h->b.y + j == size - 1)//bal alsó sarok
                {
                    map[h->b.x + 1][h->b.y + j].hit = true;        //jobbra
                    map[h->b.x + 1][h->b.y + j - 1].hit = true;    //átlósan jobbra fel
                    map[h->b.x][h->b.y + j - 1].hit = true;        //fel
                }
                else if (h->b.x == size - 1 && h->b.y + j == 0)//jobb felső sarok
                {
                    map[h->b.x - 1][h->b.y + j].hit = true;        //balra
                    map[h->b.x - 1][h->b.y + j + 1].hit = true;    //átlósan balra le
                    map[h->b.x][h->b.y + j + 1].hit = true;        //le
                }
                else if (h->b.x == size - 1 && h->b.y + j == size - 1)//jobb alsó sarok
                {
                    map[h->b.x][h->b.y + j - 1].hit = true;        //fel
                    map[h->b.x - 1][h->b.y + j - 1].hit = true;    //átlósan balra fel
                    map[h->b.x - 1][h->b.y + j].hit = true;        //balra
                }
                else if (h->b.x == 0)      //bal oldal
                {
                    map[h->b.x][h->b.y + j + 1].hit = true;        //le
                    map[h->b.x + 1][h->b.y + j + 1].hit = true;    //átlósan jobbra le
                    map[h->b.x + 1][h->b.y + j].hit = true;        //jobbra
                    map[h->b.x + 1][h->b.y + j - 1].hit = true;    //átlósan jobbra fel
                    map[h->b.x][h->b.y + j - 1].hit = true;        //fel
                }
                else if (h->b.y + j == 0)      //felső oldal
                {
                    map[h->b.x - 1][h->b.y + j].hit = true;        //balra
                    map[h->b.x - 1][h->b.y + j + 1].hit = true;    //átlósan balra le
                    map[h->b.x][h->b.y + j + 1].hit = true;        //le
                    map[h->b.x + 1][h->b.y + j + 1].hit = true;    //átlósan jobbra le
                    map[h->b.x + 1][h->b.y + j].hit = true;        //jobbra
                }
                else if (h->b.x == size - 1)      //jobb oldal
                {
                    map[h->b.x][h->b.y + j - 1].hit = true;        //fel
                    map[h->b.x - 1][h->b.y + j - 1].hit = true;    //átlósan balra fel
                    map[h->b.x - 1][h->b.y + j].hit = true;        //balra
                    map[h->b.x - 1][h->b.y + j + 1].hit = true;    //átlósan balra le
                    map[h->b.x][h->b.y + j + 1].hit = true;        //le
                }
                else if (h->b.y + j == size - 1)      //alsó oldal
                {
                    map[h->b.x + 1][h->b.y + j].hit = true;        //jobbra
                    map[h->b.x + 1][h->b.y + j - 1].hit = true;    //átlósan jobbra fel
                    map[h->b.x][h->b.y + j - 1].hit = true;        //fel
                    map[h->b.x - 1][h->b.y + j - 1].hit = true;    //átlósan balra fel
                    map[h->b.x - 1][h->b.y + j].hit = true;        //balra
                }
                else {                            //nincs szélen
                    map[h->b.x - 1][h->b.y + j].hit = true;        //balra
                    map[h->b.x - 1][h->b.y + j + 1].hit = true;    //átlósan balra le
                    map[h->b.x][h->b.y + j + 1].hit = true;        //le
                    map[h->b.x + 1][h->b.y + j + 1].hit = true;    //átlósan jobbra le
                    map[h->b.x + 1][h->b.y + j].hit = true;        //jobbra
                    map[h->b.x + 1][h->b.y + j - 1].hit = true;    //átlósan jobbra fel
                    map[h->b.x][h->b.y + j - 1].hit = true;        //fel
                    map[h->b.x - 1][h->b.y + j - 1].hit = true;    //átlósan balra fel
                }
            }
            else
            {
                if (h->b.x + j == 0 && h->b.y == 0)            //bal felső sarok
                {
                    map[h->b.x + j][h->b.y + 1].hit = true;        //le
                    map[h->b.x + j + 1][h->b.y + 1].hit = true;    //átlósan jobbra le
                    map[h->b.x + j + 1][h->b.y].hit = true;        //jobbra
                }
                else if (h->b.x + j == 0 && h->b.y == size - 1)//bal alsó sarok
                {
                    map[h->b.x + j + 1][h->b.y].hit = true;        //jobbra
                    map[h->b.x + j + 1][h->b.y - 1].hit = true;    //átlósan jobbra fel
                    map[h->b.x + j][h->b.y - 1].hit = true;        //fel
                }
                else if (h->b.x + j == size - 1 && h->b.y == 0)//jobb felső sarok
                {
                    map[h->b.x + j - 1][h->b.y].hit = true;        //balra
                    map[h->b.x + j - 1][h->b.y + 1].hit = true;    //átlósan balra le
                    map[h->b.x + j][h->b.y + 1].hit = true;        //le
                }
                else if (h->b.x + j == size - 1 && h->b.y == size - 1)//jobb alsó sarok
                {
                    map[h->b.x + j][h->b.y - 1].hit = true;        //fel
                    map[h->b.x + j - 1][h->b.y - 1].hit = true;    //átlósan balra fel
                    map[h->b.x + j - 1][h->b.y].hit = true;        //balra
                }
                else if (h->b.x + j == 0)      //bal oldal
                {
                    map[h->b.x + j][h->b.y + 1].hit = true;        //le
                    map[h->b.x + j + 1][h->b.y + 1].hit = true;    //átlósan jobbra le
                    map[h->b.x + j + 1][h->b.y].hit = true;        //jobbra
                    map[h->b.x + j + 1][h->b.y - 1].hit = true;    //átlósan jobbra fel
                    map[h->b.x + j][h->b.y - 1].hit = true;        //fel
                }
                else if (h->b.y == 0)      //felső oldal
                {
                    map[h->b.x + j - 1][h->b.y].hit = true;        //balra
                    map[h->b.x + j - 1][h->b.y + 1].hit = true;    //átlósan balra le
                    map[h->b.x + j][h->b.y + 1].hit = true;        //le
                    map[h->b.x + j + 1][h->b.y + 1].hit = true;    //átlósan jobbra le
                    map[h->b.x + j + 1][h->b.y].hit = true;        //jobbra
                }
                else if (h->b.x + j == size - 1)      //jobb oldal
                {
                    map[h->b.x + j][h->b.y - 1].hit = true;        //fel
                    map[h->b.x + j - 1][h->b.y - 1].hit = true;    //átlósan balra fel
                    map[h->b.x + j - 1][h->b.y].hit = true;        //balra
                    map[h->b.x + j - 1][h->b.y + 1].hit = true;    //átlósan balra le
                    map[h->b.x + j][h->b.y + 1].hit = true;        //le
                }
                else if (h->b.y == size - 1)      //alsó oldal
                {
                    map[h->b.x + j + 1][h->b.y].hit = true;        //jobbra
                    map[h->b.x + j + 1][h->b.y - 1].hit = true;    //átlósan jobbra fel
                    map[h->b.x + j][h->b.y - 1].hit = true;        //fel
                    map[h->b.x + j - 1][h->b.y - 1].hit = true;    //átlósan balra fel
                    map[h->b.x + j - 1][h->b.y].hit = true;        //balra
                }
                else {                            //nincs szélen
                    map[h->b.x + j - 1][h->b.y].hit = true;        //balra
                    map[h->b.x + j - 1][h->b.y + 1].hit = true;    //átlósan balra le
                    map[h->b.x + j][h->b.y + 1].hit = true;        //le
                    map[h->b.x + j + 1][h->b.y + 1].hit = true;    //átlósan jobbra le
                    map[h->b.x + j + 1][h->b.y].hit = true;        //jobbra
                    map[h->b.x + j + 1][h->b.y - 1].hit = true;    //átlósan jobbra fel
                    map[h->b.x + j][h->b.y - 1].hit = true;        //fel
                    map[h->b.x + j - 1][h->b.y - 1].hit = true;    //átlósan balra fel
                }
            }
        }
    }
}
bool can_shoot_left(int this_x, int this_y, Maptile** map) {
    if (this_x != 0)      //nincs a bal oldalon ez a mező
    {
        if (map[this_x - 1][this_y].hit == false) return true;
    }
    return false;
}
bool can_shoot_up(int this_x, int this_y, Maptile** map) {
    if (this_y != 0)      //nincs a felső oldalon ez a mező
    {
        if (map[this_x][this_y - 1].hit == false) return true;
    }
    return false;
}
bool can_shoot_right(int this_x, int this_y, Maptile** map, int size) {     //size = pályaméret
    if (this_x != size - 1)      //nincs a jobb oldalon ez a mező
    {
        if (map[this_x + 1][this_y].hit == false) return true;
    }
    return false;
}
bool can_shoot_down(int this_x, int this_y, Maptile** map, int size) {
    if (this_x != size - 1)      //nincs az alsó oldalon ez a mező
    {
        if (map[this_x][this_y + 1].hit == false) return true;
    }
    return false;
}
void reset_cpfire(int* first_hit_col, int* first_hit_row, int* damaged_ship, int* align) {      //ez csak azért van, mert többször is kell
    *first_hit_col = -1;
    *first_hit_row = -1;
    *damaged_ship = -1;
    *align = 0;
}
void cpfire(Game* g, int* p1intact, int* cp2intact, int* cpmiss, bool* if_hit, bool* ship_ended, int* damaged_ship, int* last_col, int* last_row, int* first_hit_col, int* first_hit_row, int* align) {
    int diffy = 0;
    *ship_ended = false;
    switch (g->diff)
    {
    case 0:
        diffy = 40;
        break;
    case 1:
        diffy = 25;
        break;
    case 2:
        diffy = 10;
        break;
    case 3:
        diffy = 2;
        break;
    default:
        break;
    }
    if (*damaged_ship == -1)
    {
        if (rand() % diffy == 0) *if_hit = true;
        else *if_hit = false;
        if (*if_hit)
        {
            *damaged_ship = rand() % set_navy_size(g->size);
            while (g->s.p1navy[*damaged_ship].intact_tiles == 0)         //ez a hajó már korábban elsüllyedt
            {
                *damaged_ship = rand() % set_navy_size(g->size);
            }
            int damaged_tile = rand() % g->s.p1navy[*damaged_ship].intact_tiles;
            if (g->s.p1navy[*damaged_ship].b.x == g->s.p1navy[*damaged_ship].e.x)    //függőleges
            {
                *last_col = g->s.p1navy[*damaged_ship].b.x;
                *last_row = g->s.p1navy[*damaged_ship].b.y + damaged_tile;
            }
            else
            {
                *last_col = g->s.p1navy[*damaged_ship].b.x + damaged_tile;
                *last_row = g->s.p1navy[*damaged_ship].b.y;
            }
            if (g->s.p1[*last_col][*last_row].hit == false)     //találat
            {
                g->s.p1[*last_col][*last_row].hit = true;
                g->s.p1navy[*damaged_ship].intact_tiles--;
                *first_hit_col = *last_col;
                *first_hit_row = *last_row;
                if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                {
                    ship_sunk(g->s.p1, &g->s.p1navy[*damaged_ship], g->size);
                    reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
                }
            }
            else                                              //már meglőtték a mezőt
            {
                reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
            }
        }
        else if (cpmiss != 0)
        {
            int misscol = rand() % g->size;
            int missrow = rand() % g->size;
            while (g->s.p1[misscol][missrow].hit == true || g->s.p1[misscol][missrow].value != '2')        //addig keres mezőt ameddig nem talál egy olyat amin víz van és még nem lőtték meg
            {
                //printf("val:%c\thit:%d\n", g->s.p1[misscol][missrow].value, g->s.p1[misscol][missrow].hit);
                misscol = rand() % g->size;
                missrow = rand() % g->size;
            }
            g->s.p1[misscol][missrow].hit = true;
            *if_hit = false;
            --* cpmiss;
        }
        else        //nincs több víz mező amit meg lehet lőni
        {
            *if_hit = true;
            *ship_ended = true;
        }
    }
    else
    {                                           /////////ezt meg kéne tervezni papíron
        if (*align == 0)        //még nem tudjuk hogy milyen állású a hajó, ilyenkor biztos hogy a kezdőpontnál vagyunk
        {
            *first_hit_col = *last_col;
            *first_hit_row = *last_row;
            if (can_shoot_left(*last_col, *last_row, g->s.p1))
            {
                g->s.p1[*last_col - 1][*last_row].hit = true;
                if (g->s.p1[*last_col - 1][*last_row].value == '2')
                {
                    *if_hit = false;
                    --* cpmiss;
                }
                else                    //hajó van a mezőn
                {
                    *if_hit = true;
                    --* last_col;        //áthelyezi az utolsó találat koordinátáit
                    g->s.p1navy[*damaged_ship].intact_tiles--;      //mivel, hogy nem lehetnek egymás mellett hajók így tudjuk, hogy ez csak ugyanaz a hajó lehet
                    *align = 2;         //a hajó biztos, hogy vízszintes
                    if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                    {
                        ship_sunk(g->s.p1, &g->s.p1navy[*damaged_ship], g->size);
                        reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
                    }
                }
            }
            else if (can_shoot_up(*last_col, *last_row, g->s.p1))
            {
                g->s.p1[*last_col][*last_row - 1].hit = true;
                if (g->s.p1[*last_col][*last_row - 1].value == '2')
                {
                    *if_hit = false;
                    --* cpmiss;
                }
                else
                {
                    *if_hit = true;
                    --* last_row;        //áthelyezi az utolsó találat koordinátáit
                    g->s.p1navy[*damaged_ship].intact_tiles--;
                    *align = 1;         //a hajó biztos, hogy függőleges
                    if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                    {
                        ship_sunk(g->s.p1, &g->s.p1navy[*damaged_ship], g->size);
                        reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
                    }
                }
            }
            else if (can_shoot_right(*last_col, *last_row, g->s.p1, g->size))
            {
                g->s.p1[*last_col + 1][*last_row].hit = true;
                if (g->s.p1[*last_col + 1][*last_row].value == '2')
                {
                    *if_hit = false;
                    --* cpmiss;
                }
                else
                {
                    *if_hit = true;
                    ++* last_col;        //áthelyezi az utolsó találat koordinátáit
                    g->s.p1navy[*damaged_ship].intact_tiles--;
                    *align = 2;
                    if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                    {
                        ship_sunk(g->s.p1, &g->s.p1navy[*damaged_ship], g->size);
                        reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
                    }
                }
            }
            else if (can_shoot_down(*last_col, *last_row, g->s.p1, g->size))
            {
                g->s.p1[*last_col][*last_row + 1].hit = true;
                if (g->s.p1[*last_col][*last_row + 1].value == '2')
                {
                    *if_hit = false;
                    --* cpmiss;
                }
                else
                {
                    *if_hit = true;
                    ++* last_row;        //áthelyezi az utolsó találat koordinátáit
                    g->s.p1navy[*damaged_ship].intact_tiles--;
                    *align = 1;
                    if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                    {
                        ship_sunk(g->s.p1, &g->s.p1navy[*damaged_ship], g->size);
                        reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
                    }
                }
            }
            //itt már minden eset kezelve kéne hogy legyen
        }
        else if (*align == 1) {     //a hajó függőleges, nem próbál meg balra vagy jobbra lőni
            if (can_shoot_up(*last_col, *last_row, g->s.p1))
            {
                g->s.p1[*last_col][*last_row - 1].hit = true;
                if (g->s.p1[*last_col][*last_row - 1].value == '2')
                {
                    *if_hit = false;
                    --* cpmiss;
                }
                else
                {
                    *if_hit = true;
                    --* last_row;        //áthelyezi az utolsó találat koordinátáit
                    g->s.p1navy[*damaged_ship].intact_tiles--;
                    if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                    {
                        ship_sunk(g->s.p1, &g->s.p1navy[*damaged_ship], g->size);
                        reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
                    }
                }
            }
            else if (can_shoot_down(*last_col, *last_row, g->s.p1, g->size))
            {
                g->s.p1[*last_col][*last_row + 1].hit = true;
                if (g->s.p1[*last_col][*last_row + 1].value == '2')
                {
                    *if_hit = false;
                    --* cpmiss;
                }
                else
                {
                    *if_hit = true;
                    ++* last_row;        //áthelyezi az utolsó találat koordinátáit
                    g->s.p1navy[*damaged_ship].intact_tiles--;
                    if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                    {
                        ship_sunk(g->s.p1, &g->s.p1navy[*damaged_ship], g->size);
                        reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
                    }
                }
            }
            else
            {
                *last_col = *first_hit_col;
                *last_row = *first_hit_row;
                *if_hit = true;
            }
        }
        else if (*align == 2) {     //a hajó vízszintes, nem próbál meg fel vagy le lőni
            if (can_shoot_left(*last_col, *last_row, g->s.p1))
            {
                g->s.p1[*last_col - 1][*last_row].hit = true;
                if (g->s.p1[*last_col - 1][*last_row].value == '2')
                {
                    *if_hit = false;
                    --* cpmiss;

                }
                else                    //hajó van a mezőn
                {
                    *if_hit = true;
                    --* last_col;        //áthelyezi az utolsó találat koordinátáit
                    g->s.p1navy[*damaged_ship].intact_tiles--;      //mivel, hogy nem lehetnek egymás mellett hajók így tudjuk, hogy ez csak ugyanaz a hajó lehet
                    if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                    {
                        ship_sunk(g->s.p1, &g->s.p1navy[*damaged_ship], g->size);
                        reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
                    }
                }
            }
            else if (can_shoot_right(*last_col, *last_row, g->s.p1, g->size))
            {
                g->s.p1[*last_col + 1][*last_row].hit = true;
                if (g->s.p1[*last_col + 1][*last_row].value == '2')
                {
                    *if_hit = false;
                    --* cpmiss;
                }
                else
                {
                    *if_hit = true;
                    ++* last_col;        //áthelyezi az utolsó találat koordinátáit
                    g->s.p1navy[*damaged_ship].intact_tiles--;
                    if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                    {
                        ship_sunk(g->s.p1, &g->s.p1navy[*damaged_ship], g->size);
                        reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
                     //printf("alien\n");
                    }
                }
            }
            else
            {
                *last_col = *first_hit_col;
                *last_row = *first_hit_row;
                *if_hit = true;
                *ship_ended = true;
            }
        }
    }
}
void fire(Game* g, bool* if_hit, int pov, int* palette, int* global) {
    int target_x, target_y;
    get_target(g, &target_x, &target_y, palette, global);
    if (*global == 7) return;
    if (pov == 1)
    {
        while (g->s.cp2[target_x][target_y].hit == true)
        {
            printf("Oda mar nem tudsz loni.\n");
            get_target(g, &target_x, &target_y, palette, global);
            if (*global == 7) return;
        }
        g->s.cp2[target_x][target_y].hit = true;
        if (g->s.cp2[target_x][target_y].value == '1')      //találat
        {
            *if_hit = true;
            g->s.cp2navy[get_ship_by_coords(g->s.cp2, g->s.cp2navy, g->size, target_x, target_y)].intact_tiles--;

            if (g->s.cp2navy[get_ship_by_coords(g->s.cp2, g->s.cp2navy, g->size, target_x, target_y)].intact_tiles == 0)
            {
                ship_sunk(g->s.cp2, &g->s.cp2navy[get_ship_by_coords(g->s.cp2, g->s.cp2navy, g->size, target_x, target_y)], g->size);
            }
        }
        else
        {
            *if_hit = false;
        }
    }
    else
    {
        while (g->s.p1[target_x][target_y].hit == true)
        {
            printf("Oda mar nem tudsz loni.\n");
            get_target(g, &target_x, &target_y, palette, global);
            if (*global == 7) return;
        }
        g->s.p1[target_x][target_y].hit = true;
        if (g->s.p1[target_x][target_y].value == '1')      //találat
        {
            *if_hit = true;
            g->s.p1navy[get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, target_x, target_y)].intact_tiles--;

            if (g->s.p1navy[get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, target_x, target_y)].intact_tiles == 0)
            {
                ship_sunk(g->s.p1, &g->s.p1navy[get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, target_x, target_y)], g->size);
            }
        }
        else
        {
            *if_hit = false;
        }
    }
}