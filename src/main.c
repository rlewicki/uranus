#include <stdio.h>
#include <gb/gb.h>
#include "sprites/character.h"

struct Sprite
{
    BYTE id;
    BYTE pos_x;
    BYTE pos_y;
};

#define MAX_SPRITE_COUNT 40
#define PLAYER_SPRITE_INDEX 0

struct Sprite sprites_container[MAX_SPRITE_COUNT];
BYTE next_sprite_index;
BYTE loop_iterator;

void init_sprite(BYTE tile_count, unsigned char* sprite_data)
{
    if(next_sprite_index >= MAX_SPRITE_COUNT)
    {
        printf("Cannot create more then 40 sprites");
        return;
    }

    sprites_container[next_sprite_index].id = next_sprite_index;
    sprites_container[next_sprite_index].pos_x = 50;
    sprites_container[next_sprite_index].pos_y = 50;
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
    if(joypad() == J_RIGHT)
    {
        sprites_container[PLAYER_SPRITE_INDEX].pos_x += 10;
    }
    else if(joypad() == J_LEFT)
    {
        sprites_container[PLAYER_SPRITE_INDEX].pos_x -= 10;
    }
    else if(joypad() == J_UP)
    {
        sprites_container[PLAYER_SPRITE_INDEX].pos_y -= 10;
    }
    else if(joypad() == J_DOWN)
    {
        sprites_container[PLAYER_SPRITE_INDEX].pos_y += 10;
    }
}

BYTE main(void)
{
    init_global_variables();
    // First initialized sprite is interpreted as player sprite
    init_sprite(1, character_sprite);
    init_sprite(1, character_sprite);

    SHOW_SPRITES;

    while(1)
    {
        process_input();
        update_sprites();
        delay(100);
    }

    return 0;
}