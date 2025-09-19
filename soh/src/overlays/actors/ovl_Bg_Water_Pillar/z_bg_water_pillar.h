#ifndef Z_BG_WATER_PILLAR_H
#define Z_BG_WATER_PILLAR_H

#include <libultraship/libultra.h>
#include "global.h"
#include <overlays/actors/ovl_En_Boom/z_en_boom.h>

struct BgWaterPillar;

typedef void (*BgWaterPillarActionFunc)(struct BgWaterPillar*, PlayState*);

typedef struct BgWaterPillar {
    Actor actor;

    BgWaterPillarActionFunc actionFunc;
    s16 switchFlag;
    ColliderJntSph colliderJntSph;
    ColliderJntSphElement colliderElements[1];
    ColliderCylinder colliderCylinder;
    f32 sinRotation;
    f32 cosRotation;
    s16 unk_210;
    s16 unk_212;
    s16 unk_154;
    s8 bombiwaBankIndex;
} BgWaterPillar;

#ifdef __cplusplus
extern "C" {
#endif
void BgWaterPillar_Init(Actor* thisx, PlayState* play);
void BgWaterPillar_Destroy(Actor* thisx, PlayState* play);
void BgWaterPillar_Update(Actor* thisx, PlayState* play);
void BgWaterPillar_Draw(Actor* thisx, PlayState* play);
#ifdef __cplusplus
}
#endif

#endif
