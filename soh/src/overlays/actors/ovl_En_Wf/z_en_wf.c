/*
 * File: z_en_wf.c
 * Overlay: ovl_En_Wf
 * Description: Wolfos (Normal and White)
 */

#include "z_en_wf.h"
#include "vt.h"
#include "overlays/actors/ovl_En_Encount1/z_en_encount1.h"
#include "objects/object_wf/object_wf.h"
#include "soh/Enhancements/game-interactor/GameInteractor_Hooks.h"

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_WHILE_CULLED)

void EnWf_Init(Actor* thisx, PlayState* play);
void EnWf_Destroy(Actor* thisx, PlayState* play);
void EnWf_Update(Actor* thisx, PlayState* play);
void EnWf_Draw(Actor* thisx, PlayState* play);

void EnWf_SetupWaitToAppear(EnWf* this);
void EnWf_WaitToAppear(EnWf* this, PlayState* play);
void EnWf_SetupWait(EnWf* this);
void EnWf_Wait(EnWf* this, PlayState* play);
void EnWf_SetupRunAtPlayer(EnWf* this, PlayState* play);
void EnWf_RunAtPlayer(EnWf* this, PlayState* play);
void EnWf_SetupSearchForPlayer(EnWf* this);
void EnWf_SearchForPlayer(EnWf* this, PlayState* play);
void EnWf_SetupRunAroundPlayer(EnWf* this);
void EnWf_RunAroundPlayer(EnWf* this, PlayState* play);
void EnWf_SetupSlash(EnWf* this);
void EnWf_SetupSingleSwipe(EnWf* this);
void EnWf_Slash(EnWf* this, PlayState* play);
void EnWf_RecoilFromBlockedSlash(EnWf* this, PlayState* play);
void EnWf_SetupBackflipAway(EnWf* this);
void EnWf_BackflipAway(EnWf* this, PlayState* play);
void EnWf_Stunned(EnWf* this, PlayState* play);
void EnWf_Damaged(EnWf* this, PlayState* play);
void EnWf_SetupSomersaultAndAttack(EnWf* this);
void EnWf_SomersaultAndAttack(EnWf* this, PlayState* play);
void EnWf_SetupBlocking(EnWf* this);
void EnWf_Blocking(EnWf* this, PlayState* play);
void EnWf_SetupSidestep(EnWf* this, PlayState* play);
void EnWf_Sidestep(EnWf* this, PlayState* play);
void EnWf_SetupDie(EnWf* this);
void EnWf_Die(EnWf* this, PlayState* play);
s32 EnWf_DodgeRanged(PlayState* play, EnWf* this);

static ColliderJntSphElementInit sJntSphItemsInit[4] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x10 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { WOLFOS_LIMB_FRONT_RIGHT_CLAW, { { 0, 0, 0 }, 15 }, 200 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x10 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { WOLFOS_LIMB_FRONT_LEFT_CLAW, { { 0, 0, 0 }, 15 }, 200 },
    },
    {
        {
            ELEMTYPE_UNK1,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFC1FFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_HOOKABLE,
            OCELEM_ON,
        },
        { WOLFOS_LIMB_HEAD, { { 800, 0, 0 }, 25 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK1,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFC1FFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_HOOKABLE,
            OCELEM_ON,
        },
        { WOLFOS_LIMB_THORAX, { { 0, 0, 0 }, 30 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphItemsInit),
    sJntSphItemsInit,
};

static ColliderCylinderInit sBodyCylinderInit = {
    {
        COLTYPE_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 20, 50, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sTailCylinderInit = {
    {
        COLTYPE_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 15, 20, -15, { 0, 0, 0 } },
};

typedef enum {
    /*  0 */ ENWF_DMGEFF_NONE,
    /*  1 */ ENWF_DMGEFF_STUN,
    /*  6 */ ENWF_DMGEFF_ICE_MAGIC = 6,
    /* 13 */ ENWF_DMGEFF_LIGHT_MAGIC = 13,
    /* 14 */ ENWF_DMGEFF_FIRE,
    /* 15 */ ENWF_DMGEFF_UNDEF // used like STUN in the code, but not in the table
} EnWfDamageEffect;

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, ENWF_DMGEFF_STUN),
    /* Deku stick    */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Slingshot     */ DMG_ENTRY(1, ENWF_DMGEFF_NONE),
    /* Explosive     */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Boomerang     */ DMG_ENTRY(0, ENWF_DMGEFF_STUN),
    /* Normal arrow  */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Hammer swing  */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Hookshot      */ DMG_ENTRY(0, ENWF_DMGEFF_STUN),
    /* Kokiri sword  */ DMG_ENTRY(1, ENWF_DMGEFF_NONE),
    /* Master sword  */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Giant's Knife */ DMG_ENTRY(4, ENWF_DMGEFF_NONE),
    /* Fire arrow    */ DMG_ENTRY(4, ENWF_DMGEFF_FIRE),
    /* Ice arrow     */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Light arrow   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Unk arrow 1   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(4, ENWF_DMGEFF_FIRE),
    /* Ice magic     */ DMG_ENTRY(0, ENWF_DMGEFF_ICE_MAGIC),
    /* Light magic   */ DMG_ENTRY(3, ENWF_DMGEFF_LIGHT_MAGIC),
    /* Shield        */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, ENWF_DMGEFF_NONE),
    /* Giant spin    */ DMG_ENTRY(4, ENWF_DMGEFF_NONE),
    /* Master spin   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Kokiri jump   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Giant jump    */ DMG_ENTRY(8, ENWF_DMGEFF_NONE),
    /* Master jump   */ DMG_ENTRY(4, ENWF_DMGEFF_NONE),
    /* Unknown 1     */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(4, ENWF_DMGEFF_NONE),
    /* Unknown 2     */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
};

static DamageTable sDamageTableWhite = {
    /* Deku nut      */ DMG_ENTRY(0, ENWF_DMGEFF_UNDEF),
    /* Deku stick    */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Slingshot     */ DMG_ENTRY(1, ENWF_DMGEFF_NONE),
    /* Explosive     */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Boomerang     */ DMG_ENTRY(0, ENWF_DMGEFF_UNDEF),
    /* Normal arrow  */ DMG_ENTRY(0, ENWF_DMGEFF_UNDEF),
    /* Hammer swing  */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Hookshot      */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Kokiri sword  */ DMG_ENTRY(1, ENWF_DMGEFF_NONE),
    /* Master sword  */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Giant's Knife */ DMG_ENTRY(4, ENWF_DMGEFF_NONE),
    /* Fire arrow    */ DMG_ENTRY(4, ENWF_DMGEFF_FIRE),
    /* Ice arrow     */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Light arrow   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Unk arrow 1   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(3, ENWF_DMGEFF_FIRE),
    /* Ice magic     */ DMG_ENTRY(0, ENWF_DMGEFF_ICE_MAGIC),
    /* Light magic   */ DMG_ENTRY(3, ENWF_DMGEFF_LIGHT_MAGIC),
    /* Shield        */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, ENWF_DMGEFF_NONE),
    /* Giant spin    */ DMG_ENTRY(4, ENWF_DMGEFF_NONE),
    /* Master spin   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Kokiri jump   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Giant jump    */ DMG_ENTRY(8, ENWF_DMGEFF_NONE),
    /* Master jump   */ DMG_ENTRY(4, ENWF_DMGEFF_NONE),
    /* Unknown 1     */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(4, ENWF_DMGEFF_NONE),
    /* Unknown 2     */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
};

