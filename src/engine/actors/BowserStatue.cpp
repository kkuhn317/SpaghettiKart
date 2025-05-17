#include "BowserStatue.h"

#include <libultra/gbi.h>

extern "C" {
#include "common_structs.h"
#include "math_util.h"
#include "main.h"
#include "assets/bowsers_castle_data.h"
}

Vtx gBowserStatueVtx[717];
Gfx gBowserStatueGfx[162];

ABowserStatue::ABowserStatue(FVector pos, ABowserStatue::Behaviour behaviour) {
    Name = "Bowser Statue";
    Pos = pos;
    ABowserStatue::Behaviour _behaviour = behaviour;
}

void ABowserStatue::Tick() {
    switch(_behaviour) {
        case DEFAULT:
            break;
        case CRUSH:
            break;
    }
}

void ABowserStatue::Draw(Camera *camera) {
    Mat4 mtx;
    Vec3f pos;
    pos.x = Pos.x + 76;
    pos.y = Pos.y;
    pos.z = Pos.z + 1846;
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(mtx, pos, Rot);
    if (render_set_position(mtx, 0) != 0) {

        gSPDisplayList(gDisplayListHead++, gBowserStatueGfx);
    }
}

bool ABowserStatue::IsMod() { return true; }
