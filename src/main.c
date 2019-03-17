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

const unsigned char empty_tile[] = {0x00};

const BYTE START_POS_X = 8 * 10;
const BYTE START_POS_Y = 8 * 13;

#define DIR_NONE 0x00
#define DIR_LEFT 0x01
#define DIR_RIGHT 0x02
#define DIR_BOTTOM 0x04
#define DIR_TOP 0x08

#define MAX_SPRITE_COUNT 40
#define PLAYER_SPRITE_INDEX 0
#define PLAYER_MOVEMENT_SPEED 1
#define PLAYER_SPRITE sprites_container[PLAYER_SPRITE_INDEX]
#define PLAYER_IDLE_TILE_INDEX 0
#define PLAYER_LEFT_TILE_INDEX 1
#define PLAYER_RIGHT_TILE_INDEX 2
#define PLAYER_BOTTOM_TILE_INDEX 3
#define PLAYER_TOP_TILE_INDEX 4
#define ANIMATION_STEP_DURATION 8

struct Sprite sprites_container[MAX_SPRITE_COUNT];
BYTE next_sprite_index;
BYTE loop_iterator;
BYTE current_direction;
BYTE animation_frame_count;
BYTE current_direction_tile_index;
BYTE current_animation_tile_index;

void init_map(void);
void init_player_sprite(void);
void update_sprites(void);
void init_global_variables(void);
void process_input(void);
void update_player_position(void);
void animate_player(void);
BYTE check_for_collision(BYTE potential_pos_x, BYTE potential_pos_y);

void init_map(void)
{
    set_bkg_data(0, 14, map_tiles_set);
    set_bkg_tiles(0, 0, map_dataWidth, map_dataHeight, map_data);
}

void init_player_sprite(void)
{
    set_sprite_data(PLAYER_SPRITE_INDEX, 5, pacman_tiles);
    set_sprite_tile(PLAYER_SPRITE_INDEX, PLAYER_IDLE_TILE_INDEX);
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
    animation_frame_count = 0;
    current_direction_tile_index = 0;
    current_animation_tile_index = 0;
}

void process_input(void)
{
    if(joypad() & J_RIGHT)
    {
        current_direction = DIR_RIGHT;
        current_direction_tile_index = PLAYER_RIGHT_TILE_INDEX;
    }
    else if(joypad() & J_LEFT)
    {
        current_direction = DIR_LEFT;
        current_direction_tile_index = PLAYER_LEFT_TILE_INDEX;
    }
    else if(joypad() & J_UP)
    {
        current_direction = DIR_TOP;
        current_direction_tile_index = PLAYER_TOP_TILE_INDEX;
    }
    else if(joypad() & J_DOWN)
    {
        current_direction = DIR_BOTTOM;
        current_direction_tile_index = PLAYER_BOTTOM_TILE_INDEX;
    }
}

void update_player_position(void)
{
    if(current_direction & DIR_RIGHT)
    {
        if(check_for_collision(PLAYER_SPRITE.pos_x + PLAYER_MOVEMENT_SPEED, PLAYER_SPRITE.pos_y) != 1)
        {
            PLAYER_SPRITE.pos_x += PLAYER_MOVEMENT_SPEED;
        }
    }
    else if(current_direction & DIR_LEFT)
    {
        if(check_for_collision(PLAYER_SPRITE.pos_x - PLAYER_MOVEMENT_SPEED, PLAYER_SPRITE.pos_y) != 1)
        {
            PLAYER_SPRITE.pos_x -= PLAYER_MOVEMENT_SPEED;
        }
    }
    else if(current_direction & DIR_TOP)
    {
        if(check_for_collision(PLAYER_SPRITE.pos_x, PLAYER_SPRITE.pos_y - PLAYER_MOVEMENT_SPEED) != 1)
        {
            PLAYER_SPRITE.pos_y -= PLAYER_MOVEMENT_SPEED;
        }
    }
    else if(current_direction & DIR_BOTTOM)
    {
        if(check_for_collision(PLAYER_SPRITE.pos_x, PLAYER_SPRITE.pos_y + PLAYER_MOVEMENT_SPEED) != 1)
        {
            PLAYER_SPRITE.pos_y += PLAYER_MOVEMENT_SPEED;
        }
    }
}

void animate_player(void)
{
    if(animation_frame_count > ANIMATION_STEP_DURATION)
    {
        animation_frame_count = 0;
        if(current_animation_tile_index == current_direction_tile_index)
        {
            current_animation_tile_index = PLAYER_IDLE_TILE_INDEX;
        }
        else
        {
            current_animation_tile_index = current_direction_tile_index;
        }
        
        set_sprite_tile(PLAYER_SPRITE_INDEX, current_animation_tile_index);
    }

    animation_frame_count++;
}

BYTE check_for_collision(BYTE potential_pos_x, BYTE potential_pos_y)
{
    UINT16 horizontal_tile;
    UINT16 vertical_tile;
    UINT16 tile_index;

    if(current_direction & DIR_RIGHT)
    {
        horizontal_tile = potential_pos_x / 8;
    }
    else
    {
        horizontal_tile = (potential_pos_x - 8) / 8;
    }
    
    if(current_direction & DIR_BOTTOM)
    {
        vertical_tile = (potential_pos_y - 8) / 8;
    }
    else
    {
        vertical_tile = (potential_pos_y - 16) / 8;
    }
    
    tile_index = vertical_tile * map_dataWidth + horizontal_tile;

    if(map_data[tile_index] != 0x00 && map_data[tile_index] != 0x03)
    {
        current_direction = DIR_NONE;
        current_direction_tile_index = PLAYER_IDLE_TILE_INDEX;
        return 1;
    }
    
    return 0;
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
        animate_player();
        update_sprites();
        wait_vbl_done();
    }

    return 0;
}