s16 gWolfosWhiteSwipeAnimFrameData[853] = {
	0xffff, 0xfa4b, 0x0000, 0x0015, 0x0036, 0x007b, 0x005c, 0x005c, 0x005c, 0x005c, 0x005c, 0x005c, 0x005c, 0x005c, 0x005c, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0ab3, 0x0b66, 0x0c43, 0x0c06, 0x0c06, 0x0c06, 0x0c06, 0x0c06, 0x0c06, 0x0c06, 0x0c06, 0x0c06, 
	0x0a38, 0x0a38, 0x0a38, 0x0a38, 0x0a38, 0xfc43, 0xfb3d, 0xf956, 0xfa25, 0xfa25, 0xfa25, 0xfa25, 0xfa25, 0xfa25, 0xfa25, 0xfa25, 
	0xfa25, 0xfce9, 0xfce9, 0xfce9, 0xfce9, 0xfce9, 0x8db3, 0x922a, 0x9aac, 0x97cc, 0x97cc, 0x97cc, 0x97cc, 0x97cc, 0x97cc, 0x97cc, 
	0x97cc, 0x97cc, 0x8be5, 0x8be5, 0x8be5, 0x8be5, 0x8be5, 0xd1bb, 0xd1ee, 0xd2e6, 0xd2a4, 0xd2a4, 0xd2a4, 0xd2a4, 0xd2a4, 0xd2a4, 
	0xd2a4, 0xd2a4, 0xd2a4, 0xd1d0, 0xd1d0, 0xd1d0, 0xd1d0, 0xd1d0, 0xf12b, 0xeaf2, 0xdfde, 0xe3ad, 0xe3ad, 0xe3ad, 0xe3ad, 0xe3ad, 
	0xe3ad, 0xe3ad, 0xe3ad, 0xe3ad, 0xf434, 0xf434, 0xf434, 0xf434, 0xf434, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xffff, 0xffff, 0xffff, 0xff57, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xff00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5358, 0x4a8d, 0x3ad5, 0x40df, 0x40df, 
	0x40df, 0x40df, 0x40df, 0x40df, 0x40df, 0x40df, 0x441e, 0x5890, 0x5890, 0x5890, 0x5890, 0x5890, 0x0000, 0xffff, 0xffff, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xff48, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0167, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xcfcc, 0xd41e, 
	0xdbb2, 0xd8c6, 0xd8c6, 0xd8c6, 0xd8c6, 0xd8c6, 0xd8c6, 0xd8c6, 0xd8c6, 0xd47d, 0xcd22, 0xcd22, 0xcd22, 0xcd22, 0xcd22, 0x0019, 
	0x0081, 0x0176, 0x00f4, 0x00f4, 0x00f4, 0x00f4, 0x00f4, 0x00f4, 0x00f4, 0x00f4, 0x00f4, 0xff97, 0xff97, 0xff97, 0xff97, 0xff97, 
	0x000c, 0x0017, 0x0025, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0xd741, 0xd8aa, 0xdbe8, 0xda33, 0xda33, 0xda33, 0xda33, 0xda33, 0xda33, 0xda33, 0xda33, 0xda33, 0xd582, 0xd582, 0xd582, 
	0xd582, 0xd582, 0xc002, 0xc00c, 0xc031, 0xc01c, 0xc01c, 0xc01c, 0xc01c, 0xc01c, 0xc01c, 0xc01c, 0xc01c, 0xc01c, 0xbfff, 0xbfff, 
	0xbfff, 0xbfff, 0xbfff, 0x21dc, 0x255d, 0x2c68, 0x28ef, 0x28ef, 0x28ef, 0x28ef, 0x28ef, 0x28ef, 0x28ef, 0x28ef, 0x28ef, 0x1e96, 
	0x1e96, 0x1e96, 0x1e96, 0x1e96, 0xc003, 0xc00f, 0xc037, 0xc021, 0xc021, 0xc021, 0xc021, 0xc021, 0xc021, 0xc021, 0xc021, 0xc021, 
	0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xbfff, 0xc8e6, 0xc916, 0xc924, 0xc932, 0xc932, 0xc932, 0xc932, 0xc932, 0xc932, 0xc932, 0xc932, 
	0xc932, 0xc9a0, 0xc9a0, 0xc9a0, 0xc9a0, 0xc9a0, 0xda35, 0xdad3, 0xdbf2, 0xdb80, 0xdb80, 0xdb80, 0xdb80, 0xdb80, 0xdb80, 0xdb80, 
	0xdb80, 0xdb80, 0xd835, 0xd835, 0xd835, 0xd835, 0xd835, 0xb5b4, 0xb5bc, 0xb5b5, 0xb5d1, 0xb5d1, 0xb5d1, 0xb5d1, 0xb5d1, 0xb5d1, 
	0xb5d1, 0xb5d1, 0xb5d1, 0xb521, 0xb521, 0xb521, 0xb521, 0xb521, 0x5ac0, 0x5590, 0x4e47, 0x5080, 0x5080, 0x5080, 0x5080, 0x5080, 
	0x5080, 0x5080, 0x5080, 0x5080, 0x5ed7, 0x5ed7, 0x5ed7, 0x5ed7, 0x5ed7, 0xcc1d, 0xcec9, 0xd26f, 0xd153, 0xd153, 0xd153, 0xd153, 
	0xd153, 0xd153, 0xd153, 0xd153, 0xd153, 0xc9f1, 0xc9f1, 0xc9f1, 0xc9f1, 0xc9f1, 0x02db, 0x02dc, 0x02df, 0x02de, 0x02de, 0x02de, 
	0x02de, 0x02de, 0x02de, 0x02de, 0x02de, 0x02de, 0x02da, 0x02da, 0x02da, 0x02da, 0x02da, 0x000c, 0x0008, 0x0000, 0x0004, 0x0004, 
	0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0xc2ea, 0xc328, 0xc3aa, 0xc36e, 
	0xc36e, 0xc36e, 0xc36e, 0xc36e, 0xc36e, 0xc36e, 0xc36e, 0xc36e, 0xc2c4, 0xc2c4, 0xc2c4, 0xc2c4, 0xc2c4, 0x571f, 0x6cfe, 0x3c4a, 
	0x24df, 0x2db1, 0x2f11, 0x31ac, 0x3e3f, 0x4c46, 0x6a0e, 0x71f3, 0x7338, 0x6277, 0x6277, 0x6277, 0x6277, 0x6277, 0xc841, 0xd13a, 
	0xcf46, 0xcf95, 0xd7b5, 0xda8a, 0xd9a5, 0xd468, 0xd42b, 0xd888, 0xdbd1, 0xdb36, 0xc890, 0xc890, 0xc890, 0xc890, 0xc890, 0x2a0d, 
	0x1e12, 0x47db, 0x54d4, 0x541f, 0x55dd, 0x5403, 0x46ea, 0x3ae1, 0x2a7f, 0x242a, 0x220a, 0x1e65, 0x1e65, 0x1e65, 0x1e65, 0x1e65, 
	0xce18, 0xd24a, 0x32b3, 0xd145, 0xd062, 0xd062, 0xe67c, 0x2297, 0x5724, 0x7100, 0x817c, 0x8fe1, 0xb2ea, 0xb2ea, 0xb2ea, 0xb2ea, 
	0xb2ea, 0x118d, 0x0657, 0x10a4, 0x0fb5, 0x0d98, 0x0d98, 0x1530, 0x263e, 0x2cd6, 0x1daa, 0x186b, 0x13ac, 0x0706, 0x0706, 0x0706, 
	0x0706, 0x0706, 0x61c5, 0x8b72, 0xa97c, 0xa43b, 0xa60c, 0xa60c, 0xaab8, 0xb2bd, 0xe530, 0x0f0b, 0x13b2, 0x2121, 0x4a3d, 0x4a3d, 
	0x4a3d, 0x4a3d, 0x4a3d, 0x04d7, 0x0000, 0xee3b, 0x3fdd, 0x3e19, 0x3e2f, 0x3ecd, 0xe4a5, 0xe863, 0xeb88, 0xeb30, 0xeb30, 0x0000, 
	0x0000, 0xffff, 0x0000, 0x0000, 0x1767, 0x1b79, 0xfd54, 0x0295, 0x0113, 0xff90, 0x012f, 0xf73a, 0x036b, 0x1764, 0x0c25, 0x0c25, 
	0x17b9, 0x17b9, 0x17b9, 0x17b9, 0x17b9, 0x07c2, 0xffff, 0xed78, 0x03ae, 0xff47, 0xffcb, 0x026a, 0x03de, 0x0113, 0xfbfa, 0xfcfa, 
	0xfcfa, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0xf760, 0xf98b, 0xf924, 0x0265, 0x0265, 0x0265, 0x0ddb, 0x0c89, 0x3677, 0x1db5, 
	0x1db5, 0x1db5, 0xf9c0, 0xf9c0, 0xf9c0, 0xf9c0, 0xf9c0, 0x0052, 0x038f, 0x03b6, 0x0014, 0x0014, 0x0014, 0xf5d5, 0xfb97, 0x19de, 
	0x027b, 0x027b, 0x027b, 0x037b, 0x037b, 0x037b, 0x037b, 0x037b, 0x9bf2, 0xa853, 0xa4cb, 0xb7f8, 0xb7f8, 0xb7f8, 0xf137, 0xea21, 
	0xf513, 0xf500, 0xf500, 0xf500, 0xaa21, 0xaa21, 0xaa21, 0xaa21, 0xaa21, 0xfe85, 0xfd41, 0xf322, 0xfe27, 0xfe27, 0xfe27, 0xfe27, 
	0xfe27, 0xfd56, 0xfd93, 0xfcda, 0xfcda, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x05a3, 0x0e3e, 0xfca5, 0x05b0, 0x05b0, 0x05b0, 
	0x05b0, 0x05b0, 0x000c, 0x090c, 0x09d6, 0x09d6, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x18f2, 0x1742, 0x13e0, 0x19b0, 0x19b0, 
	0x19b0, 0x19b0, 0x19b0, 0x1a6f, 0x1939, 0x1907, 0x1907, 0x1a19, 0x1a19, 0x1a19, 0x1a19, 0x1a19, 0x4cb0, 0x4d22, 0x68c4, 0x468f, 
	0x468f, 0x468f, 0x468f, 0x468f, 0x468f, 0x4292, 0x4292, 0x41de, 0x3b00, 0x3b00, 0x3b00, 0x3b00, 0x3b00, 0xea49, 0xebbf, 0xf4cc, 
	0xdb9d, 0xdb9d, 0xdb9d, 0xdb9d, 0xdb9d, 0xdb9d, 0xed03, 0xed03, 0xe736, 0xed5c, 0xed5c, 0xed5c, 0xed5c, 0xed5c, 0x4b82, 0x39f0, 
	0x3ceb, 0x3de8, 0x3de8, 0x3de8, 0x3de8, 0x3de8, 0x3de8, 0x3f30, 0x3f30, 0x3e3a, 0x66ad, 0x66ad, 0x66ad, 0x66ad, 0x66ad, 0x0f27, 
	0x075e, 0x0c56, 0x0608, 0x0608, 0x0608, 0x0608, 0x0608, 0x0608, 0xfc37, 0xfc37, 0xfbc7, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 
	0xd2b6, 0xea2b, 0xdd97, 0xee9c, 0xee9c, 0xee9c, 0xee9c, 0xee9c, 0xee9c, 0xf8aa, 0xf8aa, 0xfa4e, 0xf2ab, 0xf2ab, 0xf2ab, 0xf2ab, 
	0xf2ab, 0x0016, 0xe91e, 0xf165, 0xdd53, 0xdd53, 0xdd53, 0xdd53, 0xdd53, 0xdd53, 0xc9c5, 0xc9c5, 0xc234, 0xffff, 0xffff, 0xffff, 
	0x0000, 0x0000, 0x0654, 0x05e7, 0x047d, 0x055c, 0x055c, 0x055c, 0x055c, 0x055c, 0x055c, 0x06c5, 0x06c5, 0x099c, 0x06d7, 0x06d7, 
	0x06d7, 0x06d7, 0x06d7, 0xfd74, 0xfd53, 0xfd27, 0xfd36, 0xfd36, 0xfd36, 0xfd36, 0xfd36, 0xfd36, 0xfeba, 0xfeba, 0xfe61, 0xfd8b, 
	0xfd8b, 0xfd8b, 0xfd8b, 0xfd8b, 0xa638, 0xa1ed, 0x9fb6, 0x9eb2, 0x9eb2, 0x9eb2, 0x9eb2, 0x9eb2, 0x9eb2, 0x98d4, 0x98d4, 0x8f77, 
	0xa862, 0xa862, 0xa862, 0xa862, 0xa862, };

