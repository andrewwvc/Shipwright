/*
 * File: z_en_ru1.c
 * Overlay: En_Ru1
 * Description: Ruto (child)
 */
//#define INCLUDE_GAME_PRINTF
#include "z_en_ru1.h"
#include "objects/object_ru1/object_ru1.h"
#include "vt.h"

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_4 | ACTOR_FLAG_26)

void EnRu1_Init(Actor* thisx, PlayState* play);
void EnRu1_Destroy(Actor* thisx, PlayState* play);
void EnRu1_Update(Actor* thisx, PlayState* play);
void EnRu1_Draw(Actor* thisx, PlayState* play);

void EnRu1_Action0(EnRu1* this, PlayState* play);
void EnRu1_Action1(EnRu1* this, PlayState* play);
void EnRu1_Action2(EnRu1* this, PlayState* play);
void EnRu1_Action3(EnRu1* this, PlayState* play);
void EnRu1_Action4(EnRu1* this, PlayState* play);
void EnRu1_Action5(EnRu1* this, PlayState* play);
void EnRu1_Action6(EnRu1* this, PlayState* play);
void EnRu1_Action7(EnRu1* this, PlayState* play);
void EnRu1_Action8(EnRu1* this, PlayState* play);
void EnRu1_Action9(EnRu1* this, PlayState* play);
void EnRu1_Action10(EnRu1* this, PlayState* play);
void EnRu1_Action11(EnRu1* this, PlayState* play);
void EnRu1_Action12(EnRu1* this, PlayState* play);
void EnRu1_Action13(EnRu1* this, PlayState* play);
void EnRu1_Action14(EnRu1* this, PlayState* play);
void EnRu1_Action15(EnRu1* this, PlayState* play);
void EnRu1_Action16(EnRu1* this, PlayState* play);
void EnRu1_Action17(EnRu1* this, PlayState* play);
void EnRu1_Action18(EnRu1* this, PlayState* play);
void EnRu1_Action19(EnRu1* this, PlayState* play);
void EnRu1_Action20(EnRu1* this, PlayState* play);
void EnRu1_Action21(EnRu1* this, PlayState* play);
void EnRu1_Action22(EnRu1* this, PlayState* play);
void EnRu1_Action23(EnRu1* this, PlayState* play);
void EnRu1_Action24(EnRu1* this, PlayState* play);
void EnRu1_Action25(EnRu1* this, PlayState* play);
void EnRu1_Action26(EnRu1* this, PlayState* play);
void EnRu1_Action27(EnRu1* this, PlayState* play);
void EnRu1_Action28(EnRu1* this, PlayState* play);
void EnRu1_Action29(EnRu1* this, PlayState* play);
void EnRu1_Action30(EnRu1* this, PlayState* play);
void EnRu1_Action31(EnRu1* this, PlayState* play);
void EnRu1_Action32(EnRu1* this, PlayState* play);
void EnRu1_Action33(EnRu1* this, PlayState* play);
void EnRu1_Action34(EnRu1* this, PlayState* play);
void EnRu1_Action35(EnRu1* this, PlayState* play);
void EnRu1_Action36(EnRu1* this, PlayState* play);
void EnRu1_Action37(EnRu1* this, PlayState* play);
void EnRu1_Action38(EnRu1* this, PlayState* play);
void EnRu1_Action39(EnRu1* this, PlayState* play);
void EnRu1_Action40(EnRu1* this, PlayState* play);
void EnRu1_Action41(EnRu1* this, PlayState* play);
void EnRu1_Action42(EnRu1* this, PlayState* play);
void EnRu1_Action43(EnRu1* this, PlayState* play);
void EnRu1_Action44(EnRu1* this, PlayState* play);
void EnRu1_Action45(EnRu1* this, PlayState* play);
void EnRu1_NotAppearing(EnRu1* this, PlayState* play);
void EnRu1_DateStart(EnRu1* this, PlayState* play);
void EnRu1_DateInitialTalk(EnRu1* this, PlayState* play);
void EnRu1_DateDuringTalk(EnRu1* this, PlayState* play);
void EnRu1_GiveItem(EnRu1* this, PlayState* play);
void EnRu1_DateFinalTalk(EnRu1* this, PlayState* play);
void EnRu1_DateDuringFinalTalk(EnRu1* this, PlayState* play);
void EnRu1_DateEnd(EnRu1* this, PlayState* play);

void func_80AF0278(EnRu1* this, PlayState* play, s32 limbIndex, Vec3s* rot);
s32 EnRu1_DateConditionsMet();
s32 IsAfterRutosDate();
s32 EnRu1_IsOnIsland(Vec3f cPos);
s32 EnRu1_IsNotBehindTree(Vec3f cPos);
s32 EnRu1_IsPassedTree(Vec3f cPos);

void EnRu1_DrawNothing(EnRu1* this, PlayState* play);
void EnRu1_DrawOpa(EnRu1* this, PlayState* play);
void EnRu1_DrawXlu(EnRu1* this, PlayState* play);

static ColliderCylinderInitType1 sCylinderInit1 = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    { 0x00, { 0x00000000, 0x00, 0x00 }, { 0x00000000, 0x00, 0x00 }, 0x00, 0x00, 0x01 },
    { 25, 80, 0, { 0 } },
};

static ColliderCylinderInitType1 sCylinderInit2 = {
    {
        COLTYPE_HIT0,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    { 0x00, { 0x00000101, 0x00, 0x00 }, { 0x00000000, 0x00, 0x00 }, 0x01, 0x00, 0x01 },
    { 20, 30, 0, { 0 } },
};

static void* sEyeTextures[] = {
    gRutoChildEyeOpenTex,     gRutoChildEyeHalfTex,  gRutoChildEyeClosedTex,
    gRutoChildEyeRollLeftTex, gRutoChildEyeHalf2Tex, gRutoChildEyeHalfWithBlushTex,
};

static void* sMouthTextures[] = {
    gRutoChildMouthClosedTex,
    gRutoChildMouthFrownTex,
    gRutoChildMouthOpenTex,
};

static s32 sUnused = 0;

#include "z_en_ru1_cutscene_data.c" EARLY

static u32 D_80AF1938 = 0;

static EnRu1ActionFunc sActionFuncs[] = {
    EnRu1_Action0, EnRu1_Action1, EnRu1_Action2, EnRu1_Action3, EnRu1_Action4, EnRu1_Action5, EnRu1_Action6,
    EnRu1_Action7, EnRu1_Action8, EnRu1_Action9, EnRu1_Action10, EnRu1_Action11, EnRu1_Action12, EnRu1_Action13,
    EnRu1_Action14, EnRu1_Action15, EnRu1_Action16, EnRu1_Action17, EnRu1_Action18, EnRu1_Action19, EnRu1_Action20,
    EnRu1_Action21, EnRu1_Action22, EnRu1_Action23, EnRu1_Action24, EnRu1_Action25, EnRu1_Action26, EnRu1_Action27,
    EnRu1_Action28, EnRu1_Action29, EnRu1_Action30, EnRu1_Action31, EnRu1_Action32, EnRu1_Action33, EnRu1_Action34,
    EnRu1_Action35, EnRu1_Action36, EnRu1_Action37, EnRu1_Action38, EnRu1_Action39, EnRu1_Action40, EnRu1_Action41,
    EnRu1_Action42, EnRu1_Action43, EnRu1_Action44, EnRu1_Action45, EnRu1_NotAppearing, EnRu1_DateStart, EnRu1_DateInitialTalk,
    EnRu1_DateDuringTalk, EnRu1_GiveItem, EnRu1_DateFinalTalk, EnRu1_DateDuringFinalTalk, EnRu1_DateEnd,
};

static EnRu1PreLimbDrawFunc sPreLimbDrawFuncs[] = {
    func_80AF0278,
};

static Vec3f sMultVec = { 0.0f, 10.0f, 0.0f };

static EnRu1DrawFunc sDrawFuncs[] = {
    EnRu1_DrawNothing,
    EnRu1_DrawOpa,
    EnRu1_DrawXlu,
};

const ActorInit En_Ru1_InitVars = {
    ACTOR_EN_RU1,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_RU1,
    sizeof(EnRu1),
    (ActorFunc)EnRu1_Init,
    (ActorFunc)EnRu1_Destroy,
    (ActorFunc)EnRu1_Update,
    (ActorFunc)EnRu1_Draw,
    NULL,
};

void func_80AEAC10(EnRu1* this, PlayState* play) {
    s32 pad[5];

    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

void func_80AEAC54(EnRu1* this, PlayState* play) {
    s32 pad[5];

    Collider_UpdateCylinder(&this->actor, &this->collider2);
    if (this->unk_34C != 0) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider2.base);
    } else if (this->actor.xzDistToPlayer > 32.0f) {
        this->unk_34C = 1;
    }
}

void func_80AEACDC(EnRu1* this, PlayState* play) {
    s32 pad[5];

    Collider_UpdateCylinder(&this->actor, &this->collider2);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider2.base);
}

void func_80AEAD20(Actor* thisx, PlayState* play) {
    EnRu1* this = (EnRu1*)thisx;

    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinderType1(play, &this->collider, &this->actor, &sCylinderInit1);

    Collider_InitCylinder(play, &this->collider2);
    Collider_SetCylinderType1(play, &this->collider2, &this->actor, &sCylinderInit2);
}

void EnRu1_DestroyColliders(EnRu1* this, PlayState* play) {
    Collider_DestroyCylinder(play, &this->collider);
    Collider_DestroyCylinder(play, &this->collider2);
}

void func_80AEADD8(EnRu1* this) {
    this->unk_34C = 0;
}

u8 func_80AEADE0(EnRu1* this) {
    u8 params = this->actor.params >> 8;

    return params;
}

u8 func_80AEADF0(EnRu1* this) {
    s16 params = this->actor.params;

    return params;
}

void EnRu1_Destroy(Actor* thisx, PlayState* play) {
    EnRu1* this = (EnRu1*)thisx;

    D_80AF1938 = 0;
    EnRu1_DestroyColliders(this, play);
}

void EnRu1_UpdateEyes(EnRu1* this) {
    s32 pad[3];
    s16* blinkTimer = &this->blinkTimer;
    s16* eyeIndex = &this->eyeIndex;

    if (DECR(*blinkTimer) == 0) {
        *blinkTimer = Rand_S16Offset(60, 60);
    }

    *eyeIndex = *blinkTimer;
    if (*eyeIndex >= 3) {
        *eyeIndex = 0;
    }
}

void EnRu1_SetEyeIndex(EnRu1* this, s16 eyeIndex) {
    this->eyeIndex = eyeIndex;
}

void EnRu1_SetMouthIndex(EnRu1* this, s16 mouthIndex) {
    this->mouthIndex = mouthIndex;
}

void func_80AEAECC(EnRu1* this, PlayState* play) {
    f32* velocityY = &this->actor.velocity.y;
    f32 velocityYHeld = *velocityY;

    *velocityY = -4.0f;
    Actor_UpdateBgCheckInfo(play, &this->actor, 19.0f, 25.0f, 30.0f, 7);
    *velocityY = velocityYHeld;
}

s32 EnRu1_IsCsStateIdle(PlayState* play) {
    if (play->csCtx.state == CS_STATE_IDLE) {
        return true;
    }
    return false;
}

CsCmdActorAction* func_80AEAF58(PlayState* play, s32 npcActionIdx) {
    s32 pad[2];
    CsCmdActorAction* ret = NULL;

    if (!EnRu1_IsCsStateIdle(play)) {
        ret = play->csCtx.npcActions[npcActionIdx];
    }
    return ret;
}

s32 func_80AEAFA0(PlayState* play, u16 action, s32 npcActionIdx) {
    CsCmdActorAction* csCmdNPCAction = func_80AEAF58(play, npcActionIdx);

    if ((csCmdNPCAction != NULL) && (csCmdNPCAction->action == action)) {
        return true;
    }
    return false;
}

s32 func_80AEAFE0(PlayState* play, u16 action, s32 npcActionIdx) {
    CsCmdActorAction* csCmdNPCAction = func_80AEAF58(play, npcActionIdx);

    if ((csCmdNPCAction != NULL) && (csCmdNPCAction->action != action)) {
        return true;
    }
    return false;
}

s32 func_80AEB020(EnRu1* this, PlayState* play) {
    Actor* actorIt = play->actorCtx.actorLists[ACTORCAT_NPC].head;
    EnRu1* someEnRu1;

    while (actorIt != NULL) {
        if (actorIt->id == ACTOR_EN_RU1) {
            someEnRu1 = (EnRu1*)actorIt;
            if (someEnRu1 != this) {
                if ((someEnRu1->action == 31) || (someEnRu1->action == 32) || (someEnRu1->action == 24)) {
                    return true;
                }
            }
        }
        actorIt = actorIt->next;
    }
    return false;
}

BgBdanObjects* EnRu1_FindSwitch(PlayState* play) {
    Actor* actorIt = play->actorCtx.actorLists[ACTORCAT_BG].head;

    while (actorIt != NULL) {
        if (actorIt->id == ACTOR_BG_BDAN_OBJECTS && actorIt->params == 0) {
            return (BgBdanObjects*)actorIt;
        }
        actorIt = actorIt->next;
    }
    // "There is no stand"
    //osSyncPrintf(VT_FGCOL(RED) "お立ち台が無い!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    return NULL;
}

void func_80AEB0EC(EnRu1* this, s32 cameraSetting) {
    if (this->unk_28C != NULL) {
        this->unk_28C->cameraSetting = cameraSetting;
    }
}

s32 func_80AEB104(EnRu1* this) {
    if (this->unk_28C != NULL) {
        return this->unk_28C->cameraSetting;
    } else {
        return 0;
    }
}

Actor* func_80AEB124(PlayState* play) {
    Actor* actorIt = play->actorCtx.actorLists[ACTORCAT_BOSS].head;

    while (actorIt != NULL) {
        if ((actorIt->id == ACTOR_DEMO_EFFECT) && ((actorIt->params & 0xFF) == 0x15)) {
            return actorIt;
        }
        actorIt = actorIt->next;
    }
    return NULL;
}

