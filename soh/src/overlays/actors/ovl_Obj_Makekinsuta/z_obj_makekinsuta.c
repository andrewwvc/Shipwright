/*
 * File: z_obj_makekinsuta.c
 * Overlay: ovl_Obj_Makekinsuta
 * Description: Skulltula Sprouting from Bean Spot
 */

#include "z_obj_makekinsuta.h"
#include "vt.h"
#include "src/overlays/actors/ovl_En_Goroiwa/z_en_goroiwa.h"

#define FLAGS ACTOR_FLAG_4

void ObjMakekinsuta_Init(Actor* thisx, PlayState* play);
void ObjMakekinsuta_Update(Actor* thisx, PlayState* play);

void ObjMakekinsuta_WaitForBomb(ObjMakekinsuta* this, PlayState* play);
void ObjMakekinsuta_WaitForBoulder(ObjMakekinsuta* this, PlayState* play);
void ObjMakekinsuta_WaitForHammer(ObjMakekinsuta* this, PlayState* play);
void ObjMakekinsuta_WaitForInsectSpecial(ObjMakekinsuta* this, PlayState* play);
void func_80B98320(ObjMakekinsuta* this, PlayState* play);
void ObjMakekinsuta_DoNothing(ObjMakekinsuta* this, PlayState* play);

const ActorInit Obj_Makekinsuta_InitVars = {
    ACTOR_OBJ_MAKEKINSUTA,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjMakekinsuta),
    (ActorFunc)ObjMakekinsuta_Init,
    (ActorFunc)Actor_Noop,
    (ActorFunc)ObjMakekinsuta_Update,
    NULL,
    NULL,
};

void ObjMakekinsuta_Init(Actor* thisx, PlayState* play) {
    ObjMakekinsuta* this = (ObjMakekinsuta*)thisx;

    if ((this->actor.params & 0x6000) == 0x4000) {
        osSyncPrintf(VT_FGCOL(BLUE));
        // "Gold Star Enemy(arg_data %x)"
        osSyncPrintf("金スタ発生敵(arg_data %x)\n", this->actor.params);
        osSyncPrintf(VT_RST);
    } else {
        osSyncPrintf(VT_COL(YELLOW, BLACK));
        // "Invalid Argument (arg_data %x)(%s %d)"
        osSyncPrintf("引数不正 (arg_data %x)(%s %d)\n", this->actor.params, __FILE__, __LINE__);
        osSyncPrintf(VT_RST);
    }
    this->unk_152 = 0;
    if (!this->actor.home.rot.z) {
        this->actionFunc = func_80B98320;
    } else if (this->actor.home.rot.z == 1) {
        this->actionFunc = ObjMakekinsuta_WaitForInsectSpecial;
    } else if (this->actor.home.rot.z == 2) {
        this->actionFunc = ObjMakekinsuta_WaitForHammer;
    } else if (this->actor.home.rot.z == 3) {
        this->actor.flags |= (ACTOR_FLAG_0 | ACTOR_FLAG_27);
        this->actionFunc = ObjMakekinsuta_WaitForBoulder;
    } else {
        this->actionFunc = ObjMakekinsuta_DoNothing;
    }

}
s32 ObjMakekinsuta_CanReceiveInsect(Actor* this) {
    return this->home.rot.z < 2;
}

void ObjMakekinsuta_WaitForBomb(ObjMakekinsuta* this, PlayState* play) {
    if (this->unk_152 != 0) {
        if (this->timer >= 60 && !func_8002DEEC(GET_PLAYER(play))) {
            if (!this->actor.home.rot.z) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_SW, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, this->actor.shape.rot.y, 0, (this->actor.params | 0x8000), false);
            } else {
            }
            this->actionFunc = ObjMakekinsuta_DoNothing;
        } else {
            this->timer++;
        }
    } else {
        this->timer = 0;
    }
}
u8 isHeartPiece(Actor* this, PlayState* play) {
    return this->params == ITEM00_HEART_PIECE;
}

void ObjMakekinsuta_WaitForBoulder(ObjMakekinsuta* this, PlayState* play) {
    Actor* boulder = Actor_FindNearby(play,&this->actor,ACTOR_EN_GOROIWA,ACTORCAT_PROP,50.0f);
    //Math3D_Dist2DSq(this->actor.world.pos.x,this->actor.world.pos.z,boulder->world.pos.x,boulder->world.pos.z);
    if (boulder && ABS(this->actor.world.pos.y - boulder->world.pos.y) < 50) {
        Actor_Spawn(&play->actorCtx,play, ACTOR_EN_ITEM00,this->actor.world.pos.x,this->actor.world.pos.y,this->actor.world.pos.z, 0,0,0, (this->actor.params<<8)+ITEM00_HEART_PIECE, false);
        func_8002F7DC(&this->actor,NA_SE_SY_CORRECT_CHIME);
        EnGoroiwa_Annihilate((EnGoroiwa*)boulder, play);
        this->actor.flags &= ~(ACTOR_FLAG_0 | ACTOR_FLAG_27);
        this->actionFunc = ObjMakekinsuta_DoNothing;
    }
}

void ObjMakekinsuta_WaitForHammer(ObjMakekinsuta* this, PlayState* play) {
    if (play->actorCtx.unk_02 != 0 && this->actor.xzDistToPlayer < 60.0f &&
               ABS(this->actor.yDistToPlayer) < 20.0f && (Actor_FindNumberOf(play,&this->actor, ACTOR_EN_ITEM00, ACTORCAT_MISC,1000.0f,NULL,isHeartPiece) == 0)) {
        Item_DropCollectible(play, &this->actor.world.pos, 0x100+ITEM00_HEART_PIECE);
        func_8002F7DC(&this->actor,NA_SE_SY_CORRECT_CHIME);
        this->actionFunc = ObjMakekinsuta_DoNothing;
    }
}

void ObjMakekinsuta_WaitForInsectSpecial(ObjMakekinsuta* this, PlayState* play) {
    if (this->unk_152 != 0) {
        if (!func_8002DEEC(GET_PLAYER(play))) {
            if (!(gSaveContext.infTable[3] & (1 << this->actor.params))){
                gSaveContext.infTable[3] |= (1 << this->actor.params);
                gSaveContext.infTable[1] &= ~0x200;//Change this later so that the variable is reset on returning to the child era
                this->actionFunc = ObjMakekinsuta_DoNothing;
            }
        }
    } else {
        this->timer = 0;
    }
}

void func_80B98320(ObjMakekinsuta* this, PlayState* play) {
    if (this->unk_152 != 0) {
        if (this->timer >= 60 && !func_8002DEEC(GET_PLAYER(play))) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_SW, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, this->actor.shape.rot.y, 0, (this->actor.params | 0x8000), true);
            this->actionFunc = ObjMakekinsuta_DoNothing;
        } else {
            this->timer++;
        }
    } else {
        this->timer = 0;
    }
}

void ObjMakekinsuta_DoNothing(ObjMakekinsuta* this, PlayState* play) {
}

void ObjMakekinsuta_Update(Actor* thisx, PlayState* play) {
    ObjMakekinsuta* this = (ObjMakekinsuta*)thisx;

    this->actionFunc(this, play);
}