JointIndex gWolfosWhiteSwipeAnimJointIndices[22] = {
	{ 0x0003, 0x0014, 0x0025, },
	{ 0x0000, 0x0001, 0x0002, },
	{ 0x0036, 0x0047, 0x0058, },
	{ 0x0069, 0x007a, 0x008b, },
	{ 0x009c, 0x00ad, 0x00be, },
	{ 0x00cf, 0x00e0, 0x00f1, },
	{ 0x0102, 0x0113, 0x0124, },
	{ 0x0002, 0x0000, 0x0000, },
	{ 0x0135, 0x0146, 0x0157, },
	{ 0x0000, 0x0002, 0x0168, },
	{ 0x0002, 0x0000, 0x0179, },
	{ 0x018a, 0x019b, 0x01ac, },
	{ 0x01bd, 0x01ce, 0x01df, },
	{ 0x01f0, 0x0201, 0x0212, },
	{ 0x0223, 0x0234, 0x0245, },
	{ 0x0256, 0x0267, 0x0278, },
	{ 0x0289, 0x029a, 0x02ab, },
	{ 0x0000, 0x0000, 0x0000, },
	{ 0x0000, 0x0000, 0x0000, },
	{ 0x02bc, 0x02cd, 0x02de, },
	{ 0x02ef, 0x0300, 0x0311, },
	{ 0x0322, 0x0333, 0x0344, },
};

AnimationHeader gWolfosWhiteSwipeAnim = { { 17 }, gWolfosWhiteSwipeAnimFrameData, gWolfosWhiteSwipeAnimJointIndices, 3 };

const ActorInit En_Wf_InitVars = {
    ACTOR_EN_WF,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_WF,
    sizeof(EnWf),
    (ActorFunc)EnWf_Init,
    (ActorFunc)EnWf_Destroy,
    (ActorFunc)EnWf_Update,
    (ActorFunc)EnWf_Draw,
    NULL,
};

#define MAX_WOLFOS_WHITE_HP 32

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -3000, ICHAIN_STOP),
};

void EnWf_SetupAction(EnWf* this, EnWfActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnWf_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnWf* this = (EnWf*)thisx;

    Actor_ProcessInitChain(thisx, sInitChain);

    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    thisx->focus.pos = thisx->world.pos;
    thisx->colChkInfo.mass = MASS_HEAVY;
    thisx->colChkInfo.health = 8;
    thisx->colChkInfo.cylRadius = 50;
    thisx->colChkInfo.cylHeight = 100;
    this->switchFlag = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;
    this->eyeIndex = 0;
    this->unk_2F4 = 10.0f; // Set and not used

    Collider_InitJntSph(play, &this->colliderSpheres);
    Collider_SetJntSph(play, &this->colliderSpheres, thisx, &sJntSphInit, this->colliderSpheresElements);
    Collider_InitCylinder(play, &this->colliderCylinderBody);
    Collider_SetCylinder(play, &this->colliderCylinderBody, thisx, &sBodyCylinderInit);
    Collider_InitCylinder(play, &this->colliderCylinderTail);
    Collider_SetCylinder(play, &this->colliderCylinderTail, thisx, &sTailCylinderInit);

    if (thisx->params == WOLFOS_NORMAL) {
        SkelAnime_InitFlex(play, &this->skelAnime, &gWolfosNormalSkel, &gWolfosWaitingAnim, this->jointTable,
                           this->morphTable, WOLFOS_LIMB_MAX);
        Actor_SetScale(thisx, 0.0075f);
        thisx->colChkInfo.damageTable = &sDamageTable;
        thisx->naviEnemyId = 0x4C; // Wolfos
    } else {                       // WOLFOS_WHITE
        SkelAnime_InitFlex(play, &this->skelAnime, &gWolfosWhiteSkel, &gWolfosWaitingAnim, this->jointTable,
                           this->morphTable, WOLFOS_LIMB_MAX);
        Actor_SetScale(thisx, 0.0125f);
        thisx->colChkInfo.damageTable = &sDamageTableWhite;
        thisx->colChkInfo.health = MAX_WOLFOS_WHITE_HP;
        this->colliderSpheres.elements[0].info.toucher.damage = this->colliderSpheres.elements[1].info.toucher.damage =
            0x10;
        thisx->naviEnemyId = 0x57; // White Wolfos
    }

    EnWf_SetupWaitToAppear(this);

    if ((this->switchFlag != 0xFF) && Flags_GetSwitch(play, this->switchFlag)) {
        Actor_Kill(thisx);
    }
}

void EnWf_Destroy(Actor* thisx, PlayState* play) {
    EnWf* this = (EnWf*)thisx;

    Collider_DestroyJntSph(play, &this->colliderSpheres);
    Collider_DestroyCylinder(play, &this->colliderCylinderBody);
    Collider_DestroyCylinder(play, &this->colliderCylinderTail);

    if ((this->actor.params != WOLFOS_NORMAL) && (this->switchFlag != 0xFF)) {
        func_800F5B58();
    }

    if (this->actor.parent != NULL) {
        EnEncount1* parent = (EnEncount1*)this->actor.parent;

        if (parent->actor.update != NULL) {
            if (parent->curNumSpawn > 0) {
                parent->curNumSpawn--;
            }

            osSyncPrintf("\n\n");
            // "☆☆☆☆☆ Number of concurrent events ☆☆☆☆☆"
            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 同時発生数 ☆☆☆☆☆%d\n" VT_RST, parent->curNumSpawn);
            osSyncPrintf("\n\n");
        }
    }

    ResourceMgr_UnregisterSkeleton(&this->skelAnime);
}

s32 EnWf_ChangeAction(PlayState* play, EnWf* this, s16 mustChoose) {
    Player* player = GET_PLAYER(play);
    s32 pad;
    s16 wallYawDiff;
    s16 playerYawDiff;
    Actor* explosive;

    wallYawDiff = this->actor.wallYaw - this->actor.shape.rot.y;
    wallYawDiff = ABS(wallYawDiff);
    playerYawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    playerYawDiff = ABS(playerYawDiff);

    if (func_800354B4(play, &this->actor, 100.0f, 0x2710, 0x2EE0, this->actor.shape.rot.y)) {
        if (player->meleeWeaponAnimation == 0x11) {
            EnWf_SetupBlocking(this);
            return true;
        }

        if ((play->gameplayFrames % 2) != 0) {
            EnWf_SetupBlocking(this);
            return true;
        }
    }
    
    if (isPlayerInSpinAttack(play) && (this->actor.xzDistToPlayer > 120.0f) && (this->actor.xzDistToPlayer < 150.0f)) {
        EnWf_SetupSomersaultAndAttack(this);
    }

    if (func_800354B4(play, &this->actor, 100.0f, 0x5DC0, 0x2AA8, this->actor.shape.rot.y)) {
        this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;

        if ((this->actor.bgCheckFlags & 8) && (ABS(wallYawDiff) < 0x2EE0) &&
                    (this->actor.xzDistToPlayer < 120.0f)) {
            EnWf_SetupSomersaultAndAttack(this);
            return true;
        } else if (player->meleeWeaponAnimation == 0x11) {
            EnWf_SetupBlocking(this);
            return true;
        } else if ((this->actor.xzDistToPlayer < 80.0f) && (play->gameplayFrames % 2) != 0) {
            EnWf_SetupBlocking(this);
            return true;
        } else {
            EnWf_SetupBackflipAway(this);
            return true;
        }
    }

    explosive = Actor_FindNearby(play, &this->actor, -1, ACTORCAT_EXPLOSIVE, 80.0f);

    if (explosive != NULL) {
        this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;

        if (((this->actor.bgCheckFlags & 8) && (wallYawDiff < 0x2EE0)) || (explosive->id == ACTOR_EN_BOM_CHU)) {
            if ((explosive->id == ACTOR_EN_BOM_CHU) && (Actor_WorldDistXYZToActor(&this->actor, explosive) < 80.0f) &&
                (s16)((this->actor.shape.rot.y - explosive->world.rot.y) + 0x8000) < 0x3E80) {
                EnWf_SetupSomersaultAndAttack(this);
                return true;
            } else {
                EnWf_SetupSidestep(this, play);
                return true;
            }
        } else {
            EnWf_SetupBackflipAway(this);
            return true;
        }
    }

    if (mustChoose) {
        s16 playerFacingAngleDiff;

        if (playerYawDiff >= 0x1B58) {
            EnWf_SetupSidestep(this, play);
            return true;
        }

        playerFacingAngleDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;

        if ((this->actor.xzDistToPlayer <= 80.0f) && !Actor_OtherIsTargetedPlaceholder(play, &this->actor) &&
            (((play->gameplayFrames % 8) != 0) || (ABS(playerFacingAngleDiff) < 0x38E0))) {
            EnWf_SetupSlash(this);
            return true;
        }

        EnWf_SetupRunAroundPlayer(this);
        return true;
    }
    return false;
}

