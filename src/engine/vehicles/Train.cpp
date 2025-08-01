
#include <libultraship.h>
#include <libultra/gbi.h>
#include "Train.h"
#include <vector>

extern "C" {
#include "macros.h"
#include "main.h"
#include "defines.h"
#include "code_80005FD0.h"
#include "vehicles.h"
#include "actors.h"
#include "math_util.h"
#include "sounds.h"
#include "update_objects.h"
#include "waypoints.h"
#include "code_80057C60.h"
#include "math_util_2.h"
#include "render_objects.h"
#include "assets/common_data.h"
//  #include "common_structs.h"
}

size_t ATrain::_count = 0;

ATrain::ATrain(ATrain::TenderStatus tender, size_t numCarriages, f32 speed, uint32_t waypoint) {
    Name = "Train";
    u16 waypointOffset;
    TrainCarStuff* ptr1;
    Path2D* pos;

    Index = _count;
    Speed = speed;

    // Set to the default value
    std::fill(SmokeParticles, SmokeParticles + 128, NULL_OBJECT_ID);

    for (size_t i = 0; i < numCarriages; i++) {
        PassengerCars.push_back(TrainCarStuff());
    }

    // outputs 160 or 392 depending on the train.
    // Wraps the value around to always output a valid waypoint.
    waypointOffset = waypoint;

    // 120.0f is about the maximum usable value
    for (size_t i = 0; i < PassengerCars.size(); i++) {
        waypointOffset += 4;
        ptr1 = &PassengerCars[i];
        pos = &gVehicle2DPathPoint[waypointOffset];
        set_vehicle_pos_path_point(ptr1, pos, waypointOffset);
    }
    // Smaller offset for the tender
    waypointOffset += 3;
    pos = &gVehicle2DPathPoint[waypointOffset];
    set_vehicle_pos_path_point(&this->Tender, pos, waypointOffset);
    waypointOffset += 4;
    pos = &gVehicle2DPathPoint[waypointOffset];
    set_vehicle_pos_path_point(&Locomotive, pos, waypointOffset);

    // Only use locomotive unless overwritten below.
    NumCars = LOCOMOTIVE_ONLY;

    // Fall back in-case someone tries to spawn a train with carriages but no tender; not allowed.
    if (numCarriages > 0) {
        tender = HAS_TENDER;
    }

    Tender.isActive = static_cast<bool>(tender);

    for (size_t i = 0; i < numCarriages; i++) {
        PassengerCars[i].isActive = 1;
    }

    NumCars = NUM_TENDERS + numCarriages;

    AnotherSmokeTimer = 0;

    TrainCarStuff* tempLocomotive;
    TrainCarStuff* tempTender;
    TrainCarStuff* tempPassengerCar;
    Vec3s trainCarRot;
    s16 trainCarYRot;
    f32 origXPos;
    f32 origZPos;

    tempLocomotive = &Locomotive;
    origXPos = tempLocomotive->position[0];
    origZPos = tempLocomotive->position[2];
    trainCarYRot =
        update_vehicle_following_path(tempLocomotive->position, (s16*) &tempLocomotive->waypointIndex, Speed);
    tempLocomotive->velocity[0] = tempLocomotive->position[0] - origXPos;
    tempLocomotive->velocity[2] = tempLocomotive->position[2] - origZPos;
    vec3s_set(trainCarRot, 0, trainCarYRot, 0);
    tempLocomotive->actorIndex =
        add_actor_to_empty_slot(tempLocomotive->position, trainCarRot, tempLocomotive->velocity, ACTOR_TRAIN_ENGINE);

    tempTender = &Tender;
    if (tempTender->isActive == 1) {
        origXPos = tempTender->position[0];
        origZPos = tempTender->position[2];
        trainCarYRot = update_vehicle_following_path(tempTender->position, (s16*) &tempTender->waypointIndex, Speed);
        tempTender->velocity[0] = tempTender->position[0] - origXPos;
        tempTender->velocity[2] = tempTender->position[2] - origZPos;
        vec3s_set(trainCarRot, 0, trainCarYRot, 0);
        tempTender->actorIndex =
            add_actor_to_empty_slot(tempTender->position, trainCarRot, tempTender->velocity, ACTOR_TRAIN_TENDER);
    }

    for (size_t i = 0; i < PassengerCars.size(); i++) {
        tempPassengerCar = &PassengerCars[i];
        if (tempPassengerCar->isActive == 1) {
            origXPos = tempPassengerCar->position[0];
            origZPos = tempPassengerCar->position[2];
            trainCarYRot = update_vehicle_following_path(tempPassengerCar->position,
                                                         (s16*) &tempPassengerCar->waypointIndex, Speed);
            tempPassengerCar->velocity[0] = tempPassengerCar->position[0] - origXPos;
            tempPassengerCar->velocity[2] = tempPassengerCar->position[2] - origZPos;
            vec3s_set(trainCarRot, 0, trainCarYRot, 0);
            tempPassengerCar->actorIndex = add_actor_to_empty_slot(
                tempPassengerCar->position, trainCarRot, tempPassengerCar->velocity, ACTOR_TRAIN_PASSENGER_CAR);
        }
    }

    _count++;
}

