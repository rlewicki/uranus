#include <stdio.h>
#include <gb/gb.h>
#include "sprites/character.c"

int main(void)
{
    set_sprite_data(0, 1, character_sprite);
    set_sprite_tile(0, 0);
    move_sprite(0, 50, 50);
    SHOW_SPRITES;
    return 0;
}