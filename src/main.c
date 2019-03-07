#include <stdio.h>
#include <gb/gb.h>
#include "sprites/character.h"

struct Sprite
{
    int id;
    int pos_x;
    int pos_y;
};

struct Sprite main_character;

int sprite_index;

void init_main_character(int tile_count, unsigned char* sprite_data)
{
    main_character.id = sprite_index++;
    main_character.pos_x = 0;
    main_character.pos_y = 0;
    set_sprite_data(main_character.id, tile_count, sprite_data);
}

void update_sprite_position(struct Sprite* sprite)
{
    move_sprite(sprite->id, sprite->pos_x, sprite->pos_y);
}

void init_global_variables(void)
{
    sprite_index = 0;
}

void process_input(void)
{
    if(joypad() == J_RIGHT)
    {
        main_character.pos_x += 10;
    }
    else if(joypad() == J_LEFT)
    {
        main_character.pos_x -= 10;
    }
    else if(joypad() == J_UP)
    {
        main_character.pos_y += 10;
    }
    else if(joypad() == J_DOWN)
    {
        main_character.pos_y -= 10;
    }
}

int main(void)
{
    init_global_variables();
    init_main_character(1, character_sprite);
    set_sprite_tile(main_character.id, 0);

    while(1)
    {
        process_input();
        update_sprite_position(&main_character);
        SHOW_SPRITES;
        delay(100);
    }

    return 0;
}