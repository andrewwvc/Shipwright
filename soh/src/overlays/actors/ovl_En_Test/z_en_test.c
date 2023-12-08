/*
 * File: z_en_test.c
 * Overlay: ovl_En_Test
 * Description: Stalfos
 */

#include "z_en_test.h"
#include "objects/object_sk2/object_sk2.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_WHILE_CULLED)

void EnTest_Init(Actor* thisx, PlayState* play);
void EnTest_Destroy(Actor* thisx, PlayState* play);
void EnTest_Update(Actor* thisx, PlayState* play);
void EnTest_Draw(Actor* thisx, PlayState* play);

void EnTest_SetupWaitGround(EnTest* this);
void EnTest_SetupWaitAbove(EnTest* this);
void EnTest_SetupJumpBack(EnTest* this);
void EnTest_SetupShieldBash(EnTest* this);
void EnTest_SetupSlashDownEnd(EnTest* this);
void EnTest_SetupSlashUp(EnTest* this);
void EnTest_SetupJumpslash(EnTest* this);
void EnTest_SetupCrossoverJump(EnTest* this);
void EnTest_SetupWalkAndBlock(EnTest* this);
void func_80860EC0(EnTest* this);
void EnTest_SetupSlashDown(EnTest* this);
void EnTest_SetupThrust(EnTest* this);
void EnTest_SetupCrouch(EnTest* this);
void EnTest_SetupSpinAttack(EnTest* this);
void func_80860BDC(EnTest* this);
void EnTest_SetupIdleFromBlock(EnTest* this);
void EnTest_SetupRecoil(EnTest* this);
void EnTest_SetupFlinchBack(EnTest* this);
void EnTest_SetupFlinchFront(EnTest* this);
void EnTest_SetupStopAndBlock(EnTest* this);
void func_808627C4(EnTest* this, PlayState* play);

void EnTest_WaitGround(EnTest* this, PlayState* play);
void EnTest_WaitAbove(EnTest* this, PlayState* play);
void EnTest_Fall(EnTest* this, PlayState* play);
void EnTest_Land(EnTest* this, PlayState* play);
void EnTest_Rise(EnTest* this, PlayState* play);
void EnTest_Idle(EnTest* this, PlayState* play);
void EnTest_WalkAndBlock(EnTest* this, PlayState* play);
void func_80860C24(EnTest* this, PlayState* play);
void func_80860F84(EnTest* this, PlayState* play);
void EnTest_SlashDown(EnTest* this, PlayState* play);
void EnTest_ShieldBash(EnTest* this, PlayState* play);
void EnTest_SlashDownEnd(EnTest* this, PlayState* play);
void EnTest_Thrust(EnTest* this, PlayState* play);
void EnTest_Crouch(EnTest* this, PlayState* play);
void EnTest_SpinAttack(EnTest* this, PlayState* play);
void EnTest_SlashUp(EnTest* this, PlayState* play);
void EnTest_JumpBack(EnTest* this, PlayState* play);
void EnTest_Jumpslash(EnTest* this, PlayState* play);
void EnTest_CrossoverJump(EnTest* this, PlayState* play);
void EnTest_JumpUp(EnTest* this, PlayState* play);
void EnTest_StopAndBlock(EnTest* this, PlayState* play);
void EnTest_IdleFromBlock(EnTest* this, PlayState* play);
void EnTest_FlinchFront(EnTest* this, PlayState* play);
void EnTest_FlinchBack(EnTest* this, PlayState* play);
void EnTest_Stunned(EnTest* this, PlayState* play);
void func_808628C8(EnTest* this, PlayState* play);
void func_80862E6C(EnTest* this, PlayState* play);
void func_80863044(EnTest* this, PlayState* play);
void func_8086318C(EnTest* this, PlayState* play);
void EnTest_Recoil(EnTest* this, PlayState* play);
void func_808633E8(EnTest* this, PlayState* play);
void func_80862FA8(EnTest* this, PlayState* play);

s32 EnTest_ReactToProjectile(PlayState* play, EnTest* this);

s16 gStalfosJumpSlash2AnimFrameData[645] = {
	0xff66, 0x0000, 0xffff, 0x4000, 0xbf70, 0x4286, 0xbd79, 0xbfff, 0xff97, 0xbcc5, 0x01e7, 0x3de2, 0xfcd5, 0xfc98, 0x0031, 0x1dbe, 
	0xfff9, 0xfcd4, 0x098a, 0x08a3, 0x0795, 0x05c3, 0x061c, 0x06ab, 0x0758, 0x0809, 0x08a6, 0x0916, 0x0941, 0xfee7, 0xfebd, 0xfeae, 
	0xff3e, 0xff06, 0xfeb7, 0xfe5c, 0xfe02, 0xfdb3, 0xfd7c, 0xfd67, 0xfcc0, 0x1022, 0x198b, 0x18a6, 0x141a, 0x0d10, 0x04b2, 0xfc29, 
	0xf4a1, 0xef41, 0xed35, 0xfb40, 0xfc15, 0xfcb5, 0xfd1f, 0xfd8d, 0xfdf9, 0xfe5d, 0xfeb4, 0xfef9, 0xff27, 0xff37, 0x4820, 0x5159, 
	0x55cb, 0x554c, 0x530a, 0x4f95, 0x4b7c, 0x4751, 0x43a4, 0x4106, 0x4006, 0xf928, 0x029a, 0x02fc, 0x0786, 0x0b6f, 0x0b93, 0x0b52, 
	0x0ac5, 0x0a05, 0x092a, 0x084e, 0x00d8, 0x02c5, 0x05a5, 0x04b8, 0xfeaf, 0xfca6, 0xfa9c, 0xf8bb, 0xf72e, 0xf620, 0xf5bd, 0xf0af, 
	0xfe3f, 0x096a, 0x0cb2, 0x11e8, 0x14b5, 0x16f3, 0x1899, 0x19a2, 0x1a07, 0x19c0, 0xe69b, 0xd974, 0xd403, 0xd684, 0xdc2a, 0xe3e7, 
	0xecad, 0xf56e, 0xfd1b, 0x02a8, 0x0503, 0x09be, 0x1327, 0x1771, 0x167f, 0x13a6, 0x0f80, 0x0aa7, 0x05b5, 0x0144, 0xfded, 0xfc4c, 
	0x1482, 0x11dc, 0x109d, 0x10c8, 0x116b, 0x1264, 0x1390, 0x14cb, 0x15f2, 0x16e2, 0x1777, 0x182d, 0x0fe6, 0x0a69, 0x07c8, 0x0594, 
	0x03c7, 0x025b, 0x014b, 0x008f, 0x0023, 0x0000, 0x1554, 0x1a26, 0x1ba9, 0x19bc, 0x1641, 0x11c7, 0x0cdd, 0x0813, 0x03f6, 0x0115, 
	0xffff, 0xea63, 0xdb53, 0xd240, 0xcf54, 0xcde1, 0xcd91, 0xce10, 0xcf09, 0xd028, 0xd118, 0xd184, 0xc4d1, 0xcb6e, 0xcf12, 0xcfa6, 
	0xcf50, 0xce52, 0xccef, 0xcb67, 0xc9fe, 0xc8f4, 0xc88d, 0x2e35, 0x2f58, 0x1d0b, 0x1783, 0x120f, 0x0ce2, 0x0831, 0x0430, 0x0113, 
	0xff0e, 0xfe56, 0xfbf6, 0xfd21, 0xfdb6, 0xfd1c, 0xfc52, 0xfb6d, 0xfa81, 0xf9a6, 0xf8ef, 0xf871, 0xf843, 0x104e, 0x0f82, 0x0eb6, 
	0x0d3d, 0x0b84, 0x09ad, 0x07dc, 0x0635, 0x04da, 0x03f0, 0x039a, 0xfda8, 0xfcd5, 0xfc29, 0xfb7f, 0xfadc, 0xfa44, 0xf9be, 0xf94c, 
	0xf8f5, 0xf8bd, 0xf8aa, 0x0fc7, 0x10f0, 0x1133, 0x1105, 0x1083, 0x0fc8, 0x0ef0, 0x0e18, 0x0d5a, 0x0cd4, 0x0ca1, 0x189a, 0x1883, 
	0x1678, 0x1331, 0x0f12, 0x0a80, 0x05e0, 0x0198, 0xfe0b, 0xfba0, 0xfabb, 0xca7c, 0xcb93, 0xcea7, 0xd344, 0xd8d6, 0xdf16, 0xe5c1, 
	0xec92, 0xf345, 0xf994, 0xff3c, 0xebfe, 0xeb7c, 0xea0b, 0xe77a, 0xe3e0, 0xdfb8, 0xdb7d, 0xd7a8, 0xd4b5, 0xd31d, 0xd35d, 0xfa62, 
	0xf963, 0xf692, 0xf26f, 0xed8b, 0xe814, 0xe235, 0xdc1a, 0xd5f1, 0xcfe4, 0xca20, 0xb77b, 0xb774, 0xb760, 0xb715, 0xb684, 0xb5d6, 
	0xb531, 0xb4be, 0xb4a6, 0xb511, 0xb627, 0xc8ec, 0xc986, 0xcb3c, 0xce1e, 0xd1fc, 0xd669, 0xdafb, 0xdf44, 0xe2da, 0xe550, 0xe63a, 
	0xdfb1, 0xdfbd, 0xdfe0, 0xe01b, 0xe06a, 0xe0c4, 0xe122, 0xe179, 0xe1c2, 0xe1f5, 0xe207, 0xcddf, 0xcd35, 0xcb54, 0xc829, 0xc3e9, 
	0xbf0b, 0xba05, 0xb54e, 0xb15d, 0xaea9, 0xada8, 0x156c, 0x195b, 0xfb7d, 0xfb77, 0xfc86, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 
	0x0000, 0x0ea5, 0x0107, 0xf9c9, 0xfa53, 0xfc3e, 0xffff, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff, 0xc3a1, 0xc2fe, 0xba97, 0xba2e, 
	0xbbb7, 0xbd79, 0xbd79, 0xbd79, 0xbd79, 0xbd79, 0xbd79, 0x1b98, 0x1485, 0x1150, 0x0c94, 0x0629, 0xfddb, 0xfc9d, 0xfcd1, 0xfb28, 
	0xf943, 0xf89e, 0x1db8, 0x1b5d, 0x084c, 0x0c5c, 0x1099, 0x117a, 0x13ec, 0x15e6, 0x1744, 0x184a, 0x18b1, 0x7ee2, 0x786d, 0x847f, 
	0x8421, 0x80d3, 0x822b, 0x8fca, 0x9f2f, 0xa974, 0xb1dd, 0xb5b9, 0x032f, 0x0000, 0x0000, 0xff76, 0xffff, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0xf55d, 0x0000, 0x0000, 0x01d0, 0x0000, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xd2f7, 0xed09, 
	0xf47e, 0xf153, 0xecca, 0xe644, 0xdf23, 0xd848, 0xd295, 0xceed, 0xce31, 0x0113, 0x01f3, 0xf609, 0xfafa, 0x0000, 0xffff, 0xffff, 
	0xffff, 0xffff, 0x0000, 0xffff, 0x1050, 0xfe1b, 0x00c2, 0xfe56, 0xfe68, 0xfe57, 0xfe95, 0xff01, 0xff77, 0xffd7, 0xffff, 0xdabe, 
	0xe73a, 0xed4f, 0xeb93, 0xe6db, 0xe0bc, 0xd9f9, 0xd35a, 0xcda8, 0xc9aa, 0xc829, 0xdefc, 0x5200, 0x4c07, 0x4d4f, 0x514b, 0xd717, 
	0xddd3, 0xe49b, 0xea8c, 0xeec3, 0xf05d, 0x5ede, 0x1f71, 0x1f21, 0x2040, 0x2215, 0x5ba3, 0x592d, 0x56cd, 0x54c6, 0x535d, 0x52d6, 
	0xdb5c, 0x4cd5, 0x4622, 0x478a, 0x4bf5, 0xd268, 0xd9e6, 0xe171, 0xe80e, 0xecbf, 0xee88, 0xe453, 0xe18e, 0xe106, 0xe663, 0xe579, 
	0xe418, 0xe24f, 0xe054, 0xdea8, 0xddab, 0xdd5e, 0x23e7, 0x2087, 0x1cd5, 0x1783, 0x15aa, 0x1452, 0x1425, 0x1549, 0x174f, 0x194d, 
	0x1a2e, 0x995d, 0x9e48, 0xa8a3, 0xb4b1, 0xaee5, 0xa830, 0xa2f7, 0xa073, 0xa079, 0xa1ba, 0xa279, 0x1fe7, 0x26db, 0x2cd1, 0x3061, 
	0x3478, 0x381d, 0x39f4, 0x398a, 0x3765, 0x34db, 0x33a9, 0xea93, 0xe99e, 0xe94f, 0xe8fe, 0xe8b0, 0xe865, 0xe823, 0xe7ea, 0xe7be, 
	0xe7a1, 0xe797, 0x084a, 0xf495, 0xf42f, 0xf48e, 0xf580, 0xf6d4, 0xf85a, 0xf9df, 0xfb31, 0xfc21, 0xfc7c, 0x7af6, 0x5bc3, 0x5b1d, 
	0x5bad, 0x5d27, 0x5f3c, 0x619f, 0x6402, 0x6616, 0x678f, 0x681e, 0xe3b8, 0xf08e, 0xf345, 0xdd59, 0xddcb, 0xdfae, 0xe413, 0xeb26, 
	0xf368, 0xfa08, 0xfc9e, 0x24fb, 0x2781, 0x26f9, 0x1bf4, 0x1d43, 0x1fcf, 0x233c, 0x2669, 0x2855, 0x28f7, 0x2906, 0x7148, 0x6e3e, 
	0x6a0e, 0x51a0, 0x59e4, 0x6587, 0x738d, 0x8299, 0x9093, 0x9aa5, 0x9e6a, 0x4efc, 0x4cae, 0x4fe3, 0x608c, 0x5e21, 0x5a20, 0x54df, 
	0x4efd, 0x4960, 0x4525, 0x437b, 0xff6a, 0xf9f5, 0xf70d, 0xf895, 0xfc01, 0xffcc, 0x026d, 0x036f, 0x03a6, 0x037b, 0x0356, 0x08b2, 
	0x08e7, 0x0916, 0x093d, 0x0961, 0x0981, 0x099b, 0x09b0, 0x09bf, 0x09c8, 0x09cc, 0x7f53, 0x7c8c, 0x7aa2, 0x79d9, 0x7990, 0x7996, 
	0x79b7, 0x7a0e, 0x7aab, 0x7b41, 0x7b83, };

JointIndex gStalfosJumpSlash2AnimJointIndices[61] = {
	{ 0x0000, 0x0012, 0x001d, },
	{ 0x0001, 0x0001, 0x0001, },
	{ 0x0002, 0x0003, 0x0001, },
	{ 0x0028, 0x0033, 0x003e, },
	{ 0x0049, 0x0054, 0x005f, },
	{ 0x006a, 0x0075, 0x0080, },
	{ 0x008b, 0x0096, 0x00a1, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0001, 0x0001, 0x0004, },
	{ 0x0002, 0x0002, 0x00ac, },
	{ 0x0002, 0x0002, 0x0001, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0002, 0x0002, 0x0002, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0005, 0x0003, 0x0001, },
	{ 0x00b7, 0x00c2, 0x00cd, },
	{ 0x0002, 0x0002, 0x0001, },
	{ 0x0006, 0x0007, 0x0001, },
	{ 0x00d8, 0x00e3, 0x00ee, },
	{ 0x0002, 0x0002, 0x0001, },
	{ 0x0008, 0x0001, 0x0006, },
	{ 0x00f9, 0x0104, 0x010f, },
	{ 0x0001, 0x0002, 0x011a, },
	{ 0x0125, 0x0130, 0x013b, },
	{ 0x0001, 0x0001, 0x0002, },
	{ 0x0001, 0x0001, 0x0002, },
	{ 0x0001, 0x0002, 0x0001, },
	{ 0x0002, 0x0001, 0x0002, },
	{ 0x0146, 0x0151, 0x015c, },
	{ 0x0167, 0x0172, 0x017d, },
	{ 0x0188, 0x0193, 0x019e, },
	{ 0x01a9, 0x01b4, 0x01bf, },
	{ 0x0002, 0x0002, 0x0001, },
	{ 0x0002, 0x0002, 0x0001, },
	{ 0x0002, 0x0002, 0x0001, },
	{ 0x0002, 0x0001, 0x0001, },
	{ 0x0002, 0x0001, 0x0001, },
	{ 0x01ca, 0x01d5, 0x01e0, },
	{ 0x0002, 0x0002, 0x0007, },
	{ 0x0002, 0x0001, 0x0001, },
	{ 0x0009, 0x0002, 0x0003, },
	{ 0x01eb, 0x01f6, 0x0201, },
	{ 0x0002, 0x0001, 0x020c, },
	{ 0x0002, 0x0002, 0x000a, },
	{ 0x0001, 0x0002, 0x000b, },
	{ 0x0217, 0x0222, 0x022d, },
	{ 0x0002, 0x0001, 0x0002, },
	{ 0x0002, 0x0002, 0x000a, },
	{ 0x0001, 0x0001, 0x000c, },
	{ 0x000d, 0x000e, 0x000f, },
	{ 0x0238, 0x0243, 0x024e, },
	{ 0x0001, 0x0001, 0x0259, },
	{ 0x0010, 0x0001, 0x000a, },
	{ 0x0002, 0x0001, 0x000b, },
	{ 0x0264, 0x026f, 0x027a, },
	{ 0x0002, 0x0001, 0x0002, },
	{ 0x0010, 0x0001, 0x000a, },
	{ 0x0010, 0x0001, 0x0011, },
	{ 0x0002, 0x0001, 0x0001, },
};

AnimationHeader gStalfosJumpSlash2Anim = { { 11 }, gStalfosJumpSlash2AnimFrameData, gStalfosJumpSlash2AnimJointIndices, 18 };

