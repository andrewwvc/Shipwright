/*
 * File: z_bg_water_pillar.c
 * Overlay: ovl_Bg_Water_Piller
 * Description: Bombable Pillar In Zora's Domain
 */

#include "z_bg_water_pillar.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_spot16_obj/object_spot16_obj.h"
#include <overlays/actors/ovl_En_Bom/z_en_bom.h>

#define FLAGS (ACTOR_FLAG_UPDATE_WHILE_CULLED | ACTOR_FLAG_DRAW_WHILE_CULLED | ACTOR_FLAG_DRAGGED_BY_HOOKSHOT | ACTOR_FLAG_CAN_PRESS_SWITCH)

void BgWaterPillar_Init(Actor* thisx, PlayState* play);
void BgWaterPillar_Destroy(Actor* thisx, PlayState* play);
void BgWaterPillar_Update(Actor* thisx, PlayState* play);
void BgWaterPillar_Draw(Actor* thisx, PlayState* play);

void BgWaterPillar_WaitAction(BgWaterPillar* this, PlayState* play);

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 8, ICHAIN_STOP),
};

static InitChainEntry sInitChainBoulder[] = {
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

static Color_RGBAf sInnerColors[] = {
    { 255.0f, 255.0f, 255.0f, 255.0f },
};

static Color_RGBAf sOuterColors[] = {
    { 0.0f, 255.0f, 0.0f, 255.0f },
};

static f32 sBoulderChaff[] = { 66.0f, 51.0f, 48.0f, 36.0f, 21.0f };

static ColliderJntSphElementInit sJntSphElementsWPInit[] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x4FC1FFF6, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 50, 0 }, 288 }, 100 },
    },
};

static ColliderJntSphInit sJntSphWPInit = {
    {
        COLTYPE_HARD,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsWPInit,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000008, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 190, 160, 0, { 10, 0, 50 } },
};

static CollisionCheckInfoInit sCCInfoInit = { 0, 12, 60, MASS_HEAVY };

void BgWaterPillar_InitFlags(BgWaterPillar* this) {
    //this->switchFlag = (this->actor.params >> 8) & 0x3F;
    this->actor.params = this->actor.params & 0xFF;
}

void BgWaterPillar_InitSphere(BgWaterPillar* this, PlayState* play) {
    s32 pad;

    Collider_InitJntSph(play, &this->colliderJntSph);
    Collider_SetJntSph(play, &this->colliderJntSph, &this->actor, &sJntSphWPInit, this->colliderElements);
    this->colliderJntSph.elements[0].dim.worldSphere.center.x = this->actor.world.pos.x;
    this->colliderJntSph.elements[0].dim.worldSphere.center.y = this->actor.world.pos.y + 50.0f;
    this->colliderJntSph.elements[0].dim.worldSphere.center.z = this->actor.world.pos.z;
    this->colliderJntSph.elements[0].dim.worldSphere.radius = 120;
}

void BgWaterPillar_InitCylinder(BgWaterPillar* this, PlayState* play) {
    s32 pad;

    Collider_InitCylinder(play, &this->colliderCylinder);
    Collider_SetCylinder(play, &this->colliderCylinder, &this->actor, &sCylinderInit);
    this->colliderCylinder.dim.pos.x += (s16)this->actor.world.pos.x;
    this->colliderCylinder.dim.pos.y += (s16)this->actor.world.pos.y;
    this->colliderCylinder.dim.pos.z += (s16)this->actor.world.pos.z;
}

void BgWaterPillar_InitFuntions(BgWaterPillar* this) {
    this->actor.draw = BgWaterPillar_Draw;
    this->actionFunc = BgWaterPillar_WaitAction;
}

s32 BgWaterPillar_InitRock(BgWaterPillar* this, PlayState* play) {
    if (LINK_IS_ADULT || Flags_GetEventChkInf(EVENTCHKINF_22)) {
        osSyncPrintf("Spot16 obj 爆弾石 破壊済み\n");
        return false;
    }
    Actor_ProcessInitChain(&this->actor, sInitChainBoulder);
    Actor_SetScale(&this->actor, 0.4f);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    BgWaterPillar_InitSphere(this, play);
    BgWaterPillar_InitCylinder(this, play);
    this->sinRotation = Math_SinS(this->actor.shape.rot.y);
    this->cosRotation = Math_CosS(this->actor.shape.rot.y);
    BgWaterPillar_InitFuntions(this);
    return true;
}

void BgWaterPillar_Init(Actor* thisx, PlayState* play) {
    BgWaterPillar* this = (BgWaterPillar*)thisx;
    Player* player = GET_PLAYER(play);
    s16 shouldLive;

    BgWaterPillar_InitFlags(this);
    shouldLive = BgWaterPillar_InitRock(this, play);

    if (!shouldLive) {
        Actor_Kill(&this->actor);
        return;
    }
}

