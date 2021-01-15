// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.

#define SNAKE_MAX_LENGTH 50

#define SNAKE_LEFT 1
#define SNAKE_RIGHT 2
#define SNAKE_UP 3
#define SNAKE_DOWN 4

// Structure of coordinates in the map
typedef struct{
    int x;
    int y;
} Coordinate;

// Snake Structure
typedef struct {
    int head_x, head_y, head_px, head_py; // Location of the head of the snake
    int body_x, body_y, body_px, body_py; // Location of the body of the snake
    int tail_x, tail_y, tail_px, tail_py; // Location of the tail of the snake
    int head_pu, head_pv, tail_pu, tail_pv, body_pu, body_pv; //
    int previous_x, previous_y;
    int pos;
    int length;  // length of the snake
    Coordinate locations[SNAKE_MAX_LENGTH]; // Snake body locations
    int score;   //Current score of the snake
    int speed;   //Current speed of the snake
    int life;    //life of the snake
    int complete; //complete the game
    int exit;    //exit the game
    int pause;   //pause the game
    int unpause; //unpause the game
    int invinci; //Invincibility button 
    int slow;    //check the speed
} Snake;

// Initialize a snake structure
// You want to assign initial values to each of the variables defined above
// in the snake structure.
void snake_init (Snake * snake);