s16 gStalfosJumpSlash3AnimFrameData[645] = {
	0xff66, 0x0000, 0xffff, 0x4000, 0xbf70, 0x4286, 0xbd79, 0xbfff, 0xff97, 0xbcc5, 0x01e7, 0x3de2, 0xfcd5, 0xfc98, 0x0031, 0x1dbe, 
	0xfff9, 0xfcd4, 0x098a, 0x08a3, 0x0795, 0x05c3, 0x061c, 0x06ab, 0x0758, 0x0809, 0x08a6, 0x0916, 0x0941, 0xfee7, 0xfebd, 0xfeae, 
	0xff3e, 0xff06, 0xfeb7, 0xfe5c, 0xfe02, 0xfdb3, 0xfd7c, 0xfd67, 0xfcc0, 0x1022, 0x198b, 0x18a6, 0x141a, 0x0d10, 0x04b2, 0xfc29, 
	0xf4a1, 0xef41, 0xed35, 0xfb40, 0xfc15, 0xfcb5, 0xfd1f, 0xfd8d, 0xfdf9, 0xfe5d, 0xfeb4, 0xfef9, 0xff27, 0xff37, 0x4820, 0x5159, 
	0x55cb, 0x554c, 0x530a, 0x4f95, 0x4b7c, 0x4751, 0x43a4, 0x4106, 0x4006, 0xf95d, 0x029a, 0xf95b, 0x0166, 0x04c1, 0x05a1, 0x0b52, 
	0x0ac5, 0x0a05, 0x092a, 0x084e, 0xffb1, 0x02c5, 0x032b, 0x0463, 0x0663, 0x00c4, 0xfa9c, 0xf8bb, 0xf72e, 0xf620, 0xf5bd, 0xf09f, 
	0xfe3f, 0x0b0c, 0x0ca2, 0x0fc3, 0x1470, 0x16f3, 0x1899, 0x19a2, 0x1a07, 0x19c0, 0xe69b, 0xd974, 0xd403, 0xd684, 0xdc2a, 0xe3e7, 
	0xecad, 0xf56e, 0xfd1b, 0x02a8, 0x0503, 0x09be, 0x1327, 0x1771, 0x167f, 0x13a6, 0x0f80, 0x0aa7, 0x05b5, 0x0144, 0xfded, 0xfc4c, 
	0x1482, 0x11dc, 0x109d, 0x10c8, 0x116b, 0x1264, 0x1390, 0x14cb, 0x15f2, 0x16e2, 0x1777, 0x182d, 0x0fe6, 0x11dc, 0x0a77, 0x0e19, 
	0x03c7, 0x025b, 0x014b, 0x008f, 0x0023, 0x0000, 0x1554, 0x1a26, 0x1e73, 0x171b, 0x1717, 0x11c7, 0x0cdd, 0x0813, 0x03f6, 0x0115, 
	0xffff, 0xea63, 0xdb53, 0xd508, 0xcf03, 0xcddf, 0xcd91, 0xce10, 0xcf09, 0xd028, 0xd118, 0xd184, 0xc4d1, 0xcb6e, 0xcf12, 0xcfa6, 
	0xcf50, 0xce52, 0xccef, 0xcb67, 0xc9fe, 0xc8f4, 0xc88d, 0x2e35, 0x2f58, 0x1d0b, 0x1783, 0x120f, 0x0ce2, 0x0831, 0x0430, 0x0113, 
	0xff0e, 0xfe56, 0xfbf6, 0xfd21, 0xfdb6, 0xfd1c, 0xfc52, 0xfb6d, 0xfa81, 0xf9a6, 0xf8ef, 0xf871, 0xf843, 0x104e, 0x0f82, 0x0eb6, 
	0x0d3d, 0x0b84, 0x09ad, 0x07dc, 0x0635, 0x04da, 0x03f0, 0x039a, 0xfda8, 0xfcd5, 0xfc29, 0xfb7f, 0xfadc, 0xfa44, 0xf9be, 0xf94c, 
	0xf8f5, 0xf8bd, 0xf8aa, 0x0fc7, 0x10f0, 0x1133, 0x1105, 0x1083, 0x0fc8, 0x0ef0, 0x0e18, 0x0d5a, 0x0cd4, 0x0ca1, 0x189a, 0x1883, 
	0x1678, 0x1331, 0x0f12, 0x0a80, 0x05e0, 0x0198, 0xfe0b, 0xfba0, 0xfabb, 0xca7c, 0xcb93, 0xcea7, 0xd344, 0xd8d6, 0xdf16, 0xe5c1, 
	0xec92, 0xf345, 0xf994, 0xff3c, 0xebfe, 0xeb7c, 0xea0b, 0xe77a, 0xe3e0, 0xdfb8, 0xdb7d, 0xd7a8, 0xd4b5, 0xd31d, 0xd35d, 0xfa62, 
	0xf963, 0xf692, 0xf26f, 0xed8b, 0xe814, 0xe235, 0xdc1a, 0xd5f1, 0xcfe4, 0xca20, 0xb77b, 0xb774, 0xb760, 0xb715, 0xb684, 0xb5d6, 
	0xb531, 0xb4be, 0xb4a6, 0xb511, 0xb627, 0xc8ec, 0xc986, 0xcb3c, 0xce1e, 0xd1fc, 0xd669, 0xdafb, 0xdf44, 0xe2da, 0xe550, 0xe63a, 
	0xdfb1, 0xdfbd, 0xdfe0, 0xe01b, 0xe06a, 0xe0c4, 0xe122, 0xe179, 0xe1c2, 0xe1f5, 0xe207, 0xcddf, 0xcd35, 0xcb54, 0xc829, 0xc3e9, 
	0xbf0b, 0xba05, 0xb54e, 0xb15d, 0xaea9, 0xada8, 0x156c, 0x195b, 0x0030, 0xfc5c, 0xfbab, 0x057b, 0x0122, 0x0000, 0xffff, 0xffff, 
	0x0000, 0x0ea5, 0x0107, 0xf041, 0xf6a1, 0xeb10, 0xf5f3, 0xf820, 0xffff, 0x0000, 0xffff, 0xffff, 0xc3a1, 0xc2fe, 0xbf16, 0xbc16, 
	0xbfaf, 0xbd20, 0xbcef, 0xbd79, 0xbd79, 0xbd79, 0xbd79, 0x1b98, 0x134c, 0x1150, 0x0bb9, 0x0629, 0xfddb, 0xfc9d, 0xfcd1, 0xfb28, 
	0xf943, 0xf89e, 0x1db8, 0x1f58, 0x084c, 0x1f3b, 0x1099, 0x117a, 0x13ec, 0x15e6, 0x1744, 0x184a, 0x18b1, 0x7ee2, 0x79fc, 0x847f, 
	0x8a69, 0x80d3, 0x822b, 0x8fca, 0x9f2f, 0xa974, 0xb1dd, 0xb5b9, 0x0672, 0xf4f3, 0x00d5, 0xff76, 0xffff, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0xeeac, 0xfa23, 0xfecb, 0x01d0, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xd139, 0xecd3, 
	0xf48c, 0xf153, 0xecca, 0xe644, 0xdf23, 0xd848, 0xd295, 0xceed, 0xce31, 0x0994, 0x01f3, 0xeff6, 0xfafa, 0x0005, 0x0000, 0x03eb, 
	0xffff, 0xffff, 0x0000, 0xffff, 0xf3a2, 0xfe1b, 0x01ac, 0xfe56, 0xfde0, 0xfe57, 0x074f, 0xff01, 0xff77, 0xffd7, 0xffff, 0xd8a6, 
	0xe73a, 0xec95, 0xeb93, 0xe93c, 0xe0bc, 0xd454, 0xd35a, 0xcda8, 0xc9aa, 0xc829, 0xdefc, 0x5200, 0x4c07, 0x4d4f, 0x514b, 0xd717, 
	0xddd3, 0xe49b, 0xea8c, 0xeec3, 0xf05d, 0x5ede, 0x1f71, 0x1f21, 0x2040, 0x2215, 0x5ba3, 0x592d, 0x56cd, 0x54c6, 0x535d, 0x52d6, 
	0xdb5c, 0x4cd5, 0x4622, 0x478a, 0x4bf5, 0xd268, 0xd9e6, 0xe171, 0xe80e, 0xecbf, 0xee88, 0xe453, 0xe18e, 0xe106, 0xe663, 0xe579, 
	0xe418, 0xe24f, 0xe054, 0xdea8, 0xddab, 0xdd5e, 0x23e7, 0x2087, 0x1cd5, 0x1783, 0x15aa, 0x1452, 0x1425, 0x1549, 0x174f, 0x194d, 
	0x1a2e, 0x995d, 0x9e48, 0xa8a3, 0xb4b1, 0xaee5, 0xa830, 0xa2f7, 0xa073, 0xa079, 0xa1ba, 0xa279, 0x1fe7, 0x26db, 0x2cd1, 0x3061, 
	0x3478, 0x381d, 0x39f4, 0x398a, 0x3765, 0x34db, 0x33a9, 0xea93, 0xe99e, 0xe94f, 0xe8fe, 0xe8b0, 0xe865, 0xe823, 0xe7ea, 0xe7be, 
	0xe7a1, 0xe797, 0x084a, 0xf495, 0xf42f, 0xf48e, 0xf580, 0xf6d4, 0xf85a, 0xf9df, 0xfb31, 0xfc21, 0xfc7c, 0x7af6, 0x5bc3, 0x5b1d, 
	0x5bad, 0x5d27, 0x5f3c, 0x619f, 0x6402, 0x6616, 0x678f, 0x681e, 0xe3b8, 0xf08e, 0xf345, 0xdd59, 0xddcb, 0xdfae, 0xe413, 0xeb26, 
	0xf368, 0xfa08, 0xfc9e, 0x24fb, 0x2781, 0x26f9, 0x1bf4, 0x1d43, 0x1fcf, 0x233c, 0x2669, 0x2855, 0x28f7, 0x2906, 0x7148, 0x6e3e, 
	0x6a0e, 0x51a0, 0x59e4, 0x6587, 0x738d, 0x8299, 0x9093, 0x9aa5, 0x9e6a, 0x4efc, 0x4cae, 0x4fe3, 0x608c, 0x5e21, 0x5a20, 0x54df, 
	0x4efd, 0x4960, 0x4525, 0x437b, 0xff6a, 0xf9f5, 0xf70d, 0xf895, 0xfc01, 0xffcc, 0x026d, 0x036f, 0x03a6, 0x037b, 0x0356, 0x08b2, 
	0x08e7, 0x0916, 0x093d, 0x0961, 0x0981, 0x099b, 0x09b0, 0x09bf, 0x09c8, 0x09cc, 0x7f53, 0x7c8c, 0x7aa2, 0x79d9, 0x7990, 0x7996, 
	0x79b7, 0x7a0e, 0x7aab, 0x7b41, 0x7b83, };

JointIndex gStalfosJumpSlash3AnimJointIndices[61] = {
	{ 0x0000, 0x0012, 0x001d, },
	{ 0x0001, 0x0001, 0x0001, },
	{ 0x0002, 0x0003, 0x0001, },
	{ 0x0028, 0x0033, 0x003e, },
	{ 0x0049, 0x0054, 0x005f, },
	{ 0x006a, 0x0075, 0x0080, },
	{ 0x008b, 0x0096, 0x00a1, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0001, 0x0001, 0x0004, },
	{ 0x0002, 0x0002, 0x00ac, },
	{ 0x0002, 0x0001, 0x0001, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0002, 0x0001, 0x0001, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0005, 0x0003, 0x0001, },
	{ 0x00b7, 0x00c2, 0x00cd, },
	{ 0x0001, 0x0001, 0x0001, },
	{ 0x0006, 0x0007, 0x0001, },
	{ 0x00d8, 0x00e3, 0x00ee, },
	{ 0x0002, 0x0001, 0x0001, },
	{ 0x0008, 0x0002, 0x0006, },
	{ 0x00f9, 0x0104, 0x010f, },
	{ 0x0001, 0x0002, 0x011a, },
	{ 0x0125, 0x0130, 0x013b, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0002, 0x0001, 0x0002, },
	{ 0x0001, 0x0002, 0x0002, },
	{ 0x0146, 0x0151, 0x015c, },
	{ 0x0167, 0x0172, 0x017d, },
	{ 0x0188, 0x0193, 0x019e, },
	{ 0x01a9, 0x01b4, 0x01bf, },
	{ 0x0001, 0x0002, 0x0001, },
	{ 0x0001, 0x0002, 0x0001, },
	{ 0x0001, 0x0001, 0x0001, },
	{ 0x0002, 0x0002, 0x0002, },
	{ 0x0002, 0x0001, 0x0001, },
	{ 0x01ca, 0x01d5, 0x01e0, },
	{ 0x0002, 0x0002, 0x0007, },
	{ 0x0002, 0x0001, 0x0001, },
	{ 0x0009, 0x0002, 0x0003, },
	{ 0x01eb, 0x01f6, 0x0201, },
	{ 0x0002, 0x0001, 0x020c, },
	{ 0x0002, 0x0002, 0x000a, },
	{ 0x0001, 0x0002, 0x000b, },
	{ 0x0217, 0x0222, 0x022d, },
	{ 0x0002, 0x0001, 0x0002, },
	{ 0x0002, 0x0002, 0x000a, },
	{ 0x0001, 0x0001, 0x000c, },
	{ 0x000d, 0x000e, 0x000f, },
	{ 0x0238, 0x0243, 0x024e, },
	{ 0x0001, 0x0001, 0x0259, },
	{ 0x0010, 0x0001, 0x000a, },
	{ 0x0002, 0x0001, 0x000b, },
	{ 0x0264, 0x026f, 0x027a, },
	{ 0x0002, 0x0001, 0x0002, },
	{ 0x0010, 0x0001, 0x000a, },
	{ 0x0010, 0x0001, 0x0011, },
	{ 0x0002, 0x0001, 0x0001, },
};

AnimationHeader gStalfosJumpSlash3Anim = { { 11 }, gStalfosJumpSlash3AnimFrameData, gStalfosJumpSlash3AnimJointIndices, 18 };

s16 gStalfosStabAnimFrameData[1023] = {
	0xff66, 0xffff, 0x4000, 0x0000, 0xbf70, 0xbd79, 0xbfff, 0xf8cb, 0x0c78, 0xfad2, 0xff97, 0x0099, 0xd3e0, 0xc8a6, 0xb69b, 0xe619, 
	0xe205, 0xadcc, 0xf070, 0x52dd, 0xee9d, 0x01e7, 0xe797, 0xfc80, 0x6824, 0xfcd5, 0xfff9, 0x0341, 0x09c8, 0x7b9c, 0xfcd4, 0x0958, 
	0x096b, 0x096b, 0x0932, 0x08a8, 0x0804, 0x0778, 0x0739, 0x0736, 0x0737, 0x0738, 0x073a, 0x078e, 0x0841, 0x08f3, 0x0944, 0xfd50, 
	0xfd42, 0xfd42, 0xfd96, 0xfe65, 0xff6c, 0x0064, 0x010a, 0x013c, 0x014c, 0x0154, 0x0156, 0x00b7, 0xff5a, 0xfdfe, 0xfd5f, 0x0000, 
	0x0000, 0x0000, 0x0078, 0x0196, 0x02ec, 0x040a, 0x0482, 0x0482, 0x0482, 0x0482, 0x0482, 0x03ce, 0x0241, 0x00b4, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0110, 0x039a, 0x06a1, 0x092a, 0x0a3b, 0x0a3b, 0x0a3b, 0x0a3b, 0x0a3b, 0x08a1, 0x051d, 0x0199, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x001d, 0x0062, 0x00b4, 0x00f8, 0x0115, 0x0115, 0x0115, 0x0115, 0x0115, 0x00ea, 0x008b, 0x002b, 0x0000, 0xed0e, 
	0xed0e, 0xed0e, 0xeddd, 0xefcb, 0xf219, 0xf407, 0xf4d6, 0xf4d6, 0xf4d6, 0xf4d6, 0xf4d6, 0xf39f, 0xf0f2, 0xee45, 0xed0e, 0xff31, 
	0xff31, 0xff31, 0xff1c, 0xfee9, 0xfeac, 0xfe79, 0xfe63, 0xfe63, 0xfe63, 0xfe63, 0xfe63, 0xfe84, 0xfeca, 0xff11, 0xff31, 0x3ff4, 
	0x3ff4, 0x3ff4, 0x3ff3, 0x3ff1, 0x3fef, 0x3fed, 0x3fec, 0x3fec, 0x3fec, 0x3fec, 0x3fec, 0x3fed, 0x3ff0, 0x3ff3, 0x3ff4, 0x07ea, 
	0x07ea, 0x07ea, 0x091f, 0x0bfe, 0x0f6d, 0x124c, 0x1381, 0x1381, 0x137a, 0x1348, 0x12c1, 0x10a4, 0x0cf6, 0x0978, 0x07ea, 0xf5bb, 
	0xf5bb, 0xf5bb, 0xf572, 0xf4c4, 0xf3f4, 0xf346, 0xf2fd, 0xf2fd, 0xf2fc, 0xf2fb, 0xf2fa, 0xf368, 0xf45b, 0xf54d, 0xf5bb, 0x1963, 
	0x1963, 0x1963, 0x19bc, 0x1a8f, 0x1b8c, 0x1c5f, 0x1cb8, 0x1cb8, 0x1cbd, 0x1cc6, 0x1ccc, 0x1c43, 0x1b17, 0x19eb, 0x1963, 0x0528, 
	0x0528, 0x0528, 0x0528, 0x0528, 0x0528, 0x0528, 0x0528, 0x0528, 0x0519, 0x04fe, 0x04ef, 0x04f8, 0x050c, 0x051f, 0x0528, 0xfc12, 
	0xfc12, 0xfc12, 0xfc12, 0xfc12, 0xfc12, 0xfc12, 0xfc12, 0xfc12, 0xfc82, 0xfd52, 0xfdc2, 0xfd7e, 0xfcea, 0xfc55, 0xfc12, 0x1797, 
	0x1797, 0x1797, 0x1797, 0x1797, 0x1797, 0x1797, 0x1797, 0x1797, 0x178e, 0x177d, 0x1774, 0x177a, 0x1786, 0x1792, 0x1797, 0x0031, 
	0x0031, 0x0031, 0x005a, 0x00bd, 0x0132, 0x0194, 0x01bd, 0x01bd, 0x01bd, 0x01bd, 0x01bd, 0x017f, 0x00f7, 0x006f, 0x0031, 0x0009, 
	0x0009, 0x0009, 0xff6a, 0xfdf2, 0xfc30, 0xfab8, 0xfa1a, 0xfa1a, 0xfa1a, 0xfa1a, 0xfa1a, 0xfb07, 0xfd11, 0xff1b, 0x0009, 0xd1ce, 
	0xd1ce, 0xd1ce, 0xd09f, 0xcdcd, 0xca6e, 0xc79c, 0xc66d, 0xc66d, 0xc66d, 0xc66d, 0xc66d, 0xc834, 0xcc1d, 0xd007, 0xd1ce, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0xff82, 0xfe9b, 0xfe1e, 0xfe1e, 0xfe51, 0xfec7, 0xff47, 0xffa4, 0xffdc, 0xfff8, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0xfe54, 0xfb3a, 0xf98f, 0xf98f, 0xf9dc, 0xfaad, 0xfbdc, 0xfd40, 0xfe97, 0xff99, 0x0000, 0xc877, 
	0xc877, 0xc877, 0xc877, 0xc877, 0xcbfd, 0xd287, 0xd60c, 0xd60c, 0xd529, 0xd2f4, 0xd01e, 0xcd47, 0xcad4, 0xc91d, 0xc877, 0x4286, 
	0x4286, 0x4286, 0x4162, 0x3eaa, 0x3b6b, 0x38b3, 0x378f, 0x378f, 0x378f, 0x378f, 0x378f, 0x3945, 0x3d0a, 0x40cf, 0x4286, 0x4000, 
	0x4000, 0x4000, 0x3d70, 0x3753, 0x3008, 0x29eb, 0x275b, 0x275b, 0x275b, 0x275b, 0x275b, 0x2b35, 0x33ad, 0x3c26, 0x4000, 0x0000, 
	0x0000, 0x0000, 0xfb21, 0xef87, 0xe1ae, 0xd614, 0xd136, 0xd136, 0xd136, 0xd136, 0xd136, 0xd886, 0xe89b, 0xf8b0, 0x0000, 0xfe63, 
	0xfe63, 0xfe63, 0xff0f, 0x00a9, 0x0292, 0x042c, 0x04d7, 0x04d7, 0x04d7, 0x04d7, 0x04d7, 0x03d5, 0x019e, 0xff65, 0xfe63, 0xf840, 
	0xf840, 0xf840, 0xf937, 0xfb84, 0xfe44, 0x0092, 0x0189, 0x0189, 0x0189, 0x0189, 0x0189, 0x0016, 0xfce4, 0xf9b3, 0xf840, 0x03b6, 
	0x03b6, 0x03b6, 0x05a5, 0x0a41, 0x0fc2, 0x145f, 0x164e, 0x164e, 0x164e, 0x164e, 0x164e, 0x1366, 0x0d02, 0x069e, 0x03b6, 0x0000, 
	0x0000, 0x0000, 0x007b, 0x03d9, 0x0cfe, 0x28f8, 0x3e73, 0x3e73, 0x3e73, 0x3e73, 0x3e73, 0x34b1, 0x1f3a, 0x09c2, 0xffff, 0xffff, 
	0xffff, 0xffff, 0x0671, 0x1267, 0x18d8, 0x1809, 0x173b, 0x173b, 0x173b, 0x173b, 0x173b, 0x139a, 0x0b9d, 0x03a1, 0xffff, 0xbd79, 
	0xbd79, 0xbd79, 0xb6bf, 0xaa40, 0xa385, 0xad2c, 0xb6d3, 0xb6d3, 0xb6d3, 0xb6d3, 0xb6d3, 0xb7dd, 0xba26, 0xbc6f, 0xbd79, 0xf8dc, 
	0xf8dc, 0xf997, 0xf92d, 0xf800, 0xf6e0, 0xf7de, 0xf8dc, 0xf8bf, 0xf940, 0xfa2f, 0xfab0, 0xfa67, 0xf9c6, 0xf925, 0xf8dc, 0x18e6, 
	0x18e6, 0x1a21, 0x17e2, 0x1134, 0x0a92, 0x11bc, 0x18e6, 0x188e, 0x186f, 0x1863, 0x1862, 0x1876, 0x18a4, 0x18d1, 0x18e6, 0xb5fa, 
	0xb5fa, 0xb4f8, 0xb839, 0xbd90, 0xbf95, 0xbae3, 0xb5fa, 0xb5d6, 0xb5d1, 0xb5cb, 0xb5c7, 0xb5cf, 0xb5e1, 0xb5f2, 0xb5fa, 0x0000, 
	0x0000, 0xffcd, 0x05fe, 0x1229, 0x1989, 0x042c, 0xeeb6, 0xeea7, 0xeea7, 0xeea7, 0xeea7, 0xf15c, 0xf753, 0xfd49, 0x0000, 0x0000, 
	0x0000, 0xff2c, 0x0343, 0x0c47, 0x12dd, 0x04f4, 0xf70b, 0xf826, 0xf826, 0xf826, 0xf826, 0xf95f, 0xfc12, 0xfec5, 0x0000, 0xce96, 
	0xce96, 0xcead, 0xd4b5, 0xe274, 0xf103, 0xf7c2, 0xf948, 0xf9e7, 0xf9e7, 0xf9e7, 0xf9e7, 0xf323, 0xe43f, 0xd55b, 0xce96, 0xffff, 
	0xffff, 0x0192, 0xfe73, 0xf8aa, 0xf58d, 0xf86c, 0xfb4c, 0xfb4c, 0xfb4c, 0xfb4c, 0xfb4c, 0xfc08, 0xfda6, 0xff43, 0xffff, 0x0011, 
	0x0011, 0x013e, 0x0659, 0x0c8f, 0x0f96, 0x0c6a, 0x093f, 0x093f, 0x093f, 0x093f, 0x093f, 0x07d0, 0x04a8, 0x0180, 0x0011, 0xc82c, 
	0xc82c, 0xc841, 0xc8d0, 0xcc70, 0xd64f, 0xed43, 0xfd30, 0xfd30, 0xfd30, 0xfd30, 0xfd30, 0xf4e8, 0xe2ae, 0xd075, 0xc82c, 0xbcc5, 
	0xbcc5, 0xbcc5, 0xbc45, 0xbbc4, 0xbd2c, 0xbfc7, 0xc12e, 0xc12e, 0xc12e, 0xc12e, 0xc12e, 0xc07d, 0xbefa, 0xbd76, 0xbcc5, 0x0000, 
	0x0000, 0x0000, 0x0050, 0x0282, 0x0924, 0x1186, 0x15a6, 0x15a6, 0x15a6, 0x15a6, 0x15a6, 0x1244, 0x0ad3, 0x0362, 0x0000, 0x4000, 
	0x4000, 0x4000, 0x3f9f, 0x3f3f, 0x3fc2, 0x40b6, 0x4139, 0x4139, 0x4139, 0x4139, 0x4139, 0x4108, 0x409c, 0x4031, 0x4000, 0xdd7e, 
	0xddb2, 0xddb2, 0xe169, 0xe299, 0xe5ed, 0xe4d2, 0xe392, 0xe37a, 0xe37a, 0xe37a, 0xe37a, 0xe283, 0xe062, 0xde42, 0xdd4a, 0x1a40, 
	0x1a4d, 0x1a4d, 0x1ab8, 0x1ab9, 0x17e9, 0x1532, 0x1455, 0x1473, 0x1473, 0x1473, 0x1473, 0x1559, 0x1753, 0x194d, 0x1a33, 0xa26a, 
	0xa28a, 0xa28a, 0xa640, 0xab39, 0xb20a, 0xb546, 0xb5bc, 0xb5ab, 0xb5ab, 0xb5ab, 0xb5ab, 0xb2a4, 0xabfb, 0xa552, 0xa24a, 0xfffc, 
	0xfff9, 0xfff9, 0x01b5, 0x072d, 0x0f9e, 0x17a7, 0x1b3c, 0x1b3c, 0x1b3c, 0x1b3c, 0x1b3c, 0x16fb, 0x0d9e, 0x0441, 0xffff, 0x00ba, 
	0x0173, 0x0173, 0x0371, 0x01c6, 0x0262, 0x01bb, 0x0113, 0x0113, 0x0113, 0x0113, 0x0113, 0x00e8, 0x0089, 0x002b, 0xffff, 0x3320, 
	0x3291, 0x3291, 0x3029, 0x2b58, 0x207e, 0x1473, 0x0ec5, 0x0ec5, 0x0ec5, 0x0ec5, 0x0ec5, 0x1489, 0x213a, 0x2dea, 0x33af, 0x0000, 
	0x0000, 0x0000, 0x00dc, 0x0245, 0x0319, 0x04c0, 0x070e, 0x06da, 0x06da, 0x06da, 0x06da, 0x05c8, 0x036d, 0x0112, 0x0000, 0x0000, 
	0x0000, 0x0000, 0xfebe, 0xfbb8, 0xf81c, 0xf565, 0xf496, 0xf489, 0xf489, 0xf489, 0xf489, 0xf654, 0xfa44, 0xfe35, 0x0000, 0x3de2, 
	0x3de2, 0x3de2, 0x3d29, 0x3bf8, 0x3b40, 0x39ee, 0x3822, 0x3849, 0x3849, 0x3849, 0x3849, 0x3929, 0x3b16, 0x3d02, 0x3de2, 0xfcca, 
	0xfcfc, 0xfcfc, 0xfd78, 0xfea1, 0x0005, 0x012e, 0x01aa, 0x00da, 0x00da, 0x00da, 0x00da, 0x0030, 0xfeb8, 0xfd42, 0xfc98, 0x0058, 
	0x007e, 0x007e, 0x00f4, 0x020c, 0x035a, 0x0473, 0x04e8, 0x0404, 0x0404, 0x0404, 0x0404, 0x036b, 0x021a, 0x00ca, 0x0031, 0x1d34, 
	0x1caa, 0x1caa, 0x1beb, 0x1a22, 0x1801, 0x1638, 0x1579, 0x16df, 0x16df, 0x16df, 0x16df, 0x17f2, 0x1a4f, 0x1cab, 0x1dbe, 0xfcd7, 
	0xfcf3, 0xfcbb, 0xfd2e, 0xfe40, 0xff86, 0x0099, 0x010c, 0x010c, 0x010c, 0x010c, 0x010c, 0x005f, 0xfee3, 0xfd67, 0xfcbb, 0x290c, 
	0x290c, 0x290c, 0x298d, 0x2ac1, 0x2c30, 0x2d64, 0x2de5, 0x2de5, 0x2de5, 0x2de5, 0x2de5, 0x2d23, 0x2b78, 0x29ce, 0x290c, 0x9eb3, 
	0x9ee8, 0x9e7f, 0x9ebe, 0x9f54, 0xa007, 0xa09d, 0xa0dc, 0xa0dc, 0xa0dc, 0xa0dc, 0xa0dc, 0xa07d, 0x9fad, 0x9ede, 0x9e7f, 0xffff, 
	0xfffd, 0xfffc, 0x000d, 0x0033, 0x0061, 0x0087, 0x0097, 0x0071, 0x0071, 0x0071, 0x0071, 0x0060, 0x0039, 0x0012, 0xffff, 0x0017, 
	0x0038, 0x0077, 0x01a8, 0x0391, 0x05a2, 0x074a, 0x07f7, 0x07c7, 0x07c7, 0x07c7, 0x07c7, 0x0690, 0x03e3, 0x0137, 0xffff, 0x4374, 
	0x43ab, 0x4416, 0x47ac, 0x4e68, 0x5605, 0x5c40, 0x5ed5, 0x5e3a, 0x5e3a, 0x5e3a, 0x5e3a, 0x5a04, 0x50c2, 0x477f, 0x434a, 0x0000, 
	0xffff, 0x0000, 0xffd2, 0xff5c, 0xfeb3, 0xfdef, 0xfd29, 0xfcac, 0xfcac, 0xfcac, 0xfcac, 0xfd31, 0xfe56, 0xff7a, 0xffff, 0x0000, 
	0x0000, 0x0000, 0x00a2, 0x0223, 0x03ef, 0x0570, 0x0612, 0x05d7, 0x05d7, 0x05d7, 0x05d7, 0x04ed, 0x02eb, 0x00ea, 0xffff, 0x3de2, 
	0x3de2, 0x3de2, 0x3be7, 0x36fc, 0x30a6, 0x2a6b, 0x25d2, 0x2428, 0x2428, 0x2428, 0x2428, 0x282d, 0x3105, 0x39dd, 0x3de2, };

