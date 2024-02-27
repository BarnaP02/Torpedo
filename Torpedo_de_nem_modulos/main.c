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

Maptile** mallocMap(size) {
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
    if (align=='v')
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
        /*if (that_ship_len == 0) {
            that_ship_len = -1;    //az üres hajóhoz nem kell hasonlítani
            printf("nullashajo:[%d][%d]-[%d][%d] i[:%d]+++++++++\n", that_ship.b.x, that_ship.b.y, that_ship.e.x, that_ship.e.y, i);
            printf("masik:[%d][%d]-[%d][%d] i[:%d]+++++++++\n", ship.b.x, ship.b.y, ship.e.x, ship.e.y, i);
        }
        if (that_ship.b.x==-1)
        {
            return true;
        }*/
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
    printf("mukodik mert csodas\n");
    return true;
}
void get_ship_coords(char *coln, int *row, char *align, int index, int map_size) {
    printf("Hova keruljon egy %d hosszu hajo?\n", index+1);
    char col = tolower(_getch());
    *coln = col - 'a';
    printf("row:%c\trown:%d\n", col,*coln);
    printf("%c", col + ('A' - 'a'));
    char tempcol[3];
    gets(tempcol);
    tempcol[2] = '\0';
    if (strlen(tempcol)<2)
    {
        *row = tempcol[0] - '0' - 1;
        printf("col:%d\n",*row);
    }
    else
    {
        *row = (tempcol[0]-'0') * 10 + (tempcol[1] - '0') - 1;
        printf("col:%d\n", *row);
    }
    while (*coln<0 || *coln>map_size - 1 || *row<0 || *row>map_size - 1)
    {
        printf("rossz input\nrown:%d\ntempc:%s   coln:%d\nstrlen:%d\nHova keruljon egy %d hosszu hajo?\n", *coln, tempcol, *row, strlen(tempcol), index + 1);
        col = tolower(_getch());
        *coln = col - 'a';
        printf("--row:%c\trown:%d\n", col, *coln);
        printf("%c", col + ('A' - 'a'));
        gets(tempcol);
        if (strlen(tempcol) < 2)
        {
            *row = tempcol[0] - '0' - 1;
            printf("col:%d\n", *row);
        }
        else
        {
            *row = (tempcol[0] - '0') * 10 + (tempcol[1] - '0') - 1;
            printf("col:%d\n", *row);
        }
    }
    //*rown = *rown % 256;
    

    printf("Vizszintesen/Fuggolegesen (V/F)\n");
    *align = tolower(_getch());
    while (*align != 'v' && *align != 'f') {
        *align = tolower(_getch());
    }
    printf("align:%c\nkoord:%c%d [%d,%d]\n", *align + ('A' - 'a'), row + ('A' - 'a'), *row, *coln, *row);
    printf("++row:%c\trown:%d\n", col, *coln);
}
void get_cp_ship_coords(int* coln, int* row, bool* align, int index, int map_size) {
     //srand(time(0))
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
    printf("cp:[%d:%d] i:%d a:%d s:%d\n", *coln, *row, index, *align, map_size);
}
void place_ship(int* navy, int index, Hajo* new_navy, int map_size, int navy_size){
    char align;
    int coln, row;
    get_ship_coords(&coln, &row, &align, index, map_size);
    coln = coln % 256;
    if (coln < 0) coln += 256;
    //printf("||rown:%d\tcol:%d\talign:%d\tindex:%d\tmapsize:%d\tnavy_size:%d\n", rown, col, align, index, map_size, navy_size);
    while (!placable(row, coln, align, index, map_size, new_navy, navy_size))
    {
        //printf("nem volt a palyan\n");
        printf("placable:%d\n", placable(row, coln, align, index, map_size, new_navy, navy_size));
        get_ship_coords(&coln, &row, &align, index, map_size);
        coln = coln % 256;
        if (coln < 0) coln += 256;
    }
    Hajo ship;
    ship.b.x = coln;
    ship.b.y = row;
    ship.intact_tiles = index + 1;
    if (align == 'v')
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
    if (align) alignc = 'f';          //átírom karakterré hogy le tudjam futtatni rá a placable-t
    else alignc = 'v';
    while (!placable(row, coln, alignc, index, map_size,new_navy,  navy_size))
    {
        printf("placable:%d\n", placable(row, coln, alignc, index, map_size, new_navy, navy_size));
        get_cp_ship_coords(&coln, &row, &align, index, map_size);
        if (align) alignc = 'f';          //átírom karakterré hogy le tudjam futtatni rá a placable-t
        else alignc = 'v';
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
    for (int i = 0; i < navy_size; i++)
    {
        printf("?:[%d][%d]-[%d][%d] i[:%d]+++++++++\n", new_navy[i].b.x, new_navy[i].b.y, new_navy[i].e.x, new_navy[i].e.y, i);
    }
}
void put_navy_on_map(Maptile** mappp,int map_size, Hajo* navy, int navy_size) {
    //printf("cso\n%d\n",navy_size);
    char c = 219;
    for (int i = 0; i < navy_size; i++)
    {
        int len = ((navy[i].e.x - navy[i].b.x) + (navy[i].e.y - navy[i].b.y)) % 256;
        for (int j = 0; j < len; j++)
        {
            //printf("cso1\n");
            if (navy[i].b.x == navy[i].e.x) //ha függőleges
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
void set_color(int mod, int* palette) {        //mód az hogy víz/meglőtt víz/hajó/sérült hajó/elsüllyesztett hajó
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
    /*for (int i = 0; i < set_navy_size(g->size); i++)
    {
        printf("i:%d\n", g->s.p1navy[i].intact_tiles);        
    }*/
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
    //int* palette;
    for (int j = 0; j < g->size; j++)
    {
        //printf("%c %c %c", fvonal, j + '0', fvonal); // szét kell szedni
        if (j + 1 < 10) printf("%c %d %c", fvonal, j + 1, fvonal);
        else printf("%c%d %c", fvonal, j + 1, fvonal);

        set_color(0, palette);
        //SetConsoleTextAttribute(hc, 0x5d);
        //SetConsoleTextAttribute(hc, 0x19);
        for (int i = 0; i < g->size; ++i)
        {
            if (pov == 1)
            {
                if (g->s.p1[i][j].value == '1')
                {
                    if (g->s.p1[i][j].hit == true)
                        if (g->s.p1navy[get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, i, j)].intact_tiles == 0) {
                            set_color(4, palette);    //a hajó el van süllyedve
                            //printf("4");
                        }
                        else {
                            set_color(3, palette);
                            //printf("%d", get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, i, j));
                        }
                    else set_color(2, palette);
                    /*if (g->s.p1[i][j].hit==true) SetConsoleTextAttribute(hc, 0x58);
                    else SetConsoleTextAttribute(hc, 0x57);*/
                    //SetConsoleTextAttribute(hc, 0x17);
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
                    //SetConsoleTextAttribute(hc, 0x5d);
                    //SetConsoleTextAttribute(hc, 0x19);
                }
                else
                {
                    if (g->s.p1[i][j].hit == true) set_color(1, palette);
                    else set_color(0, palette);
                    /*if (g->s.p1[i][j].hit == true) SetConsoleTextAttribute(hc, 0x58);
                    else SetConsoleTextAttribute(hc, 0x5d);*/
                    printf(" ~");
                    //set_color(1, palette);
                    //SetConsoleTextAttribute(hc, 0x5d);
                }
            }
            else
            {
                if (g->s.p1[i][j].hit)
                {
                    if (g->s.p1navy[get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, i, j)].intact_tiles == 0) {
                        set_color(4, palette);    //a hajó el van süllyedve
                        //printf("4");
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
        //SetConsoleTextAttribute(hc, 0x07);
        printf("%c   %c", fvonal, fvonal);
        //printf("%c %c %c", fvonal, j + '0', fvonal); // szét kell szedni
        if (j + 1 < 10) printf("%c %d %c", fvonal, j + 1, fvonal);
        else printf("%c%d %c", fvonal, j + 1, fvonal);
        set_color(0, palette);
        //SetConsoleTextAttribute(hc, 0x5d);
        //SetConsoleTextAttribute(hc, 0x19);
        for (int i = 0; i < g->size; ++i)
        {
            if (pov == 1)
            {
                if (g->s.cp2[i][j].hit)
                {
                    if (g->s.cp2navy[get_ship_by_coords(g->s.cp2, g->s.cp2navy, g->size, i, j)].intact_tiles == 0) {
                        set_color(4, palette);    //a hajó el van süllyedve
                        //printf("4");
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
                /*if (g->s.cp2[i][j].hit) printf(" %c", g->s.cp2[i][j].value);
                else printf(" ~"); //, g->s.cp2[j][i].hit);*/
            }
            else
            {
                if (g->s.cp2[i][j].value == '1') {
                    if (g->s.cp2[i][j].hit == true)
                        if (g->s.cp2navy[get_ship_by_coords(g->s.cp2, g->s.cp2navy, g->size, i, j)].intact_tiles == 0) set_color(4, palette);    //a hajó el van süllyedve
                        else set_color(3, palette);
                    else set_color(2, palette);
                    /*if (g->s.cp2[i][j].hit == true) SetConsoleTextAttribute(hc, 0x58);
                    else SetConsoleTextAttribute(hc, 0x57);*/
                    //SetConsoleTextAttribute(hc, 0x17);
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
                    //SetConsoleTextAttribute(hc, 0x5d);
                    //SetConsoleTextAttribute(hc, 0x19);
                //printf(" %c", g->s.cp2[i][j].value);
                }
                else
                {
                    if (g->s.p1[i][j].hit == true) set_color(1, palette);
                    else set_color(0, palette);
                    /*if (g->s.p1[i][j].hit == true) SetConsoleTextAttribute(hc, 0x58);
                    else SetConsoleTextAttribute(hc, 0x5d);*/
                    printf(" ~");
                    //set_color(1, palette);
                    //SetConsoleTextAttribute(hc, 0x5d);
                }
            }
        }
        printf(" ");
        set_color(-1, palette);
        //SetConsoleTextAttribute(hc, 0x07);
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
    //char fajlnev[] = "last_save.txt";
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
void get_new_file(Game* g);     //ez azért kell ide mert ez a függvény meghívja a load_game-t de az is meghívja ezt
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
                index++;                        //azért van itt mert a következő karakter már másik mezőhöz tartozik
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
                index++;                        //azért van itt mert a következő karakter már másik mezőhöz tartozik
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

void menu(Game* g, int* palette) {
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
        free(g->s.p1navy);
        free(g->s.cp2navy);
        freeMap(g->s.p1, g->size);
        freeMap(g->s.cp2, g->size);
        exit(0);
        break;
    case 'e':
        free(g->s.p1navy);
        free(g->s.cp2navy);
        freeMap(g->s.p1, g->size);
        freeMap(g->s.cp2, g->size);
        exit(0);
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
void get_target(Game* g, int* coln, int* row, int* palette) {
    printf("Hova akarsz loni? (pl. 'A8') (menu: '.')\n");
    char col = _getch();
    //int row = 0;
    if (col == '.')
    {
        menu(g, palette);
        //col = _getch();
    }
    col = tolower(col);
    *coln = col - 'a';
    printf("row:%c\trown:%d\n", col, *coln);
    printf("%c", col + ('A' - 'a'));
    char tempcol[3];
    gets(tempcol);
    tempcol[2] = '\0';
    if (strlen(tempcol) < 2)
    {
        *row = tempcol[0] - '0' - 1;
        printf("col:%d\n", *row);
    }
    else
    {
        *row = (tempcol[0] - '0') * 10 + (tempcol[1] - '0') - 1;
        printf("col:%d\n", *row);
    }
    while (*coln < 0 || *coln > g->size - 1 || *row < 0 || *row > g->size - 1)
    {
        printf("rossz input\nrown:%d\ntempc:%s   coln:%d\nstrlen:%d\n", *coln, tempcol, *row, strlen(tempcol));
        col = _getch();
        if (col == '.')
        {
            menu(g, palette);
            //col = _getch();
        }
        col = tolower(col);
        *coln = col - 'a';
        printf("--row:%c\trown:%d\n", col, *coln);
        printf("%c", col + ('A' - 'a'));
        gets(tempcol);
        if (strlen(tempcol) < 2)
        {
            *row = tempcol[0] - '0' - 1;
            printf("col:%d\n", *row);
        }
        else
        {
            *row = (tempcol[0] - '0') * 10 + (tempcol[1] - '0') - 1;
            printf("col:%d\n", *row);
        }
    }
}
void ship_sunk(Maptile** map, Hajo* h, int size) {
    printf("BOOM\n");
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
                /*
                if (abs(target_x - g->s.cp2navy[i].b.x) == 0 && abs(target_y - (g->s.cp2navy[i].b.y + i)) == 0)
                {
                    g->s.cp2navy[i].intact_tiles--;
                    if (g->s.cp2navy[i].intact_tiles == 0)
                    {
                        ship_sunk(g, g->s.cp2navy[i]);
                    }
                }
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
                }*/
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
                /*
                if (abs(target_x - g->s.cp2navy[i].b.x + i) == 0 && abs(target_y - (g->s.cp2navy[i].b.y)) == 0)
                {
                    g->s.cp2navy[i].intact_tiles--;
                }
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
                }*/
            }
        }
    }

}
bool can_shoot_left(int this_x, int this_y, Maptile** map) {
    printf("1.\n");
    if (this_x != 0)      //nincs a bal oldalon ez a mező
    {
        if (map[this_x - 1][this_y].hit == false) return true;
        printf("map[%d,%d]-false L\n", this_x - 1, this_y);
    }
    return false;
}
bool can_shoot_up(int this_x, int this_y, Maptile** map) {
    printf("2.\n");
    if (this_y != 0)      //nincs a felső oldalon ez a mező
    {
        if (map[this_x][this_y - 1].hit == false) return true;
        printf("map[%d,%d]-false U\n", this_x, this_y - 1);
    }
    return false;
}
bool can_shoot_right(int this_x, int this_y, Maptile** map, int size) {     //size = pályaméret
    printf("3.\n");
    if (this_x != size - 1)      //nincs a jobb oldalon ez a mező
    {
        if (map[this_x + 1][this_y].hit == false) return true;
        printf("map[%d,%d]-false R\n", this_x + 1, this_y);
    }
    return false;
}
bool can_shoot_down(int this_x, int this_y, Maptile** map, int size) {
    printf("4.\n");
    if (this_x != size - 1)      //nincs az alsó oldalon ez a mező
    {
        if (map[this_x][this_y + 1].hit == false) return true;
        printf("map[%d,%d]-false D\n", this_x, this_y + 1);
    }
    return false;
}
void reset_cpfire(int* first_hit_col, int* first_hit_row, int* damaged_ship, int* align) {      //ez csak azért van, mert többször is kell
    *first_hit_col = -1;
    *first_hit_row = -1;
    *damaged_ship = -1;
    *align = 0;
}
void cpfire(Game* g, int* p1intact, int* cp2intact, int* p1miss, int* cp2miss, bool* if_hit, int* damaged_ship, int* last_col, int* last_row, int* first_hit_col, int* first_hit_row, int* align) {
    int diffy = 0;
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
    //printf("last:(%d,%d) first:(%d,%d) diffy:%d damaged_ship:%d if_hit:%d align:%d\n", *last_col, *last_row, *first_hit_col, *first_hit_row, diffy, *damaged_ship, *if_hit, *align);
    if (*damaged_ship == -1)
    {
        if (rand() % diffy == 0) *if_hit = true;
        else *if_hit = false;
        if (*if_hit)
        {
            printf("true");
            *damaged_ship = rand() % set_navy_size(g->size);
            while (g->s.p1navy[*damaged_ship].intact_tiles==0)         //ez a hajó már korábban elsüllyedt
            {
                *damaged_ship = rand() % set_navy_size(g->size);
            }
            //printf(":%d, ds:%d", set_navy_size(g->size), *damaged_ship);
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
                    printf("reset----\n");
                    ship_sunk(g->s.p1, &g->s.p1navy[*damaged_ship], g->size);
                    reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
                }
            }
            else                                              //már meglőtték a mezőt
            {
                reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
                cpfire(g, p1intact, cp2intact, p1miss, cp2miss, if_hit, damaged_ship, last_col, last_row, first_hit_col, first_hit_row, align);      //másik mezőt keres
            }
        }
        else if (p1miss != 0)
        {
             
            printf("false\n");
            int misscol = rand() % g->size;
            int missrow = rand() % g->size;
            while (g->s.p1[misscol][missrow].hit == true || g->s.p1[misscol][missrow].value != '2')        //addig keres mezőt ameddig nem talál egy olyat amin víz van és még nem lőtték meg
            {
                //printf("[%d,%d] hit:%d\n", misscol, missrow, g->s.p1[misscol][missrow].hit);
                misscol = rand() % g->size;
                missrow = rand() % g->size;
            }
            g->s.p1[misscol][missrow].hit = true;
            *if_hit = false;
            --*p1miss;
            printf("miss[%d,%d]\n",misscol, missrow);
        }
        else        //nincs több víz mező amit meg lehet lőni
        {
            *if_hit = true;
            cpfire(g, p1intact, cp2intact, p1miss, cp2miss, if_hit, damaged_ship, last_col, last_row, first_hit_col, first_hit_row, align);      //másik mezőt keres
        }
    }
    else                                            /////////ezt meg kéne tervezni papíron
    {
        //printf("nem -1\n");
        //if (*first_hit_col == *last_col && *first_hit_row == *last_row)   //kezdőpontnál vagyunk
        {
            if (*align == 0)        //még nem tudjuk hogy milyen állású a hajó, ilyenkor biztos hogy a kezdőpontnál vagyunk
            {
                *first_hit_col = *last_col;
                *first_hit_row = *last_row;
                if (can_shoot_left(*last_col, *last_row, g->s.p1))
                {
                    //printf("...");
                        g->s.p1[*last_col - 1][*last_row].hit = true;
                    if (g->s.p1[*last_col - 1][*last_row].value == '2')
                    {
                        *if_hit = false;
                        --* p1miss;
                    }
                    else                    //hajó van a mezőn
                    {
                        *if_hit = true;
                        --*last_col;        //áthelyezi az utolsó találat koordinátáit
                        g->s.p1navy[*damaged_ship].intact_tiles--;      //mivel, hogy nem lehetnek egymás mellett hajók így tudjuk, hogy ez csak ugyanaz a hajó lehet
                        *align = 2;         //a hajó biztos, hogy vízszintes
                        if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                        {
                            printf("reset----\n");
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
                        --* p1miss;
                    }
                    else
                    {
                        *if_hit = true;
                        --*last_row;        //áthelyezi az utolsó találat koordinátáit
                        g->s.p1navy[*damaged_ship].intact_tiles--;
                        *align = 1;         //a hajó biztos, hogy függőleges
                        if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                        {
                            printf("reset----\n");
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
                        --* p1miss;
                    }
                    else
                    {
                        *if_hit = true;
                        ++*last_col;        //áthelyezi az utolsó találat koordinátáit
                        g->s.p1navy[*damaged_ship].intact_tiles--;
                        *align = 2;
                        if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                        {
                            printf("reset----\n");
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
                        --* p1miss;
                    }
                    else
                    {
                        *if_hit = true;
                        ++*last_row;        //áthelyezi az utolsó találat koordinátáit
                        g->s.p1navy[*damaged_ship].intact_tiles--;
                        *align = 1;
                        if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                        {
                            printf("reset----\n");
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
                        --* p1miss;
                    }
                    else
                    {
                        *if_hit = true;
                        --*last_row;        //áthelyezi az utolsó találat koordinátáit
                        g->s.p1navy[*damaged_ship].intact_tiles--;
                        //*align = 1;         //a hajó biztos, hogy függőleges
                        if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                        {
                            printf("reset----\n");
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
                        --* p1miss;
                    }
                    else
                    {
                        *if_hit = true;
                        ++*last_row;        //áthelyezi az utolsó találat koordinátáit
                        g->s.p1navy[*damaged_ship].intact_tiles--;
                        if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                        {
                            printf("reset----\n");
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
                        --* p1miss;
                        
                    }
                    else                    //hajó van a mezőn
                    {
                        *if_hit = true;
                        --*last_col;        //áthelyezi az utolsó találat koordinátáit
                        g->s.p1navy[*damaged_ship].intact_tiles--;      //mivel, hogy nem lehetnek egymás mellett hajók így tudjuk, hogy ez csak ugyanaz a hajó lehet
                        if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                        {
                            printf("reset----\n");
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
                        --* p1miss;
                    }
                    else
                    {
                        *if_hit = true;
                        ++*last_col;        //áthelyezi az utolsó találat koordinátáit
                        g->s.p1navy[*damaged_ship].intact_tiles--;
                        if (g->s.p1navy[*damaged_ship].intact_tiles == 0)
                        {
                            printf("reset----\n");
                            ship_sunk(g->s.p1, &g->s.p1navy[*damaged_ship], g->size);
                            reset_cpfire(first_hit_col, first_hit_row, damaged_ship, align);    //a hajó elsüllyedt
                            printf("alien\n");
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
        }
    }
}
void fire(Game* g, bool* if_hit, int pov, int* palette) {
    int target_x, target_y;
    get_target(g, &target_x, &target_y, palette);
    if (pov == 1)
    {
            g->s.cp2[target_x][target_y].hit = true;
        if (g->s.cp2[target_x][target_y].value=='1')      //találat
        {
            *if_hit = true;
            printf("if_hit:true\n");
            /*if (g->s.p1navy[get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, target_x, target_y)].intact_tiles == 0)
            {
                printf("kKkKkKakukKkKkK\n");
            }
            printf("kKkKkKakukKkKkK %d\n", g->s.cp2navy[get_ship_by_coords(g->s.cp2, g->s.cp2navy, g->size, target_x, target_y)].intact_tiles);*/
            g->s.cp2navy[get_ship_by_coords(g->s.cp2, g->s.cp2navy, g->size, target_x, target_y)].intact_tiles--;

            if (g->s.cp2navy[get_ship_by_coords(g->s.cp2, g->s.cp2navy, g->size, target_x, target_y)].intact_tiles == 0)
            {
                ship_sunk(g->s.cp2, &g->s.cp2navy[get_ship_by_coords(g->s.cp2, g->s.cp2navy, g->size, target_x, target_y)], g->size);
            }
        }
        else
        {
            *if_hit = false;
            printf("if_hit:false1\n");
        }
        

    }
    else
    {
        g->s.p1[target_x][target_y].hit = true;
        if (g->s.p1[target_x][target_y].value == '1')      //találat
        {
            *if_hit = true;
            printf("if_hit:true\n");
            /*if (g->s.p1navy[get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, target_x, target_y)].intact_tiles == 0)
            {
                printf("kKkKkKakukKkKkK\n");
            }
            printf("kKkKkKakukKkKkK %d\n", g->s.cp2navy[get_ship_by_coords(g->s.cp2, g->s.cp2navy, g->size, target_x, target_y)].intact_tiles);*/
            g->s.p1navy[get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, target_x, target_y)].intact_tiles--;

            if (g->s.p1navy[get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, target_x, target_y)].intact_tiles == 0)
            {
                ship_sunk(g->s.p1, &g->s.p1navy[get_ship_by_coords(g->s.p1, g->s.p1navy, g->size, target_x, target_y)], g->size);
            }
        }
        else
        {
            *if_hit = false;
            printf("if_hit:false2\n");
        }
    }
}
void salvo(Game* g, int* p1intact, int* cp2intact, int* p1miss, int* cp2miss, int pov, bool* if_hit, int* damaged_ship, int* last_col, int* last_row, int* first_hit_col, int* first_hit_row, int* align, int* palette) {
    printf("pov%d\n", pov);
    if (pov==1)
    {
        system("cls");
        show_pov(g, pov, palette);         //1:játékos oldala
        fire(g, if_hit, pov, palette);
        Sleep(2);
        while (*if_hit)
        {
            system("cls");
            --*cp2intact;
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
            cpfire(g, p1intact, cp2intact, p1miss, cp2miss, if_hit, damaged_ship,  last_col, last_row, first_hit_col, first_hit_row, align);  //a gép mindig ugyanazon az oldalon van
            while (*if_hit)
            {
                system("cls");
                --*p1intact;
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
                --*p1intact;
                show_pov(g, pov, palette);
                if (*p1intact >= 0) fire(g, if_hit, pov, palette);
                else *if_hit = false;
            }
            system("cls");
            show_pov(g, pov, palette);
        }
    }
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
    int p1intact = 0, cp2intact = 0;//sértetlen hajómezők száma
    int p1miss = 0, cp2miss = 0;    //vízmezők száma
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
    int last_col;                    //elző találat koordinátái
    int last_row;
    int tagged_col = -1;             //előző előtti találat koordinátái
    int tagged_row = -1;
    int target_ship_align = 0;       //0:ismeretlen 1:függőleges 2:vízszintes
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
    int p1intact = 0, cp2intact = 0;//sértetlen hajómezők száma
    int p1miss = 0, cp2miss = 0;    //vízmezők száma
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
    int last_col;                    //elző találat koordinátái
    int last_row;
    int tagged_col = -1;             //előző előtti találat koordinátái
    int tagged_row = -1;
    int target_ship_align = 0;       //0:ismeretlen 1:függőleges 2:vízszintes
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
    else if (gamemode=='c')
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
