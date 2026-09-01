/*
 * File: z_en_yukabyun.c
 * Overlay: ovl_En_Yukabyun
 * Description: Flying floor tile
 */

#include "z_en_yukabyun.h"
#include "objects/object_yukabyun/object_yukabyun.h"
#include "soh/Enhancements/game-interactor/GameInteractor_Hooks.h"

#define FLAGS ACTOR_FLAG_UPDATE_CULLING_DISABLED

void EnYukabyun_Init(Actor* thisx, PlayState* play);
void EnYukabyun_Destroy(Actor* thisx, PlayState* play);
void EnYukabyun_Update(Actor* thisx, PlayState* play);
void EnYukabyun_Draw(Actor* thisx, PlayState* play);

void func_80B43A94(EnYukabyun* this, PlayState* play);
void func_80B43AD4(EnYukabyun* this, PlayState* play);
void func_80B43B6C(EnYukabyun* this, PlayState* play);

const ActorInit En_Yukabyun_InitVars = {
    ACTOR_EN_YUKABYUN,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_YUKABYUN,
    sizeof(EnYukabyun),
    (ActorFunc)EnYukabyun_Init,
    (ActorFunc)EnYukabyun_Destroy,
    (ActorFunc)EnYukabyun_Update,
    (ActorFunc)EnYukabyun_Draw,
    NULL,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_NO_PUSH | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x20000000, 0x00, 0x10 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_HARD,
        BUMP_ON,
        OCELEM_ON,
    },
    { 28, 8, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F(scale, 1, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 16, ICHAIN_STOP),
};

static void* D_80B43F64[] = { gFloorTileEnemyTopTex, gFloorTileEnemyBottomTex };

#define SPEED_REDUCTION_PER_HIT 1.0f
#define LAUNCH_SPEED (10.0f + (this->actor.params/4))
#define MIN_SPEED 5.0f
#define SPIN_TO_REDUCE ((0x2000/(LAUNCH_SPEED - (MIN_SPEED-1.0f)))*SPEED_REDUCTION_PER_HIT)

void EnYukabyun_Init(Actor* thisx, PlayState* play) {
    EnYukabyun* this = (EnYukabyun*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.4f);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    this->actor.params++;
    this->unk_152 = 0;
    this->unk_150 = (u8)(this->actor.params) * 0xA + 0x14;
    this->actionfunc = func_80B43A94;
}

void EnYukabyun_Destroy(Actor* thisx, PlayState* play) {
    EnYukabyun* this = (EnYukabyun*)thisx;
    Collider_DestroyCylinder(play, &this->collider);
}

void func_80B43A94(EnYukabyun* this, PlayState* play) {
    if (this->unk_150 != 0) {
        this->unk_150--;
    }
    if (this->unk_150 == 0) {
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_IGNORE_QUAKE;
        this->actionfunc = func_80B43AD4;
    }
}

void func_80B43AD4(EnYukabyun* this, PlayState* play) {
    this->unk_150 += 0xA0;
    this->actor.shape.rot.y += this->unk_150;
    if (this->unk_150 >= 0x2000) {
        f32 speed = LAUNCH_SPEED;
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        this->actor.speedXZ = speed;
        this->actionfunc = func_80B43B6C;
    }
    Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y + 30.0f, 1.0f);
    func_8002F974(&this->actor, NA_SE_EN_YUKABYUN_FLY - SFX_FLAG);
}

void func_80B43B6C(EnYukabyun* this, PlayState* play) {
    this->actor.shape.rot.y += this->unk_150;
    if (this->actor.xzDistToPlayer > 5000.0f) {
        Actor_Kill(&this->actor);
        return;
    }
    func_8002F974(&this->actor, NA_SE_EN_YUKABYUN_FLY - SFX_FLAG);
}

void EnYukabyun_Break(EnYukabyun* this, PlayState* play) {
    EffectSsHahen_SpawnBurst(play, &this->actor.world.pos, 8.0f, 0, 1300, 300, 15, OBJECT_YUKABYUN, 10,
                             gFloorTileEnemyFragmentDL);
    Actor_Kill(&this->actor);
    GameInteractor_ExecuteOnEnemyDefeat(&this->actor);
}