JointIndex gStalfosStabAnimJointIndices[61] = {
	{ 0x0000, 0x001f, 0x002f, },
	{ 0x003f, 0x004f, 0x005f, },
	{ 0x0001, 0x0002, 0x0003, },
	{ 0x006f, 0x007f, 0x008f, },
	{ 0x009f, 0x00af, 0x00bf, },
	{ 0x00cf, 0x00df, 0x00ef, },
	{ 0x00ff, 0x010f, 0x011f, },
	{ 0x0001, 0x0003, 0x0003, },
	{ 0x0001, 0x0001, 0x0004, },
	{ 0x012f, 0x013f, 0x014f, },
	{ 0x0003, 0x0001, 0x0001, },
	{ 0x0001, 0x0003, 0x0003, },
	{ 0x0001, 0x0003, 0x0003, },
	{ 0x0003, 0x0003, 0x0001, },
	{ 0x0001, 0x0003, 0x0003, },
	{ 0x0001, 0x0003, 0x0003, },
	{ 0x015f, 0x016f, 0x017f, },
	{ 0x018f, 0x019f, 0x01af, },
	{ 0x0003, 0x0003, 0x0001, },
	{ 0x0005, 0x0006, 0x0003, },
	{ 0x0007, 0x0008, 0x0009, },
	{ 0x0001, 0x0003, 0x0001, },
	{ 0x000a, 0x0001, 0x0005, },
	{ 0x000b, 0x000c, 0x000d, },
	{ 0x0001, 0x0001, 0x000e, },
	{ 0x000f, 0x0010, 0x0011, },
	{ 0x0001, 0x0003, 0x0001, },
	{ 0x0001, 0x0003, 0x0001, },
	{ 0x0003, 0x0001, 0x0001, },
	{ 0x0003, 0x0003, 0x0003, },
	{ 0x01bf, 0x01cf, 0x01df, },
	{ 0x01ef, 0x01ff, 0x020f, },
	{ 0x021f, 0x022f, 0x023f, },
	{ 0x024f, 0x025f, 0x026f, },
	{ 0x0003, 0x0003, 0x0001, },
	{ 0x0003, 0x0003, 0x0001, },
	{ 0x0003, 0x0003, 0x0001, },
	{ 0x0003, 0x0001, 0x0003, },
	{ 0x0003, 0x0003, 0x0003, },
	{ 0x0012, 0x0013, 0x0014, },
	{ 0x0003, 0x0001, 0x0006, },
	{ 0x0001, 0x0003, 0x0001, },
	{ 0x027f, 0x028f, 0x029f, },
	{ 0x02af, 0x02bf, 0x02cf, },
	{ 0x02df, 0x02ef, 0x02ff, },
	{ 0x0001, 0x0001, 0x0015, },
	{ 0x030f, 0x031f, 0x032f, },
	{ 0x0016, 0x0017, 0x0018, },
	{ 0x0001, 0x0001, 0x0003, },
	{ 0x0001, 0x0001, 0x0015, },
	{ 0x0003, 0x0001, 0x0019, },
	{ 0x033f, 0x034f, 0x035f, },
	{ 0x036f, 0x037f, 0x038f, },
	{ 0x039f, 0x03af, 0x03bf, },
	{ 0x001a, 0x0001, 0x0015, },
	{ 0x03cf, 0x03df, 0x03ef, },
	{ 0x001b, 0x001c, 0x001d, },
	{ 0x0001, 0x0003, 0x0001, },
	{ 0x001a, 0x0001, 0x0015, },
	{ 0x001a, 0x0003, 0x001e, },
	{ 0x0001, 0x0003, 0x0001, },
};

AnimationHeader gStalfosStabAnim = { { 16 }, gStalfosStabAnimFrameData, gStalfosStabAnimJointIndices, 31 };

s16 gStalfosSpinAttackAnimFrameData[685] = {
	0xff66, 0xed0e, 0xff31, 0x3ff4, 0x0528, 0xfc12, 0x1797, 0x0000, 0xffff, 0xbf70, 0x4286, 0x4000, 0xbd79, 0xbfff, 0xf8cb, 0x0c78, 
	0xfad2, 0xf070, 0x52dd, 0xee9d, 0xbcc5, 0x3de2, 0xe797, 0xfc80, 0x6824, 0x01e7, 0xfcd5, 0xfcbb, 0x290c, 0x9e7f, 0xfff9, 0x0341, 
	0x09c8, 0x7b9c, 0xfcd4, 0x0944, 0x0944, 0x0980, 0x0980, 0x0944, 0x0980, 0x0980, 0x0980, 0x0980, 0x0980, 0xfd5f, 0xfd8b, 0xfdc4, 
	0xfdc4, 0xfd8b, 0xfdc4, 0xfe02, 0xfe02, 0xfe02, 0xfe02, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xff2c, 
	0xff2c, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xfa31, 0xfa31, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0xfe5f, 0xfe5f, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xfbca, 0x0000, 0x0000, 0x4000, 
	0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x3cfa, 0x4000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0xfa94, 0x0000, 0x0000, 0x09c3, 0x0b48, 0x145a, 0x2ae9, 0x33d1, 0x3509, 0x2e89, 0x2964, 0x28eb, 0x2964, 0xf3db, 0xf34b, 0xf196, 
	0xeeaa, 0xee24, 0xee06, 0xee22, 0xf116, 0xeffa, 0xf116, 0x18d3, 0x1899, 0x1878, 0x138e, 0x1319, 0x12d9, 0x10c9, 0x12cc, 0x14ae, 
	0x12cc, 0x0031, 0x0031, 0x0031, 0x0031, 0x0031, 0x0031, 0x0031, 0xf8b2, 0xf17f, 0xeeab, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 
	0x0009, 0x0009, 0xfc2e, 0xf8cc, 0xf6a7, 0xd1ce, 0xd1ce, 0xd1ce, 0xd1ce, 0xd1ce, 0xd1ce, 0xd1ce, 0xd105, 0xd031, 0xd0e4, 0x0000, 
	0x0000, 0x0000, 0xffd0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0x0032, 0x0012, 0x0032, 0x0032, 0x0032, 
	0x0032, 0x0032, 0x0032, 0xc877, 0xc877, 0xc9d1, 0xcadb, 0xc9d1, 0xc9d1, 0xc9d1, 0xc9d1, 0xc9d1, 0xc9d1, 0xfff4, 0xff89, 0xff89, 
	0xff89, 0xff89, 0xff89, 0xff89, 0xff89, 0xff89, 0xff89, 0x0007, 0x0053, 0x0053, 0x0053, 0x0053, 0x0053, 0x0053, 0x0053, 0x0053, 
	0x0053, 0x0026, 0x01a1, 0x01a1, 0x01a1, 0x01a1, 0x01a1, 0x01a1, 0x01a1, 0x01a1, 0x01a1, 0xfe63, 0xfe63, 0xfe92, 0xfe92, 0xfe92, 
	0xfe92, 0xfe92, 0xfe92, 0xfe92, 0xfe92, 0xf840, 0xf840, 0x0746, 0x0746, 0x0746, 0x0746, 0x0746, 0x0746, 0x0746, 0x0746, 0x03b6, 
	0x03b6, 0x05f2, 0x05f2, 0x05f2, 0x05f2, 0x05f2, 0x05f2, 0x05f2, 0x05f2, 0xff97, 0xff97, 0xff97, 0xff97, 0x01c2, 0xfc93, 0xfc93, 
	0xfc93, 0xfc93, 0xfc93, 0xffff, 0xffff, 0xffff, 0xffff, 0x03b9, 0x0288, 0x0288, 0x0288, 0x0288, 0x0288, 0xbd79, 0xbd79, 0xbd79, 
	0xbd79, 0xc062, 0xc26f, 0xc26f, 0xc26f, 0xc26f, 0xc26f, 0x0099, 0xfe80, 0xee61, 0xee61, 0xee61, 0xee61, 0xee61, 0xee61, 0xee61, 
	0xee61, 0xd3e0, 0xd565, 0xdc18, 0xdc18, 0xdc18, 0xdc18, 0xdc18, 0xdc18, 0xdc18, 0xdc18, 0xc8a6, 0xccd1, 0xdb88, 0xdb88, 0xdb88, 
	0xdb88, 0xdb88, 0xdb88, 0xdb88, 0xdb88, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 0xfda8, 0xfda8, 0xfda8, 0xfda8, 0xfda8, 0x0000, 
	0x0000, 0xffff, 0xffff, 0xffff, 0x00df, 0x00df, 0x00df, 0x00df, 0x00df, 0xb69b, 0xb69b, 0xb69b, 0xb69b, 0xb69b, 0xb8ba, 0xb8ba, 
	0xb8ba, 0xb8ba, 0xb8ba, 0xe6c4, 0xeb50, 0xeb50, 0xeb50, 0xeb50, 0xeb50, 0xeb50, 0xeb50, 0xeb50, 0xeb50, 0xe22e, 0xe33a, 0xe33a, 
	0xe33a, 0xe33a, 0xe33a, 0xe33a, 0xe33a, 0xe33a, 0xe33a, 0xadf2, 0xaf07, 0xaf07, 0xaf07, 0xaf07, 0xaf07, 0xaf07, 0xaf07, 0xaf07, 
	0xaf07, 0x00b5, 0x00b5, 0x0585, 0x0585, 0x00b5, 0x01db, 0x0585, 0x083d, 0x0ef4, 0x0ef4, 0x036f, 0x05ea, 0x0824, 0x0824, 0x05ea, 
	0x08f7, 0x0824, 0x0538, 0x0e09, 0x0e09, 0xbaa2, 0xb535, 0xb136, 0xb136, 0xb535, 0xb598, 0xb136, 0xb29b, 0xaeec, 0xaeec, 0xf8dc, 
	0xf8dc, 0xf8dc, 0xeb53, 0xeb53, 0xeb53, 0xeb53, 0xeb53, 0xeecc, 0xee38, 0x18e6, 0x18e6, 0x18e6, 0x1895, 0x1895, 0x1895, 0x1895, 
	0x1895, 0x1e9b, 0x18c9, 0xb5fa, 0xb5fa, 0xb5fa, 0xa553, 0xa553, 0xa553, 0xa553, 0xa553, 0xa0da, 0xa199, 0x028b, 0xff93, 0x0118, 
	0xf6f9, 0xf6f9, 0xf45a, 0xf502, 0xf2a2, 0xf094, 0xf094, 0xfc15, 0xf9d3, 0xf72a, 0xf8e6, 0xf8e6, 0xfb1d, 0xf9fb, 0x06e8, 0x115e, 
	0x115e, 0xcb76, 0xcba4, 0xc7c8, 0xca45, 0xca45, 0xcd25, 0xcbf2, 0xd707, 0xf1ed, 0xf1ed, 0xffa5, 0x034a, 0xfd32, 0xfed9, 0xfa91, 
	0xf672, 0xfa91, 0xf7b2, 0xfa6f, 0xfa82, 0xfedf, 0xff81, 0xfa16, 0xf33d, 0xefb5, 0xedfd, 0xefb5, 0xf5a8, 0xf10f, 0xf47f, 0xc8fd, 
	0xcd8c, 0xd228, 0xd421, 0xd6a1, 0xd7d8, 0xd6a1, 0xe0cd, 0xfe54, 0x00f9, 0xdd4a, 0xdd4a, 0xdd8e, 0xdd8e, 0xdd4a, 0xdd8e, 0xdd46, 
	0xdd46, 0xdd46, 0xdd46, 0x1a33, 0x1a33, 0x1b41, 0x1b41, 0x1a33, 0x1b41, 0x1cbd, 0x1cbd, 0x1cbd, 0x1cbd, 0xa24a, 0xa24a, 0xa716, 
	0xa716, 0xa24a, 0xa716, 0xae51, 0xae51, 0xae51, 0xae51, 0xffff, 0xffff, 0x03a2, 0x03a2, 0xffff, 0x03a2, 0x0383, 0x0383, 0x0383, 
	0x0383, 0x0000, 0x0000, 0xff5b, 0xff5b, 0x0000, 0xff5b, 0xfdec, 0xfdec, 0xfdec, 0xfdec, 0x33af, 0x33af, 0x2e98, 0x2e98, 0x33af, 
	0x2e98, 0x2b92, 0x2b92, 0x2b92, 0x2b92, 0xffff, 0xffff, 0x0164, 0x0164, 0xffff, 0x0164, 0x0164, 0x0164, 0x0164, 0x0164, 0x0000, 
	0x0000, 0xfef2, 0xfef2, 0x0000, 0xfef2, 0xfef2, 0xfef2, 0xfef2, 0xfef2, 0x01e7, 0x01e7, 0x037f, 0x037f, 0x01e7, 0x037f, 0x037f, 
	0x037f, 0x037f, 0x037f, 0xfc76, 0xfbec, 0xfa19, 0xfa19, 0xfbec, 0xfa47, 0xfa19, 0xfa19, 0xfa19, 0xfa19, 0x0018, 0xffb5, 0xfe9a, 
	0xfe9a, 0xffb5, 0xfeb3, 0xfe9a, 0xfe9a, 0xfe9a, 0xfe9a, 0x1e1b, 0x1f8e, 0x2448, 0x2448, 0x1f8e, 0x23d3, 0x2448, 0x2448, 0x2448, 
	0x2448, 0xfffc, 0xfffb, 0x0003, 0x0003, 0xfffb, 0x0001, 0x0003, 0x0003, 0x0003, 0x0003, 0x004e, 0x0155, 0xffbb, 0xffbb, 0x0155, 
	0xffed, 0xffbb, 0xffbb, 0xffbb, 0xffbb, 0x43d0, 0x4595, 0x42d5, 0x42d5, 0x4595, 0x432a, 0x42d5, 0x42d5, 0x42d5, 0x42d5, 0xffc8, 
	0xfebe, 0xfc60, 0xfc60, 0xfebe, 0xfcca, 0xfc60, 0xfc60, 0xfc60, 0xfc60, 0xfffc, 0xffe8, 0xff8e, 0xff8e, 0xffe8, 0xffa2, 0xff8e, 
	0xff8e, 0xff8e, 0xff8e, 0x3d83, 0x3bb7, 0x379b, 0x379b, 0x3bb7, 0x3853, 0x379b, 0x379b, 0x379b, 0x379b, };

JointIndex gStalfosSpinAttackAnimJointIndices[61] = {
	{ 0x0000, 0x0023, 0x002d, },
	{ 0x0037, 0x0041, 0x004b, },
	{ 0x0055, 0x005f, 0x0069, },
	{ 0x0001, 0x0002, 0x0003, },
	{ 0x0073, 0x007d, 0x0087, },
	{ 0x0004, 0x0005, 0x0006, },
	{ 0x0091, 0x009b, 0x00a5, },
	{ 0x0007, 0x0007, 0x0008, },
	{ 0x0008, 0x0007, 0x0009, },
	{ 0x00af, 0x00b9, 0x00c3, },
	{ 0x00cd, 0x00d7, 0x00e1, },
	{ 0x0007, 0x0007, 0x0008, },
	{ 0x0007, 0x0007, 0x0008, },
	{ 0x0007, 0x0007, 0x0007, },
	{ 0x0007, 0x0007, 0x0007, },
	{ 0x0007, 0x0007, 0x0007, },
	{ 0x000a, 0x000b, 0x0007, },
	{ 0x00eb, 0x00f5, 0x00ff, },
	{ 0x0008, 0x0007, 0x0007, },
	{ 0x000c, 0x000d, 0x0007, },
	{ 0x000e, 0x000f, 0x0010, },
	{ 0x0007, 0x0007, 0x0008, },
	{ 0x0109, 0x0113, 0x011d, },
	{ 0x0127, 0x0131, 0x013b, },
	{ 0x0145, 0x014f, 0x0159, },
	{ 0x0163, 0x016d, 0x0177, },
	{ 0x0008, 0x0007, 0x0008, },
	{ 0x0008, 0x0007, 0x0008, },
	{ 0x0007, 0x0007, 0x0007, },
	{ 0x0008, 0x0008, 0x0008, },
	{ 0x0181, 0x018b, 0x0195, },
	{ 0x019f, 0x01a9, 0x01b3, },
	{ 0x01bd, 0x01c7, 0x01d1, },
	{ 0x01db, 0x01e5, 0x01ef, },
	{ 0x0008, 0x0007, 0x0008, },
	{ 0x0008, 0x0007, 0x0008, },
	{ 0x0008, 0x0007, 0x0008, },
	{ 0x0007, 0x0008, 0x0007, },
	{ 0x0007, 0x0007, 0x0007, },
	{ 0x0011, 0x0012, 0x0013, },
	{ 0x0007, 0x0008, 0x000d, },
	{ 0x0008, 0x0007, 0x0008, },
	{ 0x0014, 0x0007, 0x000b, },
	{ 0x01f9, 0x0203, 0x020d, },
	{ 0x0217, 0x0221, 0x022b, },
	{ 0x0235, 0x023f, 0x0249, },
	{ 0x0007, 0x0007, 0x0015, },
	{ 0x0016, 0x0017, 0x0018, },
	{ 0x0007, 0x0007, 0x0007, },
	{ 0x0008, 0x0007, 0x0019, },
	{ 0x0007, 0x0008, 0x001a, },
	{ 0x0253, 0x025d, 0x0267, },
	{ 0x001b, 0x001c, 0x001d, },
	{ 0x0271, 0x027b, 0x0285, },
	{ 0x001e, 0x0008, 0x0019, },
	{ 0x028f, 0x0299, 0x02a3, },
	{ 0x001f, 0x0020, 0x0021, },
	{ 0x0007, 0x0007, 0x0007, },
	{ 0x001e, 0x0008, 0x0019, },
	{ 0x001e, 0x0008, 0x0022, },
	{ 0x0008, 0x0007, 0x0008, },
};

AnimationHeader gStalfosSpinAttackAnim = { { 10 }, gStalfosSpinAttackAnimFrameData, gStalfosSpinAttackAnimJointIndices, 35 };