void EnWf_SetupWaitToAppear(EnWf* this) {
    Animation_Change(&this->skelAnime, &gWolfosRearingUpFallingOverAnim, 0.5f, 0.0f, 7.0f, ANIMMODE_ONCE_INTERP, 0.0f);
    this->actor.world.pos.y = this->actor.home.pos.y - 5.0f;
    this->actionTimer = 20;
    this->unk_300 = false;
    this->action = WOLFOS_ACTION_WAIT_TO_APPEAR;
    this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;
    this->actor.scale.y = 0.0f;
    this->actor.gravity = 0.0f;
    EnWf_SetupAction(this, EnWf_WaitToAppear);
}

void EnWf_WaitToAppear(EnWf* this, PlayState* play) {
    if (this->actionTimer >= 6) {
        this->actor.world.pos.y = this->actor.home.pos.y - 5.0f;

        if (this->actor.xzDistToPlayer < 240.0f) {
            this->actionTimer = 5;
            this->actor.flags |= ACTOR_FLAG_TARGETABLE;

            // Disable miniboss music with Enemy Randomizer because the music would keep
            // playing if the enemy was never defeated, which is common with Enemy Randomizer.
            if ((this->actor.params != WOLFOS_NORMAL) && (this->switchFlag != 0xFF) && !CVarGetInteger("gRandomizedEnemies", 0)) {
                func_800F5ACC(NA_BGM_MINI_BOSS);
            }
        }
    } else if (this->actionTimer != 0) {
        this->actor.scale.y += this->actor.scale.x * 0.2f;
        this->actor.world.pos.y += 0.5f;
        Math_SmoothStepToF(&this->actor.shape.shadowScale, 70.0f, 1.0f, 14.0f, 0.0f);
        this->actionTimer--;

        if (this->actionTimer == 0) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_APPEAR);
        }
    } else { // actionTimer == 0
        if (SkelAnime_Update(&this->skelAnime)) {
            this->actor.scale.y = this->actor.scale.x;
            this->actor.gravity = -2.0f;
            EnWf_SetupWait(this);
        }
    }
}

void EnWf_SetupWait(EnWf* this) {
    Animation_MorphToLoop(&this->skelAnime, &gWolfosWaitingAnim, -4.0f);
    this->action = WOLFOS_ACTION_WAIT;
    this->actionTimer = (Rand_ZeroOne() * 10.0f) + 2.0f;
    this->actor.speedXZ = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnWf_SetupAction(this, EnWf_Wait);
}

void EnWf_Wait(EnWf* this, PlayState* play) {
    Player* player;
    s32 pad;
    s16 angle;

    player = GET_PLAYER(play);
    SkelAnime_Update(&this->skelAnime);

    if (this->unk_2E2 != 0) {
        angle = (this->actor.yawTowardsPlayer - this->actor.shape.rot.y) - this->unk_4D4.y;

        if (ABS(angle) > 0x2000) {
            this->unk_2E2--;
            return;
        }

        this->unk_2E2 = 0;
    }

    angle = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    angle = ABS(angle);

    if (!EnWf_DodgeRanged(play, this)) {
        // Only use of unk_2E0: never not zero, so this if block never runs
        if (this->unk_2E0 != 0) {
            this->unk_2E0--;

            if (angle >= 0x1FFE) {
                return;
            }
            this->unk_2E0 = 0;
        } else {
            if (EnWf_ChangeAction(play, this, false)) {
                return;
            }
        }

        angle = player->actor.shape.rot.y - this->actor.shape.rot.y;
        angle = ABS(angle);

        if ((this->actor.xzDistToPlayer < 80.0f) && (player->meleeWeaponState != 0) && (angle >= 0x1F40)) {
            this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
            EnWf_SetupRunAroundPlayer(this);
        } else {
            this->actionTimer--;

            if (this->actionTimer == 0) {
                if (Actor_IsFacingPlayer(&this->actor, 0x1555)) {
                    if (Rand_ZeroOne() > 0.3f) {
                        EnWf_SetupRunAtPlayer(this, play);
                    } else {
                        EnWf_SetupRunAroundPlayer(this);
                    }
                } else {
                    EnWf_SetupSearchForPlayer(this);
                }
                if ((play->gameplayFrames & 95) == 0) {
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_CRY);
                }
            }
        }
    }
}

void EnWf_SetupRunAtPlayer(EnWf* this, PlayState* play) {
    f32 lastFrame = Animation_GetLastFrame(&gWolfosRunningAnim);

    Animation_Change(&this->skelAnime, &gWolfosRunningAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP, -4.0f);
    this->action = WOLFOS_ACTION_RUN_AT_PLAYER;
    EnWf_SetupAction(this, EnWf_RunAtPlayer);
}

void EnWf_RunAtPlayer(EnWf* this, PlayState* play) {
    s32 animPrevFrame;
    s32 sp58;
    s32 pad;
    f32 baseRange = 0.0f;
    s16 playerFacingAngleDiff;
    Player* player = GET_PLAYER(play);
    s32 playSpeed;

    if (!EnWf_DodgeRanged(play, this)) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x2EE, 0);
        this->actor.world.rot.y = this->actor.shape.rot.y;

        if (Actor_OtherIsTargetedPlaceholder(play, &this->actor)) {
            baseRange = 150.0f;
        }

        if (this->actor.xzDistToPlayer <= (50.0f + baseRange)) {
            Math_SmoothStepToF(&this->actor.speedXZ, -8.0f, 1.0f, 1.5f, 0.0f);
        } else if ((65.0f + baseRange) < this->actor.xzDistToPlayer) {
            Math_SmoothStepToF(&this->actor.speedXZ, 8.0f, 1.0f, 1.5f, 0.0f);
        } else {
            Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 1.0f, 6.65f, 0.0f);
        }

        this->skelAnime.playSpeed = this->actor.speedXZ * 0.175f;
        playerFacingAngleDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;
        playerFacingAngleDiff = ABS(playerFacingAngleDiff);

        if ((this->actor.xzDistToPlayer < (150.0f + baseRange)) && (player->meleeWeaponState != 0) &&
            (playerFacingAngleDiff >= 8000)) {
            this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;

            if (Rand_ZeroOne() > 0.7f) {
                EnWf_SetupRunAroundPlayer(this);
                return;
            }
        }

        animPrevFrame = this->skelAnime.curFrame;
        SkelAnime_Update(&this->skelAnime);
        sp58 = this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed);
        playSpeed = (f32)ABS(this->skelAnime.playSpeed);

        if (!Actor_IsFacingPlayer(&this->actor, 0x11C7)) {
            if (Rand_ZeroOne() > 0.5f) {
                EnWf_SetupRunAroundPlayer(this);
            } else {
                EnWf_SetupWait(this);
            }
        } else if (this->actor.xzDistToPlayer < (90.0f + baseRange)) {
            s16 temp_v1 = player->actor.shape.rot.y - this->actor.shape.rot.y;

            if (!Actor_OtherIsTargetedPlaceholder(play, &this->actor) &&
                ((Rand_ZeroOne() > 0.03f) || ((this->actor.xzDistToPlayer <= 80.0f) && (ABS(temp_v1) < 0x38E0)))) {
                EnWf_SetupSlash(this);
            } else if (Actor_OtherIsTargetedPlaceholder(play, &this->actor) && (Rand_ZeroOne() > 0.5f)) {
                EnWf_SetupBackflipAway(this);
            } else {
                EnWf_SetupRunAroundPlayer(this);
            }
        }

        if (!EnWf_ChangeAction(play, this, false)) {
            if ((play->gameplayFrames & 95) == 0) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_CRY);
            }
            if ((animPrevFrame != (s32)this->skelAnime.curFrame) && (sp58 <= 0) && ((playSpeed + animPrevFrame) > 0)) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_WALK);
                Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, 20.0f, 3, 3.0f, 50, 50, true);
            }
        }
    }
}

void EnWf_SetupSearchForPlayer(EnWf* this) {
    Animation_MorphToLoop(&this->skelAnime, &gWolfosSidesteppingAnim, -4.0f);
    this->action = WOLFOS_ACTION_SEARCH_FOR_PLAYER;
    EnWf_SetupAction(this, EnWf_SearchForPlayer);
}

void EnWf_SearchForPlayer(EnWf* this, PlayState* play) {
    s16 yawDiff;
    s16 phi_v1;
    f32 phi_f2;

    if (!EnWf_DodgeRanged(play, this) && !EnWf_ChangeAction(play, this, false)) {
        yawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
        phi_v1 = (yawDiff > 0) ? (yawDiff * 0.25f) + 2000.0f : (yawDiff * 0.25f) - 2000.0f;
        this->actor.shape.rot.y += phi_v1;
        this->actor.world.rot.y = this->actor.shape.rot.y;

        if (yawDiff > 0) {
            phi_f2 = phi_v1 * 0.5f;
            phi_f2 = CLAMP_MAX(phi_f2, 1.0f);
        } else {
            phi_f2 = phi_v1 * 0.5f;
            phi_f2 = CLAMP_MIN(phi_f2, -1.0f);
        }

        this->skelAnime.playSpeed = -phi_f2;
        SkelAnime_Update(&this->skelAnime);

        if (Actor_IsFacingPlayer(&this->actor, 0x1555)) {
            if (Rand_ZeroOne() > 0.8f) {
                EnWf_SetupRunAroundPlayer(this);
            } else {
                EnWf_SetupRunAtPlayer(this, play);
            }
        }

        if ((play->gameplayFrames & 95) == 0) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_CRY);
        }
    }
}