void BgWaterPillar_ShiftPlayer(BgWaterPillar* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 sinValue;
    s16 adjustedYawDiff;
    s32 yawDiff;
    s32 absYawDiff;

    if (this->actor.xzDistToPlayer < 130.0f && this->actor.yDistToPlayer < 160.0f &&
        this->actor.yDistToPlayer >= -10.0f) {
        yawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
        absYawDiff = ABS(yawDiff);

        adjustedYawDiff = absYawDiff - 0x3FFF;

        if (adjustedYawDiff > 0) {
            sinValue = Math_SinS(adjustedYawDiff) * this->actor.xzDistToPlayer;

            if (sinValue >= 0.0f) {
                player->actor.world.pos.x += sinValue * this->sinRotation;
                player->actor.world.pos.z += sinValue * this->cosRotation;
            } else {
                osSyncPrintf("Error 補正出来ない(%s %d)(arg_data 0x%04x)(hosei_angY %x)\n",
                             __FILE__, __LINE__, this->actor.params, adjustedYawDiff);
            }
        }
    }
}

void BgWaterPillar_BreakAction(BgWaterPillar* this, PlayState* play) {

    //func_808B5240(this, play);

    if (this->unk_154 == 56) {
        func_80078884(NA_SE_SY_CORRECT_CHIME);
    }

    if (this->unk_154 > 60) {
        Actor_Kill(&this->actor);
    }
}

void BgWaterPillar_SetActionBreak(BgWaterPillar* this) {
    this->unk_154 = 0;
    this->actor.draw = NULL;
    this->actionFunc = BgWaterPillar_BreakAction;
}

void BgWaterPillar_WaitAction(BgWaterPillar* this, PlayState* play) {
    s32 pad;

    BgWaterPillar_ShiftPlayer(this, play);
    //func_808B57E0(this, play);

    if (this->colliderCylinder.base.acFlags & AC_HIT) {
        this->colliderCylinder.base.acFlags &= ~AC_HIT;

        //func_808B561C(this, play);

        //OnePointCutscene_Init(play, 4180, 50, NULL, MAIN_CAM);

        //Flags_SetSwitch(play, this->switchFlag);
        Flags_SetEventChkInf(EVENTCHKINF_22);

        BgWaterPillar_SetActionBreak(this);
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderCylinder.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderJntSph.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderJntSph.base);
    }

    // if (mREG(64) == 1) {
    //     func_808B561C(this, play);
    //     mREG(64) = -10;
    // } else if (mREG(64) < 0) {
    //     mREG(64)++;
    // }
}

void BgWaterPillar_Destroy(Actor* thisx, PlayState* play) {
    BgWaterPillar* this = (BgWaterPillar*)thisx;

    // Boulder is intact so remove its collider
    Collider_DestroyJntSph(play, &this->colliderJntSph);
    Collider_DestroyCylinder(play, &this->colliderCylinder);
}

// Vec3f Vec3fNormalize(Vec3f vec) {
//     f32 norm = sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));

//     if (norm != 0.0f) {
//         vec.x /= norm;
//         vec.y /= norm;
//         vec.z /= norm;
//     } else {
//         vec.x = vec.y = vec.z = 0.0f;
//     }

//     return vec;
// }

static Vec3f D_80854A40 = { 0.0f, 40.0f, 45.0f };

static Vec3f D_808547A4 = { 0.0f, 0.5f, 0.0f };
static Vec3f D_808547B0 = { 0.0f, 0.5f, 0.0f };

static Color_RGBA8 D_808547BC = { 255, 255, 100, 255 };
static Color_RGBA8 D_808547C0 = { 255, 50, 0, 0 };


void BgWaterPillar_Update(Actor* thisx, PlayState* play) {
    BgWaterPillar* this = (BgWaterPillar*)thisx;

    this->unk_154++;
    if (this->actionFunc != NULL) {
        this->actionFunc(this, play);
    }
}

void BgWaterPillar_Draw(Actor* thisx, PlayState* play) {
    BgWaterPillar* this = (BgWaterPillar*)thisx;
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    // if (this->actor.params == 0xFF) {
        // The boulder is intact
        gSPDisplayList(POLY_OPA_DISP++, gDodongosCavernRock3DL);
    // } else {
    //     // The boulder is debris
    //     gSPSegment(POLY_OPA_DISP++, 0x06, play->objectCtx.status[this->bombiwaBankIndex].segment);
    //     gSPDisplayList(POLY_OPA_DISP++, this->dList);
    // }

    CLOSE_DISPS(play->state.gfxCtx);
}
