#ifndef Player_H_INCLUDED
#define Player_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/src/algif.h"

/*
   [Player object]
*/

typedef enum PlayerType
{
	p_STOP = 0,
	p_MOVE,
    p_CEASE_FIRE,
	p_FIRE
} PlayerType;



typedef struct _Player
{
    double x, y;     
    int timer; 
    int rpm;   
    int damage;
    int bullet_speed;   
    int velocity; 
    int width, height; // the width and height of image
    bool dir;         // true: face to right, false: face to left
    double angle;    
    int wlk_state;  
    int atk_state;      // the state of Player
    int heart;
    int exp;
    int new_shot;
    int anime;      // counting the time of animation
    int anime_time; // indicate how long the animation
    char name[20];
    Shape *hitbox; // the hitbox of object
    ALLEGRO_BITMAP* img; // gif for each state. 0: stop, 1: move, 2:attack
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    ALLEGRO_FONT* font;
} Player;


Elements *New_Player(int label,int exp);
void _Player_update_position(Elements *const self, int dx, int dy);
void Player_update(Elements *const self);
void Player_interact(Elements *const self, Elements *const target);
void Player_draw(Elements *const self);
void Player_destory(Elements *const self);

#endif
