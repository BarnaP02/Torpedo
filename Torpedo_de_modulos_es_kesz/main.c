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

void pull_out_the_cheats() {
    for (int i = 0; i < 260; i++)
    {
        printf("%3.d:%c\t", i, i);
    }
    printf("\n");
}
void set_type(char* ptype) {
    printf("LOAD/NEW (L/N)\n");
    char input;
    input = tolower(_getch());
    while (input != 'l' && input != 'n') {
        input = tolower(_getch());
    }
    *ptype = tolower(input);
}
void set_gm(char* pgm) {
    printf("Player vs Computer/Player vs Player (C/P)\n");
    char input;
    input = tolower(_getch());
    while (input != 'c' && input != 'p') {
        input = tolower(_getch());
    }
    *pgm = tolower(input);
}
void set_diff(char* pdiff) {
    printf("EASY/MEDIUM/HARD/EXTREME (E/M/H/X)\n");
    char input;
    input = tolower(_getch());
    while (input != 'e' && input != 'm' && input != 'h' && input != 'x') {
        input = tolower(_getch());
    }
    *pdiff = tolower(input);
}
void set_size(int* psize) {
    printf("SMALL/MEDIUM/BIG/HUGE (S/M/B/H)\n");
    char input;
    input = tolower(_getch());
    while (input != 's' && input != 'm' && input != 'b' && input != 'h') {
        input = tolower(_getch());
    }
    switch (input) {
    case 's':
        *psize = 10;
        break;
    case 'm':
        *psize = 15;
        break;
    case 'b':
        *psize = 20;
        break;
    case 'h':
        *psize = 26;
        break;
    default:
        break;
    }
    //*psize = tolower(input);
}
void CPGame(char diff, int map_size, Game* g) {
    int palette = 0;                 //ez egy easter egg ami szinez
    Maptile** p1map = mallocMap(map_size), ** cpmap = mallocMap(map_size);
    int len_of_navy = set_navy_size(map_size);
    Hajo* p1navy = (Hajo*)malloc(len_of_navy * sizeof(Hajo));
    if (p1navy == NULL) printf("itt a hiba\n");
    Hajo* cpnavy = (Hajo*)malloc(len_of_navy * sizeof(Hajo));
    if (cpnavy == NULL) printf("itt a hiba\n");
    if (diff != 'a')      //nincs load
    {
        clear_navy(p1navy, len_of_navy);
        State s;
        create_state(&s, p1map, cpmap, p1navy, cpnavy);
        create_game(g, s, diff, map_size, 1);       //1: CPvP
        place_all_ships(g, p1map, map_size, p1navy, false, &palette);
        clear_navy(cpnavy, len_of_navy);
        place_all_ships(g, cpmap, map_size, cpnavy, true, &palette);
        printf("Eternal Happiness\n");
        put_navy_on_map(p1map, map_size, p1navy, len_of_navy);
        put_navy_on_map(cpmap, map_size, cpnavy, len_of_navy);
    }


    //printf("%c\t%d\n", g->diff,g->s.p1navy[0].intact_tiles);
    save_game(g, "first_save.txt");
    show_pov(g, 1, &palette);         //1:játékos oldala
    show_pov(g, 2, &palette);
    system("cls");
    int p1intact = 0, cp2intact = 0;//sértetlen hajómezõk száma
    int p1miss = 0, cp2miss = 0;    //vízmezõk száma
    for (int i = 0; i < g->size; i++)
        for (int j = 0; j < g->size; j++)
            if (g->s.p1[i][j].hit == false)
                if (g->s.p1[i][j].value == '1') p1intact++;
                else p1miss++;
    for (int i = 0; i < g->size; i++)
        for (int j = 0; j < g->size; j++)
            if (g->s.cp2[i][j].hit == false)
                if (g->s.cp2[i][j].value == '1') cp2intact++;
                else cp2miss++;
    //cp2intact = p1intact;
    //cp2miss = p1miss;
    //ezek csak a cpfire-nek kellenek      ||
    bool if_hit = false;             //    \/
    int damaged_ship = -1;           //-1 ha nincs tagged_ship, egyébként meg a tagged_ship sorszáma a hajólistából
    int last_col;                    //elzõ találat koordinátái
    int last_row;
    int tagged_col = -1;             //elõzõ elõtti találat koordinátái
    int tagged_row = -1;
    int target_ship_align = 0;       //0:ismeretlen 1:függõleges 2:vízszintes
    reset_cpfire(&tagged_col, &tagged_row, &damaged_ship, &target_ship_align);
    while (p1intact > 0 && cp2intact > 0)
    {
        if (p1intact != 0)
        {
            printf("salvo: %d:%d\n", p1intact, cp2intact);
            salvo(g, &p1intact, &cp2intact, &p1miss, &cp2miss, 1, &if_hit, &damaged_ship, &last_col, &last_row, &tagged_col, &tagged_row, &target_ship_align, &palette);
        }
        if (cp2intact != 0)
        {
            printf("salvo: %d:%d\n", p1intact, cp2intact);
            salvo(g, &p1intact, &cp2intact, &p1miss, &cp2miss, 2, &if_hit, &damaged_ship, &last_col, &last_row, &tagged_col, &tagged_row, &target_ship_align, &palette);
        }
    }
    show_pov(g, 1, &palette);         //1:játékos oldala
    show_pov(g, 2, &palette);
    if (p1intact < cp2intact) printf("Androids RISE!! (you lost)\n");
    else printf("Not Today! (you won)\n");

    /*if (diff == 'a')
    {
        free(p1navy);
        free(cpnavy);
        freeMap(p1map, map_size);
        freeMap(cpmap, map_size);
    }*/

    free(g->s.p1navy);
    free(g->s.cp2navy);
    freeMap(g->s.p1, map_size);
    freeMap(g->s.cp2, map_size);
    exit(0);
    //free(ge.s.p1navy);
    //free(ge.s.cp2navy);
    //freeMap(ge.s.p1, ge.size);
    //freeMap(ge.s.cp2, ge.size);
}
void PvPGame(int map_size, Game* g, bool if_load) {
    int palette = 0;                 //ez egy easter egg ami szinez
    Maptile** p1map = mallocMap(map_size), ** cp2map = mallocMap(map_size);
    int len_of_navy = set_navy_size(map_size);
    Hajo* p1navy = (Hajo*)malloc(len_of_navy * sizeof(Hajo));
    if (p1navy == NULL) printf("itt a hiba\n");
    Hajo* cp2navy = (Hajo*)malloc(len_of_navy * sizeof(Hajo));
    if (cp2navy == NULL) printf("itt a hiba\n");

    if (!if_load)
    {
        clear_navy(p1navy, len_of_navy);
        State s;
        create_state(&s, p1map, cp2map, p1navy, cp2navy);
        create_game(g, s, 1, map_size, 2);       //2: PvP
        place_all_ships(g, p1map, map_size, p1navy, false, &palette);
        clear_navy(cp2navy, len_of_navy);
        place_all_ships(g, cp2map, map_size, cp2navy, false, &palette);
        printf("Eternal Bliss\n");

        put_navy_on_map(p1map, map_size, p1navy, len_of_navy);
        put_navy_on_map(cp2map, map_size, cp2navy, len_of_navy);
    }

    save_game(g, "fisrt_save.txt");
    //Game ge;
    //printf("ge.gm:%d, diff:%d\n", ge.gm, ge.diff);
    show_pov(g, 1, &palette);         //1:játékos oldala
    show_pov(g, 2, &palette);

    //show_pov(&ge, 1);
    //show_pov(&ge, 2);
    int p1intact = 0, cp2intact = 0;//sértetlen hajómezõk száma
    int p1miss = 0, cp2miss = 0;    //vízmezõk száma
    for (int i = 0; i < g->size; i++)
        for (int j = 0; j < g->size; j++)
            if (g->s.p1[i][j].hit == false)
                if (g->s.p1[i][j].value == '1') p1intact++;
                else p1miss++;
    for (int i = 0; i < g->size; i++)
        for (int j = 0; j < g->size; j++)
            if (g->s.cp2[i][j].hit == false)
                if (g->s.cp2[i][j].value == '1') cp2intact++;
                else cp2miss++;
    //cp2intact = p1intact;
    //cp2miss = p1miss;
    //ezek csak a cpfire-nek kellenek      ||
    bool if_hit = false;             //    \/
    int damaged_ship = -1;           //-1 ha nincs tagged_ship, egyébként meg a tagged_ship sorszáma a hajólistából
    int last_col;                    //elzõ találat koordinátái
    int last_row;
    int tagged_col = -1;             //elõzõ elõtti találat koordinátái
    int tagged_row = -1;
    int target_ship_align = 0;       //0:ismeretlen 1:függõleges 2:vízszintes
    reset_cpfire(&tagged_col, &tagged_row, &damaged_ship, &target_ship_align);
    while (p1intact > 0 && cp2intact > 0)
    {
        if (p1intact != 0)
        {
            printf("salvo: %d:%d\n", p1intact, cp2intact);
            salvo(g, &p1intact, &cp2intact, &p1miss, &cp2miss, 1, &if_hit, &damaged_ship, &last_col, &last_row, &tagged_col, &tagged_row, &target_ship_align, &palette);
        }
        if (cp2intact != 0)
        {
            printf("salvo: %d:%d\n", p1intact, cp2intact);
            salvo(g, &p1intact, &cp2intact, &p1miss, &cp2miss, 2, &if_hit, &damaged_ship, &last_col, &last_row, &tagged_col, &tagged_row, &target_ship_align, &palette);
        }
    }
    show_pov(g, 1, &palette);         //1:játékos oldala
    show_pov(g, 2, &palette);
    if (p1intact < cp2intact) printf("Player 1 won\n");
    else printf("Player 2 won\n");

    free(g->s.p1navy);
    free(g->s.cp2navy);
    freeMap(g->s.p1, map_size);
    freeMap(g->s.cp2, map_size);
    exit(0);
    /*
    if (if_load)
    {
        free(p1navy);
        free(cp2navy);
        freeMap(p1map, map_size);
        freeMap(cp2map, map_size);
    }*/
}
void NewGame(Game* g, char gamemode, char diff, int map_size) {
    //Maptile** Map = mallocMap(map_size);
    srand(time(NULL));
    if (gamemode == 'a')
    {
        if (g->gm == 1)
        {
            CPGame(diff, map_size, g);
        }
        else if (g->gm == 2)
        {
            PvPGame(map_size, g, true);
        }
    }
    else if (gamemode == 'c')
    {
        CPGame(diff, map_size, g);
    }
    else
    {
        PvPGame(map_size, g, false);
    }
}
int main()
{
    pull_out_the_cheats();
    printf("Hello world!\n");
    char type, gm, diff;
    int size;
    Game g;

    set_type(&type);
    if (type == 'n') {
        set_gm(&gm);
        set_size(&size);
        if (gm == 'c') {
            set_diff(&diff);
        }
        else {      //nincs értelme
            diff = 'b';
        }
    }
    else {          //beolvasás lesz
        gm = 'a';
        diff = 'a';
        size = 0;
        get_new_file(&g);      //eleinte csak ahhoz kellett ha nincs olyan file amit be akarnak tölteni de el tudja indítani a beolvasást
    }
    NewGame(&g, gm, diff, size);
    printf("type:%c\tgm:%c\tdiff:%c\n", type, gm, diff);
    return 0;
}
