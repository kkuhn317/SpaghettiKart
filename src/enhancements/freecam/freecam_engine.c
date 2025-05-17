#include <libultraship.h>
#include <macros.h>
#include <defines.h>
#include <common_structs.h>
#include "player_controller.h"
#include "code_80057C60.h"
#include "code_800029B0.h"
#include "code_80005FD0.h"
#include "racing/collision.h"
#include "main.h"
#include <camera.h>
#include "freecam_engine.h"

FreeCam freeCam;

#include <math.h>

f32 gDampValue = 0.99f;
f32 gRotDampValue = 0.96f;

// Update FreeCam state
void freecam_tick(Camera* camera, Vec3f forwardVector) {

    // Apply camera movement
    camera->pos.x += freeCam.velocity.x * gDeltaTime;
    camera->pos.y += freeCam.velocity.y * gDeltaTime;
    camera->pos.z += freeCam.velocity.z * gDeltaTime;

    // Damp the velocity back to zero over time (camera slowly comes to a stop)
    freeCam.velocity.x *= gDampValue;
    freeCam.velocity.y *= gDampValue;
    freeCam.velocity.z *= gDampValue;

    // Apply camera rotation
    camera->rot.x += freeCam.rotVelocity.x * gDeltaTime;
    camera->rot.y += freeCam.rotVelocity.y * gDeltaTime;
    camera->rot.z += freeCam.rotVelocity.z * gDeltaTime;

    // Damp the velocity back to zero over time (camera rot slowly comes to a stop)
    freeCam.rotVelocity.x *= gRotDampValue;
    freeCam.rotVelocity.y *= gRotDampValue;
    freeCam.rotVelocity.z *= gRotDampValue;

    // Update lookAt
    camera->lookAt.x = camera->pos.x + forwardVector.x;
    camera->lookAt.y = camera->pos.y + forwardVector.y;
    camera->lookAt.z = camera->pos.z + forwardVector.z;
}

void freecam_calculate_forward_vector_allow_rotation(Camera* camera, Vec3f forwardVector) {
    // Convert yaw from 0-65535 to degrees
    f32 pitch = (camera->rot.z / 65535.0f) * 360.0f;
    f32 yaw = (camera->rot.y / 65535.0f) * 360.0f;

    // Convert degrees to radians
    pitch = pitch * M_PI / 180.0f;
    yaw = yaw * M_PI / 180.0f;

    // Calculate the forward vector based on yaw, ignoring pitch to keep height unchanged
    forwardVector.x = -sinf(yaw);
    forwardVector.y = -sinf(pitch); // Keep the height unchanged
    forwardVector.z = cosf(yaw);
}

f32 gFreecamFollowFactor = 0.7;

void freecam_target_player(Camera* camera, Vec3f forwardVector) {
    // Apply damping to velocity
    freeCam.velocity.x *= gDampValue;
    freeCam.velocity.y *= gDampValue;
    freeCam.velocity.z *= gDampValue;

    // Calculate the direction from the player to the camera
    f32 dirX = gPlayers[fRankIndex].pos.x - camera->pos.x;
    f32 dirY = gPlayers[fRankIndex].pos.y - camera->pos.y;
    f32 dirZ = gPlayers[fRankIndex].pos.z - camera->pos.z;

    // Normalize the direction vector (if needed)
    f32 length = sqrtf(dirX * dirX + dirY * dirY + dirZ * dirZ);
    if (length > 0) {
        dirX /= length;
        dirY /= length;
        dirZ /= length;
    }

    // Update the camera's look-at direction
    camera->lookAt.x = camera->pos.x + dirX;
    camera->lookAt.y = camera->pos.y + dirY;
    camera->lookAt.z = camera->pos.z + dirZ;

    // Calculate the forward vector based on the updated look-at direction
    forwardVector.x = camera->lookAt.x - camera->pos.x;
    forwardVector.y = camera->lookAt.y - camera->pos.y;
    forwardVector.z = camera->lookAt.z - camera->pos.z;

    // Normalize the forward vector
    f32 forwardLength = sqrtf(forwardVector.x * forwardVector.x + forwardVector.y * forwardVector.y +
                              forwardVector.z * forwardVector.z);
    if (forwardLength > 0.0f) {
        forwardVector.x /= forwardLength;
        forwardVector.y /= forwardLength;
        forwardVector.z /= forwardLength;
    }

    // Store or return the forward vector as needed for subsequent movement calculations
    freeCam.forwardVector.x = forwardVector.x;
    freeCam.forwardVector.y = forwardVector.y;
    freeCam.forwardVector.z = forwardVector.z;

    // Movement
    freeCam.velocity.x += gPlayers[fRankIndex].velocity.x * gFreecamFollowFactor;
    freeCam.velocity.y += gPlayers[fRankIndex].velocity.y * gFreecamFollowFactor;
    freeCam.velocity.z += gPlayers[fRankIndex].velocity.z * gFreecamFollowFactor;
}

void freecam_get_player_from_character(s32 characterId) {
    for (size_t i = 0; i < NUM_PLAYERS; i++) {
        if (gPlayers[i].characterId == characterId) {
            fRankIndex = i;
        }
    }
}