#include "../headers/game.h"
#include "../mods/headers/lunar_rover.h"
#include "../mods/headers/player.h"

int main(int argc, char* argv[])
{
    srand(time(0));

    Lunar_Rover::run_lunar_rover(argv[0]);

    return 0;
}