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
#include "lerakas.h"

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
void CPGame(char diff, int map_size, Game* g, int* global) {
    int palette = 0;                 //ez egy easter egg ami szinez
    int len_of_navy = set_navy_size(map_size);

    if (*global == 1)      //nincs load
    {
        init_game(g, diff, map_size, 1);
        clear_navy(g->s.p1navy, len_of_navy);
        clear_navy(g->s.cp2navy, len_of_navy);
        place_all_ships(g, g->s.p1, map_size, g->s.p1navy, false, &palette);
        place_all_ships(g, g->s.cp2, map_size, g->s.cp2navy, true, &palette);
        put_navy_on_map(g->s.p1, map_size, g->s.p1navy, len_of_navy);
        put_navy_on_map(g->s.cp2, map_size, g->s.cp2navy, len_of_navy);
    }
    printf("Eternal Happiness\n");//létfontosságú

    save_game(g, "first_save.txt");
    system("cls");
    int p1intact = 0, cp2intact = 0;//sértetlen hajómezõk száma
    int cpmiss = 0;    //vízmezõk száma
    for (int i = 0; i < g->size; i++)
        for (int j = 0; j < g->size; j++)
            if (g->s.p1[i][j].hit == false)
                if (g->s.p1[i][j].value == '1') p1intact++;
    for (int i = 0; i < g->size; i++)
        for (int j = 0; j < g->size; j++)
            if (g->s.cp2[i][j].hit == false)
                if (g->s.cp2[i][j].value == '1') cp2intact++;
                else cpmiss++;
    //ezek csak a cpfire-nek kellenek      ||
    bool if_hit = false;             //    \/
    int damaged_ship = -1;           //-1 ha nincs tagged_ship, egyébként meg a tagged_ship sorszáma a hajólistából
    int last_col;                    //elzõ találat koordinátái
    int last_row;
    int tagged_col = -1;             //elõzõ elõtti találat koordinátái
    int tagged_row = -1;
    int target_ship_align = 0;       //0:ismeretlen 1:függõleges 2:vízszintes
    reset_cpfire(&tagged_col, &tagged_row, &damaged_ship, &target_ship_align);
    while (p1intact > 0 && cp2intact > 0 && *global < 5)        //rossz kilépési feltétel   true végtelen loopra ér véget (vagyis nem)
    {
        if (*global < 5)
        {
            //printf("salvo: %d:%d\n", p1intact, cp2intact);
            salvo(g, &p1intact, &cp2intact, &cpmiss, 1, &if_hit, &damaged_ship, &last_col, &last_row, &tagged_col, &tagged_row, &target_ship_align, &palette, global);
        }
        if (cp2intact == 0)
            *global = 5;
        if (*global < 5)
        {
            //printf("salvo: %d:%d\n", p1intact, cp2intact);
            salvo(g, &p1intact, &cp2intact, &cpmiss, 2, &if_hit, &damaged_ship, &last_col, &last_row, &tagged_col, &tagged_row, &target_ship_align, &palette, global);
        }
        if (p1intact == 0)
            *global = 6;
    }
    system("cls");
    if (*global != 7)
    {
        show_pov(g, 1, &palette);         //1:játékos oldala
        show_pov(g, 2, &palette);
    }
    if (*global == 5) printf("Not Today! (you won)\n");
    else if (*global == 6) printf("Androids RISE!! (you lost)\n");
    else printf("Successful exit\n");

    free(g->s.p1navy);
    free(g->s.cp2navy);
    freeMap(g->s.p1, g->size);
    freeMap(g->s.cp2, g->size);
}
void PvPGame(int map_size, Game* g, int* global) {
    int palette = 0;                 //ez egy easter egg ami szinez
    int len_of_navy = set_navy_size(map_size);

    if (*global == 2)      //nincs load
    {
        init_game(g, 1, map_size, 2);
        clear_navy(g->s.p1navy, len_of_navy);
        clear_navy(g->s.cp2navy, len_of_navy);
        place_all_ships(g, g->s.p1, map_size, g->s.p1navy, false, &palette);
        place_all_ships(g, g->s.cp2, map_size, g->s.cp2navy, false, &palette);
        put_navy_on_map(g->s.p1, map_size, g->s.p1navy, len_of_navy);
        put_navy_on_map(g->s.cp2, map_size, g->s.cp2navy, len_of_navy);
    }
    printf("Eternal Bliss\n");      //létfontosságú

    save_game(g, "fisrt_save.txt");//direkt van elírva
    system("cls");
    int p1intact = 0, cp2intact = 0;//sértetlen hajómezõk száma
    int cpmiss = 0;                 //vízmezõk száma (azért kell hogy tudjuk hogy a gép mikor nem fog tudni több vízmezõt eltalálni)
    for (int i = 0; i < g->size; i++)
        for (int j = 0; j < g->size; j++)
            if (g->s.p1[i][j].hit == false)
                if (g->s.p1[i][j].value == '1') p1intact++;
    for (int i = 0; i < g->size; i++)
        for (int j = 0; j < g->size; j++)
            if (g->s.cp2[i][j].hit == false)
                if (g->s.cp2[i][j].value == '1') cp2intact++;
                else cpmiss++;
    //ezek csak a cpfire-nek kellenek      ||
    bool if_hit = false;             //    \/
    int damaged_ship = -1;           //-1 ha nincs tagged_ship, egyébként meg a tagged_ship sorszáma a hajólistából
    int last_col;                    //elzõ találat koordinátái
    int last_row;
    int tagged_col = -1;             //elõzõ elõtti találat koordinátái
    int tagged_row = -1;
    int target_ship_align = 0;       //0:ismeretlen 1:függõleges 2:vízszintes
    reset_cpfire(&tagged_col, &tagged_row, &damaged_ship, &target_ship_align);
    while (p1intact > 0 && cp2intact > 0 || *global < 5)        //rossz kilépési feltétel   true végtelen loopra ér véget (vagyis nem)
    {
        if (*global < 5)
        {
            printf("Next: Player 1 (press a button)");
            getch();
            salvo(g, &p1intact, &cp2intact, &cpmiss, 1, &if_hit, &damaged_ship, &last_col, &last_row, &tagged_col, &tagged_row, &target_ship_align, &palette, global);
        }
        if (cp2intact == 0) *global = 5;
        if (*global < 5)
        {
            printf("Next: Player 2 (press a button)");
            getch();
            salvo(g, &p1intact, &cp2intact, &cpmiss, 2, &if_hit, &damaged_ship, &last_col, &last_row, &tagged_col, &tagged_row, &target_ship_align, &palette, global);
        }
        if (p1intact == 0) *global = 6;
    }
    system("cls");
    if (*global != 7)
    {
        show_pov(g, 1, &palette);         //1:játékos oldala
        show_pov(g, 2, &palette);
    }
    if (*global == 5) printf("Player 1 won\n");
    else if (*global == 6) printf("Player 2 won\n");
    else printf("Successful exit\n");

    free(g->s.p1navy);
    free(g->s.cp2navy);
    freeMap(g->s.p1, g->size);
    freeMap(g->s.cp2, g->size);
}
void NewGame(Game* g, char gamemode, char diff, int map_size, int* global) {
    srand(time(NULL));
    if (gamemode == 'a')
    {
        if (g->gm == 1)
        {
            CPGame(diff, map_size, g, global);
        }
        else if (g->gm == 2)
        {
            PvPGame(map_size, g, global);
        }
    }
    else if (gamemode == 'c')
    {
        CPGame(diff, map_size, g, global);
    }
    else
    {
        PvPGame(map_size, g, global);
    }
}
int main()
{
    //pull_out_the_cheats();
    //printf("Hello world!\n");
    char type, gm, diff;
    int size, global = 0;       //a global értéke alapján lehet eldönteni hogy a játék milyen fázisban van
    Game g;                     //0:kezdet    1:nc    2:np    3:lc    4:lp    5:p1win    6:cp2win

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
        if (gm == 'c')global = 1;
        else global = 2;
    }
    else {          //beolvasás lesz
        gm = 'a';
        diff = 'a';
        size = 0;
        load_game(&g);
        if (g.gm == 1)global = 3;
        else global = 4;
        //get_new_file(&g);      //eleinte csak ahhoz kellett ha nincs olyan file amit be akarnak tölteni de el tudja indítani a beolvasást
    }
    NewGame(&g, gm, diff, size, &global);
    return 0;
}
