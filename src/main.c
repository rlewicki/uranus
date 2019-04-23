#include <stdio.h>
#include <gb/gb.h>
#include "sprites/map_tiles_set.h"
#include "sprites/map_data.h"
#include "sprites/pacman_tiles_set.h"

struct Sprite
{
    BYTE id;
    INT16 pos_x;
    INT16 pos_y;
};

const unsigned char empty_tile[] = {0x00};

const INT16 START_POS_X = 8 * 10;
const INT16 START_POS_Y = 8 * 13;

#define DIR_NONE 0x01
#define DIR_LEFT 0x02
#define DIR_RIGHT 0x04
#define DIR_BOTTOM 0x08
#define DIR_TOP 0x10

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
BYTE is_debug;

INT16 potential_x;
INT16 potential_y;

void init_map(void);
void init_player_sprite(void);
void update_sprites(void);
void init_global_variables(void);
void process_input(void);
void update_player_position(void);
void animate_player(void);
BYTE check_for_collision(void);
BYTE is_aligned_vertically(void);
BYTE is_aligned_horizontally(void);

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
    current_direction = DIR_NONE;
    animation_frame_count = 0;
    current_direction_tile_index = 0;
    current_animation_tile_index = 0;
    is_debug = 0;
    potential_x = 0;
    potential_y = 0;
}

BYTE is_aligned_vertically(void)
{
    if(PLAYER_SPRITE.pos_x % 8 == 0)
    {
        return 1;
    }

    return 0;
}

BYTE is_aligned_horizontally(void)
{
    if(PLAYER_SPRITE.pos_y % 8 == 0)
    {
        return 1;
    }

    return 0;
}

void process_input(void)
{
    if(joypad() & J_RIGHT)
    {
        potential_x = PLAYER_SPRITE.pos_x + PLAYER_MOVEMENT_SPEED;
        potential_y = PLAYER_SPRITE.pos_y;
        if(check_for_collision() != 1 && is_aligned_horizontally() == 1)
        {
            current_direction = DIR_RIGHT;
            current_direction_tile_index = PLAYER_RIGHT_TILE_INDEX;
        }
    }
    else if(joypad() & J_LEFT)
    {
        potential_x = PLAYER_SPRITE.pos_x - PLAYER_MOVEMENT_SPEED;
        potential_y = PLAYER_SPRITE.pos_y;
        if(check_for_collision() != 1 && is_aligned_horizontally() == 1)
        {
            current_direction = DIR_LEFT;
            current_direction_tile_index = PLAYER_LEFT_TILE_INDEX;
        }
    }
    else if(joypad() & J_UP)
    {
        if(is_debug == 1)
        {
            printf("P POS: %u\n", PLAYER_SPRITE.pos_y);
            printf("PP POS: %u\n", PLAYER_SPRITE.pos_y - PLAYER_MOVEMENT_SPEED);
        }

        potential_x = PLAYER_SPRITE.pos_x;
        potential_y = PLAYER_SPRITE.pos_y - PLAYER_MOVEMENT_SPEED;
        if(check_for_collision() != 1 && is_aligned_vertically() == 1)
        {
            current_direction = DIR_TOP;
            current_direction_tile_index = PLAYER_TOP_TILE_INDEX;
        }
    }
    else if(joypad() & J_DOWN)
    {
        potential_x = PLAYER_SPRITE.pos_x;
        potential_y = PLAYER_SPRITE.pos_y + PLAYER_MOVEMENT_SPEED;
        if(check_for_collision() != 1 && is_aligned_vertically() == 1)
        {
            current_direction = DIR_BOTTOM;
            current_direction_tile_index = PLAYER_BOTTOM_TILE_INDEX;
        }
    }
    else if(joypad() &  J_A)
    {
        is_debug = 1;
    }
}

void update_player_position(void)
{
    if(current_direction & DIR_RIGHT)
    {
        potential_x = PLAYER_SPRITE.pos_x + PLAYER_MOVEMENT_SPEED;
        potential_y = PLAYER_SPRITE.pos_y;
        if(check_for_collision() != 1)
        {
            PLAYER_SPRITE.pos_x += PLAYER_MOVEMENT_SPEED;
        }
        else
        {
            current_direction = DIR_NONE;
            current_direction_tile_index = PLAYER_IDLE_TILE_INDEX;
        }
    }
    else if(current_direction & DIR_LEFT)
    {
        potential_x = PLAYER_SPRITE.pos_x - PLAYER_MOVEMENT_SPEED;
        potential_y = PLAYER_SPRITE.pos_y;
        if(check_for_collision() != 1)
        {
            PLAYER_SPRITE.pos_x -= PLAYER_MOVEMENT_SPEED;
        }
        else
        {
            current_direction = DIR_NONE;
            current_direction_tile_index = PLAYER_IDLE_TILE_INDEX;
        }
    }
    else if(current_direction & DIR_TOP)
    {
        potential_x = PLAYER_SPRITE.pos_x;
        potential_y = PLAYER_SPRITE.pos_y - PLAYER_MOVEMENT_SPEED;
        if(check_for_collision() != 1)
        {
            PLAYER_SPRITE.pos_y -= PLAYER_MOVEMENT_SPEED;
        }
        else
        {
            current_direction = DIR_NONE;
            current_direction_tile_index = PLAYER_IDLE_TILE_INDEX;
        }
    }
    else if(current_direction & DIR_BOTTOM)
    {
        potential_x = PLAYER_SPRITE.pos_x;
        potential_y = PLAYER_SPRITE.pos_y + PLAYER_MOVEMENT_SPEED;
        if(check_for_collision() != 1)
        {
            PLAYER_SPRITE.pos_y += PLAYER_MOVEMENT_SPEED;
        }
        else
        {
            current_direction = DIR_NONE;
            current_direction_tile_index = PLAYER_IDLE_TILE_INDEX;
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

BYTE check_for_collision(void)
{
    INT16 horizontal_tile;
    INT16 vertical_tile;
    INT16 tile_index;

    if(is_debug == 1)
    {
    }

    if(!(current_direction & DIR_LEFT) && potential_x % 8 != 0)
    {
        horizontal_tile = (potential_x - 1) / 8;
    }
    else
    {
        horizontal_tile = (potential_x - 8) / 8;
    }
    
    if(!(current_direction & DIR_TOP) && potential_y % 8 != 0)
    {
        vertical_tile = (potential_y - 9) / 8;    
    }
    else
    {
        vertical_tile = (potential_y - 16) / 8;
    }
    
    tile_index = vertical_tile * map_dataWidth + horizontal_tile;

    if(map_data[tile_index] != 0x00 && map_data[tile_index] != 0x03)
    {
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