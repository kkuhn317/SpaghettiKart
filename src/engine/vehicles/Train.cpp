
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
        pos = &gVehicle2DWaypoint[waypointOffset];
        set_vehicle_pos_waypoint(ptr1, pos, waypointOffset);
    }
    // Smaller offset for the tender
    waypointOffset += 3;
    pos = &gVehicle2DWaypoint[waypointOffset];
    set_vehicle_pos_waypoint(&this->Tender, pos, waypointOffset);
    waypointOffset += 4;
    pos = &gVehicle2DWaypoint[waypointOffset];
    set_vehicle_pos_waypoint(&Locomotive, pos, waypointOffset);

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
    origXPos = tempLocomotive->position.x;
    origZPos = tempLocomotive->position.z;
    trainCarYRot = update_vehicle_following_waypoint(
        tempLocomotive->position, (s16*) &tempLocomotive->waypointIndex, Speed);
    tempLocomotive->velocity.x = tempLocomotive->position.x - origXPos;
    tempLocomotive->velocity.z = tempLocomotive->position.z - origZPos;
    vec3s_set(trainCarRot, 0, trainCarYRot, 0);
    tempLocomotive->actorIndex = add_actor_to_empty_slot(tempLocomotive->position, trainCarRot,
                                                            tempLocomotive->velocity, ACTOR_TRAIN_ENGINE);

    tempTender = &Tender;
    if (tempTender->isActive == 1) {
        origXPos = tempTender->position.x;
        origZPos = tempTender->position.z;
        trainCarYRot = update_vehicle_following_waypoint(
            tempTender->position, (s16*) &tempTender->waypointIndex, Speed);
        tempTender->velocity.x = tempTender->position.x - origXPos;
        tempTender->velocity.z = tempTender->position.z - origZPos;
        vec3s_set(trainCarRot, 0, trainCarYRot, 0);
        tempTender->actorIndex = add_actor_to_empty_slot(tempTender->position, trainCarRot,
                                                            tempTender->velocity, ACTOR_TRAIN_TENDER);
    }

    for (size_t i = 0; i < PassengerCars.size(); i++) {
        tempPassengerCar = &PassengerCars[i];
        if (tempPassengerCar->isActive == 1) {
            origXPos = tempPassengerCar->position.x;
            origZPos = tempPassengerCar->position.z;
            trainCarYRot = update_vehicle_following_waypoint(tempPassengerCar->position,
                                                                (s16*) &tempPassengerCar->waypointIndex,
                                                                Speed);
            tempPassengerCar->velocity.x = tempPassengerCar->position.x - origXPos;
            tempPassengerCar->velocity.z = tempPassengerCar->position.z - origZPos;
            vec3s_set(trainCarRot, 0, trainCarYRot, 0);
            tempPassengerCar->actorIndex =
                add_actor_to_empty_slot(tempPassengerCar->position, trainCarRot, tempPassengerCar->velocity,
                                        ACTOR_TRAIN_PASSENGER_CAR);
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
    trainCarActor->pos.x = trainCar->position.x;
    trainCarActor->pos.y = trainCar->position.y;
    trainCarActor->pos.z = trainCar->position.z;
    if (gIsMirrorMode != 0) {
        trainCarActor->rot.y = -orientationY;
    } else {
        trainCarActor->rot.y = orientationY;
    }
    trainCarActor->velocity.x = trainCar->velocity.x;
    trainCarActor->velocity.z = trainCar->velocity.z;
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

    temp_f20 = Locomotive.position.x;
    temp_f22 = Locomotive.position.z;

    orientationYUpdate = update_vehicle_following_waypoint(
        Locomotive.position, (s16*) &Locomotive.waypointIndex, Speed);

    Locomotive.velocity.x = Locomotive.position.x - temp_f20;
    Locomotive.velocity.z = Locomotive.position.z - temp_f22;

    sync_train_components(&Locomotive, orientationYUpdate);

    if ((oldWaypointIndex != Locomotive.waypointIndex) &&
        ((Locomotive.waypointIndex == 0x00BE) ||
            (Locomotive.waypointIndex == 0x0140))) { // play crossing bell sound
        func_800C98B8(Locomotive.position, Locomotive.velocity,
                        SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x0E));
    } else if (random_int(100) == 0) { // play train whistle sound
        func_800C98B8(Locomotive.position, Locomotive.velocity,
                        SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x0D));
    }

    SomeFlags = set_vehicle_render_distance_flags(
        Locomotive.position, TRAIN_SMOKE_RENDER_DISTANCE, SomeFlags);
    // Renders locomotive smoke on all screens if any player is within range.
    if ((((s16) AnotherSmokeTimer % 5) == 0) && (SomeFlags != 0)) {
        smokePos[0] = Locomotive.position.x;
        smokePos[1] = (f32) ((f64) Locomotive.position.y + 65.0);
        smokePos[2] = (f32) ((f64) Locomotive.position.z + 25.0);
        adjust_position_by_angle(smokePos, Locomotive.position, orientationYUpdate);
        //spawn_train_smoke(Index, smokePos, 1.1f);
        AddSmoke(Index, smokePos, 1.1f);
    }

    car = &Tender;

    if (car->isActive == 1) {
        temp_f20 = car->position.x;
        temp_f22 = car->position.z;
        orientationYUpdate =
            update_vehicle_following_waypoint(car->position, (s16*) &car->waypointIndex, Speed);
        car->velocity.x = car->position.x - temp_f20;
        car->velocity.z = car->position.z - temp_f22;
        sync_train_components(car, orientationYUpdate);
    }

    for (j = 0; j < PassengerCars.size(); j++) {
        car = &PassengerCars[j];
        if (car->isActive == 1) {
            temp_f20 = car->position.x;
            temp_f22 = car->position.z;

            orientationYUpdate =
                update_vehicle_following_waypoint(car->position, (s16*) &car->waypointIndex, Speed);
            car->velocity.x = car->position.x - temp_f20;
            car->velocity.z = car->position.z - temp_f22;
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
            playerPosX = player->pos.x;
            playerPosZ = player->pos.z;
            trainCar = &Locomotive;
            x_dist = playerPosX - trainCar->position.x;
            z_dist = playerPosZ - trainCar->position.z;
            if ((x_dist > -100.0) && (x_dist < 100.0)) {
                if ((z_dist > -100.0) && (z_dist < 100.0)) {
                    if (func_80006018(trainCar->position.x, trainCar->position.z, trainCar->velocity.x,
                                        trainCar->velocity.z, 60.0f, 20.0f, playerPosX, playerPosZ) == 1) {
                        player->soundEffects |= REVERSE_SOUND_EFFECT;
                    }
                    trainCar = &Tender;
                    if (trainCar->isActive == 1) {
                        if (func_80006018(trainCar->position.x, trainCar->position.z, trainCar->velocity.x,
                                            trainCar->velocity.z, 30.0f, 20.0f, playerPosX, playerPosZ) == 1) {
                            player->soundEffects |= REVERSE_SOUND_EFFECT;
                        }
                    }
                }
            }

            for (size_t i = 0; i < PassengerCars.size(); i++) {
                trainCar = &PassengerCars[i];
                x_dist = playerPosX - trainCar->position.x;
                z_dist = playerPosZ - trainCar->position.z;
                if (trainCar->isActive == 1) {
                    if ((x_dist > -100.0) && (x_dist < 100.0)) {
                        if ((z_dist > -100.0) && (z_dist < 100.0)) {
                            if (func_80006018(trainCar->position.x, trainCar->position.z, trainCar->velocity.x,
                                                trainCar->velocity.z, 30.0f, 20.0f, playerPosX, playerPosZ) == 1) {
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
