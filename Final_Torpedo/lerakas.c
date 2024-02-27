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

bool placable(int rown, int col, char align, int index, int map_size, Hajo* new_navy, int navy_size) {  //meg kell nézni hogy az egész hajó rajta lenne e a pályán (1) és hogy nincs e túl közel egy másik hajóhoz (2)
    Hajo ship;
    ship.b.x = col;
    ship.b.y = rown;       //mert egy nagy negatív szám és nem találom hogy miért
    ship.intact_tiles = index + 1;        //mert carriernél 4 lenne
    if (align == 'h')
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

    ////////////////////////////////////
    //printf("mukodik mert csodas\n");//
    ////////////////////////////////////

    return true;
}
void get_ship_coords(char* coln, int* row, char* align, int index, int map_size) {
    printf("Where do you want to place a ship with a length of %d?\n", index + 1);
    char col = tolower(_getch());
    *coln = col - 'a';
    //printf("row:%c\trown:%d\n", col,*coln);
    printf("%c", col + ('A' - 'a'));
    char tempcol[3];
    gets(tempcol);
    tempcol[2] = '\0';
    if (strlen(tempcol) < 2)
    {
        *row = tempcol[0] - '0' - 1;
        //printf("col:%d\n",*row);
    }
    else
    {
        *row = (tempcol[0] - '0') * 10 + (tempcol[1] - '0') - 1;
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
void place_ship(int* navy, int index, Hajo* new_navy, int map_size, int navy_size) {
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
    while (!placable(row, coln, alignc, index, map_size, new_navy, navy_size))
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
    new_navy[navy_size - 1] = cp_ship;
    /*
    for (int i = 0; i < navy_size; i++)
    {
        printf("?:[%d][%d]-[%d][%d] i[:%d]+++++++++\n", new_navy[i].b.x, new_navy[i].b.y, new_navy[i].e.x, new_navy[i].e.y, i);
    }*/
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