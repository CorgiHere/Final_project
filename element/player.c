#include "player.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/src/algif.h"
#include "bullet.h"
#include "monster.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro_primitives.h>
#include<math.h>
#include <stdio.h>
/*
   [Player function]
*/
Elements *New_Player(int label,int exp)
{
    Player *pDerivedObj = (Player *)malloc(sizeof(Player));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load Player images
    pDerivedObj->img=al_load_bitmap("assets/image/player.png");
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/atk_sound.wav");
    pDerivedObj->atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_Sound, al_get_default_mixer());

    // initial the geometric information of Player
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->timer=0;
    pDerivedObj->rpm=30;
    pDerivedObj->heart=300;
    pDerivedObj->x = 10;
    pDerivedObj->y = 10;
    pDerivedObj->velocity=5;
    pDerivedObj->angle=atan2(mouse.y- pDerivedObj->y,mouse.x-pDerivedObj->x);
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x,
                                        pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);
    pDerivedObj->dir = false; // true: face to right, false: face to left
    // initial the animation component
    pObj->inter_obj[pObj->inter_len++] = Monster_L;
    pDerivedObj->wlk_state = p_STOP;
    pDerivedObj->atk_state=CEASE_FIRE;
    pDerivedObj->new_shot= 0;
    pDerivedObj->exp=exp;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Player_draw;
    pObj->Update = Player_update;
    pObj->Interact = Player_interact;
    pObj->Destroy = Player_destory;
    return pObj;
}
void Player_update(Elements *const ele)
{    
    // use the idea of finite state machine to deal with different state
    Player *chara = ((Player *)(ele->pDerivedObj));
    chara->timer+=5;
    if (chara->wlk_state == p_STOP)
    {
        if (key_state[ALLEGRO_KEY_SPACE])
        {
            chara->atk_state=FIRE;
        }
        if (key_state[ALLEGRO_KEY_A])
        {
            chara->dir = false;
            chara->wlk_state = p_MOVE;
        }
        else if (key_state[ALLEGRO_KEY_D])
        {
            chara->dir = true;
            chara->wlk_state = p_MOVE;
        }
        else if (key_state[ALLEGRO_KEY_W]){
            chara->wlk_state=p_MOVE;
        }
        else if(key_state[ALLEGRO_KEY_S]){
            chara->wlk_state=p_MOVE;
        }
        else
        {
            chara->wlk_state = p_STOP;
        }
    }
    else if (chara->wlk_state == p_MOVE)
    {
        if (key_state[ALLEGRO_KEY_SPACE])
        {
            chara->atk_state = ATK;
        }
        if (key_state[ALLEGRO_KEY_A])
        {
            chara->dir = false;
            _Player_update_position(ele, -chara->velocity, 0);
            chara->wlk_state = p_MOVE;
        }
        
        else if (key_state[ALLEGRO_KEY_D])
        {
            chara->dir = true;
            _Player_update_position(ele, chara->velocity, 0);
            chara->wlk_state = p_MOVE;
        }
        else if (key_state[ALLEGRO_KEY_W]){
            chara->wlk_state=p_MOVE;
            _Player_update_position(ele, 0,-chara->velocity);
        }
        else if(key_state[ALLEGRO_KEY_S]){
            chara->wlk_state=p_MOVE;
            _Player_update_position(ele, 0,chara->velocity);
        }
        else{
            chara->wlk_state=p_STOP;
        }
    }
    if (chara->atk_state == FIRE)
    {    
        if(chara->timer>=chara->rpm){
         double mx=mouse.x,my=mouse.y;
         double angle=atan2(my-chara->y,mx-chara->x);
         Elements *bullet;
         bullet=New_Bullet(Bullet_L,chara->x,chara->y,angle,2*chara->velocity,20);
         _Register_elements(scene,bullet);
         chara->atk_state=CEASE_FIRE;
         chara->timer%=chara->rpm;
        }
        else{
           chara->atk_state = CEASE_FIRE; 
        }
    }
    chara->angle=atan2(mouse.y- chara->y,mouse.x-chara->x);
}
void Player_draw(Elements *const ele)
{
    // with the state, draw corresponding image
    Player *Obj = ((Player *)(ele->pDerivedObj));
    al_draw_rotated_bitmap(Obj->img,Obj->width/2,Obj->height/2,Obj->x,Obj->y,Obj->angle+2.355,0);
}
void Player_destory(Elements *const ele)
{
    Player *Obj = ((Player *)(ele->pDerivedObj));
    al_destroy_sample_instance(Obj->atk_Sound);
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(ele);
}

void _Player_update_position(Elements *const ele, int dx, int dy)
{
    Player *chara = ((Player *)(ele->pDerivedObj));
    chara->x += dx;
    chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

void Player_interact(Elements *const self, Elements *const target) {
    /*
    Player *pl=((Player*)(self));
    if(target->label == Monster_L){
        Monster *mon=((Monster*)(target));
        if(mon->hitbox->overlap(mon->hitbox,pl->hitbox)){
            if(pl->heart>mon->damage){
                pl->heart-=mon->damage;
            }
            else{
                self->dele=true;
            }
        }
    }*/
    
}


