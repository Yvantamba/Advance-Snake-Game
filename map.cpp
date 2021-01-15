// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.

#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

#define NUM_MAPS 1        // change to 2
#define numBuckets 89     // define the nuumber of buckets
static Map maps[NUM_MAPS];
static int active_map;

static const MapItem CLEAR_SENTINEL = {
    .type = CLEAR,
    .draw = draw_nothing
};

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    unsigned int x_y = X << 16 | Y;                          // TODO: Fix me!
    return x_y;        //returns the unsigned int
                 
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
     return unsigned(key+0x9e3779b9) % numBuckets;              // TODO: Fix me! 
     
}

void maps_init()
{
      // TODO: Implement!
      // Initialize hash table!
      // Set width & height
    HashTable* myHT = createHashTable(map_hash,numBuckets);
    maps[0].items = myHT;
    maps[0].w = 50;
    maps[0].h = 50;
}
//get the active map for use
Map* get_active_map()
{
    return &maps[active_map];
}
//get the active map from the index
Map* set_active_map(int m)
{
    active_map = m;
    return &maps[active_map];
}

void print_map()
{
    char lookup[] = {'W', 'D', 'P', 'A', 'K', 'C', 'N',' ','S'};
    Map* map = get_active_map();
    for(int j = 0; j < map->h; j++)
    {
        for (int i = 0; i < map->w; i++)
        {
            MapItem* item = (MapItem*)getItem(map->items, XY_KEY(i, j));
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
   return get_active_map()->w;                  // TODO: Fix me!
}

int map_height()
{
   return get_active_map()->h;                  // TODO: Fix me!
}

int map_area()
{
  return map_height() * map_width();            // TODO: Fix me!
}

MapItem* get_current(int x, int y)
{
  return (MapItem*)(getItem(get_active_map()->items, XY_KEY(x,y)));    // TODO: Fix me!  
}
MapItem* get_north(int x, int y)
{
  return (MapItem*)(getItem(get_active_map()->items, XY_KEY(x,y-1)));   // TODO: Fix me!   
}
MapItem* get_south(int x, int y)
{
  return (MapItem*)(getItem(get_active_map()->items, XY_KEY(x,y+1)));    // TODO: Fix me!   
}

MapItem* get_east(int x, int y)
{
  return (MapItem*)(getItem(get_active_map()->items, XY_KEY(x+1,y)));    // TODO: Fix me!
}

MapItem* get_west(int x, int y)
{
  return (MapItem*)(getItem(get_active_map()->items, XY_KEY(x-1,y)));    // TODO: Fix me!
}

MapItem* get_here(int x, int y)
{
  MapItem* item = (MapItem*)(getItem(get_active_map()->items, XY_KEY(x,y)));  // TODO: Fix me!
    if (item == NULL)
        return NULL;
    return item;                                                 
}

void map_erase(int x, int y)
{
  deleteItem(get_active_map()->items, XY_KEY(x,y));         // TODO: Fix me!
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = ROCKWALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_goodie(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = GOODIE;
    w1->draw = draw_goodie;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
//Add nothing function
void add_nothing(int x, int y) 
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NOTHING;
    w1->draw = draw_nothing;   
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void remove_goodie(int x, int y) // I'm lazy so overwrite it with a plant
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = GOODIE;     //PLANT
    w1->draw = draw_nothing;   //draw_plant
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    //deleteItem(get_active_map()->items, XY_KEY(x,y)); 
    if (val) free(val); // If something is already there, free it
    
}

void add_snake_body(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SNAKE_BODY;
    w1->draw = draw_snake_body;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_snake_head(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SNAKE_BODY; 
    w1->draw = draw_snake_head;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_snake_tail(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SNAKE_BODY; 
    w1->draw = draw_snake_tail;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

//decoration(amazonian forest trees)
void add_tree(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = LIFE_TREE;
    w1->draw = draw_tree;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}


//buffs
void add_tileMS(int x, int y)    
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = MS_t;
    w1->draw = draw_tile6;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_tileIBM(int x, int y)    
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = IBM_t;
    w1->draw = draw_tile1;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_tileGM(int x, int y)    
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = GM_t;
    w1->draw = draw_tile2;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

//debuffs
void add_tileTRUMP(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = TRUMP_t;
    w1->draw = draw_tile3;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x,y), w1);
    if (val) 
        free(val); // If something is already there, free it  
}
void add_tilePENCE(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PENCE_t;
    w1->draw = draw_tile4;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x,y), w1);
    if (val) 
        free(val); // If something is already there, free it  
}
void add_tilePOMPEO(int x, int y)    //POMPEO
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = POMPEO_t;
    w1->draw = draw_tile5;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}