s16 gStalfosCrouchAndSlashAnimFrameData[1375] = {
	0xff66, 0x0000, 0xed0e, 0xff31, 0x3ff4, 0xffff, 0xbf70, 0xc877, 0x4286, 0x4000, 0xfe63, 0xf840, 0x03b6, 0xbd79, 0xbfff, 0xf8cb, 
	0x0c78, 0xfad2, 0x01e7, 0xfcd5, 0xfff9, 0x0341, 0x09c8, 0x7b9c, 0xfcd4, 0x0944, 0x0788, 0x070c, 0x0675, 0x0663, 0x0663, 0x0663, 
	0x0663, 0x0663, 0x0663, 0x05f7, 0x05cc, 0x05cc, 0x05cc, 0x05cc, 0x070c, 0x0788, 0x0944, 0xfd5f, 0xfe11, 0xfeb1, 0xfeb1, 0xfeb0, 
	0xfeb0, 0xfeb0, 0xfeb0, 0xfeb0, 0xfeb0, 0xfe9e, 0xfeb0, 0xfeb0, 0xfeb0, 0xfeb0, 0xfeb1, 0xfe11, 0xfd5f, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0xfd77, 0xfd77, 0xfd77, 0xfd77, 0xfd77, 0xfd77, 0xfd77, 0xfd77, 0x0000, 0x0000, 0x0000, 0xffff, 
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0111, 0x0111, 0x0111, 0x0111, 0xf237, 0xf237, 0xf237, 0xf237, 0xf237, 0xffff, 0xffff, 
	0xffff, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x3ed0, 0x3ed0, 0x3ed0, 0x3ed0, 0x507b, 0x507b, 0x507b, 0x507b, 0x507b, 
	0x4000, 0x4000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0031, 0x0031, 0x0031, 0x0031, 0xfe6a, 0xfe6a, 0xfe6a, 
	0xfe6a, 0xfe6a, 0x0000, 0x0000, 0x0000, 0x07ea, 0x07ea, 0x0583, 0x02b4, 0x02b4, 0x02b4, 0x02b4, 0xfaaf, 0xf51f, 0xf691, 0x0194, 
	0x01ee, 0x0244, 0x027e, 0x027e, 0x0583, 0x07ea, 0x07ea, 0xf5bb, 0xf5bb, 0xf2de, 0xf0a6, 0xf0a6, 0xf0a6, 0xf0a6, 0xf3b2, 0xf775, 
	0xf6a9, 0xf208, 0xf0f5, 0xf058, 0xefa5, 0xefa5, 0xf2de, 0xf5bb, 0xf5bb, 0x1963, 0x1963, 0x2167, 0x293e, 0x293e, 0x293e, 0x293e, 
	0x296d, 0x28c4, 0x28c4, 0x303c, 0x306a, 0x308d, 0x30a9, 0x30a9, 0x2167, 0x1963, 0x1963, 0x0528, 0x0528, 0x0528, 0x0678, 0x0678, 
	0x0678, 0x0678, 0x0678, 0x0678, 0x0678, 0x0678, 0xf99e, 0xf99e, 0xf99e, 0xf99e, 0x0528, 0x0528, 0x0528, 0xfc12, 0xfc12, 0xfc12, 
	0xfea2, 0xfea2, 0xfea2, 0xfea2, 0xfea2, 0xfea2, 0xfea2, 0xfea2, 0x088b, 0x088b, 0x088b, 0x088b, 0xfc12, 0xfc12, 0xfc12, 0x1797, 
	0x1797, 0x1797, 0x088f, 0x088f, 0x088f, 0x088f, 0x088f, 0x088f, 0x088f, 0x088f, 0x0752, 0x0752, 0x0752, 0x0752, 0x1797, 0x1797, 
	0x1797, 0x0031, 0x0031, 0x0031, 0x0031, 0x0031, 0x0031, 0x0023, 0x0023, 0x0023, 0x0023, 0x0023, 0x0023, 0x0023, 0x0023, 0x0023, 
	0x0031, 0x0031, 0x0031, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0024, 0x0024, 0x0024, 0x0024, 0x0024, 0x0024, 0x0024, 
	0x0024, 0x0024, 0x0009, 0x0009, 0x0009, 0xd1ce, 0xd1ce, 0xd1ce, 0xd1ce, 0xd1ce, 0xd1ce, 0xd010, 0xd010, 0xd010, 0xd010, 0xd010, 
	0xd010, 0xd010, 0xd010, 0xd010, 0xd1ce, 0xd1ce, 0xd1ce, 0xff97, 0xff97, 0xee8f, 0xee8f, 0xee8f, 0xee8f, 0xee8f, 0xee8f, 0xee8f, 
	0xee8f, 0xee8f, 0xee8f, 0xee8f, 0xee8f, 0xee8f, 0xee8f, 0xff97, 0xff97, 0xffff, 0xffff, 0xe837, 0xe837, 0xe837, 0xe837, 0xe837, 
	0xe837, 0xe837, 0xe837, 0xe837, 0xe837, 0xe837, 0xe837, 0xe837, 0xe837, 0xffff, 0xffff, 0xbd79, 0xbd79, 0xace7, 0xace7, 0xace7, 
	0xace7, 0xace7, 0xace7, 0xace7, 0xace7, 0xace7, 0xace7, 0xace7, 0xace7, 0xace7, 0xace7, 0xbd79, 0xbd79, 0x0099, 0x0cd3, 0xfec2, 
	0x1df6, 0x1c90, 0x1c90, 0x1c90, 0x1c90, 0x1c90, 0x1c90, 0x1c90, 0x1eed, 0x1eed, 0x1eed, 0x1eed, 0xfec2, 0x0cd3, 0x0099, 0xd3e0, 
	0xd296, 0xdeb6, 0xd469, 0xd4cf, 0xd4cf, 0xd4cf, 0xd4cf, 0xd4cf, 0xd4cf, 0xd4cf, 0xcf35, 0xcf35, 0xcf35, 0xcf35, 0xdeb6, 0xd296, 
	0xd3e0, 0xc8a6, 0xbd49, 0xc959, 0xa27d, 0xa326, 0xa326, 0xa326, 0xa326, 0xa326, 0xa326, 0xa326, 0x98ab, 0x98ab, 0x98ab, 0x98ab, 
	0xc959, 0xbd49, 0xc8a6, 0xffff, 0xff6d, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 
	0xffff, 0xffff, 0x0000, 0xff6d, 0xffff, 0xffff, 0xff21, 0x0000, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0x0000, 0xffff, 
	0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0xff21, 0xffff, 0xb69b, 0xb852, 0xb69b, 0xb69b, 0xb69b, 0xb69b, 0xb69b, 0xb69b, 0xb69b, 
	0xb69b, 0xb69b, 0xb69b, 0xb69b, 0xb69b, 0xb69b, 0xb69b, 0xb852, 0xb69b, 0xe619, 0xe619, 0xea11, 0xebb4, 0xebb4, 0xebb4, 0xebb4, 
	0xebb4, 0xebb4, 0xebb4, 0xebb4, 0xebb4, 0xebb4, 0xebb4, 0xebb4, 0xea11, 0xe619, 0xe619, 0xe205, 0xe205, 0xcf29, 0xc72a, 0xc72a, 
	0xc72a, 0xc72a, 0xc72a, 0xc72a, 0xc72a, 0xc72a, 0xc72a, 0xc72a, 0xc72a, 0xc72a, 0xcf29, 0xe205, 0xe205, 0xadcc, 0xadcc, 0xba4b, 
	0xc81a, 0xc81a, 0xc81a, 0xc81a, 0xc81a, 0xc81a, 0xc81a, 0xc81a, 0xc81a, 0xc81a, 0xc81a, 0xc81a, 0xba4b, 0xadcc, 0xadcc, 0x0000, 
	0x0268, 0x026d, 0xffff, 0xffff, 0xffff, 0x0054, 0xf876, 0xf876, 0xf876, 0xf86c, 0x1965, 0x1965, 0x1965, 0x1965, 0x026d, 0x0268, 
	0x0000, 0xffff, 0xff84, 0xff92, 0xffff, 0xffff, 0xffff, 0xf6ad, 0xf337, 0xf337, 0xf337, 0xf137, 0x2891, 0x2891, 0x2891, 0x2891, 
	0xff92, 0xff84, 0xffff, 0xbd79, 0xc88a, 0xca39, 0xbd79, 0xbd79, 0xbd79, 0xc4b3, 0xd3ca, 0xd3ca, 0xd3ca, 0xd32f, 0xacde, 0xacde, 
	0xacde, 0xacde, 0xca39, 0xc88a, 0xbd79, 0xf8dc, 0xf8dc, 0xe98f, 0xeec4, 0xeec4, 0xeec4, 0xeec4, 0xef1e, 0xe636, 0x457a, 0x457d, 
	0xe4e2, 0xe406, 0xe94f, 0xe94f, 0xe98f, 0xf8dc, 0xf8dc, 0x18e6, 0x18e6, 0x1a02, 0x1766, 0x1766, 0x1766, 0x1766, 0x20b8, 0x26ef, 
	0x5dd3, 0x653d, 0x18c2, 0x1843, 0x1420, 0x1420, 0x1a02, 0x18e6, 0x18e6, 0xb5fa, 0xb5fa, 0xb02d, 0xb23a, 0xb23a, 0xb23a, 0xb23a, 
	0xaeb8, 0xa39b, 0x0474, 0xf5d3, 0xad43, 0xab66, 0xb187, 0xb187, 0xb02d, 0xb5fa, 0xb5fa, 0x0000, 0x0000, 0x009b, 0xffc6, 0xffc6, 
	0xffc6, 0xffc6, 0xf92d, 0x0082, 0x0da0, 0x0a96, 0xe9f4, 0xe9f4, 0xe88a, 0xe88a, 0x009b, 0x0000, 0x0000, 0x0000, 0xffff, 0x082e, 
	0x019c, 0x019c, 0x019c, 0x019c, 0x18d6, 0x1494, 0x142c, 0x08a6, 0x0396, 0x0396, 0x0ec6, 0x0ec6, 0x082e, 0xffff, 0x0000, 0xce96, 
	0xce96, 0xd035, 0xd732, 0xd732, 0xd732, 0xd732, 0xcbeb, 0xd998, 0xe684, 0xe066, 0xfab9, 0xfab9, 0xfae1, 0xfae1, 0xd035, 0xce96, 
	0xce96, 0xffff, 0x0000, 0xeb72, 0xe704, 0xe71b, 0xe71b, 0xfe3c, 0x3da8, 0x51e9, 0x54a3, 0x8072, 0x9d48, 0x96fc, 0x9408, 0x9408, 
	0xeb72, 0x0000, 0xffff, 0x0011, 0x0011, 0x0e4d, 0x1652, 0x1682, 0x1682, 0x14e3, 0x1935, 0x189f, 0x1801, 0x30a7, 0x13c6, 0x1aab, 
	0x1c66, 0x1c66, 0x0e4d, 0x0011, 0x0011, 0xc82c, 0xc82c, 0xe0d3, 0xd5e4, 0xd607, 0xd607, 0xc99f, 0xd788, 0xde92, 0xdcf4, 0x08ba, 
	0x165a, 0x0c25, 0x0869, 0x0869, 0xe0d3, 0xc82c, 0xc82c, 0x0000, 0xffff, 0xff9b, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xffff, 0xfdc6, 0xfdc6, 0xfdc6, 0xfdc6, 0xff9b, 0xffff, 0x0000, 0x0000, 0xffff, 0x03d3, 0xffff, 0xffff, 0xffff, 0xffff, 
	0xffff, 0x0000, 0x0000, 0x0000, 0xf811, 0xf811, 0xf811, 0xf811, 0x03d3, 0xffff, 0x0000, 0xffff, 0xffff, 0xfe49, 0x0000, 0xffff, 
	0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 0x0000, 0xff47, 0xff47, 0xff47, 0xff47, 0xfe49, 0xffff, 0xffff, 0xf070, 0xf070, 0xe3bd, 
	0xe3bd, 0xe3bd, 0xe3bd, 0xe3bd, 0xe3bd, 0xe3bd, 0xe3bd, 0xe3bd, 0xe3bd, 0xe3bd, 0xe3bd, 0xe3bd, 0xe3bd, 0xf070, 0xf070, 0x52dd, 
	0x52dd, 0x56d2, 0x56d2, 0x56d2, 0x56d2, 0x56d2, 0x56d2, 0x56d2, 0x56d2, 0x56d2, 0x56d2, 0x56d2, 0x56d2, 0x56d2, 0x56d2, 0x52dd, 
	0x52dd, 0xee9d, 0xee9d, 0xe381, 0xe381, 0xe381, 0xe381, 0xe381, 0xe381, 0xe381, 0xe381, 0xe381, 0xe381, 0xe381, 0xe381, 0xe381, 
	0xe381, 0xee9d, 0xee9d, 0xbcc5, 0xbb81, 0xbb81, 0xbb81, 0xbb81, 0xbb81, 0xbb81, 0xbb81, 0xbb81, 0xbb81, 0xbb81, 0xbb81, 0xbb81, 
	0xbb81, 0xbb81, 0xbb81, 0xbb81, 0xbcc5, 0x0000, 0x004c, 0x004c, 0x004c, 0x004c, 0x004c, 0x004c, 0x004c, 0x004c, 0x004c, 0x004c, 
	0x004c, 0x004c, 0x004c, 0x004c, 0x004c, 0x004c, 0x0000, 0x4000, 0x42db, 0x42db, 0x42db, 0x42db, 0x42db, 0x42db, 0x42db, 0x42db, 
	0x42db, 0x42db, 0x42db, 0x42db, 0x42db, 0x42db, 0x42db, 0x42db, 0x4000, 0xdd4a, 0xdd08, 0xd8be, 0xdefb, 0xe156, 0xe156, 0xe156, 
	0xe7f1, 0xe7f1, 0xe7f1, 0xd80a, 0xd80a, 0xd80a, 0xd80a, 0xd80a, 0xd8be, 0xdd08, 0xdd4a, 0x1a33, 0x1ad7, 0x23d1, 0x26b0, 0x2722, 
	0x2722, 0x2722, 0x27fe, 0x27fe, 0x27fe, 0x24df, 0x24df, 0x24df, 0x24df, 0x24df, 0x23d1, 0x1ad7, 0x1a33, 0xa24a, 0xa480, 0x999b, 
	0xa10c, 0xa2fb, 0xa2fb, 0xa2fb, 0xa878, 0xa878, 0xa878, 0x9b73, 0x9b73, 0x9b73, 0x9b73, 0x9b73, 0x999b, 0xa480, 0xa24a, 0xffff, 
	0xff88, 0xffa5, 0xffd4, 0x0034, 0x0034, 0x0034, 0x0034, 0x0034, 0x0034, 0xfcf5, 0xfcf5, 0xfcf5, 0xfcf5, 0xfcf5, 0xffa5, 0xff88, 
	0xffff, 0x0000, 0xf8e3, 0xf954, 0xf9a6, 0xfab3, 0xfab3, 0xfab3, 0xfab3, 0xfab3, 0xfab3, 0xf3f1, 0xf3f1, 0xf3f1, 0xf3f1, 0xf3f1, 
	0xf954, 0xf8e3, 0x0000, 0x33af, 0x393b, 0x389a, 0x382f, 0x36c5, 0x36c5, 0x36c5, 0x36c5, 0x36c5, 0x36c5, 0x409e, 0x409e, 0x409e, 
	0x409e, 0x409e, 0x389a, 0x393b, 0x33af, 0xffff, 0x00c8, 0xf519, 0xf519, 0xf519, 0xf519, 0xf519, 0xf519, 0xf519, 0xf54b, 0xf519, 
	0xf519, 0xf519, 0xf519, 0xf519, 0xf519, 0x00c8, 0xffff, 0x0000, 0xfea4, 0x038c, 0x038c, 0x038c, 0x038c, 0x038c, 0x038c, 0x038c, 
	0x1410, 0x038c, 0x038c, 0x038c, 0x038c, 0x038c, 0x038c, 0xfea4, 0x0000, 0x01e7, 0x0026, 0xfc6d, 0xfc6d, 0xfc6d, 0xfc6d, 0xfc6d, 
	0xfc6d, 0xfc6d, 0x08fc, 0xfc6d, 0xfc6d, 0xfc6d, 0xfc6d, 0xfc6d, 0xfc6d, 0x0026, 0x01e7, 0x0000, 0x0abd, 0x0abd, 0x0f73, 0x0e97, 
	0x0e97, 0x0e97, 0x0e97, 0x0e97, 0x0e97, 0x2856, 0x2856, 0x2856, 0x2856, 0x2856, 0x0abd, 0x0abd, 0x0000, 0x0000, 0x0130, 0x0130, 
	0x01ec, 0x01c2, 0x01c2, 0x01c2, 0x01c2, 0x01c2, 0x01c2, 0xfb6b, 0xfb6b, 0xfb6b, 0xfb6b, 0xfb6b, 0x0130, 0x0130, 0x0000, 0x3de2, 
	0x35a8, 0x35a8, 0x323f, 0x32dd, 0x32dd, 0x32dd, 0x32dd, 0x32dd, 0x32dd, 0x5639, 0x5639, 0x5639, 0x5639, 0x5639, 0x35a8, 0x35a8, 
	0x3de2, 0xe797, 0xe797, 0xe797, 0xe797, 0xe797, 0xe797, 0xe797, 0xe797, 0xe797, 0xe797, 0xe6b7, 0xe6b7, 0xe6b7, 0xe6b7, 0xe6b7, 
	0xe797, 0xe797, 0xe797, 0xfc80, 0xfc80, 0xfc80, 0xfc80, 0xfc80, 0xfc80, 0xfc80, 0xfc80, 0xfc80, 0xfc80, 0xf7e9, 0xf7e9, 0xf7e9, 
	0xf7e9, 0xf7e9, 0xfc80, 0xfc80, 0xfc80, 0x6824, 0x6824, 0x6824, 0x6824, 0x6824, 0x6824, 0x6824, 0x6824, 0x6824, 0x6824, 0x6199, 
	0x6199, 0x6199, 0x6199, 0x6199, 0x6824, 0x6824, 0x6824, 0xfc98, 0x02b8, 0xfea4, 0xf4da, 0xf397, 0xf397, 0xf397, 0xf397, 0xf397, 
	0xf397, 0xf103, 0xf103, 0xf103, 0xf103, 0xf103, 0xfea4, 0x02b8, 0xfc98, 0x0031, 0x0521, 0x0f72, 0x11aa, 0x115e, 0x115e, 0x115e, 
	0x115e, 0x115e, 0x115e, 0x1526, 0x1526, 0x1526, 0x1526, 0x1526, 0x0f72, 0x0521, 0x0031, 0x1dbe, 0x0db5, 0xf85a, 0xe7b3, 0xe742, 
	0xe742, 0xe742, 0xe742, 0xe742, 0xe742, 0xdc55, 0xdc55, 0xdc55, 0xdc55, 0xdc55, 0xf85a, 0x0db5, 0x1dbe, 0xfcbb, 0xfcbb, 0x0703, 
	0x0703, 0x05b7, 0x05b7, 0x05b7, 0x0a66, 0x0a66, 0x0a66, 0x109e, 0x0a66, 0x0a66, 0x0a66, 0x0a66, 0x0703, 0xfcbb, 0xfcbb, 0x290c, 
	0x290c, 0x2c67, 0x2c67, 0x2c8e, 0x2c8e, 0x2c8e, 0x2ce6, 0x2ce6, 0x2ce6, 0x2cd1, 0x2ce5, 0x2ce5, 0x2ce5, 0x2ce5, 0x2c67, 0x290c, 
	0x290c, 0x9e7f, 0x9e7f, 0xaa99, 0xaa99, 0xa847, 0xa847, 0xa847, 0xaed7, 0xaed7, 0xaed7, 0xb782, 0xaed7, 0xaed7, 0xaed7, 0xaed7, 
	0xaa99, 0x9e7f, 0x9e7f, 0xffff, 0x00ee, 0x019b, 0x07b5, 0x07fd, 0x07fd, 0x07fd, 0x07fd, 0x07fd, 0x07fd, 0x0832, 0x098f, 0x098f, 
	0x098f, 0x098f, 0x019b, 0x00ee, 0xffff, 0xffff, 0x0784, 0x1117, 0x1647, 0x1670, 0x1670, 0x1670, 0x1670, 0x1670, 0x1670, 0x166d, 
	0x164b, 0x164b, 0x164b, 0x164b, 0x1117, 0x0784, 0xffff, 0x434a, 0x5080, 0x593e, 0x66c0, 0x674d, 0x674d, 0x674d, 0x674d, 0x674d, 
	0x674d, 0x67c0, 0x6aba, 0x6aba, 0x6aba, 0x6aba, 0x593e, 0x5080, 0x434a, 0x0000, 0xffff, 0x0218, 0xfdab, 0xfdab, 0xfdab, 0xfdab, 
	0xfdab, 0xfdab, 0xfdab, 0xfea1, 0xfec9, 0xfec9, 0xfec9, 0xfec9, 0x0218, 0xffff, 0x0000, 0xffff, 0x0000, 0xff8f, 0x007f, 0x007f, 
	0x007f, 0x007f, 0x007f, 0x007f, 0x007f, 0x00d2, 0x00dc, 0x00dc, 0x00dc, 0x00dc, 0xff8f, 0x0000, 0xffff, 0x3de2, 0x3de2, 0x3cb0, 
	0x3f32, 0x3f32, 0x3f32, 0x3f32, 0x3f32, 0x3f32, 0x3f32, 0x44c4, 0x45a6, 0x45a6, 0x45a6, 0x45a6, 0x3cb0, 0x3de2, 0x3de2, };

JointIndex gStalfosCrouchAndSlashAnimJointIndices[61] = {
	{ 0x0000, 0x0019, 0x002b, },
	{ 0x003d, 0x0001, 0x0001, },
	{ 0x004f, 0x0061, 0x0073, },
	{ 0x0002, 0x0003, 0x0004, },
	{ 0x0085, 0x0097, 0x00a9, },
	{ 0x00bb, 0x00cd, 0x00df, },
	{ 0x00f1, 0x0103, 0x0115, },
	{ 0x0005, 0x0001, 0x0001, },
	{ 0x0005, 0x0005, 0x0006, },
	{ 0x0001, 0x0001, 0x0007, },
	{ 0x0001, 0x0005, 0x0005, },
	{ 0x0005, 0x0001, 0x0001, },
	{ 0x0005, 0x0001, 0x0001, },
	{ 0x0001, 0x0001, 0x0005, },
	{ 0x0005, 0x0001, 0x0001, },
	{ 0x0005, 0x0001, 0x0001, },
	{ 0x0008, 0x0009, 0x0001, },
	{ 0x000a, 0x000b, 0x000c, },
	{ 0x0001, 0x0001, 0x0005, },
	{ 0x000d, 0x000e, 0x0001, },
	{ 0x000f, 0x0010, 0x0011, },
	{ 0x0005, 0x0001, 0x0005, },
	{ 0x0127, 0x0139, 0x014b, },
	{ 0x015d, 0x016f, 0x0181, },
	{ 0x0193, 0x01a5, 0x01b7, },
	{ 0x01c9, 0x01db, 0x01ed, },
	{ 0x0005, 0x0001, 0x0005, },
	{ 0x0005, 0x0001, 0x0005, },
	{ 0x0001, 0x0005, 0x0005, },
	{ 0x0001, 0x0001, 0x0001, },
	{ 0x01ff, 0x0211, 0x0223, },
	{ 0x0235, 0x0247, 0x0259, },
	{ 0x026b, 0x027d, 0x028f, },
	{ 0x02a1, 0x02b3, 0x02c5, },
	{ 0x02d7, 0x02e9, 0x02fb, },
	{ 0x0001, 0x0001, 0x0005, },
	{ 0x0001, 0x0001, 0x0005, },
	{ 0x0001, 0x0005, 0x0001, },
	{ 0x0001, 0x0001, 0x0001, },
	{ 0x030d, 0x031f, 0x0331, },
	{ 0x0001, 0x0005, 0x000e, },
	{ 0x0005, 0x0001, 0x0005, },
	{ 0x0343, 0x0355, 0x0367, },
	{ 0x0379, 0x038b, 0x039d, },
	{ 0x03af, 0x03c1, 0x03d3, },
	{ 0x03e5, 0x03f7, 0x0409, },
	{ 0x041b, 0x042d, 0x043f, },
	{ 0x0451, 0x0463, 0x0475, },
	{ 0x0001, 0x0001, 0x0001, },
	{ 0x0005, 0x0001, 0x0012, },
	{ 0x0001, 0x0005, 0x0013, },
	{ 0x0487, 0x0499, 0x04ab, },
	{ 0x04bd, 0x04cf, 0x04e1, },
	{ 0x04f3, 0x0505, 0x0517, },
	{ 0x0014, 0x0001, 0x0012, },
	{ 0x0529, 0x053b, 0x054d, },
	{ 0x0015, 0x0016, 0x0017, },
	{ 0x0001, 0x0005, 0x0005, },
	{ 0x0014, 0x0001, 0x0012, },
	{ 0x0014, 0x0005, 0x0018, },
	{ 0x0005, 0x0001, 0x0005, },
};

AnimationHeader gStalfosCrouchAndSlashAnimAnim = { { 18 }, gStalfosCrouchAndSlashAnimFrameData, gStalfosCrouchAndSlashAnimJointIndices, 25 };


