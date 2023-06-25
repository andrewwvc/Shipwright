#include "z_boss_ganon2.h"
#include "overlays/actors/ovl_Demo_Gj/z_demo_gj.h"
#include "overlays/actors/ovl_En_Zl3/z_en_zl3.h"
#include "objects/object_ganon/object_ganon.h"
#include "objects/object_ganon2/object_ganon2.h"
#include "objects/object_ganon_anime3/object_ganon_anime3.h"
#include "objects/object_geff/object_geff.h"
#include "soh/frame_interpolation.h"

#include <string.h>

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4 | ACTOR_FLAG_5)

void BossGanon2_Init(Actor* thisx, PlayState* play);
void BossGanon2_Destroy(Actor* thisx, PlayState* play);
void BossGanon2_Update(Actor* thisx, PlayState* play);
void BossGanon2_Draw(Actor* thisx, PlayState* play);
void BossGanon2_Reset(void);

void func_808FD5C4(BossGanon2* this, PlayState* play);
void func_808FD5F4(BossGanon2* this, PlayState* play);
void func_808FFDB0(BossGanon2* this, PlayState* play);
void func_808FFEBC(BossGanon2* this, PlayState* play);
void func_808FFFE0(BossGanon2* this, PlayState* play);
void func_80900104(BossGanon2* this, PlayState* play);
void func_8090026C(BossGanon2* this, PlayState* play);
void func_809002CC(BossGanon2* this, PlayState* play);
void func_80900344(BossGanon2* this, PlayState* play);
void func_80900580(BossGanon2* this, PlayState* play, s16 isTracking);
void func_80900650(BossGanon2* this, PlayState* play);
void func_80900890(BossGanon2* this, PlayState* play);
void func_8090120C(BossGanon2* this, PlayState* play);
void func_80905DA8(BossGanon2* this, PlayState* play);
void func_809060E8(PlayState* play);
void BossGanon2_GenShadowTexture(void* shadowTexture, BossGanon2* this, PlayState* play);
void BossGanon2_DrawShadowTexture(void* shadowTexture, BossGanon2* this, PlayState* play);

const ActorInit Boss_Ganon2_InitVars = {
    ACTOR_BOSS_GANON2,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_GANON2,
    sizeof(BossGanon2),
    (ActorFunc)BossGanon2_Init,
    (ActorFunc)BossGanon2_Destroy,
    (ActorFunc)BossGanon2_Update,
    (ActorFunc)BossGanon2_Draw,
    (ActorResetFunc)BossGanon2_Reset,
};

#include "z_boss_ganon2_data.c"

Vec3f D_8090EB20;

EnZl3* sBossGanon2Zelda;

Actor* D_8090EB30;

BossGanon2Effect sBossGanon2Particles[100];

s32 sBossGanon2Seed1;
s32 sBossGanon2Seed2;
s32 sBossGanon2Seed3;

Vec3f D_809105D8[4];

Vec3f D_80910608[4];

s8 D_80910638;

#define MAX_HEALTH 30

void BossGanon2_InitRand(s32 seedInit0, s32 seedInit1, s32 seedInit2) {
    sBossGanon2Seed1 = seedInit0;
    sBossGanon2Seed2 = seedInit1;
    sBossGanon2Seed3 = seedInit2;
}

f32 BossGanon2_RandZeroOne(void) {
    // Wichmann-Hill algorithm
    f32 randFloat;

    sBossGanon2Seed1 = (sBossGanon2Seed1 * 171) % 30269;
    sBossGanon2Seed2 = (sBossGanon2Seed2 * 172) % 30307;
    sBossGanon2Seed3 = (sBossGanon2Seed3 * 170) % 30323;

    randFloat = (sBossGanon2Seed1 / 30269.0f) + (sBossGanon2Seed2 / 30307.0f) + (sBossGanon2Seed3 / 30323.0f);
    while (randFloat >= 1.0f) {
        randFloat -= 1.0f;
    }
    return fabsf(randFloat);
}

void func_808FD080(s32 idx, ColliderJntSph* collider, Vec3f* arg2) {
    collider->elements[idx].dim.worldSphere.center.x = arg2->x;
    collider->elements[idx].dim.worldSphere.center.y = arg2->y;
    collider->elements[idx].dim.worldSphere.center.z = arg2->z;

    collider->elements[idx].dim.worldSphere.radius =
        collider->elements[idx].dim.modelSphere.radius * collider->elements[idx].dim.scale;
}

void BossGanon2_SetObjectSegment(BossGanon2* this, PlayState* play, s32 objectId, u8 setRSPSegment) {
    s32 pad;
    s32 objectIdx = Object_GetIndex(&play->objectCtx, objectId);

    gSegments[6] = PHYSICAL_TO_VIRTUAL(play->objectCtx.status[objectIdx].segment);

    if (setRSPSegment) {
        OPEN_DISPS(play->state.gfxCtx);

        gSPSegment(POLY_OPA_DISP++, 0x06, play->objectCtx.status[objectIdx].segment);
        gSPSegment(POLY_XLU_DISP++, 0x06, play->objectCtx.status[objectIdx].segment);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void func_808FD210(PlayState* play, Vec3f* arg1) {
    BossGanon2Effect* effect = play->specialEffects;

    effect->type = 1;
    effect->position = *arg1;
    effect->unk_2E = 0;
    effect->unk_01 = 0;
    effect->velocity.x = 25.0f;
    effect->velocity.y = 15.0f;
    effect->velocity.z = 0.0f;
    effect->accel.x = 0.0f;
    effect->accel.y = -1.0f;
    effect->accel.z = 0.0f;
}

void func_808FD27C(PlayState* play, Vec3f* position, Vec3f* velocity, f32 scale) {
    BossGanon2Effect* effect = play->specialEffects;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(sBossGanon2Particles); i++, effect++) {
        if (effect->type == 0) {
            effect->type = 2;
            effect->position = *position;
            effect->velocity = *velocity;
            effect->accel.x = 0.0;
            effect->accel.y = -1.0f;
            effect->accel.z = 0.0;
            effect->unk_38.z = Rand_ZeroFloat(2 * M_PI);
            effect->unk_38.y = Rand_ZeroFloat(2 * M_PI);
            effect->unk_38.x = Rand_ZeroFloat(2 * M_PI);
            effect->scale = scale;
            break;
        }
    }
}

void BossGanon2_Init(Actor* thisx, PlayState* play) {
    BossGanon2* this = (BossGanon2*)thisx;
    s32 pad;
    s16 i;

    play->specialEffects = sBossGanon2Particles;

    for (i = 0; i < ARRAY_COUNT(sBossGanon2Particles); i++) {
        sBossGanon2Particles[i].type = 0;
    }

    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.colChkInfo.health = MAX_HEALTH;
    Collider_InitJntSph(play, &this->unk_424);
    Collider_SetJntSph(play, &this->unk_424, &this->actor, &sJntSphInit1, this->unk_464);
    Collider_InitJntSph(play, &this->swordSpheres1);
    Collider_InitJntSph(play, &this->swordSpheres2);
    Collider_SetJntSph(play, &this->swordSpheres1, &this->actor, &sJntSphInit2, this->swordSphereElement1);
    Collider_SetJntSph(play, &this->swordSpheres2, &this->actor, &sJntSphInit3, this->swordSphereElement2);
    Collider_InitTris(play, &this->unk_444);
    Collider_InitTris(play, &this->unk_445);
    Collider_SetTris(play, &this->unk_444, &this->actor, &sTrisInit2, this->unk_864);
    Collider_SetTris(play, &this->unk_445, &this->actor, &sTrisInit3, this->unk_865);
    BossGanon2_SetObjectSegment(this, play, OBJECT_GANON, false);
    SkelAnime_InitFlex(play, &this->skelAnime, &gGanondorfSkel, NULL, NULL, NULL, 0);
    func_808FD5C4(this, play);
    this->isTrackingLink = false;
    this->actor.naviEnemyId = 0x3E;
    this->actor.gravity = 0.0f;
}

void BossGanon2_Destroy(Actor* thisx, PlayState* play) {
    BossGanon2* this = (BossGanon2*)thisx;

    SkelAnime_Free(&this->skelAnime, play);
    Collider_DestroyJntSph(play, &this->unk_424);
    Collider_DestroyJntSph(play, &this->swordSphereElement1);
    Collider_DestroyJntSph(play, &this->swordSphereElement2);
    Collider_DestroyTris(play, &this->unk_444);
    Collider_DestroyTris(play, &this->unk_445);
}

void func_808FD4D4(BossGanon2* this, PlayState* play, s16 arg2, s16 arg3) {
    if ((arg2 == 0) || (arg2 == 1)) {
        Actor_SpawnFloorDustRing(play, &this->actor, &this->unk_1D0, 25.0f, arg3, 8.0f, 500, 10, true);
    }

    if ((arg2 == 0) || (arg2 == 2)) {
        Actor_SpawnFloorDustRing(play, &this->actor, &this->unk_1DC, 25.0f, arg3, 8.0f, 500, 10, true);
    }

    Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_WALK);
    func_80033E88(&this->actor, play, 2, 0xA);
}

void func_808FD5C4(BossGanon2* this, PlayState* play) {
    this->actionFunc = func_808FD5F4;
    this->actor.flags &= ~ACTOR_FLAG_0;
    this->actor.world.pos.y = -3000.0f;
}