void EnWf_SetupRunAroundPlayer(EnWf* this) {
    f32 lastFrame = Animation_GetLastFrame(&gWolfosRunningAnim);

    Animation_Change(&this->skelAnime, &gWolfosRunningAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP, -4.0f);

    if (Rand_ZeroOne() > 0.5f) {
        this->runAngle = 16000;
    } else {
        this->runAngle = -16000;
    }

    this->skelAnime.playSpeed = this->actor.speedXZ = 6.0f;
    this->skelAnime.playSpeed *= 0.175f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actionTimer = (Rand_ZeroOne() * 30.0f) + 30.0f;
    this->action = WOLFOS_ACTION_RUN_AROUND_PLAYER;
    this->runSpeed = 0.0f;

    EnWf_SetupAction(this, EnWf_RunAroundPlayer);
}

void EnWf_RunAroundPlayer(EnWf* this, PlayState* play) {
    s16 angle1;
    s16 angle2;
    s32 pad;
    f32 baseRange = 0.0f;
    s32 animPrevFrame;
    s32 animFrameSpeedDiff;
    s32 animSpeed;
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer + this->runAngle, 1, 4000, 1);

    if (!EnWf_DodgeRanged(play, this) && !EnWf_ChangeAction(play, this, false)) {
        this->actor.world.rot.y = this->actor.shape.rot.y;
        angle1 = player->actor.shape.rot.y + this->runAngle + 0x8000;

        // Actor_TestFloorInDirection is useless here (see comment below)
        if ((this->actor.bgCheckFlags & 8) ||
            !Actor_TestFloorInDirection(&this->actor, play, this->actor.speedXZ, this->actor.shape.rot.y)) {
            angle2 = (this->actor.bgCheckFlags & 8)
                         ? (this->actor.wallYaw - this->actor.yawTowardsPlayer) - this->runAngle
                         : 0;

            // This is probably meant to reverse direction if the edge of a floor is encountered, but does nothing
            // unless bgCheckFlags & 8 anyway, since angle2 = 0 otherwise
            if (ABS(angle2) > 0x2EE0) {
                this->runAngle = -this->runAngle;
            }
        }

        if (Actor_OtherIsTargetedPlaceholder(play, &this->actor)) {
            baseRange = 150.0f;
        }

        if (this->actor.xzDistToPlayer <= (60.0f + baseRange)) {
            Math_SmoothStepToF(&this->runSpeed, -4.0f, 1.0f, 1.5f, 0.0f);
        } else if ((80.0f + baseRange) < this->actor.xzDistToPlayer) {
            Math_SmoothStepToF(&this->runSpeed, 4.0f, 1.0f, 1.5f, 0.0f);
        } else {
            Math_SmoothStepToF(&this->runSpeed, 0.0f, 1.0f, 6.65f, 0.0f);
        }

        if (this->runSpeed != 0.0f) {
            this->actor.world.pos.x += Math_SinS(this->actor.shape.rot.y) * this->runSpeed;
            this->actor.world.pos.z += Math_CosS(this->actor.shape.rot.y) * this->runSpeed;
        }

        if (ABS(this->runSpeed) < ABS(this->actor.speedXZ)) {
            this->skelAnime.playSpeed = this->actor.speedXZ * 0.175f;
        } else {
            this->skelAnime.playSpeed = this->runSpeed * 0.175f;
        }

        this->skelAnime.playSpeed = CLAMP(this->skelAnime.playSpeed, -3.0f, 3.0f);
        animPrevFrame = this->skelAnime.curFrame;
        SkelAnime_Update(&this->skelAnime);
        animFrameSpeedDiff = this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed);
        animSpeed = (f32)ABS(this->skelAnime.playSpeed);

        if ((animPrevFrame != (s32)this->skelAnime.curFrame) && (animFrameSpeedDiff <= 0) &&
            (animSpeed + animPrevFrame > 0)) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_WALK);
            Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, 20.0f, 3, 3.0f, 50, 50, true);
        }

        if ((play->gameplayFrames & 95) == 0) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_CRY);
        }

        if ((Math_CosS(angle1 - this->actor.shape.rot.y) < -0.85f) && !Actor_OtherIsTargetedPlaceholder(play, &this->actor) &&
            (this->actor.xzDistToPlayer <= 80.0f)) {
            EnWf_SetupSlash(this);
        } else {
            this->actionTimer--;

            if (this->actionTimer == 0) {
                if (Actor_OtherIsTargetedPlaceholder(play, &this->actor) && (Rand_ZeroOne() > 0.5f)) {
                    EnWf_SetupBackflipAway(this);
                } else {
                    EnWf_SetupWait(this);
                    this->actionTimer = (Rand_ZeroOne() * 3.0f) + 1.0f;
                }
            }
        }
    }
}

void EnWf_SetupSlash(EnWf* this) {
    if ((this->actor.params != WOLFOS_NORMAL) && (!(this->actor.colChkInfo.health > 28) && ((this->actor.colChkInfo.health > 24) || (Rand_ZeroOne() > 0.5f)))) {
        EnWf_SetupSingleSwipe(this);
        return;
    }
    Animation_PlayOnce(&this->skelAnime, &gWolfosSlashingAnim);
    this->colliderSpheres.base.atFlags &= ~AT_BOUNCED;
    this->colliderSpheres.base.atFlags &= ~AT_HIT;
    this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
    this->action = WOLFOS_ACTION_SLASH;
    this->unk_2FA = 0; // Set and not used
    this->actionTimer = 7;
    this->skelAnime.endFrame = 20.0f;
    this->actor.speedXZ = 0.0f;

    EnWf_SetupAction(this, EnWf_Slash);
}

void EnWf_SetupSingleSwipe(EnWf* this) {
    Animation_PlayOnce(&this->skelAnime, &gWolfosWhiteSwipeAnim);
    this->colliderSpheres.base.atFlags &= ~AT_BOUNCED;
    this->colliderSpheres.base.atFlags &= ~AT_HIT;
    this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
    this->action = WOLFOS_ACTION_SLASH;
    this->unk_2FA = 0; // Set and not used
    this->actionTimer = 7;
    this->actor.speedXZ = 2.0f;

    EnWf_SetupAction(this, EnWf_Slash);
}

void EnWf_Slash(EnWf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 shapeAngleDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;
    s16 yawAngleDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    s32 curFrame = this->skelAnime.curFrame;

    shapeAngleDiff = ABS(shapeAngleDiff);
    yawAngleDiff = ABS(yawAngleDiff);
    this->actor.speedXZ = 0.0f;

    if (((curFrame >= 9) && (curFrame <= 12)) || ((curFrame >= 17) && (curFrame <= 19))) {
        if (this->slashStatus == 0) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_ATTACK);
        }

        this->slashStatus = 1;

        if ((curFrame >= 9) && (curFrame <= 12) && (this->actor.params == WOLFOS_WHITE) && (this->skelAnime.animation != &gWolfosWhiteSwipeAnim)) {
            if (player->stateFlags1 & PLAYER_STATE1_SHIELDING)
                this->colliderSpheres.base.atFlags &= ~AT_WEAK;
            else
                this->colliderSpheres.base.atFlags |= AT_WEAK;
            this->colliderSpheres.elements[0].info.toucher.dmgFlags = 0x00100000;
            this->colliderSpheres.elements[1].info.toucher.dmgFlags = 0x00100000;
        } else {
            this->colliderSpheres.base.atFlags &= ~AT_WEAK;
            this->colliderSpheres.elements[0].info.toucher.dmgFlags = 0xFFCFFFFF;
            this->colliderSpheres.elements[1].info.toucher.dmgFlags = 0xFFCFFFFF;
        }
    } else {
        this->slashStatus = 0;
    }

    if ((curFrame == 14) && (this->actor.params != WOLFOS_NORMAL)) {
        this->colliderSpheres.base.atFlags &= ~AT_BOUNCED;
        this->colliderSpheres.base.atFlags &= ~AT_HIT;
    }

    if (((curFrame == 15) && (LINK_IS_ADULT || (this->actor.params != WOLFOS_NORMAL)) &&
         /*!Actor_IsTargeted(play, &this->actor) &&*/
         (!Actor_IsFacingPlayer(&this->actor, 0x2000) || (this->actor.xzDistToPlayer >= 100.0f))) ||
        SkelAnime_Update(&this->skelAnime)) {
        if ((curFrame != 15) && (this->actionTimer != 0) && ((this->actor.params != WOLFOS_WHITE) || player->invincibilityTimer == 0)) {
            this->actor.shape.rot.y += (s16)(3276.0f * (1.5f + (this->actionTimer - 4) * 0.4f));
            Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, 15.0f, 1, 2.0f, 50, 50, true);
            this->actionTimer--;
        } else if (!Actor_IsFacingPlayer(&this->actor, 0x1554) && (curFrame != 15)) {
            EnWf_SetupWait(this);
            this->actionTimer = (Rand_ZeroOne() * 5.0f) + 5.0f;

            if (yawAngleDiff > 13000) {
                this->unk_2E2 = 7;
            }
        } else if ((Rand_ZeroOne() > 0.7f) || (this->actor.xzDistToPlayer >= 120.0f)) {
            EnWf_SetupWait(this);
            this->actionTimer = (Rand_ZeroOne() * 5.0f) + 5.0f;
        } else {
            this->actor.world.rot.y = this->actor.yawTowardsPlayer;

            if (Rand_ZeroOne() > 0.7f) {
                EnWf_SetupSidestep(this, play);
            } else if (shapeAngleDiff <= 10000) {
                if (yawAngleDiff > 16000) {
                    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
                    EnWf_SetupRunAroundPlayer(this);
                } else {
                    EnWf_ChangeAction(play, this, true);
                }
            } else {
                EnWf_SetupRunAroundPlayer(this);
            }
        }
    }
}

