//=================================================================
// The main program file.
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "snake.h"
#include "speech.h" 
#include "Speaker.h"  
#include <math.h>
#include<stdio.h>

#define CITY_HIT_MARGIN 1
#define CITY_UPPER_BOUND (SIZE_Y-(LANDSCAPE_HEIGHT+MAX_BUILDING_HEIGHT))

#define NO_ACTION 0
#define NO_RESULT 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define GAME_OVER 7
#define FULL_DRAW 8
#define HIT_WALL 9
#define FOUND_GOODIE 10
#define LIFE 11
#define SPEED_UP 12
#define PAUSE 13
#define UNPAUSE 14
#define INVINCI 15
// Helper function declarations
Speaker mySpeaker(p26);
void playSound(char* wav);
//void draw_game(int init);

int indexPushButton = 0;
int numberBody = 1 ;
static char isGoodie = 0;
MapItem* Head;

/**
 * The main game state. Must include snake locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;      //current location
    int px, py;   //previous location
    
} camera;    
/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
Snake snake;

/**
 * Draw the border for the map.
 */
void draw_border()
{
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
}

/**
 * Draw the upper status bar.
 */
void draw_upper_status()
{
    char texte[64];
    uLCD.line(0, 9, 127, 9, GREEN);
    uLCD.locate(0,0);
    uLCD.color(RED);
    uLCD.textbackground_color(BLACK);
    memset(texte, 0 , sizeof(texte));
    sprintf(texte,"Pos:%d,%d  Sc:%d ",snake.head_x, snake.head_y, snake.score);
    //uLCD.printf("Pos:%d %d Sc:%d",snake.head_x, snake.head_y, snake.score);
    uLCD.printf("%s",texte);
   
}

/**
 * Draw the lower status bar.
 */
void draw_lower_status()
{
    //char texte[64];
    uLCD.line(0, 118, 127, 118, GREEN);
    uLCD.locate(0,15);
    uLCD.textbackground_color(BLACK);
    uLCD.color(RED);
    //memset(texte, 0 , sizeof(texte));
    //sprintf(texte, "Life:%d Length:%d",snake.life, snake.length);
    uLCD.printf("Life:%d Length:%d",snake.life, snake.length);
    //uLCD.printf("%s",texte);
    
}

volatile int prev_pos = SNAKE_RIGHT;



// Function prototypes