void EnYukabyun_Update(Actor* thisx, PlayState* play) {
    EnYukabyun* this = (EnYukabyun*)thisx;
    Player* player = GET_PLAYER(play);
    Vec3f icePos;

        // Freeze the trap if hit by ice arrows:
    if (((this->collider.base.acFlags & AC_HIT) != 0) && (this->collider.info.acHitInfo->toucher.dmgFlags & (DMG_ARROW_ICE))) {
        icePos = thisx->world.pos;
        this->collider.base.acFlags &= ~AC_HIT;
        Actor_SetColorFilter(thisx, 0, 250, 0, 250);
        icePos.y += 10.0f;
        icePos.z += 10.0f;
        EffectSsEnIce_SpawnFlyingVec3f(play, thisx, &icePos, 150, 150, 150, 250, 235, 245, 255, 1.8f);
        icePos.x += 10.0f;
        icePos.z -= 20.0f;
        EffectSsEnIce_SpawnFlyingVec3f(play, thisx, &icePos, 150, 150, 150, 250, 235, 245, 255, 1.8f);
        icePos.x -= 20.0f;
        EffectSsEnIce_SpawnFlyingVec3f(play, thisx, &icePos, 150, 150, 150, 250, 235, 245, 255, 1.8f);
    }

    if (thisx->colorFilterTimer != 0) {
        //Nothing happens when frozen except hammer/bomb smashing
        if ((this->collider.base.acFlags & AC_HIT) && (this->collider.info.acHitInfo->toucher.dmgFlags & (DMG_HAMMER | DMG_EXPLOSIVE))) {
            thisx->colorFilterTimer = 0;
            this->actor.flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
            SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 30, NA_SE_EN_OCTAROCK_ROCK);
            this->actionfunc = EnYukabyun_Break;
        }
    } else if (((this->collider.base.acFlags & AC_HIT) && !(this->collider.info.acHitInfo->toucher.dmgFlags & (DMG_HAMMER | DMG_EXPLOSIVE | DMG_ARROW_LIGHT))) ||
        ((this->collider.base.atFlags & AT_HIT) && (player->invincibilityTimer < 0))) {
        if ((this->unk_152 == 0) && (this->actionfunc == func_80B43B6C)) {
            s16 yDiff = (this->actor.yawTowardsPlayer - this->actor.world.rot.y);
            this->unk_152 = 1;
            if (-0x4000 < yDiff && yDiff < 0x4000)
                this->actor.world.rot.y = this->actor.yawTowardsPlayer + 0.75f*yDiff + 0x8000;
            else
                this->actor.world.rot.y = 0.5f*this->actor.world.rot.y + 0.5f*(this->actor.yawTowardsPlayer + 0x8000);
            this->actor.speedXZ -= 1.0f;
            this->unk_150 -= SPIN_TO_REDUCE;
        }
    } else if (((this->collider.base.atFlags & AT_HIT) && (this->collider.base.at->id == ACTOR_PLAYER) && (this->unk_152 == 0)) ||
                (this->collider.base.acFlags & AC_HIT) ||
                ((this->actionfunc == func_80B43B6C) && (this->actor.speedXZ < MIN_SPEED))) {
        if ((this->actionfunc == func_80B43AD4) && (this->collider.base.acFlags & AC_HIT) && (this->collider.info.acHitInfo->toucher.dmgFlags & DMG_HAMMER)) {
            this->unk_152 = 0;
            this->unk_150 = (u8)0x14;
            this->actionfunc = func_80B43A94;
            this->actor.world = this->actor.home;
            this->actor.shape.rot = this->actor.home.rot;
            this->actor.flags &= (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_IGNORE_QUAKE);
        } else {
            this->actor.flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
            SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 30, NA_SE_EN_OCTAROCK_ROCK);
            this->actionfunc = EnYukabyun_Break;
        }
    } else if ((this->actionfunc == func_80B43B6C) && (this->actor.bgCheckFlags & 8)) {
        s16 yawChange = (2*(s16)(this->actor.wallYaw + 0x8000) - this->actor.world.rot.y);
        this->actor.world.rot.y = yawChange + 0x8000;
        this->unk_152 = 0;
        this->actor.speedXZ -= 1.0f;
        this->unk_150 -= SPIN_TO_REDUCE;
    } else if ((this->collider.base.ocFlags1 & OC1_HIT)) {
        if (((this->collider.base.oc->id == ACTOR_EN_YUKABYUN) && (this->actor.speedXZ >= this->collider.base.oc->speedXZ)) ||
                ((this->collider.base.oc->id != ACTOR_EN_YUKABYUN) && (this->collider.base.oc->id != ACTOR_PLAYER))) {
            this->unk_152 = 0;
            s16 yawToCollision = (s16)(Math_FAtan2F(this->collider.base.oc->world.pos.x-this->actor.world.pos.x, this->collider.base.oc->world.pos.z-this->actor.world.pos.z) * (0x8000 / M_PI));
            s16 yDiff = (yawToCollision - this->actor.world.rot.y);
            if (-0x4000 < yDiff && yDiff < 0x4000)
                this->actor.world.rot.y = yawToCollision + 0.75f*yDiff + 0x8000;
            else
                this->actor.world.rot.y = 0.5f*this->actor.world.rot.y + 0.5f*(yawToCollision + 0x8000);
        }
    }

    this->collider.base.atFlags &= ~AT_HIT;
    this->collider.base.acFlags &= ~AC_HIT;
    this->collider.base.ocFlags1 &= ~OC1_HIT;

    if (thisx->colorFilterTimer == 0) {
        this->actionfunc(this, play);
        Actor_MoveXZGravity(&this->actor);
    }

    if (!(this->actionfunc == func_80B43A94 || this->actionfunc == EnYukabyun_Break)) {
        if (thisx->colorFilterTimer == 0) {
            Actor_UpdateBgCheckInfo(play, &this->actor, 5.0f, 20.0f, 8.0f, 5);
            this->actor.flags |= ACTOR_FLAG_SFX_FOR_PLAYER_BODY_HIT;

            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        }

        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        Collider_UpdateCylinder(&this->actor, &this->collider);
    }
    Actor_SetFocus(&this->actor, 4.0f);
}

void EnYukabyun_Draw(Actor* thisx, PlayState* play) {
    EnYukabyun* this = (EnYukabyun*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_80B43F64[0]));
    gSPMatrix(POLY_OPA_DISP++, MATRIX_NEWMTX(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gFloorTileEnemyDL);

    CLOSE_DISPS(play->state.gfxCtx);
}
