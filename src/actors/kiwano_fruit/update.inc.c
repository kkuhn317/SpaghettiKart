#include <actors.h>
#include <main.h>
#include <defines.h>
#include <waypoints.h>

/**
 * @brief Updates the kiwano fruit actor.
 * Actor used in DK's Jungle Parkway.
 *
 * @param fruit
 */
void update_actor_kiwano_fruit(struct KiwanoFruit* fruit) {
    Player* player;
    f32 temp_f2;
    f32 temp_f16;
    f32 temp_f14;
    f32 temp_f12;
    s32 nearestWaypoint;

    player = &gPlayers[fruit->targetPlayer];
    if (((player->type & PLAYER_KART_AI) != 0) || (player->collision.unk34 == 0)) {
        fruit->state = 0;
        return;
    }
    switch (fruit->state) { /* irregular */
        case 0:
            if ((get_surface_type(player->collision.meshIndexZX) & 0xFF) != GRASS) {
                return;
            }
            fruit->state = 1;
            fruit->velocity.x = 80.0f;
        case 1:
            nearestWaypoint = gNearestWaypointByPlayerId[(u16) (player - gPlayerOne)];
            temp_f2 = player->pos.x - D_80164490[nearestWaypoint].posX;
            temp_f16 = player->pos.y - D_80164490[nearestWaypoint].posY;
            temp_f14 = player->pos.z - D_80164490[nearestWaypoint].posZ;
            temp_f12 = fruit->velocity.x / sqrtf((temp_f2 * temp_f2) + (temp_f16 * temp_f16) + (temp_f14 * temp_f14));
            temp_f2 *= temp_f12;
            temp_f16 *= temp_f12;
            temp_f14 *= temp_f12;
            fruit->pos.x = player->pos.x + temp_f2;
            fruit->pos.y = player->pos.y + temp_f16;
            fruit->pos.z = player->pos.z + temp_f14;
            fruit->velocity.x -= 2.0f;
            if (fruit->velocity.x <= 0.0f) {
                fruit->state = 2;
                fruit->bonkTimer = 30.0f;
                fruit->velocity.x = 0.0f;
                fruit->velocity.y = 2.3f;
                fruit->velocity.z = 0.0f;
                if ((player->effects & STAR_EFFECT) != 0) {
                    func_800C9060(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0xA0, 0x52));
                } else {
                    player->effects |= 0x8000;
                    player->pos.x -= temp_f2 * 4.0f;
                    player->pos.z -= temp_f14 * 4.0f;
                    player->velocity.x -= temp_f2 * 0.7f;
                    player->velocity.z -= temp_f14 * 0.7f;
                    func_800C9060(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x18));
                    if (gModeSelection != GRAND_PRIX) {
                        D_80162DF8 = 1;
                    }
                }
            }
            break;
        case 2:
            fruit->velocity.y -= 0.3f;
            fruit->pos.x += fruit->velocity.x;
            fruit->pos.y += fruit->velocity.y;
            fruit->pos.z += fruit->velocity.z;
            fruit->bonkTimer -= 1.0f;
            if (fruit->bonkTimer < 0.0f) {
                fruit->state = 0;
            }
            break;
        default:
            break;
    }
    if (fruit->state != 0) {
        fruit->animTimer += 1;
        if (fruit->animTimer == 8) {
            fruit->animTimer = 0;
            fruit->animState += 1;
            if (fruit->animState == 3) {
                fruit->animState = 0;
            }
        }
    }
}
