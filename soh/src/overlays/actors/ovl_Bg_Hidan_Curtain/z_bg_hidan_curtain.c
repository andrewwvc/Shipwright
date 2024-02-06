/*
 * File: z_bg_hidan_curtain.c
 * Overlay: ovl_Bg_Hidan_Curtain
 * Description: Flame circle
 */

#include "z_bg_hidan_curtain.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "overlays/actors/ovl_En_Bw/z_en_bw.h"

#define FLAGS ACTOR_FLAG_UPDATE_WHILE_CULLED

void BgHidanCurtain_Init(Actor* thisx, PlayState* play);
void BgHidanCurtain_Destroy(Actor* thisx, PlayState* play);
void BgHidanCurtain_Update(Actor* thisx, PlayState* play);
void BgHidanCurtain_Draw(Actor* thisx, PlayState* play);

void BgHidanCurtain_WaitForSwitchOn(BgHidanCurtain* this, PlayState* play);
void BgHidanCurtain_WaitForCutscene(BgHidanCurtain* this, PlayState* play);
void BgHidanCurtain_WaitForClear(BgHidanCurtain* this, PlayState* play);
void BgHidanCurtain_TurnOn(BgHidanCurtain* this, PlayState* play);
void BgHidanCurtain_TurnOff(BgHidanCurtain* this, PlayState* play);
void BgHidanCurtain_WaitForTimer(BgHidanCurtain* this, PlayState* play);
void BgHidanCurtain_WaitForSlugPower(BgHidanCurtain* this, PlayState* play);
void BgHidanCurtain_WaitForDragonPower(BgHidanCurtain* this, PlayState* play);

#define NORMAL_LIMIT -650.0f
#define LOWER_LIMIT -1000.0f

typedef struct {
    /* 0x00 */ s16 radius;
    /* 0x02 */ s16 height;
    /* 0x04 */ f32 scale;
    /* 0x08 */ f32 riseDist;
    /* 0x0C */ f32 riseSpeed;
} BgHidanCurtainParams; // size = 0x10

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x20000000, 0x01, 0x08 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 81, 144, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sCcInfoInit = { 1, 80, 100, MASS_IMMOVABLE };

static BgHidanCurtainParams sHCParams[] = { { 81, 144, 0.090f, 144.0f, 5.0f }, { 46, 88, 0.055f, 88.0f, 3.0f }, { 500, 180, 0.65f, 200.0f, 10.0f } };

const ActorInit Bg_Hidan_Curtain_InitVars = {
    ACTOR_BG_HIDAN_CURTAIN,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgHidanCurtain),
    (ActorFunc)BgHidanCurtain_Init,
    (ActorFunc)BgHidanCurtain_Destroy,
    (ActorFunc)BgHidanCurtain_Update,
    (ActorFunc)BgHidanCurtain_Draw,
    NULL,
};

void BgHidanCurtain_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    BgHidanCurtain* this = (BgHidanCurtain*)thisx;
    BgHidanCurtainParams* hcParams;

    osSyncPrintf("Curtain (arg_data 0x%04x)\n", this->actor.params);
    Actor_SetFocus(&this->actor, 20.0f);
    this->type = (thisx->params >> 0xC) & 0xF;
    if (this->type > 9) {
        // "Type is not set"
        osSyncPrintf("Error : object のタイプが設定されていない(%s %d)(arg_data 0x%04x)\n", __FILE__,
                     __LINE__, this->actor.params);
        Actor_Kill(&this->actor);
        return;
    }

    this->size = ((this->type == 2) || (this->type == 4) || (this->type == 8)) ? 1 : 0;
    if (this->type == 9) {
        this->size = 2;
    }

    hcParams = &sHCParams[this->size];
    this->treasureFlag = (thisx->params >> 6) & 0x3F;
    thisx->params &= 0x3F;

    if ((this->actor.params < 0) || (this->actor.params > 0x3F)) {
        // "Save bit is not set"
        osSyncPrintf("Warning : object のセーブビットが設定されていない(%s %d)(arg_data 0x%04x)\n",
                     __FILE__, __LINE__, this->actor.params);
    }
    Actor_SetScale(&this->actor, hcParams->scale);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    this->collider.dim.pos.x = this->actor.world.pos.x;
    this->collider.dim.pos.y = this->actor.world.pos.y;
    this->collider.dim.pos.z = this->actor.world.pos.z;
    this->collider.dim.radius = hcParams->radius;
    this->collider.dim.height = hcParams->height;
    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetInfo(&thisx->colChkInfo, NULL, &sCcInfoInit);
    if (this->type == 9) {
        this->collider.info.toucher.damage = 0x20;
        this->actor.world.pos.y =  -100.0f+NORMAL_LIMIT;
    }
    if (this->type == 0) {
        this->actionFunc = BgHidanCurtain_WaitForClear;
    } else if (this->type <= 6) {
        this->actionFunc = BgHidanCurtain_WaitForSwitchOn;
        if ((this->type == 4) || (this->type == 5)) {
            this->actor.world.pos.y = this->actor.home.pos.y - hcParams->riseDist;
        }
    } else if (this->type < 9) {
        this->actionFunc = BgHidanCurtain_WaitForSlugPower;
    } else {
        this->actionFunc = BgHidanCurtain_WaitForDragonPower;
    }
    if (((this->type == 1) && Flags_GetTreasure(play, this->treasureFlag)) ||
        (((this->type == 0) || (this->type == 6)) && Flags_GetClear(play, this->actor.room))) {
        Actor_Kill(&this->actor);
    }
    this->texScroll = Rand_ZeroOne() * 15.0f;
}

