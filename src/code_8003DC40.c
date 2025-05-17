#include <libultraship.h>
#include <macros.h>
#include <mk64.h>
#include "math_util.h"
#include <common_structs.h>
#include "player_controller.h"
#include "effects.h"
#include "code_8003DC40.h"
#include "defines.h"
#include "port/Game.h"

void func_8003DC40(Player* player) {
    player->unk_DAC = 1.0f;
}

UNUSED void func_8003DC50(Player* player, Vec3f arg1) {
    s32 thing1 = 0;
    s32 thing2 = 0;
    // Praise Fake Match!!
    if (player) {};
    if (player->unk_230 <= player->unk_23C) {
        if (player->collision.unk48.y >= 0.1736) {
            arg1.x = (player->unk_206 / 182) * 0x78;
            arg1.z = -(player->slopeAccel / 182) * 0x78;
        } else {
            arg1.x = thing1;
            arg1.z = thing2;
        }
    } else if (player->collision.unk48.y >= 0.1736) {
        arg1.x = (player->unk_206 / 182) * 0x78;
        arg1.z = -(player->slopeAccel / 182) * 0x78;
    } else {
        arg1.x = thing1;
        arg1.z = thing2;
    }
    arg1.y = 0.0f;
    mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
}

UNUSED void func_8003DE4C(Player* player, Vec3f arg1) {
    s32 thing1 = 0;
    s32 thing2 = 0;
    // Praise Fake Match!!
    if (player) {};
    if (player->unk_230 <= player->unk_23C) {
        if (player->collision.unk54.y >= 0.1736) {
            arg1.x = (player->unk_206 / 182) * 0x78;
            arg1.z = -(player->slopeAccel / 182) * 0x78;
        } else {
            arg1.x = thing1;
            arg1.z = thing2;
        }
    } else if (player->collision.unk54.y >= 0.1736) {
        arg1.x = (player->unk_206 / 182) * 0x78;
        arg1.z = -(player->slopeAccel / 182) * 0x78;
    } else {
        arg1.x = thing1;
        arg1.z = thing2;
    }
    arg1.y = 0.0f;
    mtxf_translate_vec3f_mat3(arg1, player->orientationMatrix);
}
// Stick to ground?
void func_8003E048(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1.x * player->collision.surfaceDistance.z * 1;
    *arg6 += arg1.y * player->collision.surfaceDistance.z * 0.1;
    *arg7 += arg1.z * player->collision.surfaceDistance.z * 1;
    func_8002A5F4(arg1, *arg4, arg2, 1, 2);
    if (player->collision.orientationVector.y <= 0.8829f) {
        arg3.x = ((player->unk_206 / 182) * 0xB4);
        arg3.z = (-(player->slopeAccel / 182) * 0xB4);
        decelerate_ai_player(player, 4.0f);
        player->unk_DAC = 0.5f;
        if ((player->effects & BOOST_EFFECT) != 0) {
            remove_boost_effect(player);
            player->currentSpeed /= 2;
            player->unk_08C /= 2;
        }
    } else if ((((player->unk_094 / 18.0f) * 216.0f) > 20.0f) || ((player->effects & 0x10000) == 0x10000)) {
        arg3.x = ((player->unk_206 / 182) * 0x32);
        arg3.z = (-(player->slopeAccel / 182) * 0x3C);
    } else {
        arg3.x = 0.0f;
        arg3.z = (-(player->slopeAccel / 182) * 0x32);
    }
    arg3.y = 0.0f;
    mtxf_translate_vec3f_mat3(arg3, player->orientationMatrix);
}

void func_8003E37C(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1.x * player->collision.surfaceDistance.z * 1;
    *arg6 += arg1.y * player->collision.surfaceDistance.z * 0.2;
    *arg7 += arg1.z * player->collision.surfaceDistance.z * 1;
    func_8002A5F4(arg1, *arg4, arg2, 0.5f, 2);
    if ((player->collision.orientationVector.y <= 0.7318f) || (player->surfaceType == CLIFF)) {
        arg3.x = ((player->unk_206 / 182) * 0xB4);
        arg3.z = (-(player->slopeAccel / 182) * 0xB4);
        if (((player->unk_094 / 18.0f) * 216.0f) >= 8.0f) {
            decelerate_ai_player(player, 5.0f);
        }
        player->unk_DAC = 0.5f;
        if ((player->effects & BOOST_EFFECT) != 0) {
            remove_boost_effect(player);
            player->currentSpeed /= 2;
            player->unk_08C /= 2;
        }
    } else if ((((player->unk_094 / 18.0f) * 216.0f) > 20.0f) || ((player->effects & 0x10000) == 0x10000)) {
        arg3.x = ((player->unk_206 / 182) * 0x32);
        arg3.z = (-(player->slopeAccel / 182) * 0x32);
    } else {
        arg3.x = 0.0f;
        arg3.z = (-(player->slopeAccel / 182) * 0x32);
    }
    arg3.y = 0.0f;
    mtxf_translate_vec3f_mat3(arg3, player->orientationMatrix);
}