static u8 sJointCopyFlags[] = {
    false, // STALFOS_LIMB_NONE
    false, // STALFOS_LIMB_ROOT
    false, // STALFOS_LIMB_UPPERBODY_ROOT
    false, // STALFOS_LIMB_CORE_LOWER_ROOT
    true,  // STALFOS_LIMB_CORE_UPPER_ROOT
    true,  // STALFOS_LIMB_NECK_ROOT
    true,  // STALFOS_LIMB_HEAD_ROOT
    true,  // STALFOS_LIMB_7
    true,  // STALFOS_LIMB_8
    true,  // STALFOS_LIMB_JAW_ROOT
    true,  // STALFOS_LIMB_JAW
    true,  // STALFOS_LIMB_HEAD
    true,  // STALFOS_LIMB_NECK_UPPER
    true,  // STALFOS_LIMB_NECK_LOWER
    true,  // STALFOS_LIMB_CORE_UPPER
    true,  // STALFOS_LIMB_CHEST
    true,  // STALFOS_LIMB_SHOULDER_R_ROOT
    true,  // STALFOS_LIMB_SHOULDER_ARMOR_R_ROOT
    true,  // STALFOS_LIMB_SHOULDER_ARMOR_R
    true,  // STALFOS_LIMB_SHOULDER_L_ROOT
    true,  // STALFOS_LIMB_SHOULDER_ARMOR_L_ROOT
    true,  // STALFOS_LIMB_SHOULDER_ARMOR_L
    true,  // STALFOS_LIMB_ARM_L_ROOT
    true,  // STALFOS_LIMB_UPPERARM_L_ROOT
    true,  // STALFOS_LIMB_FOREARM_L_ROOT
    true,  // STALFOS_LIMB_HAND_L_ROOT
    true,  // STALFOS_LIMB_HAND_L
    true,  // STALFOS_LIMB_SHIELD
    true,  // STALFOS_LIMB_FOREARM_L
    true,  // STALFOS_LIMB_UPPERARM_L
    true,  // STALFOS_LIMB_ARM_R_ROOT
    true,  // STALFOS_LIMB_UPPERARM_R_ROOT
    true,  // STALFOS_LIMB_FOREARM_R_ROOT
    true,  // STALFOS_LIMB_HAND_R_ROOT
    true,  // STALFOS_LIMB_SWORD
    true,  // STALFOS_LIMB_HAND_R
    true,  // STALFOS_LIMB_FOREARM_R
    true,  // STALFOS_LIMB_UPPERARM_R
    true,  // STALFOS_LIMB_CORE_LOWER
    false, // STALFOS_LIMB_LOWERBODY_ROOT
    false, // STALFOS_LIMB_WAIST_ROOT
    false, // STALFOS_LIMB_LEGS_ROOT
    false, // STALFOS_LIMB_LEG_L_ROOT
    false, // STALFOS_LIMB_THIGH_L_ROOT
    false, // STALFOS_LIMB_LOWERLEG_L_ROOT
    false, // STALFOS_LIMB_ANKLE_L_ROOT
    false, // STALFOS_LIMB_ANKLE_L
    false, // STALFOS_LIMB_FOOT_L_ROOT
    false, // STALFOS_LIMB_FOOT_L
    false, // STALFOS_LIMB_LOWERLEG_L
    false, // STALFOS_LIMB_THIGH_L
    false, // STALFOS_LIMB_LEG_R_ROOT
    false, // STALFOS_LIMB_THIGH_R_ROOT
    false, // STALFOS_LIMB_LOWERLEG_R_ROOT
    false, // STALFOS_LIMB_ANKLE_R_ROOT
    false, // STALFOS_LIMB_ANKLE_R
    false, // STALFOS_LIMB_FOOT_R_ROOT
    false, // STALFOS_LIMB_FOOT_R
    false, // STALFOS_LIMB_LOWERLEG_R
    false, // STALFOS_LIMB_THIGH_R
    false, // STALFOS_LIMB_WAIST
};

const ActorInit En_Test_InitVars = {
    ACTOR_EN_TEST,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SK2,
    sizeof(EnTest),
    (ActorFunc)EnTest_Init,
    (ActorFunc)EnTest_Destroy,
    (ActorFunc)EnTest_Update,
    (ActorFunc)EnTest_Draw,
    NULL,
};

static ColliderCylinderInit sBodyColliderInit = {
    {
        COLTYPE_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 25, 65, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sShieldColliderInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFC1FFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 20, 70, -50, { 0, 0, 0 } },
};

static ColliderQuadInit sSwordColliderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x20 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL | TOUCH_UNK7,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

typedef enum {
    /* 0x0 */ STALFOS_DMGEFF_NORMAL,
    /* 0x1 */ STALFOS_DMGEFF_STUN,
    /* 0x6 */ STALFOS_DMGEFF_FIREMAGIC = 6,
    /* 0xD */ STALFOS_DMGEFF_SLING = 0xD,
    /* 0xE */ STALFOS_DMGEFF_LIGHT,
    /* 0xF */ STALFOS_DMGEFF_FREEZE
} StalfosDamageEffect;

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, STALFOS_DMGEFF_STUN),
    /* Deku stick    */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Slingshot     */ DMG_ENTRY(1, STALFOS_DMGEFF_SLING),
    /* Explosive     */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Boomerang     */ DMG_ENTRY(0, STALFOS_DMGEFF_STUN),
    /* Normal arrow  */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Hammer swing  */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Hookshot      */ DMG_ENTRY(0, STALFOS_DMGEFF_STUN),
    /* Kokiri sword  */ DMG_ENTRY(1, STALFOS_DMGEFF_NORMAL),
    /* Master sword  */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Giant's Knife */ DMG_ENTRY(4, STALFOS_DMGEFF_NORMAL),
    /* Fire arrow    */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Ice arrow     */ DMG_ENTRY(4, STALFOS_DMGEFF_FREEZE),
    /* Light arrow   */ DMG_ENTRY(2, STALFOS_DMGEFF_LIGHT),
    /* Unk arrow 1   */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Unk arrow 2   */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Unk arrow 3   */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Fire magic    */ DMG_ENTRY(0, STALFOS_DMGEFF_FIREMAGIC),
    /* Ice magic     */ DMG_ENTRY(3, STALFOS_DMGEFF_FREEZE),
    /* Light magic   */ DMG_ENTRY(0, STALFOS_DMGEFF_LIGHT),
    /* Shield        */ DMG_ENTRY(0, STALFOS_DMGEFF_NORMAL),
    /* Mirror Ray    */ DMG_ENTRY(0, STALFOS_DMGEFF_NORMAL),
    /* Kokiri spin   */ DMG_ENTRY(1, STALFOS_DMGEFF_NORMAL),
    /* Giant spin    */ DMG_ENTRY(4, STALFOS_DMGEFF_NORMAL),
    /* Master spin   */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Kokiri jump   */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Giant jump    */ DMG_ENTRY(8, STALFOS_DMGEFF_NORMAL),
    /* Master jump   */ DMG_ENTRY(4, STALFOS_DMGEFF_NORMAL),
    /* Unknown 1     */ DMG_ENTRY(0, STALFOS_DMGEFF_NORMAL),
    /* Unblockable   */ DMG_ENTRY(0, STALFOS_DMGEFF_NORMAL),
    /* Hammer jump   */ DMG_ENTRY(4, STALFOS_DMGEFF_NORMAL),
    /* Unknown 2     */ DMG_ENTRY(0, STALFOS_DMGEFF_NORMAL),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x1B, ICHAIN_CONTINUE),    ICHAIN_F32(targetArrowOffset, 500, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 15, ICHAIN_CONTINUE), ICHAIN_F32(scale.y, 0, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -1500, ICHAIN_STOP),
};

#define FLAME_SWORD_PARAM 8
#define ELITE_PARAM 16
#define DARK_PARAM 32
#define IS_ELITE 1

#define VULNERABLE_IN_JUMP (this->skelAnime.curFrame >= this->skelAnime.animLength-5.0f && this->timer == 1)
#define IS_FULL_SHIELDING ((this->actionFunc == EnTest_Jumpslash && !VULNERABLE_IN_JUMP) || IS_ELITE && (this->actionFunc == EnTest_SlashDown || this->actionFunc == EnTest_SlashDownEnd || this->actionFunc == EnTest_SlashUp ||this->actionFunc == EnTest_SpinAttack || this->actionFunc == EnTest_Thrust))
#define IS_VULNERABLE ((this->actionFunc == EnTest_SlashDown && isPlayerInHorizontalAttack(play)) || (this->actionFunc == EnTest_SlashDownEnd && isPlayerInHorizontalAttack(play)) || (this->actionFunc == EnTest_SlashUp && isPlayerInHorizontalAttack(play)) ||\
                        (this->actionFunc == EnTest_SpinAttack && isPlayerInVerticalAttack(play)) || (this->actionFunc == EnTest_Crouch && isPlayerInJumpAttack(play)))

void EnTest_SetupAction(EnTest* this, EnTestActionFunc actionFunc) {
    EffectBlure* blur = Effect_GetByIndex(this->effectIndex);
    if (actionFunc == EnTest_SpinAttack) {
        blur->p1StartColor.r = blur->p1EndColor.r = blur->p2StartColor.r = blur->p2EndColor.r = 255;
        blur->p1StartColor.g = blur->p1EndColor.g = blur->p2StartColor.g = blur->p2EndColor.g = 170;
        blur->p1StartColor.b = blur->p1EndColor.b = blur->p2StartColor.b = blur->p2EndColor.b = 0;
    } else if (actionFunc == EnTest_Thrust){
        blur->p1StartColor.r = blur->p1EndColor.r = blur->p2StartColor.r = blur->p2EndColor.r = 50;
        blur->p1StartColor.g = blur->p1EndColor.g = blur->p2StartColor.g = blur->p2EndColor.g = 200;
        blur->p1StartColor.b = blur->p1EndColor.b = blur->p2StartColor.b = blur->p2EndColor.b = 0;
    } else {
        blur->p1StartColor.r = blur->p1EndColor.r = blur->p2StartColor.r = blur->p2EndColor.r = 255;
        blur->p1StartColor.g = blur->p1EndColor.g = blur->p2StartColor.g = blur->p2EndColor.g = 255;
        blur->p1StartColor.b = blur->p1EndColor.b = blur->p2StartColor.b = blur->p2EndColor.b = 255;
    }

    this->actionFunc = actionFunc;
}

void EnTest_SetDirectionIndicator(EnTest* this, s16 index) {
    EffectBlure* blur = Effect_GetByIndex(this->effectIndicatorIndex[index]);
    blur->p1StartColor.r = blur->p1EndColor.r = blur->p2StartColor.r = blur->p2EndColor.r = 140;
    blur->p1StartColor.g = blur->p1EndColor.g = blur->p2StartColor.g = blur->p2EndColor.g = 0;
    blur->p1StartColor.b = blur->p1EndColor.b = blur->p2StartColor.b = blur->p2EndColor.b = 200;
}

void EnTest_Init(Actor* thisx, PlayState* play) {
    EffectBlureInit1 slashBlure;
    EnTest* this = (EnTest*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);

    SkelAnime_Init(play, &this->skelAnime, &gStalfosSkel, &gStalfosMiddleGuardAnim, this->jointTable,
                   this->morphTable, STALFOS_LIMB_MAX);
    SkelAnime_Init(play, &this->upperSkelanime, &gStalfosSkel, &gStalfosMiddleGuardAnim, this->upperJointTable,
                   this->upperMorphTable, STALFOS_LIMB_MAX);

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawFeet, 90.0f);

    this->actor.colChkInfo.cylRadius = 40;
    this->actor.colChkInfo.cylHeight = 100;
    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += 45.0f;
    this->actor.colChkInfo.damageTable = &sDamageTable;

    Collider_InitCylinder(play, &this->bodyCollider);
    Collider_SetCylinder(play, &this->bodyCollider, &this->actor, &sBodyColliderInit);

    Collider_InitCylinder(play, &this->shieldCollider);
    Collider_SetCylinder(play, &this->shieldCollider, &this->actor, &sShieldColliderInit);

    Collider_InitQuad(play, &this->swordCollider);
    Collider_SetQuad(play, &this->swordCollider, &this->actor, &sSwordColliderInit);
    if (ABS(this->actor.params) & FLAME_SWORD_PARAM) {
        if (this->actor.params > 0)
            this->actor.params -= FLAME_SWORD_PARAM;
        else
            this->actor.params += FLAME_SWORD_PARAM;
        this->variant |= FLAME_SWORD_PARAM;
        this->swordCollider.info.toucher.dmgFlags = 0x20000000;
    }

    if (ABS(this->actor.params) & ELITE_PARAM) {
        if (this->actor.params > 0)
            this->actor.params -= ELITE_PARAM;
        else
            this->actor.params += ELITE_PARAM;
        this->variant |= ELITE_PARAM;
    }

    if (ABS(this->actor.params) & DARK_PARAM) {
        if (this->actor.params > 0)
            this->actor.params -= DARK_PARAM;
        else
            this->actor.params += DARK_PARAM;
        this->variant |= DARK_PARAM;
    }

    this->actor.colChkInfo.mass = MASS_HEAVY;
    this->actor.colChkInfo.health = 10;

    slashBlure.p1StartColor[0] = slashBlure.p1StartColor[1] = slashBlure.p1StartColor[2] = slashBlure.p1StartColor[3] =
        slashBlure.p2StartColor[0] = slashBlure.p2StartColor[1] = slashBlure.p2StartColor[2] =
            slashBlure.p1EndColor[0] = slashBlure.p1EndColor[1] = slashBlure.p1EndColor[2] = slashBlure.p2EndColor[0] =
                slashBlure.p2EndColor[1] = slashBlure.p2EndColor[2] = 255;

    slashBlure.p1EndColor[3] = 0;
    slashBlure.p2EndColor[3] = 0;
    slashBlure.p2StartColor[3] = 64;

    slashBlure.elemDuration = 4;
    slashBlure.unkFlag = 0;
    slashBlure.calcMode = 2;

    Effect_Add(play, &this->effectIndex, EFFECT_BLURE1, 0, 0, &slashBlure);
    slashBlure.elemDuration = 8;
    Effect_Add(play, &this->effectIndicatorIndex[0], EFFECT_BLURE1, 0, 0, &slashBlure);
    Effect_Add(play, &this->effectIndicatorIndex[1], EFFECT_BLURE1, 0, 0, &slashBlure);
    EnTest_SetDirectionIndicator(this, 0);
    EnTest_SetDirectionIndicator(this, 1);

    if (this->actor.params != STALFOS_TYPE_CEILING) {
        EnTest_SetupWaitGround(this);
    } else {
        EnTest_SetupWaitAbove(this);
    }

    if (this->actor.params == STALFOS_TYPE_INVISIBLE) {
        this->actor.flags |= ACTOR_FLAG_LENS;
    }
}

void EnTest_Destroy(Actor* thisx, PlayState* play) {
    EnTest* this = (EnTest*)thisx;

    if ((this->actor.params != STALFOS_TYPE_2) &&
        !Actor_FindNearby(play, &this->actor, ACTOR_EN_TEST, ACTORCAT_ENEMY, 8000.0f)) {
        func_800F5B58();
    }

    Effect_Delete(play, this->effectIndex);
    Effect_Delete(play, this->effectIndicatorIndex[0]);
    Effect_Delete(play, this->effectIndicatorIndex[1]);
    Collider_DestroyCylinder(play, &this->shieldCollider);
    Collider_DestroyCylinder(play, &this->bodyCollider);
    Collider_DestroyQuad(play, &this->swordCollider);

    ResourceMgr_UnregisterSkeleton(&this->skelAnime);
    ResourceMgr_UnregisterSkeleton(&this->upperSkelanime);
}

/**
 * If EnTest_ChooseAction failed to pick a new action, this function will unconditionally pick
 * a new action as a last resort
 */
void EnTest_ChooseRandomAction(EnTest* this, PlayState* play) {
    switch ((u32)(Rand_ZeroOne() * 10.0f)) {
        case 0:
        case 1:
        case 5:
        case 6:
            if ((this->actor.xzDistToPlayer < 220.0f) && (this->actor.xzDistToPlayer > 170.0f) &&
                Actor_IsFacingPlayer(&this->actor, 0x71C) && Actor_IsTargeted(play, &this->actor)) {
                EnTest_SetupJumpslash(this);
                break;
            }
            // fallthrough
        case 8:
            EnTest_SetupWalkAndBlock(this);
            break;

        case 3:
        case 4:
        case 7:
            func_808627C4(this, play);
            break;

        case 2:
        case 9:
        case 10:
            EnTest_SetupStopAndBlock(this);
            break;
    }
}

void EnTest_ChooseAction(EnTest* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);
    s16 yawDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;

    yawDiff = ABS(yawDiff);

    if (yawDiff >= 0x61A8) {
        switch ((u32)(Rand_ZeroOne() * 10.0f)) {
            case 0:
            case 3:
            case 7:
                EnTest_SetupStopAndBlock(this);
                break;

            case 1:
            case 5:
            case 6:
            case 8:
                func_808627C4(this, play);
                break;

            case 2:
            case 4:
            case 9:
                if (this->actor.params != STALFOS_TYPE_CEILING) {
                    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
                    EnTest_SetupJumpBack(this);
                }
                break;
        }
    } else if (yawDiff <= 0x3E80) {
        if (ABS((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y)) > 0x3E80) {
            if (((play->gameplayFrames % 2) != 0) && (this->actor.params != STALFOS_TYPE_CEILING)) {
                this->actor.world.rot.y = this->actor.yawTowardsPlayer;
                EnTest_SetupJumpBack(this);
            } else if ((this->actor.xzDistToPlayer < 220.0f) && (this->actor.xzDistToPlayer > 170.0f)) {
                if (Actor_IsFacingPlayer(&this->actor, 0x71C) && !Actor_IsTargeted(play, &this->actor)) {
                    EnTest_SetupJumpslash(this);
                }
            } else {
                EnTest_SetupWalkAndBlock(this);
            }
        } else {
            if (this->actor.xzDistToPlayer < 110.0f) {
                if (Rand_ZeroOne() > 0.2f) {
                    if (player->stateFlags1 & 0x10) {
                        if (1 || this->actor.isTargeted) {
                            EnTest_SetupSlashDown(this);
                        } else {
                            func_808627C4(this, play);
                        }
                    } else {
                        EnTest_SetupSlashDown(this);
                    }
                }
            } else {
                EnTest_ChooseRandomAction(this, play);
            }
        }
    } else {
        EnTest_ChooseRandomAction(this, play);
    }
}

void EnTest_SetupWaitGround(EnTest* this) {
    Animation_PlayLoop(&this->skelAnime, &gStalfosMiddleGuardAnim);
    this->unk_7C8 = 0;
    this->timer = 15;
    this->actor.scale.y = 0.0f;
    this->actor.world.pos.y = this->actor.home.pos.y - 3.5f;
    this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;
    EnTest_SetupAction(this, EnTest_WaitGround);
}

void EnTest_WaitGround(EnTest* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

    if ((this->timer == 0) && (ABS(this->actor.yDistToPlayer) < 150.0f)) {
        this->unk_7C8 = 3;
        EnTest_SetupAction(this, EnTest_Rise);
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        this->actor.shape.rot.y = this->actor.yawTowardsPlayer;

        if (this->actor.params != STALFOS_TYPE_2) {
            func_800F5ACC(NA_BGM_MINI_BOSS);
        }
    } else {
        if (this->timer != 0) {
            this->timer--;
        }

        this->actor.world.pos.y = this->actor.home.pos.y - 3.5f;
    }
}

void EnTest_SetupWaitAbove(EnTest* this) {
    Animation_PlayLoop(&this->skelAnime, &gStalfosMiddleGuardAnim);
    this->unk_7C8 = 0;
    this->actor.world.pos.y = this->actor.home.pos.y + 150.0f;
    Actor_SetScale(&this->actor, 0.0f);
    this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;
    EnTest_SetupAction(this, EnTest_WaitAbove);
}

void EnTest_WaitAbove(EnTest* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    this->actor.world.pos.y = this->actor.home.pos.y + 150.0f;

    if ((this->actor.xzDistToPlayer < 200.0f) && (ABS(this->actor.yDistToPlayer) < 450.0f)) {
        EnTest_SetupAction(this, EnTest_Fall);
        this->actor.flags |= ACTOR_FLAG_TARGETABLE;
        this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        Actor_SetScale(&this->actor, 0.015f);
    }
}

void EnTest_SetupIdle(EnTest* this) {
    Animation_PlayLoop(&this->skelAnime, &gStalfosMiddleGuardAnim);
    this->unk_7C8 = 0xA;
    this->timer = (Rand_ZeroOne() * 10.0f) + 5.0f;
    this->actor.speedXZ = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnTest_SetupAction(this, EnTest_Idle);
}

void EnTest_Idle(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 yawDiff;

    SkelAnime_Update(&this->skelAnime);

    if (!EnTest_ReactToProjectile(play, this)) {
        yawDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;

        if (this->actor.xzDistToPlayer < 100.0f) {
            if ((player->swordState != 0) && (ABS(yawDiff) >= 0x1F40)) {
                this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;

                if (Rand_ZeroOne() > 0.7f && player->meleeWeaponAnimation != 0x11) {
                    EnTest_SetupJumpBack(this);
                } else {
                    func_808627C4(this, play);
                }
                return;
            }
        }

        if (this->timer != 0) {
            this->timer--;
        } else {
            if (Actor_IsFacingPlayer(&this->actor, 0x1555)) {
                if ((this->actor.xzDistToPlayer < 220.0f) && (this->actor.xzDistToPlayer > 160.0f) &&
                    (Rand_ZeroOne() < 0.3f)) {
                    if (Actor_IsTargeted(play, &this->actor)) {
                        EnTest_SetupJumpslash(this);
                    } else {
                        func_808627C4(this, play);
                    }
                } else {
                    if (Rand_ZeroOne() > 0.3f) {
                        EnTest_SetupWalkAndBlock(this);
                    } else {
                        func_808627C4(this, play);
                    }
                }
            } else {
                if (Rand_ZeroOne() > 0.7f) {
                    func_80860BDC(this);
                } else {
                    EnTest_ChooseAction(this, play);
                }
            }
        }
    }
}

void EnTest_Fall(EnTest* this, PlayState* play) {
    Animation_PlayOnceSetSpeed(&this->skelAnime, &gStalfosLandFromLeapAnim, 0.0f);
    SkelAnime_Update(&this->skelAnime);

    if (this->actor.world.pos.y <= this->actor.floorHeight) {
        this->skelAnime.playSpeed = 1.0f;
        this->unk_7C8 = 0xC;
        this->timer = this->unk_7E4 * 0.15f;
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_RIZA_DOWN);
        EnTest_SetupAction(this, EnTest_Land);
    }
}

void EnTest_Land(EnTest* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        EnTest_SetupIdle(this);
        this->timer = (Rand_ZeroOne() * 10.0f) + 5.0f;
    }
}

void EnTest_SetupWalkAndBlock(EnTest* this) {
    Animation_Change(&this->upperSkelanime, &gStalfosBlockWithShieldAnim, 2.0f, 0.0f,
                     Animation_GetLastFrame(&gStalfosBlockWithShieldAnim), 2, 2.0f);
    Animation_PlayLoop(&this->skelAnime, &gStalfosSlowAdvanceAnim);
    this->timer = (s16)(Rand_ZeroOne() * 5.0f);
    this->unk_7C8 = 0xD;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnTest_SetupAction(this, EnTest_WalkAndBlock);
}

