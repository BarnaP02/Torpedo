#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>

//#define meret 26
//#define meret 24

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

typedef struct Map {
    Maptile terkep[26][26];     // ennél nem lehet nagyobb térkép
}Map;

typedef struct State {
    Map p1;
    Map cp;
    Hajo p1navy[26];
    Hajo cpnavy[26];
}State;

typedef struct Game {
    int diff;
    int size;
    int gm;
    State s;
}Game;

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

void set_size(int * psize) {
    printf("SMALL/MEDIUM/BIG/HUGE (S/M/B/H)\n");
    char input;
    input = tolower(_getch());
    while (input != 's' && input != 'm' && input != 'b' && input != 'h') {
        input = tolower(_getch());
    }
    switch (input){
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

void get_ship_coords(char *rown, int *col, char *align, int index, int map_size) {
    printf("Hova keruljon egy %d hosszu hajo?\n", index+1);
    char row = tolower(_getch());
    *rown = row - 'a';
    printf("%c", row + ('A' - 'a'));
    char tempcol[3];
    gets(tempcol);
    tempcol[2] = '\0';
    if (strlen(tempcol)<3)
    {
        *col = tempcol[0];
    }
    else
    {
        *col = (tempcol[0]-'0') * 10 + (tempcol[1]-'0');
    }
    while (*rown<0 || *rown>map_size - 1 || *col<0 || *col>map_size - 1)
    {
        printf("rossz input\nrown:%d\ntempc:%s   coln:%d\nstrlen:%d\nHova keruljon egy %d hosszu hajo?\n", *rown, tempcol, *col, strlen(tempcol), index + 1);
        row = tolower(_getch());
        *rown = row - 'a';
        printf("%c", row + ('A' - 'a'));
        gets(tempcol);
        for (int i = 0; i < 260; i++)
        {
            printf("%d\t%c\t", i, i);
        }
        printf("\n");
        if (strlen(tempcol) < 3)
        {
            *col = tempcol[0];
        }
        else
        {
            *col = tempcol[0] * 10 + tempcol[1];
        }
    }
    /*scanf("%d", col);
    while (*rown<0 || *rown>map_size - 1 || *col<0 || *col>map_size - 1)
    {
        printf("rossz input\nrown:%d\nHova keruljon egy %d hosszu hajo?\n", *rown, index+1);
        row = tolower(_getch());
        *rown = row - 'a';
        printf("%c",row+('A'-'a'));
        scanf("%d", col);      //ha ez nem szám akkor meghal
    }*/


    printf("Vizszintesen/Fuggolegesen (V/F)\n");
    *align = tolower(_getch());
    while (*align != 'v' && *align != 'f') {
        *align = tolower(_getch());
    }
    printf("align:%c\nkoord:%c%d\n", *align + ('A' - 'a'), row + ('A' - 'a'), *col);
}

bool placable(int rown, int coln, char align, int index, int map_size, Hajo* new_navy) {  //meg kell nézni hogy az egész hajó rajta lenne e a pályán (1) és hogy nincs e túl közel egy másik hajóhoz (2)
    Hajo ship;
    ship.b.x = coln;
    ship.b.y = rown;
    ship.intact_tiles = index+1;        //mert carriernél 4 lenne
    if (align=='v')
    {
        ship.e.x = coln + index;
        ship.e.y = rown;
    }
    else
    {
        ship.e.x = coln;
        ship.e.y = rown + index;
    }
    printf("mapsize:%d\nship-end:%c(%d) %d\n", map_size,ship.e.y+'A', ship.e.y, ship.e.x);
    if (ship.e.x > map_size - 1 || ship.e.y+'a' > map_size - 1) //csak az endet tesztelem mert begininget újra kéri a függvény ha nem megfelelelő
    {
        printf("placfals\n");
        return false;
    }
    return true;
}

void place_ship(int* navy, int index, Hajo* new_navy, int map_size) {
    char row, align;
    int rown, coln;
    get_ship_coords(&rown, &coln, &align, index, map_size);
    while (!placable(rown, coln, align, index, map_size, new_navy))
    {
        printf("nem volt a palyan\n");
        printf("placable:%d\n", placable(rown, coln, align, index, map_size, new_navy));
        get_ship_coords(&rown, &coln, &align, index, map_size);
    }
    /*for (int i = 0; i < index; i++)
    {
        if (align==f)
        {

        }
    }*/
}
void place_all_ships(int map_size, Hajo* new_navy) {
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
        navy[0] = 10;   //sub        (1x1)(azért csak tíz, mert már így is lehet egy teljes órát játszani, mivel hogy 676 mezőből áll egy oldal)
        navy[1] = 11;   //destroyer  (2x1)
        navy[2] = 9;    //cruiser    (3x1)
        navy[3] = 7;    //battleship (4x1)
        navy[4] = 4;    //carrier    (5x1)
        break;
    default:
        break;
    }
    int index = 4;
    for (int i = 4; i >= 0; i--)
    {
        while (navy[i] != 0) {
            place_ship(navy, i, new_navy, map_size);
            navy[i]--;
        }
    }
}

void clear_navy(Hajo* navy) {
    for (int i = 0; i < 41; i++)
    {
        navy[i].b.x = -1;
        navy[i].b.y = -1;
        navy[i].e.x = -1;
        navy[i].e.y = -1;
        navy[i].intact_tiles = -1;
    }
}

void CPGame(char gamemode, char diff, int map_size) {
    Map p1map;
    Hajo p1navy[41];    //a legnagyobb pályán 41 hajó van
    clear_navy(p1navy);
    place_all_ships(map_size,p1navy);
}

void NewGame(char gamemode, char diff, int map_size) {
    if (gamemode=='c')
    {
        CPGame(gamemode, diff, map_size);
    }
    else
    {
        //PVPGame();
    }
}



int main()
{
    printf("Hello world!\n");
    char type;
    char gm;
    char diff;
    int size;

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
    }
    NewGame(gm, diff, size);
    printf("type:%c\tgm:%c\tdiff:%c\n", type, gm, diff);

    return 0;
}
/*
* valtozas ami nagyon nagy
Naplóm:
11.06
amikor másodjára van megadva hibásan a hajó koordinátája lehelyezéskor akkor egy végtelen loopba fut
jól működik: tudja a program amikor olyan koordinátákat adunk meg amik nincsenek a térképen, figyelembe vaszi hogy mekkorára állítjuk azt
pl small (10x10) pályán nem hagyja hogy v5-re vagy a15-re rakjunk hajót
még nem tud: megnézni hogy lerakható e a hajó
álom: tudja hogy rárakná e egy másik hajóra vagy közvetlenül mellé



11.07
Ennél az outputnál látszik, hogy a betűs oszlopokat még nem kezeli jól
LOAD/NEW (L/N)
Player vs Computer/Player vs Player (C/P)
SMALL/MEDIUM/BIG/HUGE (S/M/B/H)
EASY/MEDIUM/HARD/EXTREME (E/M/H/X)
Hova keruljon egy 5 hosszu hajo?
A8
Vizszintesen/Fuggolegesen (V/F)
align:V
koord:A8
mapsize:10
ship-end:A(-858993664) 12                   <--ez itt jó mert a 12 az kilóg a pályáról
placfals
nem volt a palyan
mapsize:10
ship-end:A(-858993664) 12
placfals
placable:0                                  <--ez nem működik még a betűkkel
Hova keruljon egy 5 hosszu hajo?
G6
Vizszintesen/Fuggolegesen (V/F)
align:F
koord:G6
mapsize:10
ship-end:K(-858993654) 6                    <--itt a K az a 11. sor lenne (elvileg(még központosíítani kéne hogy mi az x és mi az y))
Hova keruljon egy 4 hosszu hajo?
*/
