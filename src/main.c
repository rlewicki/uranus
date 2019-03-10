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

#define MAX_SPRITE_COUNT 40
#define PLAYER_SPRITE_INDEX 0
#define PLAYER_MOVEMENT_SPEED 1
#define PLAYER_SPRITE sprites_container[PLAYER_SPRITE_INDEX]

struct Sprite sprites_container[MAX_SPRITE_COUNT];
BYTE next_sprite_index;
BYTE loop_iterator;

void init_map(void);
void init_sprite(BYTE tile_count, unsigned char* sprite_data);
void update_sprites(void);
void init_global_variables(void);
void process_input(void);

void init_map(void)
{
    set_bkg_data(0, 14, map_tiles_set);
    set_bkg_tiles(0, 0, 20, 18, map_data);
}

void init_sprite(BYTE tile_count, unsigned char* sprite_data)
{
    if(next_sprite_index >= MAX_SPRITE_COUNT)
    {
        printf("40 sprites limit exceeded");
        return;
    }

    sprites_container[next_sprite_index].id = next_sprite_index;
    sprites_container[next_sprite_index].pos_x = SCREENWIDTH / 2;
    sprites_container[next_sprite_index].pos_y = SCREENHEIGHT / 2;
    next_sprite_index++;
    set_sprite_data(sprites_container[next_sprite_index].id, tile_count, sprite_data);
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
}

void process_input(void)
{
    if(joypad() & J_RIGHT)
    {
        PLAYER_SPRITE.pos_x += PLAYER_MOVEMENT_SPEED;
    }
    else if(joypad() & J_LEFT)
    {
        PLAYER_SPRITE.pos_x -= PLAYER_MOVEMENT_SPEED;
    }
    else if(joypad() & J_UP)
    {
        PLAYER_SPRITE.pos_y -= PLAYER_MOVEMENT_SPEED;
    }
    else if(joypad() & J_DOWN)
    {
        PLAYER_SPRITE.pos_y += PLAYER_MOVEMENT_SPEED;
    }
}

BYTE main(void)
{
    init_global_variables();
    init_map();

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    while(1)
    {
        process_input();
        update_sprites();
        wait_vbl_done();
    }

    return 0;
}