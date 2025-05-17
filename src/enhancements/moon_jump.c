#include <libultraship.h>
#include "main.h"
#include "code_800029B0.h"
#include "common_structs.h"

void moon_jump(Player* player, struct Controller* controller) {
    if (controller->button & L_TRIG) {
        if (player->velocity.y <= 3.2f) {
            player->velocity.y += 0.5f;
        }
    }
}