void func_808FD5F4(BossGanon2* this, PlayState* play) {
    s16 pad;
    u8 sp8D;
    Player* player;
    s32 objectIdx;
    s32 zero = 0;
    s32 pad2;

    sp8D = false;
    player = GET_PLAYER(play);
    this->unk_398++;

    switch (this->unk_39C) {
        case 0:
            objectIdx = Object_GetIndex(&play->objectCtx, OBJECT_GANON_ANIME3);
            if (Object_IsLoaded(&play->objectCtx, objectIdx)) {
                func_80064520(play, &play->csCtx);
                func_8002DF54(play, &this->actor, 8);
                this->unk_39E = Play_CreateSubCamera(play);
                Play_ChangeCameraStatus(play, MAIN_CAM, CAM_STAT_WAIT);
                Play_ChangeCameraStatus(play, this->unk_39E, CAM_STAT_ACTIVE);
                this->unk_39C = 1;
                sBossGanon2Zelda = (EnZl3*)Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_ZL3, 970.0f,
                                                    1086.0f, -200.0f, 0, 0, 0, 1);
                sBossGanon2Zelda->unk_3C8 = 0;
                sBossGanon2Zelda->actor.world.pos.x = 970.0f;
                sBossGanon2Zelda->actor.world.pos.y = 1086.0f;
                sBossGanon2Zelda->actor.world.pos.z = -214.0f;
                sBossGanon2Zelda->actor.shape.rot.y = -0x7000;
                this->unk_3BC.x = 0.0f;
                this->unk_3BC.y = 1.0f;
                this->unk_3BC.z = 0.0f;
                this->unk_398 = 0;
                this->unk_3A4.x = 0.0f;
                this->unk_3A4.y = 1400.0f;
                this->unk_3A4.z = 1600.0f;
                player->actor.world.pos.x = 970.0f;
                player->actor.world.pos.y = 1086.0f;
                player->actor.world.pos.z = -186.0f;
                player->actor.shape.rot.y = -0x5000;
                Animation_MorphToLoop(&this->skelAnime, &gGanondorfBurstOutAnim, 0.0f);
                play->envCtx.unk_D8 = 0.0f;
                // fake, tricks the compiler into allocating more stack
                if (zero) {
                    this->unk_3A4.x *= 2.0;
                }
            } else {
                break;
            }
        case 1:
            if (this->unk_398 < 70) {
                play->envCtx.unk_D8 = 0.0f;
            }
            this->unk_339 = 3;
            Math_ApproachF(&this->unk_3A4.x, 1500.0f, 0.1f, this->unk_410.x * 1500.0f);
            Math_ApproachF(&this->unk_3A4.z, -160.0f, 0.1f, this->unk_410.x * 1760.0f);
            Math_ApproachF(&this->unk_410.x, 0.0075f, 1.0f, 0.0001f);
            this->unk_3B0.x = -200.0f;
            this->unk_3B0.y = 1086.0f;
            this->unk_3B0.z = -200.0f;
            if (this->unk_398 == 150) {
                Message_StartTextbox(play, 0x70D3, NULL);
            }
            if (this->unk_398 > 250 && Message_GetState(&play->msgCtx) == TEXT_STATE_NONE) {
                this->unk_39C = 2;
                this->unk_398 = 0;
                this->unk_410.x = 0.0f;
                play->envCtx.unk_D8 = 1.0f;
            } else {
                break;
            }
        case 2:
            this->unk_339 = 4;
            player->actor.world.pos.x = 970.0f;
            player->actor.world.pos.y = 1086.0f;
            player->actor.world.pos.z = -166.0f;
            sBossGanon2Zelda->actor.world.pos.x = 974.0f;
            sBossGanon2Zelda->actor.world.pos.y = 1086.0f;
            sBossGanon2Zelda->actor.world.pos.z = -186.0f;
            player->actor.shape.rot.y = -0x5000;
            sBossGanon2Zelda->actor.shape.rot.y = -0x5000;
            if (this->unk_398 == 60) {
                Message_StartTextbox(play, 0x70D4, NULL);
            }
            if (this->unk_398 == 40) {
                sBossGanon2Zelda->unk_3C8 = 1;
                func_8002DF54(play, &this->actor, 0x4E);
            }
            if (this->unk_398 == 85) {
                sBossGanon2Zelda->unk_3C8 = 2;
                func_8002DF54(play, &this->actor, 0x4F);
            }
            this->unk_3A4.x = 930.0f;
            this->unk_3A4.y = 1129.0f;
            this->unk_3A4.z = -181.0f;
            this->unk_3B0.x = player->actor.world.pos.x;
            this->unk_3B0.z = (player->actor.world.pos.z - 15.0f) + 5.0f;
            if (this->unk_398 > 104) {
                Math_ApproachF(&this->unk_3B0.y, player->actor.world.pos.y + 47.0f + 7.0f + 15.0f, 0.1f,
                               this->unk_410.x);
                Math_ApproachF(&this->unk_410.x, 2.0f, 1.0f, 0.1f);
            } else {
                this->unk_3B0.y = player->actor.world.pos.y + 47.0f + 7.0f;
            }
            if ((this->unk_398 > 170) && (Message_GetState(&play->msgCtx) == TEXT_STATE_NONE)) {
                this->unk_39C = 3;
                this->unk_398 = 0;
                this->unk_410.x = 0.0f;
            }
            break;
        case 3:
            Math_ApproachF(&this->unk_3B0.y, player->actor.world.pos.y + 47.0f + 7.0f, 0.1f, 2.0f);
            this->unk_339 = 4;
            if (this->unk_398 == 10) {
                func_80078914(&D_80906D6C, NA_SE_EV_STONE_BOUND);
                Audio_QueueSeqCmd(SEQ_PLAYER_BGM_MAIN << 24 | NA_BGM_STOP);
            }
            if (this->unk_398 == 20) {
                sBossGanon2Zelda->unk_3C8 = 3;
                func_8002DF54(play, &this->actor, 0x50);
            }
            if (this->unk_398 == 55) {
                this->unk_39C = 4;
                this->unk_398 = 0;
                this->unk_410.x = 0.0f;
                sBossGanon2Zelda->unk_3C8 = 4;
                func_8002DF54(play, &this->actor, 0x50);
            }
            break;
        case 4:
            this->unk_339 = 4;
            Math_ApproachF(&this->unk_3A4.x, -360.0f, 0.1f, this->unk_410.x * 1290.0f);
            Math_ApproachF(&this->unk_3A4.z, -20.0f, 0.1f, this->unk_410.x * 170.0f);
            Math_ApproachF(&this->unk_410.x, 0.04f, 1.0f, 0.0005f);
            if (this->unk_398 == 100) {
                Camera* camera = Play_GetCamera(play, MAIN_CAM);

                camera->eye = this->unk_3A4;
                camera->eyeNext = this->unk_3A4;
                camera->at = this->unk_3B0;
                func_800C08AC(play, this->unk_39E, 0);
                this->unk_39E = 0;
                func_80064534(play, &play->csCtx);
                func_8002DF54(play, &this->actor, 7);
                this->unk_39C = 5;
                this->unk_398 = 0;
            }
            break;
        case 5:
            this->unk_339 = 4;
            if (this->actor.xzDistToPlayer < 500.0f) {
                Message_CloseTextbox(play);
                this->unk_39C = 10;
                this->unk_398 = 0;
                func_80064520(play, &play->csCtx);
                this->unk_39E = Play_CreateSubCamera(play);
                Play_ChangeCameraStatus(play, MAIN_CAM, CAM_STAT_WAIT);
                Play_ChangeCameraStatus(play, this->unk_39E, CAM_STAT_ACTIVE);
            } else {
                break;
            }
        case 10:
            player->actor.world.pos.x = 490.0f;
            player->actor.world.pos.y = 1086.0f;
            player->actor.world.pos.z = -166.0f;
            sBossGanon2Zelda->actor.world.pos.x = 724.0f;
            sBossGanon2Zelda->actor.world.pos.y = 1086.0f;
            sBossGanon2Zelda->actor.world.pos.z = -186.0f;
            player->actor.shape.rot.y = -0x4000;
            sBossGanon2Zelda->actor.shape.rot.y = -0x5000;
            this->unk_3A4.x = 410.0f;
            this->unk_3A4.y = 1096.0f;
            this->unk_3A4.z = -110.0f;
            this->unk_3B0.x = player->actor.world.pos.x + 10.0f;
            this->unk_3B0.y = (player->actor.world.pos.y + 200.0f) - 160.0f;
            this->unk_3B0.z = player->actor.world.pos.z;
            if (this->unk_398 >= 20) {
                func_80078884(NA_SE_EN_GOMA_LAST - SFX_FLAG);
                Math_ApproachF(&this->unk_324, 255.0f, 1.0f, 10.0f);
                this->unk_339 = 5;
                if (this->unk_398 == 20) {
                    this->unk_33C = 0.0f;
                    play->envCtx.unk_D8 = 0.0f;
                }
            } else {
                this->unk_339 = 4;
            }
            if (this->unk_398 == 30) {
                sBossGanon2Zelda->unk_3C8 = 5;
                func_8002DF54(play, &this->actor, 0x51);
            }
            if (this->unk_398 == 50) {
                this->unk_398 = 0;
                this->unk_39C = 11;
            }
            break;
        case 11:
            this->unk_339 = 5;
            func_80078884(NA_SE_EN_GOMA_LAST - SFX_FLAG);
            player->actor.world.pos.x = 490.0f;
            player->actor.world.pos.y = 1086.0f;
            player->actor.world.pos.z = -166.0f;
            sBossGanon2Zelda->actor.world.pos.x = 724.0f;
            sBossGanon2Zelda->actor.world.pos.y = 1086.0f;
            sBossGanon2Zelda->actor.world.pos.z = -186.0f;
            player->actor.shape.rot.y = -0x4000;
            sBossGanon2Zelda->actor.shape.rot.y = -0x5000;
            this->unk_3A4.x = 450.0f;
            this->unk_3A4.y = 1121.0f;
            this->unk_3A4.z = -158.0f;
            this->unk_3B0.x = (player->actor.world.pos.x - 20.0f) + 2.0f;
            this->unk_3B0.y = ((player->actor.world.pos.y + 200.0f) - 151.0f) - 2.0f;
            this->unk_3B0.z = player->actor.world.pos.z + 2.0f;
            if (this->unk_398 == 10) {
                func_80078914(&D_80906D6C, NA_SE_EV_STONE_BOUND);
            }
            if (this->unk_398 == 20) {
                func_80078884(NA_SE_EV_STONE_BOUND);
            }
            if (this->unk_398 == 30) {
                func_8002DF54(play, &this->actor, 0x52);
            }
            if (this->unk_398 == 50) {
                this->unk_398 = 0;
                this->unk_39C = 12;
                Animation_MorphToPlayOnce(&this->skelAnime, &gGanondorfBurstOutAnim, 0.0f);
                this->unk_194 = Animation_GetLastFrame(&gGanondorfBurstOutAnim);
                this->actor.world.pos.x = this->actor.world.pos.z = -200.0f;
                this->actor.world.pos.y = 1009.0f;
                this->actor.shape.yOffset = 7000.0f;
                this->actor.world.rot.y = 0x5000;
                this->unk_3A4.x = -60.0f;
                this->unk_3A4.y = 1106.0f;
                this->unk_3A4.z = -200.0f;
                this->unk_3B0.x = this->unk_3B0.z = -200.0f;
                this->unk_3B0.y = this->actor.world.pos.y + 70.0f;
                play->envCtx.unk_D8 = 0.0f;
                play->envCtx.unk_BE = play->envCtx.unk_BD = 0;
                this->unk_339 = 0;
            } else {
                break;
            }
        case 12:
        case 13:
            SkelAnime_Update(&this->skelAnime);
            if (this->unk_398 == 30) {
                D_80906D78 = 1;
                this->unk_314 = 1;
                func_800A9F6C(0.0f, 0xC8, 0x14, 0x14);
            }
            if (this->unk_398 == 30) {
                func_80078884(NA_SE_EV_GRAVE_EXPLOSION);
            }
            if (this->unk_398 >= 30) {
                Math_ApproachF(&this->actor.world.pos.y, 1289.0f, 0.1f, 10.0f);
                this->unk_3B0.y = this->actor.world.pos.y + 70.0f;
            }
            if (Animation_OnFrame(&this->skelAnime, this->unk_194)) {
                Animation_MorphToLoop(&this->skelAnime, &gGanondorfFloatingHeavyBreathingAnim, 0.0f);
                this->unk_39C = 14;
                this->unk_398 = 0;
                this->actor.world.pos.x = -200.0f;
                this->actor.world.pos.y = this->actor.world.pos.y - 30.0f;
                this->actor.world.pos.z = -200.0f;
                Audio_QueueSeqCmd(SEQ_PLAYER_BGM_MAIN << 24 | NA_BGM_OPENING_GANON);
            } else {
                break;
            }
        case 14:
            SkelAnime_Update(&this->skelAnime);
            Math_ApproachF(&this->actor.world.pos.y, 1289.0f, 0.05f, 1.0f);
            player->actor.world.pos.x = 250.0f;
            player->actor.world.pos.y = 1086.0f;
            player->actor.world.pos.z = -266.0f;
            player->actor.shape.rot.y = -0x4000;
            sBossGanon2Zelda->actor.world.pos.x = 724.0f;
            sBossGanon2Zelda->actor.world.pos.y = 1086.0f;
            sBossGanon2Zelda->actor.world.pos.z = -186.0f;
            this->unk_3A4.x = this->actor.world.pos.x + -10.0f;
            this->unk_3A4.y = this->actor.world.pos.y + 80.0f;
            this->unk_3A4.z = this->actor.world.pos.z + 50.0f;
            this->unk_3B0.x = player->actor.world.pos.x;
            this->unk_3B0.y = player->actor.world.pos.y;
            this->unk_3B0.z = player->actor.world.pos.z - 200.0f;
            if (this->unk_398 == 20) {
                func_8002DF54(play, &this->actor, 0x1E);
            }
            if (this->unk_398 == 60) {
                this->unk_3A4.x = (this->actor.world.pos.x + 200.0f) - 154.0f;
                this->unk_3A4.y = this->actor.world.pos.y + 60.0f;
                this->unk_3A4.z = this->actor.world.pos.z - 15.0f;
                this->unk_39C = 15;
                this->unk_398 = 0;
                this->unk_3B0.y = this->actor.world.pos.y + 77.0f + 100.0f;
                this->unk_314 = 2;
                this->unk_3B0.z = this->actor.world.pos.z + 5.0f;
                this->unk_3B0.x = this->actor.world.pos.x;
            }
            if ((play->gameplayFrames % 32) == 0) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_GANON_BREATH);
            }
            break;
        case 15:
            if (((play->gameplayFrames % 32) == 0) && (this->unk_398 < 100)) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_GANON_BREATH);
            }
            SkelAnime_Update(&this->skelAnime);
            Math_ApproachF(&this->unk_3B0.y, this->actor.world.pos.y + 77.0f, 0.05f, 5.0f);
            if (this->unk_398 >= 50) {
                if (this->unk_398 == 50) {
                    Animation_MorphToPlayOnce(&this->skelAnime, &gGanondorfShowTriforceStartAnim, 0.0f);
                    this->unk_194 = Animation_GetLastFrame(&gGanondorfShowTriforceStartAnim);
                    this->unk_314 = 3;
                }
                if (Animation_OnFrame(&this->skelAnime, this->unk_194)) {
                    Animation_MorphToLoop(&this->skelAnime, &gGanondorfShowTriforceLoopAnim, 0.0f);
                    this->unk_194 = 1000.0f;
                }
            }
            if (this->unk_398 > 70) {
                Math_ApproachF(&this->unk_1B4, 255.0f, 1.0f, 10.0f);
            }
            if (this->unk_398 == 140) {
                this->unk_39C = 16;
                this->unk_398 = 0;
                Animation_MorphToPlayOnce(&this->skelAnime, &gGanondorfTransformStartAnim, 0.0f);
                this->unk_194 = Animation_GetLastFrame(&gGanondorfTransformStartAnim);
                this->unk_339 = 55;
                play->envCtx.unk_D8 = 1.0f;
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_GANON_CASBREAK);
            } else {
                break;
            }
        case 16:
            if (this->unk_398 < 25) {
                this->unk_339 = 55;
            } else {
                this->unk_339 = 6;
                if (this->unk_194 > 100.0f) {
                    Math_ApproachF(&this->unk_30C, 15.0f, 1.0f, 2.0f);
                } else {
                    Math_ApproachF(&this->unk_30C, 7.0f, 1.0f, 0.2f);
                }
            }
            this->unk_1B4 = 0.0f;
            SkelAnime_Update(&this->skelAnime);
            if (Animation_OnFrame(&this->skelAnime, this->unk_194)) {
                Animation_MorphToPlayOnce(&this->skelAnime, &gGanondorfTransformEndAnim, 0.0f);
                this->unk_194 = 1000.0f;
            }
            Math_ApproachF(&this->unk_3A4.x, (this->actor.world.pos.x + 200.0f) - 90.0f, 0.1f, 6.3999996f);
            Math_ApproachF(&this->unk_3A4.y, ((this->actor.world.pos.y + 60.0f) - 60.0f) - 70.0f, 0.1f, 13.0f);
            Math_ApproachF(&this->unk_3B0.y, this->actor.world.pos.y + 40.0f, 0.1f, 3.6999998f);
            if (this->unk_398 == 30) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_GANON_BIGMASIC);
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_GANON_THROW_BIG);
            }
            if (this->unk_398 <= 50) {
                sp8D = true;
            }
            if (this->unk_398 >= 60) {
                Camera* camera = Play_GetCamera(play, MAIN_CAM);

                camera->eye = this->unk_3A4;
                camera->eyeNext = this->unk_3A4;
                camera->at = this->unk_3B0;
                this->unk_39C = 17;
                this->unk_398 = 0;
                this->unk_337 = 2;
                BossGanon2_SetObjectSegment(this, play, OBJECT_GANON2, false);
                SkelAnime_Free(&this->skelAnime, play);
                SkelAnime_InitFlex(play, &this->skelAnime, &gGanonSkel, NULL, NULL, NULL, 0);
                BossGanon2_SetObjectSegment(this, play, OBJECT_GANON_ANIME3, false);
                func_8002DF54(play, &this->actor, 0x54);
                this->unk_314 = 3;

                // At this point, the actor has Ganon's skeleton but is still playing an animation for Ganondorf. This
                // causes issues when trying to access the limb posotions as Ganon has more limbs than Ganondorf. When
                // animating, data from past the end of the animation data is accessed. This is a hack solution so
                // that we are at least playing an animation meant for Ganon. There is no visible change since Ganon is
                // off-screen. There is actually 1 frame where he is visible, and in the vanilla game he is an
                // explosion of limbs since half of them are in random positions from the junk data accessed.
                Animation_PlayOnce(&this->skelAnime, &gGanonUncurlAndFlailAnim);
            }
            // fake, tricks the compiler into using stack the way we need it to
            if (zero) {
                Math_ApproachF(&this->unk_3B0.y, 0.0f, 0.0f, 0.0f);
            }
            break;
        case 17:
            this->unk_339 = 6;
            SkelAnime_Update(&this->skelAnime);
            this->unk_3A4.x = player->actor.world.pos.x - 40.0f;
            this->unk_3A4.y = player->actor.world.pos.y + 40.0f;
            this->unk_3A4.z = player->actor.world.pos.z + 20.0f;
            this->unk_3B0.x = player->actor.world.pos.x;
            this->unk_3B0.y = (player->actor.world.pos.y + 10.0f + 60.0f) - 30.0f;
            this->unk_3B0.z = player->actor.world.pos.z;
            if (this->unk_398 == 25) {
                this->unk_39C = 18;
                this->unk_398 = 0;
                Animation_MorphToPlayOnce(&this->skelAnime, &gGanonUncurlAndFlailAnim, 0.0f);
                this->skelAnime.playSpeed = 0.0f;
                this->unk_3A4.x = ((this->actor.world.pos.x + 500.0f) - 350.0f) - 50.0f;
                this->unk_3A4.y = this->actor.world.pos.y;
                this->unk_3A4.z = this->actor.world.pos.z;
                this->unk_3B0.x = this->actor.world.pos.x + 50.0f;
                this->unk_3B0.y = this->actor.world.pos.y + 60.0f;
                this->unk_3B0.z = this->actor.world.pos.z;
                this->actor.world.rot.y = 0x4000;
            }
            break;
        case 18:
            this->unk_339 = 6;
            if (this->unk_398 == 30) {
                Audio_QueueSeqCmd(SEQ_PLAYER_BGM_MAIN << 24 | NA_BGM_GANON_BOSS);
            }
            Math_ApproachF(&this->unk_30C, 7.0f, 1.0f, 0.1f);
            Math_ApproachF(&this->unk_3A4.x, (this->actor.world.pos.x + 500.0f) - 350.0f, 0.1f, 1.0f);
            Math_ApproachF(&this->unk_3B0.x, this->actor.world.pos.x, 0.1f, 1.0f);
            Math_ApproachF(&this->unk_228, 1.0f, 0.1f, 0.02f);
            if (this->unk_398 == 65) {
                this->unk_39C = 19;
                this->unk_398 = 0;
            }
            break;
        case 19:
            this->unk_394 += 0.5f;
            this->unk_339 = 6;
            this->actor.world.pos.y += this->actor.velocity.y;
            this->actor.velocity.y -= 1.0f;
            if (this->unk_398 == 10) {
                this->unk_39C = 20;
                this->unk_398 = 0;
                this->actor.world.pos.x += 250;
                this->actor.world.pos.y = 1886.0f;
                this->unk_394 = 0.0f;
                func_8002DF54(play, &this->actor, 0x53);
                this->unk_30C = 5.0f;
                this->unk_228 = 1.0f;
            }
            break;
        case 20:
            this->unk_339 = 6;
            SkelAnime_Update(&this->skelAnime);
            this->actor.world.pos.y += this->actor.velocity.y;
            this->actor.velocity.y -= 1.0f;
            player->actor.world.pos.x = 250.0f;
            player->actor.world.pos.y = 1086.0f;
            player->actor.world.pos.z = -266.0f;
            player->actor.shape.rot.y = -0x4000;
            this->unk_3A4.x = (player->actor.world.pos.x - 40.0f) - 200.0f;
            this->unk_3A4.y = (player->actor.world.pos.y + 40.0f) - 30.0f;
            this->unk_3A4.z = (player->actor.world.pos.z - 20.0f) + 100.0f;
            this->unk_3B0.x = player->actor.world.pos.x;
            this->unk_3B0.y = ((player->actor.world.pos.y + 10.0f + 60.0f) - 20.0f) + 30.0f;
            this->unk_3B0.z = player->actor.world.pos.z;
            this->unk_3BC.x = 0.8f;
            if (this->actor.world.pos.y <= 1099.0f) {
                this->actor.world.pos.y = 1099.0f;
                this->unk_39C = 21;
                this->unk_398 = 0;
                this->unk_420 = 10.0f;
                this->actor.velocity.y = 0.0f;
                Animation_MorphToPlayOnce(&this->skelAnime, &gGanonUncurlAndFlailAnim, 0.0f);
                func_808FD4D4(this, play, 0, 3);
                func_800A9F6C(0.0f, 0xC8, 0x14, 0x14);
            }
            break;
        case 21:
            this->unk_339 = 6;
            SkelAnime_Update(&this->skelAnime);
            this->unk_41C = Math_CosS(play->gameplayFrames * 0x8000) * this->unk_420;
            Math_ApproachZeroF(&this->unk_420, 1.0f, 0.75f);
            if (this->unk_398 == 30) {
                this->unk_39C = 22;
                this->unk_30C = 10.0f;
            } else {
                break;
            }
        case 22:
            if (this->unk_398 < 60) {
                this->unk_339 = 7;
            }
            this->unk_3BC.x = 0.0f;
            this->actor.world.pos.y = 1099.0f;
            SkelAnime_Update(&this->skelAnime);
            Math_ApproachZeroF(&this->unk_30C, 1.0f, 0.1f);
            if (this->unk_398 > 50) {
                Math_ApproachF(&this->unk_224, 1.0f, 1.0f, 0.025f);
            }
            if (this->unk_398 == 60) {
                this->unk_336 = 2;
            }
            if (this->unk_398 == 80) {
                BossGanon2_SetObjectSegment(this, play, OBJECT_GANON2, false);
                TitleCard_InitBossName(play, &play->actorCtx.titleCtx,
                                       SEGMENTED_TO_VIRTUAL(gGanonTitleCardTex), 160, 180, 128, 40, true);
                                       //It has translation but they are all the same. they all say "GANON" only
            }
            this->unk_3A4.x = ((this->actor.world.pos.x + 500.0f) - 350.0f) + 100.0f;
            this->unk_3A4.y = this->actor.world.pos.y;
            this->unk_3A4.z = this->actor.world.pos.z;
            this->unk_3B0.x = this->actor.world.pos.x;
            this->unk_3B0.z = this->actor.world.pos.z;
            this->unk_3B0.y = (this->unk_1B8.y + 60.0f) - 40.0f;
            if (this->unk_398 > 166 && this->unk_398 < 173) {
                this->unk_312 = 2;
            }
            if (this->unk_398 > 186 && this->unk_398 < 196) {
                this->unk_312 = 1;
            }
            if (this->unk_398 > 202 && this->unk_398 < 210) {
                this->unk_312 = 2;
            }
            if ((this->unk_398 == 166) || (this->unk_398 == 185) || (this->unk_398 == 200)) {
                func_80078884(NA_SE_EN_MGANON_SWORD);
                func_80078884(NA_SE_EN_MGANON_ROAR);
            }
            if (this->unk_398 == 215) {
                this->unk_39C = 23;
                this->unk_224 = 0.0f;
                func_8002DF54(play, &this->actor, 0x55);
            }
            break;
        case 23:
            SkelAnime_Update(&this->skelAnime);
            if (this->unk_398 > 222 && this->unk_398 < 232) {
                this->unk_312 = 2;
            }
            if (this->unk_398 == 222) {
                func_80078884(NA_SE_EN_MGANON_SWORD);
                func_80078884(NA_SE_EN_MGANON_ROAR);
            }
            this->unk_3A4.x = (player->actor.world.pos.x - 40.0f) + 6.0f;
            this->unk_3A4.y = player->actor.world.pos.y + 40.0f;
            this->unk_3A4.z = (player->actor.world.pos.z + 20.0f) - 7.0f;
            this->unk_3B0.x = player->actor.world.pos.x;
            this->unk_3B0.y = ((player->actor.world.pos.y + 10.0f + 60.0f) - 20.0f) - 2.0f;
            this->unk_3B0.z = player->actor.world.pos.z;
            if (this->unk_398 == 228) {
                func_80078884(NA_SE_IT_SHIELD_REFLECT_SW);
                func_8002DF54(play, &this->actor, 0x56);
                func_800A9F6C(0.0f, 0xFF, 0xA, 0x32);
            }
            if (this->unk_398 >= 229) {
                play->envCtx.fillScreen = true;
                play->envCtx.screenFillColor[0] = play->envCtx.screenFillColor[1] =
                    play->envCtx.screenFillColor[2] = 255;
                play->envCtx.screenFillColor[3] = 100;
                if (this->unk_398 == 234) {
                    Vec3f sp68;

                    play->envCtx.fillScreen = false;
                    this->unk_39C = 24;
                    this->unk_398 = 0;
                    sp68 = player->actor.world.pos;
                    sp68.y += 60.0f;
                    func_808FD210(play, &sp68);
                    play->envCtx.unk_D8 = 0.0f;
                    play->envCtx.unk_BE = 0;
                    this->unk_339 = 0;
                }
            }
            break;
        case 24:
            SkelAnime_Update(&this->skelAnime);
            if (1) {
                BossGanon2Effect* effect = play->specialEffects;

                this->unk_3B0 = effect->position;
                this->unk_3A4.x = effect->position.x + 70.0f;
                this->unk_3A4.y = effect->position.y - 30.0f;
                this->unk_3A4.z = effect->position.z + 70.0f;
            }
            if ((this->unk_398 & 3) == 0) {
                func_80078884(NA_SE_IT_SWORD_SWING);
            }
            if (this->unk_398 == 25) {
                func_8002DF54(play, &this->actor, 0x57);
                this->unk_39C = 25;
                this->unk_398 = 0;
            }
            break;
        case 25:
            SkelAnime_Update(&this->skelAnime);
            this->unk_3A4.x = (player->actor.world.pos.x - 40.0f) + 80.0f;
            this->unk_3A4.y = player->actor.world.pos.y + 40.0f + 10.0f;
            this->unk_3A4.z = player->actor.world.pos.z + 20.0f + 10.0f;
            this->unk_3B0.x = player->actor.world.pos.x - 20.0f;
            this->unk_3B0.y = ((player->actor.world.pos.y + 10.0f + 60.0f) - 20.0f) - 3.0f;
            this->unk_3B0.z = (player->actor.world.pos.z - 40.0f) - 10.0f;
            if (this->unk_398 == 10) {
                BossGanon2Effect* effect = play->specialEffects;

                effect->unk_2E = 1;
                effect->position.x = sBossGanon2Zelda->actor.world.pos.x + 50.0f + 10.0f;
                effect->position.y = sBossGanon2Zelda->actor.world.pos.y + 350.0f;
                effect->position.z = sBossGanon2Zelda->actor.world.pos.z - 25.0f;
                effect->velocity.x = 0.0f;
                effect->velocity.z = 0.0f;
                effect->velocity.y = -30.0f;
                this->unk_39C = 26;
                this->unk_398 = 0;
            } else {
                break;
            }
        case 26:
            this->unk_3A4.x = sBossGanon2Zelda->actor.world.pos.x + 100.0f + 30.0f;
            this->unk_3A4.y = sBossGanon2Zelda->actor.world.pos.y + 10.0f;
            this->unk_3A4.z = sBossGanon2Zelda->actor.world.pos.z + 5.0f;
            this->unk_3B0.x = sBossGanon2Zelda->actor.world.pos.x;
            this->unk_3B0.y = sBossGanon2Zelda->actor.world.pos.y + 30.0f;
            this->unk_3B0.z = sBossGanon2Zelda->actor.world.pos.z - 20.0f;
            this->unk_3BC.z = -0.5f;
            if (this->unk_398 == 13) {
                sBossGanon2Zelda->unk_3C8 = 6;
            }
            if (this->unk_398 == 50) {
                this->unk_39C = 27;
                this->unk_398 = 0;
            }
            break;
        case 27:
            this->unk_3BC.z = 0.0f;
            if (this->unk_398 == 4) {
                func_8002DF54(play, &this->actor, 0x58);
            }
            this->unk_3A4.x = player->actor.world.pos.x - 20.0f;
            this->unk_3A4.y = player->actor.world.pos.y + 50.0f;
            this->unk_3A4.z = player->actor.world.pos.z;
            this->unk_3B0.x = player->actor.world.pos.x;
            this->unk_3B0.y = player->actor.world.pos.y + 50.0f;
            this->unk_3B0.z = player->actor.world.pos.z;
            if (this->unk_398 == 26) {
                D_8090EB30 = play->actorCtx.actorLists[ACTORCAT_ITEMACTION].head;
                while (D_8090EB30 != NULL) {
                    if (D_8090EB30->id == ACTOR_EN_ELF) {
                        this->unk_3A4.x = D_8090EB30->world.pos.x - 30.0f;
                        this->unk_3A4.y = D_8090EB30->world.pos.y;
                        this->unk_3A4.z = D_8090EB30->world.pos.z;
                        this->unk_3B0.x = D_8090EB30->world.pos.x;
                        this->unk_3B0.y = D_8090EB30->world.pos.y;
                        this->unk_3B0.z = D_8090EB30->world.pos.z;
                        break;
                    }
                    D_8090EB30 = D_8090EB30->next;
                }
                this->unk_39C = 28;
                this->unk_398 = 0;
            }
            break;
        case 28:
            if (this->unk_398 == 5) {
                Message_StartTextbox(play, 0x70D6, NULL);
            }
            if (D_8090EB30 != NULL) {
                this->unk_3A4.x = D_8090EB30->world.pos.x - 20.0f;
                this->unk_3A4.y = D_8090EB30->world.pos.y;
                this->unk_3A4.z = D_8090EB30->world.pos.z;
                Math_ApproachF(&this->unk_3B0.x, D_8090EB30->world.pos.x, 0.2f, 50.0f);
                Math_ApproachF(&this->unk_3B0.y, D_8090EB30->world.pos.y, 0.2f, 50.0f);
                Math_ApproachF(&this->unk_3B0.z, D_8090EB30->world.pos.z, 0.2f, 50.0f);
                if ((this->unk_398 > 40) && (Message_GetState(&play->msgCtx) == TEXT_STATE_NONE)) {
                    this->unk_39C = 29;
                    this->unk_398 = 0;
                    Animation_MorphToPlayOnce(&this->skelAnime, &gGanonRoarAnim, 0.0f);
                    this->unk_194 = Animation_GetLastFrame(&gGanonRoarAnim);
                    this->actor.shape.yOffset = 0.0f;
                    this->actor.world.pos.y = 1086.0f;
                    this->actor.gravity = -1.0f;
                    this->unk_335 = 1;
                    this->unk_224 = 1.0f;
                }
            }
            break;
        case 29:
            SkelAnime_Update(&this->skelAnime);
            this->unk_3A4.x = (((this->actor.world.pos.x + 500.0f) - 350.0f) + 100.0f) - 60.0f;
            this->unk_3B0.x = this->actor.world.pos.x;
            this->unk_3B0.z = this->actor.world.pos.z;
            this->unk_3A4.y = this->actor.world.pos.y;
            this->unk_3A4.z = this->actor.world.pos.z + 10.0f;
            this->unk_3B0.y = (this->unk_1B8.y + 60.0f) - 40.0f;
            player->actor.shape.rot.y = -0x4000;
            player->actor.world.pos.x = 140.0f;
            player->actor.world.pos.z = -196.0f;
            if (this->unk_398 == 50) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_ROAR);
            }
            if (Animation_OnFrame(&this->skelAnime, this->unk_194)) {
                Camera* camera = Play_GetCamera(play, MAIN_CAM);

                camera->eye = this->unk_3A4;
                camera->eyeNext = this->unk_3A4;
                camera->at = this->unk_3B0;
                func_800C08AC(play, this->unk_39E, 0);
                this->unk_39E = 0;
                func_80064534(play, &play->csCtx);
                func_8002DF54(play, &this->actor, 7);
                this->unk_39C = 0;
                this->unk_337 = 1;
                func_808FFDB0(this, play);
                this->unk_1A2[1] = 50;
                this->actor.flags |= ACTOR_FLAG_0;
                sBossGanon2Zelda->unk_3C8 = 7;
            }
            break;
    }

    if ((this->unk_30C > 4.0f) && !sp8D) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_GANON_BODY_SPARK - SFX_FLAG);
    }

    if (this->unk_39E != 0) {
        // fake, tricks the compiler into putting some pointers on the stack
        if (zero) {
            osSyncPrintf(NULL, 0, 0);
        }
        this->unk_3B0.y += this->unk_41C;
        Play_CameraSetAtEyeUp(play, this->unk_39E, &this->unk_3B0, &this->unk_3A4, &this->unk_3BC);
    }
}

void func_808FF898(BossGanon2* this, PlayState* play) {
    if ((this->unk_312 != 0) && (this->unk_39E == 0)) {
        Actor* actor = play->actorCtx.actorLists[ACTORCAT_PROP].head;
        while (actor != NULL) {
            if (actor->id == ACTOR_DEMO_GJ) {
                DemoGj* gj = (DemoGj*)actor;

                if (((actor->params & 0xFF) == 0x10) || ((actor->params & 0xFF) == 0x11) ||
                    ((actor->params & 0xFF) == 0x16)) {
                    if (SQ(this->unk_218.x - gj->dyna.actor.world.pos.x) +
                            SQ(this->unk_218.z - gj->dyna.actor.world.pos.z) <
                        SQ(100.0f)) {
                        s32 pad;
                        Vec3f sp28;

                        Matrix_RotateY(((this->actor.shape.rot.y / (f32)0x8000) * M_PI) + 0.5f, MTXMODE_NEW);
                        sp28.x = 0.0f;
                        sp28.y = 0.0f;
                        sp28.z = 1.0f;
                        Matrix_MultVec3f(&sp28, &gj->unk_26C);
                        gj->killFlag = true;
                        func_800A9F6C(0.0f, 0x96, 0x14, 0x32);
                        this->unk_392 = 6;
                        return;
                    }
                }
            }

            actor = actor->next;
        }

        if (this->unk_392 == 4) {
            func_80078884(NA_SE_EV_GRAVE_EXPLOSION);
        }

        if (this->unk_392 == 3) {
            func_80078884(NA_SE_EN_MGANON_SWDIMP);
        }
    }
}

s32 func_808FFA24(BossGanon2* this, PlayState* play) {
    Actor* actor = play->actorCtx.actorLists[ACTORCAT_PROP].head;

    while (actor != NULL) {
        if (actor->id == ACTOR_DEMO_GJ) {
            DemoGj* gj = (DemoGj*)actor;

            if (((actor->params & 0xFF) == 0x10) || ((actor->params & 0xFF) == 0x11) ||
                ((actor->params & 0xFF) == 0x16)) {
                if (SQ(this->actor.world.pos.x - gj->dyna.actor.world.pos.x) +
                        SQ(this->actor.world.pos.z - gj->dyna.actor.world.pos.z) <
                    SQ(200.0f)) {
                    return true;
                }
            }
        }

        actor = actor->next;
    }

    return false;
}

void func_808FFAC8(BossGanon2* this, PlayState* play, u8 arg2) {
    s16 temp_v1;
    s16 phi_a1;

    if (this->unk_313 || (arg2 != 0)) {
        phi_a1 = this->actor.shape.rot.y - this->actor.yawTowardsPlayer;

        if (phi_a1 > 0x3000) {
            phi_a1 = 0x3000;
        } else if (phi_a1 < -0x3000) {
            phi_a1 = -0x3000;
        }
    } else if (this->unk_19C & 0x20) {
        phi_a1 = 0x3000;
    } else {
        phi_a1 = -0x3000;
    }

    Math_ApproachS(&this->unk_31A, phi_a1, 5, 0x7D0);

    temp_v1 = Math_Atan2S(this->actor.xzDistToPlayer, 150.0f) - 0xBB8;
    temp_v1 = CLAMP_MAX(temp_v1, 0x1B58);
    temp_v1 = CLAMP_MIN(temp_v1, -0x1B58);

    Math_ApproachS(&this->unk_31C, temp_v1, 5, 0x7D0);
}

void func_808FFBBC(BossGanon2* this, PlayState* play, u8 arg2) {
    if (arg2 != 0 || this->unk_313) {
        f32 phi_f0;
        f32 phi_f2;

        Math_ApproachS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 5, this->unk_320);

        if (this->unk_334 != 0) {
            phi_f0 = 5000.0f;
            phi_f2 = 200.0f;
        } else {
            phi_f0 = 3000.0f;
            phi_f2 = 30.0f;
        }

        Math_ApproachF(&this->unk_320, phi_f0, 1.0f, phi_f2);
    } else {
        this->unk_320 = 0.0f;
    }
}

void func_808FFC84(BossGanon2* this) {
    if (ABS((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y)) < 0x2800) {
        this->unk_313 = true;
        this->actor.focus.pos = this->unk_1B8;
    } else {
        this->unk_313 = false;
        this->actor.focus.pos = this->unk_1C4;
    }
}

