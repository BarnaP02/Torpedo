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

bool placable(int rown, int col, char align, int index, int map_size, Hajo* new_navy, int navy_size) {  //meg kell nézni hogy az egész hajó rajta lenne e a pályán (1) és hogy nincs e túl közel egy másik hajóhoz (2)
    Hajo ship;
    ship.b.x = col;
    ship.b.y = rown;       //mert egy nagy negatív szám és nem találom hogy miért
    ship.intact_tiles = index + 1;        //mert carriernél 4 lenne
    if (align == 'v')
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
                if (ship.b.x == ship.e.x) //ha függőleges
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
void get_ship_coords(char* coln, int* row, char* align, int index, int map_size) {
    printf("Hova keruljon egy %d hosszu hajo?\n", index + 1);
    char col = tolower(_getch());
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
void place_ship(int* navy, int index, Hajo* new_navy, int map_size, int navy_size) {
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
    while (!placable(row, coln, alignc, index, map_size, new_navy, navy_size))
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
    new_navy[navy_size - 1] = cp_ship;
    for (int i = 0; i < navy_size; i++)
    {
        printf("?:[%d][%d]-[%d][%d] i[:%d]+++++++++\n", new_navy[i].b.x, new_navy[i].b.y, new_navy[i].e.x, new_navy[i].e.y, i);
    }
}
void show_pov(Game* g, int pov, int* palette) {        // welcome to the jungle
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
                    else if (i == g->size - 1 && j == 0) // jobb felső sarok
                    {
                        if (g->s.p1[i - 1][j].value == '1') printf("%c%c", khajo, khajo); //balra lévő mező
                        else if (g->s.p1[i][j + 1].value == '1') printf(" %c", ahajo); //alatta lévő mező
                        else printf(" %c", ahajo);
                    }
                    else if (i == g->size - 1 && j == g->size - 1) // jobb alsó sarok
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
            while (g->s.p1navy[*damaged_ship].intact_tiles == 0)         //ez a hajó már korábban elsüllyedt
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
            if (g->s.p1[*last_col][*last_row].hit == false)     //találat
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
            --* p1miss;
            printf("miss[%d,%d]\n", misscol, missrow);
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
                        --* last_col;        //áthelyezi az utolsó találat koordinátáit
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
                        --* last_row;        //áthelyezi az utolsó találat koordinátáit
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
                        ++* last_col;        //áthelyezi az utolsó találat koordinátáit
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
                        ++* last_row;        //áthelyezi az utolsó találat koordinátáit
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
                        --* last_row;        //áthelyezi az utolsó találat koordinátáit
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
                        ++* last_row;        //áthelyezi az utolsó találat koordinátáit
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
                        --* last_col;        //áthelyezi az utolsó találat koordinátáit
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
                        ++* last_col;        //áthelyezi az utolsó találat koordinátáit
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
        if (g->s.cp2[target_x][target_y].value == '1')      //találat
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