void BgHidanCurtain_Destroy(Actor* thisx, PlayState* play) {
    s32 pad;
    BgHidanCurtain* this = (BgHidanCurtain*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void BgHidanCurtain_WaitForSwitchOn(BgHidanCurtain* this, PlayState* play) {
    if (Flags_GetSwitch(play, this->actor.params)) {
        if (this->type == 1) {
            this->actionFunc = BgHidanCurtain_WaitForCutscene;
            OnePointCutscene_Init(play, 3350, -99, &this->actor, MAIN_CAM);
            this->timer = 50;
        } else if (this->type == 3) {
            this->actionFunc = BgHidanCurtain_WaitForCutscene;
            OnePointCutscene_Init(play, 3360, 60, &this->actor, MAIN_CAM);
            this->timer = 30;
        } else {
            this->actionFunc = BgHidanCurtain_TurnOff;
        }
    }
}

void BgHidanCurtain_WaitForCutscene(BgHidanCurtain* this, PlayState* play) {
    if (this->timer-- == 0) {
        this->actionFunc = BgHidanCurtain_TurnOff;
    }
}

void BgHidanCurtain_WaitForClear(BgHidanCurtain* this, PlayState* play) {
    if (Flags_GetClear(play, this->actor.room)) {
        this->actionFunc = BgHidanCurtain_TurnOff;
    }
}

void BgHidanCurtain_WaitForSwitchOff(BgHidanCurtain* this, PlayState* play) {
    if (!Flags_GetSwitch(play, this->actor.params)) {
        this->actionFunc = BgHidanCurtain_TurnOn;
    }
}

void BgHidanCurtain_TurnOn(BgHidanCurtain* this, PlayState* play) {
    f32 riseSpeed = sHCParams[this->size].riseSpeed;

    if (Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y, riseSpeed)) {
        Flags_UnsetSwitch(play, this->actor.params);
        this->actionFunc = BgHidanCurtain_WaitForSwitchOn;
    }
}

void BgHidanCurtain_TurnOff(BgHidanCurtain* this, PlayState* play) {
    BgHidanCurtainParams* hcParams = &sHCParams[this->size];

    if (Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y - hcParams->riseDist, hcParams->riseSpeed)) {
        if ((this->type == 0) || (this->type == 6)) {
            Actor_Kill(&this->actor);
        } else if (this->type == 5) {
            this->actionFunc = BgHidanCurtain_WaitForSwitchOff;
        } else {
            if (this->type == 2) {
                this->timer = 400;
            } else if (this->type == 4) {
                this->timer = 200;
            } else if (this->type == 3) {
                this->timer = 160;
            } else { // this->type == 1
                this->timer = 300;
            }
            this->actionFunc = BgHidanCurtain_WaitForTimer;
        }
    }
}

void BgHidanCurtain_WaitForTimer(BgHidanCurtain* this, PlayState* play) {
    DECR(this->timer);
    if (this->timer == 0) {
        this->actionFunc = BgHidanCurtain_TurnOn;
    }
    if ((this->type == 1) || (this->type == 3)) {
        func_8002F994(&this->actor, this->timer);
    }
}

void BgHidanCurtain_WaitForSlugCease(BgHidanCurtain* this, PlayState* play) {
    this->actor.world.pos.y = this->actor.home.pos.y;
}

void BgHidanCurtain_WaitForSlugPower(BgHidanCurtain* this, PlayState* play) {
    Actor* par = this->actor.parent;
    if (!par) {
        this->actionFunc = BgHidanCurtain_WaitForSlugCease;
        return;
    }
    this->actor.home.pos = par->world.pos;
    this->actor.world.pos.x = this->actor.home.pos.x;
    this->actor.world.pos.z = this->actor.home.pos.z;
    //EnBwActionFunc BWfunc;
    if ((this->actor.parent->id == ACTOR_EN_BW) && EnBw_Is_On_Fire(this->actor.parent)) {
        Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y, sHCParams[this->size].riseSpeed);
    } else {
        Math_StepToF(&this->actor.world.pos.y,this->actor.home.pos.y - sHCParams[this->size].riseDist,sHCParams[this->size].riseSpeed);
    }
    this->collider.dim.pos.x = this->actor.world.pos.x;
    this->collider.dim.pos.y = this->actor.world.pos.y;
    this->collider.dim.pos.z = this->actor.world.pos.z;
}