void snake_right()
{
    snake.pos = SNAKE_RIGHT;
    snake.previous_x = snake.head_x;
    snake.previous_y = snake.head_y;
    
    add_nothing(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
    add_nothing(snake.locations[0].x, snake.locations[0].y);
    for(int i=1; i<(snake.length-1); i++)
    {
       add_nothing(snake.locations[i].x, snake.locations[i].y);
    }
    
    for(int i=1; i<(snake.length); i++)
    {
       snake.locations[snake.length-i].x =  snake.locations[snake.length-i-1].x;
       snake.locations[snake.length-i].y =  snake.locations[snake.length-i-1].y;
    }
    
    add_snake_tail(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
    for(int i=1; i<(snake.length-1); i++)
    {
       add_snake_body(snake.locations[i].x, snake.locations[i].y);
    }
    
    snake.head_x += 1;
    snake.head_y += 0; 
    snake.locations[0].x = snake.head_x;
    snake.locations[0].y = snake.head_y;
    
    
    add_snake_head(snake.locations[0].x, snake.locations[0].y);
    
}    
void snake_left()
{  
    snake.pos = SNAKE_LEFT;
    snake.previous_x = snake.head_x;
    snake.previous_y = snake.head_y;
    
    add_nothing(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
    add_nothing(snake.locations[0].x, snake.locations[0].y);
    
    for(int i=1; i<(snake.length-1); i++)
    {
       add_nothing(snake.locations[i].x, snake.locations[i].y);
    }
    
    for(int i=1; i<(snake.length); i++)
    {
       snake.locations[snake.length-i].x =  snake.locations[snake.length-i-1].x;
       snake.locations[snake.length-i].y =  snake.locations[snake.length-i-1].y;
    }
    
    add_snake_tail(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
    for(int i=1; i<(snake.length-1); i++)
    {
       add_snake_body(snake.locations[i].x, snake.locations[i].y);
    }
    
    snake.head_x -= 1;
    snake.head_y += 0; 
    snake.locations[0].x = snake.head_x;
    snake.locations[0].y = snake.head_y;
    add_snake_head(snake.locations[0].x, snake.locations[0].y);
    
} 

void snake_up()
{
    snake.pos = SNAKE_UP;
    snake.previous_x = snake.head_x;
    snake.previous_y = snake.head_y;
    
    add_nothing(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
    add_nothing(snake.locations[0].x, snake.locations[0].y);
    
    for(int i=1; i<(snake.length-1); i++)
    {
       add_nothing(snake.locations[i].x, snake.locations[i].y);
    }
    
    for(int i=1; i<(snake.length); i++)
    {
       snake.locations[snake.length-i].x =  snake.locations[snake.length-i-1].x;
       snake.locations[snake.length-i].y =  snake.locations[snake.length-i-1].y;
    }
    
    add_snake_tail(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
    for(int i=1; i<(snake.length-1); i++)
    {
       add_snake_body(snake.locations[i].x, snake.locations[i].y);
    }
    
    snake.head_x += 0;
    snake.head_y -= 1; 
    snake.locations[0].x = snake.head_x;
    snake.locations[0].y = snake.head_y;
     
    add_snake_head(snake.locations[0].x, snake.locations[0].y);  
} 
void snake_down()
{  
    snake.pos = SNAKE_DOWN;
    snake.previous_x = snake.head_x;
    snake.previous_y = snake.head_y;
    
    add_nothing(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
    add_nothing(snake.locations[0].x, snake.locations[0].y);
    
    for(int i=1; i<(snake.length-1); i++)
    {
       add_nothing(snake.locations[i].x, snake.locations[i].y);
    }
    
    for(int i=1; i<(snake.length); i++)
    {
       snake.locations[snake.length-i].x =  snake.locations[snake.length-i-1].x;
       snake.locations[snake.length-i].y =  snake.locations[snake.length-i-1].y;
    }
    
    add_snake_tail(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
    for(int i=1; i<(snake.length-1); i++)
    {
       add_snake_body(snake.locations[i].x, snake.locations[i].y);
    }

    snake.head_x += 0;
    snake.head_y += 1; 
    snake.locations[0].x = snake.head_x;
    snake.locations[0].y = snake.head_y;
       
    add_snake_head(snake.locations[0].x, snake.locations[0].y);    
}          
    

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 */

// Get Actions from User (push buttons & accelerometer)
// Based on push button and accelerometer inputs, determine which action
// needs to be performed (may be no action).
int get_action(GameInputs inputs)
{
    if(inputs.ay < -0.5)
    {
        return GO_DOWN; 
    }   
    if(inputs.ay > 0.5)
    {
        return GO_UP;
    }
    if(inputs.ax < -0.5)
    {
        return GO_LEFT;
    }
    if(inputs.ax > 0.5)
    {
        return GO_RIGHT;
    }           
    
    //Pause the game                     
    if(inputs.b3)
    {
        while(inputs.b3)
        {
            GameInputs in= read_inputs();
            inputs.b3 = in.b3;
        }      
            return PAUSE;   
    }  
    
    //Activate the button of invincibility
    if(inputs.b2)
    {
        return INVINCI;
    }                                                     
                                                         
    return NO_ACTION;
}
/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the snake position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the snake has not moved.
 */
int update_game(int action)
{
    
    snake.head_px = snake.head_x;
    snake.head_py = snake.head_y;
    snake.body_px = snake.body_x;
    snake.body_py = snake.body_y;
    snake.tail_px = snake.tail_x;
    snake.tail_py = snake.tail_y;
    
switch(action)
{
        case GO_UP:
             Head = get_north(snake.head_x, snake.head_y);
           
            if(snake.pos != SNAKE_DOWN)
            {            
                     //Increase its length when eats a GOODIE
                    if(Head->type == GOODIE)
                    {
                        numberBody++;
                        remove_goodie(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                        add_nothing(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                            add_nothing(snake.locations[i].x, snake.locations[i].y);
                        }
            
                        snake.length++; 
                        snake.score++;  
                        
                        for(int i=1; i<(snake.length); i++)
                        {
                           snake.locations[snake.length-i].x =  snake.locations[snake.length-i-1].x;
                           snake.locations[snake.length-i].y =  snake.locations[snake.length-i-1].y+1;
                        }
                        
                        add_snake_tail(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                           add_snake_body(snake.locations[i].x, snake.locations[i].y);
                        } 
                        snake.locations[0].x = snake.head_x;
                        snake.locations[0].y = snake.head_y;
                        add_snake_head(snake.locations[0].x, snake.locations[0].y);
                        mySpeaker.PlayNote(200.0,0.25,0.005);       
                    }
                      
                    //Buffs
                    if (Head->type == IBM_t)   //This buff increases the snake's life
                     {
                        snake.life++;
                        map_erase(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                     } 
                    if(Head->type == GM_t)    //This buff increases the score but not the length
                     {
                        snake.score +=5;
                        map_erase(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                     } 
                    if (Head->type == MS_t )      //This buff  slows down the snake
                     {
                            if(snake.slow == 0)
                             {
                                   snake.head_y--;
                                   map_erase(snake.head_x, snake.head_y);
                                   add_nothing(snake.head_x, snake.head_y);
                             } 
                             wait(1.0); 
                       }
                       
                     //Debuffs        
                    if(Head->type == POMPEO_t)   //This debuff cut down its lives
                    {
                          snake.life -=1; 
                    }               
                    if(Head->type == TRUMP_t)   //This debuff increases length but not the score
                    {
                          numberBody++;
                        remove_goodie(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                        add_nothing(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                            add_nothing(snake.locations[i].x, snake.locations[i].y);
                        }
            
                        snake.length++;  
                        
                        for(int i=1; i<(snake.length); i++)
                        {
                           snake.locations[snake.length-i].x =  snake.locations[snake.length-i-1].x;
                           snake.locations[snake.length-i].y =  snake.locations[snake.length-i-1].y+1;
                        }
                        
                        add_snake_tail(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                           add_snake_body(snake.locations[i].x, snake.locations[i].y);
                        } 
                        snake.locations[0].x = snake.head_x;
                        snake.locations[0].y = snake.head_y;
                        add_snake_head(snake.locations[0].x, snake.locations[0].y);       
                    }
                    
                    if (Head->type == PENCE_t)  //This debuff increases the speed of the snake by 5
                    {
                         snake.head_y -=5;   
                         map_erase(snake.head_x, snake.head_y);
                         add_nothing(snake.head_x, snake.head_y);                 
                    }  
                    
                     //move the snake up  
                    if(Head->walkable)
                    {
                         snake_up();
                    }   
                      
                     //Game Over when it hits a non walkable object(the wall or its body)
                    if((Head->type == ROCKWALL) || (Head->type == SNAKE_BODY))   
                    {
                       snake.life -=1;
                    }  
                    //Game Over if Snake reaches max value 
                    if(snake.length == 18)
                    {
                         snake.complete = 0;          
                    } 
           }      
        break;
        
        case GO_DOWN:
             Head = get_south(snake.head_x, snake.head_y);
                 
            if(snake.pos != SNAKE_UP)
            {       
                      //Increase its lenght when eats a GOODIE
                    if(Head->type == GOODIE)
                    {
                        numberBody++;
                        remove_goodie(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                        add_nothing(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                            add_nothing(snake.locations[i].x, snake.locations[i].y);
                        }
            
                        snake.length++; 
                        snake.score++;  
                        
                        for(int i=1; i<(snake.length); i++)
                        {
                           snake.locations[snake.length-i].x =  snake.locations[snake.length-i-1].x;
                           snake.locations[snake.length-i].y =  snake.locations[snake.length-i-1].y-1;
                        }
                        
                        add_snake_tail(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                           add_snake_body(snake.locations[i].x, snake.locations[i].y);
                        } 
                        snake.locations[0].x = snake.head_x;
                        snake.locations[0].y = snake.head_y;
                        add_snake_head(snake.locations[0].x, snake.locations[0].y);  
                        mySpeaker.PlayNote(200.0,0.25,0.005);      
                     }
                      
                     //Buffs
                     if (Head->type == IBM_t)   //This buff increases the snake's life
                     {
                        snake.life++;
                        map_erase(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                     } 
                     if(Head->type == GM_t)    //This buff increases the score but not the length
                     {
                        snake.score +=5;
                        map_erase(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                     } 
                     if (Head->type == MS_t )      //This buff slows down the snake
                     {
                         if(snake.slow == 0)
                             {
                                   snake.head_y++;
                                   map_erase(snake.head_x, snake.head_y);
                                   add_nothing(snake.head_x, snake.head_y);
                             } 
                             wait(1.0);   
                     }
                      
                     //Debuffs        
                     if(Head->type == POMPEO_t)   //This debuff cut down its lives
                     {
                          snake.life -=1; 
                     }               
                     if(Head->type == TRUMP_t)   //This debuff increases length but not the score
                     {
                         numberBody++;
                        remove_goodie(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                        add_nothing(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                            add_nothing(snake.locations[i].x, snake.locations[i].y);
                        }
            
                        snake.length++;  
                        
                        for(int i=1; i<(snake.length); i++)
                        {
                           snake.locations[snake.length-i].x =  snake.locations[snake.length-i-1].x;
                           snake.locations[snake.length-i].y =  snake.locations[snake.length-i-1].y-1;
                        }
                        
                        add_snake_tail(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                           add_snake_body(snake.locations[i].x, snake.locations[i].y);
                        } 
                        snake.locations[0].x = snake.head_x;
                        snake.locations[0].y = snake.head_y;
                        add_snake_head(snake.locations[0].x, snake.locations[0].y);       
                     }    
                     if(Head->type == PENCE_t)  //This debuff increases the speed of the snake by 5
                     {
                         snake.head_y +=5;
                         map_erase(snake.head_x, snake.head_y);
                         add_nothing(snake.head_x, snake.head_y);
                     }
                      
                     //moves the snake down  
                     if(Head->walkable)
                     {
                         snake_down();
                     }
                     
                     //Game Over when it hits a non walkable object
                     if((Head->type == ROCKWALL) || (Head->type == SNAKE_BODY))   //!Head->walkable
                     {
                       snake.life -=1;
                     }
                     //Game Over if Snake reaches max value 
                     if(snake.length == 18)
                     {
                         snake.complete = 0;          
                     } 
           }                  
        break;
         
        case GO_RIGHT:
             Head = get_east(snake.head_x, snake.head_y);
           
           if(snake.pos != SNAKE_LEFT)
           {
                     //Increase its lenght when eats a GOODIE
                    if(Head->type == GOODIE)
                    {
                        numberBody++;
                        remove_goodie(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                        add_nothing(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                            add_nothing(snake.locations[i].x, snake.locations[i].y);
                        }
            
                        snake.length++; 
                        snake.score++;  
                        
                        for(int i=1; i<(snake.length); i++)
                        {
                           snake.locations[snake.length-i].x =  snake.locations[snake.length-i-1].x -1;
                           snake.locations[snake.length-i].y =  snake.locations[snake.length-i-1].y;
                        }
                        
                        add_snake_tail(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                           add_snake_body(snake.locations[i].x, snake.locations[i].y);
                        } 
                        snake.locations[0].x = snake.head_x;
                        snake.locations[0].y = snake.head_y;
                        add_snake_head(snake.locations[0].x, snake.locations[0].y);    
                        mySpeaker.PlayNote(200.0,0.25,0.005);    
                     }
                      
                     //Buffs
                     if (Head->type == IBM_t)   //This buff increases the snake's life
                     {
                        snake.life++;
                        map_erase(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                     } 
                     if(Head->type == GM_t)    //This buff increases the score but not the length
                     {
                        snake.score +=5;
                        map_erase(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                     }  
                     if (Head->type == MS_t )      //This buff slows down the snake
                     {
                         if(snake.slow == 0)
                             {
                                   snake.head_x++;
                                   map_erase(snake.head_x, snake.head_y);
                                   add_nothing(snake.head_x, snake.head_y);
                             } 
                             wait(1.0); 
                     }
                        
                     //Debuffs        
                     if(Head->type == POMPEO_t)   //This debuff cuts down its lives
                     {
                          snake.life -=1; 
                     }               
                     if(Head->type == TRUMP_t)   //This debuff increases length but not the score
                     {
                        numberBody++;
                        remove_goodie(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                        add_nothing(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                            add_nothing(snake.locations[i].x, snake.locations[i].y);
                        }
            
                        snake.length++;  
                        
                        for(int i=1; i<(snake.length); i++)
                        {
                           snake.locations[snake.length-i].x =  snake.locations[snake.length-i-1].x-1;
                           snake.locations[snake.length-i].y =  snake.locations[snake.length-i-1].y;
                        }
                        
                        add_snake_tail(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                           add_snake_body(snake.locations[i].x, snake.locations[i].y);
                        } 
                        snake.locations[0].x = snake.head_x;
                        snake.locations[0].y = snake.head_y;
                        add_snake_head(snake.locations[0].x, snake.locations[0].y);       
                     }  
                     if(Head->type == PENCE_t)  //This debuff increases the speed of the snake by 5
                     {
                         snake.head_x +=5;
                         map_erase(snake.head_x, snake.head_y);
                         add_nothing(snake.head_x, snake.head_y);
                     }              
                     
                     //moves the snake right
                     if(Head->walkable)
                     {
                         snake_right();
                     }
                     
                     //Game Over when it hits a non walkable object
                     if((Head->type == ROCKWALL) || (Head->type == SNAKE_BODY))   //!Head->walkable
                     {
                       snake.life -=1;
                     } 
                     //Game Over if Snake reaches max value 
                     if(snake.length == 18)
                     {
                         snake.complete = 0;          
                     }  
           }   
        break;
        
        case GO_LEFT:
             Head = get_west(snake.head_x, snake.head_y);
              
             if(snake.pos != SNAKE_RIGHT)
             {     
                    //Increase its lenght when eats a GOODIE
                    if(Head->type == GOODIE)
                    {
                        numberBody++;
                        remove_goodie(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                        add_nothing(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                            add_nothing(snake.locations[i].x, snake.locations[i].y);
                        }
            
                        snake.length++; 
                        snake.score++;  
                        
                        for(int i=1; i<(snake.length); i++)
                        {
                           snake.locations[snake.length-i].x =  snake.locations[snake.length-i-1].x+1;
                           snake.locations[snake.length-i].y =  snake.locations[snake.length-i-1].y;
                        }
                        
                        add_snake_tail(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                           add_snake_body(snake.locations[i].x, snake.locations[i].y);
                        } 
                        snake.locations[0].x = snake.head_x;
                        snake.locations[0].y = snake.head_y;
                        add_snake_head(snake.locations[0].x, snake.locations[0].y);  
                        mySpeaker.PlayNote(200.0,0.25,0.005);      
                     }
                      
                     //Buffs
                     if (Head->type == IBM_t)   //This buff increases the snake's life
                     {
                        snake.life++;
                        map_erase(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                     } 
                     if(Head->type == GM_t)    //This buff increases the score but not the length
                     {
                        snake.score +=5;
                        map_erase(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                     }
                     if (Head->type == MS_t )      //This buff slows down the snake
                     {
                         if(snake.slow == 0)
                             {
                                   snake.head_x--;
                                   map_erase(snake.head_x, snake.head_y);
                                   add_nothing(snake.head_x, snake.head_y);
                             } 
                             wait(1.0); 
                     }   
                    
                     //Debuffs        
                     if(Head->type == POMPEO_t)   //This debuff cut down its lives
                     {
                          snake.life -=1; 
                     }               
                     if(Head->type == TRUMP_t)   //This debuff increases length but not the score
                     {
                        numberBody++;
                        remove_goodie(snake.head_x, snake.head_y);
                        add_nothing(snake.head_x, snake.head_y);
                        add_nothing(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                            add_nothing(snake.locations[i].x, snake.locations[i].y);
                        }
            
                        snake.length++;  
                        
                        for(int i=1; i<(snake.length); i++)
                        {
                           snake.locations[snake.length-i].x =  snake.locations[snake.length-i-1].x+1;
                           snake.locations[snake.length-i].y =  snake.locations[snake.length-i-1].y;
                        }
                        
                        add_snake_tail(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
                        for(int i=1; i<(snake.length-1); i++)
                        {
                           add_snake_body(snake.locations[i].x, snake.locations[i].y);
                        } 
                        snake.locations[0].x = snake.head_x;
                        snake.locations[0].y = snake.head_y;
                        add_snake_head(snake.locations[0].x, snake.locations[0].y);       
                     }   
                     if(Head->type == PENCE_t)  //This debuff increases the speed of the snake by 5
                     {
                         snake.head_x -=5;
                         map_erase(snake.head_x, snake.head_y);
                         add_nothing(snake.head_x, snake.head_y);
                     }               
                     
                     //moves the snake left
                     if(Head->walkable) 
                     {
                         snake_left();
                     }
                     
                     //Game Over when it hits a non walkable object
                     if((Head->type == ROCKWALL) || (Head->type == SNAKE_BODY))   //!Head->walkable
                     {
                       snake.life -=1;
                     }
                     //Game Over if Snake reaches max value 
                     if(snake.length == 18)
                     {
                         snake.complete = 0;          
                     } 
           }  
        break;         
        
        
        case INVINCI:
             snake.life = snake.invinci - 1;              
             break;
        
        case PAUSE: 
             snake.pause = true;  
             
             uLCD.filled_rectangle(0,    0, 255, 255, BLACK);
             uLCD.filled_rectangle(0,    9, 127,  14, 0xFFFFFF);
             uLCD.filled_rectangle(0,   13,   2, 114, 0xFFFFFF);
             uLCD.filled_rectangle(0,  114, 127, 117, 0xFFFFFF);
             uLCD.filled_rectangle(124, 14, 127, 117, 0xFFFFFF);
             
             int u = 58;
             int v = 56;
             uLCD.filled_rectangle(0, 0, 255, 255, BLACK);
             uLCD.textbackground_color(BLACK);
             uLCD.color(RED);
             uLCD.locate(4,3);
             uLCD.printf("GAME PAUSED");
             uLCD.locate(1,6);
             uLCD.printf("ax >  0.5: RIGHT");
             uLCD.locate(1,7);
             uLCD.printf("ax < -0.5: LEFT");
             uLCD.locate(1,8);
             uLCD.printf("ay >  0.5: UP");
             uLCD.locate(1,9);
             uLCD.printf("ay < -0.5: DOWN");
             uLCD.locate(4,11);
             uLCD.printf("B3: Continue");
             uLCD.locate(4,12);
             uLCD.printf("B1: Exit");
             uLCD.color(WHITE);
             uLCD.locate(4,13);
             uLCD.printf("LoveSoldier");
             draw_upper_status();
             draw_lower_status();
             mySpeaker.PlayNote(400.0,0.25,0.005);
             mySpeaker.PlayNote(350.0,0.25,0.005);
             mySpeaker.PlayNote(300.0,0.25,0.005);
             mySpeaker.PlayNote(250.0,0.25,0.005);
             mySpeaker.PlayNote(200.0,0.25,0.005); 
             mySpeaker.PlayNote(150.0,0.25,0.005);
             mySpeaker.PlayNote(100.0,0.25,0.005);
             
             while(1)
             {
                 GameInputs in= read_inputs();
                 if(in.b3) break;
                 if(in.b1)
                 {
                     snake.exit = 0;
                     break;
                 }    
             }  
             
             mySpeaker.PlayNote(200.0,0.25,0.005); 
             mySpeaker.PlayNote(150.0,0.25,0.005);
             mySpeaker.PlayNote(100.0,0.25,0.005);
             
                 snake.pause = false;        
                 uLCD.filled_rectangle(0,    9, 127,  14, 0xFFFFFF);
                 uLCD.filled_rectangle(0,   13,   2, 114, 0xFFFFFF);
                 uLCD.filled_rectangle(0,  114, 127, 117, 0xFFFFFF);
                 uLCD.filled_rectangle(124, 14, 127, 117, 0xFFFFFF);
                 uLCD.filled_rectangle(0,    0, 255, 255, BLACK);
                 
                  break;
                  
             default: 
                break;     
             
    } 
    return NO_RESULT;
}
 
 
/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int draw_option)   //replaced draw_option by init
{
   
    // Draw game border first
    if(draw_option == FULL_DRAW)         //(draw_option == FULL_DRAW)
    
    {
        draw_border();
        int u = 58;
        int v = 59; //59 56
           
        snake.pos = prev_pos = SNAKE_RIGHT;
        snake.locations[0].x = snake.head_pu = u;
        snake.locations[0].y = snake.head_pv = v;
        
        snake.locations[1].x = snake.body_pu = u-11;
        snake.locations[1].y = snake.body_pv = v;
        
        snake.locations[2].x = snake.tail_pu = u-22;
        snake.locations[2].y = snake.tail_pv = v;
                
        draw_snake_head(snake.locations[0].x, snake.locations[0].y);
        draw_snake_body(snake.locations[1].x, snake.locations[1].y);
        draw_snake_tail(snake.locations[2].x, snake.locations[2].y);
                
        /*
        int u = 58;
        int v = 56;
        
        snake.pos = SNAKE_UP;
        snake.head_pu = u;
        snake.head_pv = v;
        
        snake.body_pu = u-11;
        snake.body_pv = v;
        
        snake.tail_pu = u-22;
        snake.tail_pv = v;
        
        
        draw_snake_head(u, v);
        draw_snake_body(u-11, v);
        draw_snake_tail(u-22, v);
        */
        
       
        return;
    }
       
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + snake.head_x;
            int y = j + snake.head_y;

            // Compute the previous map (px, py) of this tile
            int px = i + snake.head_px;
            int py = j + snake.head_py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map       
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                
                if (draw_option || curr_item != prev_item) { // Only draw if they're different (draw_option by init)
                    if (curr_item) { // There's something here! Draw it
                        draw = curr_item->draw;
                    } else { // There used to be something, but now there isn't
                        draw = draw_nothing;
                    }
                } else if (curr_item && curr_item->type == CLEAR) {
                    // This is a special case for erasing things like doors.
                    draw = curr_item->draw; // i.e. draw_nothing
                }
            } else if (draw_option) { // If doing a full draw, but we're out of bounds, draw the walls. (draw_option by init)
                draw = draw_wall;
            }          
            
            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars
    draw_upper_status();  //Fixed this
    draw_lower_status();   //Fixed this
} 

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // "Random" plants, tiles, and trees.
    Map* map = set_active_map(0);
    
    for(int i = map_width() + 3; i < map_area(); i += 39) {
        add_goodie((i % map_width()), (i / map_width()));
    } 
    for(int i = map_width(); i < map_area(); i += 40) {
        add_tree((i % map_width()), (i / map_width()));
    } 
    
    //add buffs (IBM, GM, MS)
    add_tileIBM(7+ map_width()/2, 17);
    add_tileIBM(1, 10);
    add_tileGM(13+ map_width()/2, 5);
    add_tileGM(map_width()/2 - 9, 5);
    add_tileGM(20, 20);
    add_tileMS(15, 30);
    add_tileMS(45, 10);
    
    //add debuffs (TRUMP, POMPEO, PENCE)
    add_tilePOMPEO(4+ map_width()/2, 0);
    add_tilePOMPEO(6+ map_width()/2, 5);
    add_tilePOMPEO(7+ map_width()/2, 20);
    add_tilePOMPEO(8+ map_width()/2, 11);
    add_tilePOMPEO(9+ map_width()/2, 2);
    add_tilePOMPEO(map_width()/2 - 9, 5);
    add_tilePOMPEO(10 + map_width()/2, 9);
    add_tilePOMPEO(map_width()/2 - 10, 3);
    add_tilePOMPEO(map_width()/2 - 6, 13);
    add_tilePOMPEO(11+ map_width()/2, 5);
    add_tilePOMPEO(2+ map_width()/2, 7);
    add_tilePOMPEO(11+ map_width()/2, 25);
    add_tilePOMPEO(11+ map_width()/2, 5);
    add_tilePOMPEO(18, 20);
    add_tilePOMPEO(10, 36);
    
    add_tileTRUMP(5+ map_width()/2 + 2, 3*map_height()/4);
    add_tileTRUMP(40,25);
    add_tilePENCE(5+ map_width()/2 - 1, 3*map_height()/4 + 2);
    add_tilePENCE(15,20);
    
    
    pc.printf("plants\r\n");

    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");

   
    add_snake_head(snake.locations[0].x, snake.locations[0].y);
    add_snake_body(snake.locations[1].x, snake.locations[1].y);
    add_snake_tail(snake.locations[2].x, snake.locations[2].y);

    pc.printf("Add extra chamber\r\n");
    add_wall(30, 0, VERTICAL, 10);
    //add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39, 0, VERTICAL, 10);
    pc.printf("Added!\r\n");


    // Add stairs to chamber (map 1)
    //add_stairs(15, 5, 1, 5, 5);

//    profile_hashtable();
    print_map();
}

/**
 * Program entry point! This is where it all begins.
 * This function or all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
 
#define YELLOW 0xFFFF00  //0xFFC300
#define LBLUE  0x107A9B 
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
     
    
    uLCD.filled_rectangle(0,0,127,127,BLACK);     //Added this
    uLCD.textbackground_color(BLACK);
    uLCD.color(LBLUE);
    uLCD.locate(1,2);
    uLCD.puts("    ECE 2035");
    uLCD.locate(1,3);
    uLCD.puts("   Snake Game");
    uLCD.color(WHITE);
    uLCD.locate(1,5);
    uLCD.puts("   START MENU");
    uLCD.locate(1,8);
    uLCD.puts("PRESS1: GAME MODE");
    uLCD.locate(1,9);
    uLCD.puts("PRESS2: INVINCI");
    uLCD.locate(1,10);
    uLCD.puts("PRESS3: PAUSE");
    mySpeaker.PlayNote(150.0,0.25,0.005);
    mySpeaker.PlayNote(200.0,0.25,0.005);
    mySpeaker.PlayNote(250.0,0.25,0.005);
    
    
   while(1){
        GameInputs inputs = read_inputs();
        if (inputs.b1)
        {
            uLCD.filled_rectangle(0,0,127,127,BLACK);
            uLCD.textbackground_color(BLACK);
            uLCD.color(WHITE);
            uLCD.locate(1,3);
            uLCD.puts("DIFFICULTY LEVEL");
            uLCD.locate(1,6);
            uLCD.puts("PRESS1: EASY");
            uLCD.locate(1,8);
            uLCD.puts("PRESS2: MEDIUM");
            uLCD.locate(1,10);
            uLCD.puts("PRESS3: HARD");
            
               if(inputs.b1)       //EASY LEVEL of difficulty
               {
                   
                    break;
               }
               else if(inputs.b2) //MEDIUM LEVEL of difficulty
               {
               
                    break;
               }
               else if(inputs.b3) //HARD LEVEL of difficulty
               {
                   
                   break;
               }
            break;
        }
    }
    
    wait(2.5);
    
    uLCD.filled_rectangle(0,0,127,127,BLACK);
    uLCD.textbackground_color(BLACK);
    uLCD.color(WHITE);
    uLCD.locate(1,4);
    uLCD.puts("    READY!!!");
    uLCD.locate(1,7);
    uLCD.puts("   GOOD LUCK");
    mySpeaker.PlayNote(150.0,0.25,0.005);
    mySpeaker.PlayNote(200.0,0.25,0.005);
    mySpeaker.PlayNote(250.0,0.25,0.005);
    wait(1.5);
    uLCD.filled_rectangle(0,0,127,127,BLACK);
    uLCD.textbackground_color(BLACK);
    
    
    snake_init(&snake);
    // 0. Initialize the maps -- implement this function:
    maps_init();
    init_main_map();
    
    // Initialize game state
    set_active_map(0);
    snake.head_x = snake.head_y = 5;
    // Initial drawing
    draw_game(FULL_DRAW);
    snake.locations[0].x = snake.head_x;
    snake.locations[0].y = snake.head_y;
    
    snake.locations[1].x = snake.head_x - 1;
    snake.locations[1].y = snake.head_y;
    
    snake.locations[2].x = snake.head_x - 2;
    snake.locations[2].y = snake.head_y;
    // Main game loop
    while(1) {
        // Timer to measure game update speed
        Timer t;
        t.start();

        // 1. Read inputs -- implement this function:
        GameInputs inputs = read_inputs();
        
        // 2. Determine action (move, act, menu, etc.) -- implement this function:
        int action = get_action(inputs);
        
        // 3. Update game -- implement this function:
        int result  = update_game(action);          //int result
        
        // 3b. Check for game over based on result
        // and if so, handle game over -- implement this.
        
        /**
        //  GAME OVER ROUTINE
        **/
        
        //when exit the game
        if (snake.exit <= 0) {
            mySpeaker.PlayNote(400.0,0.25,0.005);
            mySpeaker.PlayNote(350.0,0.25,0.005);
            mySpeaker.PlayNote(300.0,0.25,0.005);
            mySpeaker.PlayNote(250.0,0.25,0.005);
            mySpeaker.PlayNote(200.0,0.25,0.005); 
            mySpeaker.PlayNote(150.0,0.25,0.005);
            uLCD.filled_rectangle(0, 128, 128, 0, BLACK);
            uLCD.textbackground_color(BLACK);
            uLCD.color(RED);
            uLCD.locate(0,7);
            uLCD.printf("Thanks for playing");
            uLCD.locate(2,9);
            uLCD.printf(" The Snake Game");
            wait(2);
            break;
        }
        
        //When reaching max value
        if (snake.complete <= 0) {
            mySpeaker.PlayNote(400.0,0.25,0.005);
            mySpeaker.PlayNote(350.0,0.25,0.005);
            mySpeaker.PlayNote(300.0,0.25,0.005);
            mySpeaker.PlayNote(250.0,0.25,0.005);
            mySpeaker.PlayNote(200.0,0.25,0.005); 
            mySpeaker.PlayNote(150.0,0.25,0.005);
            uLCD.filled_rectangle(0, 128, 128, 0, BLACK);
            uLCD.textbackground_color(BLACK);
            uLCD.color(RED);
            uLCD.locate(3,7);
            uLCD.printf("You completed");
            uLCD.locate(4,9);
            uLCD.printf("  The Game!");
            wait(2);
            //main();        //start over
            break;
         }       
        //When dying during a play
        if (snake.life <= 0) {
            mySpeaker.PlayNote(200.0,0.25,0.005);
            mySpeaker.PlayNote(150.0,0.25,0.005);
            mySpeaker.PlayNote(100.0,0.25,0.005);
            uLCD.filled_rectangle(0, 128, 128, 0, BLACK);
            uLCD.textbackground_color(BLACK);
            uLCD.color(RED);
            uLCD.locate(3,7);
            uLCD.printf(" Game Over");
            uLCD.locate(4,9);
            uLCD.printf("Try Again");
            wait(2);
            //main();                   //start over
            break;
         }       
        // 4. Draw screen -- provided:
        draw_game(result);             //result
        
        // Compute update time
        t.stop();
        int dt = t.read_ms();

        // Display and wait
        // NOTE: Text is 8 pixels tall
        if (dt < 100) wait_ms(100 - dt);
    }
}

// Plays a wavfile
void playSound(char* wav)
{
    //File *wave_file;
    //wave_file = fopen(wav,"r"); 
    //waver.play(wave_file);
    //fclose(wave_file);
}