void func_8003E6EC(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1.x * player->collision.surfaceDistance.z * 1;
    *arg6 += arg1.y * player->collision.surfaceDistance.z * 0.1;
    *arg7 += arg1.z * player->collision.surfaceDistance.z * 1;
    func_8002A5F4(arg1, *arg4, arg2, 0.5f, 2);
    if (player->collision.orientationVector.y <= 0.8829f) {
        arg3.x = ((player->unk_206 / 182) * 0xB4);
        arg3.z = (-(player->slopeAccel / 182) * 0xB4);
        decelerate_ai_player(player, 4.0f);
        func_8003DC40(player);
    } else if ((((player->unk_094 / 18.0f) * 216.0f) > 20.0f) || ((player->effects & 0x10000) == 0x10000)) {
        arg3.x = ((player->unk_206 / 182) * 0x32);
        arg3.z = (-(player->slopeAccel / 182) * 0x3C);
    } else {
        arg3.x = 0.0f;
        arg3.z = (-(player->slopeAccel / 182) * 0x32);
    }
    arg3.y = 0.0f;
    mtxf_translate_vec3f_mat3(arg3, player->orientationMatrix);
}

void func_8003E9EC(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1.x * player->collision.surfaceDistance.z * 1;
    *arg6 += arg1.y * player->collision.surfaceDistance.z * 0.1;
    *arg7 += arg1.z * player->collision.surfaceDistance.z * 1;
    func_8002A5F4(arg1, *arg4, arg2, 1.2f, 2);
    if (player->collision.orientationVector.y <= 0.8357f) {
        arg3.x = ((player->unk_206 / 182) * 0x78);
        arg3.z = (-(player->slopeAccel / 182) * 0xB4);
        decelerate_ai_player(player, 4.0f);
        func_8003DC40(player);
    } else {
        if ((((player->unk_094 / 18.0f) * 216.0f) > 20.0f) || ((player->effects & 0x10000) == 0x10000)) {
            if ((player->tyres[BACK_LEFT].surfaceType == ASPHALT) ||
                (player->tyres[BACK_RIGHT].surfaceType == ASPHALT) ||
                (player->tyres[FRONT_RIGHT].surfaceType == ASPHALT) ||
                (player->tyres[FRONT_LEFT].surfaceType == ASPHALT)) {
                arg3.x = ((player->unk_206 / 182) * 5);
            } else {
                arg3.x = ((player->unk_206 / 182) * 0x28);
            }
            arg3.z = (-(player->slopeAccel / 182) * 0x28);
        } else {
            arg3.x = 0.0f;
            arg3.z = (-(player->slopeAccel / 182) * 0x32);
        }
        if ((player->effects & 0x10000) != 0) {
            arg3.x = ((player->unk_206 / 182) * 0x78);
            arg3.z = (-(player->slopeAccel / 182) * 0xB4);
        }
    }
    arg3.y = 0.0f;
    mtxf_translate_vec3f_mat3(arg3, player->orientationMatrix);
}

void func_8003EE2C(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1.x * player->collision.surfaceDistance.z * 1;
    *arg6 += arg1.y * player->collision.surfaceDistance.z * 0.1;
    *arg7 += arg1.z * player->collision.surfaceDistance.z * 1;
    func_8002A5F4(arg1, *arg4, arg2, 0.5f, 2);
    if (player->collision.orientationVector.y <= 0.8357f) {
        arg3.x = ((player->unk_206 / 182) * 0x78);
        arg3.z = (-(player->slopeAccel / 182) * 0xB4);
        decelerate_ai_player(player, 4.0f);
        func_8003DC40(player);
    } else if ((((player->unk_094 / 18.0f) * 216.0f) > 20.0f) || ((player->effects & 0x10000) == 0x10000)) {
        arg3.x = ((player->unk_206 / 182) * 0x32);
        arg3.z = (-(player->slopeAccel / 182) * 0x3C);
    } else {
        arg3.x = 0.0f;
        arg3.z = (-(player->slopeAccel / 182) * 0x32);
    }
    arg3.y = 0.0f;
    mtxf_translate_vec3f_mat3(arg3, player->orientationMatrix);
}