void func_808FFCFC(BossGanon2* this, PlayState* play) {
    if (this->actor.xzDistToPlayer < 200.0f &&
        ABS((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y)) < 0x2800) {
        this->unk_311 = false;
        func_80900580(this, play, true);
        Audio_StopSfxById(NA_SE_EN_MGANON_UNARI);
    } else if ((this->actor.bgCheckFlags & 8) && func_808FFA24(this, play)) {
        this->unk_311 = false;
        func_80900580(this, play, true);
        Audio_StopSfxById(NA_SE_EN_MGANON_UNARI);
    }
}

void func_808FFDB0(BossGanon2* this, PlayState* play) {
    s32 sp28;
    s32 objectIdx = Object_GetIndex(&play->objectCtx, OBJECT_GANON2);

    if (Object_IsLoaded(&play->objectCtx, objectIdx)) {
        gSegments[6] = PHYSICAL_TO_VIRTUAL(play->objectCtx.status[objectIdx].segment);
        Animation_MorphToLoop(&this->skelAnime, &gGanonGuardIdleAnim, -10.0f);
        this->actionFunc = func_808FFEBC;

        if (this->unk_334 != 0) {
            this->unk_1A2[0] = Rand_ZeroFloat(30.0f);
        } else {
            this->unk_1A2[0] = 40;
        }

        this->unk_336 = 1;
        this->actor.flags |= ACTOR_FLAG_0;
        this->unk_228 = 1.0f;
        this->unk_224 = 1.0f;
    } else {
        this->actionFunc = func_808FFDB0;
    }
}

void func_808FFEBC(BossGanon2* this, PlayState* play) {
    if (this->unk_390 == 0) {
        this->unk_390 = (s16)Rand_ZeroFloat(50.0f) + 30;
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_UNARI);
    }

    SkelAnime_Update(&this->skelAnime);
    Math_ApproachZeroF(&this->actor.speedXZ, 0.5f, 1.0f);

    if ((this->actor.colChkInfo.health != MAX_HEALTH) || this->unk_1A2[0] == 0) {
        func_809002CC(this, play);
    } else if (this->unk_1A2[1] == 0) {
        func_808FFCFC(this, play);
    }

    func_808FFAC8(this, play, 0);
    func_808FFBBC(this, play, 0);
}

void func_808FFF90(BossGanon2* this, PlayState* play) {
    Animation_MorphToLoop(&this->skelAnime, &gGanonGuardIdleAnim, -10.0f);
    this->actionFunc = func_808FFFE0;
    this->unk_1A2[0] = 40;
}

void func_808FFFE0(BossGanon2* this, PlayState* play) {
    s16 target;

    SkelAnime_Update(&this->skelAnime);
    Math_ApproachZeroF(&this->actor.speedXZ, 0.5f, 1.0f);

    if (this->unk_1A2[0] == 0) {
        func_809002CC(this, play);
    }

    if (this->unk_1A2[0] < 30 && this->unk_1A2[0] >= 10) {
        target = Math_SinS(this->unk_1A2[0] * 0x3000) * (f32)0x2000;
    } else {
        target = 0;
    }

    Math_ApproachS(&this->unk_31A, target, 2, 0x4000);
}

void func_809000A0(BossGanon2* this, PlayState* play) {
    Animation_MorphToLoop(&this->skelAnime, &gGanonStunStartAnim, -2.0f);
    this->unk_194 = Animation_GetLastFrame(&gGanonStunStartAnim);
    this->unk_1AC = 0;
    this->actionFunc = func_80900104;
}

void func_80900104(BossGanon2* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    Math_ApproachZeroF(&this->actor.speedXZ, 0.5f, 1.0f);

    switch (this->unk_1AC) {
        case 0:
            if (Animation_OnFrame(&this->skelAnime, this->unk_194)) {
                this->unk_1AC = 1;
                Animation_MorphToLoop(&this->skelAnime, &gGanonStunLoopAnim, 0.0f);
                this->unk_1A2[0] = 80;
            }
            break;
        case 1:
            if (this->unk_1A2[0] == 0) {
                this->unk_1AC = 2;
                Animation_MorphToLoop(&this->skelAnime, &gGanonStunEndAnim, -5.0f);
                this->unk_194 = Animation_GetLastFrame(&gGanonStunEndAnim);
            }
            break;
        case 2:
            if (Animation_OnFrame(&this->skelAnime, this->unk_194)) {
                func_809002CC(this, play);
            }
            break;
    }
}

void func_80900210(BossGanon2* this, PlayState* play) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gGanonDamageAnim, -3.0f);
    this->unk_194 = Animation_GetLastFrame(&gGanonDamageAnim);
    this->actionFunc = func_8090026C;
}

void func_8090026C(BossGanon2* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    Math_ApproachZeroF(&this->actor.speedXZ, 0.5f, 2.0f);

    if (Animation_OnFrame(&this->skelAnime, this->unk_194)) {
        func_809002CC(this, play);
    }
}

void func_809002CC(BossGanon2* this, PlayState* play) {
    Animation_MorphToLoop(&this->skelAnime, &gGanonGuardWalkAnim, -10.0f);
    this->actionFunc = func_80900344;
    this->unk_338 = 0;
    this->unk_1A2[0] = 100;
    this->unk_390 = (s16)Rand_ZeroFloat(50.0f) + 50;
}

void func_80900344(BossGanon2* this, PlayState* play) {
    f32 phi_f0;

    if (this->unk_390 == 0) {
        this->unk_390 = (s16)Rand_ZeroFloat(50.0f) + 30;
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_UNARI);
    }

    Math_ApproachF(&this->unk_324, 255.0f, 1.0f, 10.0f);

    if (this->unk_338 != 0) {
        if (Animation_OnFrame(&this->skelAnime, 13.0f)) {
            func_808FD4D4(this, play, 1, 3);
        } else if (Animation_OnFrame(&this->skelAnime, 28.0f)) {
            func_808FD4D4(this, play, 2, 3);
        }
        if (this->actor.xzDistToPlayer < 200.0f) {
            this->unk_338 = 0;
            Animation_MorphToLoop(&this->skelAnime, &gGanonGuardWalkAnim, -10.0f);
        } else {
            this->skelAnime.playSpeed = ((this->actor.xzDistToPlayer - 300.0f) * 0.005f) + 1.0f;
            if (this->skelAnime.playSpeed > 2.0f) {
                this->skelAnime.playSpeed = 2.0f;
            }
            if (this->unk_334 != 0) {
                this->skelAnime.playSpeed *= 1.5f;
            }
        }
        phi_f0 = this->skelAnime.playSpeed * 3.0f;
    } else {
        phi_f0 = 2.0f;
        if (this->actor.xzDistToPlayer >= 200.0f) {
            this->unk_338 = 1;
            Animation_MorphToLoop(&this->skelAnime, &gGanonWalkAnim, -10.0f);
        }
    }

    SkelAnime_Update(&this->skelAnime);
    Math_ApproachF(&this->actor.speedXZ, phi_f0, 0.5f, 1.0f);

    if (this->unk_1A2[0] == 0) {
        func_808FFDB0(this, play);
    } else {
        func_808FFCFC(this, play);
    }

    func_808FFAC8(this, play, 1);
    func_808FFBBC(this, play, 1);
}

