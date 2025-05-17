#include <libultraship.h>
#include <macros.h>
#include <common_structs.h>

#include "camera_junk.h"
#include "camera.h"
#include "math_util.h"
#include "ceremony_and_credits.h"
#include "main.h"

void update_camera_podium_ceremony(void) {
    Camera* camera;
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;

    camera = &cameras[0];
    func_80283648(camera);

    x_dist = camera->lookAt.x - camera->pos.x;
    y_dist = camera->lookAt.y - camera->pos.y;
    z_dist = camera->lookAt.z - camera->pos.z;
    camera->rot.x = atan2s(x_dist, z_dist);
    camera->rot.y = atan2s(sqrtf((x_dist * x_dist) + (z_dist * z_dist)), y_dist);
    camera->rot.z = 0;
}

// GP podium ceremony camera settings?
void init_camera_podium_ceremony(void) {
    cameras[0].pos.x = -3133.0f;
    cameras[0].pos.y = 19.0f;
    cameras[0].pos.z = -467.0f;
    cameras[0].lookAt.x = -3478.0f;
    cameras[0].lookAt.y = 21.0f;
    cameras[0].lookAt.z = -528.0f;
    cameras[0].up.x = 0.0f;
    cameras[0].up.y = 1.0f;
    cameras[0].up.z = 0.0f;
    gCameraZoom[0] = 40.0f;
    gScreenAspect = 1.33333333f;
    D_80150150 = 3.0f;
    D_8015014C = 6800.0f;
    init_cinematic_camera();
}