void EnTest_WalkAndBlock(EnTest* this, PlayState* play) {
    s32 pad;
    f32 checkDist = 0.0f;
    s32 pad1;
    s32 prevFrame;
    s32 temp_f16;
    f32 playSpeed;
    Player* player = GET_PLAYER(play);
    s32 absPlaySpeed;
    s16 yawDiff;

    if (!EnTest_ReactToProjectile(play, this)) {
        this->timer++;

        if (Actor_OtherIsTargeted(play, &this->actor)) {
            checkDist = 150.0f;
        }

        if (this->actor.xzDistToPlayer <= (80.0f + checkDist)) {
            Math_SmoothStepToF(&this->actor.speedXZ, -5.0f, 1.0f, 0.8f, 0.0f);
        } else if (this->actor.xzDistToPlayer > (110.0f + checkDist)) {
            Math_SmoothStepToF(&this->actor.speedXZ, 5.0f, 1.0f, 0.8f, 0.0f);
        }

        if (this->actor.speedXZ >= 5.0f) {
            this->actor.speedXZ = 5.0f;
        } else if (this->actor.speedXZ < -5.0f) {
            this->actor.speedXZ = -5.0f;
        }

        if ((this->actor.params == STALFOS_TYPE_CEILING) &&
            !Actor_TestFloorInDirection(&this->actor, play, this->actor.speedXZ, this->actor.world.rot.y)) {
            this->actor.speedXZ *= -1.0f;
        }

        if (ABS(this->actor.speedXZ) < 3.0f) {
            Animation_Change(&this->skelAnime, &gStalfosSlowAdvanceAnim, 0.0f, this->skelAnime.curFrame,
                             Animation_GetLastFrame(&gStalfosSlowAdvanceAnim), 0, -6.0f);
            playSpeed = this->actor.speedXZ * 10.0f;
        } else {
            Animation_Change(&this->skelAnime, &gStalfosFastAdvanceAnim, 0.0f, this->skelAnime.curFrame,
                             Animation_GetLastFrame(&gStalfosFastAdvanceAnim), 0, -4.0f);
            playSpeed = this->actor.speedXZ * 10.0f * 0.02f;
        }

        if (this->actor.speedXZ >= 0.0f) {
            if (this->shieldState == 0) {
                this->shieldState++;
            }

            playSpeed = CLAMP_MAX(playSpeed, 2.5f);
            this->skelAnime.playSpeed = playSpeed;
        } else {
            playSpeed = CLAMP_MIN(playSpeed, -2.5f);
            this->skelAnime.playSpeed = playSpeed;
        }

        yawDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;

        if ((this->actor.xzDistToPlayer < 100.0f) && (player->swordState != 0)) {
            if (ABS(yawDiff) >= 0x1F40) {
                this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;

                if ((Rand_ZeroOne() > 0.7f) && (player->meleeWeaponAnimation != 0x11)) {
                    EnTest_SetupJumpBack(this);
                } else {
                    EnTest_SetupStopAndBlock(this);
                }

                return;
            }
        }

        prevFrame = this->skelAnime.curFrame;
        SkelAnime_Update(&this->skelAnime);

        temp_f16 = this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed);
        absPlaySpeed = (f32)ABS(this->skelAnime.playSpeed);

        if ((s32)this->skelAnime.curFrame != prevFrame) {
            s32 temp_v0_2 = absPlaySpeed + prevFrame;

            if (((temp_v0_2 > 1) && (temp_f16 <= 0)) || ((temp_f16 < 7) && (temp_v0_2 >= 8))) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_WALK);
            }
        }

        if ((this->timer % 32) == 0) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_WARAU);
            this->timer += (s16)(Rand_ZeroOne() * 5.0f);
        }

        if ((this->actor.xzDistToPlayer < 220.0f) && (this->actor.xzDistToPlayer > 160.0f) &&
            (Actor_IsFacingPlayer(&this->actor, 0x71C))) {
            if (Actor_IsTargeted(play, &this->actor)) {
                if (Rand_ZeroOne() < 0.1f) {
                    EnTest_SetupJumpslash(this);
                    return;
                }
            } else if (player->heldItemAction != PLAYER_IA_NONE) {
                if (this->actor.isTargeted) {
                    if ((play->gameplayFrames % 2) != 0) {
                        func_808627C4(this, play);
                        return;
                    }

                    EnTest_ChooseAction(this, play);
                } else {
                    func_80860EC0(this);
                }
            }
        }

        if (Rand_ZeroOne() < 0.4f) {
            this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        }

        if (!Actor_IsFacingPlayer(&this->actor, 0x11C7)) {
            EnTest_SetupIdle(this);
            this->timer = (Rand_ZeroOne() * 10.0f) + 10.0f;
            return;
        }

        if (this->actor.xzDistToPlayer < 110.0f) {
            if (Rand_ZeroOne() > 0.2f) {
                if (player->stateFlags1 & 0x10) {
                    if (1 || this->actor.isTargeted) {
                        EnTest_SetupSlashDown(this);
                    } else {
                        func_808627C4(this, play);
                    }
                } else {
                    EnTest_SetupSlashDown(this);
                }
            } else {
                EnTest_SetupStopAndBlock(this);
            }
        } else if (Rand_ZeroOne() < 0.1f) {
            this->actor.speedXZ = 5.0f;
        }
    }
}

// a variation of sidestep
void func_80860BDC(EnTest* this) {
    Animation_PlayLoop(&this->skelAnime, &gStalfosSidestepAnim);
    this->unk_7C8 = 0xE;
    EnTest_SetupAction(this, func_80860C24);
}

// a variation of sidestep
void func_80860C24(EnTest* this, PlayState* play) {
    s16 yawDiff;
    s16 yawChange;
    f32 playSpeed;
    s32 prevFrame;
    s32 temp1;
    s32 temp2;
    s32 absPlaySpeed;

    if (!EnTest_ReactToProjectile(play, this)) {
        yawDiff = this->actor.yawTowardsPlayer;
        yawDiff -= this->actor.shape.rot.y;

        if (yawDiff > 0) {
            yawChange = (yawDiff / 42.0f) + 300.0f;
            this->actor.shape.rot.y += yawChange * 2;
        } else {
            yawChange = (yawDiff / 42.0f) - 300.0f;
            this->actor.shape.rot.y += yawChange * 2;
        }

        this->actor.world.rot.y = this->actor.shape.rot.y;

        if (yawDiff > 0) {
            playSpeed = yawChange * 0.02f;
            playSpeed = CLAMP_MAX(playSpeed, 1.0f);
            this->skelAnime.playSpeed = playSpeed;
        } else {
            playSpeed = yawChange * 0.02f;
            playSpeed = CLAMP_MIN(playSpeed, -1.0f);
            this->skelAnime.playSpeed = playSpeed;
        }

        prevFrame = this->skelAnime.curFrame;
        SkelAnime_Update(&this->skelAnime);
        temp1 = this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed);
        absPlaySpeed = (f32)ABS(this->skelAnime.playSpeed);

        if ((s32)this->skelAnime.curFrame != prevFrame) {
            temp2 = absPlaySpeed + prevFrame;

            if (((temp2 > 2) && (temp1 <= 0)) || ((temp1 < 7) && (temp2 >= 9))) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_WALK);
            }
        }

        if (Actor_IsFacingPlayer(&this->actor, 0x71C)) {
            if (Rand_ZeroOne() > 0.8f) {
                if ((Rand_ZeroOne() > 0.7f)) {
                    func_80860EC0(this);
                } else {
                    EnTest_ChooseAction(this, play);
                }
            } else {
                EnTest_SetupWalkAndBlock(this);
            }
        }
    }
}

// a variation of sidestep
void func_80860EC0(EnTest* this) {
    Animation_PlayLoop(&this->skelAnime, &gStalfosSidestepAnim);
    this->unk_7C8 = 0xF;
    this->actor.speedXZ = (Rand_ZeroOne() > 0.5f) ? -0.5f : 0.5f;
    this->timer = (s16)((Rand_ZeroOne() * 15.0f) + 25.0f);
    this->unk_7EC = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnTest_SetupAction(this, func_80860F84);
}

// a variation of sidestep
void func_80860F84(EnTest* this, PlayState* play) {
    s16 playerYaw180;
    s32 pad;
    s32 prevFrame;
    s32 temp_f16;
    s16 yawDiff;
    Player* player = GET_PLAYER(play);
    f32 checkDist = 0.0f;
    s16 newYaw;
    s32 absPlaySpeed;

    if (!EnTest_ReactToProjectile(play, this)) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA0, 1);
        this->actor.world.rot.y = this->actor.shape.rot.y + 0x3E80;
        playerYaw180 = player->actor.shape.rot.y + 0x8000;

        if (this->actor.speedXZ >= 0.0f) {
            if (this->actor.speedXZ < 6.0f) {
                this->actor.speedXZ += 0.5f;
            } else {
                this->actor.speedXZ = 6.0f;
            }
        } else {
            if (this->actor.speedXZ > -6.0f) {
                this->actor.speedXZ -= 0.5f;
            } else {
                this->actor.speedXZ = -6.0f;
            }
        }

        if ((this->actor.bgCheckFlags & 8) ||
            ((this->actor.params == STALFOS_TYPE_CEILING) &&
             !Actor_TestFloorInDirection(&this->actor, play, this->actor.speedXZ, this->actor.world.rot.y))) {
            if (this->actor.bgCheckFlags & 8) {
                if (this->actor.speedXZ >= 0.0f) {
                    newYaw = this->actor.shape.rot.y + 0x3FFF;
                } else {
                    newYaw = this->actor.shape.rot.y - 0x3FFF;
                }

                newYaw = this->actor.wallYaw - newYaw;
            } else {
                this->actor.speedXZ *= -0.8f;
                newYaw = 0;
            }

            if (ABS(newYaw) > 0x4000) {
                this->actor.speedXZ *= -0.8f;

                if (this->actor.speedXZ < 0.0f) {
                    this->actor.speedXZ -= 0.5f;
                } else {
                    this->actor.speedXZ += 0.5f;
                }
            }
        }

        if (Actor_OtherIsTargeted(play, &this->actor)) {
            checkDist = 200.0f;
        }

        if (this->actor.xzDistToPlayer <= (80.0f + checkDist)) {
            Math_SmoothStepToF(&this->unk_7EC, -2.5f, 1.0f, 0.8f, 0.0f);
        } else if (this->actor.xzDistToPlayer > (110.0f + checkDist)) {
            Math_SmoothStepToF(&this->unk_7EC, 2.5f, 1.0f, 0.8f, 0.0f);
        } else {
            Math_SmoothStepToF(&this->unk_7EC, 0.0f, 1.0f, 6.65f, 0.0f);
        }

        if (this->unk_7EC != 0.0f) {
            this->actor.world.pos.x += Math_SinS(this->actor.shape.rot.y) * this->unk_7EC;
            this->actor.world.pos.z += Math_CosS(this->actor.shape.rot.y) * this->unk_7EC;
        }

        this->skelAnime.playSpeed = this->actor.speedXZ * 0.5f;

        prevFrame = this->skelAnime.curFrame;
        SkelAnime_Update(&this->skelAnime);
        temp_f16 = this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed);
        absPlaySpeed = (f32)ABS(this->skelAnime.playSpeed);

        if ((s32)this->skelAnime.curFrame != prevFrame) {
            s32 temp_v0_2 = absPlaySpeed + prevFrame;

            if (((temp_v0_2 > 1) && (temp_f16 <= 0)) || ((temp_f16 < 7) && (temp_v0_2 >= 8))) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_WALK);
            }
        }

        if ((play->gameplayFrames & 95) == 0) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_WARAU);
        }

        yawDiff = playerYaw180 - this->actor.shape.rot.y;
        yawDiff = ABS(yawDiff);

        if ((yawDiff > 0x6800) || (this->timer == 0)) {
            EnTest_ChooseAction(this, play);
        } else if (this->timer != 0) {
            this->timer--;
        }
    }
}

void EnTest_SetupSlashDown(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosDownSlashAnim);
    Audio_StopSfxByPosAndId(&this->actor.projectedPos, NA_SE_EN_STAL_WARAU);
    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    this->swordCollider.base.atFlags &= ~AT_WEAK;
    this->unk_7C8 = 0x10;
    this->actor.speedXZ = 0.0f;
    EnTest_SetupAction(this, EnTest_SlashDown);
    this->swordCollider.info.toucher.damage = 0x20;

    if (IS_ELITE) {
        this->shieldState = 5;
    } else {
        if (this->shieldState != 0) {
            this->shieldState = 3;
        }
    }
}

void EnTest_SlashDown(EnTest* this, PlayState* play) {
    this->actor.speedXZ = 0.0f;

    if ((s32)this->skelAnime.curFrame < 4) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xBB8, 0);
        this->actor.world.rot.y = this->actor.shape.rot.y;
    }

    if ((s32)this->skelAnime.curFrame == 7) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_SAKEBI);
    }

    if ((s32)this->skelAnime.curFrame < 8){
        if (this->actor.xzDistToPlayer > 40.0f)
            this->actor.speedXZ = 5.0f;

        if (IS_ELITE) {
            Vec3f circ;
            Vec3f center = this->actor.world.pos;
            EffectBlure* eb1 = Effect_GetByIndex(this->effectIndicatorIndex[0]);
            EffectBlure* eb2 = Effect_GetByIndex(this->effectIndicatorIndex[1]);
            f32 sin1 = Math_SinS((s16)(this->skelAnime.curFrame*0x2000))*40;
            f32 cos1 = Math_CosS((s16)(this->skelAnime.curFrame*0x2000))*40;
            center.y += 50.0f;

            circ = center;
            circ.z += sin1;
            circ.x += cos1;
            EffectBlure_AddVertex(eb1, &circ, &center);

            center.y -= 10;
            circ = center;
            circ.z -= sin1;
            circ.x -= cos1;
            EffectBlure_AddVertex(eb2, &circ, &center);
        }
    }

    if ((this->skelAnime.curFrame > 7.0f) && (this->skelAnime.curFrame < 11.0f)) {
        this->swordState = 1;
    } else {
        this->swordState = 0;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        if ((this->actor.xzDistToPlayer > 60.0f) || Rand_ZeroOne() > 0.8f) {
            EnTest_SetupSlashDownEnd(this);
        } else {
            EnTest_SetupSlashUp(this);
        }
    }
}

void EnTest_SetupShieldBash(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosDownSlashAnim);
    Audio_StopSfxByPosAndId(&this->actor.projectedPos, NA_SE_EN_STAL_WARAU);
    this->skelAnime.playSpeed = 2.0f;
    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    this->swordCollider.base.atFlags &= ~AT_WEAK;
    this->unk_7C8 = 0x10;
    this->actor.speedXZ = 0.0f;
    EnTest_SetupAction(this, EnTest_ShieldBash);
    this->swordCollider.info.toucher.damage = 0x10;
    this->shieldState = 1;
}

void EnTest_ShieldBash(EnTest* this, PlayState* play) {
    this->actor.speedXZ = 0.0f;

    if ((s32)this->skelAnime.curFrame < 4) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xBB8, 0);
        this->actor.world.rot.y = this->actor.shape.rot.y;
    }

    if ((s32)this->skelAnime.curFrame == 7) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_SAKEBI);
    }

    if ((this->skelAnime.curFrame > 7.0f) && (this->skelAnime.curFrame < 11.0f)) {
        this->swordState = 1;
    } else {
        this->swordState = 0;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        this->skelAnime.playSpeed = 1.0f;
        EnTest_SetupSlashDownEnd(this);
    }
}

void EnTest_SetupSlashDownEnd(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosRecoverFromDownSlashAnim);
    this->unk_7C8 = 0x12;
    this->actor.speedXZ = 0.0f;
    EnTest_SetupAction(this, EnTest_SlashDownEnd);
}

void EnTest_SlashDownEnd(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 yawDiff;

    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->hitTracker) {
            this->hitTracker = 0;
            //Player_SetShieldRecoveryDefault(play);
            if (this->actor.params != STALFOS_TYPE_CEILING) {
                EnTest_SetupJumpBack(this);
                return;
            }
        }

        if (Rand_ZeroOne() > 0.7f) {
            EnTest_SetupIdle(this);
            this->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
            return;
        }

        this->actor.world.rot.y = Actor_WorldYawTowardActor(&this->actor, &player->actor);

        if (Rand_ZeroOne() > 0.7f) {
            if (this->actor.params != STALFOS_TYPE_CEILING) {
                EnTest_SetupJumpBack(this);
                return;
            }
        }

        yawDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;

        if (ABS(yawDiff) <= 0x2710) {
            yawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

            if ((ABS(yawDiff) > 0x3E80) && (this->actor.params != STALFOS_TYPE_CEILING)) {
                this->actor.world.rot.y = this->actor.yawTowardsPlayer;
                EnTest_SetupJumpBack(this);
            } else if (player->stateFlags1 & 0x10) {
                if (1 || this->actor.isTargeted) {
                    EnTest_SetupSlashDown(this);
                } else if ((play->gameplayFrames % 2) != 0) {
                    func_808627C4(this, play);
                } else {
                    EnTest_SetupJumpBack(this);
                }
            } else {
                EnTest_SetupSlashDown(this);
            }
        } else {
            func_808627C4(this, play);
        }
    }
}

void EnTest_SetupSlashUp(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosUpSlashAnim);
    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    this->swordCollider.base.atFlags &= ~AT_WEAK;
    this->unk_7C8 = 0x11;
    this->swordCollider.info.toucher.damage = 0x20;
    this->actor.speedXZ = 0.0f;
    EnTest_SetupAction(this, EnTest_SlashUp);

    if (IS_ELITE) {
        this->shieldState = 5;
    } else {
        if (this->shieldState != 0) {
            this->shieldState = 3;
        }
    }
}

void EnTest_SlashUp(EnTest* this, PlayState* play) {
    this->actor.speedXZ = 0.0f;

    if ((s32)this->skelAnime.curFrame == 2) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_SAKEBI);
    }

    if ((s32)this->skelAnime.curFrame < 8){
        if ((s32)this->skelAnime.curFrame < 5) {
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x1000, 0);
            this->actor.world.rot.y = this->actor.shape.rot.y;
        }
        if (this->actor.xzDistToPlayer > 50.0f)
            this->actor.speedXZ = 5.0f;
    }

    if ((this->skelAnime.curFrame > 1.0f) && (this->skelAnime.curFrame < 8.0f)) {
        this->swordState = 1;
    } else {
        this->swordState = 0;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        EnTest_SetupSlashDown(this);
    }
}

void EnTest_SetupThrust(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosStabAnim);
    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    this->swordCollider.base.atFlags |= AT_WEAK;
    this->unk_7C8 = 0x11;
    this->swordCollider.info.toucher.damage = 0x20;
    this->actor.speedXZ = 0.0f;
    EnTest_SetupAction(this, EnTest_Thrust);

    if (this->shieldState != 0) {
        this->shieldState = 3;
    }
}

void EnTest_Thrust(EnTest* this, PlayState* play) {
    this->actor.speedXZ = 0.0f;

    if ((s32)this->skelAnime.curFrame == 2) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_SAKEBI);
    }

    if ((s32)this->skelAnime.curFrame < 8){
        if (this->actor.xzDistToPlayer > 80.0f)
            this->actor.speedXZ = 9.0f;
    }

    if ((this->skelAnime.curFrame > 3.0f) && (this->skelAnime.curFrame < 12.0f)) {
        this->swordState = 1;
    } else {
        this->swordState = 0;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        EnTest_SetupSlashDownEnd(this);
    }
}

void EnTest_SetupSpinAttack(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosSpinAttackAnim);
    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    this->swordCollider.base.atFlags &= ~AT_WEAK;
    this->unk_7C8 = 0x11;
    this->swordCollider.info.toucher.damage = 0x20;
    this->actor.speedXZ = 0.0f;
    this->timer = 0;
    EnTest_SetupAction(this, EnTest_SpinAttack);

    if (IS_ELITE) {
        this->shieldState = 5;
    } else {
        if (this->shieldState != 0) {
            this->shieldState = 3;
        }
    }
}

void EnTest_SpinAttack(EnTest* this, PlayState* play) {
    this->actor.speedXZ = 0.0f;
    Player *player = GET_PLAYER(play);

    if ((s32)this->skelAnime.curFrame == 2) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_SAKEBI);
    }

    if ((s32)this->skelAnime.curFrame < 8){
        if ((s32)this->skelAnime.curFrame < 5) {
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x1000, 0);
            this->actor.world.rot.y = this->actor.shape.rot.y;
        }

        if (IS_ELITE) {
            Vec3f circ;
            Vec3f center = this->actor.world.pos;
            EffectBlure* eb1 = Effect_GetByIndex(this->effectIndicatorIndex[0]);
            EffectBlure* eb2 = Effect_GetByIndex(this->effectIndicatorIndex[1]);
            f32 sin1 = Math_SinS((s16)(this->skelAnime.curFrame*0x2000))*40;
            f32 cos1 = Math_CosS((s16)(this->skelAnime.curFrame*0x2000))*40;
            f32 sin2 = Math_SinS(this->actor.shape.rot.y);
            f32 cos2 = Math_CosS(this->actor.shape.rot.y);
            center.y += 50.0f;

            circ = center;
            circ.z += sin1*cos2;
            circ.x += sin1*sin2;
            circ.y += cos1;
            EffectBlure_AddVertex(eb1, &circ, &center);

            center.z += 10*sin2;
            center.x += 10*cos2;
            circ = center;
            circ.z -= sin1*cos2;
            circ.x -= sin1*sin2;
            circ.y -= cos1;
            EffectBlure_AddVertex(eb2, &circ, &center);
        }
    }

    if ((this->skelAnime.curFrame > 3.0f) && (this->timer >= 0)) {
        this->swordState = 1;
    } else {
        this->swordState = 0;
    }

    if (this->timer > 0) {
        this->timer--;
        this->actor.shape.rot.y -= 0x7FFF/5;
        if (this->timer == 0)
            this->timer--;
    } else if (this->timer < 0) {
        this->timer--;
        if (this->timer == -4) {
            EnTest_SetupIdle(this);
        }
    } else if (SkelAnime_Update(&this->skelAnime)) {
        this->timer = 10;
    }
}

void EnTest_SetupCrouch(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosCrouchAndSlashAnimAnim);
    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    this->swordCollider.base.atFlags &= ~AT_WEAK;
    this->unk_7C8 = 0x11;
    this->swordCollider.info.toucher.damage = 0x20;
    this->actor.speedXZ = 0.0f;
    this->timer = 0;
    EnTest_SetupAction(this, EnTest_Crouch);

    this->shieldState = 5;
}