bool ATrain::IsMod() {
    return true;
}

void ATrain::SyncComponents(TrainCarStuff* trainCar, s16 orientationY) {
    struct TrainCar* trainCarActor;

    //! @todo: Change actorIndex to ptr to TrainCar actor
    trainCarActor = (struct TrainCar*) GET_ACTOR(trainCar->actorIndex);
    trainCarActor->pos[0] = trainCar->position[0];
    trainCarActor->pos[1] = trainCar->position[1];
    trainCarActor->pos[2] = trainCar->position[2];
    if (gIsMirrorMode != 0) {
        trainCarActor->rot[1] = -orientationY;
    } else {
        trainCarActor->rot[1] = orientationY;
    }
    trainCarActor->velocity[0] = trainCar->velocity[0];
    trainCarActor->velocity[2] = trainCar->velocity[2];
}

void ATrain::Tick() {
    f32 temp_f20;
    TrainCarStuff* car;
    u16 oldWaypointIndex;
    s16 orientationYUpdate;
    f32 temp_f22;
    s32 j;
    Vec3f smokePos;

    AnotherSmokeTimer += 1;

    oldWaypointIndex = (u16) Locomotive.waypointIndex;

    temp_f20 = Locomotive.position[0];
    temp_f22 = Locomotive.position[2];

    orientationYUpdate = update_vehicle_following_path(Locomotive.position, (s16*) &Locomotive.waypointIndex, Speed);

    Locomotive.velocity[0] = Locomotive.position[0] - temp_f20;
    Locomotive.velocity[2] = Locomotive.position[2] - temp_f22;

    sync_train_components(&Locomotive, orientationYUpdate);

    if ((oldWaypointIndex != Locomotive.waypointIndex) &&
        ((Locomotive.waypointIndex == 0x00BE) || (Locomotive.waypointIndex == 0x0140))) { // play crossing bell sound
        func_800C98B8(Locomotive.position, Locomotive.velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x0E));
    } else if (random_int(100) == 0) { // play train whistle sound
        func_800C98B8(Locomotive.position, Locomotive.velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x0D));
    }

    SomeFlags = set_vehicle_render_distance_flags(Locomotive.position, TRAIN_SMOKE_RENDER_DISTANCE, SomeFlags);
    // Renders locomotive smoke on all screens if any player is within range.
    if ((((s16) AnotherSmokeTimer % 5) == 0) && (SomeFlags != 0)) {
        smokePos[0] = Locomotive.position[0];
        smokePos[1] = (f32) ((f64) Locomotive.position[1] + 65.0);
        smokePos[2] = (f32) ((f64) Locomotive.position[2] + 25.0);
        adjust_position_by_angle(smokePos, Locomotive.position, orientationYUpdate);
        // spawn_train_smoke(Index, smokePos, 1.1f);
        AddSmoke(Index, smokePos, 1.1f);
    }

    car = &Tender;

    if (car->isActive == 1) {
        temp_f20 = car->position[0];
        temp_f22 = car->position[2];
        orientationYUpdate = update_vehicle_following_path(car->position, (s16*) &car->waypointIndex, Speed);
        car->velocity[0] = car->position[0] - temp_f20;
        car->velocity[2] = car->position[2] - temp_f22;
        sync_train_components(car, orientationYUpdate);
    }

    for (j = 0; j < PassengerCars.size(); j++) {
        car = &PassengerCars[j];
        if (car->isActive == 1) {
            temp_f20 = car->position[0];
            temp_f22 = car->position[2];

            orientationYUpdate = update_vehicle_following_path(car->position, (s16*) &car->waypointIndex, Speed);
            car->velocity[0] = car->position[0] - temp_f20;
            car->velocity[2] = car->position[2] - temp_f22;
            sync_train_components(car, orientationYUpdate);
        }
    }
}

