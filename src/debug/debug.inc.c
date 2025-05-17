#include <libultraship.h>
#include <debug.h>
#include "all_variables.h"

#if DVDL

extern s32 gGlobalTimer;

/**
 * Edit this to edit what displays on the screen while the DVDL is active.
 * The Size of the structure array is calculated at compile time.
 */
variableWatchAttributes gMainVariableWatchList[] = {
    { "Global Timer: ", &gGlobalTimer, sizeof(gGlobalTimer), DISPLAY_DECIMAL_NUMBER | DISPLAY_SIGNED_NUMBER, 0, 0 },
    { "Actors: ", &gNumActors, sizeof(gNumActors), DISPLAY_DECIMAL_NUMBER, 0, 0 },
    { "Player Type: ", &gPlayers[0].type, sizeof(gPlayerOne->type), DISPLAY_HEXIDECIMAL_NUMBER, 0, 0 },
    { "X ", &gPlayers[0].pos.x, sizeof(gPlayerOne->pos.x), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Y ", &gPlayers[0].pos.y, sizeof(gPlayerOne->pos.y), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Z ", &gPlayers[0].pos.z, sizeof(gPlayerOne->pos.z), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Torque 9C: ", &gPlayers[0].currentSpeed, sizeof(gPlayerOne->currentSpeed), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Top Speed 214: ", &gPlayers[0].topSpeed, sizeof(gPlayerOne->topSpeed), DISPLAY_FLOAT_NUMBER, 0, 0 },
    { "Accel Offset C4: ", &gPlayers[0].slopeAccel, sizeof(gPlayerOne->slopeAccel), DISPLAY_SIGNED_NUMBER, 0, 0 },

};

#endif