s16 gGanonSkelGganonleftswordswinganimAnimFrameData[3101] = {
	0x0000, 0xffff, 0x8046, 0x0088, 0xc069, 0x4000, 0xbfff, 0xfff9, 0xffe7, 0xffcb, 0xffa7, 0xff7e, 0xff51, 0xff22, 0xfef4, 0xfec9, 
	0xfea3, 0xfe83, 0xfe6c, 0xfe60, 0xfe8b, 0xfef2, 0xff5f, 0xff5f, 0xffaf, 0xffbf, 0xffcb, 0xffd6, 0xffde, 0xffe4, 0xffe9, 0xffed, 
	0xffef, 0xfff1, 0xfff2, 0xfff4, 0xfff5, 0xfff6, 0xfff9, 0xfffc, 0x0000, 0x2369, 0x2389, 0x23ba, 0x23f7, 0x243e, 0x248b, 0x24da, 
	0x2527, 0x256e, 0x25ac, 0x25dd, 0x25fd, 0x2609, 0x25a6, 0x24c8, 0x1ecf, 0x1ecf, 0x234a, 0x2337, 0x232c, 0x2328, 0x232a, 0x2330, 
	0x233a, 0x2346, 0x2353, 0x2360, 0x236c, 0x2375, 0x237c, 0x237e, 0x237a, 0x2370, 0x235d, 0xfff1, 0xffc6, 0xff87, 0xff38, 0xfedf, 
	0xfe83, 0xfe28, 0xfdd5, 0xfd90, 0xfd5e, 0xfd45, 0xfd4b, 0xfd76, 0xfea0, 0x00e1, 0x031b, 0x031b, 0x0440, 0x043c, 0x0424, 0x03fb, 
	0x03c2, 0x037d, 0x032d, 0x02d6, 0x0279, 0x021a, 0x01ba, 0x015d, 0x0104, 0x00b2, 0x006a, 0x002e, 0x0000, 0x3ea0, 0x3acc, 0x3503, 
	0x2e09, 0x26d0, 0x2066, 0x1bf0, 0x191d, 0x1704, 0x15f4, 0x1640, 0x182e, 0x1bf0, 0x2b72, 0xf7ec, 0xdd17, 0x584f, 0x5847, 0x57cb, 
	0x56ec, 0x55b9, 0x543f, 0x528e, 0x50b4, 0x4ebf, 0x4cbd, 0x4ab9, 0x48c2, 0x46e1, 0x4521, 0x438a, 0x4223, 0x40f3, 0x4000, 0xd90f, 
	0xdd32, 0xe308, 0xe9af, 0xf043, 0xf5e4, 0xf9b3, 0xfc34, 0xfe38, 0xff5c, 0xff40, 0xfd7f, 0xf9b3, 0xddfa, 0xc7fd, 0xbcfb, 0xcfa2, 
	0xcff6, 0xd045, 0xd092, 0xd0de, 0xd12d, 0xd180, 0xd1da, 0xd23d, 0xd2aa, 0xd322, 0xd3a5, 0xd434, 0xd4cc, 0xd56e, 0xd617, 0xd6c7, 
	0xd77c, 0x3f9c, 0x3ec0, 0x3dd0, 0x3cf1, 0x3c29, 0x3b7f, 0x3b06, 0x3aa7, 0x3a47, 0x3a04, 0x39fc, 0x3a4b, 0x3b06, 0x3f05, 0x8780, 
	0xa7a1, 0x2d8c, 0x2d9e, 0x2e07, 0x2ebb, 0x2fae, 0x30d4, 0x3223, 0x3390, 0x350f, 0x3696, 0x381b, 0x3995, 0x3afc, 0x3c48, 0x3d72, 
	0x3e77, 0x3f52, 0x4000, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xeab4, 
	0xe456, 0xea78, 0x8735, 0xe80d, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 
	0xf186, 0xf186, 0xf186, 0xf186, 0xf186, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 
	0xcab2, 0xd3ae, 0xda13, 0xcbd7, 0xe82f, 0xcc52, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 
	0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xcab2, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 
	0xb063, 0xb063, 0xb063, 0xbbba, 0xc763, 0xa4eb, 0xfa50, 0xc307, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 
	0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 0xb063, 0x08e6, 0x09e5, 0x0b69, 0x0d52, 0x0f84, 0x11df, 0x1446, 
	0x1699, 0x18ba, 0x1a8c, 0x1bef, 0x1cc5, 0x1cf0, 0x1995, 0x190f, 0x0a39, 0x3c57, 0x034d, 0x0306, 0x039c, 0x047a, 0x0509, 0x052f, 
	0x053e, 0x053f, 0x053b, 0x053b, 0x0547, 0x0568, 0x05a8, 0x0610, 0x06a7, 0x0778, 0x088b, 0xe48b, 0xe424, 0xe38c, 0xe2d3, 0xe208, 
	0xe13c, 0xe07f, 0xdfe0, 0xdf70, 0xdf3f, 0xdf5d, 0xdfd9, 0xe0c4, 0xe49b, 0xe087, 0xf363, 0xcebe, 0xfa30, 0xfac4, 0xfa6a, 0xf97c, 
	0xf853, 0xf6ed, 0xf51b, 0xf2fc, 0xf0ab, 0xee46, 0xebe9, 0xe9b1, 0xe7ba, 0xe622, 0xe506, 0xe481, 0xe4b1, 0xcffc, 0xcdf8, 0xcae5, 
	0xc700, 0xc288, 0xbdb7, 0xb8cc, 0xb403, 0xaf99, 0xabca, 0xa8d5, 0xa6f4, 0xa666, 0xaca9, 0xb231, 0xc9f4, 0xc7eb, 0xd721, 0xd797, 
	0xd66c, 0xd4cb, 0xd3dd, 0xd39d, 0xd342, 0xd2d3, 0xd258, 0xd1da, 0xd161, 0xd0f5, 0xd09d, 0xd064, 0xd04f, 0xd068, 0xd0b6, 0x218a, 
	0x2257, 0x2388, 0x24fd, 0x2696, 0x2836, 0x29bc, 0x2b0a, 0x2c01, 0x2c82, 0x2c6e, 0x2ba5, 0x2a08, 0x2273, 0x0c17, 0xf97e, 0xf9a8, 
	0xfab1, 0xf9a4, 0xfa51, 0xfc0b, 0xfe25, 0x00a0, 0x03ce, 0x077f, 0x0b86, 0x0fb0, 0x13d0, 0x17b5, 0x1b31, 0x1e12, 0x202a, 0x2148, 
	0x213e, 0xea07, 0xe964, 0xe86c, 0xe72f, 0xe5c1, 0xe434, 0xe29b, 0xe107, 0xdf8a, 0xde39, 0xdd23, 0xdc5d, 0xdbf8, 0xe52e, 0xefee, 
	0xed4d, 0xf7ef, 0xe7f4, 0xe7ff, 0xe7a7, 0xe745, 0xe72f, 0xe781, 0xe80c, 0xe8c0, 0xe989, 0xea55, 0xeb12, 0xebad, 0xec15, 0xec37, 
	0xec00, 0xeb5f, 0xea41, 0xeb90, 0xea1a, 0xe7e7, 0xe52b, 0xe217, 0xdede, 0xdbb2, 0xd8c6, 0xd64b, 0xd474, 0xd374, 0xd37b, 0xd4be, 
	0xf768, 0xf450, 0x08bc, 0x0919, 0x1167, 0x1287, 0x117d, 0x0f55, 0x0d16, 0x0aba, 0x07a9, 0x0413, 0x002a, 0xfc1d, 0xf820, 0xf462, 
	0xf115, 0xee69, 0xec8f, 0xebb9, 0xec18, 0x1ac2, 0x19a5, 0x1817, 0x1658, 0x1456, 0x11ef, 0x0f4f, 0x0ca2, 0x0a11, 0x07ca, 0x05f6, 
	0x04c0, 0x0677, 0x1ad9, 0x20c3, 0x3033, 0x1aa5, 0x08a7, 0x06bd, 0x032a, 0xfc92, 0xf404, 0xeb0c, 0xe336, 0xde0f, 0xdd24, 0xe0c9, 
	0xe79f, 0xf0a3, 0xfad4, 0x0530, 0x0eb4, 0x165f, 0x1b2f, 0xfa3e, 0xfffe, 0x05d2, 0x08a5, 0x087a, 0x0785, 0x05f7, 0x0406, 0x01e5, 
	0xffc5, 0xfdde, 0xfc61, 0x0350, 0x0766, 0xfb5c, 0x295d, 0x3d19, 0x1b81, 0x21db, 0x2360, 0x2048, 0x1b03, 0x1489, 0x0dd1, 0x07d4, 
	0x0388, 0x0091, 0xfdf0, 0xfbb2, 0xf9e0, 0xf885, 0xf7ab, 0xf75c, 0xf7a4, 0x0d5c, 0x0fa2, 0x11a3, 0x11e8, 0x1038, 0x0d90, 0x0a4d, 
	0x06cb, 0x0366, 0x007b, 0xfe64, 0xfd80, 0xfff9, 0x0c92, 0x32e7, 0x4db3, 0x3255, 0x2444, 0x24ca, 0x2382, 0x207f, 0x1c78, 0x17fe, 
	0x13a0, 0x0fef, 0x0d78, 0x0c2d, 0x0b83, 0x0b53, 0x0b77, 0x0bc8, 0x0c20, 0x0c57, 0x0c47, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 
	0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff54, 0xf9f1, 0xff62, 0xff62, 0xff62, 0xff62, 
	0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0xff62, 0x0010, 0x0010, 0x0010, 
	0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x001d, 0x031b, 0x0010, 0x0010, 
	0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x400d, 
	0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x3e6d, 0x3929, 
	0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 0x400d, 
	0x400d, 0xfff5, 0xffd9, 0xffab, 0xff6f, 0xff26, 0xfed1, 0xfe73, 0xfe0c, 0xfda0, 0xfd30, 0xfcbd, 0xfc49, 0xfbd6, 0xfb67, 0xfafc, 
	0xfa97, 0xfa3c, 0xf9ea, 0xf9a4, 0xf96c, 0xf944, 0xf92e, 0xf92a, 0xf93c, 0xf965, 0xf9a6, 0xfa02, 0xfa7b, 0xfb11, 0xfbc7, 0xfc9e, 
	0xfd99, 0xfeb9, 0x0000, 0x1dc3, 0x1db8, 0x1da7, 0x1d90, 0x1d75, 0x1d55, 0x1d31, 0x1d0b, 0x1ce2, 0x1cb8, 0x1c8d, 0x1c62, 0x1c38, 
	0x1c0e, 0x1be7, 0x1bc2, 0x1ba0, 0x1b81, 0x1b68, 0x1b53, 0x1b45, 0x1b3c, 0x1b3b, 0x1b42, 0x1b51, 0x1b69, 0x1b8a, 0x1bb7, 0x1bee, 
	0x1c32, 0x1c82, 0x1ce0, 0x1d4c, 0x1dc6, 0x986d, 0x986f, 0x9872, 0x9876, 0x987c, 0x9882, 0x9889, 0x9891, 0x9899, 0x98a2, 0x98ab, 
	0x98b4, 0x98bd, 0x98c6, 0x98cf, 0x98d7, 0x98de, 0x98e5, 0x98eb, 0x98ef, 0x98f3, 0x98f5, 0x98f5, 0x98f3, 0x98f0, 0x98eb, 0x98e3, 
	0x98d9, 0x98cd, 0x98be, 0x98ad, 0x989a, 0x9884, 0x986c, 0xfbe7, 0xfbd6, 0xfbbb, 0xfb98, 0xfb6e, 0xfb40, 0xfb0e, 0xfadb, 0xfaa8, 
	0xfa76, 0xfa48, 0xfa1e, 0xf9fb, 0xf9e0, 0xf9ce, 0xf9c8, 0xf9d5, 0xf9fb, 0xfa34, 0xfa7d, 0xfad2, 0xfb2e, 0xfb8d, 0xfbec, 0xfc46, 
	0xfc97, 0xfcdb, 0xfd0e, 0xfd2b, 0xfd2f, 0xfd15, 0xfcda, 0xfc79, 0xfbee, 0xfbfb, 0xfc24, 0xfc64, 0xfcb6, 0xfd18, 0xfd85, 0xfdf9, 
	0xfe71, 0xfeea, 0xff5e, 0xffcb, 0x002e, 0x0080, 0x00c0, 0x00e9, 0x00f7, 0x00fa, 0x0101, 0x010a, 0x0112, 0x0117, 0x0117, 0x010e, 
	0x00fb, 0x00dc, 0x00ad, 0x006c, 0x0017, 0xffab, 0xff27, 0xfe88, 0xfdca, 0xfced, 0xfbed, 0xe872, 0xe89c, 0xe8dd, 0xe932, 0xe997, 
	0xea07, 0xea7f, 0xeafb, 0xeb77, 0xebef, 0xec5f, 0xecc3, 0xed18, 0xed5a, 0xed84, 0xed93, 0xed8d, 0xed7e, 0xed64, 0xed41, 0xed15, 
	0xece0, 0xeca3, 0xec5e, 0xec12, 0xebbf, 0xeb66, 0xeb06, 0xeaa1, 0xea37, 0xe9c8, 0xe955, 0xe8dd, 0xe863, 0xc463, 0xc8f0, 0xcff5, 
	0xd8f7, 0xe381, 0xef1a, 0xfb49, 0x0799, 0x138f, 0x1eb5, 0x2891, 0x30ae, 0x3692, 0x3707, 0x3117, 0x27ab, 0x1dad, 0x1605, 0x104e, 
	0x0a41, 0x03f2, 0xfd79, 0xf6ed, 0xf063, 0xe9f2, 0xe3b0, 0xddb4, 0xd813, 0xd2e6, 0xce40, 0xca3b, 0xc6ea, 0xc466, 0xc2c3, 0x277a, 
	0x2761, 0x273a, 0x2705, 0x26c3, 0x2675, 0x261c, 0x25ba, 0x254f, 0x24db, 0x2461, 0x23e2, 0x235d, 0x2272, 0x2106, 0x1f87, 0x1e65, 
	0x1e0e, 0x1e60, 0x1eed, 0x1fa8, 0x2088, 0x2183, 0x228e, 0x239e, 0x24aa, 0x25a6, 0x2689, 0x2749, 0x27da, 0x2833, 0x2848, 0x2811, 
	0x2782, 0xcb11, 0xce39, 0xd313, 0xd94b, 0xe08d, 0xe883, 0xf0db, 0xf93e, 0x0159, 0x08d6, 0x0f61, 0x14a5, 0x184f, 0x17a8, 0x11e8, 
	0x0997, 0x013f, 0xfb69, 0xf790, 0xf387, 0xef5d, 0xeb22, 0xe6e6, 0xe2ba, 0xdead, 0xdace, 0xd72f, 0xd3df, 0xd0ed, 0xce6a, 0xcc66, 
	0xcaf1, 0xca1a, 0xc9f1, 0xe59f, 0xda81, 0xd008, 0xccca, 0xd36b, 0xdfb9, 0xee05, 0xfaa1, 0x01e1, 0x0493, 0x061e, 0x06b1, 0x067a, 
	0x05a7, 0x0467, 0x02e9, 0x015b, 0xffea, 0xfec8, 0xfdb6, 0xfc62, 0xfad9, 0xf928, 0xf75c, 0xf582, 0xf3a7, 0xf1d8, 0xf022, 0xee93, 
	0xed36, 0xec1a, 0xeb4b, 0xead7, 0xeaca, 0x0f3d, 0x0eff, 0x0e5d, 0x0d27, 0x0ac8, 0x0770, 0x0415, 0x01ad, 0x012f, 0x026e, 0x0478, 
	0x0719, 0x0a1c, 0x0d4e, 0x107a, 0x136d, 0x15f2, 0x17d4, 0x18e1, 0x194a, 0x196a, 0x1948, 0x18ed, 0x1862, 0x17ad, 0x16d6, 0x15e7, 
	0x14e7, 0x13df, 0x12d5, 0x11d2, 0x10df, 0x1003, 0x0f47, 0x068a, 0x03e5, 0x014b, 0x0038, 0x013b, 0x0365, 0x05fa, 0x083c, 0x096e, 
	0x09b0, 0x09ac, 0x096f, 0x090a, 0x0889, 0x07fd, 0x0772, 0x06f8, 0x069e, 0x0671, 0x0681, 0x06cc, 0x0743, 0x07da, 0x0884, 0x0933, 
	0x09da, 0x0a6c, 0x0adb, 0x0b1b, 0x0b1e, 0x0ad7, 0x0a38, 0x0935, 0x07c1, 0x0099, 0x0242, 0x04c9, 0x07fa, 0x0ba3, 0x0f92, 0x1393, 
	0x1775, 0x1b04, 0x1e0d, 0x205f, 0x21c6, 0x2210, 0x1bf7, 0x0e20, 0xffde, 0xfa4b, 0xf8dd, 0xf7cc, 0xf711, 0xf6a5, 0xf680, 0xf69b, 
	0xf6ef, 0xf773, 0xf822, 0xf8f2, 0xf9de, 0xfade, 0xfbe9, 0xfcfa, 0xfe08, 0xff0c, 0x0000, 0xff54, 0xfd78, 0xfaa8, 0xf71b, 0xf30e, 
	0xeeba, 0xea58, 0xe625, 0xe259, 0xdf2f, 0xdce1, 0xdbaa, 0xdbc3, 0xe3c7, 0xf3a9, 0x03f9, 0x0e23, 0x0f95, 0x107e, 0x10eb, 0x10ea, 
	0x1087, 0x0fce, 0x0ecc, 0x0d8e, 0x0c21, 0x0a91, 0x08eb, 0x073c, 0x0590, 0x03f4, 0x0275, 0x0120, 0xffff, 0x1c74, 0x1b26, 0x1928, 
	0x16a4, 0x13c0, 0x10a5, 0x0d79, 0x0a64, 0x078f, 0x0520, 0x033f, 0x0214, 0x01c6, 0x065a, 0xfb75, 0x098e, 0x1ffc, 0x2114, 0x21ce, 
	0x2233, 0x2250, 0x222e, 0x21d9, 0x215a, 0x20be, 0x200f, 0x1f57, 0x1ea2, 0x1df9, 0x1d69, 0x1cfb, 0x1cba, 0x1cb2, 0x1cec, 0xf049, 
	0xf169, 0xf2ff, 0xf4f4, 0xf730, 0xf99a, 0xfc1c, 0xfe9f, 0x0112, 0x0365, 0x0594, 0x07aa, 0x09d3, 0x917b, 0xb4b2, 0xc7b6, 0xc9c3, 
	0xcbaf, 0xce8b, 0xd1fc, 0xd588, 0xd8a3, 0xdaac, 0xdbc3, 0xdc92, 0xdd28, 0xdd8d, 0xddc5, 0xddc7, 0xdd73, 0xdc75, 0xd9c3, 0xd0d0, 
	0x8c1b, 0xbfd5, 0xbf62, 0xbeb5, 0xbddd, 0xbcea, 0xbbea, 0xbaed, 0xba02, 0xb937, 0xb89b, 0xb83d, 0xb829, 0xb86a, 0xc5a1, 0xc49d, 
	0xc872, 0xcbd4, 0xcd84, 0xcebe, 0xcf8a, 0xcfeb, 0xcfe1, 0xcf6e, 0xce87, 0xcd36, 0xcb94, 0xc9bd, 0xc7cd, 0xc5de, 0xc40d, 0xc273, 
	0xc12e, 0xc057, 0xbfff, 0x9c2a, 0x9be0, 0x9b9a, 0x9b4c, 0x9af2, 0x9a89, 0x9a0d, 0x9976, 0x98bb, 0x97cc, 0x9691, 0x94e2, 0x9274, 
	0x0698, 0xdaae, 0xbe75, 0xb6c7, 0xb351, 0xb071, 0xae04, 0xac05, 0xaa7e, 0xa992, 0xa923, 0xa8e4, 0xa8d0, 0xa8e5, 0xa92c, 0xa9b5, 
	0xaaa8, 0xac63, 0xaff9, 0xb9ff, 0x0000, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 
	0xbfff, 0xbfff, 0xbfb7, 0xbf6f, 0xbfff, 0xc1cf, 0xc45d, 0xc720, 0xc993, 0xcb2d, 0xcb68, 0xc874, 0xc2b1, 0xbcd9, 0xb9a9, 0xb910, 
	0xb918, 0xb9a5, 0xba97, 0xbbd2, 0xbd36, 0xbea4, 0xbfff, 0xe69b, 0xe64b, 0xe5d8, 0xe551, 0xe4c4, 0xe442, 0xe3d8, 0xe397, 0xe38d, 
	0xe3c9, 0xe45b, 0xe551, 0xe6ba, 0xedf6, 0xf9fb, 0x0136, 0x01b7, 0x0060, 0xfdd6, 0xfabf, 0xf7c0, 0xf57e, 0xf400, 0xf2c4, 0xf1a5, 
	0xf07c, 0xef2b, 0xedbc, 0xec40, 0xeacb, 0xe971, 0xe843, 0xe755, 0xe6ba, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 
	0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 
	0xbfff, 0xbfff, 0xbfff, 0xbffc, 0xbff6, 0xbfee, 0xbfe6, 0xbfe1, 0xbfe1, 0xbfeb, 0xbfff, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 
	0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0031, 0x0082, 0x0092, 
	0xffff, 0xfe65, 0xfc0d, 0xf997, 0xf7a6, 0xf6dc, 0xf760, 0xf8b2, 0xfa80, 0xfc75, 0xfe3f, 0xff88, 0xffff, 0x0000, 0x0000, 0x0000, 
	0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0x0084, 0x0109, 0x0000, 0xfc4f, 
	0xf723, 0xf251, 0xefb3, 0xefc0, 0xf143, 0xf390, 0xf5fc, 0xf7d8, 0xf934, 0xfa8b, 0xfbd7, 0xfd0f, 0xfe2e, 0xff2b, 0xffff, 0xd933, 
	0xd904, 0xd8bf, 0xd86b, 0xd80f, 0xd7b4, 0xd760, 0xd71b, 0xd6ec, 0xd6da, 0xd6ee, 0xd72e, 0xd7a3, 0xd852, 0xd944, 0xde2e, 0xe6b7, 
	0xecd3, 0xeebc, 0xef15, 0xee77, 0xed77, 0xec1c, 0xea33, 0xe803, 0xe5d3, 0xe3ea, 0xe240, 0xe0a2, 0xdf0f, 0xdd88, 0xdc0e, 0xdaa2, 
	0xd944, 0x2a28, 0x2af0, 0x2c1a, 0x2d8a, 0x2f24, 0x30cb, 0x3263, 0x33d0, 0x34f5, 0x35b6, 0x35f7, 0x359b, 0x3486, 0x2e30, 0x221f, 
	0x164d, 0x10b2, 0x1067, 0x1085, 0x1101, 0x11d2, 0x12f0, 0x144f, 0x15e7, 0x17af, 0x199c, 0x1ba5, 0x1dc1, 0x1fe7, 0x220c, 0x2427, 
	0x262f, 0x281a, 0x29e0, 0x0009, 0x0022, 0x0046, 0x0070, 0x009b, 0x00c3, 0x00e3, 0x00f7, 0x00f9, 0x00e4, 0x00b5, 0x0067, 0xfff3, 
	0xfed8, 0xfce2, 0xfa8d, 0xf850, 0xf6a5, 0xf604, 0xf671, 0xf77e, 0xf8fc, 0xfabd, 0xfc91, 0xfe4a, 0xffb9, 0x00b0, 0x0124, 0x013f, 
	0x0118, 0x00cb, 0x0072, 0x0025, 0x0000, 0xfff5, 0xffdc, 0xffb8, 0xff8f, 0xff68, 0xff46, 0xff31, 0xff2e, 0xff42, 0xff73, 0xffc7, 
	0x0043, 0x00ed, 0x028b, 0x0566, 0x08bf, 0x0bda, 0x0dfc, 0x0e66, 0x0cfc, 0x0a60, 0x06f5, 0x031e, 0xff3e, 0xfbb9, 0xf8f1, 0xf74a, 
	0xf722, 0xf841, 0xfa2a, 0xfc62, 0xfe6b, 0xffc9, 0x0000, 0xdd38, 0xdcf0, 0xdc86, 0xdc06, 0xdb7c, 0xdaf3, 0xda76, 0xda13, 0xd9d3, 
	0xd9c3, 0xd9ee, 0xda60, 0xdb25, 0xdd14, 0xe085, 0xe4c4, 0xe91d, 0xecdb, 0xef4a, 0xf0aa, 0xf1b5, 0xf269, 0xf2c6, 0xf2cb, 0xf277, 
	0xf1cb, 0xf0c5, 0xeeec, 0xec0d, 0xe896, 0xe4f4, 0xe195, 0xdee5, 0xdd53, 0xfffe, 0xfffc, 0xfff8, 0xfff3, 0xffee, 0xffe8, 0xffe3, 
	0xffde, 0xffd9, 0xffd6, 0xffd4, 0xffd3, 0xffd5, 0x0059, 0x0137, 0x017b, 0x002f, 0xfc97, 0xf769, 0xf1d6, 0xed0b, 0xea39, 0xe9cc, 
	0xeaf9, 0xed1f, 0xef99, 0xf1c7, 0xf3ce, 0xf624, 0xf89a, 0xfb00, 0xfd27, 0xfee1, 0x0000, 0x0003, 0x000a, 0x0014, 0x001e, 0x0027, 
	0x002c, 0x002b, 0x0023, 0x0010, 0xfff1, 0xffc5, 0xff89, 0xff3b, 0xfdca, 0xfb50, 0xf987, 0xfa2b, 0xfea8, 0x05cf, 0x0d66, 0x1338, 
	0x1510, 0x1110, 0x08b2, 0xfec0, 0xf608, 0xf156, 0xf0a9, 0xf1cb, 0xf434, 0xf75c, 0xfabb, 0xfdca, 0x0000, 0xec0a, 0xeca4, 0xed8b, 
	0xeea9, 0xefe8, 0xf132, 0xf272, 0xf393, 0xf47d, 0xf51d, 0xf55c, 0xf524, 0xf460, 0xee84, 0xe398, 0xda63, 0xd9ac, 0xe47c, 0xf64f, 
	0x0a9e, 0x1cdf, 0x288b, 0x2cb2, 0x2c58, 0x28e8, 0x23c9, 0x1e64, 0x1833, 0x104a, 0x077d, 0xfe9c, 0xf67f, 0xeff5, 0xebd1, 0x1e0e, 
	0x1d1b, 0x1b8d, 0x1960, 0x1696, 0x1344, 0x0f9d, 0x0bf1, 0x089f, 0x05ff, 0x045f, 0x0403, 0x0546, 0x1208, 0x2339, 0x295a, 0x2ab7, 
	0x2ac9, 0x2ad5, 0x2ada, 0x2ad4, 0x2abf, 0x2a97, 0x2a56, 0x29f5, 0x296e, 0x28b9, 0x27d0, 0x26ad, 0x2552, 0x23c2, 0x2209, 0x2037, 
	0x1e62, 0x2523, 0x25dc, 0x26e8, 0x281f, 0x2959, 0x2a71, 0x2b4d, 0x2be2, 0x2c3b, 0x2c72, 0x2ca9, 0x2cfd, 0x2d73, 0x2d77, 0x265d, 
	0x1bc3, 0x1670, 0x15e9, 0x15c7, 0x15fe, 0x1684, 0x174f, 0x1854, 0x1988, 0x1ae0, 0x1c50, 0x1dc9, 0x1f3f, 0x20a2, 0x21e6, 0x22fd, 
	0x23de, 0x247f, 0x24de, 0x0165, 0x003c, 0xfe53, 0xfbaa, 0xf843, 0xf43a, 0xefcd, 0xeb5c, 0xe758, 0xe434, 0xe251, 0xe20d, 0xe3d4, 
	0xf3bc, 0x0943, 0x110f, 0x1212, 0x11ed, 0x11c1, 0x118f, 0x1153, 0x1107, 0x10a5, 0x1023, 0x0f7a, 0x0ea2, 0x0d96, 0x0c51, 0x0ad3, 
	0x0923, 0x074e, 0x0565, 0x0385, 0x01cc, 0x049e, 0x044c, 0x03ed, 0x03a5, 0x0397, 0x059c, 0x0572, 0x0067, 0xfa21, 0xf31e, 0xebd9, 
	0xe4cf, 0xde7c, 0xd95b, 0xd5ea, 0xd4a4, 0xd4f9, 0xd5f6, 0xd78b, 0xd9a3, 0xdc2d, 0xdf17, 0xe24d, 0xe5be, 0xe956, 0xed04, 0xf0b4, 
	0xf455, 0xf7d3, 0xfb1c, 0xfe1e, 0x00c8, 0x0304, 0x04c1, 0xefcd, 0xefa0, 0xef75, 0xef69, 0xef95, 0xeff5, 0xf218, 0xf67f, 0xfbc6, 
	0x0190, 0x0779, 0x0d22, 0x122c, 0x1637, 0x18e1, 0x19cc, 0x196d, 0x1883, 0x171e, 0x154b, 0x131c, 0x109d, 0x0dde, 0x0aef, 0x07dd, 
	0x04b8, 0x018f, 0xfe70, 0xfb6c, 0xf890, 0xf5eb, 0xf38e, 0xf185, 0xefe1, 0xeed6, 0xefe1, 0xf116, 0xf207, 0xf246, 0xef81, 0xeca7, 
	0xec5a, 0xec41, 0xec51, 0xec7f, 0xecc1, 0xed0a, 0xed51, 0xed8b, 0xedab, 0xedbf, 0xedd7, 0xedf3, 0xee11, 0xee32, 0xee52, 0xee72, 
	0xee90, 0xeeac, 0xeec3, 0xeed5, 0xeee1, 0xeee5, 0xeee1, 0xeed3, 0xeeba, 0xee96, 0xee65, 0xfa95, 0xfbc1, 0xfd50, 0xfeec, 0x0043, 
	0x018b, 0x031e, 0x04d4, 0x0686, 0x080c, 0x093e, 0x09f7, 0x0a0e, 0x095b, 0x0625, 0x0015, 0xf91e, 0xf335, 0xf04c, 0xefc5, 0xefac, 
	0xeff1, 0xf086, 0xf15b, 0xf25f, 0xf385, 0xf4bc, 0xf5f6, 0xf722, 0xf832, 0xf915, 0xf9bd, 0xfa1b, 0xfa1e, 0xf384, 0xf2bb, 0xf1ef, 
	0xf18a, 0xf1f9, 0xf30e, 0xf456, 0xf5cc, 0xf76d, 0xf936, 0xfb23, 0xfd31, 0xff5d, 0x01a3, 0x04fb, 0x098e, 0x0e24, 0x1183, 0x1272, 
	0x117c, 0x1006, 0x0e25, 0x0bee, 0x0974, 0x06cc, 0x040a, 0x0143, 0xfe89, 0xfbf4, 0xf996, 0xf784, 0xf5d2, 0xf494, 0xf3df, 0x053a, 
	0x02c1, 0xff47, 0xfb57, 0xf777, 0xf2f7, 0xed30, 0xe6ba, 0xe02d, 0xda21, 0xd52e, 0xd1ed, 0xd0f5, 0xd2de, 0xddb3, 0xf28e, 0x0aa1, 
	0x1f18, 0x2924, 0x2b16, 0x2be2, 0x2ba6, 0x2a83, 0x2899, 0x2608, 0x22f0, 0x1f71, 0x1bab, 0x17bd, 0x13c8, 0x0fec, 0x0c48, 0x08fd, 
	0x062b, 0xdefa, 0xe038, 0xe225, 0xe4a1, 0xe78c, 0xeac2, 0xee15, 0xf151, 0xf43a, 0xf68c, 0xf7fb, 0xf830, 0xf6cf, 0xebc5, 0xde3a, 
	0xd8c6, 0xd714, 0xd66f, 0xd5d7, 0xd552, 0xd4e5, 0xd49a, 0xd478, 0xd487, 0xd4cd, 0xd551, 0xd612, 0xd70d, 0xd83b, 0xd98b, 0xdaea, 
	0xdc43, 0xdd7e, 0xde88, 0x5cf3, 0x5c85, 0x5be3, 0x5b1d, 0x5a46, 0x5969, 0x5890, 0x57bf, 0x56f4, 0x562d, 0x5567, 0x54aa, 0x5411, 
	0x5523, 0x5ccf, 0x6719, 0x6c2b, 0x6c4c, 0x6c03, 0x6b5f, 0x6a70, 0x6946, 0x67f1, 0x6683, 0x650a, 0x6397, 0x6239, 0x60fb, 0x5fe7, 
	0x5f03, 0x5e4f, 0x5dc8, 0x5d65, 0x5d1c, 0xfe28, 0xfee3, 0x0005, 0x017d, 0x0337, 0x051a, 0x0700, 0x08be, 0x0a1a, 0x0ad5, 0x0aa5, 
	0x0937, 0x0634, 0xf84a, 0xe74e, 0xe037, 0xdef2, 0xdf68, 0xe00a, 0xe0d9, 0xe1d6, 0xe304, 0xe469, 0xe607, 0xe7e4, 0xe9fe, 0xec54, 
	0xeedd, 0xf18b, 0xf44a, 0xf702, 0xf996, 0xfbea, 0xfde5, 0xfd8f, 0xfe0f, 0xfed9, 0xffe2, 0x0123, 0x0290, 0x0420, 0x05c9, 0x0782, 
	0x0941, 0x0afc, 0x0cab, 0x0e43, 0x0fba, 0x1107, 0x130e, 0x11ed, 0x0a64, 0x0050, 0xf727, 0xf263, 0xf295, 0xf548, 0xf96b, 0xfdee, 
	0x01c2, 0x03d5, 0x0438, 0x03d4, 0x02dd, 0x0188, 0x000a, 0xfe96, 0xfd62, 0x17c0, 0x170a, 0x15ec, 0x1476, 0x12b5, 0x10b8, 0x0e8e, 
	0x0c45, 0x09ec, 0x0792, 0x0545, 0x0314, 0x010d, 0xff3e, 0xfdb7, 0xfdc3, 0x00c3, 0x0710, 0x0f0b, 0x15eb, 0x18e5, 0x1741, 0x1301, 
	0x0d6f, 0x07d6, 0x0381, 0x01ba, 0x02cc, 0x05b3, 0x09c1, 0x0e49, 0x129f, 0x1616, 0x1800, 0xeb2a, 0xeb3a, 0xeb53, 0xeb74, 0xeb99, 
	0xebc3, 0xebee, 0xec19, 0xec42, 0xec69, 0xec89, 0xeca4, 0xecb5, 0xecbc, 0xecb7, 0xeb2c, 0xea5c, 0xec4f, 0xef0c, 0xf225, 0xf52c, 
	0xf892, 0xfca2, 0x00bb, 0x0439, 0x067a, 0x06db, 0x0517, 0x01b2, 0xfd35, 0xf82c, 0xf31f, 0xee99, 0xeb24, 0x09c8, 0x0983, 0x0915, 
	0x0883, 0x07d1, 0x0703, 0x061f, 0x0527, 0x0421, 0x0312, 0x01fc, 0x00e6, 0xffd1, 0xfec5, 0xfdc5, 0xfcac, 0xfb59, 0xf9d8, 0xf833, 
	0xf675, 0xf422, 0xf11c, 0xedff, 0xeb6a, 0xe9f8, 0xea46, 0xec7b, 0xf00c, 0xf489, 0xf986, 0xfe95, 0x0348, 0x0730, 0x09e0, 0x0d02, 
	0x0ca3, 0x0c0f, 0x0b50, 0x0a6e, 0x0972, 0x0864, 0x074f, 0x063a, 0x052f, 0x0436, 0x0358, 0x029f, 0x0212, 0x01bb, 0x028a, 0x04b2, 
	0x0725, 0x08d4, 0x08b1, 0x0603, 0x017f, 0xfc34, 0xf732, 0xf38a, 0xf24c, 0xf38e, 0xf660, 0xfa46, 0xfec6, 0x0365, 0x07a5, 0x0b0e, 
	0x0d23, 0x03af, 0x036b, 0x02ff, 0x026b, 0x01b3, 0x00da, 0xffe0, 0xfecc, 0xfd9d, 0xfc58, 0xfafe, 0xf992, 0xf817, 0xf68f, 0xf4fe, 
	0xf1e2, 0xece4, 0xe7c3, 0xe43f, 0xe418, 0xe837, 0xef5c, 0xf808, 0x00bb, 0x07f4, 0x0c34, 0x0d69, 0x0ce0, 0x0b24, 0x08c4, 0x064a, 
	0x0444, 0x033f, 0x03c6, 0xce86, 0xcf60, 0xd0b1, 0xd263, 0xd460, 0xd68f, 0xd8d0, 0xdb00, 0xdcf4, 0xde7d, 0xdf69, 0xdf85, 0xdea3, 
	0xd748, 0xca0f, 0xbe88, 0xb999, 0xb962, 0xb997, 0xba2a, 0xbb0d, 0xbc34, 0xbd93, 0xbf20, 0xc0d0, 0xc296, 0xc468, 0xc639, 0xc7fd, 
	0xc9a7, 0xcb29, 0xcc76, 0xcd80, 0xce38, 0xffb9, 0xfef8, 0xfdd6, 0xfc6b, 0xfad3, 0xf928, 0xf784, 0xf601, 0xf4b8, 0xf3be, 0xf328, 
	0xf30a, 0xf379, 0xf79d, 0x001a, 0x08c2, 0x0d0a, 0x0d7c, 0x0d9b, 0x0d72, 0x0d07, 0x0c65, 0x0b93, 0x0a9a, 0x0984, 0x0858, 0x071f, 
	0x05e2, 0x04a9, 0x037e, 0x0268, 0x0170, 0x00a0, 0xffff, 0xbfd1, 0xbf50, 0xbe8c, 0xbd92, 0xbc6f, 0xbb31, 0xb9e5, 0xb89a, 0xb761, 
	0xb64c, 0xb572, 0xb4e8, 0xb4c2, 0xb6af, 0xba69, 0xbe06, 0xc009, 0xc083, 0xc0df, 0xc121, 0xc149, 0xc15c, 0xc15c, 0xc14d, 0xc131, 
	0xc10e, 0xc0e4, 0xc0b7, 0xc08a, 0xc05f, 0xc039, 0xc01b, 0xc007, 0xbfff, 0xf104, 0xefc7, 0xed9a, 0xea4d, 0xe5b1, 0xdfca, 0xd901, 
	0xd21a, 0xcbd4, 0xc689, 0xc23c, 0xbede, 0xbc9d, 0xc452, 0xe00a, 0xf3a6, 0xf3a6, 0xe46d, 0xe512, 0xe5f2, 0xe6fd, 0xe822, 0xe953, 
	0xea83, 0xeba7, 0xecb8, 0xedad, 0xee85, 0xef3e, 0xefda, 0xf05b, 0xf0c7, 0xf122, 0xf16d, 0xde68, 0xdd3c, 0xdb8f, 0xd9ab, 0xd7e7, 
	0xd69d, 0xd615, 0xd662, 0xd751, 0xd87c, 0xd962, 0xd982, 0xd881, 0xd231, 0xd5d5, 0xe18f, 0xe18f, 0xebda, 0xec47, 0xec42, 0xebda, 
	0xeb1d, 0xea1c, 0xe8e5, 0xe78a, 0xe61b, 0xe4ab, 0xe34b, 0xe20b, 0xe0f8, 0xe01c, 0xdf79, 0xdf0f, 0xded7, 0xd992, 0xdb7e, 0xdeb3, 
	0xe34c, 0xe95e, 0xf0c7, 0xf8fd, 0x010e, 0x080f, 0x0d76, 0x111c, 0x12fd, 0x12fc, 0x038f, 0xdc90, 0xbf01, 0xbf01, 0xc9ae, 0xc97c, 
	0xc974, 0xc997, 0xc9e8, 0xca68, 0xcb1a, 0xcc02, 0xcd20, 0xce73, 0xcff4, 0xd19a, 0xd355, 0xd50c, 0xd6a3, 0xd7f8, 0xd8e9, 0x0000, 
	0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xfd27, 0xfd27, 
	0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 
	0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0332, 0x0332, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 
	0xffff, 0x0000, 0x0000, 0x375e, 0x36c7, 0x35c6, 0x344d, 0x3250, 0x2fc8, 0x2cc0, 0x2952, 0x25b7, 0x224b, 0x1f98, 0x1e47, 0x1ee5, 
	0x2875, 0x3193, 0x3d1d, 0x3d1d, 0x31c9, 0x3215, 0x3281, 0x3308, 0x339f, 0x343f, 0x34de, 0x3574, 0x35f8, 0x3666, 0x36bb, 0x36f6, 
	0x371c, 0x3733, 0x3746, 0x3760, 0x3790, 0x3e7b, 0x3f3a, 0x3f3a, 0x3e82, 0x42d7, 0x45a7, 0x4fbc, 0x558a, 0x5d75, 0x4000, 0x4e11, 
	0xea45, 0x0853, 0x4000, 0x4000, 0x20fd, 0x20fd, 0x4000, 0x3ea9, 0x3884, 0x34bf, 0x3122, 0x2d4c, 0x28fc, 0x23ec, 0x1dbe, 0x15de, 
	0x0b5f, 0xfcd7, 0xe870, 0x4000, 0x5b13, 0x4a26, 0x3e45, 0xc383, 0xc3bc, 0xc3bc, 0xc46c, 0xc43d, 0xc34e, 0xc1fd, 0xc3ea, 0xc3ad, 
	0xc35b, 0xc2f7, 0xbd5f, 0xbd4e, 0xba78, 0xb5ad, 0xb38c, 0xb38c, 0xb24d, 0xb324, 0xb40f, 0xb50b, 0xb613, 0xb724, 0xb838, 0xb94b, 
	0xba56, 0xbb52, 0xbc33, 0xbce9, 0xbd5d, 0xc282, 0xc2af, 0xc307, 0xc36b, 0xf8fa, 0xf907, 0xf907, 0xfa7a, 0xf878, 0xf6f1, 0xee9d, 
	0xf230, 0xefa9, 0xeba0, 0xe475, 0x5767, 0x4356, 0x22aa, 0x19fb, 0x1a4c, 0x1a4c, 0x1a87, 0x1b03, 0x1ba9, 0x1c82, 0x1d9c, 0x1f0a, 
	0x20ea, 0x2368, 0x26cb, 0x2b88, 0x3254, 0x3c28, 0x4990, 0xd8f6, 0xe6d7, 0xf158, 0xf8ea, 0xf1f4, 0xf2df, 0xf427, 0xf59a, 0xf716, 
	0xf88c, 0xf9fa, 0xfb67, 0xfce2, 0xfe7a, 0x0040, 0x023c, 0x0475, 0x0a00, 0x124d, 0x1af2, 0x1af2, 0x1b42, 0x19f7, 0x1832, 0x15fb, 
	0x1360, 0x106e, 0x0d37, 0x09ce, 0x0648, 0x02bc, 0xff42, 0xfbf6, 0xf8f1, 0xf64d, 0xf423, 0xf28b, 0xf19b, 0x11b8, 0x11a1, 0x1177, 
	0x113d, 0x10ff, 0x10cb, 0x10af, 0x10b6, 0x10df, 0x1122, 0x1172, 0x11be, 0x11f7, 0x1270, 0x14f5, 0x23e4, 0x23e4, 0x1cfa, 0x1d57, 
	0x1d79, 0x1d60, 0x1d0a, 0x1c77, 0x1ba9, 0x1aa4, 0x1972, 0x1820, 0x16c0, 0x1566, 0x1429, 0x131c, 0x1252, 0x11d9, 0x11c0, 0xeefa, 
	0xef1a, 0xef63, 0xefe0, 0xf095, 0xf17c, 0xf28a, 0xf3b1, 0xf4e3, 0xf613, 0xf737, 0xf842, 0xf928, 0xf9b5, 0xf9fc, 0xfce1, 0xfce1, 
	0xfc61, 0xfbcd, 0xfaf4, 0xf9e1, 0xf8a1, 0xf744, 0xf5de, 0xf482, 0xf341, 0xf229, 0xf142, 0xf08e, 0xf007, 0xefa6, 0xef5f, 0xef26, 
	0xeef2, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 0x0000, 0xffff, 
	0xf075, 0xf075, 0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 
	0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0379, 0x0379, 0xffff, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 
	0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 0x4635, 0x4606, 0x45b8, 0x454a, 0x44bf, 0x441a, 0x4362, 0x429d, 0x41d4, 0x4110, 0x4057, 
	0x3fb2, 0x3f28, 0x3edb, 0x3ee1, 0x485d, 0x485d, 0x3f09, 0x3f5d, 0x3fbd, 0x4028, 0x409b, 0x4114, 0x4192, 0x4210, 0x428e, 0x430a, 
	0x4383, 0x43f9, 0x446e, 0x44e2, 0x4556, 0x45cc, 0x4645, 0xba88, 0xbe37, 0xc3dc, 0xcb09, 0xd346, 0xdc21, 0xbfff, 0xcc01, 0xd6be, 
	0xdd53, 0xe1af, 0xe421, 0xe494, 0xd7be, 0xd6d8, 0x34c8, 0x34c8, 0x2d76, 0x2be8, 0x2ae4, 0x2a5a, 0x2a3c, 0x2a81, 0x2b23, 0x2c19, 
	0x2d5c, 0x2ede, 0x3090, 0x325f, 0x3432, 0x35eb, 0x376c, 0x3891, 0xb937, 0xd26a, 0xd24b, 0xd235, 0xd244, 0xd28e, 0xd31b, 0xd3e5, 
	0xd4d9, 0xd5d9, 0xd6c5, 0xd77a, 0xd7db, 0xd7cf, 0xd592, 0xd3ad, 0xac48, 0xac48, 0xa6cd, 0xa69f, 0xa6a5, 0xa6da, 0xa736, 0xa7b2, 
	0xa846, 0xa8ea, 0xa996, 0xaa42, 0xaae7, 0xab81, 0xac0b, 0xac84, 0xaceb, 0xad40, 0xd278, 0x4a86, 0x488f, 0x4590, 0x41ce, 0x3da3, 
	0x396f, 0x3588, 0x322e, 0x2f82, 0x2d90, 0x2c5d, 0x2bed, 0x2c53, 0x32c7, 0x4382, 0xd540, 0xd540, 0xdbf7, 0xdc77, 0xdc9e, 0xdc77, 
	0xdc08, 0xdb59, 0xda6c, 0xd947, 0xd7ef, 0xd66a, 0xd4c1, 0xd301, 0xd138, 0xcf77, 0xcdd3, 0xcc62, 0x4b3a, };