void ATrain::VehicleCollision(s32 playerId, Player* player) {
    TrainCarStuff* trainCar;
    f32 playerPosX;
    f32 playerPosZ;
    f32 x_dist;
    f32 z_dist;
    s32 trainIndex;

    if (D_801631E0[playerId] != 1) {
        if (!(player->effects & 0x01000000)) {
            playerPosX = player->pos[0];
            playerPosZ = player->pos[2];
            trainCar = &Locomotive;
            x_dist = playerPosX - trainCar->position[0];
            z_dist = playerPosZ - trainCar->position[2];
            if ((x_dist > -100.0) && (x_dist < 100.0)) {
                if ((z_dist > -100.0) && (z_dist < 100.0)) {
                    if (is_collide_with_vehicle(trainCar->position[0], trainCar->position[2], trainCar->velocity[0],
                                                trainCar->velocity[2], 60.0f, 20.0f, playerPosX, playerPosZ) == 1) {
                        player->soundEffects |= REVERSE_SOUND_EFFECT;
                    }
                    trainCar = &Tender;
                    if (trainCar->isActive == 1) {
                        if (is_collide_with_vehicle(trainCar->position[0], trainCar->position[2], trainCar->velocity[0],
                                                    trainCar->velocity[2], 30.0f, 20.0f, playerPosX, playerPosZ) == 1) {
                            player->soundEffects |= REVERSE_SOUND_EFFECT;
                        }
                    }
                }
            }

            for (size_t i = 0; i < PassengerCars.size(); i++) {
                trainCar = &PassengerCars[i];
                x_dist = playerPosX - trainCar->position[0];
                z_dist = playerPosZ - trainCar->position[2];
                if (trainCar->isActive == 1) {
                    if ((x_dist > -100.0) && (x_dist < 100.0)) {
                        if ((z_dist > -100.0) && (z_dist < 100.0)) {
                            if (is_collide_with_vehicle(trainCar->position[0], trainCar->position[2],
                                                        trainCar->velocity[0], trainCar->velocity[2], 30.0f, 20.0f,
                                                        playerPosX, playerPosZ) == 1) {
                                player->soundEffects |= REVERSE_SOUND_EFFECT;
                            }
                        }
                    }
                }
            }
        }
    }
}

void ATrain::Draw(Camera* camera) {
}

s32 ATrain::AddSmoke(s32 trainIndex, Vec3f pos, f32 velocity) {
    s32 objectIndex = add_unused_obj_index(SmokeParticles, &NextParticlePtr, gObjectParticle2_SIZE);
    if (objectIndex != NULL_OBJECT_ID) {
        init_train_smoke(objectIndex, pos, velocity);
    }
    return objectIndex;
}