s32 func_80AEB174(PlayState* play) {
    return (Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play);
}

s32 EnRu1_HandleMessageChoice(PlayState* play) {
    return (Message_GetState(&play->msgCtx) == TEXT_STATE_CHOICE) && Message_ShouldAdvance(play);
}

s32 func_80AEB1B4(PlayState* play) {
    return Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING;
}

void func_80AEB1D8(EnRu1* this) {
    this->action = 36;
    this->drawConfig = 0;
    this->actor.velocity.x = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.velocity.z = 0.0f;
    this->actor.speedXZ = 0.0f;
    this->actor.gravity = 0.0f;
    this->actor.minVelocityY = 0.0f;
    func_80AEB0EC(this, 0);
}

void func_80AEB220(EnRu1* this, PlayState* play) {
    if ((EnRu1_IsCsStateIdle(play)) && (this->actor.params == 0xA)) {
        func_80AEB1D8(this);
    }
}

void func_80AEB264(EnRu1* this, AnimationHeader* animation, u8 arg2, f32 transitionRate, s32 arg4) {
    s32 pad[2];
    AnimationHeader* animHeader = SEGMENTED_TO_VIRTUAL(animation);
    f32 frameCount = Animation_GetLastFrame(animHeader);
    f32 playbackSpeed;
    f32 unk0;
    f32 fc;

    if (arg4 == 0) {
        unk0 = 0.0f;
        fc = frameCount;
        playbackSpeed = 1.0f;
    } else {
        unk0 = frameCount;
        fc = 0.0f;
        playbackSpeed = -1.0f;
    }

    Animation_Change(&this->skelAnime, animHeader, playbackSpeed, unk0, fc, arg2, transitionRate);
}

s32 EnRu1_UpdateSkelAnime(EnRu1* this) {
    // why?
    if (this->action != 32) {
        return SkelAnime_Update(&this->skelAnime);
    } else {
        return SkelAnime_Update(&this->skelAnime);
    }
}

void func_80AEB364(EnRu1* this, PlayState* play) {
    this->skelAnime.moveFlags |= 1;
    AnimationContext_SetMoveActor(play, &this->actor, &this->skelAnime, 1.0f);
}

void func_80AEB3A4(EnRu1* this, PlayState* play) {
    this->skelAnime.moveFlags |= 1;
    func_80AEB364(this, play);
}

void func_80AEB3CC(EnRu1* this) {
    this->skelAnime.moveFlags &= ~0x1;
}

void func_80AEB3DC(EnRu1* this, PlayState* play) {
    func_80AEB264(this, &gRutoChildWaitHandsBehindBackAnim, 0, 0, 0);
    this->action = 0;
    this->drawConfig = 1;
    EnRu1_SetEyeIndex(this, 4);
    EnRu1_SetMouthIndex(this, 0);
}

CsCmdActorAction* func_80AEB438(PlayState* play) {
    return func_80AEAF58(play, 3);
}

s32 func_80AEB458(PlayState* play, u16 action) {
    return func_80AEAFA0(play, action, 3);
}

s32 func_80AEB480(PlayState* play, u16 action) {
    return func_80AEAFE0(play, action, 3);
}

void EnRu1_SpawnRipple(EnRu1* this, PlayState* play, s16 radiusMax, s16 life) {
    Vec3f pos;
    Actor* thisx = &this->actor;

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y + this->actor.yDistToWater;
    pos.z = this->actor.world.pos.z;
    EffectSsGRipple_Spawn(play, &pos, 100, radiusMax, life);
}

void func_80AEB50C(EnRu1* this, PlayState* play) {
    this->unk_270 += 1.0f;
    if (this->unk_270 >= kREG(3) + 10.0f) {
        EnRu1_SpawnRipple(this, play, kREG(1) + 500, 0);
        this->unk_270 = 0.0f;
    }
}

void func_80AEB59C(EnRu1* this, PlayState* play) {
    EnRu1_SpawnRipple(this, play, kREG(2) + 500, 0);
    EnRu1_SpawnRipple(this, play, kREG(2) + 500, kREG(3) + 10.0f);
    EnRu1_SpawnRipple(this, play, kREG(2) + 500, (kREG(3) + 10.0f) * 2.0f);
}

void EnRu1_SpawnSplash(EnRu1* this, PlayState* play) {
    Vec3f pos;

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y + this->actor.yDistToWater;
    pos.z = this->actor.world.pos.z;

    EffectSsGSplash_Spawn(play, &pos, 0, 0, 1, 0);
}

void func_80AEB6E0(EnRu1* this, PlayState* play) {
    SkelAnime* skelAnime = &this->skelAnime;

    if (skelAnime->baseTransl.y < skelAnime->jointTable[0].y) {
        skelAnime->moveFlags |= 3;
        AnimationContext_SetMoveActor(play, &this->actor, skelAnime, 1.0f);
    }
}

void func_80AEB738(EnRu1* this, PlayState* play) {
    SkelAnime* skelAnime = &this->skelAnime;

    skelAnime->baseTransl = skelAnime->jointTable[0];
    skelAnime->prevTransl = skelAnime->jointTable[0];
    if (skelAnime->baseTransl.y < skelAnime->jointTable[0].y) {
        skelAnime->moveFlags |= 3;
        AnimationContext_SetMoveActor(play, &this->actor, skelAnime, 1.0f);
    }
}

void func_80AEB7D0(EnRu1* this) {
    this->skelAnime.moveFlags &= ~0x3;
}

f32 func_80AEB7E0(CsCmdActorAction* csCmdNPCAction, PlayState* play) {
    s32 csCtxFrames = play->csCtx.frames;

    if ((csCtxFrames < csCmdNPCAction->endFrame) && (csCmdNPCAction->endFrame - csCmdNPCAction->startFrame > 0)) {
        return (Math_CosS(((csCtxFrames - csCmdNPCAction->startFrame) /
                           (f32)(csCmdNPCAction->endFrame - csCmdNPCAction->startFrame)) *
                          32768.0f) *
                -0.5f) +
               0.5f;
    }
    return 1.0f;
}

f32 func_80AEB87C(f32 arg0, s32 arg1, s32 arg2) {
    return (((f32)arg2 - arg1) * arg0) + arg1;
}

void func_80AEB89C(EnRu1* this, PlayState* play) {
    CsCmdActorAction* npcAction = func_80AEB438(play);
    s16 npcActionRotY;

    if (npcAction != NULL) {
        npcActionRotY = npcAction->rot.y;
        this->actor.shape.rot.y = npcActionRotY;
        this->actor.world.rot.y = npcActionRotY;
        this->actor.world.pos.x = npcAction->startPos.x;
        this->actor.world.pos.y = npcAction->startPos.y;
        this->actor.world.pos.z = npcAction->startPos.z;
    }
}

void func_80AEB914(EnRu1* this, PlayState* play) {
    func_80AEB89C(this, play);
}

void func_80AEB934(EnRu1* this, PlayState* play) {
    func_80AEB89C(this, play);
}

void func_80AEB954(EnRu1* this, PlayState* play) {
    func_80AEB6E0(this, play);
}

void func_80AEB974(EnRu1* this, PlayState* play) {
    Vec3f* thisPos;
    f32 sp30;
    CsCmdActorAction* csCmdNPCAction = func_80AEB438(play);
    s32 pad;

    if (csCmdNPCAction != NULL) {
        sp30 = func_80AEB7E0(csCmdNPCAction, play);
        thisPos = &this->actor.world.pos;
        thisPos->x = func_80AEB87C(sp30, csCmdNPCAction->startPos.x, csCmdNPCAction->endPos.x);
        thisPos->y = func_80AEB87C(sp30, csCmdNPCAction->startPos.y, csCmdNPCAction->endPos.y);
        thisPos->z = func_80AEB87C(sp30, csCmdNPCAction->startPos.z, csCmdNPCAction->endPos.z);
    }
}

void func_80AEBA0C(EnRu1* this, PlayState* play) {
    func_80AEB6E0(this, play);
}

void func_80AEBA2C(EnRu1* this, PlayState* play) {
    s32 pad;
    Vec3f* unk_364 = &this->unk_364;
    Vec3f* thisPos;
    f32 temp_ret_2;
    CsCmdActorAction* csCmdNPCAction = func_80AEB438(play);
    s32 pad2;

    if (csCmdNPCAction != NULL) {
        temp_ret_2 = func_80AEB7E0(csCmdNPCAction, play);
        thisPos = &this->actor.world.pos;
        thisPos->x = func_80AEB87C(temp_ret_2, unk_364->x, csCmdNPCAction->endPos.x);
        thisPos->y = func_80AEB87C(temp_ret_2, unk_364->y, csCmdNPCAction->endPos.y);
        thisPos->z = func_80AEB87C(temp_ret_2, unk_364->z, csCmdNPCAction->endPos.z);
    }
}

void func_80AEBAFC(EnRu1* this) {
    if (this->unk_298 == 0) {
        func_80078914(&this->actor.projectedPos, NA_SE_EV_DIVE_INTO_WATER);
        this->unk_298 = 1;
    }
}

void func_80AEBB3C(EnRu1* this) {
    if (Animation_OnFrame(&this->skelAnime, 5.0f)) {
        func_80078914(&this->actor.projectedPos, NA_SE_PL_FACE_UP);
    }
}

void func_80AEBB78(EnRu1* this) {
    SkelAnime* skelAnime = &this->skelAnime;

    if (Animation_OnFrame(skelAnime, 4.0f) || Animation_OnFrame(skelAnime, 13.0f) ||
        Animation_OnFrame(skelAnime, 22.0f) || Animation_OnFrame(skelAnime, 31.0f)) {
        func_80078914(&this->actor.projectedPos, NA_SE_PL_SWIM);
    }
}

void func_80AEBBF4(EnRu1* this) {
    if (Animation_OnFrame(&this->skelAnime, 8.0f)) {
        func_80078914(&this->actor.projectedPos, NA_SE_PL_SUBMERGE);
    }
}

void func_80AEBC30(PlayState* play) {
    Player* player;

    if (play->csCtx.frames == 0xCD) {
        player = GET_PLAYER(play);
        Audio_PlaySoundGeneral(NA_SE_EV_DIVE_INTO_WATER, &player->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }
}

void func_80AEBC84(EnRu1* this, PlayState* play) {
    if (play->csCtx.frames == 0x82) {
        func_80078914(&this->actor.projectedPos, NA_SE_VO_RT_LAUGH_0);
    }
}

void func_80AEBCB8(EnRu1* this, UNK_TYPE arg1) {
    if (arg1 != 0) {
        Animation_Change(&this->skelAnime, &gRutoChildSwimOnBackAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildSwimOnBackAnim), ANIMMODE_LOOP, -8.0f);
    }
}

void func_80AEBD1C(EnRu1* this, PlayState* play) {
    if (func_80AEB480(play, 2)) {
        this->action = 1;
        this->drawConfig = 0;
        func_80AEB914(this, play);
        func_80AEAECC(this, play);
        EnRu1_SpawnSplash(this, play);
        func_80AEB59C(this, play);
    }
}

void func_80AEBD94(EnRu1* this, PlayState* play) {
    s32 pad[2];
    f32 frameCount;

    if (func_80AEB480(play, 3)) {
        frameCount = Animation_GetLastFrame(&gRutoChildAnim_009060);
        func_80AEB934(this, play);
        func_80AEB738(this, play);
        Animation_Change(&this->skelAnime, &gRutoChildAnim_009060, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, 0.0f);
        this->action = 2;
        this->drawConfig = 1;
    }
}

void func_80AEBE3C(EnRu1* this, PlayState* play, s32 arg2) {
    s32 pad[2];
    f32 frameCount;

    if (arg2 != 0) {
        frameCount = Animation_GetLastFrame(&gRutoChildTreadWaterAnim);
        func_80AEB7D0(this);
        Animation_Change(&this->skelAnime, &gRutoChildTreadWaterAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP, -8.0f);
        this->action = 3;
    } else {
        func_80AEB954(this, play);
    }
}

void func_80AEBEC8(EnRu1* this, PlayState* play) {
    s32 pad[2];
    f32 frameCount;

    if (func_80AEB458(play, 6)) {
        frameCount = Animation_GetLastFrame(&gRutoChildTransitionToSwimOnBackAnim);
        func_80AEB738(this, play);
        Animation_Change(&this->skelAnime, &gRutoChildTransitionToSwimOnBackAnim, 1.0f, 0, frameCount, ANIMMODE_ONCE,
                         -8.0f);
        this->action = 4;
    }
}

void func_80AEBF60(EnRu1* this, PlayState* play) {
    if (func_80AEB480(play, 6)) {
        func_80AEB7D0(this);
        this->action = 5;
        this->unk_364 = this->actor.world.pos;
    } else {
        func_80AEBA0C(this, play);
    }
}

void func_80AEBFD8(EnRu1* this, PlayState* play) {
    CsCmdActorAction* csCmdNPCAction = func_80AEB438(play);
    f32 frameCount;
    u16 csCtxFrames;
    u16 endFrame;

    if (csCmdNPCAction != NULL) {
        csCtxFrames = play->csCtx.frames;
        endFrame = csCmdNPCAction->endFrame;
        if (csCtxFrames >= endFrame - 2) {
            frameCount = Animation_GetLastFrame(&gRutoChildTransitionFromSwimOnBackAnim);
            Animation_Change(&this->skelAnime, &gRutoChildTransitionFromSwimOnBackAnim, 1.0, 0, frameCount,
                             ANIMMODE_ONCE, -8.0f);
            this->action = 6;
        }
    }
}

void func_80AEC070(EnRu1* this, PlayState* play, UNK_TYPE arg2) {
    if ((func_80AEB458(play, 8)) && (arg2 != 0)) {
        Actor_Kill(&this->actor);
    }
}

void EnRu1_Action0(EnRu1* this, PlayState* play) {
    func_80AEB89C(this, play);
    EnRu1_UpdateSkelAnime(this);
    func_80AEBC84(this, play);
    func_80AEBC30(play);
    func_80AEBD1C(this, play);
}

