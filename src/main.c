#include <stdio.h>
#include <gb/gb.h>
#include "sprites/map_tiles_set.h"
#include "sprites/map_data.h"
#include "sprites/pacman_tiles_set.h"

struct Sprite
{
    BYTE id;
    BYTE pos_x;
    BYTE pos_y;
};

unsigned char empty_tile[] = {0x00};

const BYTE START_POS_X = 8 * 10;
const BYTE START_POS_Y = 8 * 13;

#define DIR_LEFT 0x01U
#define DIR_RIGHT 0x02U
#define DIR_BOTTOM 0x04U
#define DIR_TOP 0x08U

#define MAX_SPRITE_COUNT 40
#define PLAYER_SPRITE_INDEX 0
#define PLAYER_MOVEMENT_SPEED 1
#define PLAYER_SPRITE sprites_container[PLAYER_SPRITE_INDEX]

struct Sprite sprites_container[MAX_SPRITE_COUNT];
BYTE next_sprite_index;
BYTE loop_iterator;
BYTE current_direction;

void init_map(void);
void init_player_sprite(void);
void update_sprites(void);
void init_global_variables(void);
void process_input(void);
void update_player_position(void);

void init_map(void)
{
    set_bkg_data(0, 14, map_tiles_set);
    set_bkg_tiles(0, 0, 20, 18, map_data);
}

void init_player_sprite(void)
{
    set_sprite_data(PLAYER_SPRITE_INDEX, 2, pacman_tiles);
    set_sprite_tile(PLAYER_SPRITE_INDEX, 0);
    move_sprite(PLAYER_SPRITE_INDEX, START_POS_X, START_POS_Y);

    PLAYER_SPRITE.id = PLAYER_SPRITE_INDEX;
    PLAYER_SPRITE.pos_x = START_POS_X;
    PLAYER_SPRITE.pos_y = START_POS_Y;

    next_sprite_index++; 
}

void update_sprites(void)
{
    for(loop_iterator = 0; loop_iterator < next_sprite_index; loop_iterator++)
    {
        move_sprite(
            sprites_container[loop_iterator].id, 
            sprites_container[loop_iterator].pos_x,
            sprites_container[loop_iterator].pos_y);
    }
}

void init_global_variables(void)
{
    next_sprite_index = 0;
    loop_iterator = 0;
    current_direction = 0;
}

void process_input(void)
{
    if(joypad() & J_RIGHT)
    {
        current_direction = DIR_RIGHT;
    }
    else if(joypad() & J_LEFT)
    {
        current_direction = DIR_LEFT;
    }
    else if(joypad() & J_UP)
    {
        current_direction = DIR_TOP;
    }
    else if(joypad() & J_DOWN)
    {
        current_direction = DIR_BOTTOM;
    }
}

void update_player_position(void)
{
    if(current_direction & DIR_RIGHT)
    {
        PLAYER_SPRITE.pos_x += PLAYER_MOVEMENT_SPEED;
    }
    else if(current_direction & DIR_LEFT)
    {
        PLAYER_SPRITE.pos_x -= PLAYER_MOVEMENT_SPEED;
    }
    else if(current_direction & DIR_TOP)
    {
        PLAYER_SPRITE.pos_y -= PLAYER_MOVEMENT_SPEED;
    }
    else if(current_direction & DIR_BOTTOM)
    {
        PLAYER_SPRITE.pos_y += PLAYER_MOVEMENT_SPEED;
    }
}

BYTE main(void)
{
    init_global_variables();
    init_map();
    init_player_sprite();

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    while(1)
    {
        process_input();
        update_player_position();
        update_sprites();
        wait_vbl_done();
    }

    return 0;
}