void EnTest_Crouch(EnTest* this, PlayState* play) {
    this->actor.speedXZ = 0.0f;

    if ((s32)this->skelAnime.curFrame == 7) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_SAKEBI);
    }

    if ((s32)this->skelAnime.curFrame < 6){
        if ((s32)this->skelAnime.curFrame < 5) {
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x1000, 0);
            this->actor.world.rot.y = this->actor.shape.rot.y;
        }
    }

    if ((8.0f < this->skelAnime.curFrame) && (this->skelAnime.curFrame < 15)) {
        this->swordState = 1;
    } else {
        this->swordState = 0;
    }

    if (this->skelAnime.curFrame < 6) {
        SkelAnime_Update(&this->skelAnime);
    } else if (this->timer < 1) {
        this->timer++;
    } else {
        if (SkelAnime_Update(&this->skelAnime)){
            EnTest_SetupIdle(this);
        }
    }
}

void EnTest_SetupJumpBack(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosJumpBackwardsAnim);
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_JUMP);
    this->unk_7C8 = 0x14;
    this->timer = 5;
    EnTest_SetupAction(this, EnTest_JumpBack);

    if (this->shieldState != 0) {
        this->shieldState = 3;
    }

    if (this->actor.params != STALFOS_TYPE_CEILING) {
        this->actor.speedXZ = -11.0f;
    } else {
        this->actor.speedXZ = -7.0f;
    }
}

void EnTest_JumpBack(EnTest* this, PlayState* play) {
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xBB8, 1);

    if (this->timer == 0) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_WARAU);
    } else {
        this->timer--;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        if (!EnTest_ReactToProjectile(play, this)) {
            if (this->actor.xzDistToPlayer <= 100.0f) {
                if (Actor_IsFacingPlayer(&this->actor, 0x1555)) {
                    EnTest_SetupSlashDown(this);
                } else {
                    EnTest_SetupIdle(this);
                    this->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
                }
            } else {
                if ((this->actor.xzDistToPlayer <= 220.0f) && Actor_IsFacingPlayer(&this->actor, 0xE38)) {
                    EnTest_SetupJumpslash(this);
                } else {
                    EnTest_SetupIdle(this);
                    this->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
                }
            }
            this->actor.flags |= ACTOR_FLAG_TARGETABLE;
        }
    } else if (this->skelAnime.curFrame == (this->skelAnime.endFrame - 4.0f)) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_DODO_M_GND);
    }
}

void EnTest_SetupJumpslash(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosJumpAnim);
    Audio_StopSfxByPosAndId(&this->actor.projectedPos, NA_SE_EN_STAL_WARAU);
    this->timer = 0;
    this->unk_7C8 = 0x17;
    this->actor.velocity.y = 10.0f;
    this->actor.speedXZ = 8.0f;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_JUMP);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    this->swordCollider.base.atFlags &= ~AT_WEAK;
    EnTest_SetupAction(this, EnTest_Jumpslash);
    this->swordCollider.info.toucher.damage = 0x20;

    // if (this->shieldState != 0) {
    //     this->shieldState = 3;
    // }
    this->shieldState = 5;
}

void EnTest_Jumpslash(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    if ((this->timer == 0) && this->skelAnime.curFrame <= (this->skelAnime.endFrame - 0.0f)) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x900, 1);
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    }

    s32 derivedFrame;
    f32 dot;
    s16 yawToPlayerDiff = this->actor.yawTowardsPlayer-this->actor.shape.rot.y;
    if (this->timer == 0) {
        derivedFrame = this->skelAnime.curFrame;
    } else {
        derivedFrame = this->skelAnime.curFrame + 7;
    }
    dot = Math_CosS(yawToPlayerDiff);
    s32 framesToLand = (13-derivedFrame);
    f32 overshoot = framesToLand*this->actor.speedXZ - this->actor.xzDistToPlayer*dot;

    if (overshoot > 0.0f && framesToLand > 0) {
        this->actor.speedXZ -= overshoot/framesToLand;
        if (this->actor.speedXZ < 0.0f)
            this->actor.speedXZ = 0.0f;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->timer == 0) {
            f32 sideVel = Math_SinS(this->actor.shape.rot.y-player->actor.world.rot.y)*player->actor.speedXZ;
            if (yawToPlayerDiff > 0x400 || sideVel > 0.5f)
                Animation_PlayOnce(&this->skelAnime, &gStalfosJumpSlash3Anim);
            else if (yawToPlayerDiff < -0x400 || sideVel < 0.5f)
                Animation_PlayOnce(&this->skelAnime, &gStalfosJumpslashAnim);
            else
                Animation_PlayOnce(&this->skelAnime, &gStalfosJumpSlash2Anim);
            this->timer = 1;
            this->swordState = 1;
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_SAKEBI);
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_JUMP);
        } else {
            this->actor.speedXZ = 0.0f;
            EnTest_SetupIdle(this);
        }
    }

    if ((this->timer != 0) && (this->skelAnime.curFrame >= 5.0f)) {
        this->swordState = 0;
    }

    if (this->actor.world.pos.y <= this->actor.floorHeight) {
        if (this->actor.speedXZ != 0.0f) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_DODO_M_GND);
        }

        this->actor.world.pos.y = this->actor.floorHeight;
        this->actor.velocity.y = 0.0f;
        this->actor.speedXZ = 0.0f;
    }
}

void EnTest_SetupCrossoverJump(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosJumpAnim);
    Audio_StopSfxByPosAndId(&this->actor.projectedPos, NA_SE_EN_STAL_WARAU);
    this->timer = 0;
    this->unk_7C8 = 0x17;
    this->actor.velocity.y = 18.0f;
    this->actor.speedXZ = 10.0f;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_JUMP);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    this->swordCollider.base.atFlags &= ~AT_WEAK;
    EnTest_SetupAction(this, EnTest_CrossoverJump);
    this->swordCollider.info.toucher.damage = 0x20;

    if (this->shieldState != 0) {
        this->shieldState = 3;
    }
}

void EnTest_CrossoverJump(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    this->actor.gravity = -2.5f;
    if ((this->timer == 0) && this->skelAnime.curFrame <= (this->skelAnime.endFrame - 0.0f)) {
        Vec3f curPos;
        Vec3f newVecToPlayer;
        curPos = this->actor.world.pos;
        curPos.x += Math_SinS(this->actor.world.rot.y) * 100.0f;
        curPos.z += Math_CosS(this->actor.world.rot.y) * 100.0f;
        newVecToPlayer.x = player->actor.world.pos.x - curPos.x;
        newVecToPlayer.z = player->actor.world.pos.z - curPos.z;
        Math_SmoothStepToS(&this->actor.shape.rot.y, Math_Atan2S(newVecToPlayer.z, newVecToPlayer.x), 1, 0x2000, 1);
    } else {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x1000, 1);
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->timer < 3) {
            this->timer++;
            //this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
            if (this->timer == 3) {
                Animation_PlayOnce(&this->skelAnime, &gStalfosJumpSlash2Anim);
                this->swordState = 1;
                this->actor.speedXZ = 6.0f;
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_SAKEBI);
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_JUMP);
            }
        } else {
            this->actor.speedXZ = 0.0f;
            this->actor.world.rot.y = this->actor.shape.rot.y;
            EnTest_SetupIdle(this);
        }
    }

    if ((this->timer != 0) && (this->skelAnime.curFrame >= 5.0f)) {
        this->swordState = 0;
    }

    if (this->actor.world.pos.y <= this->actor.floorHeight) {
        if (this->actor.speedXZ != 0.0f) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_DODO_M_GND);
        }

        this->actor.world.pos.y = this->actor.floorHeight;
        this->actor.velocity.y = 0.0f;
        this->actor.speedXZ = 0.0f;
        //this->actor.world.rot.y = this->actor.shape.rot.y;
    }
}

void EnTest_SetupJumpUp(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosJumpAnim);
    this->timer = 0;
    this->unk_7C8 = 4;
    this->actor.velocity.y = 14.0f;
    this->actor.speedXZ = 6.0f;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_JUMP);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnTest_SetupAction(this, EnTest_JumpUp);
}

void EnTest_JumpUp(EnTest* this, PlayState* play) {
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    SkelAnime_Update(&this->skelAnime);

    if (this->actor.world.pos.y <= this->actor.floorHeight) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_DODO_M_GND);
        this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
        this->actor.world.pos.y = this->actor.floorHeight;
        this->unk_7E4 = -(s32)this->actor.velocity.y;

        if (this->unk_7E4 == 0) {
            this->unk_7E4 = 1;
        }

        this->actor.velocity.y = 0.0f;
        this->actor.speedXZ = 0.0f;
        this->unk_7C8 = 0xC;
        this->timer = 4;
        Animation_Change(&this->skelAnime, &gStalfosLandFromLeapAnim, 0.0f, 0.0f, 0.0f, 2, 0.0f);
        EnTest_SetupAction(this, EnTest_Land);
    }
}

void EnTest_SetupStopAndBlock(EnTest* this) {
    Animation_Change(&this->skelAnime, &gStalfosBlockWithShieldAnim, 2.0f, 0.0f,
                     Animation_GetLastFrame(&gStalfosBlockWithShieldAnim), 2, 2.0f);
    this->unk_7C8 = 0x15;
    this->actor.speedXZ = 0.0f;
    this->timer = (Rand_ZeroOne() * 10.0f) + 11.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->shieldState = 5;
    EnTest_SetupAction(this, EnTest_StopAndBlock);
}

void EnTest_StopAndBlock(EnTest* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
    SkelAnime_Update(&this->skelAnime);

    if ((ABS((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y)) > 0x3E80) &&
        (this->actor.params != STALFOS_TYPE_CEILING) && ((play->gameplayFrames % 2) != 0)) {
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        EnTest_SetupJumpBack(this);
    }

    if (this->timer == 0) {
        EnTest_SetupIdleFromBlock(this);
    } else {
        this->timer--;
    }
}

void EnTest_SetupIdleFromBlock(EnTest* this) {
    Animation_MorphToLoop(&this->skelAnime, &gStalfosMiddleGuardAnim, -4.0f);
    this->unk_7C8 = 0x16;
    EnTest_SetupAction(this, EnTest_IdleFromBlock);
}

void EnTest_IdleFromBlock(EnTest* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 1.0f, 1.5f, 0.0f);
    SkelAnime_Update(&this->skelAnime);

    if (this->skelAnime.morphWeight == 0.0f) {
        this->actor.speedXZ = 0.0f;
        this->shieldState = 0;

        if (!EnTest_ReactToProjectile(play, this)) {
            if (this->actor.xzDistToPlayer < 500.0f) {
                EnTest_ChooseAction(this, play);
            } else {
                func_808627C4(this, play);
            }
        }
    }
}

void EnTest_SetupFlinchFront(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosFlinchFromHitFrontAnim);
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_DAMAGE);
    this->unk_7C8 = 8;
    this->actor.speedXZ = -2.0f;
    Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 8);
    EnTest_SetupAction(this, EnTest_FlinchFront);
}

void EnTest_FlinchFront(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 1.0f, 0.1f, 0.0f);

    if (SkelAnime_Update(&this->skelAnime)) {
        this->actor.speedXZ = 0.0f;

        if ((this->actor.bgCheckFlags & 8) && ((ABS((s16)(this->actor.wallYaw - this->actor.shape.rot.y)) < 0x38A4) &&
                                               (this->actor.xzDistToPlayer < 80.0f))) {
            EnTest_SetupJumpUp(this);
        } else if (!EnTest_ReactToProjectile(play, this)) {
            EnTest_ChooseAction(this, play);
        } else {
            return;
        }
    }

    if (player->swordState != 0) {
        if ((this->actor.bgCheckFlags & 8) && ((ABS((s16)(this->actor.wallYaw - this->actor.shape.rot.y)) < 0x38A4) &&
                                               (this->actor.xzDistToPlayer < 80.0f))) {
            EnTest_SetupJumpUp(this);
        } else if ((Rand_ZeroOne() > 0.7f) && (this->actor.params != STALFOS_TYPE_CEILING) &&
                   (player->meleeWeaponAnimation != 0x11)) {
            EnTest_SetupJumpBack(this);
        } else {
            EnTest_SetupStopAndBlock(this);
        }

        this->unk_7C8 = 8;
    }
}

void EnTest_SetupFlinchBack(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosFlinchFromHitBehindAnim);
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_DAMAGE);
    this->unk_7C8 = 9;
    this->actor.speedXZ = -2.0f;
    Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 8);
    EnTest_SetupAction(this, EnTest_FlinchBack);
}

void EnTest_FlinchBack(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 1.0f, 0.1f, 0.0f);

    if (SkelAnime_Update(&this->skelAnime)) {
        this->actor.speedXZ = 0.0f;

        if (!EnTest_ReactToProjectile(play, this)) {
            EnTest_ChooseAction(this, play);
        } else {
            return;
        }
    }

    if (player->swordState != 0) {
        if ((this->actor.bgCheckFlags & 8) && ((ABS((s16)(this->actor.wallYaw - this->actor.shape.rot.y)) < 0x38A4) &&
                                               (this->actor.xzDistToPlayer < 80.0f))) {
            EnTest_SetupJumpUp(this);
        } else if ((Rand_ZeroOne() > 0.7f) && (this->actor.params != STALFOS_TYPE_CEILING) &&
                   (player->meleeWeaponAnimation != 0x11)) {
            EnTest_SetupJumpBack(this);
        } else {
            EnTest_SetupStopAndBlock(this);
        }

        this->unk_7C8 = 8;
    }
}

void EnTest_SetupStunned(EnTest* this) {
    this->unk_7C8 = 0xB;
    this->shieldState = 0;
    this->swordState = 0;
    this->skelAnime.playSpeed = 0.0f;
    this->actor.speedXZ = -4.0f;

    if (this->lastDamageEffect == STALFOS_DMGEFF_LIGHT) {
        Actor_SetColorFilter(&this->actor, -0x8000, 0x78, 0, 0x50);
    } else {
        Actor_SetColorFilter(&this->actor, 0, 0x78, 0, 0x50);

        if (this->lastDamageEffect == STALFOS_DMGEFF_FREEZE) {
            this->iceTimer = 36;
        } else {
            Animation_PlayOnceSetSpeed(&this->skelAnime, &gStalfosFlinchFromHitFrontAnim, 0.0f);
        }
    }

    Audio_PlayActorSound2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
    EnTest_SetupAction(this, EnTest_Stunned);
}

void EnTest_Stunned(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 1.0f, 1.0f, 0.0f);

    if (this->actor.colorFilterTimer == 0) {
        if (this->actor.colChkInfo.health == 0) {
            func_80862FA8(this, play);
        } else if (player->swordState != 0) {
            if ((this->actor.bgCheckFlags & 8) &&
                ((ABS((s16)(this->actor.wallYaw - this->actor.shape.rot.y)) < 0x38A4) &&
                 (this->actor.xzDistToPlayer < 80.0f))) {
                EnTest_SetupJumpUp(this);
            } else if ((Rand_ZeroOne() > 0.7f) && (player->meleeWeaponAnimation != 0x11)) {
                EnTest_SetupJumpBack(this);
            } else {
                EnTest_SetupStopAndBlock(this);
            }

            this->unk_7C8 = 8;
        } else {
            this->actor.speedXZ = 0.0f;
            if (!EnTest_ReactToProjectile(play, this)) {
                EnTest_ChooseAction(this, play);
            }
        }
    }
}

// a variation of sidestep
void func_808627C4(EnTest* this, PlayState* play) {
    if (Actor_OtherIsTargeted(play, &this->actor)) {
        func_80860EC0(this);
        return;
    }

    Animation_MorphToLoop(&this->skelAnime, &gStalfosSidestepAnim, -2.0f);
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    this->actor.speedXZ = ((play->gameplayFrames % 2) != 0) ? -4.0f : 4.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y + 0x3FFF;
    this->timer = (Rand_ZeroOne() * 20.0f) + 20.0f;
    this->unk_7C8 = 0x18;
    EnTest_SetupAction(this, func_808628C8);
    this->unk_7EC = 0.0f;
}

// a variation of sidestep
void func_808628C8(EnTest* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);
    s32 pad1;
    s32 prevFrame;
    s32 temp_f16;
    s32 pad2;
    f32 checkDist = 0.0f;
    s16 newYaw;
    f32 absPlaySpeed;

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA0, 1);

    if (this->shieldState == 0) {
        this->shieldState++;
    }

    if (this->actor.speedXZ >= 0.0f) {
        if (this->actor.speedXZ < 6.0f) {
            this->actor.speedXZ += 0.125f;
        } else {
            this->actor.speedXZ = 6.0f;
        }
    } else {
        if (this->actor.speedXZ > -6.0f) {
            this->actor.speedXZ -= 0.125f;
        } else {
            this->actor.speedXZ = -6.0f;
        }
    }

    if ((this->actor.bgCheckFlags & 8) ||
        ((this->actor.params == STALFOS_TYPE_CEILING) &&
         !Actor_TestFloorInDirection(&this->actor, play, this->actor.speedXZ, this->actor.shape.rot.y + 0x3FFF))) {
        if (this->actor.bgCheckFlags & 8) {
            if (this->actor.speedXZ >= 0.0f) {
                newYaw = (this->actor.shape.rot.y + 0x3FFF);
            } else {
                newYaw = (this->actor.shape.rot.y - 0x3FFF);
            }

            newYaw = this->actor.wallYaw - newYaw;
        } else {
            this->actor.speedXZ *= -0.8f;
            newYaw = 0;
        }

        if (ABS(newYaw) > 0x4000) {
            this->actor.speedXZ *= -0.8f;

            if (this->actor.speedXZ < 0.0f) {
                this->actor.speedXZ -= 0.5f;
            } else {
                this->actor.speedXZ += 0.5f;
            }
        }
    }

    this->actor.world.rot.y = this->actor.shape.rot.y + 0x3FFF;

    if (Actor_OtherIsTargeted(play, &this->actor)) {
        checkDist = 200.0f;
    }

    if (this->actor.xzDistToPlayer <= (80.0f + checkDist)) {
        Math_SmoothStepToF(&this->unk_7EC, -2.5f, 1.0f, 0.8f, 0.0f);
    } else if (this->actor.xzDistToPlayer > (110.0f + checkDist)) {
        Math_SmoothStepToF(&this->unk_7EC, 2.5f, 1.0f, 0.8f, 0.0f);
    } else {
        Math_SmoothStepToF(&this->unk_7EC, 0.0f, 1.0f, 6.65f, 0.0f);
    }

    if (this->unk_7EC != 0.0f) {
        this->actor.world.pos.x += (Math_SinS(this->actor.shape.rot.y) * this->unk_7EC);
        this->actor.world.pos.z += (Math_CosS(this->actor.shape.rot.y) * this->unk_7EC);
    }

    this->skelAnime.playSpeed = this->actor.speedXZ * 0.5f;
    prevFrame = this->skelAnime.curFrame;
    SkelAnime_Update(&this->skelAnime);

    temp_f16 = this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed);
    absPlaySpeed = ABS(this->skelAnime.playSpeed);

    if ((this->timer % 32) == 0) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_WARAU);
    }
    if ((s32)this->skelAnime.curFrame != prevFrame) {
        s32 temp_v0_2 = (s32)absPlaySpeed + prevFrame;

        if (((temp_v0_2 > 1) && (temp_f16 <= 0)) || ((temp_f16 < 7) && (temp_v0_2 >= 8))) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_WALK);
        }
    }

    if (this->timer == 0) {
        if (Actor_OtherIsTargeted(play, &this->actor)) {
            EnTest_SetupIdle(this);
        } else if (Actor_IsTargeted(play, &this->actor)) {
            if (!EnTest_ReactToProjectile(play, this)) {
                EnTest_ChooseAction(this, play);
            }
        } else if (player->heldItemAction != PLAYER_IA_NONE) {
            if ((play->gameplayFrames % 2) != 0) {
                EnTest_SetupIdle(this);
            } else {
                EnTest_SetupWalkAndBlock(this);
            }
        } else {
            EnTest_SetupWalkAndBlock(this);
        }

    } else {
        this->timer--;
    }
}

void func_80862DBC(EnTest* this, PlayState* play) {
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_DAMAGE);
    this->unk_7C8 = 2;
    BodyBreak_Alloc(&this->bodyBreak, 60, play);
    this->actor.home.rot.x = 0;

    if (this->swordState >= 0) {
        EffectBlure_AddSpace(Effect_GetByIndex(this->effectIndex));
        this->swordState = -1;
    }

    this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;

    if (this->actor.params == STALFOS_TYPE_5) {
        Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_PROP);
    }

    EnTest_SetupAction(this, func_80862E6C);
}

void func_80862E6C(EnTest* this, PlayState* play) {
    if (this->actor.child == NULL) {
        if (this->actor.home.rot.x == 0) {
            this->actor.home.rot.x = this->bodyBreak.count;
        }

        if (BodyBreak_SpawnParts(&this->actor, &this->bodyBreak, play, this->actor.params + 8)) {
            this->actor.child = &this->actor;
        }
    } else {
        if (this->actor.home.rot.x == 0) {
            this->actor.colChkInfo.health = 10;

            if (this->actor.params == STALFOS_TYPE_4) {
                this->actor.params = -1;
            } else {
                Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_ENEMY);
            }

            this->actor.child = NULL;
            this->actor.flags |= ACTOR_FLAG_TARGETABLE;
            EnTest_SetupJumpBack(this);
        } else if ((this->actor.params == STALFOS_TYPE_5) &&
                   !Actor_FindNearby(play, &this->actor, ACTOR_EN_TEST, ACTORCAT_ENEMY, 8000.0f)) {
            Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0xD0);

            if (this->actor.parent != NULL) {
                this->actor.parent->home.rot.z--;
            }

            Actor_Kill(&this->actor);
            gSaveContext.sohStats.count[COUNT_ENEMIES_DEFEATED_STALFOS]++;
        }
    }
}

void func_80862FA8(EnTest* this, PlayState* play) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosFallOverBackwardsAnim);
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_DEAD);
    this->shieldState = 0;
    this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;
    this->actor.colorFilterTimer = 0;
    this->actor.speedXZ = 0.0f;

    if (this->actor.params <= STALFOS_TYPE_CEILING) {
        this->unk_7C8 = 5;
        EnTest_SetupAction(this, func_80863044);
    } else {
        func_80862DBC(this, play);
    }
}

void func_80863044(EnTest* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        this->timer = (Rand_ZeroOne() * 10.0f) + 10.0f;
        this->unk_7C8 = 7;
        EnTest_SetupAction(this, func_808633E8);
        BodyBreak_Alloc(&this->bodyBreak, 60, play);
    }

    if ((s32)this->skelAnime.curFrame == 15) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_RIZA_DOWN);
    }
}

void func_808630F0(EnTest* this, PlayState* play) {
    Animation_PlayOnce(&this->skelAnime, &gStalfosFallOverForwardsAnim);
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_STAL_DEAD);
    this->unk_7C8 = 6;
    this->actor.colorFilterTimer = 0;
    this->shieldState = 0;
    this->actor.speedXZ = 0.0f;

    if (this->actor.params <= STALFOS_TYPE_CEILING) {
        this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;
        EnTest_SetupAction(this, func_8086318C);
    } else {
        func_80862DBC(this, play);
    }
}