void EnWf_SetupRecoilFromBlockedSlash(EnWf* this) {
    f32 endFrame = 1.0f;

    if ((s32)this->skelAnime.curFrame >= 16) {
        endFrame = 15.0f;
    }

    Animation_Change(&this->skelAnime, &gWolfosSlashingAnim, -0.5f, this->skelAnime.curFrame - 1.0f, endFrame,
                     ANIMMODE_ONCE_INTERP, 0.0f);
    this->action = WOLFOS_ACTION_RECOIL_FROM_BLOCKED_SLASH;
    this->slashStatus = 0;
    EnWf_SetupAction(this, EnWf_RecoilFromBlockedSlash);
}

void EnWf_RecoilFromBlockedSlash(EnWf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 angle1 = player->actor.shape.rot.y - this->actor.shape.rot.y;
    s16 angle2 = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

    angle1 = ABS(angle1);
    angle2 = ABS(angle2);

    if (SkelAnime_Update(&this->skelAnime)) {
        if (!Actor_IsFacingPlayer(&this->actor, 0x1554)) {
            EnWf_SetupWait(this);
            this->actionTimer = (Rand_ZeroOne() * 5.0f) + 5.0f;

            if (angle2 > 0x32C8) {
                this->unk_2E2 = 30;
            }
        } else {
            if ((Rand_ZeroOne() > 0.7f) || (this->actor.xzDistToPlayer >= 120.0f)) {
                EnWf_SetupWait(this);
                this->actionTimer = (Rand_ZeroOne() * 5.0f) + 5.0f;
            } else {
                this->actor.world.rot.y = this->actor.yawTowardsPlayer;

                if (Rand_ZeroOne() > 0.7f) {
                    EnWf_SetupSidestep(this, play);
                } else if (angle1 <= 0x2710) {
                    if (angle2 > 0x3E80) {
                        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
                        EnWf_SetupRunAroundPlayer(this);
                    } else {
                        EnWf_ChangeAction(play, this, true);
                    }
                } else {
                    EnWf_SetupRunAroundPlayer(this);
                }
            }
        }
    }
}

void EnWf_SetupBackflipAway(EnWf* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gWolfosBackflippingAnim, -3.0f);
    this->actor.speedXZ = -6.0f;
    this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    this->actionTimer = 0;
    this->unk_300 = true;
    this->action = WOLFOS_ACTION_BACKFLIP_AWAY;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_JUMP);
    EnWf_SetupAction(this, EnWf_BackflipAway);
}

void EnWf_BackflipAway(EnWf* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        if (!Actor_OtherIsTargetedPlaceholder(play, &this->actor) && (this->actor.xzDistToPlayer < 170.0f) &&
            (this->actor.xzDistToPlayer > 140.0f) && (Rand_ZeroOne() < 0.2f)) {
            EnWf_SetupRunAtPlayer(this, play);
        } else if ((play->gameplayFrames % 2) != 0) {
            EnWf_SetupSidestep(this, play);
        } else {
            EnWf_SetupWait(this);
        }
    }
    if ((play->state.frames & 95) == 0) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_CRY);
    }
}

void EnWf_SetupStunned(EnWf* this) {
    if (this->actor.bgCheckFlags & 1) {
        this->actor.speedXZ = 0.0f;
    }

    Audio_PlayActorSound2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
    Animation_PlayOnceSetSpeed(&this->skelAnime, &gWolfosDamagedAnim, 0.0f);
    this->action = WOLFOS_ACTION_STUNNED;
    EnWf_SetupAction(this, EnWf_Stunned);
}

void EnWf_Stunned(EnWf* this, PlayState* play) {
    if (this->actor.bgCheckFlags & 2) {
        this->actor.speedXZ = 0.0f;
    }

    if (this->actor.bgCheckFlags & 1) {
        if (this->actor.speedXZ < 0.0f) {
            this->actor.speedXZ += 0.05f;
        }

        this->unk_300 = false;
    }

    if ((this->actor.colorFilterTimer == 0) && (this->actor.bgCheckFlags & 1)) {
        if (this->actor.colChkInfo.health == 0) {
            EnWf_SetupDie(this);
        } else {
            EnWf_ChangeAction(play, this, true);
        }
    }
}

void EnWf_SetupDamaged(EnWf* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gWolfosDamagedAnim, -4.0f);

    if (this->actor.bgCheckFlags & 1) {
        this->unk_300 = false;
        this->actor.speedXZ = -4.0f;
    } else {
        this->unk_300 = true;
    }

    this->unk_2E2 = 0;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_DAMAGE);
    this->action = WOLFOS_ACTION_DAMAGED;
    EnWf_SetupAction(this, EnWf_Damaged);
}

void EnWf_Damaged(EnWf* this, PlayState* play) {
    s16 angleToWall;

    if (this->actor.bgCheckFlags & 2) {
        this->actor.speedXZ = 0.0f;
    }

    if (this->actor.bgCheckFlags & 1) {
        if (this->actor.speedXZ < 0.0f) {
            this->actor.speedXZ += 0.05f;
        }

        this->unk_300 = false;
    }

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 4500, 0);

    if (!EnWf_ChangeAction(play, this, false) && SkelAnime_Update(&this->skelAnime)) {
        if (this->actor.bgCheckFlags & 1) {
            angleToWall = this->actor.wallYaw - this->actor.shape.rot.y;
            angleToWall = ABS(angleToWall);

            if ((this->actor.bgCheckFlags & 8) && (ABS(angleToWall) < 12000) && (this->actor.xzDistToPlayer < 120.0f)) {
                EnWf_SetupSomersaultAndAttack(this);
            } else if (!EnWf_DodgeRanged(play, this)) {
                if ((this->actor.xzDistToPlayer <= 80.0f) && !Actor_OtherIsTargetedPlaceholder(play, &this->actor) &&
                    ((play->gameplayFrames % 8) != 0)) {
                    EnWf_SetupSlash(this);
                } else if (Rand_ZeroOne() > 0.5f) {
                    EnWf_SetupWait(this);
                    this->actionTimer = (Rand_ZeroOne() * 5.0f) + 5.0f;
                    this->unk_2E2 = 30;
                } else {
                    EnWf_SetupBackflipAway(this);
                }
            }
        }
    }
}

void EnWf_SetupSomersaultAndAttack(EnWf* this) {
    f32 lastFrame = Animation_GetLastFrame(&gWolfosBackflippingAnim);

    Animation_Change(&this->skelAnime, &gWolfosBackflippingAnim, -1.0f, lastFrame, 0.0f, ANIMMODE_ONCE, -3.0f);
    this->actionTimer = 0;
    this->unk_300 = false;
    this->action = WOLFOS_ACTION_TURN_TOWARDS_PLAYER;
    this->actor.speedXZ = 6.5f;
    this->actor.velocity.y = 15.0f;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_JUMP);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnWf_SetupAction(this, EnWf_SomersaultAndAttack);
}

void EnWf_SomersaultAndAttack(EnWf* this, PlayState* play) {
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 4000, 1);

    if (this->actor.velocity.y >= 5.0f) {
        //! @bug unk_4C8 and unk_4BC are used but not set (presumably intended to be feet positions like other actors)
        func_800355B8(play, &this->unk_4C8);
        func_800355B8(play, &this->unk_4BC);
    }

    if (SkelAnime_Update(&this->skelAnime) && (this->actor.bgCheckFlags & (1 | 2))) {
        this->actor.world.rot.y = this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
        this->actor.shape.rot.x = 0;
        this->actor.speedXZ = this->actor.velocity.y = 0.0f;
        this->actor.world.pos.y = this->actor.floorHeight;

        if (!Actor_OtherIsTargetedPlaceholder(play, &this->actor)) {
            EnWf_SetupSlash(this);
        } else {
            EnWf_SetupWait(this);
        }
    }
}

void EnWf_SetupBlocking(EnWf* this) {
    f32 lastFrame = Animation_GetLastFrame(&gWolfosBlockingAnim);

    if (this->slashStatus != 0) {
        this->slashStatus = -1;
    }

    this->actor.speedXZ = 0.0f;
    this->action = WOLFOS_ACTION_BLOCKING;
    this->actionTimer = 10;

    Animation_Change(&this->skelAnime, &gWolfosBlockingAnim, 0.0f, 0.0f, lastFrame, ANIMMODE_ONCE_INTERP, -4.0f);
    EnWf_SetupAction(this, EnWf_Blocking);
}

void EnWf_Blocking(EnWf* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 pad;

    if (this->actionTimer != 0) {
        this->actionTimer--;
    } else {
        this->skelAnime.playSpeed = 1.0f;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        s16 yawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

        if ((ABS(yawDiff) <= 0x4000) && (this->actor.xzDistToPlayer < 60.0f) &&
            (ABS(this->actor.yDistToPlayer) < 50.0f)) {
            if (func_800354B4(play, &this->actor, 100.0f, 10000, 0x4000, this->actor.shape.rot.y)) {
                if (player->meleeWeaponAnimation == 0x11) {
                    EnWf_SetupBlocking(this);
                } else if ((play->gameplayFrames % 2) != 0) {
                    EnWf_SetupBlocking(this);
                } else {
                    EnWf_SetupBackflipAway(this);
                }

            } else {
                s16 angleFacingLink = player->actor.shape.rot.y - this->actor.shape.rot.y;

                if (!Actor_OtherIsTargetedPlaceholder(play, &this->actor) &&
                    (((play->gameplayFrames % 2) != 0) || (ABS(angleFacingLink) < 0x38E0))) {
                    EnWf_SetupSlash(this);
                } else {
                    EnWf_SetupRunAroundPlayer(this);
                }
            }
        } else {
            EnWf_SetupRunAroundPlayer(this);
        }
    } else if (this->actionTimer == 0) {
        if (func_800354B4(play, &this->actor, 100.0f, 10000, 0x4000, this->actor.shape.rot.y)) {
            if (player->meleeWeaponAnimation == 0x11) {
                EnWf_SetupBlocking(this);
            } else if ((play->gameplayFrames % 2) != 0) {
                EnWf_SetupBlocking(this);
            } else {
                EnWf_SetupBackflipAway(this);
            }
        }
    }
}