void func_8003F138(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1.x * player->collision.surfaceDistance.z * 1;
    *arg6 += arg1.y * player->collision.surfaceDistance.z * 0.1;
    *arg7 += arg1.z * player->collision.surfaceDistance.z * 1;
    func_8002A5F4(arg1, *arg4, arg2, 0.5f, 2);
    if (player->surfaceType == GRASS) {
        player->unk_044 &= ~1;
    }
    if (player->collision.orientationVector.y <= 0.8357f) {
        arg3.x = ((player->unk_206 / 182) * 0xC8);
        arg3.z = (-(player->slopeAccel / 182) * 0xC8);
        decelerate_ai_player(player, 4.0f);
        player->unk_DAC = 0.5f;
        arg3.x = 0;
    } else if ((((player->unk_094 / 18.0f) * 216.0f) > 20.0f) || ((player->effects & 0x10000) == 0x10000)) {
        arg3.x = ((player->unk_206 / 182) * 0x78);
        arg3.z = (-(player->slopeAccel / 182) * 0x78);
        arg3.x = 0;
    } else {
        arg3.x = 0.0f;
        arg3.z = (-(player->slopeAccel / 182) * 0x32);
    }
    arg3.y = 0.0f;
    arg3.z = 0.0f;
    mtxf_translate_vec3f_mat3(arg3, player->orientationMatrix);
}