void func_8086318C(EnTest* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        this->timer = (Rand_ZeroOne() * 10.0f) + 10.0f;
        this->unk_7C8 = 7;
        EnTest_SetupAction(this, func_808633E8);
        BodyBreak_Alloc(&this->bodyBreak, 60, play);
    }

    if (((s32)this->skelAnime.curFrame == 10) || ((s32)this->skelAnime.curFrame == 25)) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_RIZA_DOWN);
    }
}

void EnTest_SetupRecoil(EnTest* this) {
    this->swordState = 0;
    this->skelAnime.moveFlags = 2;
    this->unk_7C8 = 0x13;
    this->skelAnime.playSpeed = -1.0f;
    this->skelAnime.startFrame = this->skelAnime.curFrame;
    this->skelAnime.endFrame = 0.0f;
    if (this->actionFunc == EnTest_CrossoverJump)
        this->actor.world.rot.y = this->actor.shape.rot.y;
    EnTest_SetupAction(this, EnTest_Recoil);
}

void EnTest_Recoil(EnTest* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        if (Rand_ZeroOne() > 0.7f) {
            EnTest_SetupIdle(this);
            this->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
        } else if (((play->gameplayFrames % 2) != 0) && (this->actor.params != STALFOS_TYPE_CEILING)) {
            EnTest_SetupJumpBack(this);
        } else {
            func_808627C4(this, play);
        }
    }
}

void EnTest_Rise(EnTest* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

    if (this->actor.scale.y < 0.015f) {
        this->actor.scale.y += 0.002f;
        this->actor.world.pos.y = this->actor.home.pos.y - 3.5f;
    } else {
        this->actor.world.pos.y = this->actor.home.pos.y;
        EnTest_SetupJumpBack(this);
    }
}

void func_808633E8(EnTest* this, PlayState* play) {
    this->actor.params = STALFOS_TYPE_1;

    if (BodyBreak_SpawnParts(&this->actor, &this->bodyBreak, play, this->actor.params)) {
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0xD0);

        if (this->actor.parent != NULL) {
            this->actor.parent->home.rot.z--;
        }

        Actor_Kill(&this->actor);
        gSaveContext.sohStats.count[COUNT_ENEMIES_DEFEATED_STALFOS]++;
    }
}

void EnTest_UpdateHeadRot(EnTest* this, PlayState* play) {
    s16 lookAngle = this->actor.yawTowardsPlayer;

    lookAngle -= (s16)(this->headRot.y + this->actor.shape.rot.y);

    this->headRotOffset.y = CLAMP(lookAngle, -0x7D0, 0x7D0);
    this->headRot.y += this->headRotOffset.y;
    this->headRot.y = CLAMP(this->headRot.y, -0x382F, 0x382F);
}

void EnTest_UpdateDamage(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 bounced = 0;

    if (this->shieldCollider.base.acFlags & AC_BOUNCED) {
        bounced = 1;
        this->shieldCollider.base.acFlags &= ~AC_BOUNCED;
    }

    if (bounced && ((!IS_ELITE && (this->unk_7C8 != 0x10 && this->unk_7C8 != 0x11)) ||
                    (IS_ELITE && !IS_VULNERABLE))) {
        this->bodyCollider.base.acFlags &= ~AC_HIT;
        if ((this->actionFunc == EnTest_Crouch))
            player->linearVelocity = 0.0f;//Prevents Link from moving out of range of the crouching attack when his attack bounces, assumes this collision is handled after the player's

        if (this->unk_7C8 >= 0xA) {
            this->actor.speedXZ = -4.0f;
        }
    } else if (this->bodyCollider.base.acFlags & AC_HIT) {
        this->bodyCollider.base.acFlags &= ~AC_HIT;

        if ((this->actor.colChkInfo.damageEffect != STALFOS_DMGEFF_SLING) &&
            (this->actor.colChkInfo.damageEffect != STALFOS_DMGEFF_FIREMAGIC) &&
            (this->actionFunc != EnTest_Jumpslash || VULNERABLE_IN_JUMP)) {
            this->lastDamageEffect = this->actor.colChkInfo.damageEffect;
            if (this->swordState >= 1) {
                this->swordState = 0;
            }
            this->unk_7DC = player->unk_845;
            this->actor.world.rot.y = this->actor.yawTowardsPlayer;
            Actor_SetDropFlag(&this->actor, &this->bodyCollider.info, false);
            Audio_StopSfxByPosAndId(&this->actor.projectedPos, NA_SE_EN_STAL_WARAU);

            if ((this->actor.colChkInfo.damageEffect == STALFOS_DMGEFF_STUN) ||
                (this->actor.colChkInfo.damageEffect == STALFOS_DMGEFF_FREEZE) ||
                (this->actor.colChkInfo.damageEffect == STALFOS_DMGEFF_LIGHT)) {
                if (this->unk_7C8 != 0xB) {
                    Actor_ApplyDamage(&this->actor);
                    EnTest_SetupStunned(this);
                }
            } else {
                if (Actor_IsFacingPlayer(&this->actor, 0x4000)) {
                    if (Actor_ApplyDamage(&this->actor) == 0) {
                        Enemy_StartFinishingBlow(play, &this->actor);
                        func_80862FA8(this, play);
                    } else {
                        EnTest_SetupFlinchFront(this);
                    }
                } else if (Actor_ApplyDamage(&this->actor) == 0) {
                    func_808630F0(this, play);
                    Enemy_StartFinishingBlow(play, &this->actor);
                } else {
                    EnTest_SetupFlinchBack(this);
                }
            }
        }
    }
}

void EnTest_Update(Actor* thisx, PlayState* play) {
    EnTest* this = (EnTest*)thisx;
    Player* player = GET_PLAYER(play);
    f32 oldWeight;
    u32 floorProperty;
    s32 pad;

    EnTest_UpdateDamage(this, play);

    if (this->actor.colChkInfo.damageEffect != STALFOS_DMGEFF_FIREMAGIC) {
        Actor_MoveForward(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 75.0f, 30.0f, 30.0f, 0x1D);

        if (this->actor.params == STALFOS_TYPE_1) {
            if (this->actor.world.pos.y <= this->actor.home.pos.y) {
                this->actor.world.pos.y = this->actor.home.pos.y;
                this->actor.velocity.y = 0.0f;
            }

            if (this->actor.floorHeight <= this->actor.home.pos.y) {
                this->actor.floorHeight = this->actor.home.pos.y;
            }
        } else if (this->actor.bgCheckFlags & 2) {
            floorProperty = func_80041EA4(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);

            if ((floorProperty == 5) || (floorProperty == 0xC) ||
                func_80041D4C(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId) == 9) {
                Actor_Kill(&this->actor);
                gSaveContext.sohStats.count[COUNT_ENEMIES_DEFEATED_STALFOS]++;
                return;
            }
        }

        this->actor.gravity = -1.5f;
        this->actionFunc(this, play);

        switch (this->shieldState) {
            case 0:
                break;

            case 1:
                Animation_Change(&this->upperSkelanime, &gStalfosBlockWithShieldAnim, 2.0f, 0.0f,
                                 Animation_GetLastFrame(&gStalfosBlockWithShieldAnim), 2, 2.0f);
                AnimationContext_SetCopyTrue(play, this->skelAnime.limbCount, this->skelAnime.jointTable,
                                             this->upperSkelanime.jointTable, sJointCopyFlags);
                this->shieldState++;
                break;

            case 2:
                SkelAnime_Update(&this->upperSkelanime);
                SkelAnime_CopyFrameTableTrue(&this->skelAnime, this->skelAnime.jointTable,
                                             this->upperSkelanime.jointTable, sJointCopyFlags);
                break;

            case 3:
                this->shieldState++;
                this->upperSkelanime.morphWeight = 4.0f;
                // fallthrough
            case 4:
                oldWeight = this->upperSkelanime.morphWeight;
                this->upperSkelanime.morphWeight -= 1.0f;

                if (this->upperSkelanime.morphWeight <= 0.0f) {
                    this->shieldState = 0;
                }

                SkelAnime_InterpFrameTable(this->skelAnime.limbCount, this->upperSkelanime.jointTable,
                                           this->upperSkelanime.jointTable, this->skelAnime.jointTable,
                                           1.0f - (this->upperSkelanime.morphWeight / oldWeight));
                SkelAnime_CopyFrameTableTrue(&this->skelAnime, this->skelAnime.jointTable,
                                             this->upperSkelanime.jointTable, sJointCopyFlags);
                break;
        }

        if ((this->actor.colorFilterTimer == 0) && (this->actor.colChkInfo.health != 0)) {
            if ((this->unk_7C8 != 0x10) && (this->unk_7C8 != 0x17)) {
                EnTest_UpdateHeadRot(this, play);
            } else {
                Math_SmoothStepToS(&this->headRot.y, 0, 1, 0x3E8, 0);
            }
        }
    }

    Collider_UpdateCylinder(&this->actor, &this->bodyCollider);

    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += 45.0f;

    if ((this->actor.colChkInfo.health > 0) || (this->actor.colorFilterTimer != 0)) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyCollider.base);

        s16 inFullShield = IS_FULL_SHIELDING;

        if ((this->unk_7C8 >= 0xA) && (IS_VULNERABLE || !inFullShield) &&
            ((this->actor.colorFilterTimer == 0) || (!(this->actor.colorFilterParams & 0x4000)))) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodyCollider.base);
        }

        if(inFullShield) {
            this->shieldCollider.dim.pos.x = this->actor.world.pos.x;
            this->shieldCollider.dim.pos.y = this->actor.world.pos.y + 55.0f;
            this->shieldCollider.dim.pos.z = this->actor.world.pos.z;
            this->shieldCollider.dim.radius = 25.0f;
        }

        if (this->shieldState != 0 && !IS_VULNERABLE) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->shieldCollider.base);
        }
    }

    if (this->swordState >= 1) {
        if (!(this->swordCollider.base.atFlags & AT_BOUNCED)) {
            if (!(this->variant & FLAME_SWORD_PARAM)) {
                if (this->actionFunc == EnTest_Thrust)
                    this->swordCollider.info.toucher.dmgFlags = 0xFFDFFFFF;
                else if (this->actionFunc == EnTest_SpinAttack)
                    this->swordCollider.info.toucher.dmgFlags = 0x00100000;
                else
                    this->swordCollider.info.toucher.dmgFlags = 0xFFCFFFFF;

                s16 angleToPlayer = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
                if (angleToPlayer == (s16)0x8000)
                    angleToPlayer = (s16)0x7FFF;
                angleToPlayer = ABS(angleToPlayer);
                s16 playerAngleDifference = player->actor.shape.rot.y - this->actor.shape.rot.y;
                if (playerAngleDifference == (s16)0x8000)
                    playerAngleDifference = (s16)0x7FFF;
                playerAngleDifference = ABS(playerAngleDifference);

                //Make sword attack unblockable if behind player.
                //or using shield bash
                if ((angleToPlayer < 0x4000 && playerAngleDifference < 0x4000 && !Player_IsChildWithHylianShield(player)) ||
                    (this->actionFunc == EnTest_ShieldBash)) {
                    this->swordCollider.info.toucher.dmgFlags = 0x20000000;
                }
            }

            if ((this->swordCollider.base.atFlags & AT_HIT))
                Player_SetShieldRecoveryDefault(play);

            CollisionCheck_SetAT(play, &play->colChkCtx, &this->swordCollider.base);
        } else {
            this->swordCollider.base.atFlags &= ~AT_BOUNCED;
            if (this->actionFunc == EnTest_SpinAttack)
                Player_SetShieldRecoveryDefault(play);
            EnTest_SetupRecoil(this);
        }
    }

    if (this->swordCollider.base.atFlags & AT_HIT) {
        this->swordCollider.base.atFlags &= ~AT_HIT;
        this->hitTracker = 1;
    }

    if (this->actor.params == STALFOS_TYPE_INVISIBLE) {
        if (play->actorCtx.lensActive) {
            this->actor.flags |= ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_LENS;
            this->actor.shape.shadowDraw = ActorShadow_DrawFeet;
        } else {
            this->actor.flags &= ~(ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_LENS);
            this->actor.shape.shadowDraw = NULL;
        }
    }
}

s32 EnTest_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnTest* this = (EnTest*)thisx;
    s32 pad;

    if (limbIndex == STALFOS_LIMB_HEAD_ROOT) {
        rot->x += this->headRot.y;
        rot->y -= this->headRot.x;
        rot->z += this->headRot.z;
    } else if (limbIndex == STALFOS_LIMB_HEAD) {
        OPEN_DISPS(play->state.gfxCtx);

        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, 80 + ABS((s16)(Math_SinS(play->gameplayFrames * 2000) * 175.0f)), 0, 0,
                       255);

        CLOSE_DISPS(play->state.gfxCtx);
    } else if (limbIndex == STALFOS_LIMB_SWORD) {
        if (this->variant & FLAME_SWORD_PARAM) {
            this->flameTimer++;
            OPEN_DISPS(play->state.gfxCtx);
            Gfx_SetupDL_25Xlu(play->state.gfxCtx);
            gSPSegment(
                POLY_XLU_DISP++, 0x08,
                Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 32, 64, 1, 0, (this->flameTimer * -20) & 511, 32, 128));
            gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 0, 170, 255, 255);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 255, 0);
            Matrix_Push();
                Matrix_RotateZ(M_PI, MTXMODE_APPLY);
                Matrix_Translate(-2000.0f, -700.0f, 100.0f, MTXMODE_APPLY);
                Matrix_Scale(0.65f, 0.1f, 1.0f, MTXMODE_APPLY);
                gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                        G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
                Matrix_Translate(0, 0, -200.0f, MTXMODE_APPLY);
                gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                        G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
            Matrix_Pop();
            CLOSE_DISPS(play->state.gfxCtx);
        }
    }

    if ((this->actor.params == STALFOS_TYPE_INVISIBLE) && !CHECK_FLAG_ALL(this->actor.flags, ACTOR_FLAG_LENS)) {
        *dList = NULL;
    }

    return false;
}

#define SWORD_EXTENSION_LENGTH 7000.0f

void EnTest_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f unused1 = { 1100.0f, -700.0f, 0.0f };
    static Vec3f D_80864658 = { 300.0f, 0.0f, 0.0f };
    static Vec3f D_80864664 = { SWORD_EXTENSION_LENGTH, 0.0f, 0.0f };
    static Vec3f D_80864670 = { 0.0f, 0.0f, 0.0f };
    static Vec3f D_8086467C = { SWORD_EXTENSION_LENGTH, 1000.0f, 0.0f };
    static Vec3f D_80864688 = { -2000.0f, -1000.0f, 0.0f };
    static Vec3f D_80864694 = { -2000.0f, 1000.0f, 0.0f };
    static Vec3f D_808646A0 = { SWORD_EXTENSION_LENGTH, -3000.0f, 0.0f };
    static Vec3f unused2 = { -3000.0f, 1900.0f, 800.0f };
    static Vec3f unused3 = { -3000.0f, -1100.0f, 800.0f };
    static Vec3f unused4 = { 1900.0f, 1900.0f, 800.0f };
    static Vec3f unused5 = { -3000.0f, -1100.0f, 800.0f };
    static Vec3f unused6 = { 1900.0f, -1100.0f, 800.0f };
    static Vec3f unused7 = { 1900.0f, 1900.0f, 800.0f };
    s32 bodyPart = -1;
    Vec3f sp70;
    Vec3f sp64;
    EnTest* this = (EnTest*)thisx;
    s32 pad;
    Vec3f sp50;

    BodyBreak_SetInfo(&this->bodyBreak, limbIndex, 0, 60, 60, dList, BODYBREAK_OBJECT_DEFAULT);

    if (limbIndex == STALFOS_LIMB_SWORD) {
        Matrix_MultVec3f(&D_80864694, &this->swordCollider.dim.quad[0]);
        Matrix_MultVec3f(&D_80864688, &this->swordCollider.dim.quad[1]);
        Matrix_MultVec3f(&D_8086467C, &this->swordCollider.dim.quad[2]);
        Matrix_MultVec3f(&D_808646A0, &this->swordCollider.dim.quad[3]);

        Collider_SetQuadVertices(&this->swordCollider, &this->swordCollider.dim.quad[0],
                                 &this->swordCollider.dim.quad[1], &this->swordCollider.dim.quad[2],
                                 &this->swordCollider.dim.quad[3]);

        Matrix_MultVec3f(&D_80864664, &sp70);
        Matrix_MultVec3f(&D_80864670, &sp64);

        if ((this->swordState >= 1) &&
            ((this->actor.params != STALFOS_TYPE_INVISIBLE) || play->actorCtx.lensActive)) {
            EffectBlure_AddVertex(Effect_GetByIndex(this->effectIndex), &sp70, &sp64);
        } else if (this->swordState >= 0) {
            EffectBlure_AddSpace(Effect_GetByIndex(this->effectIndex));
            this->swordState = -1;
        }

    } else if ((limbIndex == STALFOS_LIMB_SHIELD) && (this->shieldState != 0)) {
        if (!IS_FULL_SHIELDING) {
            Matrix_MultVec3f(&D_80864670, &sp64);

            this->shieldCollider.dim.pos.x = sp64.x;
            this->shieldCollider.dim.pos.y = sp64.y;
            this->shieldCollider.dim.pos.z = sp64.z;
            this->shieldCollider.dim.radius = 20.0f;
        }
    } else {
        Actor_SetFeetPos(&this->actor, limbIndex, STALFOS_LIMB_FOOT_L, &D_80864658, STALFOS_LIMB_ANKLE_R, &D_80864658);

        if ((limbIndex == STALFOS_LIMB_FOOT_L) || (limbIndex == STALFOS_LIMB_ANKLE_R)) {
            if ((this->unk_7C8 == 0x15) || (this->unk_7C8 == 0x16)) {
                if (this->actor.speedXZ != 0.0f) {
                    Matrix_MultVec3f(&D_80864658, &sp64);
                    Actor_SpawnFloorDustRing(play, &this->actor, &sp64, 10.0f, 1, 8.0f, 100, 15, false);
                }
            }
        }
    }

    if (this->iceTimer != 0) {
        switch (limbIndex) {
            case STALFOS_LIMB_HEAD:
                bodyPart = 0;
                break;
            case STALFOS_LIMB_CHEST:
                bodyPart = 1;
                break;
            case STALFOS_LIMB_SWORD:
                bodyPart = 2;
                break;
            case STALFOS_LIMB_SHIELD:
                bodyPart = 3;
                break;
            case STALFOS_LIMB_UPPERARM_R:
                bodyPart = 4;
                break;
            case STALFOS_LIMB_UPPERARM_L:
                bodyPart = 5;
                break;
            case STALFOS_LIMB_WAIST:
                bodyPart = 6;
                break;
            case STALFOS_LIMB_FOOT_L:
                bodyPart = 7;
                break;
            case STALFOS_LIMB_FOOT_R:
                bodyPart = 8;
                break;
        }

        if (bodyPart >= 0) {
            Matrix_MultVec3f(&D_80864670, &sp50);

            this->bodyPartsPos[bodyPart].x = sp50.x;
            this->bodyPartsPos[bodyPart].y = sp50.y;
            this->bodyPartsPos[bodyPart].z = sp50.z;
        }
    }
}

void EnTest_Draw(Actor* thisx, PlayState* play) {
    EnTest* this = (EnTest*)thisx;

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    func_8002EBCC(&this->actor, play, 1);

    if ((thisx->params <= STALFOS_TYPE_CEILING) || (thisx->child == NULL)) {
        SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnTest_OverrideLimbDraw,
                          EnTest_PostLimbDraw, this);
    }

    if (this->iceTimer != 0) {
        thisx->colorFilterTimer++;
        this->iceTimer--;

        if ((this->iceTimer % 4) == 0) {
            s32 iceIndex = this->iceTimer >> 2;

            EffectSsEnIce_SpawnFlyingVec3s(play, thisx, &this->bodyPartsPos[iceIndex], 150, 150, 150, 250, 235,
                                           245, 255, 1.5f);
        }
    }
}

// a variation of sidestep
void func_80864158(EnTest* this, f32 xzSpeed) {
    Animation_MorphToLoop(&this->skelAnime, &gStalfosSidestepAnim, -2.0f);
    this->actor.speedXZ = xzSpeed;
    this->actor.world.rot.y = this->actor.shape.rot.y + 0x3FFF;
    this->timer = (Rand_ZeroOne() * 20.0f) + 15.0f;
    this->unk_7C8 = 0x18;
    EnTest_SetupAction(this, func_808628C8);
}

/**
 * Check if a projectile actor is within 300 units and react accordingly.
 * Returns true if the projectile test passes and a new action is performed.
 */
s32 EnTest_ReactToProjectile(PlayState* play, EnTest* this) {
    Actor* projectileActor;
    s16 yawToProjectile;
    s16 wallYawDiff;
    s16 touchingWall;
    s16 directionFlag;

    projectileActor = Actor_GetProjectileActor(play, &this->actor, 300.0f);

    if (projectileActor != NULL) {
        yawToProjectile = Actor_WorldYawTowardActor(&this->actor, projectileActor) - (u16)this->actor.shape.rot.y;

        if ((u8)(this->actor.bgCheckFlags & 8)) {
            wallYawDiff = ((u16)this->actor.wallYaw - (u16)this->actor.shape.rot.y);
            touchingWall = true;
        } else {
            touchingWall = false;
        }

        if (Math_Vec3f_DistXYZ(&this->actor.world.pos, &projectileActor->world.pos) < 200.0f) {
            if (Actor_IsTargeted(play, &this->actor) && (projectileActor->id == ACTOR_ARMS_HOOK)) {
                EnTest_SetupJumpUp(this);
            } else if (ABS(yawToProjectile) < 0x2000) {
                EnTest_SetupStopAndBlock(this);
            } else if (ABS(yawToProjectile) < 0x6000) {
                EnTest_SetupJumpBack(this);
            } else {
                EnTest_SetupJumpUp(this);
            }

            return true;
        }

        if (Actor_IsTargeted(play, &this->actor) && (projectileActor->id == ACTOR_ARMS_HOOK)) {
            EnTest_SetupJumpUp(this);
            return true;
        }

        if ((ABS(yawToProjectile) < 0x2000) || (ABS(yawToProjectile) > 0x6000)) {
            directionFlag = play->gameplayFrames % 2;

            if (touchingWall && (wallYawDiff > 0x2000) && (wallYawDiff < 0x6000)) {
                directionFlag = false;
            } else if (touchingWall && (wallYawDiff < -0x2000) && (wallYawDiff > -0x6000)) {
                directionFlag = true;
            }

            if (directionFlag) {
                func_80864158(this, 4.0f);
            } else {
                func_80864158(this, -4.0f);
            }
        } else if (ABS(yawToProjectile) < 0x6000) {
            directionFlag = play->gameplayFrames % 2;

            if (touchingWall && (ABS(wallYawDiff) > 0x6000)) {
                directionFlag = false;
            } else if (touchingWall && (ABS(wallYawDiff) < 0x2000)) {
                directionFlag = true;
            }

            if (directionFlag) {
                EnTest_SetupJumpBack(this);
            } else {
                EnTest_SetupJumpUp(this);
            }
        }

        return true;
    }

    return false;
}