JointIndex gGanonSkelGganonleftswordswinganimAnimJointIndices[47] = {
	{ 0x0007, 0x0029, 0x004b, },
	{ 0x0000, 0x0000, 0x0000, },
	{ 0x006d, 0x008f, 0x00b1, },
	{ 0x00d3, 0x00f5, 0x0117, },
	{ 0x0139, 0x015b, 0x017d, },
	{ 0x019f, 0x01c1, 0x01e3, },
	{ 0x0205, 0x0227, 0x0249, },
	{ 0x026b, 0x028d, 0x02af, },
	{ 0x0001, 0x0000, 0x0001, },
	{ 0x02d1, 0x02f3, 0x0315, },
	{ 0x0337, 0x0359, 0x037b, },
	{ 0x039d, 0x03bf, 0x03e1, },
	{ 0x0403, 0x0425, 0x0447, },
	{ 0x0002, 0x0003, 0x0004, },
	{ 0x0000, 0x0001, 0x0001, },
	{ 0x0469, 0x048b, 0x04ad, },
	{ 0x04cf, 0x04f1, 0x0513, },
	{ 0x0001, 0x0000, 0x0000, },
	{ 0x0535, 0x0557, 0x0579, },
	{ 0x0001, 0x0000, 0x0001, },
	{ 0x059b, 0x05bd, 0x05df, },
	{ 0x0001, 0x0001, 0x0001, },
	{ 0x0005, 0x0601, 0x0005, },
	{ 0x0623, 0x0645, 0x0667, },
	{ 0x0689, 0x06ab, 0x06cd, },
	{ 0x06ef, 0x0711, 0x0733, },
	{ 0x0755, 0x0777, 0x0799, },
	{ 0x07bb, 0x07dd, 0x07ff, },
	{ 0x0821, 0x0843, 0x0865, },
	{ 0x0887, 0x08a9, 0x08cb, },
	{ 0x08ed, 0x090f, 0x0931, },
	{ 0x0001, 0x0000, 0x0000, },
	{ 0x0001, 0x0000, 0x0000, },
	{ 0x0001, 0x0000, 0x0000, },
	{ 0x0001, 0x0000, 0x0000, },
	{ 0x0953, 0x0975, 0x0997, },
	{ 0x09b9, 0x09db, 0x09fd, },
	{ 0x0a1f, 0x0a41, 0x0a63, },
	{ 0x0a85, 0x0aa7, 0x0ac9, },
	{ 0x0aeb, 0x0b0d, 0x0b2f, },
	{ 0x0b51, 0x0b73, 0x0b95, },
	{ 0x0bb7, 0x0bd9, 0x0bfb, },
	{ 0x0005, 0x0006, 0x0000, },
	{ 0x0000, 0x0001, 0x0001, },
	{ 0x0000, 0x0001, 0x0001, },
	{ 0x0000, 0x0001, 0x0001, },
	{ 0x0000, 0x0001, 0x0001, },
};

AnimationHeader gGanonSkelGganonleftswordswinganimAnim = { { 34 }, gGanonSkelGganonleftswordswinganimAnimFrameData, gGanonSkelGganonleftswordswinganimAnimJointIndices, 7 };

void func_80900580(BossGanon2* this, PlayState* play, s16 isTracking) {
    if (this->unk_311 == 0) {
        Animation_MorphToPlayOnce(&this->skelAnime, &gGanonSkelGganonleftswordswinganimAnim, -5.0f);
        this->unk_194 = Animation_GetLastFrame(&gGanonSkelGganonleftswordswinganimAnim);
        this->unk_198 = (this->unk_194 - 15.0f) - 5.0f;
    } else {
        Animation_MorphToPlayOnce(&this->skelAnime, &gGanonRightSwordSwingAnim, -5.0f);
        this->unk_194 = Animation_GetLastFrame(&gGanonRightSwordSwingAnim);
        this->unk_198 = (this->unk_194 - 15.0f) - 5.0f;
    }

    this->isTrackingLink = isTracking;
    this->actionFunc = func_80900650;
}

void func_80900650(BossGanon2* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

    if (Animation_OnFrame(&this->skelAnime, this->unk_198)) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_SWORD);
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_ROAR);
    }

    if (this->unk_311 == 0) {
        if (((this->unk_198 - 4.0f) < this->skelAnime.curFrame) &&
            (this->skelAnime.curFrame < (this->unk_198 + 6.0f))) {
            this->unk_312 = 1;
        }
    } else if ((((this->unk_198 - 4.0f) + 4.0f) < this->skelAnime.curFrame) &&
               (this->skelAnime.curFrame < (this->unk_198 + 6.0f))) {
        this->unk_312 = 2;
    }

    if (this->isTrackingLink && this->unk_311 == 0 && (this->skelAnime.curFrame <= (this->unk_198 - 4.0f))) {
        if (this->actor.xzDistToPlayer > 120.0f)
            this->actor.speedXZ = 6.0f;
        else if (this->actor.xzDistToPlayer < 80.0f)
            this->actor.speedXZ = -4.0f;
        else
            Math_ApproachZeroF(&this->actor.speedXZ, 0.5f, 1.0f);

        if (!Actor_IsFacingPlayer(&this->actor, 0x500)) {
            Math_ApproachS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 3, 0x400);
        }
    } else if (this->isTrackingLink && this->unk_311 == 1 && (this->skelAnime.curFrame <= (this->unk_198 - 2.0f))) {
        if (this->actor.xzDistToPlayer > 140.0f && Actor_IsFacingPlayer(&this->actor, 0x2500))
            this->actor.speedXZ = 6.0f;
        else
            Math_ApproachZeroF(&this->actor.speedXZ, 0.5f, 1.0f);

        if (!Actor_IsFacingPlayer(&this->actor, 0x500)) {
            Math_ApproachS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 3, 0x400);
        }
    } else {
        Math_ApproachZeroF(&this->actor.speedXZ, 0.5f, 1.0f);
    }

    if (Animation_OnFrame(&this->skelAnime, this->unk_194)) {
        this->unk_311 = 1 - this->unk_311;

        if ((this->unk_311 == 1) && (this->actor.xzDistToPlayer < 250.0f) && this->unk_313) {
            func_80900580(this, play, true);
        } else {
            func_808FFDB0(this, play);
        }
    }

    func_808FFAC8(this, play, 0);

    if ((this->unk_334 == 0) && (this->unk_311 == 0)) {
        this->unk_320 = 0.0f;
    } else {
        func_808FFBBC(this, play, 0);
    }
}

void func_80900818(BossGanon2* this, PlayState* play) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gGanonDownedStartAnim, -5.0f);
    this->unk_194 = Animation_GetLastFrame(&gGanonDownedStartAnim);
    this->actionFunc = func_80900890;
    this->unk_1AC = 0;
    this->unk_39C = 0;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_DEAD1);
    this->unk_336 = 0;
}

void func_80900890(BossGanon2* this, PlayState* play) {
    Vec3f sp5C;
    Vec3f sp50;
    Camera* sp4C;
    Player* player;
    Camera* temp_v0;
    Camera* temp_v0_2;
    s32 pad;
    f32 temp_f12;
    f32 temp_f2;

    sp4C = Play_GetCamera(play, MAIN_CAM);
    player = GET_PLAYER(play);
    SkelAnime_Update(&this->skelAnime);
    this->unk_398++;
    this->unk_339 = 20;

    switch (this->unk_39C) {
        case 0:
            func_80064520(play, &play->csCtx);
            this->unk_39E = Play_CreateSubCamera(play);
            Play_ChangeCameraStatus(play, MAIN_CAM, CAM_STAT_WAIT);
            Play_ChangeCameraStatus(play, this->unk_39E, CAM_STAT_ACTIVE);
            func_8002DF54(play, &this->actor, 8);
            this->unk_39C = 1;
            this->unk_3A4 = sp4C->eye;
            this->unk_3B0 = sp4C->at;
            this->unk_1A2[0] = 300;
            this->unk_1A2[1] = 100;
            play->envCtx.unk_D8 = 0.0f;
        case 1:
            if (this->unk_1A2[1] == 50) {
                func_80078884(NA_SE_EN_MGANON_WALK);
            }
            Matrix_RotateY(((this->actor.shape.rot.y / (f32)0x8000) * M_PI) + 0.3f, MTXMODE_NEW);
            sp5C.x = 0.0f;
            sp5C.y = 0.0f;
            sp5C.z = 250.0f;
            Matrix_MultVec3f(&sp5C, &sp50);
            Math_ApproachF(&this->unk_3A4.x, this->actor.world.pos.x + sp50.x, 0.2f, 100.0f);
            Math_ApproachF(&this->unk_3A4.y, 1136.0f, 0.2f, 100.0f);
            Math_ApproachF(&this->unk_3A4.z, this->actor.world.pos.z + sp50.z, 0.2f, 100.0f);
            Math_ApproachF(&this->unk_3B0.x, this->unk_1B8.x, 0.2f, 100.0f);
            Math_ApproachF(&this->unk_3B0.y, this->unk_1B8.y, 0.2f, 100.0f);
            Math_ApproachF(&this->unk_3B0.z, this->unk_1B8.z, 0.2f, 100.0f);
            if (this->unk_1A2[1] == 0) {
                this->unk_39C = 2;
                this->unk_1A2[1] = 90;
            }
            break;
        case 2:
            this->unk_1A2[0] = 300;
            this->unk_3A4.x = sBossGanon2Zelda->actor.world.pos.x - 100.0f;
            this->unk_3A4.y = sBossGanon2Zelda->actor.world.pos.y + 30.0f;
            this->unk_3A4.z = (sBossGanon2Zelda->actor.world.pos.z + 30.0f) - 60.0f;
            this->unk_3B0.x = sBossGanon2Zelda->actor.world.pos.x;
            this->unk_3B0.y = sBossGanon2Zelda->actor.world.pos.y + 30.0f;
            this->unk_3B0.z = sBossGanon2Zelda->actor.world.pos.z - 10.0f;
            Math_ApproachZeroF(&this->unk_324, 1.0f, 5.0f);
            Math_ApproachF(&play->envCtx.unk_D8, 1.0f, 1.0f, 1.0f / 51);
            if (this->unk_1A2[1] == 80) {
                Message_StartTextbox(play, 0x70D7, NULL);
            }
            if ((this->unk_1A2[1] < 30) && (Message_GetState(&play->msgCtx) == TEXT_STATE_NONE)) {
                temp_v0 = Play_GetCamera(play, MAIN_CAM);
                temp_v0->eye = this->unk_3A4;
                temp_v0->eyeNext = this->unk_3A4;
                temp_v0->at = this->unk_3B0;
                func_800C08AC(play, this->unk_39E, 0);
                this->unk_39E = 0;
                func_80064534(play, &play->csCtx);
                func_8002DF54(play, &this->actor, 7);
                this->unk_39C = 3;
            }
            break;
        case 10:
            func_80064520(play, &play->csCtx);
            this->unk_39E = Play_CreateSubCamera(play);
            Play_ChangeCameraStatus(play, MAIN_CAM, CAM_STAT_WAIT);
            Play_ChangeCameraStatus(play, this->unk_39E, CAM_STAT_ACTIVE);
            this->unk_39C = 11;
            this->unk_334 = 1;
            func_8002DF54(play, &this->actor, 0x60);
            this->unk_398 = 0;
        case 11:
            player->actor.world.pos.x = sBossGanon2Zelda->actor.world.pos.x + 50.0f + 10.0f;
            player->actor.world.pos.z = sBossGanon2Zelda->actor.world.pos.z - 25.0f;
            player->actor.shape.rot.y = -0x8000;
            this->unk_3A4.x = (player->actor.world.pos.x + 100.0f) - 80.0f;
            this->unk_3A4.y = (player->actor.world.pos.y + 60.0f) - 40.0f;
            this->unk_3A4.z = player->actor.world.pos.z - 110.0f;
            this->unk_3B0.x = player->actor.world.pos.x;
            this->unk_3B0.y = (player->actor.world.pos.y + 60.0f) - 25.0f;
            this->unk_3B0.z = player->actor.world.pos.z;
            if (this->unk_398 == 80) {
                temp_v0_2 = Play_GetCamera(play, MAIN_CAM);
                temp_v0_2->eye = this->unk_3A4;
                temp_v0_2->eyeNext = this->unk_3A4;
                temp_v0_2->at = this->unk_3B0;
                this->unk_39C = 3;
                func_800C08AC(play, this->unk_39E, 0);
                this->unk_39E = 0;
                func_80064534(play, &play->csCtx);
                func_8002DF54(play, &this->actor, 7);
            }
            break;
    }

    if (this->unk_39E != 0) {
        Play_CameraSetAtEye(play, this->unk_39E, &this->unk_3B0, &this->unk_3A4);
    }

    switch (this->unk_1AC) {
        case 0:
            if (Animation_OnFrame(&this->skelAnime, this->unk_194)) {
                Animation_MorphToLoop(&this->skelAnime, &gGanonDownedLoopAnim, 0.0f);
                this->unk_1AC = 1;
            }
            break;
        case 1:
            if ((play->gameplayFrames % 32) == 0) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_BREATH);
            }

            if ((this->unk_1A2[0] == 0) || (this->unk_334 != 0)) {
                temp_f2 = -200.0f - player->actor.world.pos.x;
                temp_f12 = -200.0f - player->actor.world.pos.z;

                if (sqrtf(SQ(temp_f2) + SQ(temp_f12)) <= 784.0f) {
                    Animation_MorphToPlayOnce(&this->skelAnime, &gGanonGetUpAnim, 0.0f);
                    this->unk_194 = Animation_GetLastFrame(&gGanonGetUpAnim);
                    this->unk_1AC = 2;
                    this->unk_1A2[0] = 40;
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_ROAR);
                }
            }
            break;
        case 2:
            Math_ApproachF(&this->unk_324, 255.0f, 1.0f, 10.0f);
            Math_ApproachZeroF(&play->envCtx.unk_D8, 1.0f, 2.0f / 51.0f);
            if (Animation_OnFrame(&this->skelAnime, this->unk_194)) {
                func_808FFDB0(this, play);
                if (this->unk_334 == 0) {
                    this->actor.colChkInfo.health = 25;
                }
                this->unk_336 = 1;
            }
            break;
    }

    Math_ApproachZeroF(&this->actor.speedXZ, 0.5f, 1.0f);
}

void func_80901020(BossGanon2* this, PlayState* play) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gGanonDownedStartAnim, -5.0f);
    this->unk_194 = Animation_GetLastFrame(&gGanonDownedStartAnim);
    this->actionFunc = func_8090120C;
    this->unk_1AC = 0;
    this->unk_39C = 0;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_DEAD1);
    this->unk_314 = 4;
}

void func_8090109C(BossGanon2* this, PlayState* play) {
    u8 i;

    static Color_RGBA8 sPrimColor = { 0, 120, 0, 255 };
    static Color_RGBA8 sEnvColor = { 0, 120, 0, 255 };

    if(CVar_GetS32("gRedGanonBlood", 0)) {
        sPrimColor.r = 120;
        sPrimColor.g = 0;

        sEnvColor.r = 120;
        sEnvColor.g = 0;
    }

    for (i = 0; i < 70; i++) {
        Vec3f velocity;
        Vec3f accel;
        Vec3f pos;

        velocity.x = Rand_CenteredFloat(50.0f);
        velocity.y = Rand_CenteredFloat(10.0f) + 5.0f;
        velocity.z = Rand_CenteredFloat(50.0f);
        accel.x = 0.0f;
        accel.y = -1.0f;
        accel.z = 0.0f;
        pos.x = this->unk_1B8.x;
        pos.y = this->unk_1B8.y;
        pos.z = this->unk_1B8.z;
        func_8002836C(play, &pos, &velocity, &accel, &sPrimColor, &sEnvColor, (s16)Rand_ZeroFloat(50.0f) + 50, 0,
                      17);
    }
}