void EnWf_SetupSidestep(EnWf* this, PlayState* play) {
    s16 angle;
    Player* player;
    f32 lastFrame = Animation_GetLastFrame(&gWolfosRunningAnim);

    Animation_Change(&this->skelAnime, &gWolfosRunningAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP, -4.0f);

    player = GET_PLAYER(play);
    angle = player->actor.shape.rot.y + this->runAngle;

    if (Math_SinS(angle - this->actor.yawTowardsPlayer) > 0.0f) {
        this->runAngle = 16000;
    } else if (Math_SinS(angle - this->actor.yawTowardsPlayer) < 0.0f) {
        this->runAngle = -16000;
    } else if (Rand_ZeroOne() > 0.5f) {
        this->runAngle = 16000;
    } else {
        this->runAngle = -16000;
    }

    this->skelAnime.playSpeed = this->actor.speedXZ = 6.0f;
    this->skelAnime.playSpeed *= 0.175f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->runSpeed = 0.0f;
    this->actionTimer = (Rand_ZeroOne() * 10.0f) + 5.0f;
    this->action = WOLFOS_ACTION_SIDESTEP;

    EnWf_SetupAction(this, EnWf_Sidestep);
}

void EnWf_Sidestep(EnWf* this, PlayState* play) {
    s16 angleDiff1;
    Player* player = GET_PLAYER(play);
    s32 animPrevFrame;
    s32 animFrameSpeedDiff;
    s32 animSpeed;
    f32 baseRange = 0.0f;

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer + this->runAngle, 1, 3000, 1);

    // Actor_TestFloorInDirection is useless here (see comment below)
    if ((this->actor.bgCheckFlags & 8) ||
        !Actor_TestFloorInDirection(&this->actor, play, this->actor.speedXZ, this->actor.shape.rot.y)) {
        s16 angle =
            (this->actor.bgCheckFlags & 8) ? (this->actor.wallYaw - this->actor.yawTowardsPlayer) - this->runAngle : 0;

        // This is probably meant to reverse direction if the edge of a floor is encountered, but does nothing
        // unless bgCheckFlags & 8 anyway, since angle = 0 otherwise
        if (ABS(angle) > 0x2EE0) {
            this->runAngle = -this->runAngle;
        }
    }

    this->actor.world.rot.y = this->actor.shape.rot.y;

    if (Actor_OtherIsTargetedPlaceholder(play, &this->actor)) {
        baseRange = 150.0f;
    }

    if (this->actor.xzDistToPlayer <= (60.0f + baseRange)) {
        Math_SmoothStepToF(&this->runSpeed, -4.0f, 1.0f, 1.5f, 0.0f);
    } else if ((80.0f + baseRange) < this->actor.xzDistToPlayer) {
        Math_SmoothStepToF(&this->runSpeed, 4.0f, 1.0f, 1.5f, 0.0f);
    } else {
        Math_SmoothStepToF(&this->runSpeed, 0.0f, 1.0f, 6.65f, 0.0f);
    }

    if (this->runSpeed != 0.0f) {
        this->actor.world.pos.x += Math_SinS(this->actor.shape.rot.y) * this->runSpeed;
        this->actor.world.pos.z += Math_CosS(this->actor.shape.rot.y) * this->runSpeed;
    }

    if (ABS(this->runSpeed) < ABS(this->actor.speedXZ)) {
        this->skelAnime.playSpeed = this->actor.speedXZ * 0.175f;
    } else {
        this->skelAnime.playSpeed = this->runSpeed * 0.175f;
    }

    this->skelAnime.playSpeed = CLAMP(this->skelAnime.playSpeed, -3.0f, 3.0f);

    animPrevFrame = this->skelAnime.curFrame;
    SkelAnime_Update(&this->skelAnime);
    animFrameSpeedDiff = this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed);
    animSpeed = (f32)ABS(this->skelAnime.playSpeed);

    if (!EnWf_ChangeAction(play, this, false)) {
        this->actionTimer--;

        if (this->actionTimer == 0) {
            angleDiff1 = player->actor.shape.rot.y - this->actor.yawTowardsPlayer;
            angleDiff1 = ABS(angleDiff1);

            if (angleDiff1 >= 0x3A98) {
                EnWf_SetupWait(this);
                this->actionTimer = (Rand_ZeroOne() * 3.0f) + 1.0f;
            } else {
                Player* player2 = GET_PLAYER(play);
                s16 angleDiff2 = player2->actor.shape.rot.y - this->actor.yawTowardsPlayer;

                this->actor.world.rot.y = this->actor.shape.rot.y;

                if ((this->actor.xzDistToPlayer <= 80.0f) && !Actor_OtherIsTargetedPlaceholder(play, &this->actor) &&
                    (((play->gameplayFrames % 4) == 0) || (ABS(angleDiff2) < 0x38E0))) {
                    EnWf_SetupSlash(this);
                } else {
                    EnWf_SetupRunAtPlayer(this, play);
                }
            }
        }

        if ((animPrevFrame != (s32)this->skelAnime.curFrame) && (animFrameSpeedDiff <= 0) &&
            ((animSpeed + animPrevFrame) > 0)) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_WALK);
            Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, 20.0f, 3, 3.0f, 50, 50, true);
        }

        if ((play->gameplayFrames & 95) == 0) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_CRY);
        }
    }
}

void EnWf_SetupDie(EnWf* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gWolfosRearingUpFallingOverAnim, -4.0f);
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;

    if (this->actor.bgCheckFlags & 1) {
        this->unk_300 = false;
        this->actor.speedXZ = -6.0f;
    } else {
        this->unk_300 = true;
    }

    this->action = WOLFOS_ACTION_DIE;
    this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;
    this->actionTimer = this->skelAnime.animLength;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_WOLFOS_DEAD);
    EnWf_SetupAction(this, EnWf_Die);
    GameInteractor_ExecuteOnEnemyDefeat(&this->actor);
}

void EnWf_Die(EnWf* this, PlayState* play) {
    if (this->actor.bgCheckFlags & 2) {
        this->actor.speedXZ = 0.0f;
    }

    if (this->actor.bgCheckFlags & 1) {
        Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
        this->unk_300 = false;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0xD0);

        if (this->switchFlag != 0xFF) {
            Flags_SetSwitch(play, this->switchFlag);
        }

        Actor_Kill(&this->actor);
    } else {
        s32 i;
        Vec3f pos;
        Vec3f velAndAccel = { 0.0f, 0.5f, 0.0f };

        this->actionTimer--;

        for (i = ((s32)this->skelAnime.animLength - this->actionTimer) >> 1; i >= 0; i--) {
            pos.x = Rand_CenteredFloat(60.0f) + this->actor.world.pos.x;
            pos.z = Rand_CenteredFloat(60.0f) + this->actor.world.pos.z;
            pos.y = Rand_CenteredFloat(50.0f) + (this->actor.world.pos.y + 20.0f);
            EffectSsDeadDb_Spawn(play, &pos, &velAndAccel, &velAndAccel, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1,
                                 9, true);
        }
    }
}

void func_80B36F40(EnWf* this, PlayState* play) {
    if ((this->action == WOLFOS_ACTION_WAIT) && (this->unk_2E2 != 0)) {
        this->unk_4D4.y = Math_SinS(this->unk_2E2 * 4200) * 8920.0f;
    } else if (this->action != WOLFOS_ACTION_STUNNED) {
        if (this->action != WOLFOS_ACTION_SLASH) {
            Math_SmoothStepToS(&this->unk_4D4.y, this->actor.yawTowardsPlayer - this->actor.shape.rot.y, 1, 1500, 0);
            this->unk_4D4.y = CLAMP(this->unk_4D4.y, -0x3127, 0x3127);
        } else {
            this->unk_4D4.y = 0;
        }
    }
}