void EnRu1_Action1(EnRu1* this, PlayState* play) {
    func_80AEBAFC(this);
    func_80AEBD94(this, play);
}

void EnRu1_Action2(EnRu1* this, PlayState* play) {
    s32 something = EnRu1_UpdateSkelAnime(this);

    func_80AEAECC(this, play);
    func_80AEBB3C(this);
    func_80AEBE3C(this, play, something);
}

void EnRu1_Action3(EnRu1* this, PlayState* play) {
    func_80AEB974(this, play);
    func_80AEAECC(this, play);
    EnRu1_UpdateSkelAnime(this);
    func_80AEB50C(this, play);
    func_80AEBEC8(this, play);
}

void EnRu1_Action4(EnRu1* this, PlayState* play) {
    s32 something;

    something = EnRu1_UpdateSkelAnime(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateEyes(this);
    func_80AEB50C(this, play);
    func_80AEBCB8(this, something);
    func_80AEBBF4(this);
    func_80AEBF60(this, play);
}

void EnRu1_Action5(EnRu1* this, PlayState* play) {
    s32 something;

    something = EnRu1_UpdateSkelAnime(this);
    func_80AEBA2C(this, play);
    func_80AEAECC(this, play);
    EnRu1_UpdateEyes(this);
    func_80AEB50C(this, play);
    func_80AEBCB8(this, something);
    func_80AEBB78(this);
    func_80AEBFD8(this, play);
}

void EnRu1_Action6(EnRu1* this, PlayState* play) {
    s32 something;

    something = EnRu1_UpdateSkelAnime(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateEyes(this);
    func_80AEB50C(this, play);
    func_80AEC070(this, play, something);
}

// Convenience function used so that Ruto always spawns in Jabu in rando, even after she's been kidnapped
// Equivalent to !(gSaveContext.infTable[20] & 0x20) in vanilla
bool shouldSpawnRuto() {
    // gSaveContext.infTable[20] & 0x40 check is to prevent Ruto from spawning during the short period of time when
    // she's on the Zora's Sapphire pedestal but hasn't been kidnapped yet (would result in multiple Rutos otherwise)
    return !(gSaveContext.infTable[20] & 0x20) || (gSaveContext.n64ddFlag && (gSaveContext.infTable[20] & 0x40));
}

void func_80AEC320(EnRu1* this, PlayState* play) {
    s8 actorRoom;

    if (!(gSaveContext.infTable[20] & 2)) {
        func_80AEB264(this, &gRutoChildWait2Anim, 0, 0, 0);
        this->action = 7;
        EnRu1_SetMouthIndex(this, 1);
    } else if ((gSaveContext.infTable[20] & 0x80) && !(gSaveContext.infTable[20] & 1) && shouldSpawnRuto()) {
        if (!func_80AEB020(this, play)) {
            func_80AEB264(this, &gRutoChildWait2Anim, 0, 0, 0);
            actorRoom = this->actor.room;
            this->action = 22;
            this->actor.room = -1;
            this->drawConfig = 0;
            this->roomNum1 = actorRoom;
            this->roomNum3 = actorRoom;
            this->roomNum2 = actorRoom;
        } else {
            Actor_Kill(&this->actor);
        }
    } else {
        Actor_Kill(&this->actor);
    }
}

void func_80AEC40C(EnRu1* this) {
    f32 unk_26C = this->unk_26C;

    if (unk_26C < 8.0f) {
        this->actor.speedXZ = (((kREG(3) * 0.01f) + 2.7f) / 8.0f) * unk_26C;
    } else {
        this->actor.speedXZ = (kREG(3) * 0.01f) + 2.7f;
    }
    this->actor.velocity.y = -1.0f;
    Actor_MoveForward(&this->actor);
}

void func_80AEC4CC(EnRu1* this) {
    this->actor.velocity.y = -1.0f;
    Actor_MoveForward(&this->actor);
}

void func_80AEC4F4(EnRu1* this) {
    f32* speedXZ = &this->actor.speedXZ;
    f32* unk_26C = &this->unk_26C;

    if (this->unk_26C < 8.0f) {
        *unk_26C += 1.0f;
        *speedXZ *= (8.0f - *unk_26C) / 8.0f;
        this->actor.velocity.y = -*unk_26C * (((kREG(4) * 0.01f) + 13.0f) / 8.0f);
    } else {
        *speedXZ = 0.0f;
        this->actor.velocity.y = -((kREG(4) * 0.01f) + 13.0f);
    }
    Actor_MoveForward(&this->actor);
}

s32 func_80AEC5FC(EnRu1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 thisPosZ = this->actor.world.pos.z;
    f32 playerPosZ = player->actor.world.pos.z;

    if ((playerPosZ - thisPosZ <= 265.0f) && (player->actor.world.pos.y >= this->actor.world.pos.y)) {
        return true;
    }
    return false;
}

void func_80AEC650(EnRu1* this) {
    s32 pad[2];

    if (this->unk_280 == 0) {
        if (Animation_OnFrame(&this->skelAnime, 2.0f) || Animation_OnFrame(&this->skelAnime, 7.0f)) {
            func_80078914(&this->actor.projectedPos, NA_SE_PL_WALK_DIRT);
        }
    }
}

void func_80AEC6B0(EnRu1* this) {
    func_80078914(&this->actor.projectedPos, NA_SE_EV_FALL_DOWN_DIRT);
    func_80078914(&this->actor.projectedPos, NA_SE_VO_RT_FALL);
}

void func_80AEC6E4(EnRu1* this, PlayState* play) {
    if ((func_80AEAFA0(play, 4, 3)) && (this->unk_280 == 0)) {
        Animation_Change(&this->skelAnime, &gRutoChildBringArmsUpAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildBringArmsUpAnim), ANIMMODE_ONCE, -8.0f);
        this->unk_280 = 1;
        func_80AEC6B0(this);
    }
}

void func_80AEC780(EnRu1* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);

    if ((func_80AEC5FC(this, play)) && (!Play_InCsMode(play)) && (!(player->stateFlags1 & 0x206000)) &&
        (player->actor.bgCheckFlags & 1)) {

        play->csCtx.segment = &D_80AF0880;
        gSaveContext.cutsceneTrigger = 1;
        player->linearVelocity = 0.0f;
        this->action = 8;
    }
}

void func_80AEC81C(EnRu1* this, PlayState* play) {
    CsCmdActorAction* csCmdNPCAction;
    s16 newRotY;

    if (func_80AEAFE0(play, 1, 3)) {
        csCmdNPCAction = play->csCtx.npcActions[3];
        this->actor.world.pos.x = csCmdNPCAction->startPos.x;
        this->actor.world.pos.y = csCmdNPCAction->startPos.y;
        this->actor.world.pos.z = csCmdNPCAction->startPos.z;
        newRotY = csCmdNPCAction->rot.y;
        this->actor.shape.rot.y = newRotY;
        this->actor.world.rot.y = newRotY;
        this->action = 9;
        this->drawConfig = 1;
    }
}

void func_80AEC8B8(EnRu1* this, PlayState* play) {
    if (func_80AEAFA0(play, 3, 3)) {
        Animation_Change(&this->skelAnime, &gRutoChildTurnAroundAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildTurnAroundAnim), ANIMMODE_ONCE, -8.0f);
        this->action = 10;
    }
}

void func_80AEC93C(EnRu1* this, UNK_TYPE arg1) {
    if (arg1 != 0) {
        Animation_Change(&this->skelAnime, &gRutoChildWalkAnim, 1.0f, 0, Animation_GetLastFrame(&gRutoChildWalkAnim),
                         ANIMMODE_LOOP, -8.0f);
        this->actor.world.rot.y += 0x8000;
        this->action = 0xB;
        this->unk_26C = 0.0f;
    }
}

void func_80AEC9C4(EnRu1* this) {
    this->unk_26C += 1.0f;
    if (this->unk_26C >= 8.0f) {
        this->action = 12;
        this->unk_26C = 0.0f;
        this->actor.velocity.y = -1.0f;
    }
}

void func_80AECA18(EnRu1* this) {
    if (!(this->actor.bgCheckFlags & 1)) {
        this->action = 13;
        this->unk_26C = 0.0f;
        this->actor.velocity.y = 0.0f;
    }
}

void func_80AECA44(EnRu1* this, PlayState* play) {
    if (func_80AEAFA0(play, 5, 3)) {
        gSaveContext.infTable[20] |= 2;
        this->action = 14;
    }
}

void EnRu1_Action7(EnRu1* this, PlayState* play) {
    func_80AEC780(this, play);
}

void EnRu1_Action8(EnRu1* this, PlayState* play) {
    func_80AEC81C(this, play);
}

void EnRu1_Action9(EnRu1* this, PlayState* play) {
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEC8B8(this, play);
}

void EnRu1_Action10(EnRu1* this, PlayState* play) {
    s32 something;

    something = EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEC93C(this, something);
}

void EnRu1_Action11(EnRu1* this, PlayState* play) {
    func_80AEC40C(this);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEC650(this);
    func_80AEC9C4(this);
}

void EnRu1_Action12(EnRu1* this, PlayState* play) {
    func_80AEC4CC(this);
    func_80AEC6E4(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEC650(this);
    func_80AECA18(this);
}

void EnRu1_Action13(EnRu1* this, PlayState* play) {
    func_80AEC4F4(this);
    func_80AEC6E4(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEC650(this);
    func_80AECA44(this, play);
}

void EnRu1_Action14(EnRu1* this, PlayState* play) {
    if (play->csCtx.state == CS_STATE_IDLE) {
        Actor_Kill(&this->actor);
    }
}

void func_80AECCB0(EnRu1* this, PlayState* play) {
    s32 pad;
    Vec3f* pos;
    s16 yawTowardsPlayer;
    f32 spawnX;
    f32 spawnY;
    f32 spawnZ;
    s32 pad2[2];

    yawTowardsPlayer = this->actor.yawTowardsPlayer;
    pos = &this->actor.world.pos;
    spawnX = ((kREG(1) + 12.0f) * Math_SinS(yawTowardsPlayer)) + pos->x;
    spawnY = pos->y;
    spawnZ = ((kREG(1) + 12.0f) * Math_CosS(yawTowardsPlayer)) + pos->z;
    this->blueWarp = (DoorWarp1*)Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_DOOR_WARP1,
                                                    spawnX, spawnY, spawnZ, 0, yawTowardsPlayer, 0, WARP_BLUE_RUTO);
}

void func_80AECDA0(EnRu1* this, PlayState* play) {
    func_80AEB264(this, &gRutoChildWaitHandsOnHipsAnim, 0, 0, 0);
    this->action = 15;
    this->actor.shape.yOffset = -10000.0f;
    EnRu1_SetEyeIndex(this, 5);
    EnRu1_SetMouthIndex(this, 2);
}

void func_80AECE04(EnRu1* this, PlayState* play) {
    this->actor.shape.yOffset += (250.0f / 3.0f);
}

void func_80AECE20(EnRu1* this, PlayState* play) {
    s32 pad2;
    Player* player = GET_PLAYER(play);
    Vec3f* playerPos = &player->actor.world.pos;
    s16 shapeRotY = player->actor.shape.rot.y;
    s32 pad;
    f32 unk_27C = this->unk_27C;
    Vec3f* pos = &this->actor.world.pos;

    pos->x = (Math_SinS(shapeRotY) * unk_27C) + playerPos->x;
    pos->y = playerPos->y;
    pos->z = (Math_CosS(shapeRotY) * unk_27C) + playerPos->z;
}

void func_80AECEB4(EnRu1* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);
    Vec3f* player_unk_450 = &player->unk_450;
    Vec3f* pos = &this->actor.world.pos;
    s16 shapeRotY = this->actor.shape.rot.y;

    player_unk_450->x = ((kREG(2) + 30.0f) * Math_SinS(shapeRotY)) + pos->x;
    player_unk_450->z = ((kREG(2) + 30.0f) * Math_CosS(shapeRotY)) + pos->z;
}

s32 func_80AECF6C(EnRu1* this, PlayState* play) {
    s16* shapeRotY;
    Player* player = GET_PLAYER(play);
    Player* otherPlayer;
    s16 temp_f16;
    f32 temp1;
    f32 temp2;
    s32 pad2[5];

    this->unk_26C += 1.0f;
    if ((player->actor.speedXZ == 0.0f) && (this->unk_26C >= 3.0f)) {
        otherPlayer = GET_PLAYER(play);
        player->actor.world.pos.x = otherPlayer->unk_450.x;
        player->actor.world.pos.y = otherPlayer->unk_450.y;
        player->actor.world.pos.z = otherPlayer->unk_450.z;
        shapeRotY = &player->actor.shape.rot.y;
        temp1 = this->actor.world.pos.x - player->actor.world.pos.x;
        temp2 = this->actor.world.pos.z - player->actor.world.pos.z;
        temp_f16 = Math_FAtan2F(temp1, temp2) * (0x8000 / M_PI);
        if (*shapeRotY != temp_f16) {
            Math_SmoothStepToS(shapeRotY, temp_f16, 0x14, 0x1838, 0x64);
            player->actor.world.rot.y = *shapeRotY;
        } else {
            return true;
        }
    }
    return false;
}

s32 func_80AED084(EnRu1* this, s32 state) {
    if (this->blueWarp != NULL && this->blueWarp->rutoWarpState == state) {
        return true;
    }
    return false;
}

void func_80AED0B0(EnRu1* this, s32 state) {
    if (this->blueWarp != NULL) {
        this->blueWarp->rutoWarpState = state;
    }
}

void func_80AED0C8(EnRu1* this, PlayState* play) {
    this->action = 16;
}

void func_80AED0D8(EnRu1* this, PlayState* play) {
    this->action = 17;
    this->drawConfig = 1;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
    func_80AECCB0(this, play);
}

void func_80AED110(EnRu1* this) {
    if (this->actor.shape.yOffset >= 0.0f) {
        this->action = 18;
        this->actor.shape.yOffset = 0.0f;
        func_80AED0B0(this, WARP_BLUE_RUTO_STATE_READY);
    }
}