void func_8090120C(BossGanon2* this, PlayState* play) {
    Player* player;
    f32 temp_f14;
    f32 temp_f12;
    Camera* temp_v0_2;
    s16 temp_a0_2;
    f32 phi_f0;
    s32 phi_a1;

    player = GET_PLAYER(play);
    this->unk_398++;
    SkelAnime_Update(&this->skelAnime);

    this->unk_3BC.x = 0.0f;
    this->unk_3BC.y = 1.0f;
    this->unk_3BC.z = 0.0f;

    switch (this->unk_39C) {
        case 0:
            func_80064520(play, &play->csCtx);
            this->unk_39E = Play_CreateSubCamera(play);
            Play_ChangeCameraStatus(play, MAIN_CAM, CAM_STAT_WAIT);
            Play_ChangeCameraStatus(play, this->unk_39E, CAM_STAT_ACTIVE);
            func_8002DF54(play, &this->actor, 8);
            this->unk_39C = 1;
            this->unk_398 = 0;
            sBossGanon2Zelda->unk_3C8 = 9;
            this->unk_31C = 0;
            this->unk_1A2[2] = 0;
            this->unk_336 = 0;
            this->unk_324 = 0.0f;
            this->actor.speedXZ = 0.0f;
            this->unk_31A = this->unk_31C;
            play->envCtx.unk_D8 = 0.0f;
        case 1:
            if (this->unk_398 < 90) {
                this->unk_339 = 20;
                Math_ApproachF(&play->envCtx.unk_D8, 1.0f, 1.0f, 0.1f);
            } else if (this->unk_398 >= 90) {
                this->unk_339 = 21;
                Math_ApproachZeroF(&play->envCtx.unk_D8, 1.0f, 0.08f);
            }
            if (this->unk_398 == 50) {
                func_80078884(NA_SE_EN_MGANON_WALK);
            }
            if (this->unk_398 > 90) {
                Math_ApproachF(&this->unk_380, 0.25f, 1.0f, 0.0125f);
                this->unk_37C = 200.0f;
                func_80078884(NA_SE_EV_TIMETRIP_LIGHT - SFX_FLAG);
            }
            if (this->unk_398 >= 110) {
                if (this->unk_398 == 110) {
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_GANON_HIT_THUNDER);
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_DAMAGE);
                }
                Math_ApproachF(&this->unk_30C, 10.0f, 0.2f, 5.0f);
                this->skelAnime.playSpeed = 3.0f;
            }
            if (this->unk_398 == 120) {
                func_8002DF54(play, &this->actor, 0x63);
            }
            this->actor.world.rot.y = 0x4000;
            this->actor.world.pos.x = this->actor.world.pos.z = 0.0f;
            player->actor.shape.rot.y = -0x4000;
            player->actor.world.pos.x = 200.0f;
            player->actor.world.pos.z = 30.0f;
            sBossGanon2Zelda->actor.world.pos.x = 340.0f;
            sBossGanon2Zelda->actor.world.pos.z = -250.0f;
            sBossGanon2Zelda->actor.world.rot.y = sBossGanon2Zelda->actor.shape.rot.y = -0x2000;
            this->unk_3A4.x = 250;
            this->unk_3A4.y = 1150.0f;
            this->unk_3A4.z = 0.0f;
            this->unk_3B0.x = this->unk_1B8.x;
            this->unk_3B0.y = this->unk_1B8.y;
            this->unk_3B0.z = this->unk_1B8.z;
            if (this->unk_398 > 135) {
                this->unk_39C = 2;
                this->unk_398 = 0;
            }
            break;
        case 2:
            this->unk_339 = 22;
            Math_ApproachF(&play->envCtx.unk_D8, 1.0f, 1.0f, 0.1f);
            func_80078884(NA_SE_EV_TIMETRIP_LIGHT - SFX_FLAG);
            this->unk_3A4.x = 250;
            this->unk_3A4.y = 1150.0f;
            this->unk_3A4.z = 0.0f;
            Math_ApproachF(&this->unk_3B0.x, sBossGanon2Zelda->actor.world.pos.x, 0.2f, 20.0f);
            Math_ApproachF(&this->unk_3B0.y, sBossGanon2Zelda->actor.world.pos.y + 50.0f, 0.2f, 10.0f);
            Math_ApproachF(&this->unk_3B0.z, sBossGanon2Zelda->actor.world.pos.z, 0.2f, 20.0f);
            if (this->unk_398 == 50) {
                this->unk_39C = 3;
                this->unk_398 = 0;
            }
            break;
        case 3:
            this->unk_339 = 22;
            func_80078884(NA_SE_EV_TIMETRIP_LIGHT - SFX_FLAG);
            this->unk_3A4.x = 330.0f;
            this->unk_3A4.y = 1120.0f;
            this->unk_3A4.z = -150.0f;
            this->unk_3B0.x = sBossGanon2Zelda->actor.world.pos.x;
            this->unk_3B0.y = sBossGanon2Zelda->actor.world.pos.y + 40.0f;
            this->unk_3B0.z = sBossGanon2Zelda->actor.world.pos.z;
            if (this->unk_398 == 10) {
                Message_StartTextbox(play, 0x70D8, NULL);
            }
            if ((this->unk_398 > 80) && (Message_GetState(&play->msgCtx) == TEXT_STATE_NONE)) {
                this->unk_39C = 4;
                this->unk_398 = 0;
            }
            break;
        case 4:
            if (this->unk_398 > 10) {
                Math_ApproachZeroF(&this->unk_37C, 1.0f, 10.0f);
                if (this->unk_398 == 30) {
                    sBossGanon2Zelda->unk_3C8 = 10;
                }
                this->unk_339 = 23;
                Math_ApproachZeroF(&play->envCtx.unk_D8, 1.0f, 0.05f);
            } else {
                this->unk_339 = 22;
            }
            if (this->unk_398 == 100) {
                this->unk_39C = 5;
                this->unk_398 = 40;
                this->skelAnime.playSpeed = 1.0f;
                func_8002DF54(play, &this->actor, 0x64);
            }
            break;
        case 5:
            this->unk_339 = 23;
            if ((this->unk_398 >= 60) && (this->unk_398 <= 90)) {
                if (this->unk_398 == 62) {
                    func_80078884(NA_SE_EV_TRIFORCE_FLASH);
                }
                Math_ApproachF(&this->unk_38C, 200.0f, 1.0f, 8.0f);
            } else {
                Math_ApproachZeroF(&this->unk_38C, 1.0f, 8.0f);
            }
            if (this->unk_398 == 70) {
                func_8002DF54(play, &this->actor, 0x65);
            }
            if (this->unk_398 == 150) {
                func_8002DF54(play, &this->actor, 0x66);
            }
            this->unk_30C = 10.0f;
            player->actor.world.pos.x = 250.0f;
            player->actor.world.pos.z = 30.0f;
            this->unk_3A4.x = player->actor.world.pos.x - 50.0f;
            this->unk_3A4.y = player->actor.world.pos.y + 50.0f;
            this->unk_3A4.z = player->actor.world.pos.z + 40.0f;
            this->unk_3B0.x = player->actor.world.pos.x;
            this->unk_3B0.y = player->actor.world.pos.y + 40.0f;
            this->unk_3B0.z = player->actor.world.pos.z;
            if (this->unk_398 == 166) {
                temp_v0_2 = Play_GetCamera(play, MAIN_CAM);
                temp_v0_2->eye = this->unk_3A4;
                temp_v0_2->eyeNext = this->unk_3A4;
                temp_v0_2->at = this->unk_3B0;
                func_800C08AC(play, this->unk_39E, 0);
                this->unk_39E = 0;
                func_80064534(play, &play->csCtx);
                func_8002DF54(play, &this->actor, 7);
                this->unk_39C = 6;
            }
            break;
        case 6:
            this->unk_339 = 23;
            temp_f14 = this->unk_1B8.x - player->actor.world.pos.x;
            temp_f12 = this->unk_1B8.z - player->actor.world.pos.z;
            temp_a0_2 = Math_Atan2S(temp_f12, temp_f14) - player->actor.shape.rot.y;
            if ((ABS(temp_a0_2) < 0x2000) && (sqrtf(SQ(temp_f14) + SQ(temp_f12)) < 70.0f) &&
                (player->swordState != 0) && (player->heldItemAction == PLAYER_IA_SWORD_MASTER)) {
                func_80064520(play, &play->csCtx);
                gSaveContext.sohStats.gameComplete = true;
                gSaveContext.sohStats.timestamp[TIMESTAMP_DEFEAT_GANON] = GAMEPLAYSTAT_TOTAL_TIME;
                this->unk_39E = Play_CreateSubCamera(play);
                Play_ChangeCameraStatus(play, MAIN_CAM, CAM_STAT_WAIT);
                Play_ChangeCameraStatus(play, this->unk_39E, CAM_STAT_ACTIVE);
                this->unk_39C = 7;
                this->unk_398 = 0;
                Animation_MorphToPlayOnce(&this->skelAnime, &gGanonFinalBlowAnim, 0.0f);
                this->unk_194 = Animation_GetLastFrame(&gGanonFinalBlowAnim);
                play->startPlayerCutscene(play, &this->actor, 0x61);
            } else {
                break;
            }
        case 7:
            this->unk_339 = 23;
            Math_ApproachZeroF(&play->envCtx.unk_D8, 1.0f, 0.2f);
            player->actor.world.pos.x = 250.0f;
            player->actor.shape.rot.y = -0x4000;
            player->actor.world.pos.z = 30.0f;
            if ((this->unk_398 == 20) || (this->unk_398 == 30) || (this->unk_398 == 65) || (this->unk_398 == 40)) {
                func_80078884(NA_SE_VO_LI_SWORD_N);
                func_80078884(NA_SE_IT_SWORD_SWING_HARD);
            }
            if ((this->unk_398 == 22) || (this->unk_398 == 35) || (this->unk_398 == 72) || (this->unk_398 == 45)) {
                func_80078884(NA_SE_EN_MGANON_DAMAGE);
                func_80078884(NA_SE_IT_SHIELD_BOUND);
                play->envCtx.unk_D8 = 1.0f;
            }
            if ((this->unk_398 == 22) || (this->unk_398 == 35) || (this->unk_398 == 72) || (this->unk_398 == 45)) {
                func_8090109C(this, play);
            }
            if ((this->unk_398 >= 34) && (this->unk_398 < 40)) {
                this->unk_3A4.x = 269.0f;
                this->unk_3A4.y = 1112.0f;
                this->unk_3A4.z = -28.0f;
                this->unk_3B0.x = 234.0f;
                this->unk_3B0.y = 1117.0f;
                this->unk_3B0.z = -11.0f;
            } else {
                if (this->unk_398 < 30) {
                    phi_a1 = 0;
                } else if (this->unk_398 < 43) {
                    phi_a1 = 1;
                } else {
                    this->unk_3BC.z = -0.8f;
                    player->actor.world.pos.x = 200.0f;
                    player->actor.world.pos.z = 10.0f;
                    phi_a1 = 2;
                }
                this->unk_3A4.x = D_8090702C[phi_a1].x + (player->actor.world.pos.x - 50.0f);
                this->unk_3A4.y = D_8090702C[phi_a1].y + (player->actor.world.pos.y + 50.0f);
                this->unk_3A4.z = D_8090702C[phi_a1].z + (player->actor.world.pos.z + 40.0f);
                this->unk_3B0.x = D_80907050[phi_a1].x + player->actor.world.pos.x;
                this->unk_3B0.y = D_80907050[phi_a1].y + (player->actor.world.pos.y + 40.0f);
                this->unk_3B0.z = D_80907050[phi_a1].z + player->actor.world.pos.z;
            }
            if (this->unk_398 > 80) {
                Audio_QueueSeqCmd(0x1 << 28 | SEQ_PLAYER_BGM_MAIN << 24 | 0x100FF);
                this->unk_39C = 75;
                this->unk_398 = 0;
                this->unk_3A4.x = 112.0f;
                this->unk_3A4.y = 1146.0f;
                this->unk_3A4.z = 202.0f;
                this->unk_3B0.x = 110.0f;
                this->unk_3B0.y = 1144.0f;
                this->unk_3B0.z = 177.0f;
                player->actor.world.pos.x = 200.0f;
                this->unk_3BC.z = 0.0f;
            }
            break;
        case 75:
            this->unk_339 = 23;
            if (this->unk_398 == 55) {
                Animation_MorphToPlayOnce(&this->skelAnime, &gGanonDeadStartAnim, 0.0f);
                this->unk_194 = Animation_GetLastFrame(&gGanonDeadStartAnim);
                func_8002DF54(play, &this->actor, 0x62);
                this->unk_39C = 8;
                this->unk_398 = 1000;
            }
            break;
        case 8:
            if (this->unk_398 == 1025) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_STAND);
            }
            if (this->unk_398 >= 1000) {
                if (this->unk_398 < 1040) {
                    this->unk_339 = 23;
                    Math_ApproachZeroF(&play->envCtx.unk_D8, 1.0f, 0.2f);
                }
            }
            if (this->unk_398 == 1040) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_DEAD2);
                this->unk_336 = 2;
                this->unk_339 = 0;
                play->envCtx.unk_BE = 0;
                play->envCtx.unk_D8 = 0.0f;
            }
            if (Animation_OnFrame(&this->skelAnime, this->unk_194)) {
                Animation_MorphToPlayOnce(&this->skelAnime, &gGanonDeadLoopAnim, 0.0f);
                this->unk_398 = 0;
                this->unk_194 = 1000.0f;
            }
            this->unk_3A4.x = 250;
            this->unk_3A4.y = 1150.0f;
            this->unk_3A4.z = 0.0f;
            this->unk_3B0.x = this->unk_1B8.x;
            this->unk_3B0.y = this->unk_1B8.y;
            this->unk_3B0.z = this->unk_1B8.z;
            if ((this->unk_398 < 1000) && ((this->unk_398 % 16) == 0)) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_SWORD);
            }
            if (this->unk_398 == 40) {
                this->unk_39C = 9;
                this->unk_398 = 0;
                sBossGanon2Zelda->unk_3C8 = 11;
                Message_StartTextbox(play, 0x70D9, NULL);
                this->unk_336 = 0;
                play->envCtx.unk_D8 = 0.0f;
            }
            break;
        case 9:
            this->unk_339 = 24;
            this->unk_3A4.x = 330.0f;
            this->unk_3A4.y = 1120.0f;
            this->unk_3A4.z = -150.0f;
            this->unk_3B0.x = sBossGanon2Zelda->actor.world.pos.x;
            this->unk_3B0.y = sBossGanon2Zelda->actor.world.pos.y + 40.0f;
            this->unk_3B0.z = sBossGanon2Zelda->actor.world.pos.z;
            if (this->unk_398 > 60) {
                this->unk_39C = 10;
                this->unk_398 = 0;
                this->unk_410.x = 0.0f;
            }
            break;
        case 10:
            this->unk_339 = 24;
            Math_ApproachF(&this->unk_3A4.x, 290.0f, 0.05f, this->unk_410.x);
            Math_ApproachF(&this->unk_3A4.y, 1130.0f, 0.05f, this->unk_410.x * 0.25f);
            Math_ApproachF(&this->unk_3A4.z, -260.0f, 0.05f, this->unk_410.x * 1.25f);
            if ((this->unk_398 >= 40) && (this->unk_398 <= 110)) {
                Math_ApproachF(&play->envCtx.unk_D8, 1.0f, 1.0f, 0.02f);
                Math_ApproachF(&this->unk_384, 10.0f, 0.1f, 0.2f);
                Audio_PlayActorSound2(&sBossGanon2Zelda->actor, NA_SE_EV_GOD_LIGHTBALL_2 - SFX_FLAG);
            } else {
                Math_ApproachZeroF(&this->unk_384, 1.0f, 0.2f);
            }
            if (this->unk_398 > 130) {
                Math_ApproachF(&this->unk_3B0.y, (sBossGanon2Zelda->actor.world.pos.y + 40.0f + 10.0f) - 20.0f, 0.1f,
                               this->unk_410.x);
            } else {
                Math_ApproachF(&this->unk_3B0.y, sBossGanon2Zelda->actor.world.pos.y + 40.0f + 10.0f, 0.05f,
                               this->unk_410.x * 0.25f);
            }
            Math_ApproachF(&this->unk_410.x, 1.0f, 1.0f, 0.01f);
            if (this->unk_398 == 10) {
                sBossGanon2Zelda->unk_3C8 = 12;
            }
            if (this->unk_398 == 110) {
                sBossGanon2Zelda->unk_3C8 = 13;
            }
            if (this->unk_398 == 140) {
                Audio_PlayActorSound2(&sBossGanon2Zelda->actor, NA_SE_EV_HUMAN_BOUND);
            }
            if (this->unk_398 < 160) {
                break;
            }
        case 20:
            play->nextEntranceIndex = 0x6B;
            gSaveContext.nextCutsceneIndex = 0xFFF2;
            play->sceneLoadFlag = 0x14;
            play->fadeTransition = 3;
            play->linkAgeOnLoad = 1;
            break;
    }

    if (this->unk_39E != 0) {
        Play_CameraSetAtEyeUp(play, this->unk_39E, &this->unk_3B0, &this->unk_3A4, &this->unk_3BC);
    }

    switch (this->unk_1AC) {
        case 0:
            if (Animation_OnFrame(&this->skelAnime, this->unk_194)) {
                Animation_MorphToLoop(&this->skelAnime, &gGanonDownedLoopAnim, 0.0f);
                this->unk_1AC = 1;
            }
            break;
        case 1:
            if ((this->unk_39C < 7) && ((play->gameplayFrames % 32) == 0)) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_BREATH);
            }
            break;
    }
}

void func_80902348(BossGanon2* this, PlayState* play) {
    Player* player;
    f32 temp_f2;
    f32 temp_f12;
    s16 i;
    s16 j;
    s16 phi_v0_2;
    s16 isBlocked1 = 0;
    s16 isBlocked2 = 0;
    s16 isBlockedSphere1 = 0;
    s16 isBlockedSphere2 = 0;

    if (this->unk_316 == 0) {
        for (i = 0; i < ARRAY_COUNT(this->unk_864); i++) {
            if (this->unk_444.elements[i].info.bumperFlags & 2) {
                this->unk_444.elements[i].info.bumperFlags &= ~2;
                isBlocked1 = 1;
            }
            if (this->unk_445.elements[i].info.bumperFlags & 2) {
                this->unk_445.elements[i].info.bumperFlags &= ~2;
                isBlocked2 = 1;
            }
        }
        for (i = 0; i < ARRAY_COUNT(this->swordSphereElement1); i++) {
            if (this->swordSpheres1.elements[i].info.bumperFlags & 2) {
                this->swordSpheres1.elements[i].info.bumperFlags &= ~2;
                isBlocked1 = 1;
            }
        }
        for (i = 0; i < ARRAY_COUNT(this->swordSphereElement2); i++) {
            if (this->swordSpheres2.elements[i].info.bumperFlags & 2) {
                this->swordSpheres2.elements[i].info.bumperFlags &= ~2;
                isBlocked2 = 1;
            }
        }

        for (i = 0; i < ARRAY_COUNT(this->unk_864); i++) {
            if (this->unk_444.elements[i].info.toucherFlags & 2) {
                this->unk_444.elements[i].info.toucherFlags &= ~2;
                if (!isBlocked1) {
                    if (this->unk_312 == 1) {
                        phi_v0_2 = 0x1800;
                    } else {
                        phi_v0_2 = 0;
                    }

                    func_8002F6D4(play, &this->actor, 15.0f, this->actor.yawTowardsPlayer + phi_v0_2, 2.0f, 0);
                    sBossGanon2Zelda->unk_3C8 = 8;
                    this->unk_316 = 10;
                    break;
                }
            }
            if (this->unk_445.elements[i].info.toucherFlags & 2) {
                this->unk_445.elements[i].info.toucherFlags &= ~2;
                if (!isBlocked2) {
                    if (this->unk_312 == 1) {
                        phi_v0_2 = 0x1800;
                    } else {
                        phi_v0_2 = 0;
                    }

                    func_8002F6D4(play, &this->actor, 15.0f, this->actor.yawTowardsPlayer + phi_v0_2, 2.0f, 0);
                    sBossGanon2Zelda->unk_3C8 = 8;
                    this->unk_316 = 10;
                    break;
                }
            }
        }
        for (i = 0; i < ARRAY_COUNT(this->swordSphereElement1); i++) {
            if (this->swordSpheres1.elements[i].info.toucherFlags & 2) {
                this->swordSpheres1.elements[i].info.toucherFlags &= ~2;
                if (!isBlocked1) {
                    if (this->unk_312 == 1) {
                        phi_v0_2 = 0x1800;
                    } else {
                        phi_v0_2 = 0;
                    }

                    func_8002F6D4(play, &this->actor, 15.0f, this->actor.yawTowardsPlayer + phi_v0_2, 2.0f, 0);
                    sBossGanon2Zelda->unk_3C8 = 8;
                    this->unk_316 = 10;
                    break;
                }
            }
        }
        for (i = 0; i < ARRAY_COUNT(this->swordSphereElement2); i++) {
            if (this->swordSpheres2.elements[i].info.toucherFlags & 2) {
                this->swordSpheres2.elements[i].info.toucherFlags &= ~2;
                if (!isBlocked2) {
                    if (this->unk_312 == 1) {
                        phi_v0_2 = 0x1800;
                    } else {
                        phi_v0_2 = 0;
                    }

                    func_8002F6D4(play, &this->actor, 15.0f, this->actor.yawTowardsPlayer + phi_v0_2, 2.0f, 0);
                    sBossGanon2Zelda->unk_3C8 = 8;
                    this->unk_316 = 10;
                    break;
                }
            }
        }
    }

    if (this->unk_324 > 0.0f) {
        player = GET_PLAYER(play);
        temp_f2 = -200.0f - player->actor.world.pos.x;
        temp_f12 = -200.0f - player->actor.world.pos.z;

        if (sqrtf(SQ(temp_f2) + SQ(temp_f12)) > 784.0f) {
            for (j = 0; j < ARRAY_COUNT(player->flameTimers); j++) {
                player->flameTimers[j] = Rand_S16Offset(0, 200);
            }

            player->isBurning = true;
            func_8002F6D4(play, &this->actor, 10.0f, Math_Atan2S(temp_f12, temp_f2), 0.0f, 0x10);
            sBossGanon2Zelda->unk_3C8 = 8;
        }
    }
}

void func_80902524(BossGanon2* this, PlayState* play) {
    s8 temp_v0_4;
    ColliderInfo* acHitInfo;
    s16 i;
    u8 phi_v1_2;

    osSyncPrintf("this->no_hit_time %d\n", this->unk_316);
    if (this->unk_316 != 0 || ((this->unk_334 == 0) && (this->actionFunc == func_80900890))) {
        for (i = 0; i < ARRAY_COUNT(this->unk_464); i++) {
            this->unk_424.elements[i].info.bumperFlags &= ~2;
        }
    }

    osSyncPrintf("this->look_on %d\n", this->unk_313);
    if (this->unk_313) {
        if (this->actionFunc != func_808FFFE0) {
            if (this->unk_424.elements[0].info.bumperFlags & 2) {
                this->unk_424.elements[0].info.bumperFlags &= ~2;
                acHitInfo = this->unk_424.elements[0].info.acHitInfo;
                //This is currently never true
                if (0 && (acHitInfo->toucher.dmgFlags & 0x2000) && (this->actionFunc != func_80900890)) {
                    func_809000A0(this, play);
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_FANTOM_HIT_THUNDER);
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_DAMAGE);
                    Audio_StopSfxById(NA_SE_EN_MGANON_UNARI);
                } else if ((this->actionFunc == func_80900890) && (acHitInfo->toucher.dmgFlags & 0x9000200)) {
                    this->unk_316 = 60;
                    this->unk_342 = 5;
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_DAMAGE);
                    Audio_StopSfxById(NA_SE_EN_MGANON_UNARI);
                    this->actor.colChkInfo.health -= 2;
                    temp_v0_4 = this->actor.colChkInfo.health;
                    if (temp_v0_4 < 0x15 && this->unk_334 == 0) {
                        func_80900818(this, play);
                    } else {
                        if (temp_v0_4 <= 0) {
                            func_80901020(this, play);
                        } else {
                            func_80900210(this, play);
                        }
                    }
                } else if (this->actionFunc != func_80900890) {
                    func_808FFF90(this, play);
                    Audio_PlayActorSound2(&this->actor, NA_SE_IT_HOOKSHOT_REFLECT);
                }
            }
        }
    } else {
        if (this->unk_424.elements[15].info.bumperFlags & 2) {
            this->unk_424.elements[15].info.bumperFlags &= ~2;
            acHitInfo = this->unk_424.elements[15].info.acHitInfo;
            this->unk_316 = 60;
            this->unk_344 = 0x32;
            this->unk_342 = 5;
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_MGANON_DAMAGE);
            Audio_StopSfxById(NA_SE_EN_MGANON_UNARI);
            phi_v1_2 = 1;
            if (acHitInfo->toucher.dmgFlags & 0x9000200) {
                if (acHitInfo->toucher.dmgFlags & 0x8000000) {
                    phi_v1_2 = 4;
                } else {
                    phi_v1_2 = 2;
                }
            }
            this->actor.colChkInfo.health -= phi_v1_2;
            temp_v0_4 = this->actor.colChkInfo.health;
            if ((temp_v0_4 < 0x15) && (this->unk_334 == 0)) {
                func_80900818(this, play);
            } else if ((temp_v0_4 <= 0) && (phi_v1_2 >= 2)) {
                func_80901020(this, play);
            } else {
                if (temp_v0_4 <= 0) {
                    this->actor.colChkInfo.health = 1;
                }
                func_80900210(this, play);
            }
        }
    }
}

