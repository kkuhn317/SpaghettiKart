#include <actor_types.h>
#include "main.h"
#include "code_800029B0.h"
#include "code_80091440.h"
#include "camera.h"
#include "math_util.h"
#include "collision.h"

void func_800914A0(void) {
    D_80152308 = gPlayerOneCopy->unk_006 + 7;
    if ((s32) D_8015F6F8 < D_80152308) {
        D_80152308 -= D_8015F6F8;
    }
}

UNUSED void func_800914E0(void) {
    UNUSED Vec3f sp64 = { 0.0f, -20.0f, 150.0f };
    UNUSED Vec3f sp58 = { 0.0f, -6.0f, 4.0f };
    Camera* camera = &cameras[0];
    struct ActorSpawnData* sp48 = D_8015F718[0];
    struct ActorSpawnData* tempData;

    s16 temp3 = (s16) D_80152308;
    s16 temp2 = (s16) gPlayerOneCopy->unk_006;
    s16 temp;
    f32 sp38;
    f32 sp34;
    f32 sp30;
    camera->up[0] = 0;
    camera->up[1] = 1;
    camera->up[2] = 0;

    if (1) {} // waa?

    temp = temp2 - temp3;
    if (temp == 7) {
        func_800914A0();
    } else if (temp < 14) {
        temp += (s16) D_8015F6F8;
        if (temp == 7) {
            func_800914A0();
        }
    }
    tempData = sp48 + D_80152308;

    camera->pos.x = (f32) (tempData->pos.x + 10);
    camera->pos.y = (f32) (tempData->pos.y + 7);
    camera->pos.z = (f32) (tempData->pos.z - 20);
    camera->lookAt.x = gPlayerOneCopy->pos.x;

    camera->lookAt.y = gPlayerOneCopy->pos.y;
    camera->lookAt.z = gPlayerOneCopy->pos.z;
    check_bounding_collision(&camera->collision, 20.0f, camera->pos.x, camera->pos.y, camera->pos.z);
    sp38 = camera->lookAt.x - camera->pos.x;
    sp34 = camera->lookAt.y - camera->pos.y;
    sp30 = camera->lookAt.z - camera->pos.z;
    camera->rot.y = atan2s(sp38, sp30);
    camera->rot.x = atan2s(sqrtf((sp38 * sp38) + (sp30 * sp30)), sp34);
    camera->rot.z = 0;
}