void func_80AED154(EnRu1* this, PlayState* play) {
    if (func_80AED084(this, WARP_BLUE_RUTO_STATE_ENTERED)) {
        this->action = 0x13;
        this->unk_26C = 0.0f;
        func_80AECEB4(this, play);
    }
}

void func_80AED19C(EnRu1* this, s32 cond) {
    if (cond) {
        Animation_Change(&this->skelAnime, &gRutoChildTransitionHandsOnHipToCrossArmsAndLegsAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildTransitionHandsOnHipToCrossArmsAndLegsAnim), ANIMMODE_ONCE,
                         -8.0f);
        this->action = 20;
        func_80AED0B0(this, WARP_BLUE_RUTO_STATE_3);
    }
}

void func_80AED218(EnRu1* this, UNK_TYPE arg1) {
    if (func_80AED084(this, WARP_BLUE_RUTO_STATE_TALKING)) {
        if (arg1 != 0) {
            Animation_Change(&this->skelAnime, &gRutoChildWaitSittingAnim, 1.0f, 0,
                             Animation_GetLastFrame(&gRutoChildWaitSittingAnim), ANIMMODE_LOOP, -8.0f);
        }
    } else if (func_80AED084(this, WARP_BLUE_RUTO_STATE_WARPING)) {
        Animation_Change(&this->skelAnime, &gRutoChildWaitInBlueWarpAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildWaitInBlueWarpAnim), ANIMMODE_ONCE, -8.0f);
        this->action = 21;
        this->unk_27C = this->actor.xzDistToPlayer;
    }
}

void EnRu1_Action15(EnRu1* this, PlayState* play) {
    func_80AED0C8(this, play);
}

void EnRu1_Action16(EnRu1* this, PlayState* play) {
    func_80AED0D8(this, play);
}

void EnRu1_Action17(EnRu1* this, PlayState* play) {
    func_80AECE04(this, play);
    EnRu1_UpdateSkelAnime(this);
    func_80AED110(this);
}

void EnRu1_Action18(EnRu1* this, PlayState* play) {
    EnRu1_UpdateSkelAnime(this);
    func_80AED154(this, play);
}

void EnRu1_Action19(EnRu1* this, PlayState* play) {
    EnRu1_UpdateSkelAnime(this);
    func_80AED19C(this, func_80AECF6C(this, play));
}

void EnRu1_Action20(EnRu1* this, PlayState* play) {
    func_80AEAECC(this, play);
    func_80AED218(this, EnRu1_UpdateSkelAnime(this));
}

void EnRu1_Action21(EnRu1* this, PlayState* play) {
    func_80AECE20(this, play);
    func_80AEAECC(this, play);
    EnRu1_UpdateSkelAnime(this);
}

void func_80AED44C(EnRu1* this, PlayState* play) {
    s8 actorRoom;

    if ((gSaveContext.infTable[20] & 2) && shouldSpawnRuto() && !(gSaveContext.infTable[20] & 1) &&
        !(gSaveContext.infTable[20] & 0x80)) {
        if (!func_80AEB020(this, play)) {
            func_80AEB264(this, &gRutoChildWait2Anim, 0, 0, 0);
            actorRoom = this->actor.room;
            this->action = 22;
            this->actor.room = -1;
            this->roomNum1 = actorRoom;
            this->roomNum3 = actorRoom;
            this->roomNum2 = actorRoom;
        } else {
            Actor_Kill(&this->actor);
        }
    } else {
        Actor_Kill(&this->actor);
    }
}

void func_80AED4FC(EnRu1* this) {
    func_80078914(&this->actor.projectedPos, NA_SE_EV_LAND_DIRT);
}

void func_80AED520(EnRu1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Audio_PlaySoundGeneral(NA_SE_PL_PULL_UP_RUTO, &player->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                           &D_801333E8);
    func_80078914(&this->actor.projectedPos, NA_SE_VO_RT_LIFT);
}

void func_80AED57C(EnRu1* this) {
    if (this->actor.speedXZ != 0.0f) {
        func_80078914(&this->actor.projectedPos, NA_SE_VO_RT_THROW);
    }
}

void EnRu1_ExpressHurt() {
    if (EnRu1_DateConditionsMet()) {
        gSaveContext.infTable[20] |= 0x600;
    }
}

void func_80AED5B8(EnRu1* this) {
    func_80078914(&this->actor.projectedPos, NA_SE_VO_RT_CRASH);
    EnRu1_ExpressHurt();
}

void func_80AED5DC(EnRu1* this) {
    func_80078914(&this->actor.projectedPos, NA_SE_VO_RT_UNBALLANCE);
}

void func_80AED600(EnRu1* this) {
    func_80078914(&this->actor.projectedPos, NA_SE_VO_RT_DISCOVER);
}

s32 func_80AED624(EnRu1* this, PlayState* play) {
    s8 curRoomNum = play->roomCtx.curRoom.num;

    if (this->roomNum2 != curRoomNum) {
        Actor_Kill(&this->actor);
        return false;
    } else if (((this->roomNum1 != curRoomNum) || (this->roomNum2 != curRoomNum)) &&
               (this->actor.yDistToWater > kREG(16) + 50.0f) && (this->action != 33)) {
        this->action = 33;
        this->drawConfig = 2;
        this->alpha = 0xFF;
        this->unk_2A4 = 0.0f;
    }
    return true;
}

void func_80AED6DC(EnRu1* this, PlayState* play) {
    s8 curRoomNum = play->roomCtx.curRoom.num;

    this->roomNum2 = curRoomNum;
    this->unk_288 = 0.0f;
}

void func_80AED6F8(PlayState* play) {
    s8 curRoomNum;

    if ((!(gSaveContext.infTable[20] & 0x80))) {
        curRoomNum = play->roomCtx.curRoom.num;
        if (curRoomNum == 2) {
            gSaveContext.infTable[20] |= 0x80;
        }
    }
}

void func_80AED738(EnRu1* this, PlayState* play) {
    u32 temp_v0;

    if (func_80AED624(this, play)) {
        this->unk_2A4 += 1.0f;
        if (this->unk_2A4 < 20.0f) {
            temp_v0 = ((20.0f - this->unk_2A4) * 255.0f) / 20.0f;
            this->alpha = temp_v0;
            this->actor.shape.shadowAlpha = temp_v0;
        } else {
            Actor_Kill(&this->actor);
        }
    }
}

void func_80AED83C(EnRu1* this) {
    s32 pad[2];
    Vec3s* tempPtr;
    Vec3s* tempPtr2;

    tempPtr = &this->unk_374.unk_08;
    Math_SmoothStepToS(&tempPtr->x, 0, 0x14, 0x1838, 0x64);
    Math_SmoothStepToS(&tempPtr->y, 0, 0x14, 0x1838, 0x64);
    tempPtr2 = &this->unk_374.unk_0E;
    Math_SmoothStepToS(&tempPtr2->x, 0, 0x14, 0x1838, 0x64);
    Math_SmoothStepToS(&tempPtr2->y, 0, 0x14, 0x1838, 0x64);
}

void func_80AED8DC(EnRu1* this) {
    s32 temp_hi;
    s16* unk_2AC = &this->unk_2AC;
    s16* someY = &this->unk_374.unk_08.y;
    s16* unk_29E = &this->unk_29E;
    s32 pad[2];

    if (DECR(*unk_2AC) == 0) {
        *unk_2AC = Rand_S16Offset(0xA, 0x19);
        temp_hi = *unk_2AC % 5;
        if (temp_hi == 0) {
            this->unk_2B0 = 1;
        } else if (temp_hi == 1) {
            this->unk_2B0 = 2;
        } else {
            this->unk_2B0 = 0;
        }
        *unk_29E = 0;
    }

    if (this->unk_2B0 == 0) {
        Math_SmoothStepToS(unk_29E, 0 - *someY, 1, 0x190, 0x190);
        Math_SmoothStepToS(someY, 0, 3, ABS(*unk_29E), 0x64);
    } else if (this->unk_2B0 == 1) {
        Math_SmoothStepToS(unk_29E, -0x2AAA - *someY, 1, 0x190, 0x190);
        Math_SmoothStepToS(someY, -0x2AAA, 3, ABS(*unk_29E), 0x64);
    } else {
        Math_SmoothStepToS(unk_29E, 0x2AAA - *someY, 1, 0x190, 0x190);
        Math_SmoothStepToS(someY, 0x2AAA, 3, ABS(*unk_29E), 0x64);
    }
}

void func_80AEDAE0(EnRu1* this, PlayState* play) {
    DynaPolyActor* dynaPolyActor = DynaPoly_GetActor(&play->colCtx, this->actor.floorBgId);

    if (dynaPolyActor == NULL || dynaPolyActor->actor.id == ACTOR_EN_BOX) {
        this->actor.bgCheckFlags &= ~0x19;
    }
}

void func_80AEDB30(EnRu1* this, PlayState* play) {
    DynaPolyActor* dynaPolyActor;
    f32* velocityY;
    f32* speedXZ;
    f32* gravity;
    s16 wallYaw;
    s16 rotY;
    s32 temp_a1_2;
    s32 temp_a0;
    s32 phi_v1;

    if (this->actor.bgCheckFlags & 1) {
        velocityY = &this->actor.velocity.y;
        dynaPolyActor = DynaPoly_GetActor(&play->colCtx, this->actor.floorBgId);
        if (*velocityY <= 0.0f) {
            speedXZ = &this->actor.speedXZ;
            if (dynaPolyActor != NULL) {
                if (dynaPolyActor->actor.id != ACTOR_EN_BOX) {
                    *speedXZ = 0.0f;
                }
            } else {
                if (*speedXZ >= (kREG(27) * 0.01f) + 3.0f) {
                    *speedXZ *= (kREG(19) * 0.01f) + 0.8f;
                } else {
                    *speedXZ = 0.0f;
                }
            }
            gravity = &this->actor.gravity;
            if (dynaPolyActor != NULL) {
                if (dynaPolyActor->actor.id != ACTOR_EN_BOX) {
                    *velocityY = 0.0f;
                    this->actor.minVelocityY = 0.0f;
                    *gravity = 0.0f;
                } else {
                    *velocityY *= -1.0f;
                }
            } else {
                *velocityY *= -((kREG(20) * 0.01f) + 0.6f);
                if (*velocityY <= -*gravity * ((kREG(20) * 0.01f) + 0.6f)) {
                    *velocityY = 0.0f;
                    this->actor.minVelocityY = 0.0f;
                    *gravity = 0.0f;
                }
            }
            func_80AED4FC(this);
        }
    }
    if (this->actor.bgCheckFlags & 0x10) {
        speedXZ = &this->actor.speedXZ;
        velocityY = &this->actor.velocity.y;
        if (*speedXZ >= (kREG(27) * 0.01f) + 3.0f) {
            *speedXZ *= (kREG(19) * 0.01f) + 0.8f;
        } else {
            *speedXZ = 0.0f;
        }
        if (*velocityY >= 0.0f) {
            *velocityY *= -((kREG(20) * 0.01f) + 0.6f);
            func_80AED4FC(this);
        }
    }
    if (this->actor.bgCheckFlags & 8) {
        speedXZ = &this->actor.speedXZ;
        if (*speedXZ != 0.0f) {
            rotY = this->actor.world.rot.y;
            wallYaw = this->actor.wallYaw;
            temp_a0 = (wallYaw * 2) - rotY;
            temp_a1_2 = temp_a0 + 0x8000;
            if ((s16)((temp_a0 - wallYaw) + 0x8000) >= 0) {
                phi_v1 = (s16)(temp_a1_2 - wallYaw);
            } else {
                phi_v1 = -(s16)(temp_a1_2 - wallYaw);
            }
            if (phi_v1 < 0x4001) {
                if (*speedXZ >= (kREG(27) * 0.01f) + 3.0f) {
                    *speedXZ *= (kREG(21) * 0.01f) + 0.6f;
                } else {
                    *speedXZ = 0.0f;
                }
                this->actor.world.rot.y = temp_a1_2;
                func_80AED4FC(this);
                func_80AED5B8(this);
            }
        }
    }
}

void func_80AEDEF4(EnRu1* this, PlayState* play) {
    f32* speedXZ = &this->actor.speedXZ;
    DynaPolyActor* dynaPolyActor = DynaPoly_GetActor(&play->colCtx, this->actor.floorBgId);

    if (dynaPolyActor != NULL && dynaPolyActor->actor.id == ACTOR_EN_BOX) {
        if (*speedXZ != 0.0f) {
            *speedXZ *= 1.1f;
        } else {
            *speedXZ = 1.0f;
        }
    }
    if (*speedXZ >= (kREG(27) * 0.01f) + 3.0f) {
        *speedXZ *= (kREG(22) * 0.01f) + 0.98f;
    } else {
        *speedXZ = 0.0f;
    }
}

void func_80AEDFF4(EnRu1* this, PlayState* play) {
    func_80AEDB30(this, play);
    func_80AEDEF4(this, play);
    Actor_MoveForward(&this->actor);
}

void func_80AEE02C(EnRu1* this) {
    this->actor.velocity.x = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.velocity.z = 0.0f;
    this->actor.speedXZ = 0.0f;
    this->actor.gravity = 0.0f;
    this->actor.minVelocityY = 0.0f;
}

