#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include "Debug/debugmalloc.h"
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
    Maptile **p1;
    Maptile **cp2;
    Hajo* p1navy;
    Hajo* cp2navy;
}State;
typedef struct Game {
    int diff;
    int size;
    int gm;
    State s;
}Game;
void pull_out_the_cheats() {
    for (int i = 0; i < 260; i++)
    {
        printf("%3.d:%c\t", i, i);
    }
    printf("\n");
}

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

bool placable(int rown, int col, char align, int index, int map_size, Hajo* new_navy, int navy_size) {  //meg kell nézni hogy az egész hajó rajta lenne e a pályán (1) és hogy nincs e túl közel egy másik hajóhoz (2)
    Hajo ship;
    ship.b.x = col;
    ship.b.y = rown;       //mert egy nagy negatív szám és nem találom hogy miért
    ship.intact_tiles = index+1;        //mert carriernél 4 lenne
    if (align=='h')
    {
        ship.e.x = col + index;
        ship.e.y = rown;
    }
    else
    {
        ship.e.x = col;
        ship.e.y = rown + index;
    }
    if (ship.e.x > map_size - 1 || ship.e.y > map_size - 1) //csak az endet tesztelem mert begininget újra kéri a függvény ha nem megfelelelő
    {
        return false;
    }

    int this_ship_len = ((ship.e.x - ship.b.x) + (ship.e.y - ship.b.y)) % 256;
    Hajo that_ship;
    int that_ship_len = 0;
    for (int i = 0; i < navy_size - 1; i++)
    {
        that_ship.b.x = new_navy[i].b.x;
        that_ship.b.y = new_navy[i].b.y;
        that_ship.e.x = new_navy[i].e.x;
        that_ship.e.y = new_navy[i].e.y;
        that_ship_len = ((new_navy[i].e.x - new_navy[i].b.x) + (new_navy[i].e.y - new_navy[i].b.y)) % 256;  //ha vizszintes akkor b.y és e.y ugyan az, ha függőleges akkor pedig a b.x és e.x
        for (int i = 0; i < that_ship_len + 1; i++)     //végig megyünk az összes mezőjén a nézendő hajónak
        {
            for (int j = 0; j < this_ship_len + 1; j++)     //végignézzük az összes elemét a hajónak
            {
                if (ship.b.x==ship.e.x) //ha függőleges
                {
                    if (that_ship.b.x == that_ship.e.x)  //ha függőleges
                    {
                        if (abs(ship.b.x - that_ship.b.x) < 2 && abs((ship.b.y + j) - (that_ship.b.y + i)) < 2) {
                            return false;   //túl közel vannak egymáshoz
                        }
                    }
                    else
                    {
                        if (abs(ship.b.x - (that_ship.b.x + i)) < 2 && abs((ship.b.y + j) - that_ship.b.y) < 2) {
                            return false;   //túl közel vannak egymáshoz
                        }
                    }
                }
                else
                {
                    if (that_ship.b.x == that_ship.e.x)  //ha függőleges
                    {
                        if (abs((ship.b.x + j) - that_ship.b.x) < 2 && abs(ship.b.y - (that_ship.b.y + i)) < 2) {
                            return false;   //túl közel vannak egymáshoz
                        }
                    }
                    else
                    {
                        if (abs((ship.b.x + j) - (that_ship.b.x + i)) < 2 && abs(ship.b.y - that_ship.b.y) < 2) {
                            return false;   //túl közel vannak egymáshoz
                        }
                    }
                }
            }
        }

    }
    
    ////////////////////////////////////
    //printf("mukodik mert csodas\n");//
    ////////////////////////////////////
    
    return true;
}
void get_ship_coords(char *coln, int *row, char *align, int index, int map_size) {
    printf("Where do you want to place a ship with a length of %d?\n", index+1);
    char col = tolower(_getch());
    *coln = col - 'a';
    //printf("row:%c\trown:%d\n", col,*coln);
    printf("%c", col + ('A' - 'a'));
    char tempcol[3];
    gets(tempcol);
    tempcol[2] = '\0';
    if (strlen(tempcol)<2)
    {
        *row = tempcol[0] - '0' - 1;
        //printf("col:%d\n",*row);
    }
    else
    {
        *row = (tempcol[0]-'0') * 10 + (tempcol[1] - '0') - 1;
        //printf("col:%d\n", *row);
    }
    while (*coln<0 || *coln>map_size - 1 || *row<0 || *row>map_size - 1)
    {
        printf("invalid input\nWhere do you want to place a ship with a length of %d?\n", index + 1);
        col = tolower(_getch());
        *coln = col - 'a';
        //printf("--row:%c\trown:%d\n", col, *coln);
        printf("%c", col + ('A' - 'a'));
        gets(tempcol);
        if (strlen(tempcol) < 2)
        {
            *row = tempcol[0] - '0' - 1;
            //printf("col:%d\n", *row);
        }
        else
        {
            *row = (tempcol[0] - '0') * 10 + (tempcol[1] - '0') - 1;
            //printf("col:%d\n", *row);
        }
    }
    //*rown = *rown % 256;
    
    if (index != 0)
    {
        printf("Horizontal/Vertical (H/V)\n");
        *align = tolower(_getch());
        while (*align != 'h' && *align != 'v') {
            *align = tolower(_getch());
        }
    }
    else
    {
        *align = 'v';       //nem kéri be hogy milyen irányba akarjuk lerakni az 1 hosszú hajót
    }
}
void get_cp_ship_coords(int* coln, int* row, bool* align, int index, int map_size) {
    if (rand() % 2) *align = false;
    else *align = true;
    if (align)      //függőleges
    {
        *coln = rand() % map_size; //srand(time(0))
        *row = rand() % (map_size - (index + 1)); //srand(time(0))
    }
    else {
        *coln = rand() % (map_size - (index + 1)); //srand(time(0))
        *row = rand() % map_size; //srand(time(0))
    }
}
void place_ship(int* navy, int index, Hajo* new_navy, int map_size, int navy_size){
    char align;
    int coln, row;
    get_ship_coords(&coln, &row, &align, index, map_size);
    coln = coln % 256;
    if (coln < 0) coln += 256;
    while (!placable(row, coln, align, index, map_size, new_navy, navy_size))
    {
        //printf("nem volt a palyan\n");
        //printf("placable:%d\n", placable(row, coln, align, index, map_size, new_navy, navy_size));
        printf("Unplacable\n");
        get_ship_coords(&coln, &row, &align, index, map_size);
        coln = coln % 256;
        if (coln < 0) coln += 256;
    }
    Hajo ship;
    ship.b.x = coln;
    ship.b.y = row;
    ship.intact_tiles = index + 1;
    if (align == 'h')
    {
        ship.e.x = coln + index;
        ship.e.y = row;
    }
    else
    {
        ship.e.x = coln;
        ship.e.y = row + index;
    }
    new_navy[navy_size - 1] = ship;
}
void cp_place_ship(int* navy, int index, Hajo* new_navy, int map_size, int navy_size) {
    int coln, row;
    bool align;
    char alignc;
    get_cp_ship_coords(&coln, &row, &align, index, map_size);
    if (align) alignc = 'v';          //átírom karakterré hogy le tudjam futtatni rá a placable-t
    else alignc = 'h';
    while (!placable(row, coln, alignc, index, map_size,new_navy,  navy_size))
    {
        //printf("placable:%d\n", placable(row, coln, align, index, map_size, new_navy, navy_size));
        printf("Unplacable\n");
        get_cp_ship_coords(&coln, &row, &align, index, map_size);
        if (align) alignc = 'v';          //átírom karakterré hogy le tudjam futtatni rá a placable-t
        else alignc = 'h';
        coln = coln % 256;
        if (coln < 0) coln += 256;
    }
    Hajo cp_ship;
    cp_ship.b.x = coln;
    cp_ship.b.y = row;
    cp_ship.intact_tiles = index + 1;
    if (align == 0)
    {
        cp_ship.e.x = coln + index;
        cp_ship.e.y = row;
    }
    else
    {
        cp_ship.e.x = coln;
        cp_ship.e.y = row + index;
    }
    new_navy[navy_size-1] = cp_ship;
    /*
    for (int i = 0; i < navy_size; i++)
    {
        printf("?:[%d][%d]-[%d][%d] i[:%d]+++++++++\n", new_navy[i].b.x, new_navy[i].b.y, new_navy[i].e.x, new_navy[i].e.y, i);
    }*/
}
void put_navy_on_map(Maptile** mappp,int map_size, Hajo* navy, int navy_size) {
    char c = 219;
    for (int i = 0; i < navy_size; i++)
    {
        int len = ((navy[i].e.x - navy[i].b.x) + (navy[i].e.y - navy[i].b.y)) % 256;
        for (int j = 0; j < len; j++)
        {
            if (navy[i].b.x == navy[i].e.x) //ha függőleges
            {
                mappp[navy[i].b.x][navy[i].b.y + j].value = '1';
            }
            else
            {
                mappp[navy[i].b.x + j][navy[i].b.y].value = '1';
            }
            mappp[navy[i].e.x][navy[i].e.y].value = '1';
        }
        if (navy[i].intact_tiles == 1) {
            mappp[navy[i].b.x][navy[i].b.y].value = '1';
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
                //printf(".%c", mappp[j][i].value);
                mappp[i][j].hit = false;
            }
            //printf("\n");
        }
        //printf("\n");
    }
}
void set_color(int mod, int* palette) {        //mód az hogy víz/meglőtt víz/hajó/sérült hajó/elsüllyesztett hajó
    HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);
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
    case 1:         //meglőtt víz
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
    case 3:         //meglőtt hajó
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
int get_ship_by_coords(Maptile** map, Hajo* h, int size, int x, int y) {       //visszaadja a sorszámát annak a hajónak a listából amelyiknek az egyik mezője a megadott (x,y)
    for (int i = 0; i < set_navy_size(size); i++)     //végig megyünk az összes mezőjén a nézendő hajónak    //a placableből módosítva
    {
        int this_ship_len = (h[i].e.x - h[i].b.x) + (h[i].e.y - h[i].b.y);
        for (int j = 0; j < this_ship_len + 1; j++)     //végignézzük az összes elemét a hajónak
        {
            if (h[i].b.x == h[i].e.x) //ha függőleges
            {
                if (h[i].b.x == x && h[i].b.y + j == y) return i;
            }
            else
            {
                if (h[i].b.x + j == x && h[i]. b.y == y) return i;
            }
        }
    }
}
void show_pov(Game* g, int pov, int* palette){        // welcome to the jungle
    char sarok1 = 201; //╔
    char sarok2 = 187; //╗
    char sarok3 = 200; //╚
    char sarok4 = 188; //╝
    char vvonal = 205; //═
    char fvonal = 186; //║
    char fhajo = 223; //▀
    char khajo = 254; //■
    char ahajo = 220; //▄
    char thajo = 219; //█
    set_color(-1, palette);
    //printf("size:%d", g->size);
    //elso sor
    printf("%c", sarok1);
    for (int i = 0; i < g->size * 2 + 9; i++)
    {
        printf("%c", vvonal);
    }
    printf("%c%c", sarok2, sarok1);
    for (int i = 0; i < g->size * 2 + 9; i++)
    {
        printf("%c", vvonal);
    }
    printf("%c\n", sarok2);
    //szamsor
    printf("%c     ", fvonal);
    for (int i = 0; i < g->size; ++i)
    {
        printf("%c ", i + 'A');
    }
    printf("    %c", fvonal);
    printf("%c     ", fvonal);
    for (int i = 0; i < g->size; ++i)
    {
        printf("%c ", i + 'A');
    }
    printf("    %c\n", fvonal);
    //szamsor alatti
    printf("%c   %c", fvonal, sarok1);
    for (int i = 0; i < g->size * 2 + 1; ++i)
    {
        printf("%c", vvonal);
    }
    printf("%c   %c", sarok2, fvonal);
    printf("%c   %c", fvonal, sarok1);
    for (int i = 0; i < g->size * 2 + 1; ++i)
    {
        printf("%c", vvonal);
    }
    printf("%c   %c\n", sarok2, fvonal);
    //egesz palya
    for (int j = 0; j < g->size; j++)
    {
        if (j + 1 < 10) printf("%c %d %c", fvonal, j + 1, fvonal);
        else printf("%c%d %c", fvonal, j + 1, fvonal);

        set_color(0, palette);
        for (int i = 0; i < g->size; ++i)
        {
            if (pov == 1)
            {
                if (g->s.p1[i][j].value == '1')
                {
                    if (g->s.p1[i][j].hit == true)
                        if (g->s.p1navy[get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, i, j)].intact_tiles == 0) {
                            set_color(4, palette);    //a hajó el van süllyedve
                        }
                        else {
                            set_color(3, palette);
                        }
                    else set_color(2, palette);
                    if (i == 0 && j == 0) // bal felső sarok
                    {
                        if (g->s.p1[i + 1][j].value == '1') printf(" %c", khajo); //jobbra lévő mező
                        else if (g->s.p1[i][j + 1].value == '1') printf(" %c", ahajo); //alatta lévő mező, nem kell else mert a kód nem engedi hogy olyan helyzet jöjjön létre, de így tudjuk hogy ha ez sem teljesül akkor nincs mellette hajó mező
                        else printf(" %c", ahajo);
                    }
                    else if (i == 0 && j == g->size - 1) // bal alsó sarok
                    {
                        if (g->s.p1[i][j - 1].value == '1') printf(" %c", fhajo); //fölötte lévő mező
                        else if (g->s.p1[i + 1][j].value == '1') printf(" %c", khajo); //jobbra lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (i == g->size-1 && j == 0) // jobb felső sarok
                    {
                        if (g->s.p1[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else if (g->s.p1[i][j + 1].value == '1') printf(" %c", ahajo); //alatta lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (i == g->size-1 && j == g->size - 1) // jobb alsó sarok
                    {
                        if (g->s.p1[i][j - 1].value == '1') printf(" %c", fhajo); //fölötte lévő mező
                        else if (g->s.p1[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (i == 0)    //bal oldal
                    {
                        if (g->s.p1[i + 1][j].value == '1') printf(" %c", khajo); //jobbra lévő mező
                        else if (g->s.p1[i][j - 1].value == '1' && g->s.p1[i][j + 1].value == '1') printf(" %c", thajo); //fölötte és alatta lévő mező
                        else if (g->s.p1[i][j - 1].value == '1') printf(" %c", fhajo); //csak a fölötte lévő mező
                        else if (g->s.p1[i][j + 1].value == '1') printf(" %c", ahajo); //csak az alatta lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (j == 0) //felső oldal
                    {
                        if (g->s.p1[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else if (g->s.p1[i + 1][j].value == '1') printf(" %c", khajo); //jobbra lévő mező     azért kell else mert a bal is megycsinálja ugyan azt mint amit ez csinálna
                        else if (g->s.p1[i][j + 1].value == '1') printf(" %c", ahajo); //alatta lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (i == g->size - 1) //jobb oldal
                    {
                        if (g->s.p1[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else if (g->s.p1[i][j - 1].value == '1' && g->s.p1[i][j + 1].value == '1') printf(" %c", thajo); //fölötte és alatta lévő mező
                        else if (g->s.p1[i][j - 1].value == '1') printf(" %c", fhajo); //fölötte lévő mező
                        else if (g->s.p1[i][j + 1].value == '1') printf(" %c", ahajo); //alatta lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (j == g->size - 1) //alsó oldal
                    {
                        if (g->s.p1[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else if (g->s.p1[i + 1][j].value == '1') printf(" %c", khajo); //jobbra lévő mező
                        else if (g->s.p1[i][j - 1].value == '1') printf(" %c", fhajo); //fölötte lévő mező
                        else printf(" %c", ahajo);
                    }
                    else
                    {
                        if (g->s.p1[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else if (g->s.p1[i + 1][j].value == '1') printf(" %c", khajo); //jobbra lévő mező
                        else if (g->s.p1[i][j - 1].value == '1' && g->s.p1[i][j + 1].value == '1') printf(" %c", thajo); //fölötte és alatta lévő mező
                        else if (g->s.p1[i][j - 1].value == '1') printf(" %c", fhajo); //fölötte lévő mező
                        else if (g->s.p1[i][j + 1].value == '1') printf(" %c", ahajo); //alatta lévő mező
                        else printf(" %c", khajo);
                    }
                    set_color(0, palette);
                }
                else
                {
                    if (g->s.p1[i][j].hit == true) set_color(1, palette);
                    else set_color(0, palette);
                    printf(" ~");
                }
            }
            else
            {
                if (g->s.p1[i][j].hit)
                {
                    if (g->s.p1navy[get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, i, j)].intact_tiles == 0) {
                        set_color(4, palette);    //a hajó el van süllyedve
                    }
                    else set_color(3, palette);
                    if (g->s.p1[i][j].value == '1')
                    {
                        printf(" x");
                    }
                    else
                    {
                        printf(" o");
                    }
                }
                else
                {
                    set_color(0, palette);
                    printf(" ~");
                }
            }
            
        }
        printf(" ");
        set_color(-1, palette);
        printf("%c   %c", fvonal, fvonal);
        if (j + 1 < 10) printf("%c %d %c", fvonal, j + 1, fvonal);
        else printf("%c%d %c", fvonal, j + 1, fvonal);
        set_color(0, palette);
        for (int i = 0; i < g->size; ++i)
        {
            if (pov == 1)
            {
                if (g->s.cp2[i][j].hit)
                {
                    if (g->s.cp2navy[get_ship_by_coords(g->s.cp2, g->s.cp2navy, g->size, i, j)].intact_tiles == 0) {
                        set_color(4, palette);    //a hajó el van süllyedve
                    }
                    else set_color(3, palette);
                    if (g->s.cp2[i][j].value == '1')
                    {
                        printf(" x");
                    }
                    else
                    {
                        printf(" o");
                    }
                }
                else
                {
                    set_color(0, palette);
                    printf(" ~");
                }
            }
            else
            {
                if (g->s.cp2[i][j].value == '1') {
                    if (g->s.cp2[i][j].hit == true)
                        if (g->s.cp2navy[get_ship_by_coords(g->s.cp2, g->s.cp2navy, g->size, i, j)].intact_tiles == 0) set_color(4, palette);    //a hajó el van süllyedve
                        else set_color(3, palette);
                    else set_color(2, palette);
                    if (i == 0 && j == 0) // bal felső sarok
                    {
                        if (g->s.cp2[i + 1][j].value == '1') printf(" %c", khajo); //jobbra lévő mező
                        else if (g->s.cp2[i][j + 1].value == '1') printf(" %c", ahajo); //alatta lévő mező, nem kell else mert a kód nem engedi hogy olyan helyzet jöjjön létre, de így tudjuk hogy ha ez sem teljesül akkor nincs mellette hajó mező
                        else printf(" %c", ahajo);
                    }
                    else if (i == 0 && j == g->size - 1) // bal alsó sarok
                    {
                        if (g->s.cp2[i][j - 1].value == '1') printf(" %c", fhajo); //fölötte lévő mező
                        else if (g->s.cp2[i + 1][j].value == '1') printf(" %c", khajo); //jobbra lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (i == g->size - 1 && j == 0) // jobb felső sarok
                    {
                        if (g->s.cp2[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else if (g->s.cp2[i][j + 1].value == '1') printf(" %c", ahajo); //alatta lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (i == g->size - 1 && j == g->size - 1) // jobb alsó sarok
                    {
                        if (g->s.cp2[i][j - 1].value == '1') printf(" %c", fhajo); //fölötte lévő mező
                        else if (g->s.cp2[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (i == 0)    //bal oldal
                    {
                        if (g->s.cp2[i + 1][j].value == '1') printf(" %c", khajo); //jobbra lévő mező
                        else if (g->s.cp2[i][j - 1].value == '1' && g->s.cp2[i][j + 1].value == '1') printf(" %c", thajo); //fölötte és alatta lévő mező
                        else if (g->s.cp2[i][j - 1].value == '1') printf(" %c", fhajo); //csak a fölötte lévő mező
                        else if (g->s.cp2[i][j + 1].value == '1') printf(" %c", ahajo); //csak az alatta lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (j == 0) //felső oldal
                    {
                        if (g->s.cp2[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else if (g->s.cp2[i + 1][j].value == '1') printf(" %c", khajo); //jobbra lévő mező     azért kell else mert a bal is megycsinálja ugyan azt mint amit ez csinálna
                        else if (g->s.cp2[i][j + 1].value == '1') printf(" %c", ahajo); //alatta lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (i == g->size - 1) //jobb oldal
                    {
                        if (g->s.cp2[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else if (g->s.cp2[i][j - 1].value == '1' && g->s.cp2[i][j + 1].value == '1') printf(" %c", thajo); //fölötte és alatta lévő mező
                        else if (g->s.cp2[i][j - 1].value == '1') printf(" %c", fhajo); //fölötte lévő mező
                        else if (g->s.cp2[i][j + 1].value == '1') printf(" %c", ahajo); //alatta lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (j == g->size - 1) //alsó oldal
                    {
                        if (g->s.cp2[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else if (g->s.cp2[i + 1][j].value == '1') printf(" %c", khajo); //jobbra lévő mező
                        else if (g->s.cp2[i][j - 1].value == '1') printf(" %c", fhajo); //fölötte lévő mező
                        else printf(" %c", ahajo);
                    }
                    else
                    {
                        if (g->s.cp2[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else if (g->s.cp2[i + 1][j].value == '1') printf(" %c", khajo); //jobbra lévő mező
                        else if (g->s.cp2[i][j - 1].value == '1' && g->s.cp2[i][j + 1].value == '1') printf(" %c", thajo); //fölötte és alatta lévő mező
                        else if (g->s.cp2[i][j - 1].value == '1') printf(" %c", fhajo); //fölötte lévő mező
                        else if (g->s.cp2[i][j + 1].value == '1') printf(" %c", ahajo); //alatta lévő mező
                        else printf(" %c", khajo);
                    }
                    set_color(0, palette);
                }
                else
                {
                    if (g->s.cp2[i][j].hit == true) set_color(1, palette);
                    else set_color(0, palette);
                    printf(" ~");
                }
            }
        }
        printf(" ");
        set_color(-1, palette);
        printf("%c   %c\n", fvonal, fvonal);
    }
    //palya alatti
    printf("%c   %c", fvonal, sarok3);
    for (int i = 0; i < g->size * 2 + 1; ++i)
    {
        printf("%c", vvonal);
    }
    printf("%c   %c", sarok4, fvonal);
    printf("%c   %c", fvonal, sarok3);
    for (int i = 0; i < g->size * 2 + 1; ++i)
    {
        printf("%c", vvonal);
    }
    printf("%c   %c\n", sarok4, fvonal);
    //szamok helye alul
    printf("%c     ", fvonal);
    for (int i = 0; i < g->size; ++i)
    {
        printf("  ");
    }
    printf("    %c", fvonal);
    printf("%c     ", fvonal);
    for (int i = 0; i < g->size; ++i)
    {
        printf("  ");
    }
    printf("    %c\n", fvonal);    
    //legalso
    printf("%c", sarok3);
    for (int i = 0; i < g->size * 2 + 9; i++)
    {
        printf("%c", vvonal);
    }
    printf("%c%c", sarok4, sarok3);
    for (int i = 0; i < g->size * 2 + 9; i++)
    {
        printf("%c", vvonal);
    }
    printf("%c\n", sarok4);

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
    int navy_size = 0;
    for (int i = 4; i >= 0; i--)
    {
        while (navy[i] != 0) {
            system("cls");
            if (g->gm == 1) {
                if (!is_cp_game) show_pov(g, 1, palette);         //1:játékos oldala
                navy_size++;
                if (is_cp_game) cp_place_ship(navy, i, new_navy, map_size, navy_size);
                else place_ship(navy, i, new_navy, map_size, navy_size);
                put_navy_on_map(mappp, map_size, new_navy, set_navy_size(map_size));
                navy[i]--;
            }
            else
            {
                if(is_cp_game) show_pov(g, 2, palette);
                else show_pov(g, 1, palette);         //1:játékos oldala
                navy_size++;
                place_ship(navy, i, new_navy, map_size, navy_size);
                put_navy_on_map(mappp, map_size, new_navy, set_navy_size(map_size));
                navy[i]--;
            }
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
void save_game(Game* g, char* name) {
    FILE* fp; /* fájl mutató (file pointer/handle) */
    int len_of_navy = set_navy_size(g->size);
    fp = fopen(name, "w"); /* megnyitás */
    if (fp == NULL) {
        perror("Fájl megnyitása sikertelen");
        return;    /* nem folytathatjuk! */
    }
    fprintf(fp, "%d %d %d\n",g->gm,g->diff,g->size);
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
void init_game(Game* g, int diff, int size, int gm) {
    Maptile** p1map = mallocMap(size), ** cpmap = mallocMap(size);
    int len_of_navy = set_navy_size(size);
    Hajo* p1navy = (Hajo*)malloc(len_of_navy * sizeof(Hajo));
    if (p1navy == NULL) printf("itt a hiba\n");
    Hajo* cpnavy = (Hajo*)malloc(len_of_navy * sizeof(Hajo));
    if (cpnavy == NULL) printf("itt a hiba\n");
    State s;
    create_state(&s, p1map, cpmap, p1navy, cpnavy);
    create_game(g, s, diff, size, gm);       //1: CPvP
}
void get_new_file(char* name) {
    char input_name[251];
    for (int i = 0; i < 255; i++)
    {
        name[i] = '\0';
    }
    for (int i = 0; i < 251; i++)
    {
        input_name[i] = '\0';
    }
    gets(input_name);
    strcat(name, input_name);
    strcat(name, ".txt");
}
void load_game(Game* g) {
    char name[255];
    char input;
    bool back = false;
    get_new_file(name);
    FILE* fp = fopen(name, "r"); /* megnyitás */
    while(fp == NULL || back) {
        perror("Fajl megnyitasa sikertelen");
        printf("\nPress:\n (b)\tto go back\n(l)\tto load an other file\n");
        input = tolower(_getch());
        while (input !='b'&& input!='l')
            input = tolower(_getch());
        if (input='l')
        {
            get_new_file(name);
        }
        else
        {
            back = true;
        }
    }
    if (back)
    {
        fclose(fp);
        return;    /* nem folytathatjuk! */
    }              /* itt már megtalálta a fájlt*/
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
                index++;                        //azért van itt mert a következő karakter már másik mezőhöz tartozik
            }
            else {
                g->s.p1[index][i].value = shift;
            }
            shift = fgetc(fp);
            if (shift == ' ') {
                was_space = true;
                shift = fgetc(fp);
            }
        }
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
                index++;                        //azért van itt mert a következő karakter már másik mezőhöz tartozik
            }
            else {
                g->s.cp2[index][i].value = shift;
            }
            shift = fgetc(fp);
            if (shift == ' ') {
                was_space = true;
                shift = fgetc(fp);
            }
        }
    }
    for (int i = 0; i < set_navy_size(g->size); i++)
    {
        g->s.cp2navy[i].b.x = get_next_int(fp);
        g->s.cp2navy[i].b.y = get_next_int(fp);
        g->s.cp2navy[i].e.x = get_next_int(fp);
        g->s.cp2navy[i].e.y = get_next_int(fp);
        g->s.cp2navy[i].intact_tiles = get_next_int(fp);
    }

    fclose(fp);
}

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
            while (g->s.p1navy[*damaged_ship].intact_tiles==0)         //ez a hajó már korábban elsüllyedt
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
            if (g->s.p1[*last_col][*last_row].hit==false)     //találat
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
            --*cpmiss;
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
void salvo(Game* g, int* p1intact, int* cp2intact, int* cpmiss, int pov, bool* if_hit, int* damaged_ship, int* last_col, int* last_row, int* first_hit_col, int* first_hit_row, int* align, int* palette, int* global) {
    //printf("pov%d\n", pov);
    if (*global == 7) return;
    bool ship_ended = false;        //azért kell hogy ne legyen rekurzív a cpfire   //akkor lesz true amikor visszaugrik az első találat koordinátára a cpfire anélkül hogy lőne
    if (pov==1)
    {
        system("cls");
        show_pov(g, pov, palette);         //1:játékos oldala
        fire(g, if_hit, pov, palette, global);
        if (*global == 7) return;
        Sleep(2);
        while (*if_hit)
        {
            system("cls");
            --*cp2intact;
            show_pov(g, pov, palette);         //1:játékos oldala
            if (*cp2intact > 0) fire(g, if_hit, pov, palette, global);
            else *if_hit = false;
            if (*global == 7) return;
            Sleep(2);
        }
        //printf("fire--\n");
        system("cls");
        show_pov(g, pov, palette);         //1:játékos oldala
        Sleep(2);
        //getch();
    }
    else
    {
        //printf("gm:%d\n", g->gm);
        if (g->gm == 1)
        {
            cpfire(g, p1intact, cp2intact, cpmiss, if_hit, &ship_ended, damaged_ship,  last_col, last_row, first_hit_col, first_hit_row, align);  //a gép mindig ugyanazon az oldalon van
            while (*if_hit)
            {
                system("cls");
                if (!ship_ended) --*p1intact;   //ha ship_ended van akkor lövés nélkül ugrik vissza a cpfire                
                //printf("loves utan:\n");
                show_pov(g, 1, palette);         //1:játékos oldala
                Sleep(1200);
                if (*p1intact > 0) cpfire(g, p1intact, cp2intact, cpmiss, if_hit, &ship_ended, damaged_ship, last_col, last_row, first_hit_col, first_hit_row, align);
                else *if_hit = false;

            }
            //printf("[%d,%d]\n", *last_row, *last_col);
        }
        else
        {
            system("cls");
            show_pov(g, pov, palette);
            fire(g, if_hit, pov, palette, global);      //itt a pov 2
            if (*global == 7) return;
            Sleep(2);
            while (*if_hit)
            {
                system("cls");
                --*p1intact;
                show_pov(g, pov, palette);
                if (*p1intact > 0) fire(g, if_hit, pov, palette, global);
                else *if_hit = false;
                if (*global == 7) return;
            }
            system("cls");
            show_pov(g, pov, palette);
            //printf("salvo: %d:%d\n", *p1intact, *cp2intact);
        }
    }
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
    int p1intact = 0, cp2intact = 0;//sértetlen hajómezők száma
    int cpmiss = 0;    //vízmezők száma
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
    int last_col;                    //elző találat koordinátái
    int last_row;
    int tagged_col = -1;             //előző előtti találat koordinátái
    int tagged_row = -1;
    int target_ship_align = 0;       //0:ismeretlen 1:függőleges 2:vízszintes
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
        place_all_ships(g, g->s.cp2, map_size, g->s.cp2navy, true, &palette);
        put_navy_on_map(g->s.p1, map_size, g->s.p1navy, len_of_navy);
        put_navy_on_map(g->s.cp2, map_size, g->s.cp2navy, len_of_navy);
    }
    printf("Eternal Bliss\n");      //létfontosságú

    save_game(g, "fisrt_save.txt");//direkt van elírva
    system("cls");
    int p1intact = 0, cp2intact = 0;//sértetlen hajómezők száma
    int cpmiss = 0;                 //vízmezők száma (azért kell hogy tudjuk hogy a gép mikor nem fog tudni több vízmezőt eltalálni)
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
    int last_col;                    //elző találat koordinátái
    int last_row;
    int tagged_col = -1;             //előző előtti találat koordinátái
    int tagged_row = -1;
    int target_ship_align = 0;       //0:ismeretlen 1:függőleges 2:vízszintes
    reset_cpfire(&tagged_col, &tagged_row, &damaged_ship, &target_ship_align);
    while (p1intact > 0 && cp2intact > 0 && *global < 5)        //rossz kilépési feltétel   true végtelen loopra ér véget (vagyis nem)
    {
        if (*global < 5)
        {
            Sleep(2000);
            system("cls");
            printf("Next: Player 1 (press a button)");
            getch();
            salvo(g, &p1intact, &cp2intact, &cpmiss, 1, &if_hit, &damaged_ship, &last_col, &last_row, &tagged_col, &tagged_row, &target_ship_align, &palette, global);
        }
        if (cp2intact == 0) *global = 5;
        if (*global < 5)
        {
            Sleep(2000);
            system("cls");
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
    //Maptile** Map = mallocMap(map_size);
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
    else if (gamemode=='c')
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
    printf("*******************************************************************\n");
    printf("*  ______    ______   _______   ______  ______   _____    ______  *\n");
    printf("* |__   __| /  __  | |  __   | |  __ | |  ,---* |  .  '. /  __  | *\n");
    printf("*    | |    | /  | | |  |/  /  |  '^_/ |  '---, |  ||  | | /  | | *\n");
    printf("*    | |    | |__/ | |  .  ',  |  |'   |  ,_-_' |  |/  / | |__/ | *\n");
    printf("*    |_|    |______/ |__|'.__| |__|    |______| |_____/  |______/ *\n");
    printf("*                                                                 *\n");
    printf("*******************************************************************\n\n\n");
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
