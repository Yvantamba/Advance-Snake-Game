// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.

#include "snake.h"
#include <string.h>

void snake_init (Snake * s)
{ 
    s->head_x =  0;
    s->head_y =  0;
    s->head_px = 0;
    s->head_py = 0;
    s->body_x =  0;
    s->body_y =  0;
    s->body_px = 0;
    s->body_py = 0;
    s->tail_x =  0;
    s->tail_y =  0;
    s->tail_px = 0;
    s->tail_py = 0;
    s->length =  3;
    s->head_pu = 0;
    s->head_pv = 0;
    s->previous_x = 0; //new
    s->previous_y = 0; //new
    s->tail_pu = 0;
    s->tail_pv = 0;
    s->pos = 0; 
    s->score = 0;
    s->speed = 0;
    s->life = 1;
    s->complete = 1;
    s->pause;
    s->unpause;
    s->slow = 0;
    s->invinci = 1000;
    s->exit = 1;
    
    for(int i = 0 ; i < SNAKE_MAX_LENGTH ; i++)
    {
        s->locations[i].x = 0;
        s->locations[i].y = 0;
    }
    s->score = 0;
}