void BossGanon2_Update(Actor* thisx, PlayState* play) {
    BossGanon2* this = (BossGanon2*)thisx;
    s32 pad;
    s16 i;
    f32 phi_f2;
    u16 i2;
    Vec3f sp58;
    Vec3f sp4C;
    f32 angle;
    f32 sp44;

    if ((this->unk_337 == 0) || (this->unk_337 == 2)) {
        BossGanon2_SetObjectSegment(this, play, OBJECT_GANON_ANIME3, false);
    } else {
        BossGanon2_SetObjectSegment(this, play, OBJECT_GANON2, false);
        Math_ApproachZeroF(&this->unk_30C, 1.0f, 0.5f);
    }
    func_808FFC84(this);
    this->unk_312 = 0;
    this->unk_19C++;
    Actor_SetScale(&this->actor, 0.01f);
    this->actionFunc(this, play);
    for (i = 0; i < ARRAY_COUNT(this->unk_1A2); i++) {
        if (this->unk_1A2[i] != 0) {
            this->unk_1A2[i]--;
        }
    }
    if (this->unk_316 != 0) {
        this->unk_316--;
    }
    if (this->unk_342 != 0) {
        this->unk_342--;
    }
    if (this->unk_390 != 0) {
        this->unk_390--;
    }
    if (this->unk_392 != 0) {
        this->unk_392--;
    }
    Actor_MoveForward(&this->actor);
    this->actor.shape.rot = this->actor.world.rot;
    if (this->unk_335 != 0) {
        Actor_UpdateBgCheckInfo(play, &this->actor, 60.0f, 60.0f, 100.0f, 5);
        if (this->actor.bgCheckFlags & 1) {
            if (this->actor.velocity.y < -5.0f) {
                func_80033E88(&this->actor, play, 5, 20);
                func_80078884(NA_SE_IT_BOMB_EXPLOSION);
            }
            this->actor.velocity.y = 0.0f;
        }
    }
    if (((this->unk_19C & 0x1F) == 0) && (Rand_ZeroOne() < 0.3f)) {
        this->unk_318 = 4;
    }
    this->unk_310 = D_80907074[this->unk_318];
    if (this->unk_318 != 0) {
        this->unk_318--;
    }
    this->unk_1B0 = (Math_SinS(this->unk_19C * 0x2AAA) * 64.0f) + 191.0f;
    if (this->unk_344 != 0) {
        this->unk_344--;
        Math_ApproachF(&this->unk_360.x, 5000.0f, 0.5f, 3000.0f);
        Math_ApproachF(&this->unk_370.x, 5500.0f, 0.5f, 3000.0f);
        Math_ApproachF(&this->unk_360.z, 8000.0f, 0.1f, 4000.0f);
        Math_ApproachF(&this->unk_370.z, 8000.0f, 0.1f, 4000.0f);
        Math_ApproachS(&this->unk_346, 0xFA0, 0xA, 0x7D0);
    } else {
        this->unk_360.y = 14000.0f;
        Math_ApproachF(&this->unk_360.x, 2000.0f, 0.1f, 100.0f);
        this->unk_370.y = 12000.0f;
        Math_ApproachF(&this->unk_370.x, 1500.0f, 0.1f, 100.0f);
        if ((this->actionFunc == func_808FFEBC) || (this->actionFunc == func_808FFFE0) ||
            (this->actionFunc == func_80900104)) {
            Math_ApproachF(&this->unk_360.z, 1000.0f, 0.1f, 100.0f);
            Math_ApproachF(&this->unk_370.z, 1000.0f, 0.1f, 100.0f);
            Math_ApproachS(&this->unk_346, -0xFA0, 0xA, 0x64);
        } else {
            Math_ApproachF(&this->unk_360.z, 5000.0f, 0.1f, 200.0f);
            Math_ApproachF(&this->unk_370.z, 5000.0f, 0.1f, 200.0f);
            Math_ApproachS(&this->unk_346, 0, 0xA, 0x64);
        }
    }
    if (this->unk_39C != 75) {
        this->unk_35C += this->unk_360.x;
        this->unk_36C += this->unk_370.x;
    }
    if (this->unk_337 == 2) {
        this->unk_370.z = 0.0f;
        this->unk_360.z = 0.0f;
    }

    for (i = 0; i < ARRAY_COUNT(this->unk_348); i++) {
        if (i == 0) {
            phi_f2 = 0.2f;
        } else if (i == 1) {
            phi_f2 = 0.5f;
        } else {
            phi_f2 = 1.0f;
        }

        this->unk_348[i] = Math_SinS(((s16)this->unk_35C + (i * (s16)this->unk_360.y))) * phi_f2 * this->unk_360.z;
        this->unk_352[i] = Math_SinS(((s16)this->unk_36C + (i * (s16)this->unk_370.y))) * phi_f2 * this->unk_370.z;
    }

    func_808FF898(this, play);
    func_80902348(this, play);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->unk_424.base);
    if (this->actionFunc != func_8090120C) {
        func_80902524(this, play);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->unk_424.base);
        if (this->unk_312 != 1) {
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->swordSpheres1.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->swordSpheres1.base);
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->swordSpheres2.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->swordSpheres2.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->unk_444.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->unk_444.base);
        //CollisionCheck_SetOC(play, &play->colChkCtx, &this->unk_445.base);
        //CollisionCheck_SetAC(play, &play->colChkCtx, &this->unk_445.base);
        if (this->unk_39E == 0) {
            if (this->unk_312 != 1) {
                CollisionCheck_SetAT(play, &play->colChkCtx, &this->swordSpheres1.base);
            }
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->swordSpheres2.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->unk_444.base);
            //CollisionCheck_SetAT(play, &play->colChkCtx, &this->unk_445.base);
        }
    }
    if ((this->unk_332 == 0) && (this->unk_336 != 0)) {
        if (this->unk_336 == 2) {
            this->unk_332 = (s16)Rand_ZeroFloat(30.0f) + 8;
        } else {
            this->unk_332 = (s16)Rand_ZeroFloat(60.0f) + 0xA;
        }
        this->unk_339 = 0;
        play->envCtx.unk_BE = 0;
        play->envCtx.unk_BD = (s8)Rand_ZeroFloat(1.9f) + 1;
        play->envCtx.unk_D8 = 1.0f;
        D_8090EB20.y = 0.0f;
        D_8090EB20.x = D_8090EB20.y;
        D_8090EB20.z = D_8090EB20.x;
        if (Rand_ZeroOne() < 0.5f) {
            D_8090EB20.z = Rand_ZeroFloat(1000.0f);
        }
        func_80078914(&D_8090EB20, NA_SE_EV_LIGHTNING);
        this->unk_328 = 0xFF;
        this->unk_330 = 5;
        this->unk_32C = 0.0f;
        this->unk_340 = (s16)Rand_ZeroFloat(10000.0f);
    } else if (this->unk_332 != 0) {
        this->unk_332--;
    }
    if ((play->envCtx.unk_D8 > 0.0f) && (this->unk_336 != 0)) {
        play->envCtx.customSkyboxFilter = 1;
        play->envCtx.skyboxFilterColor[0] = 255;
        play->envCtx.skyboxFilterColor[1] = 255;
        play->envCtx.skyboxFilterColor[2] = 255;
        play->envCtx.skyboxFilterColor[3] = (s16)(play->envCtx.unk_D8 * 200.0f);
    } else {
        play->envCtx.customSkyboxFilter = 0;
    }
    play->envCtx.unk_BF = 0;
    play->envCtx.unk_DC = 2;

    switch (this->unk_339) {
        case 0:
            Math_ApproachZeroF(&play->envCtx.unk_D8, 1.0f, 0.1f);
            break;
        case 3:
            play->envCtx.unk_BE = 3;
            play->envCtx.unk_BD = 4;
            Math_ApproachF(&play->envCtx.unk_D8, 1.0f, 1.0f, 0.0125f);
            break;
        case 4:
            play->envCtx.unk_BE = 5;
            play->envCtx.unk_BD = 6;
            Math_ApproachF(&play->envCtx.unk_D8, 1.0f, 1.0f, 0.0125f);
            break;
        case 5:
            play->envCtx.unk_BE = 6;
            play->envCtx.unk_BD = 7;
            Math_ApproachF(&this->unk_33C, 0.69f, 1.0f, 0.05f);
            play->envCtx.unk_D8 =
                (Math_SinS(play->gameplayFrames * 0x5000) * 0.15f) + (0.15f + this->unk_33C);
            break;
        case 55:
            play->envCtx.unk_BE = 2;
            play->envCtx.unk_BD = 0;
            Math_ApproachZeroF(&play->envCtx.unk_D8, 1.0f, 0.05f);
            break;
        case 6:
            play->envCtx.unk_BE = 2;
            play->envCtx.unk_BD = 8;
            Math_ApproachF(&this->unk_33C, 0.69f, 1.0f, 0.05f);
            play->envCtx.unk_D8 =
                (Math_SinS(play->gameplayFrames * 0x7000) * 0.15f) + (0.15f + this->unk_33C);
            break;
        case 7:
            play->envCtx.unk_BE = 0;
            play->envCtx.unk_BD = 8;
            Math_ApproachZeroF(&play->envCtx.unk_D8, 1.0f, 0.02f);
            break;
        case 20:
            play->envCtx.unk_BE = 0;
            play->envCtx.unk_BD = 9;
            break;
        case 21:
            play->envCtx.unk_BE = 0xA;
            play->envCtx.unk_BD = 9;
            break;
        case 22:
            play->envCtx.unk_BE = 0xA;
            play->envCtx.unk_BD = 0xB;
            break;
        case 23:
            play->envCtx.unk_BE = 9;
            play->envCtx.unk_BD = 0xB;
            break;
        case 24:
            play->envCtx.unk_BE = 0;
            play->envCtx.unk_BD = 0xC;
            break;
        case -1:
            break;
    }

    if (this->unk_339 >= 0) {
        this->unk_339 = 0;
    }
    if (D_80906D78 != 0) {
        D_80906D78 = 0;

        for (i2 = 0; i2 < ARRAY_COUNT(sBossGanon2Particles); i2++) {
            angle = Rand_ZeroFloat(2 * M_PI);
            sp44 = Rand_ZeroFloat(40.0f) + 10.0f;
            sp58 = this->actor.world.pos;
            sp58.y = 1200.0f;
            sp4C.x = cosf(angle) * sp44;
            sp4C.z = sinf(angle) * sp44;
            sp4C.y = Rand_ZeroFloat(15.0f) + 15.0f;
            sp58.x += sp4C.x * 10.0f * 0.1f;
            sp58.z += sp4C.z * 10.0f * 0.1f;
            func_808FD27C(play, &sp58, &sp4C, Rand_ZeroFloat(0.3f) + 0.2f);
        }
    }
    this->unk_388 += 0.15f;
    func_80905DA8(this, play);
}

void func_809034E4(Vec3f* arg0, Vec3f* arg1) {
    Vtx* vtx;
    Vec3f sp2D0;
    s16 temp_s1;
    s16 temp_a1;
    s16 sp2CA;
    s16 sp2C8;
    s16 i;
    u8 phi_s2;
    u8 temp_s4;
    u8 temp_s4_2;
    f32 temp_f12;
    Vec3f temp_f20;
    Vec3f temp_f2;
    Vec3f temp_f22;
    f32 sp294;
    f32 phi_f30;
    f32 temp_f28;
    f32 temp_f26;
    s32 pad[3];
    Vec3f sp18C[20];
    Vec3f sp9C[20];

    for (i = 0; i < 20; i++) {
        sp18C[i] = *arg0;
        sp9C[i] = *arg1;
    }

    temp_s4 = 0;

    D_809105D8[3] = D_809105D8[2];
    D_809105D8[2] = D_809105D8[1];
    D_809105D8[1] = D_809105D8[0];
    D_809105D8[0] = *arg0;

    sp2D0 = D_809105D8[0];

    temp_f20.x = D_809105D8[1].x - sp2D0.x;
    temp_f20.y = D_809105D8[1].y - sp2D0.y;
    temp_f20.z = D_809105D8[1].z - sp2D0.z;

    sp2CA = Math_Atan2S(temp_f20.z, temp_f20.x);
    sp2C8 = Math_Atan2S(sqrtf(SQXZ(temp_f20)), temp_f20.y);

    temp_f2.x = D_809105D8[2].x - D_809105D8[1].x;
    temp_f2.y = D_809105D8[2].y - D_809105D8[1].y;
    temp_f2.z = D_809105D8[2].z - D_809105D8[1].z;

    temp_f22.x = D_809105D8[3].x - D_809105D8[2].x;
    temp_f22.y = D_809105D8[3].y - D_809105D8[2].y;
    temp_f22.z = D_809105D8[3].z - D_809105D8[2].z;

    temp_f12 = sqrtf(SQXYZ(temp_f20)) + sqrtf(SQXYZ(temp_f2)) + sqrtf(SQXYZ(temp_f22));
    if (temp_f12 <= 1.0f) {
        temp_f12 = 1.0f;
    }

    temp_f28 = temp_f12 * 0.083f;
    phi_f30 = sqrtf(SQXYZ(temp_f20)) / 2.0f;
    sp294 = sqrtf(SQXYZ(temp_f2)) / 2.0f;

    phi_s2 = 1;

    while (true) {
        temp_f20.x = D_809105D8[phi_s2].x - sp2D0.x;
        temp_f20.y = D_809105D8[phi_s2].y - sp2D0.y;
        temp_f20.z = D_809105D8[phi_s2].z - sp2D0.z;

        temp_s1 = Math_Atan2S(temp_f20.z, temp_f20.x);
        temp_a1 = Math_Atan2S(sqrtf(SQXZ(temp_f20)), temp_f20.y);

        Math_ApproachS(&sp2C8, temp_a1, 1, 0x1000);
        Math_ApproachS(&sp2CA, temp_s1, 1, 0x1000);

        temp_f26 = temp_f28 * Math_CosS(sp2C8);

        sp18C[temp_s4] = sp2D0;

        sp2D0.x += temp_f26 * Math_SinS(sp2CA);
        sp2D0.y += temp_f28 * Math_SinS(sp2C8);
        sp2D0.z += temp_f26 * Math_CosS(sp2CA);

        temp_f20.x = D_809105D8[phi_s2].x - sp2D0.x;
        temp_f20.y = D_809105D8[phi_s2].y - sp2D0.y;
        temp_f20.z = D_809105D8[phi_s2].z - sp2D0.z;

        if (phi_s2 < 3) {
            if (sqrtf(SQXYZ(temp_f20)) <= phi_f30) {
                phi_f30 = sp294;
                phi_s2++;
            }
        } else {
            if (sqrtf(SQXYZ(temp_f20)) <= (temp_f28 + 1.0f)) {
                phi_s2++;
            }
        }

        temp_s4++;

        if ((temp_s4 >= 20) || (phi_s2 >= 4)) {
            break;
        }
    }

    temp_s4_2 = 0;

    D_80910608[3] = D_80910608[2];
    D_80910608[2] = D_80910608[1];
    D_80910608[1] = D_80910608[0];
    D_80910608[0] = *arg1;

    sp2D0 = D_80910608[0];

    temp_f20.x = D_80910608[1].x - sp2D0.x;
    temp_f20.y = D_80910608[1].y - sp2D0.y;
    temp_f20.z = D_80910608[1].z - sp2D0.z;

    sp2CA = Math_Atan2S(temp_f20.z, temp_f20.x);
    sp2C8 = Math_Atan2S(sqrtf(SQXZ(temp_f20)), temp_f20.y);

    temp_f2.x = D_80910608[2].x - D_80910608[1].x;
    temp_f2.y = D_80910608[2].y - D_80910608[1].y;
    temp_f2.z = D_80910608[2].z - D_80910608[1].z;

    temp_f22.x = D_80910608[3].x - D_80910608[2].x;
    temp_f22.y = D_80910608[3].y - D_80910608[2].y;
    temp_f22.z = D_80910608[3].z - D_80910608[2].z;

    temp_f12 = sqrtf(SQXYZ(temp_f20)) + sqrtf(SQXYZ(temp_f2)) + sqrtf(SQXYZ(temp_f22));
    if (temp_f12 <= 1.0f) {
        temp_f12 = 1.0f;
    }

    temp_f28 = temp_f12 * 0.083f;
    phi_f30 = sqrtf(SQXYZ(temp_f20)) / 2.0f;
    sp294 = sqrtf(SQXYZ(temp_f2)) / 2.0f;

    phi_s2 = 1;

    while (true) {
        temp_f20.x = D_80910608[phi_s2].x - sp2D0.x;
        temp_f20.y = D_80910608[phi_s2].y - sp2D0.y;
        temp_f20.z = D_80910608[phi_s2].z - sp2D0.z;

        temp_s1 = Math_Atan2S(temp_f20.z, temp_f20.x);
        temp_a1 = Math_Atan2S(sqrtf(SQXZ(temp_f20)), temp_f20.y);

        Math_ApproachS(&sp2C8, temp_a1, 1, 0x1000);
        Math_ApproachS(&sp2CA, temp_s1, 1, 0x1000);

        temp_f26 = temp_f28 * Math_CosS(sp2C8);

        sp9C[temp_s4_2] = sp2D0;

        sp2D0.x += temp_f26 * Math_SinS(sp2CA);
        sp2D0.y += temp_f28 * Math_SinS(sp2C8);
        sp2D0.z += temp_f26 * Math_CosS(sp2CA);

        temp_f20.x = D_80910608[phi_s2].x - sp2D0.x;
        temp_f20.y = D_80910608[phi_s2].y - sp2D0.y;
        temp_f20.z = D_80910608[phi_s2].z - sp2D0.z;

        if (phi_s2 < 3) {
            if (sqrtf(SQXYZ(temp_f20)) <= phi_f30) {
                phi_f30 = sp294;
                phi_s2++;
            }
        } else {
            if (sqrtf(SQXYZ(temp_f20)) <= (temp_f28 + 1.0f)) {
                phi_s2++;
            }
        }

        temp_s4_2++;

        if ((temp_s4_2 >= 20) || (phi_s2 >= 4)) {
            break;
        }
    }

    vtx = ResourceMgr_LoadVtxByName(SEGMENTED_TO_VIRTUAL(ovl_Boss_Ganon2_Vtx_00BA20));
    for (i = 0; i < 11; i++) {
        if ((temp_s4 - i) > 0) {
            vtx[D_80907084[i]].n.ob[0] = sp18C[temp_s4 - i - 1].x;
            vtx[D_80907084[i]].n.ob[1] = sp18C[temp_s4 - i - 1].y;
            vtx[D_80907084[i]].n.ob[2] = sp18C[temp_s4 - i - 1].z;
        }
        if ((temp_s4_2 - i) > 0) {
            vtx[D_80907090[i]].n.ob[0] = sp9C[temp_s4_2 - i - 1].x;
            vtx[D_80907090[i]].n.ob[1] = sp9C[temp_s4_2 - i - 1].y;
            vtx[D_80907090[i]].n.ob[2] = sp9C[temp_s4_2 - i - 1].z;
        }
    }
}

void func_80903F38(BossGanon2* this, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    if (this->unk_312 != 0) {
        func_809034E4(&this->unk_200, &this->unk_20C);
        D_80907080 = 0xFF;
    }

    if (D_80910638 >= 4) {
        gSPSegment(
            POLY_XLU_DISP++, 0x08,
            Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 32, 32, 1, play->gameplayFrames * 18, 0, 32, 32));
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, D_80907080);
        Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_NEW);
        gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, ovl_Boss_Ganon2_DL_00BB80);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80904108(BossGanon2* this, PlayState* play) {
    s32 pad;

    if (this->unk_324 > 0.0f) {
        OPEN_DISPS(play->state.gfxCtx);

        Matrix_Push();
        gDPPipeSync(POLY_XLU_DISP++);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(play->state.gfxCtx, 0, (s32)play->gameplayFrames, 0, 32, 64, 1,
                                    -play->gameplayFrames * 2, -play->gameplayFrames * 8, 32, 32));
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 200, 0, (s8)this->unk_324);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 128);
        Matrix_Translate(-200.0f, 1086.0f, -200.0f, MTXMODE_NEW);
        Matrix_Scale(0.098000005f, 0.1f, 0.098000005f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(ovl_Boss_Ganon2_DL_00E1C0));
        Matrix_Pop();

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void func_80904340(BossGanon2* this, PlayState* play) {
    s16 i;
    f32 rand;
    f32 angle;
    f32 sin;
    f32 cos;

    OPEN_DISPS(play->state.gfxCtx);
    Matrix_Push();

    if ((this->unk_330 != 0) || (this->unk_328 != 0)) {
        if (this->unk_330 != 0) {
            this->unk_330--;
        } else {
            this->unk_328 -= 70;

            if (this->unk_328 < 0) {
                this->unk_328 = 0;
            }
        }

        Math_ApproachF(&this->unk_32C, 0.13f, 1.0f, 0.065f);
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, this->unk_328);
        BossGanon2_InitRand(this->unk_340 + 1, 0x71AC - this->unk_340, 0x263A);
        rand = BossGanon2_RandZeroOne();

        for (i = 0; i < 5; i++) {
            FrameInterpolation_RecordOpenChild("Ganon 80904340", i);
            angle = (i * (2 * M_PI / 5)) + (rand * M_PI);
            sin = 5000.0f * sinf(angle);
            cos = 5000.0f * cosf(angle);
            Matrix_Translate(-200.0f + sin, 4786.0f, -200.0f + cos, MTXMODE_NEW);
            Matrix_Scale(this->unk_32C, this->unk_32C, this->unk_32C, MTXMODE_APPLY);
            Matrix_RotateY(angle, MTXMODE_APPLY);
            Matrix_RotateZ((BossGanon2_RandZeroOne() - 0.5f) * 100.0f * 0.01f, MTXMODE_APPLY);

            if (BossGanon2_RandZeroOne() < 0.5f) {
                Matrix_RotateY(M_PI, MTXMODE_APPLY);
            }

            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(ovl_Boss_Ganon2_DL_00D798));
            FrameInterpolation_RecordCloseChild();
        }
    }

    Matrix_Pop();
    CLOSE_DISPS(play->state.gfxCtx);
}

void func_8090464C(BossGanon2* this, PlayState* play) {
    s32 pad;

    if (this->unk_1B4 > 0.0f) {
        OPEN_DISPS(play->state.gfxCtx);

        Matrix_Push();
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 170, (s16)this->unk_1B4);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 200, 0, 128);
        Matrix_Translate(this->unk_1B8.x, this->unk_1B8.y, this->unk_1B8.z, MTXMODE_NEW);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        Matrix_RotateZ(-0.2f, MTXMODE_APPLY);
        Matrix_Scale(0.6f, 0.6f, 1.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(ovl_Boss_Ganon2_DL_00CCD8));
        Matrix_Pop();

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

