
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.

#ifndef GRAPHICS_H
#define GRAPHICS_H

#define YELLOW    0xFFFF00
#define BROWNISH  0xFD962C
#define DIRT      0xD2691E
#define GREY      0xA9A9A9
#define BLUE      0x1730FB
#define DGREEN    0x0D6334
#define LGREEN    0x30EC1A
#define GOLD      0xCFBE31
#define LBLUE     0x107A9B
#define WHITE     0xFFFFFF

/**
 * Takes a string image and draws it to the screen. The string is 121 characters
 * long, and represents an 11x11 tile in row-major ordering (across, then down,
 * like a regular multi-dimensional array). The available colors are:
 *      R = Red
 *      Y = Yellow
 *      G = Green
 *      D = Brown ("dirt")
 *      5 = Light grey (50%)
 *      3 = Dark grey (30%)
 *      Any other character is black
 * More colors can be easily added by following the pattern already given.
 */
void draw_img(int u, int v, const char* img);

/**
 * DrawFunc functions. 
 * These can be used as the MapItem draw functions.
 */
//void draw_ntg(int u, int v); 
void draw_nothing(int u, int v);
void draw_plant(int u, int v);
void draw_wall(int u, int v);
void draw_goodie(int u, int v);
void draw_snake_body(int u, int v);
void draw_snake_head(int u, int v);
void draw_snake_tail(int u, int v);
void draw_snake_turn(int u, int v);
void draw_tree(int u, int v);
//void draw_border();
//void draw_upper_status(int x, int y, int a);
//void draw_lower_status(int value1, int value2);
void draw_tile1(int u, int v);
void draw_tile2(int u, int v);
void draw_tile3(int u, int v);
void draw_tile4(int u, int v);
void draw_tile5(int u, int v);
void draw_tile6(int u, int v);
#endif // GRAPHICS_H