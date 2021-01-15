// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.

#include "graphics.h"
#include "globals.h"
#include "decor.h"

extern char ROCKWALL[121];
extern char LIFE_TREE[121];

void draw_nothing(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}
/*
void draw_ntg(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+1, v+1, BLACK);
}    
*/
void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = YELLOW;      
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = DIRT;         
        else if (img[i] == '5') colors[i] = LGREY;       // 50% grey
        else if (img[i] == '3') colors[i] = DGREY;
        else if (img[i] == 'W') colors[i] = WHITE;       
        else if (img[i] == '8') colors[i] = BLUE;        
        else if (img[i] == '7') colors[i] = LGREEN; 
        else if (img[i] == '4') colors[i] = BROWNISH;  
        else if (img[i] == '9') colors[i] = LBLUE;   
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_tree(int u, int v)                             //Fixed this
{
    draw_img(u,v,&LIFE_TREE[0]);   // (const char *)
}
void draw_wall(int u, int v)
{
    draw_img(u,v,&ROCKWALL[0]);//uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_plant(int u, int v)                             //Fixed this
{
    uLCD.filled_circle(u+5, v+5, 4, DGREEN);
}

void draw_goodie(int u, int v)                             //Fixed this
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BROWNISH);        //uLCD.triangle(u+3, v+1, u+1, v+3, u+3, v+5, RED);
    uLCD.filled_rectangle(u+1, v+1, u+1, v+3, YELLOW);
    uLCD.filled_rectangle(u+1, v+5, u+1, v+9, YELLOW);
    uLCD.filled_rectangle(u+4, v+5, u+7, v+5, YELLOW);
    uLCD.filled_rectangle(u+9, v+3, u+9, v+7, YELLOW);
    uLCD.filled_rectangle(u+1, v+9, u+3, v+9, YELLOW);
}

void draw_tile1(int u, int v)
{
    draw_img(u,v,&GM[0]);
}
void draw_tile2(int u, int v)
{
    draw_img(u,v,&IBM[0]);
}
void draw_tile3(int u, int v)
{
    draw_img(u,v,&TRUMP[0]);
}
void draw_tile4(int u, int v)
{
    draw_img(u,v,&PENCE[0]);
}
void draw_tile5(int u, int v)
{
    draw_img(u,v,&POMPEO[0]);
}
void draw_tile6(int u, int v)
{
    draw_img(u,v,&MS[0]);
}

void draw_snake_body(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}
void draw_snake_head(int u, int v)
{  
    uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);     
    uLCD.filled_rectangle(u+1, v+1, u+3,  v+3, RED);
    uLCD.filled_rectangle(u+7, v+1, u+9,  v+3, RED); 
    uLCD.filled_rectangle(u+1, v+7, u+3,  v+9, RED);
    uLCD.filled_rectangle(u+7, v+7, u+9,  v+9, RED);
}
void draw_snake_tail(int u, int v)
{   
    uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);     
    uLCD.filled_rectangle(u+1, v+1, u+3,  v+3, YELLOW);
    uLCD.filled_rectangle(u+7, v+1, u+9,  v+3, YELLOW); 
    uLCD.filled_rectangle(u+1, v+7, u+3,  v+9, YELLOW);
    uLCD.filled_rectangle(u+7, v+7, u+9,  v+9, YELLOW);
}
 
void draw_snake_turn(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}    