void BgHidanCurtain_WaitForDragonPower(BgHidanCurtain* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* par = this->actor.parent;
    //if (!par) {
    //    this->actionFunc = BgHidanCurtain_WaitForSlugCease;
    //    return;
    //}

    static Vec3f home = {0.0f, -100.0f, 0.0f};
    this->actor.home.pos = home;
    this->actor.world.pos.x = this->actor.home.pos.x;
    this->actor.world.pos.z = this->actor.home.pos.z;

   if (player->actor.world.pos.y > 0.0f && !((player->stateFlags1 & 0x2000) || (player->stateFlags1 & PLAYER_STATE1_CLIMBING_LADDER)))
        Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y+NORMAL_LIMIT, sHCParams[this->size].riseSpeed);
   else if (player->actor.world.pos.y < 0.0f)
       Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y+LOWER_LIMIT, sHCParams[this->size].riseSpeed);

    this->collider.dim.pos.x = this->actor.home.pos.x;
    this->collider.dim.pos.y = this->actor.home.pos.y;
    this->collider.dim.pos.z = this->actor.home.pos.z;

    if (player->actor.floorHeight < 0.0f && player->actor.world.pos.y > 90.0f && !((player->stateFlags1 & 0x2000) || (player->stateFlags1 & PLAYER_STATE1_CLIMBING_LADDER))) {
        this->collider.dim.radius = sHCParams[this->size].radius+150;
        this->collider.dim.height = sHCParams[this->size].height+100;
    } else {
        this->collider.dim.radius = sHCParams[this->size].radius;
        this->collider.dim.height = sHCParams[this->size].height;
    }
}

void BgHidanCurtain_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    Player* player = GET_PLAYER(play);
    BgHidanCurtain* this = (BgHidanCurtain*)thisx;
    BgHidanCurtainParams* hcParams = &sHCParams[this->size];
    f32 riseProgress;

    if ((play->cameraPtrs[MAIN_CAM]->setting == CAM_SET_SLOW_CHEST_CS) ||
        (play->cameraPtrs[MAIN_CAM]->setting == CAM_SET_TURN_AROUND)) {
        this->collider.base.atFlags &= ~AT_HIT;
    } else {
        if (this->collider.base.atFlags & AT_HIT) {
            this->collider.base.atFlags &= ~AT_HIT;
            func_8002F71C(play, &this->actor, 5.0f, this->actor.yawTowardsPlayer, 1.0f);
        }
        if ((this->type == 4) || (this->type == 5)) {
            this->actor.world.pos.y = (2.0f * this->actor.home.pos.y) - hcParams->riseDist - this->actor.world.pos.y;
        }

        this->actionFunc(this, play);

        if ((this->type == 4) || (this->type == 5)) {
            this->actor.world.pos.y = (2.0f * this->actor.home.pos.y) - hcParams->riseDist - this->actor.world.pos.y;
        }
        if (this->type != 9) {
            riseProgress = (hcParams->riseDist - (this->actor.home.pos.y - this->actor.world.pos.y)) / hcParams->riseDist;
        } else {
            if (player->actor.world.pos.y > 100.0f && !((player->stateFlags1 & 0x2000) || (player->stateFlags1 & PLAYER_STATE1_CLIMBING_LADDER)))
                riseProgress = 1;
            else
                riseProgress = (this->actor.world.pos.y - (this->actor.home.pos.y+LOWER_LIMIT)) / (NORMAL_LIMIT-LOWER_LIMIT);//1.0f;
        }
        this->alpha = 255.0f * riseProgress;
        if (this->alpha > 50) {
            if ((this->type != 9) || !(player->actor.floorHeight < 0.0f && player->actor.world.pos.y > 100.0f && !((player->stateFlags1 & 0x2000) || (player->stateFlags1 & PLAYER_STATE1_CLIMBING_LADDER))))
                this->collider.dim.height = hcParams->height * riseProgress;
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
            if (gSaveContext.sceneSetupIndex <= 3) {
                func_8002F974(&this->actor, NA_SE_EV_FIRE_PILLAR_S - SFX_FLAG);
            }
        } else if ((this->type == 1) && Flags_GetTreasure(play, this->treasureFlag)) {
            Actor_Kill(&this->actor);
        }
        this->texScroll++;
    }
}

void BgHidanCurtain_Draw(Actor* thisx, PlayState* play) {
    BgHidanCurtain* this = (BgHidanCurtain*)thisx;

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 220, 0, this->alpha);

    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, 0, this->texScroll & 0x7F, 0, 0x20, 0x40, 1, 0,
                                (this->texScroll * -0xF) & 0xFF, 0x20, 0x40));

    gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_XLU_DISP++, gEffFireCircleDL);

    CLOSE_DISPS(play->state.gfxCtx);
}