void func_80AEE050(EnRu1* this) {
    s32 pad;
    f32 sp28;
    f32 sp24;
    f32 temp_f10;
    EnRu1* thisx = this; // necessary to match

    if (this->unk_350 == 0) {
        if ((this->actor.minVelocityY == 0.0f) && (this->actor.speedXZ == 0.0f)) {
            this->unk_350 = 1;
            func_80AEE02C(this);
            this->unk_35C = 0;
            this->unk_358 = (this->actor.yDistToWater - 10.0f) * 0.5f;
            this->unk_354 = this->actor.world.pos.y + thisx->unk_358; // thisx only used here
        } else {
            this->actor.gravity = 0.0f;
            this->actor.minVelocityY *= 0.2f;
            this->actor.velocity.y *= 0.2f;
            if (this->actor.minVelocityY >= -0.1f) {
                this->actor.minVelocityY = 0.0f;
                this->actor.velocity.y = 0.0f;
            }
            this->actor.speedXZ *= 0.5f;
            if (this->actor.speedXZ <= 0.1f) {
                this->actor.speedXZ = 0.0f;
            }
            this->actor.velocity.x = Math_SinS(this->actor.world.rot.y) * this->actor.speedXZ;
            this->actor.velocity.z = Math_CosS(this->actor.world.rot.y) * this->actor.speedXZ;
            func_8002D7EC(&this->actor);
        }
    } else {
        if (this->unk_350 == 1) {
            if (this->unk_358 <= 1.0f) {
                func_80AEE02C(this);
                this->unk_350 = 2;
                this->unk_360 = 0.0f;
            } else {
                sp28 = this->unk_358;
                sp24 = this->unk_354;
                temp_f10 = Math_CosS(this->unk_35C) * -sp28;
                this->actor.world.pos.y = temp_f10 + sp24;
                this->unk_35C += 0x3E8;
                this->unk_358 *= 0.95f;
            }
        } else {
            this->unk_360 += 1.0f;
            if (this->unk_360 > 0.0f) {
                this->unk_350 = 3;
            }
        }
    }
}

s32 func_80AEE264(EnRu1* this, PlayState* play) {
    if (!Actor_ProcessTalkRequest(&this->actor, play)) {
        this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_3;
        if ((gSaveContext.infTable[20] & 8)) {
            this->actor.textId = 0x404E;
            func_8002F2F4(&this->actor, play);
        } else if (gSaveContext.infTable[20] & 4) {
            this->actor.textId = 0x404D;
            func_8002F2F4(&this->actor, play);
        } else {
            this->actor.textId = 0x404C;
            func_8002F2F4(&this->actor, play);
        }
        return false;
    }
    return true;
}

void func_80AEE2F8(EnRu1* this, PlayState* play) {
    DynaPolyActor* dynaPolyActor;
    s32 floorBgId;

    if ((this->actor.bgCheckFlags & 1) && (this->actor.floorBgId != BGCHECK_SCENE)) {
        floorBgId = this->actor.floorBgId;
        dynaPolyActor = DynaPoly_GetActor(&play->colCtx, floorBgId);
        if ((dynaPolyActor != NULL) && (dynaPolyActor->actor.id == ACTOR_BG_BDAN_SWITCH)) {
            if (((dynaPolyActor->actor.params >> 8) & 0x3F) == 0x38) {
                gSaveContext.infTable[20] |= 1;
                return;
            }
        }
    }
    gSaveContext.infTable[20] &= ~0x1;
}

s32 func_80AEE394(EnRu1* this, PlayState* play) {
    s32 pad[2];
    CollisionContext* colCtx;
    DynaPolyActor* dynaPolyActor;
    s32 floorBgId;

    if ((this->actor.bgCheckFlags & 1) && this->actor.floorBgId != BGCHECK_SCENE) {
        colCtx = &play->colCtx;
        floorBgId = this->actor.floorBgId; // necessary match, can't move this out of this block unfortunately
        dynaPolyActor = DynaPoly_GetActor(colCtx, floorBgId);
        if (dynaPolyActor != NULL && dynaPolyActor->actor.id == ACTOR_BG_BDAN_OBJECTS &&
            dynaPolyActor->actor.params == 0 && !Player_InCsMode(play) && play->msgCtx.msgLength == 0) {
            func_80AEE02C(this);
            play->csCtx.segment = &D_80AF10A4;
            gSaveContext.cutsceneTrigger = 1;
            this->action = 36;
            this->drawConfig = 0;
            this->unk_28C = (BgBdanObjects*)dynaPolyActor;
            this->actor.shape.shadowAlpha = 0;
            return true;
        }
    }
    return false;
}

void func_80AEE488(EnRu1* this, PlayState* play) {
    s8 curRoomNum;

    if (Actor_HasParent(&this->actor, play)) {
        curRoomNum = play->roomCtx.curRoom.num;
        this->roomNum3 = curRoomNum;
        this->action = 31;
        this->expressionState = 0;
        func_80AED520(this, play);
    } else if ((!func_80AEE394(this, play)) && (!(this->actor.bgCheckFlags & 1))) {
        this->actor.minVelocityY = -((kREG(24) * 0.01f) + 6.8f);
        this->actor.gravity = -((kREG(23) * 0.01f) + 1.3f);
        this->action = 28;
    }
}

void func_80AEE568(EnRu1* this, PlayState* play) {
    if (!func_80AEE394(this, play)) {
        if ((this->actor.bgCheckFlags & 1) && (this->actor.speedXZ == 0.0f) && (this->actor.minVelocityY == 0.0f)) {
            func_80AEE02C(this);
            func_8002F580(&this->actor, play);
            this->action = 27;
            this->expressionState = 0;
            func_80AEADD8(this);
        } else if (this->actor.yDistToWater > 0.0f) {
            this->action = 29;
            this->unk_350 = 0;
        }
    }
}

void func_80AEE628(EnRu1* this, PlayState* play) {
    s32 pad[2];
    s8 curRoomNum = play->roomCtx.curRoom.num;

    if (EnRu1_IsCsStateIdle(play)) {
        Animation_Change(&this->skelAnime, &gRutoChildSittingAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildSittingAnim), ANIMMODE_LOOP, -8.0f);
        gSaveContext.infTable[20] |= 0x10;
        this->action = 31;
        this->expressionState = 0;
    }
    this->roomNum3 = curRoomNum;
}

s32 func_80AEE6D0(EnRu1* this, PlayState* play) {
    s32 pad;
    s8 curRoomNum = play->roomCtx.curRoom.num;

    if (!(gSaveContext.infTable[20] & 0x10) && (func_80AEB124(play) != 0)) {
        if (!Player_InCsMode(play)) {
            Animation_Change(&this->skelAnime, &gRutoChildSeesSapphireAnim, 1.0f, 0,
                             Animation_GetLastFrame(&gRutoChildSquirmAnim), ANIMMODE_LOOP, -8.0f);
            func_80AED600(this);
            this->action = 34;
            this->unk_26C = 0.0f;
            play->csCtx.segment = &D_80AF1728;
            gSaveContext.cutsceneTrigger = 1;
        }
        this->roomNum3 = curRoomNum;
        return true;
    }
    this->roomNum3 = curRoomNum;
    return false;
}

void func_80AEE7C4(EnRu1* this, PlayState* play) {
    f32 frameCount;
    s32 pad[13];
    Player* player;
    f32* unk_370 = &this->unk_370;

    if (Actor_HasNoParent(&this->actor, play)) {
        frameCount = Animation_GetLastFrame(&gRutoChildSittingAnim);
        Animation_Change(&this->skelAnime, &gRutoChildSittingAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP, -8.0f);
        func_80AED6DC(this, play);
        this->actor.speedXZ *= (kREG(25) * 0.01f) + 1.0f;
        this->actor.velocity.y *= (kREG(26) * 0.01f) + 1.0f;
        this->actor.minVelocityY = -((kREG(24) * 0.01f) + 6.8f);
        this->actor.gravity = -((kREG(23) * 0.01f) + 1.3f);
        func_80AED57C(this);
        this->action = 28;
        *unk_370 = 0.0f;
        return;
    }

    if (func_80AEE6D0(this, play)) {
        *unk_370 = 0.0f;
        return;
    }

    player = GET_PLAYER(play);
    if (player->stateFlags2 & 0x10000000) {
        this->unk_370 += 1.0f;
        if (this->action != 32) {
            if (*unk_370 > 30.0f) {
                if (Rand_S16Offset(0, 3) == 0) {
                    frameCount = Animation_GetLastFrame(&gRutoChildSquirmAnim);
                    Animation_Change(&this->skelAnime, &gRutoChildSquirmAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP,
                                     -8.0f);
                    func_80AED5DC(this);
                    this->action = 32;
                }
                *unk_370 = 0.0f;
            }
        } else {
            if (*unk_370 > 50.0f) {
                frameCount = Animation_GetLastFrame(&gRutoChildSittingAnim);
                Animation_Change(&this->skelAnime, &gRutoChildSittingAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP, -8.0f);
                this->action = 31;
                this->expressionState = 0;
                *unk_370 = 0.0f;
            }
        }
    } else {
        frameCount = Animation_GetLastFrame(&gRutoChildSittingAnim);
        Animation_Change(&this->skelAnime, &gRutoChildSittingAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP, -8.0f);
        *unk_370 = 0.0f;
    }
}

s32 func_80AEEAC8(EnRu1* this, PlayState* play) {
    if (this->actor.bgCheckFlags & 1) {
        func_80AEE02C(this);
        func_8002F580(&this->actor, play);
        this->action = 27;
        this->expressionState = 0;
        func_80AEADD8(this);
        return true;
    }
    return false;
}

void func_80AEEB24(EnRu1* this, PlayState* play) {
    if ((func_80AEEAC8(this, play) == 0) && (this->unk_350 == 3)) {
        this->action = 30;
        this->timer = 0;
        func_80AEE02C(this);
        this->actor.gravity = -0.1f;
        this->actor.minVelocityY = -((kREG(18) * 0.1f) + 0.7f);
    }
}

void func_80AEEBB4(EnRu1* this, PlayState* play) {
    func_8002F580(&this->actor, play);
}

void EnRu1_ExpressDiscomfort(EnRu1* this, PlayState* play) {
    if (!this->expressionState) {
        func_80078914(&this->actor.projectedPos, NA_SE_VO_RT_CRASH);
        this->expressionState = 2;
        if (EnRu1_DateConditionsMet() && !(gSaveContext.infTable[20] & 0x600)) {
            gSaveContext.infTable[20] |= 0x200;
        }
    }
}

void EnRu1_DetectExpressHarm(EnRu1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    if (player->invincibilityTimer > 0 && !(this->expressionState & 1)) {
        this->expressionState = 1;
        func_80078914(&this->actor.projectedPos, NA_SE_VO_RT_UNBALLANCE);
        if (EnRu1_DateConditionsMet() && !(gSaveContext.infTable[20] & 0x400)) {
            gSaveContext.infTable[20] |= 0x200;
            gSaveContext.infTable[20] |= 0x400;
        }
    }
}

void EnRu1_DetectGroundState(EnRu1* this, PlayState* play) {
    if (this->actor.bgCheckFlags & 1) {
        u16 groundType = SurfaceType_GetSfx(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);
        u16 inWater = !!(this->actor.bgCheckFlags & 0x20);
        u16 isGroundHard = 0;
        osSyncPrintf("Ruto Floor SFX Type: %d",groundType);
        osSyncPrintf("Ruto is in Water: %d", inWater);
        switch (groundType) {
            case 2:
            case 10:
            case 11:
            isGroundHard = 1;
            break;
        }
        if (!inWater && isGroundHard)
            EnRu1_ExpressDiscomfort(this,play);
    }
}