s32 BossGanon2_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                void* thisx) {
    s32 pad;
    BossGanon2* this = (BossGanon2*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    if (limbIndex == 15) {
        rot->y += this->unk_31A;
        rot->z += this->unk_31C;
    }

    if (limbIndex >= 42) {
        rot->x += this->unk_2F4[limbIndex] + this->unk_346;
        rot->y += this->unk_2FE[limbIndex];

        if (this->unk_342 & 1) {
            gDPSetEnvColor(POLY_OPA_DISP++, 255, 0, 0, 255);
        } else {
            gDPSetEnvColor(POLY_OPA_DISP++, (s16)this->unk_1B0, (s16)this->unk_1B0, (s16)(*this).unk_1B0, 255);
        }
    }

    if ((limbIndex == 7) || (limbIndex == 13) || (limbIndex == 33) || (limbIndex == 34)) {
        *dList = NULL;
    }

    CLOSE_DISPS(play->state.gfxCtx);
    return 0;
}

void BossGanon2_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    s8 pad;
    s8 temp_v0;
    BossGanon2* this = (BossGanon2*)thisx;
    Vec3f sp4C;

    OPEN_DISPS(play->state.gfxCtx);

    D_80907120.z = 17000.0f;
    D_8090712C.z = 3000.0f;

    if (D_809070CC[limbIndex] >= 0) {
        Matrix_MultVec3f(&D_80906D60, &this->unk_234[D_809070CC[limbIndex]]);
    }

    if (limbIndex == 15) {
        Matrix_MultVec3f(&D_80906D60, &this->unk_1B8);
    } else if (limbIndex == 3) {
        Matrix_MultVec3f(&D_80907108, &this->unk_1F4);
    } else if (limbIndex == 9) {
        Matrix_MultVec3f(&D_80907114, &this->unk_1E8);
    } else if (limbIndex == 38) {
        Matrix_MultVec3f(&D_80906D60, &this->unk_1DC);
    } else if (limbIndex == 41) {
        Matrix_MultVec3f(&D_80906D60, &this->unk_1D0);
    } else if (limbIndex == 45) {
        Matrix_MultVec3f(&D_80907138, &this->unk_1C4);
    }

    temp_v0 = D_8090709C[limbIndex];
    if (temp_v0 >= 0) {
        Matrix_MultVec3f(&D_80906D60, &sp4C);
        func_808FD080(temp_v0, &this->unk_424, &sp4C);
    }

    if ((limbIndex == 7) || (limbIndex == 13)) {
        Matrix_Push();
        Matrix_Scale(this->unk_224, this->unk_224, this->unk_224, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, *dList);
        Matrix_Pop();
    } else if ((limbIndex == 33) || (limbIndex == 34)) {
        Matrix_Push();
        Matrix_Scale(this->unk_228, this->unk_228, this->unk_228, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, *dList);
        Matrix_Pop();
    }

    if (*dList != NULL) {
        if ((limbIndex == 7)) {
            Vec3f tripos[3];
            Vec3f tripos2[3];
            Vec3f spherePos[4];
            Vec3f* leftVerts1;
            Vec3f* leftVerts2;
            if (1 || this->unk_312 == 1) {
                leftVerts1 = Sword_VertsAug;
                leftVerts2 = Sword_VertsAug2;
            } else {
                leftVerts1 = Sword_Verts;
                leftVerts2 = Sword_Verts2;
            }
            Matrix_MultVec3f(&SpherePos0, spherePos);
            Matrix_MultVec3f(&D_809070FC, spherePos+1);
            Matrix_MultVec3f(&SpherePos2, spherePos+2);
            Matrix_MultVec3f(&SpherePos3, spherePos+3);
            Matrix_MultVec3f(&leftVerts1[0], &tripos[0]);
            Matrix_MultVec3f(&leftVerts1[1], &tripos[1]);
            Matrix_MultVec3f(&leftVerts1[2], &tripos[2]);
            Matrix_MultVec3f(&leftVerts2[0], &tripos2[0]);
            Matrix_MultVec3f(&leftVerts2[1], &tripos2[1]);
            Matrix_MultVec3f(&leftVerts2[2], &tripos2[2]);
            func_808FD080(0, &this->swordSpheres1, spherePos);
            func_808FD080(1, &this->swordSpheres1, spherePos+1);
            func_808FD080(2, &this->swordSpheres1, spherePos+2);
            func_808FD080(3, &this->swordSpheres1, spherePos+3);
            Collider_SetTrisVertices(&this->unk_444, 0, tripos,tripos+1,tripos+2);
            Collider_SetTrisVertices(&this->unk_444, 1, tripos2,tripos2+1,tripos2+2);
            Matrix_MultVec3f(&D_80907120, &this->unk_200);
            Matrix_MultVec3f(&D_8090712C, &this->unk_20C);
            if (this->unk_312 == 1) {
                Matrix_MultVec3f(&D_809070FC, &this->unk_218);
                for (s32 ii = 0; ii < ARRAY_COUNT(this->unk_864); ii++) {
                    this->unk_444.elements[ii].info.toucher.dmgFlags = 0x20000000;
                    this->unk_444.elements[ii].info.bumper.dmgFlags = 0xFFCFFFFF;
                }
                for (s32 ii = 0; ii < ARRAY_COUNT(this->swordSphereElement1); ii++) {
                    this->swordSpheres1.elements[ii].info.toucher.dmgFlags = 0x20000000;
                    this->swordSpheres1.elements[ii].info.bumper.dmgFlags = 0xFFCFFFFF;
                }
            } else {
                for (s32 ii = 0; ii < ARRAY_COUNT(this->unk_864); ii++) {
                    this->unk_444.elements[ii].info.toucher.dmgFlags = 0xFFCFFFFF;
                    this->unk_444.elements[ii].info.bumper.dmgFlags = 0xFFDFFFFF;
                }
                for (s32 ii = 0; ii < ARRAY_COUNT(this->swordSphereElement1); ii++) {
                    this->swordSpheres1.elements[ii].info.toucher.dmgFlags = 0xFFCFFFFF;
                    this->swordSpheres1.elements[ii].info.bumper.dmgFlags = 0xFFDFFFFF;
                }
            }
        } else if ((limbIndex == 13)) {
            Vec3f tripos[3];
            Vec3f tripos2[3];
            Vec3f spherePos[8];
            Matrix_MultVec3f(&SpherePos0, spherePos);
            Matrix_MultVec3f(&D_809070FC, spherePos+1);
            Matrix_MultVec3f(&SpherePos2, spherePos+2);
            Matrix_MultVec3f(&SpherePos3, spherePos+3);
            Matrix_MultVec3f(&SpherePosX0, spherePos+4);
            Matrix_MultVec3f(&SpherePosX1, spherePos+5);
            Matrix_MultVec3f(&SpherePosX2, spherePos+6);
            Matrix_MultVec3f(&SpherePosX3, spherePos+7);
            Matrix_MultVec3f(&Sword_Verts[0], &tripos[0]);
            Matrix_MultVec3f(&Sword_Verts[1], &tripos[1]);
            Matrix_MultVec3f(&Sword_Verts[2], &tripos[2]);
            Matrix_MultVec3f(&Sword_Verts2[0], &tripos2[0]);
            Matrix_MultVec3f(&Sword_Verts2[1], &tripos2[1]);
            Matrix_MultVec3f(&Sword_Verts2[2], &tripos2[2]);
            func_808FD080(0, &this->swordSpheres2, spherePos);
            func_808FD080(1, &this->swordSpheres2, spherePos+1);
            func_808FD080(2, &this->swordSpheres2, spherePos+2);
            func_808FD080(3, &this->swordSpheres2, spherePos+3);
            func_808FD080(4, &this->swordSpheres2, spherePos+4);
            func_808FD080(5, &this->swordSpheres2, spherePos+5);
            func_808FD080(5, &this->swordSpheres2, spherePos+6);
            func_808FD080(5, &this->swordSpheres2, spherePos+7);
            Collider_SetTrisVertices(&this->unk_445, 0, tripos,tripos+1,tripos+2);
            Collider_SetTrisVertices(&this->unk_445, 1, tripos2,tripos2+1,tripos2+2);
            Matrix_MultVec3f(&D_80907120, &this->unk_200);
            Matrix_MultVec3f(&D_8090712C, &this->unk_20C);
            if (this->unk_312 == 2) {
                Matrix_MultVec3f(&D_809070FC, &this->unk_218);
                for (s32 ii = 0; ii < ARRAY_COUNT(this->unk_865); ii++) {
                    this->unk_445.elements[ii].info.toucher.dmgFlags = 0x20000000;
                    this->unk_445.elements[ii].info.bumper.dmgFlags = 0xFFCFFFFF;
                }
                for (s32 ii = 0; ii < ARRAY_COUNT(this->swordSphereElement2); ii++) {
                    this->swordSpheres2.elements[ii].info.toucher.dmgFlags = 0x20000000;
                    this->swordSpheres2.elements[ii].info.bumper.dmgFlags = 0xFFCFFFFF;
                }
            } else {
                for (s32 ii = 0; ii < ARRAY_COUNT(this->unk_865); ii++) {
                    this->unk_445.elements[ii].info.toucher.dmgFlags = 0xFFCFFFFF;
                    this->unk_445.elements[ii].info.bumper.dmgFlags = 0xFFDFFFFF;
                }
                for (s32 ii = 0; ii < ARRAY_COUNT(this->swordSphereElement2); ii++) {
                    this->swordSpheres2.elements[ii].info.toucher.dmgFlags = 0xFFCFFFFF;
                    this->swordSpheres2.elements[ii].info.bumper.dmgFlags = 0xFFDFFFFF;
                }
            }
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80904D88(BossGanon2* this, PlayState* play) {
    s32 pad;
    s16 i;

    OPEN_DISPS(play->state.gfxCtx);

    if (this->unk_30C > 0.0f) {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        if (this->unk_380 > 0.0f) {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 170, 255);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 200, 0, 0);
        } else {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
            gDPSetEnvColor(POLY_XLU_DISP++, 100, 255, 255, 0);
        }
        gSPDisplayList(POLY_XLU_DISP++, ovl_Boss_Ganon2_DL_00B308);

        for (i = 0; i < 15; i++) {
            FrameInterpolation_RecordOpenChild("Ganon 80904D88", i);

            Matrix_Translate(this->unk_234[i].x, this->unk_234[i].y, this->unk_234[i].z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(this->unk_30C, this->unk_30C, this->unk_30C, MTXMODE_APPLY);
            Matrix_RotateZ(Rand_CenteredFloat(M_PI), MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, ovl_Boss_Ganon2_DL_00B378);
            
            FrameInterpolation_RecordCloseChild();
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80904FC8(BossGanon2* this, PlayState* play) {
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx);

    if (this->unk_384 > 0.0f) {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 200);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 200, 0, 0);
        gSPDisplayList(POLY_XLU_DISP++, ovl_Boss_Ganon2_DL_00B308);
        Matrix_Translate(sBossGanon2Zelda->actor.world.pos.x, sBossGanon2Zelda->actor.world.pos.y + 80.0f, sBossGanon2Zelda->actor.world.pos.z,
                         MTXMODE_NEW);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        Matrix_Scale(this->unk_384, this->unk_384, this->unk_384, MTXMODE_APPLY);
        Matrix_RotateZ(this->unk_388, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(ovl_Boss_Ganon2_DL_00B378));
        Matrix_RotateZ(this->unk_388 * -2.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(ovl_Boss_Ganon2_DL_00B378));
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_8090523C(BossGanon2* this, PlayState* play) {
    Player* player;
    f32 phi_f20;

    OPEN_DISPS(play->state.gfxCtx);

    if (this->unk_38C > 0.0f) {
        s8 i;

        player = GET_PLAYER(play);
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (s16)this->unk_38C);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 255, 255, 0);
        gSPDisplayList(POLY_XLU_DISP++, ovl_Boss_Ganon2_DL_00B308);

        for (i = 0; i < 11; i++) {
            FrameInterpolation_RecordOpenChild("Ganon 8090523C", i);

            Matrix_Mult(&player->mf_9E0, MTXMODE_NEW);
            Matrix_Translate((i * 250.0f) + 900.0f, 350.0f, 0.0f, MTXMODE_APPLY);

            if (i < 7) {
                phi_f20 = 1.0f;
            } else {
                phi_f20 = 1.0f - ((i - 7) * 0.2333333f); // 7 / 30
            }

            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(200.0f * phi_f20, 200.0f * phi_f20, 1.0f, MTXMODE_APPLY);
            Matrix_RotateZ(Rand_ZeroFloat(2.0f * M_PI), MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(ovl_Boss_Ganon2_DL_00B378));
            
            FrameInterpolation_RecordCloseChild();
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void BossGanon2_PostLimbDraw2(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    s8 temp_v1 = D_80907144[limbIndex];
    BossGanon2* this = (BossGanon2*)thisx;

    if (temp_v1 >= 0) {
        Matrix_MultVec3f(&D_80906D60, &this->unk_234[temp_v1]);
    }
    if (limbIndex == 11) {
        OPEN_DISPS(play->state.gfxCtx);

        gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(gGanondorfEyesDL));

        CLOSE_DISPS(play->state.gfxCtx);
    } else if (limbIndex == 10) {
        Matrix_MultVec3f(&D_80907164, &this->unk_1B8);
    }
}

void func_80905674(BossGanon2* this, PlayState* play) {
    s32 pad;

    if (this->unk_380 > 0.0f) {
        OPEN_DISPS(play->state.gfxCtx);

        Matrix_Push();
        gDPPipeSync(POLY_XLU_DISP++);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(play->state.gfxCtx, 0, this->unk_19C * -8, 0, 32, 64, 1, this->unk_19C * -4,
                                    this->unk_19C * -8, 32, 32));
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 170, (s16)this->unk_37C);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 200, 0, 128);
        Matrix_Translate(sBossGanon2Zelda->actor.world.pos.x + 100.0f, sBossGanon2Zelda->actor.world.pos.y + 35.0f + 7.0f,
                         sBossGanon2Zelda->actor.world.pos.z - 100.0f, MTXMODE_NEW);
        Matrix_RotateY(-M_PI / 4.0f, MTXMODE_APPLY);
        Matrix_Scale(0.040000003f, 0.040000003f, this->unk_380, MTXMODE_APPLY);
        Matrix_RotateX(M_PI / 2.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(ovl_Boss_Ganon2_DL_00EC40));
        Matrix_Pop();

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void BossGanon2_Draw(Actor* thisx, PlayState* play) {
    void* shadowTexture = Graph_Alloc(play->state.gfxCtx, 4096);
    BossGanon2* this = (BossGanon2*)thisx;
    s16 i;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    switch (this->unk_337) {
        case 0:
            BossGanon2_SetObjectSegment(this, play, OBJECT_GANON, true);
            gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(gGanondorfEmptyEyeTex));
            gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(gGanondorfEmptyEyeTex));
            SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                  this->skelAnime.dListCount, NULL, BossGanon2_PostLimbDraw2, this);
            break;
        case 1:
        case 2:
            BossGanon2_SetObjectSegment(this, play, OBJECT_GANON2, true);
            gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[this->unk_310]));
            //func_808FD080(0, &this->unk_444, &D_8090717C);
            //func_808FD080(1, &this->unk_444, &D_8090717C);
            for (s16 ii = 0; ii < ARRAY_COUNT(this->swordSphereElement1); ii++) {
                func_808FD080(ii, &this->swordSpheres1, &D_8090717C);
            }
            for (s16 ii = 0; ii < ARRAY_COUNT(this->swordSphereElement2); ii++) {
                func_808FD080(ii, &this->swordSpheres2, &D_8090717C);
            }
            Collider_SetTrisVertices(&this->unk_444, 0, Sword_Verts_Rev,Sword_Verts_Rev+1,Sword_Verts_Rev+2);
            Collider_SetTrisVertices(&this->unk_444, 1, Sword_Verts_Rev,Sword_Verts_Rev+1,Sword_Verts_Rev+2);
            Collider_SetTrisVertices(&this->unk_445, 0, Sword_Verts_Rev,Sword_Verts_Rev+1,Sword_Verts_Rev+2);
            Collider_SetTrisVertices(&this->unk_445, 1, Sword_Verts_Rev,Sword_Verts_Rev+1,Sword_Verts_Rev+2);
            this->unk_218 = D_8090717C;
            if (this->unk_342 & 1) {
                POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, 0xFF, 0, 0, 0xFF, 0x384, 0x44B);
            }
            Matrix_Translate(0.0f, -4000.0f, 4000.0f, MTXMODE_APPLY);
            Matrix_RotateX(this->unk_394, MTXMODE_APPLY);
            Matrix_Translate(0.0f, 4000.0f, -4000.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                  this->skelAnime.dListCount, BossGanon2_OverrideLimbDraw, BossGanon2_PostLimbDraw,
                                  this);
            POLY_OPA_DISP = Play_SetFog(play, POLY_OPA_DISP);
            BossGanon2_GenShadowTexture(shadowTexture, this, play);
            BossGanon2_DrawShadowTexture(shadowTexture, this, play);
            break;
    }

    BossGanon2_SetObjectSegment(this, play, OBJECT_GANON2, true);
    func_80904340(this, play);
    func_80904108(this, play);
    func_80904D88(this, play);
    func_8090464C(this, play);
    func_80905674(this, play);
    func_80904FC8(this, play);
    func_8090523C(this, play);

    if ((this->unk_312 != 0) || (D_80907080 != 0)) {
        func_80903F38(this, play);
        if (this->unk_312 == 0) {
            s32 pad;

            D_80907080 -= 40;
            if (D_80907080 <= 0) {
                D_80907080 = 0;
            }
        }

        D_80910638++;
    } else {
        for (i = 0; i < 3; i++) {
            D_809105D8[i] = this->unk_200;
            D_80910608[i] = this->unk_20C;
        }

        D_80910638 = 0;
    }

    CLOSE_DISPS(play->state.gfxCtx);

    func_809060E8(play);
}

void func_80905DA8(BossGanon2* this, PlayState* play) {
    s32 pad[5];
    Player* player = GET_PLAYER(play);
    BossGanon2Effect* effect = play->specialEffects;
    Vec3f sp78;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(sBossGanon2Particles); i++, effect++) {
        if (effect->type != 0) {
            effect->position.x += effect->velocity.x;
            effect->position.y += effect->velocity.y;
            effect->position.z += effect->velocity.z;
            effect->unk_01++;
            effect->velocity.x += effect->accel.x;
            effect->velocity.y += effect->accel.y;
            effect->velocity.z += effect->accel.z;
            if (effect->type == 1) {
                if (effect->unk_2E == 0) {
                    effect->unk_38.z += 1.0f;
                    effect->unk_38.y = (2.0f * M_PI) / 5.0f;
                } else {
                    effect->unk_38.z = M_PI / 2.0f;
                    effect->unk_38.y = 0.0f;
                    if (effect->position.y <= 1098.0f) {
                        effect->position.y = 1098.0f;
                        if (effect->velocity.y < -10.0f) {
                            sp78 = effect->position;
                            sp78.y = 1086.0f;
                            func_80078884(NA_SE_IT_SHIELD_REFLECT_SW);
                            CollisionCheck_SpawnShieldParticlesMetal(play, &sp78);
                        }
                        effect->velocity.y = 0.0f;
                    }
                    if ((SQ(player->actor.world.pos.x - effect->position.x) +
                         SQ(player->actor.world.pos.z - effect->position.z)) < SQ(25.0f)) {
                        effect->type = 0;
                        this->unk_39C = 10;
                    }
                }
            } else if (effect->type == 2) {
                effect->unk_38.x += 0.1f;
                effect->unk_38.y += 0.4f;
                if ((sqrtf(SQ(-200.0f - effect->position.x) + SQ(-200.0f - effect->position.z)) < 1000.0f)) {
                    if (effect->position.y < 1186.0f) {
                        if (effect->unk_2E == 0) {
                            effect->unk_2E++;
                            effect->position.y = 1186.0f;
                            effect->velocity.x *= 0.75f;
                            effect->velocity.z *= 0.75f;
                            effect->velocity.y *= -0.2f;
                        } else {
                            effect->type = 0;
                        }
                    }
                } else if ((effect->position.y < 0.0f)) {
                    effect->type = 0;
                }
            }
        }
    }
}

void func_809060E8(PlayState* play) {
    s16 alpha;
    u8 usingObjectGEff = false;
    BossGanon2Effect* effect;
    s16 i;
    BossGanon2Effect* effects;

    effects = effect = play->specialEffects;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    for (i = 0; i < 1; i++) {
        if (effect->type == 1) {
            FrameInterpolation_RecordOpenChild("Ganon 809060E8 0", i);

            Vec3f spA0;
            f32 temp_f0;
            f32 angle;

            Gfx_SetupDL_25Xlu(play->state.gfxCtx);
            spA0.x = play->envCtx.dirLight1.params.dir.x;
            spA0.y = play->envCtx.dirLight1.params.dir.y;
            spA0.z = play->envCtx.dirLight1.params.dir.z;
            func_8002EABC(&effect->position, &play->view.eye, &spA0, play->state.gfxCtx);
            Matrix_Translate(effect->position.x, effect->position.y, effect->position.z, MTXMODE_NEW);
            Matrix_Scale(0.03f, 0.03f, 0.03f, MTXMODE_APPLY);
            Matrix_RotateY(effect->unk_38.z, MTXMODE_APPLY);
            Matrix_RotateX(effect->unk_38.y, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(POLY_OPA_DISP++, 0x08,
                       Gfx_TexScroll(play->state.gfxCtx, 0, 0 - (play->gameplayFrames & 0x7F), 32, 32));
            gSPDisplayList(POLY_OPA_DISP++, ovl_Boss_Ganon2_DL_0103A8);
            if ((play->envCtx.unk_BD == 1) || (play->envCtx.unk_BD == 2)) {
                alpha = (s16)(play->envCtx.unk_D8 * 150.0f) + 50;
                angle = M_PI / 5.0f;
            } else {
                alpha = 100;
                angle = M_PI / 2.0f;
            }
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 0, alpha);
            temp_f0 = effect->position.y - 1098.0f;
            Matrix_Translate(effect->position.x + temp_f0, 1086.0f, (effect->position.z - 1.0f) + temp_f0, MTXMODE_NEW);
            Matrix_RotateY(angle, MTXMODE_APPLY);
            Matrix_Scale(1.0f, 0.0f, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, ovl_Boss_Ganon2_DL_00F188);
            
            FrameInterpolation_RecordCloseChild();
        }
    }

    effect = effects;

    for (i = 0; i < ARRAY_COUNT(sBossGanon2Particles); i++, effect++) {
        if (effect->type == 2) {
            FrameInterpolation_RecordOpenChild("Ganon 809060E8 1", i);

            if (!usingObjectGEff) {
                BossGanon2_SetObjectSegment(NULL, play, OBJECT_GEFF, true);
                usingObjectGEff++;
            }
            Matrix_Translate(effect->position.x, effect->position.y, effect->position.z, MTXMODE_NEW);
            Matrix_Scale(effect->scale, effect->scale, effect->scale, MTXMODE_APPLY);
            Matrix_RotateY(effect->unk_38.z, MTXMODE_APPLY);
            Matrix_RotateX(effect->unk_38.y, MTXMODE_APPLY);
            Matrix_RotateZ(effect->unk_38.x, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gGanonRubbleDL);
            
            FrameInterpolation_RecordCloseChild();
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80906538(BossGanon2* this, u8* shadowTexture, f32 arg2) {
    s16 temp_t0;
    s16 temp_v0;
    s16 temp_a3;
    s16 phi_v1;
    s16 phi_a1;
    s16 i;
    f32 lerpx;
    s16 j;
    f32 lerpy;
    f32 lerpz;
    Vec3f sp70;
    Vec3f sp64;

    for (i = 0; i < 15; i++) {
        if ((arg2 == 0.0f) || ((j = D_809071CC[i]) >= 0)) {
            if (arg2 > 0.0f) {
                lerpx = this->unk_234[i].x + (this->unk_234[j].x - this->unk_234[i].x) * arg2;
                lerpy = this->unk_234[i].y + (this->unk_234[j].y - this->unk_234[i].y) * arg2;
                lerpz = this->unk_234[i].z + (this->unk_234[j].z - this->unk_234[i].z) * arg2;

                sp70.x = lerpx - this->actor.world.pos.x;
                sp70.y = lerpy - this->actor.world.pos.y + 76.0f + 30.0f + 30.0f + 100.0f;
                sp70.z = lerpz - this->actor.world.pos.z;
            } else {
                sp70.x = this->unk_234[i].x - this->actor.world.pos.x;
                sp70.y = this->unk_234[i].y - this->actor.world.pos.y + 76.0f + 30.0f + 30.0f + 100.0f;
                sp70.z = this->unk_234[i].z - this->actor.world.pos.z;
            }

            Matrix_MultVec3f(&sp70, &sp64);
            sp64.x *= 0.2f;
            sp64.y *= 0.2f;
            temp_a3 = sp64.x + 32.0f;
            temp_t0 = (s16)sp64.y * 64;

            if (D_809071EC[i] == 2) {
                for (j = 0, phi_a1 = -0x180; j < 12; j++, phi_a1 += 0x40) {
                    for (phi_v1 = -D_809071B4[j]; phi_v1 < D_809071B4[j]; phi_v1++) {
                        temp_v0 = temp_a3 + phi_v1 + temp_t0 + phi_a1;
                        if ((temp_v0 >= 0) && (temp_v0 < 0x1000)) {
                            shadowTexture[temp_v0] = 0xFF;
                        }
                    }
                }
            } else if (D_809071EC[i] == 1) {
                for (j = 0, phi_a1 = -0x100; j < 8; j++, phi_a1 += 0x40) {
                    for (phi_v1 = -D_809071A4[j]; phi_v1 < D_809071A4[j]; phi_v1++) {
                        temp_v0 = temp_a3 + phi_v1 + temp_t0 + phi_a1;
                        if ((temp_v0 >= 0) && (temp_v0 < 0x1000)) {
                            shadowTexture[temp_v0] = 0xFF;
                        }
                    }
                }
            } else if (D_809071EC[i] == 0) {
                for (j = 0, phi_a1 = -0xC0; j < 7; j++, phi_a1 += 0x40) {
                    for (phi_v1 = -D_80907194[j]; phi_v1 < D_80907194[j] - 1; phi_v1++) {
                        temp_v0 = temp_a3 + phi_v1 + temp_t0 + phi_a1;
                        if ((temp_v0 >= 0) && (temp_v0 < 0x1000)) {
                            shadowTexture[temp_v0] = 0xFF;
                        }
                    }
                }
            } else {
                for (j = 0, phi_a1 = -0x80; j < 6; j++, phi_a1 += 0x40) {
                    for (phi_v1 = -D_80907188[j]; phi_v1 < D_80907188[j] - 1; phi_v1++) {
                        temp_v0 = temp_a3 + phi_v1 + temp_t0 + phi_a1;
                        if ((temp_v0 >= 0) && (temp_v0 < 0x1000)) {
                            shadowTexture[temp_v0] = 0xFF;
                        }
                    }
                }
            }
        }
    }
}

void BossGanon2_GenShadowTexture(void* shadowTexture, BossGanon2* this, PlayState* play) {
    s16 i;
    u32* p = shadowTexture;

    for (i = 0; i < 1024; i++, p++) {
        *p = 0;
    }

    Matrix_RotateX(1.0f, MTXMODE_NEW);

    for (i = 0; i < 6; i++) {
        func_80906538(this, shadowTexture, i / 5.0f);
    }
}

void BossGanon2_DrawShadowTexture(void* shadowTexture, BossGanon2* this, PlayState* play) {
    s32 pad;
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    s16 alpha;

    OPEN_DISPS(gfxCtx);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    if ((play->envCtx.unk_BD == 1) || (play->envCtx.unk_BD == 2)) {
        alpha = (s16)(play->envCtx.unk_D8 * 180.0f) + 30;
    } else {
        alpha = 120;
    }

    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 0, alpha);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0);
    Matrix_Translate(this->actor.world.pos.x, this->actor.floorHeight, this->actor.world.pos.z - 20.0f, MTXMODE_NEW);
    Matrix_Scale(1.65f, 1.0f, 1.65f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, ovl_Boss_Ganon2_DL_00B3D0);
    gDPLoadTextureBlock(POLY_OPA_DISP++, shadowTexture, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                        G_TX_NOMIRROR | G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
    gSPDisplayList(POLY_OPA_DISP++, ovl_Boss_Ganon2_DL_00B3F0);

    CLOSE_DISPS(gfxCtx);
}

void BossGanon2_Reset(void) {
    D_8090EB20.x = 0;
    D_8090EB20.y = 0;
    D_8090EB20.z = 0;
    D_80910638 = 0;
    sBossGanon2Zelda = NULL;
    D_8090EB30 = NULL;
    sBossGanon2Seed1 = 0;
    sBossGanon2Seed2 = 0;
    sBossGanon2Seed3 = 0;
    memset(D_809105D8, 0, sizeof(D_809105D8));
    memset(D_80910608, 0, sizeof(D_80910608));
    memset(sBossGanon2Particles, 0, sizeof(sBossGanon2Particles));
}