void EnWf_UpdateDamage(EnWf* this, PlayState* play) {
    if (this->colliderSpheres.base.acFlags & AC_BOUNCED) {
        this->colliderSpheres.base.acFlags &= ~(AC_HIT | AC_BOUNCED);
        this->colliderCylinderBody.base.acFlags &= ~AC_HIT;
        this->colliderCylinderTail.base.acFlags &= ~AC_HIT;
    } else if ((this->colliderCylinderBody.base.acFlags & AC_HIT) ||
               (this->colliderCylinderTail.base.acFlags & AC_HIT)) {
        if (this->action >= WOLFOS_ACTION_WAIT) {
            s16 yawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

            if ((!(this->colliderCylinderBody.base.acFlags & AC_HIT) &&
                 (this->colliderCylinderTail.base.acFlags & AC_HIT)) ||
                ((ABS(yawDiff) > 19000) &&
                    !((this->colliderCylinderBody.base.acFlags & AC_HIT) &&
                      (this->colliderCylinderTail.base.acFlags & AC_HIT)))) {
                this->actor.colChkInfo.damage *= 2;
            }

            this->colliderCylinderBody.base.acFlags &= ~AC_HIT;
            this->colliderCylinderTail.base.acFlags &= ~AC_HIT;

            if (this->actor.colChkInfo.damageEffect != ENWF_DMGEFF_ICE_MAGIC) {
                this->damageEffect = this->actor.colChkInfo.damageEffect;
                Actor_SetDropFlag(&this->actor, &this->colliderCylinderBody.info, 1);
                this->slashStatus = 0;

                if ((this->actor.colChkInfo.damageEffect == ENWF_DMGEFF_STUN)) {
                    if (this->action != WOLFOS_ACTION_STUNNED) {
                        Actor_SetColorFilter(&this->actor, 0, 120, 0, 80);
                        Actor_ApplyDamage(&this->actor);
                        EnWf_SetupStunned(this);
                    }
                } else if (this->actor.colChkInfo.damageEffect == ENWF_DMGEFF_UNDEF) {
                    //Do nothing
                } else { // LIGHT_MAGIC, FIRE, NONE
                    Actor_SetColorFilter(&this->actor, 0x4000, 255, 0, 8);

                    if (this->damageEffect == ENWF_DMGEFF_FIRE) {
                        this->fireTimer = 40;
                    }

                    if (Actor_ApplyDamage(&this->actor) == 0) {
                        EnWf_SetupDie(this);
                        Enemy_StartFinishingBlow(play, &this->actor);
                    } else {
                        EnWf_SetupDamaged(this);
                    }
                }
            }
        }
    }
}

void EnWf_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    EnWf* this = (EnWf*)thisx;

    EnWf_UpdateDamage(this, play);

    if (this->actor.colChkInfo.damageEffect != ENWF_DMGEFF_ICE_MAGIC) {
        Actor_MoveForward(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 32.0f, 30.0f, 60.0f, 0x1D);
        this->actionFunc(this, play);
        func_80B36F40(this, play);
    }

    if (this->actor.bgCheckFlags & (1 | 2)) {
        func_800359B8(&this->actor, this->actor.shape.rot.y, &this->actor.shape.rot);
    } else {
        Math_SmoothStepToS(&this->actor.shape.rot.x, 0, 1, 1000, 0);
        Math_SmoothStepToS(&this->actor.shape.rot.z, 0, 1, 1000, 0);
    }

    CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderSpheres.base);

    if (this->action >= WOLFOS_ACTION_WAIT && !((this->actionFunc == EnWf_Slash) && (this->actor.params == WOLFOS_WHITE))) {
        if ((this->actor.colorFilterTimer == 0) || !(this->actor.colorFilterParams & 0x4000)) {
            Collider_UpdateCylinder(&this->actor, &this->colliderCylinderBody);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderCylinderTail.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderCylinderBody.base);
        }
    }

    if (this->action == WOLFOS_ACTION_BLOCKING || ((this->actionFunc == EnWf_Slash) && (this->actor.params == WOLFOS_WHITE))) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderSpheres.base);
    }

    if ((this->actor.params == WOLFOS_WHITE) && (this->colliderSpheres.base.atFlags & AC_HIT)) {
        this->colliderSpheres.base.atFlags &= ~AC_HIT;
        Player_SetShieldRecoveryDefault(play);
    }

    if (this->slashStatus > 0) {
        if (!(this->colliderSpheres.base.atFlags & AT_BOUNCED)) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderSpheres.base);
        } else {
            if (this->actor.params == WOLFOS_NORMAL)
                EnWf_SetupRecoilFromBlockedSlash(this);
        }
    } else if (this->actionFunc == EnWf_SomersaultAndAttack)
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderSpheres.base);

    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += 25.0f;

    if (this->eyeIndex == 0) {
        if ((Rand_ZeroOne() < 0.2f) && ((play->gameplayFrames % 4) == 0) && (this->actor.colorFilterTimer == 0)) {
            this->eyeIndex++;
        }
    } else {
        this->eyeIndex = (this->eyeIndex + 1) & 3;
    }
}

s32 EnWf_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnWf* this = (EnWf*)thisx;

    if ((limbIndex == WOLFOS_LIMB_HEAD) || (limbIndex == WOLFOS_LIMB_EYES)) {
        rot->y -= this->unk_4D4.y;
    }

    return false;
}

void EnWf_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f colliderVec = { 1200.0f, 0.0f, 0.0f };
    static Vec3f bodyPartVec = { 0.0f, 0.0f, 0.0f };
    EnWf* this = (EnWf*)thisx;
    s32 bodyPartIndex = -1;

    Collider_UpdateSpheres(limbIndex, &this->colliderSpheres);

    if (limbIndex == WOLFOS_LIMB_TAIL) {
        Vec3f colliderPos;

        bodyPartIndex = -1;
        Matrix_MultVec3f(&colliderVec, &colliderPos);
        this->colliderCylinderTail.dim.pos.x = colliderPos.x;
        this->colliderCylinderTail.dim.pos.y = colliderPos.y;
        this->colliderCylinderTail.dim.pos.z = colliderPos.z;
    }

    if ((this->fireTimer != 0) || ((this->actor.colorFilterTimer != 0) && (this->actor.colorFilterParams & 0x4000))) {
        switch (limbIndex) {
            case WOLFOS_LIMB_EYES:
                bodyPartIndex = 0;
                break;
            case WOLFOS_LIMB_FRONT_RIGHT_LOWER_LEG:
                bodyPartIndex = 1;
                break;
            case WOLFOS_LIMB_FRONT_LEFT_LOWER_LEG:
                bodyPartIndex = 2;
                break;
            case WOLFOS_LIMB_THORAX:
                bodyPartIndex = 3;
                break;
            case WOLFOS_LIMB_ABDOMEN:
                bodyPartIndex = 4;
                break;
            case WOLFOS_LIMB_TAIL:
                bodyPartIndex = 5;
                break;
            case WOLFOS_LIMB_BACK_RIGHT_SHIN:
                bodyPartIndex = 6;
                break;
            case 37:
                //! @bug There is no limb with index this large, so bodyPartsPos[7] is uninitialised. Thus a flame will
                //! be drawn at 0,0,0 when the Wolfos is on fire.
                bodyPartIndex = 7;
                break;
            case WOLFOS_LIMB_BACK_RIGHT_PASTERN:
                bodyPartIndex = 8;
                break;
            case WOLFOS_LIMB_BACK_LEFT_PAW:
                bodyPartIndex = 9;
                break;
        }

        if (bodyPartIndex >= 0) {
            Vec3f bodyPartPos;

            Matrix_MultVec3f(&bodyPartVec, &bodyPartPos);
            this->bodyPartsPos[bodyPartIndex].x = bodyPartPos.x;
            this->bodyPartsPos[bodyPartIndex].y = bodyPartPos.y;
            this->bodyPartsPos[bodyPartIndex].z = bodyPartPos.z;
        }
    }
}

static void* sWolfosNormalEyeTextures[] = { gWolfosNormalEyeOpenTex, gWolfosNormalEyeHalfTex, gWolfosNormalEyeNarrowTex,
                                            gWolfosNormalEyeHalfTex };
static void* sWolfosWhiteEyeTextures[] = { gWolfosWhiteEyeOpenTex, gWolfosWhiteEyeHalfTex, gWolfosWhiteEyeNarrowTex,
                                           gWolfosWhiteEyeHalfTex };

void EnWf_Draw(Actor* thisx, PlayState* play) {
    EnWf* this = (EnWf*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    // This conditional will always evaluate to true, since unk_300 is false whenever action is
    // WOLFOS_ACTION_WAIT_TO_APPEAR.
    if ((this->action != WOLFOS_ACTION_WAIT_TO_APPEAR) || !this->unk_300) {
        Gfx_SetupDL_25Opa(play->state.gfxCtx);

        if (this->actor.params == WOLFOS_NORMAL) {
            gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sWolfosNormalEyeTextures[this->eyeIndex]));
        } else {
            gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sWolfosWhiteEyeTextures[this->eyeIndex]));
        }

        SkelAnime_DrawSkeletonOpa(play, &this->skelAnime, EnWf_OverrideLimbDraw, EnWf_PostLimbDraw, &this->actor);

        if (this->fireTimer != 0) {
            this->actor.colorFilterTimer++;
            this->fireTimer--;

            if ((this->fireTimer % 4) == 0) {
                s32 fireIndex = this->fireTimer >> 2;

                EffectSsEnFire_SpawnVec3s(play, &this->actor, &this->bodyPartsPos[fireIndex], 75, 0, 0, fireIndex);
            }
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

s32 EnWf_DodgeRanged(PlayState* play, EnWf* this) {
    Actor* actor = Actor_GetProjectileActor(play, &this->actor, 600.0f);

    if (actor != NULL) {
        s16 angleToFacing;
        s16 pad;
        f32 dist;

        angleToFacing = Actor_WorldYawTowardActor(&this->actor, actor) - this->actor.shape.rot.y;
        this->actor.world.rot.y = (u16)this->actor.shape.rot.y & 0xFFFF;
        dist = Actor_WorldDistXYZToPoint(&this->actor, &actor->world.pos);

        if ((ABS(angleToFacing) < 0x2EE0) && (sqrt(dist) < 400.0)) {
            EnWf_SetupBlocking(this);
        } else {
            this->actor.world.rot.y = this->actor.shape.rot.y + 0x3FFF;
            if ((ABS(angleToFacing) < 0x2000) || (ABS(angleToFacing) > 0x5FFF)) {
                EnWf_SetupSidestep(this, play);
                this->actor.speedXZ *= 2.0f;
            } else if (ABS(angleToFacing) < 0x5FFF) {
                EnWf_SetupBackflipAway(this);
            }
        }
        return true;
    }

    return false;
}