void EnRu1_Action27(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAC54(this, play);
    func_80AEAECC(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    EnRu1_DetectExpressHarm(this,play);
    EnRu1_DetectGroundState(this, play);
    func_80AEEBB4(this, play);
    func_80AEE488(this, play);
    func_80AED624(this, play);
    func_80AEDAE0(this, play);
    if (gSaveContext.infTable[20] & 0x800) {
        if (this->timer == 0) {
            this->timer = 8;
            func_80AEB264(this, &gRutoChildWait2Anim, 0, -8, 0);
        }
        this->timer--;
        if (this->timer == 0) {
            this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_3;
            this->action = 51;
        }
    } else {
        this->timer = 0;
    }
}

void EnRu1_Action28(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEACDC(this, play);
    func_80AEAECC(this, play);
    func_80AEE2F8(this, play);
    func_80AEDFF4(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEE568(this, play);
    func_80AED624(this, play);
    func_80AEDAE0(this, play);
}

void EnRu1_Action29(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAECC(this, play);
    func_80AEE050(this);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEEB24(this, play);
    func_80AED624(this, play);
}

void EnRu1_Action30(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAECC(this, play);
    Actor_MoveForward(&this->actor);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    if (IsAfterRutosDate()) {
        if ((gSaveContext.infTable[20] & 0x100) && this->timer == 0) {
            this->timer = 32;
            gSaveContext.infTable[21] |= 0x8;
        }
        if (this->timer > 0) {
            this->timer--;
            this->drawConfig = 2;
            Math_ApproachS(&this->alpha, 0, 1, 10);
            if (this->timer == 0) {
                Actor_Kill(&this->actor);
            }
        } else {
            func_80AEEAC8(this, play);
        }
    } else {
        func_80AEEAC8(this, play);
    }
    func_80AED624(this, play);
    func_80AEDAE0(this, play);
}

void EnRu1_Action31(EnRu1* this, PlayState* play) {
    func_80AED8DC(this);
    EnRu1_UpdateSkelAnime(this);
    func_80AEAECC(this, play);
    func_80AEE2F8(this, play);
    EnRu1_UpdateEyes(this);
    EnRu1_DetectExpressHarm(this,play);
    func_80AED6F8(play);
    func_80AEE7C4(this, play);
}

void EnRu1_Action32(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    EnRu1_UpdateSkelAnime(this);
    func_80AEAECC(this, play);
    func_80AEE2F8(this, play);
    EnRu1_UpdateEyes(this);
    func_80AED6F8(play);
    func_80AEE7C4(this, play);
}

void EnRu1_Action33(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAECC(this, play);
    func_80AEDFF4(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AED738(this, play);
    func_80AED624(this, play);
}

void EnRu1_Action34(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    EnRu1_UpdateSkelAnime(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateEyes(this);
    func_80AEE628(this, play);
}

void EnRu1_Action35(EnRu1* this, PlayState* play) {
}

void func_80AEEF68(EnRu1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 something;

    this->unk_374.unk_18 = player->actor.world.pos;
    this->unk_374.unk_14 = kREG(16) - 3.0f;
    something = kREG(17) + 0xC;
    func_80034A14(&this->actor, &this->unk_374, something, 2);
}

void func_80AEEFEC(EnRu1* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 something;

    this->unk_374.unk_18 = player->actor.world.pos;
    this->unk_374.unk_14 = kREG(16) - 3.0f;
    something = kREG(17) + 0xC;
    func_80034A14(&this->actor, &this->unk_374, something, 4);
    this->actor.world.rot.y = this->actor.shape.rot.y;
}

void func_80AEF080(EnRu1* this) {
    if (Animation_OnFrame(&this->skelAnime, 11.0f)) {
        func_80078914(&this->actor.projectedPos, NA_SE_EV_LAND_DIRT);
    }
}

s32 func_80AEF0BC(EnRu1* this, PlayState* play) {
    s32 frameCount;

    if (gSaveContext.infTable[20] & 4) {
        frameCount = Animation_GetLastFrame(&gRutoChildSitAnim);
        Animation_Change(&this->skelAnime, &gRutoChildSitAnim, 1.0f, 0, frameCount, ANIMMODE_ONCE, -8.0f);
        play->msgCtx.msgMode = MSGMODE_PAUSED;
        this->action = 26;
        this->actor.flags &= ~(ACTOR_FLAG_0 | ACTOR_FLAG_3);
        return true;
    }
    return false;
}

void func_80AEF170(EnRu1* this, PlayState* play, s32 cond) {
    if (cond) {
        this->action = 25;
    }
}

void func_80AEF188(EnRu1* this, PlayState* play) {
    if (func_80AEB174(play) && !func_80AEF0BC(this, play)) {
        Message_CloseTextbox(play);
        gSaveContext.infTable[20] |= 4;
        this->action = 24;
    }
}

void func_80AEF1F0(EnRu1* this, PlayState* play, UNK_TYPE arg2) {
    if (arg2 != 0) {
        Animation_Change(&this->skelAnime, &gRutoChildSittingAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gRutoChildSittingAnim), ANIMMODE_LOOP, 0.0f);
        Message_CloseTextbox(play);
        gSaveContext.infTable[20] |= 8;
        func_80AED6DC(this, play);
        func_8002F580(&this->actor, play);
        this->action = 27;
        this->expressionState = 0;
        func_80AEADD8(this);
    }
}

void EnRu1_Action22(EnRu1* this, PlayState* play) {
    this->action = 23;
}

void EnRu1_Action23(EnRu1* this, PlayState* play) {
    this->action = 24;
    this->drawConfig = 1;
    this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_3;
}

void EnRu1_Action24(EnRu1* this, PlayState* play) {
    s32 cond;

    func_80AEEF68(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAC10(this, play);
    func_80AEAECC(this, play);
    cond = func_80AEE264(this, play);
    func_80AED624(this, play);
    func_80AEF170(this, play, cond);
}

void EnRu1_Action25(EnRu1* this, PlayState* play) {
    func_80AEEFEC(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEF188(this, play);
}

void EnRu1_Action26(EnRu1* this, PlayState* play) {
    s32 something;

    func_80AED83C(this);
    something = EnRu1_UpdateSkelAnime(this);
    func_80AEF080(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEF1F0(this, play, something);
}

void func_80AEF40C(EnRu1* this) {
    SkelAnime* skelAnime = &this->skelAnime;

    if (Animation_OnFrame(skelAnime, 2.0f) || Animation_OnFrame(skelAnime, 7.0f) ||
        Animation_OnFrame(skelAnime, 12.0f) || Animation_OnFrame(skelAnime, 18.0f) ||
        Animation_OnFrame(skelAnime, 25.0f) || Animation_OnFrame(skelAnime, 33.0f)) {
        func_80078914(&this->actor.projectedPos, NA_SE_PL_WALK_DIRT);
    }
}

void func_80AEF4A8(EnRu1* this, PlayState* play) {
    SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.projectedPos, 20, NA_SE_VO_RT_FALL);
}

void func_80AEF4E0(EnRu1* this) {
    if (Animation_OnFrame(&this->skelAnime, 5.0f)) {
        func_80078914(&this->actor.projectedPos, NA_SE_VO_RT_LAUGH_0);
    }
}

void func_80AEF51C(EnRu1* this) {
    func_80078914(&this->actor.projectedPos, NA_SE_VO_RT_THROW);
}

void func_80AEF540(EnRu1* this) {
    if (func_80AEB104(this) == 2) {
        EnRu1_SetEyeIndex(this, 3);
        EnRu1_SetMouthIndex(this, 2);
        if (this->skelAnime.mode != 2) {
            func_80AEB264(this, &gRutoChildShutterAnim, 2, -8.0f, 0);
            func_80AEF51C(this);
        }
    }
}

void func_80AEF5B8(EnRu1* this) {
    f32 curFrame;

    if (D_80AF1938 == 0) {
        curFrame = this->skelAnime.curFrame;
        if (curFrame >= 60.0f) {
            EnRu1_SetEyeIndex(this, 3);
            EnRu1_SetMouthIndex(this, 0);
            func_80AED57C(this);
            D_80AF1938 = 1;
        }
    }
}

void func_80AEF624(EnRu1* this, PlayState* play) {
    f32 frameCount;
    CsCmdActorAction* csCmdNPCAction;
    CsCmdActorAction* csCmdNPCAction2;
    s16 newRotTmp;

    if (func_80AEAFE0(play, 1, 3)) {
        frameCount = Animation_GetLastFrame(&gRutoChildWalkToAndHoldUpSapphireAnim);
        // this weird part with the redundant variable is necessary to match for some reason
        csCmdNPCAction2 = play->csCtx.npcActions[3];
        csCmdNPCAction = csCmdNPCAction2;
        this->actor.world.pos.x = csCmdNPCAction->startPos.x;
        this->actor.world.pos.y = csCmdNPCAction->startPos.y;
        this->actor.world.pos.z = csCmdNPCAction->startPos.z;
        newRotTmp = csCmdNPCAction->rot.x;
        this->actor.shape.rot.x = newRotTmp;
        this->actor.world.rot.x = newRotTmp;
        newRotTmp = csCmdNPCAction->rot.y;
        this->actor.shape.rot.y = newRotTmp;
        this->actor.world.rot.y = newRotTmp;
        newRotTmp = csCmdNPCAction->rot.z;
        this->actor.shape.rot.z = newRotTmp;
        this->actor.world.rot.z = newRotTmp;
        Animation_Change(&this->skelAnime, &gRutoChildWalkToAndHoldUpSapphireAnim, 1.0f, 0.0f, frameCount,
                         ANIMMODE_ONCE, 0.0f);
        func_80AEB3A4(this, play);
        this->action = 37;
        this->drawConfig = 1;
        this->actor.shape.shadowAlpha = 0xFF;
    }
}

void func_80AEF728(EnRu1* this, UNK_TYPE arg1) {
    if (arg1 != 0) {
        Animation_Change(&this->skelAnime, &gRutoChildHoldArmsUpAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gRutoChildHoldArmsUpAnim), ANIMMODE_LOOP, 0.0f);
        func_80AEB3CC(this);
        this->action = 38;
    }
}

void func_80AEF79C(EnRu1* this, PlayState* play) {
    if (func_80AEAFE0(play, 2, 3)) {
        Animation_Change(&this->skelAnime, &gRutoChildBringHandsDownAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildBringHandsDownAnim), ANIMMODE_ONCE, -8.0f);
        this->action = 39;
    }
}

void func_80AEF820(EnRu1* this, UNK_TYPE arg1) {
    if (arg1 != 0) {
        Animation_Change(&this->skelAnime, &gRutoChildWait2Anim, 1.0f, 0, Animation_GetLastFrame(&gRutoChildWait2Anim),
                         ANIMMODE_LOOP, -8.0f);
        this->action = 40;
    }
}

void func_80AEF890(EnRu1* this, PlayState* play) {
    s32 pad[2];
    s8 curRoomNum;

    if ((gSaveContext.sceneSetupIndex < 4) && (EnRu1_IsCsStateIdle(play))) {
        curRoomNum = play->roomCtx.curRoom.num;
        gSaveContext.infTable[20] |= 0x20;
        Flags_SetSwitch(play, func_80AEADE0(this));
        func_80AEB0EC(this, 1);
        this->action = 42;
        this->actor.room = curRoomNum;
    }
}

void func_80AEF930(EnRu1* this, PlayState* play) {
    if (func_80AEB104(this) == 3) {
        this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_3;
        this->actor.textId = 0x4048;
        Message_ContinueTextbox(play, this->actor.textId);
        func_80AEF4A8(this, play);
        this->action = 43;
        this->drawConfig = 0;
    }
}

void func_80AEF99C(EnRu1* this, PlayState* play) {
    if (func_80AEB1B4(play) != 0) {
        func_80AEB0EC(this, 4);
        Actor_Kill(&this->actor);
    }
}

void EnRu1_Action36(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEF624(this, play);
    func_80AEB220(this, play);
}

void EnRu1_Action37(EnRu1* this, PlayState* play) {
    s32 something;

    func_80AED83C(this);
    func_80AEB364(this, play);
    func_80AEAECC(this, play);
    something = EnRu1_UpdateSkelAnime(this);
    func_80AEF4E0(this);
    func_80AEF5B8(this);
    func_80AEF40C(this);
    func_80AEF728(this, something);
    func_80AEB220(this, play);
}

void EnRu1_Action38(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateSkelAnime(this);
    func_80AEF79C(this, play);
    func_80AEB220(this, play);
}

void EnRu1_Action39(EnRu1* this, PlayState* play) {
    s32 something;

    func_80AED83C(this);
    func_80AEAECC(this, play);
    something = EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEF820(this, something);
    func_80AEB220(this, play);
}

void EnRu1_Action40(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEF890(this, play);
    func_80AEB220(this, play);
}

void EnRu1_Action42(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEAECC(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEF540(this);
    func_80AEF930(this, play);
}

void EnRu1_Action43(EnRu1* this, PlayState* play) {
    func_80AED83C(this);
    func_80AEF99C(this, play);
}

void func_80AEFC54(EnRu1* this, PlayState* play) {
    if ((gSaveContext.infTable[20] & 0x20) && !(gSaveContext.infTable[20] & 0x40)) {
        func_80AEB264(this, &gRutoChildWait2Anim, 0, 0, 0);
        this->action = 41;
        this->unk_28C = EnRu1_FindSwitch(play);
        func_80AEB0EC(this, 1);
        this->actor.flags &= ~(ACTOR_FLAG_0 | ACTOR_FLAG_3);
    } else {
        Actor_Kill(&this->actor);
    }
}

void EnRu1_Action41(EnRu1* this, PlayState* play) {
    this->unk_28C = EnRu1_FindSwitch(play);
    if (this->unk_28C != NULL) {
        this->action = 42;
        this->drawConfig = 1;
        func_80AEB0EC(this, 1);
    }
}

void EnRu1_DateInfoReset() {
    gSaveContext.infTable[20] &= 0xFF;//Stores only the flags used for Ruto's status in the original game
    gSaveContext.infTable[21] = 0x0;
}

void DetectAndPerformDateInfoReset() {
    if (gSaveContext.RutoDateDay < 2 + gSaveContext.totalDays - DAYS_IN_CYCLE) {
        EnRu1_DateInfoReset();
    }
}

void EnRu1_SetupDate() {
    gSaveContext.infTable[21] |= 0x1;
    gSaveContext.infTable[21] &= ~0x2;
    //Assumes that the date night will be the night of the full moon, AKA the first day of the cycle
    gSaveContext.RutoDateDay = DAYS_IN_CYCLE*(1+(gSaveContext.totalDays-1)/DAYS_IN_CYCLE);
    if (gSaveContext.RutoDateDay < 0)
        gSaveContext.RutoDateDay = 0;//Handles the improbable case where you might date Ruto on the first night of the game
}

void EnRu1_CancelDate() {
    gSaveContext.infTable[21] &= ~0x1;
    gSaveContext.infTable[21] |= 0x2;
}

s32 EnRu1_DateConditionsMet() {
    return !!(gSaveContext.infTable[21] & 0x1) && IS_NIGHT && getDayOfCycle() == 0;//Night of the full moon
}

void EnRu1_InitiateDate() {
    gSaveContext.infTable[21] |= 0x0004;
    if (!(gSaveContext.infTable[20] & 0x7000))
        gSaveContext.infTable[20] |= 0x4000;//Records that you spoke to Ruto without being spotted

}

//This will be true if you are in a day of the cycle after the date has finished
s32 IsAfterRutosDate() {
    return !!(gSaveContext.infTable[21] & 0x1) && (gSaveContext.RutoDateDay < gSaveContext.totalDays);
}

//These functions deal with whether Ruto herself has acknowledged the start and/or end of the date
s32 EnRu1_HadDateStarted() {
    return (gSaveContext.infTable[21] & 0x0004) || (gSaveContext.infTable[20] & 0x7000);
}

s32 EnRu1_HadDateEnded() {
    return ((gSaveContext.infTable[20] & 0x800) || (gSaveContext.infTable[20] & 0x100));
}

s32 EnRu1_DetermineDatePreScore() {
    s32 score = 1000;

    if (!(gSaveContext.infTable[21] & 0x0004) || !(gSaveContext.infTable[20] & 0x7000)) {//You failed to even show up, you nonce
        score -= 1000;
    } else if (gSaveContext.infTable[20] & 0x3000 == 0x1000) {//You showed up late
        score -= 200;
    } else if (gSaveContext.infTable[20] & 0x3000 == 0x2000) {//You were early
        score -= 100;
    } else if ((gSaveContext.infTable[20] & 0x3000) == 0x0000) {//You talked to her before she noticed you
        score -= 50;
    }

    if (gSaveContext.infTable[20] & 0x4000) {//You were hiding behind the pillars
        score -= 100;
    }

    if (gSaveContext.infTable[21] & 0x40) {//The owl saw Ruto in Lake Hylia on the day
        score -= 50;
    }

    if (!(gSaveContext.infTable[21] & 0x80)) {//You weren't close enough to her at sunrise
        score -= 50;
    }

    if (gSaveContext.infTable[20] & 0x8000) {//You left Ruto
        score -= 500;
    }

    if (!(gSaveContext.infTable[20] & 0x800) || (gSaveContext.infTable[20] & 0x100)) {//You failed to finish the date in time
        score -= 500;
    }

    if (gSaveContext.infTable[20] & 0x600 == 0x600) {//You hurt Ruto
        score -= 300;
    } else if (gSaveContext.infTable[20] & 0x600 == 0x400) {//You let her get scared
        score -= 200;
    } else if (gSaveContext.infTable[20] & 0x600 == 0x200) {//You made her unconfortable
        score -= 100;
    }

    return score;
}

s32 EnRu1_DetermineDateScore() {
    s32 score = EnRu1_DetermineDatePreScore();

    if (gSaveContext.infTable[21] & 0x10) {//Other Zoras saw you in the domain on the day
        score -= 100;
    }

    if (gSaveContext.infTable[21] & 0x20) {//Other Zoras saw you in Lake Hylia on the day
        score -= 100;
    }

    return score;
}

s32 EnRu1_DateConversation(EnRu1* this, PlayState* play) {
    if (!Actor_ProcessTalkRequest(&this->actor, play)) {
        u16 RutoMsg = GetTextID("ruto");
        this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_3;
        if (!(gSaveContext.infTable[21] & 0x800)) {
            if (!(gSaveContext.infTable[21] & 0x0004)) {
                if (gSaveContext.infTable[20] & 0x8000)
                    this->actor.textId = RutoMsg+16;
                else if ((gSaveContext.infTable[20] & 0x3000) == 0x1000)
                    this->actor.textId = RutoMsg+0;
                else if ((gSaveContext.infTable[20] & 0x3000) == 0x2000)
                    this->actor.textId = RutoMsg+13;
                else if ((this->dateProgress & 0x8) || ((gSaveContext.infTable[20] & 0x3000) == 0x0000))
                    this->actor.textId = RutoMsg+15;
                else
                    this->actor.textId = RutoMsg+14;
                func_8002F2F4(&this->actor, play);
            } else {
                this->actor.textId = RutoMsg+1;
                func_8002F2F4(&this->actor, play);
            }
        } else {
            this->actor.textId = RutoMsg+25;
            func_8002F2F4(&this->actor, play);
        }
        return false;
    }
    return true;
}

void func_80AEFD38(EnRu1* this, PlayState* play) {
    DetectAndPerformDateInfoReset();
    if ((gSaveContext.eventChkInf[3] & 0x80) && LINK_IS_CHILD && !(EnRu1_DateConditionsMet())) {
        func_80AEB264(this, &gRutoChildWait2Anim, 0, 0, 0);
        this->actor.flags &= ~ACTOR_FLAG_4;
        this->action = 44;
        this->drawConfig = 1;
        if (getDayOfCycle() == DAYS_IN_CYCLE-1) {
            this->actor.home.pos.x =  this->actor.world.pos.x = this->actor.prevPos.x = 740;
        }
    } else {
        Actor_Kill(&this->actor);
    }
}

s32 func_80AEFDC0(EnRu1* this, PlayState* play) {
    if (!Actor_ProcessTalkRequest(&this->actor, play)) {
        u16 RutoMsg = GetTextID("ruto");
        this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_3;
        this->actor.textId = Text_GetFaceReaction(play, 0x1F);
        if (this->actor.textId == 0) {
            if (IsAfterRutosDate()) {
                if ((!(gSaveContext.infTable[21] & 0x0004) && !(gSaveContext.infTable[20] & 0x7000)) || (gSaveContext.infTable[21] & 0x800))//You failed to show up or talk to her before sunrise
                    this->actor.textId = RutoMsg+20;
                else if (EnRu1_DetermineDateScore() == 1000) {
                    if (!(gSaveContext.infTable[21] & 0x8)) {//If you failed to give her a final talk
                        this->actor.textId = RutoMsg+24;
                    } else {
                        if (gSaveContext.infTable[19] & 0x1) {
                            this->actor.textId = RutoMsg+21;
                        } else {
                            this->actor.textId = RutoMsg+18;
                        }
                    }
                } else if (EnRu1_DetermineDatePreScore() == 1000) {
                    if (!(gSaveContext.infTable[21] & 0x8))
                        this->actor.textId = RutoMsg+24;
                    else
                        this->actor.textId = RutoMsg+17;
                } else {
                    this->actor.textId = RutoMsg+22;
                }
            } else if (gSaveContext.infTable[21] & 0x1)
                this->actor.textId = RutoMsg+3;
            else if (gSaveContext.infTable[21] & 0x2)
                this->actor.textId = RutoMsg+4;
            else if (getDayOfCycle() == DAYS_IN_CYCLE-1) {
                this->actor.textId = RutoMsg+2;
            } else {
                this->actor.textId = 0x402C;
            }
        }
        func_8002F2F4(&this->actor, play);
        return false;
    }
    return true;
}

s32 func_80AEFE38(EnRu1* this, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
        this->actor.flags &= ~(ACTOR_FLAG_0 | ACTOR_FLAG_3);
        return true;
    } else if (EnRu1_HandleMessageChoice(play)) {
        u16 RutoMsg = GetTextID("ruto");
        switch (play->msgCtx.choiceIndex) {
            case 0:
                this->actor.textId = RutoMsg+3;
                EnRu1_SetupDate();
                break;

            case 1:
                this->actor.textId = RutoMsg+4;
                EnRu1_CancelDate();
                break;
        }
        Message_ContinueTextbox(play, this->actor.textId);
        return false;
    } else if (func_80AEB174(play)) {
        u16 RutoMsg = GetTextID("ruto");
        if (this->actor.textId == RutoMsg+18) {
            func_8002F434(&this->actor, play, GI_HEART_PIECE, this->actor.xzDistToPlayer + 1.0f, fabsf(this->actor.yDistToPlayer) + 1.0f);
            Message_CloseTextbox(play);
            gSaveContext.infTable[19] |= 0x1;
            this->action = 50;
            return false;
        }
        return true;
    }
    return false;
}

//Action50
void EnRu1_GiveItem(EnRu1* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->action = 44;
    } else {
        func_8002F434(&this->actor, play, GI_HEART_PIECE, this->actor.xzDistToPlayer + 1.0f, fabsf(this->actor.yDistToPlayer) + 1.0f);
    }
}

void func_80AEFE84(EnRu1* this, PlayState* play, s32 cond) {
    if (cond) {
        this->action = 45;
    }
}

void func_80AEFE9C(EnRu1* this, PlayState* play) {
    if (func_80AEFE38(this, play)) {
        this->action = 44;
    }
}

void EnRu1_Action44(EnRu1* this, PlayState* play) {
    func_80AEEF68(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAC10(this, play);
    func_80AEAECC(this, play);
    func_80AEFE84(this, play, func_80AEFDC0(this, play));
}

void EnRu1_Action45(EnRu1* this, PlayState* play) {
    func_80AEEFEC(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAECC(this, play);
    func_80AEFE9C(this, play);
}

void func_80AEFF94(EnRu1* this, PlayState* play) {
    s8 actorRoom;

    if ((gSaveContext.infTable[20] & 2) && (gSaveContext.infTable[20] & 1) && shouldSpawnRuto() &&
        (!(func_80AEB020(this, play)))) {
        func_80AEB264(this, &gRutoChildWait2Anim, 0, 0, 0);
        actorRoom = this->actor.room;
        this->action = 22;
        this->actor.room = -1;
        this->drawConfig = 0;
        this->roomNum1 = actorRoom;
        this->roomNum3 = actorRoom;
        this->roomNum2 = actorRoom;
        // "Ruto switch set"
        //osSyncPrintf("スイッチルトセット!!!!!!!!!!!!!!!!!!!!!!\n");
    } else {
        // "Ruto switch not set"
        //osSyncPrintf("スイッチルトセットしない!!!!!!!!!!!!!!!!!!!!!!\n");
        Actor_Kill(&this->actor);
    }
}

void EnRu1_SpotLink(EnRu1* this, PlayState* play, s16 relTime) {
    if (!EnRu1_HadDateStarted()) {
        Player* player = GET_PLAYER(play);
        f32 heightMod = (relTime < 0 ? -20 : 10);//Acounts for the movement of Ruto's root position when her animations transition
        Vec3f headPos = this->actor.world.pos;
        Vec3f headPosLink = player->actor.world.pos;
        headPos.y += 20.0f + heightMod;
        headPosLink.y += 25.0f;
        Vec3f result;
        CollisionPoly* outPoly;
        s32 bgID;
        if (!BgCheck_CheckLineImpl(&play->colCtx,(1<<1),0,&headPos,&headPosLink,&result,
                        &outPoly,&bgID,&player->actor,1.0,BgCheck_GetBccFlags(1,1,1,1,1))) {
            if (relTime == -1) {
                gSaveContext.infTable[20] |= 0x2000;
                u16 RutoMsg = GetTextID("ruto");
                Message_StartTextbox(play, RutoMsg+5, NULL);
            } else if (relTime == 0) {
                if (this->dateProgress & 8)
                    gSaveContext.infTable[20] |= 0x4000;
                gSaveContext.infTable[20] |= 0x3000;
                u16 RutoMsg = GetTextID("ruto");
                Message_StartTextbox(play, RutoMsg+5, NULL);
            } else if (relTime == 1) {
                if (this->dateProgress & 8)
                    gSaveContext.infTable[20] |= 0x4000;
                gSaveContext.infTable[20] |= 0x1000;
                u16 RutoMsg = GetTextID("ruto");
                Message_StartTextbox(play, RutoMsg+5, NULL);
            }
        } else {
            if (this->actor.xzDistToPlayer < 800 && this->actor.yDistToPlayer+25 > heightMod) {
                this->dateProgress |= 0x8;//Sets flag to indicate you were noticed behind a pillar before you became visible
            }
        }
    }
}

//Action46
void EnRu1_NotAppearing(EnRu1* this, PlayState* play) {
    if (EnRu1_DateConditionsMet()) {
        //EnRu1_InitiateDate();
        func_80AEB264(this, &gRutoChildTreadWaterAnim, 0, 0, 0);
        this->timer = 25;
        this->action = 47;
        this->drawConfig = 2;
        this->alpha = 0.0f;
        this->actor.speedXZ = 0.0f;
    }
}

//Action47
void EnRu1_DateStart(EnRu1* this, PlayState* play) {
    s32 cond;

    this->actor.gravity = 0.0f;
    this->actor.minVelocityY = 0.0f;
    func_80AEEF68(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAC10(this, play);
    func_80AEAECC(this, play);
    if (this->actor.world.pos.z > 3560){
        if (this->timer > 0) {
            Math_ApproachS(&this->alpha, 255, 1, 10);
            this->timer--;
            if (this->timer == 0) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EV_OUT_OF_WATER);
                EnRu1_SpawnSplash(this,play);
                this->alpha = 255;
                this->drawConfig = 1;
            }
        } else {
            this->actor.speedXZ = 1.0f;
        }
    } else {
        if (this->timer == 0) {
            this->timer = 8;
            func_80AEB264(this, &gRutoChildWait2Anim, 0, -8, 0);
        } else {
            this->actor.speedXZ = 0.0f;
            this->timer--;
            this->actor.world.pos.y -= 3;//(1336-1313)/8

            if (this->timer <= 0) {
                this->drawConfig = 1;
                this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_3;
                this->action = 48;
            }
        }
    }
    if (this->actor.speedXZ == 1.0f) {
        EnRu1_SpotLink(this, play, -1);
    }
    Actor_MoveForward(&this->actor);
    if (this->actor.world.pos.y <= -1336)
        this->actor.world.pos.y = -1336;
}

//Action48
void EnRu1_DateInitialTalk(EnRu1* this, PlayState* play) {
    s32 cond;
    EnRu1_SpotLink(this, play, (gSaveContext.dayTime < (49153+8192)) ? 0 : 1);
    func_80AEEF68(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAC10(this, play);
    func_80AEAECC(this, play);
    cond = EnRu1_DateConversation(this, play);
    func_80AED624(this, play);
    if (cond) {
        this->action = 49;
    }
}

s32 EnRu1_IsWillingToSit(EnRu1* this, PlayState* play) {
    s32 frameCount;

    if (!(gSaveContext.infTable[21] & 0x800)) {
        frameCount = Animation_GetLastFrame(&gRutoChildSitAnim);
        Animation_Change(&this->skelAnime, &gRutoChildSitAnim, 1.0f, 0, frameCount, ANIMMODE_ONCE, -8.0f);
        play->msgCtx.msgMode = MSGMODE_PAUSED;
        EnRu1_InitiateDate();
        this->action = 26;
        this->actor.flags &= ~(ACTOR_FLAG_0 | ACTOR_FLAG_3);
        Actor_Spawn(&play->actorCtx,play,ACTOR_EN_CROW, -3040,-980,4235, 0,0,0, 0x0,false);
        Actor_Spawn(&play->actorCtx,play,ACTOR_EN_CROW, -3050,-1020,4685, 0,0,0, 0x0,false);
        Actor_Spawn(&play->actorCtx,play,ACTOR_EN_CROW, -3045,-1010,5130, 0,0,0, 0x0,false);
        return true;
    }
    return false;
}

//Action49
void EnRu1_DateDuringTalk(EnRu1* this, PlayState* play) {
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    if (func_80AEB174(play) && !EnRu1_IsWillingToSit(this, play)) {
        Message_CloseTextbox(play);
        this->drawConfig = 1;
        this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_3;
        this->action = 48;
    }
}

s32 EnRu1_DateEndConversation(EnRu1* this, PlayState* play) {
    if (!Actor_ProcessTalkRequest(&this->actor, play)) {
        u16 RutoMsg = GetTextID("ruto");
        this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_3;
        if (EnRu1_DetermineDatePreScore() == 1000)
            this->actor.textId = RutoMsg+12;
        else
            this->actor.textId = RutoMsg+23;
        func_8002F2F4(&this->actor, play);
        return false;
    }
    return true;
}

//Action51
void EnRu1_DateFinalTalk(EnRu1* this, PlayState* play) {
    s32 cond;
    func_80AEEF68(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAC10(this, play);
    func_80AEAECC(this, play);
    cond = EnRu1_DateEndConversation(this, play);
    func_80AED624(this, play);
    if (cond) {
        this->action = 52;
    }
}

//Action52
void EnRu1_DateDuringFinalTalk(EnRu1* this, PlayState* play) {
    if (func_80AEB1B4(play)) {
        Message_CloseTextbox(play);
        this->timer = 0;
        Animation_Change(&this->skelAnime, &gRutoChildWalkAnim, 1.0f, 0, Animation_GetLastFrame(&gRutoChildWalkAnim),
                         ANIMMODE_LOOP, -8.0f);
        this->actor.shape.rot.y = this->actor.world.rot.y + (EnRu1_IsPassedTree(this->actor.world.pos) ? 0x0 : 0x8000);
        this->action = 53;
        gSaveContext.infTable[21] |= 0x8;//Records that you've seen her off
        this->actor.flags &= ~(ACTOR_FLAG_0 | ACTOR_FLAG_3);
    }
}

//Action53
void EnRu1_DateEnd(EnRu1* this, PlayState* play) {
    s32 cond;

    this->actor.speedXZ = 2.0f;
    Math_ApproachS(&this->actor.world.rot.y, 0x8000, 1, 0x200);
    func_80AEEF68(this, play);
    EnRu1_UpdateSkelAnime(this);
    EnRu1_UpdateEyes(this);
    func_80AEAC10(this, play);
    if (this->actor.world.pos.y <= -1336){
        this->actor.minVelocityY = 0.0f;
        this->actor.gravity = 0.0f;
        if (this->timer == 0) {
            func_80AEB264(this, &gRutoChildTreadWaterAnim, 0, -8, 0);
            this->actor.world.pos.y = -1336;
            this->timer = 8;
        }
        if (this->timer > 0) {
            Math_ApproachS(&this->actor.shape.rot.y, this->actor.world.rot.y, 1, 0x1000);
            this->timer--;
            if (this->timer == 0) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EV_OUT_OF_WATER);
                EnRu1_SpawnSplash(this,play);
                this->timer = -32;
            }
        } else if (this->timer < 0) {
            this->timer++;
            if (this->dateProgress & 0x10) {
                Math_ApproachS(&this->alpha, 0, 1, 10);
                if (this->timer == 0) {
                    Actor_Kill(&this->actor);
                }
            } else {
                if (this->timer == 0) {
                    this->timer = -30;
                    this->dateProgress |= 0x10;
                    this->drawConfig = 2;
                }
            }
        }
    } else {
        Math_ApproachS(&this->actor.shape.rot.y, this->actor.world.rot.y+0x8000, 1, 0x1000);
        this->actor.minVelocityY = -((kREG(24) * 0.01f) + 6.8f);
        this->actor.gravity = -((kREG(23) * 0.01f) + 1.3f);
        func_80AEAECC(this, play);
    }
    Actor_MoveForward(&this->actor);
}

void EnRu1_LakeDateSpawn(EnRu1* this, PlayState* play) {
    s8 actorRoom;
    if (LINK_IS_CHILD) {
        if ( EnRu1_DateConditionsMet()) {//If the date start time is passed
            if (!func_80AEB020(this, play)) {
                //EnRu1_InitiateDate();
                func_80AEB264(this, &gRutoChildWait2Anim, 0, 0, 0);
                const Vec3f dateStartPos = {-918,-1336,3560};
                this->actor.world.pos = dateStartPos;
                actorRoom = this->actor.room;
                this->drawConfig = 1;
                if (EnRu1_HadDateStarted()) {
                    gSaveContext.infTable[20] |= 0x8000;
                }
                this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_3;
                this->action = 48;
                this->actor.room = -1;
                this->roomNum1 = actorRoom;
                this->roomNum3 = actorRoom;
                this->roomNum2 = actorRoom;
            } else {
                Actor_Kill(&this->actor);
            }
        } else {//Otherwise spawn a 'dormant' Ruto, who will wait until the right time
            func_80AEB264(this, &gRutoChildTreadWaterAnim, 0, 0, 0);
            const Vec3f dateStartPos = {-918,-1313,3734};
            this->actor.world.pos = dateStartPos;
            actorRoom = this->actor.room;
            this->drawConfig = 0;
            this->actor.flags &= ~(ACTOR_FLAG_0 | ACTOR_FLAG_3);
            this->action = 46;
            this->actor.room = -1;
            this->roomNum1 = actorRoom;
            this->roomNum3 = actorRoom;
            this->roomNum2 = actorRoom;
        }
    } else {
        Actor_Kill(&this->actor);
    }
}

void func_80AF0050(EnRu1* this, PlayState* play) {
    func_80AEB264(this, &gRutoChildWait2Anim, 0, 0, 0);
    this->action = 36;
    this->roomNum1 = this->actor.room;
    this->unk_28C = EnRu1_FindSwitch(play);
    this->actor.room = -1;
}

s32 EnRu1_IsOnIsland(Vec3f cPos) {
    return -1311 < cPos.x && cPos.x < -420 && 6860 < cPos.z && cPos.z < 7700;
}

s32 EnRu1_IsNotBehindTree(Vec3f cPos) {
    return -1311 < cPos.x && cPos.x < -420 && 6860 < cPos.z && cPos.z < 7300;
}

s32 EnRu1_IsPassedTree(Vec3f cPos) {
    return 7500 < cPos.z;
}

void EnRu1_Update(Actor* thisx, PlayState* play) {
    EnRu1* this = (EnRu1*)thisx;

    if (0x57 == play->sceneNum) {
        u16 RutoMsg = GetTextID("ruto");
        Vec3f cPos = this->actor.world.pos;

        if (EnRu1_DateConditionsMet()) {
            if (!(this->dateProgress & 0x1) &&
                -3120 < cPos.x && cPos.x < -2950 &&
                -1100 < cPos.y &&
                4000 < cPos.z) {

                this->dateProgress |= 0x1;
                Message_StartTextbox(play, RutoMsg+6, NULL);
            }

            if (!(this->dateProgress & 0x2) &&
                -3205 < cPos.x && cPos.x < -2885 &&
                5915 < cPos.z && cPos.z < 6235 &&
                (Actor_FindNearby(play,thisx,ACTOR_EN_OWL,ACTORCAT_NPC,300.0f) != NULL)) {

                this->dateProgress |= 0x2;
                gSaveContext.infTable[21] |= 0x40;
                Message_StartTextbox(play, RutoMsg+7, NULL);
            }

            if (!(this->dateProgress & 0x4) &&
                EnRu1_IsOnIsland(cPos) &&
                IS_NIGHT) {

                this->dateProgress |= 0x4;
                Message_StartTextbox(play, RutoMsg+8, NULL);
            }
        } else {
            if (IS_DAY && gSaveContext.RutoDateDay == (gSaveContext.totalDays-1)) {
                Player* player = GET_PLAYER(play);
                if (EnRu1_HadDateStarted() && !EnRu1_HadDateEnded()) {
                    if ((this->dateProgress & 0x4) &&
                        (gSaveContext.infTable[21] & 0x0004) &&
                        EnRu1_IsOnIsland(cPos) &&
                        EnRu1_IsOnIsland(player->actor.world.pos) &&
                        this->action == 27) {

                        gSaveContext.infTable[20] |= 0x800;
                        if (EnRu1_IsNotBehindTree(cPos)) {
                            if (this->actor.xzDistToPlayer < 100.0f){
                                gSaveContext.infTable[21] |= 0x80;
                                if (EnRu1_DetermineDatePreScore() < 1000)
                                    Message_StartTextbox(play, RutoMsg+10, NULL);
                                else
                                    Message_StartTextbox(play, RutoMsg+11, NULL);
                            } else {
                                gSaveContext.infTable[20] |= 0x100;
                                Message_StartTextbox(play, RutoMsg+19, NULL);
                            }
                        } else {
                            gSaveContext.infTable[20] |= 0x100;
                            Message_StartTextbox(play, RutoMsg+26, NULL);
                        }
                    } else {
                        gSaveContext.infTable[20] |= 0x100;
                        Message_StartTextbox(play, RutoMsg+9, NULL);
                    }
                } else if ((gSaveContext.infTable[20] & 0x7000) && !(gSaveContext.infTable[21] & 0x0004)) {//Ruto had spotted you, but not talked
                    gSaveContext.infTable[21] |= 0x800;
                }
            }
        }
    }

    if (this->action < 0 || this->action >= ARRAY_COUNT(sActionFuncs) || sActionFuncs[this->action] == NULL) {
        // "Main mode is improper!"
        //osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }

    osSyncPrintf("Ruto Action: %d",this->action);
    sActionFuncs[this->action](this, play);
}

void EnRu1_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnRu1* this = (EnRu1*)thisx;
    this->timer = 0;
    this->expressionState = 0;
    this->dateProgress = 0;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &gRutoChildSkel, NULL, this->jointTable, this->morphTable, 17);
    func_80AEAD20(&this->actor, play);
    switch (func_80AEADF0(this)) {
        case 0:
            func_80AECDA0(this, play);
            break;
        case 1:
            func_80AEB3DC(this, play);
            break;
        case 2:
            func_80AEC320(this, play);
            break;
        case 3:
            func_80AED44C(this, play);
            break;
        case 4:
            func_80AEFC54(this, play);
            break;
        case 5:
            func_80AEFD38(this, play);
            break;
        case 6:
            func_80AEFF94(this, play);
            break;
        case 10:
            func_80AF0050(this, play);
            break;
        case 11:
            EnRu1_LakeDateSpawn(this, play);
            break;
        default:
            Actor_Kill(&this->actor);
            // "Relevant arge_data = %d unacceptable"
            //osSyncPrintf("該当 arge_data = %d 無し\n", func_80AEADF0(this));
            break;
    }
}

void func_80AF0278(EnRu1* this, PlayState* play, s32 limbIndex, Vec3s* rot) {
    Vec3s* vec1 = &this->unk_374.unk_0E;
    Vec3s* vec2 = &this->unk_374.unk_08;

    switch (limbIndex) {
        case RUTO_CHILD_LEFT_UPPER_ARM:
            rot->x += vec1->y;
            rot->y -= vec1->x;
            break;
        case RUTO_CHILD_TORSO:
            rot->x += vec2->y;
            rot->z += vec2->x;
            break;
    }
}

s32 EnRu1_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                           Gfx** gfx) {
    EnRu1* this = (EnRu1*)thisx;

    if ((this->unk_290 < 0) || (this->unk_290 > 0) || (*sPreLimbDrawFuncs[this->unk_290] == NULL)) {
        // "Neck rotation mode is improper!"
        //osSyncPrintf(VT_FGCOL(RED) "首回しモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    } else {
        sPreLimbDrawFuncs[this->unk_290](this, play, limbIndex, rot);
    }
    return false;
}

void EnRu1_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    EnRu1* this = (EnRu1*)thisx;
    Vec3f vec1;
    Vec3f vec2;

    if (limbIndex == RUTO_CHILD_TORSO) {
        vec1 = sMultVec;
        Matrix_MultVec3f(&vec1, &vec2);
        this->actor.focus.pos.x = vec2.x;
        this->actor.focus.pos.y = vec2.y;
        this->actor.focus.pos.z = vec2.z;
        this->actor.focus.rot.x = this->actor.world.rot.x;
        this->actor.focus.rot.y = this->actor.world.rot.y;
        this->actor.focus.rot.z = this->actor.world.rot.z;
    }
}