void func_8003F46C(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    arg1.x = -player->collision.orientationVector.x;
    arg1.y = -player->collision.orientationVector.y;
    arg1.z = -player->collision.orientationVector.z;
    if ((player->collision.orientationVector.y < 0.0f) && ((player->unk_0CA & 2) == 0)) {
        *arg5 += arg1.x * player->collision.surfaceDistance.z * 1;
        *arg6 += arg1.y * player->collision.surfaceDistance.z * 1;
        *arg7 += arg1.z * player->collision.surfaceDistance.z * 1;
        func_8002A5F4(arg1, *arg4, arg2, 1.2f, 0.0f);
        player->kartHopJerk = 0.0f;
        player->kartHopAcceleration = 0.0f;
        player->kartHopVelocity = 0.0f;
        return;
    } else {
        CM_SomeCollisionThing(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        // switch (gCurrentCourseId) {
        //     case COURSE_MARIO_RACEWAY:
        //         //func_8003E048(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        //         break;
        //     case COURSE_CHOCO_MOUNTAIN:
        //     case COURSE_KOOPA_BEACH:
        //         func_8003E37C(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        //         break;
        //     case COURSE_BOWSER_CASTLE:
        //         func_8003E6EC(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        //         break;
        //     case COURSE_LUIGI_RACEWAY:
        //         func_8003E9EC(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        //         break;
        //     case COURSE_WARIO_STADIUM:
        //         func_8003EE2C(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        //         break;
        //     case COURSE_DK_JUNGLE:
        //         func_8003F138(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        //         break;
        //     default:
        //         func_8003E048(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        //         break;
        // }
        if (player->effects & 0x10000) {
            player->unk_DAC = 0.5f;
        }
    }
}

void func_8003F734(Player* player, Vec3f arg1, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, f32* arg6) {
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f0_2;

    arg1.x = -player->collision.unk48.x;
    arg1.y = -player->collision.unk48.y;
    arg1.z = -player->collision.unk48.z;
    if (player->collision.unk48.y == 0) {
        *arg4 += arg1.x * player->collision.surfaceDistance.x * 1;
        *arg5 += arg1.y * player->collision.surfaceDistance.x * 0.1;
        *arg6 += arg1.z * player->collision.surfaceDistance.x * 1;
        if ((player->slopeAccel < 0) && (((player->unk_094 / 18.0f) * 216.0f) < 10.0f)) {
            func_8002A5F4(arg1, *arg3, arg2, 2.5f, 0);
        } else {
            func_8002A5F4(arg1, *arg3, arg2, 0.5f, 0);
        }
    } else if (player->collision.unk48.y <= 0.5) {
        *arg4 += arg1.x * player->collision.surfaceDistance.x * 1;
        *arg5 += arg1.y * player->collision.surfaceDistance.x * 0.1;
        *arg6 += arg1.z * player->collision.surfaceDistance.x * 1;
        func_8002A5F4(arg1, *arg3, arg2, 1, 0);
        if ((!(player->effects & 0x10000)) && ((player->effects & 8) == 0)) {
            arg2.y *= -1e-05;
        }
    } else {
        *arg4 += arg1.x * player->collision.surfaceDistance.x * 1;
        temp_f0_2 = player->collision.surfaceDistance.x * arg1.y;
        if (temp_f0_2 < 0) {
            *arg5 += temp_f0_2 * 0.1;
        } else {
            *arg5 += temp_f0_2 * 0;
        }
        *arg6 += arg1.z * player->collision.surfaceDistance.x * 1;
        func_8002A5F4(arg1, *arg3, arg2, 1.2f, 0);
        if ((!(player->effects & 0x10000)) && ((player->effects & 8) == 0)) {
            arg2.y *= -1e-05;
        }
    }
    player->effects &= ~0x10;
    temp_f12 = player->collision.surfaceDistance.x * arg1.x;
    temp_f14 = player->collision.surfaceDistance.x * arg1.z;
    if (((temp_f12 >= 0) && (temp_f14 >= 0)) || ((temp_f12 < 0) && (temp_f14 >= 0))) {
        temp_f0_2 = player->boundingBoxSize / 2;
        player->unk_218 = *arg4 - temp_f12 - temp_f0_2;
        player->unk_21C = *arg6 - temp_f14 - temp_f0_2;
    }
    if (((temp_f12 < 0) && (temp_f14 < 0)) || ((temp_f12 >= 0) && (temp_f14 < 0))) {
        temp_f0_2 = player->boundingBoxSize / 2;
        player->unk_218 = *arg4 + temp_f12 + temp_f0_2;
        player->unk_21C = *arg6 + temp_f14 + temp_f0_2;
    }
}

void func_8003FBAC(Player* player, Vec3f arg1, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, f32* arg6) {
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f14;

    arg1.x = -player->collision.unk54.x;
    arg1.y = -player->collision.unk54.y;
    arg1.z = -player->collision.unk54.z;
    if (player->collision.unk54.y == 0) {
        *arg4 += arg1.x * player->collision.surfaceDistance.y * 1;
        *arg5 += arg1.y * player->collision.surfaceDistance.y * 0.1;
        *arg6 += arg1.z * player->collision.surfaceDistance.y * 1;
        if ((player->slopeAccel < 0) && (((player->unk_094 / 18.0f) * 216.0f) < 10.0f)) {
            func_8002A5F4(arg1, *arg3, arg2, 1.5f, 0);
        } else {
            func_8002A5F4(arg1, *arg3, arg2, 0.5f, 0);
        }
    } else if (player->collision.unk54.y <= 0.5) {
        *arg4 += arg1.x * player->collision.surfaceDistance.y * 1;
        *arg5 += arg1.y * player->collision.surfaceDistance.y * 0.1;
        *arg6 += arg1.z * player->collision.surfaceDistance.y * 1;
        func_8002A5F4(arg1, *arg3, arg2, 1, 0);
        if ((!(player->effects & 0x10000)) && ((player->effects & 8) == 0)) {
            arg2.y *= -1e-05;
        }
    } else {
        *arg4 += arg1.x * player->collision.surfaceDistance.y * 1;
        temp_f0_2 = player->collision.surfaceDistance.y * arg1.y;
        if (temp_f0_2 < 0) {
            *arg5 += temp_f0_2 * 0.1;
        } else {
            *arg5 += temp_f0_2 * 0;
        }
        *arg6 += arg1.z * player->collision.surfaceDistance.y * 1;
        func_8002A5F4(arg1, *arg3, arg2, 1.2f, 0);
        if ((!(player->effects & 0x10000)) && ((player->effects & 8) == 0)) {
            arg2.y *= -1e-05;
        }
    }
    player->effects &= ~0x10;
    temp_f12 = player->collision.surfaceDistance.y * arg1.x;
    temp_f14 = player->collision.surfaceDistance.y * arg1.z;
    if (((temp_f12 >= 0) && (temp_f14 >= 0)) || ((temp_f12 >= 0) && (temp_f14 < 0))) {
        temp_f0_2 = player->boundingBoxSize / 2;
        player->unk_218 = *arg4 - temp_f12 - temp_f0_2;
        player->unk_21C = *arg6 - temp_f14 - temp_f0_2;
    }
    if (((temp_f12 < 0) && (temp_f14 >= 0)) || ((temp_f12 < 0) && (temp_f14 < 0))) {
        temp_f0_2 = player->boundingBoxSize / 2;
        player->unk_218 = *arg4 + temp_f12 + temp_f0_2;
        player->unk_21C = *arg6 + temp_f14 + temp_f0_2;
    }
}
