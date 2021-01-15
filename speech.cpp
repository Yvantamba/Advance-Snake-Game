#include "speech.h"

#include "globals.h"
#include "hardware.h"

/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);


 //Delay until it is time to scroll.
 static void speech_bubble_wait();
 
void draw_speech_bubble()
{
   uLCD.filled_rectangle(0, 118, 127, 127, WHITE);      //Fix this
}

void erase_speech_bubble()                             //Fix this
{
      uLCD.locate(1,10);
    uLCD.textbackground_color(WHITE);
    uLCD.color(WHITE);
      uLCD.locate(1,12);
    uLCD.textbackground_color(WHITE);
    uLCD.color(WHITE);
}

void erase_speech_line(const char* line, int which){   //Added this
    if (which==0){
       uLCD.locate(1,10);
       uLCD.textbackground_color(WHITE);
       uLCD.color(WHITE);
    } else {
       uLCD.locate(1,12);
       uLCD.textbackground_color(WHITE);
       uLCD.color(WHITE);
    }
    uLCD.set_font_size(2,2);
    uLCD.printf(line);      
}

void draw_speech_line(const char* line, int which)       //Fix this
{
if (which==0){
       uLCD.locate(1,10);
       uLCD.textbackground_color(WHITE);
       uLCD.color(BLUE);
    } else {
       uLCD.locate(1,12);
       uLCD.textbackground_color(WHITE);
       uLCD.color(BLUE);
    }
    uLCD.set_font_size(2,2);
    uLCD.printf(line);    
}
 
void speech_bubble_wait()                           //Fix this
{
    while(1){
        GameInputs inputs = read_inputs();
        if (inputs.b1==1){
            return;
        } wait(0.3);
    }
}

void speech(const char* line1, const char* line2)  //Fix this
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_line(line1, TOP);
    erase_speech_line(line2, BOTTOM);
}

void long_speech(const char* lines[], int n)       //Fix this
{
    uLCD.filled_rectangle(1, 70, 127, 110, BLACK);
    for (int i = 0; i < n/2; i++) {
        draw_speech_bubble();
        draw_speech_line(lines[i*2], TOP);
        draw_speech_line(lines[i*2+1], BOTTOM);
        speech_bubble_wait();
        draw_speech_line(lines[i*2+1], BOTTOM);
    }
}