void EnRu1_DrawNothing(EnRu1* this, PlayState* play) {
}

void EnRu1_DrawOpa(EnRu1* this, PlayState* play) {
    s32 pad[2];
    s16 eyeIndex = this->eyeIndex;
    void* eyeTex = sEyeTextures[eyeIndex];
    s16 mouthIndex = this->mouthIndex;
    SkelAnime* skelAnime = &this->skelAnime;
    void* mouthTex = sMouthTextures[mouthIndex];
    s32 pad1;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(mouthTex));
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);

    if (this->action == 53) {
        Matrix_Push();
        if (this->actor.world.pos.y >= -1253.300f)//This is here to keep Ruto above the island while she walks off it
            Matrix_Translate(0.0f, 800.0f*(1.0f-this->skelAnime.morphWeight), 0.0f, MTXMODE_APPLY);
    }

    POLY_OPA_DISP = SkelAnime_DrawFlex(play, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                                       EnRu1_OverrideLimbDraw, EnRu1_PostLimbDraw, this, POLY_OPA_DISP);

    if (this->action == 53) {
        Matrix_Pop();
    }
    CLOSE_DISPS(play->state.gfxCtx);
}

void EnRu1_DrawXlu(EnRu1* this, PlayState* play) {
    s32 pad[2];
    s16 eyeIndex = this->eyeIndex;
    void* eyeTex = sEyeTextures[eyeIndex];
    s16 mouthIndex = this->mouthIndex;
    SkelAnime* skelAnime = &this->skelAnime;
    void* mouthTex = sMouthTextures[mouthIndex];
    s32 pad1;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(mouthTex));
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->alpha);
    gSPSegment(POLY_XLU_DISP++, 0x0C, &D_80116280[0]);

    POLY_XLU_DISP = SkelAnime_DrawFlex(play, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                                       EnRu1_OverrideLimbDraw, NULL, this, POLY_XLU_DISP);

    CLOSE_DISPS(play->state.gfxCtx);
}

void EnRu1_Draw(Actor* thisx, PlayState* play) {
    EnRu1* this = (EnRu1*)thisx;

    if (this->drawConfig < 0 || this->drawConfig >= ARRAY_COUNT(sDrawFuncs) || sDrawFuncs[this->drawConfig] == 0) {
        // "Draw mode is improper!"
        //osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    sDrawFuncs[this->drawConfig](this, play);
}
