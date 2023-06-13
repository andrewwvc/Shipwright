/*
 * File: z_en_ik.c
 * Overlay: ovl_En_Ik
 * Description: Iron Knuckle
 */

#include "z_en_ik.h"
#include "scenes/dungeons/jyasinboss/jyasinboss_scene.h"
#include "objects/object_ik/object_ik.h"
#include "vt.h"

// s16 gIronKnuckleSkelGironknucklehorizontalattackanimAnimFrameData[1161] = {
// 	0x0000, 0xffff, 0x4000, 0xa3a8, 0xf5d5, 0x0bfc, 0x0cea, 0x0cdb, 0x0cce, 0x0cc2, 0x0cb8, 0x0cb0, 0x0ca8, 0x0ca2, 0x0c9c, 0x0c96, 
// 	0x0c90, 0x0c8b, 0x0c85, 0x0c86, 0x0c8d, 0x0c8d, 0x0c8d, 0x0c78, 0x0c6a, 0x0c5c, 0x0c52, 0xfe62, 0xfe81, 0xfea6, 0xfed1, 0xff02, 
// 	0xff36, 0xff6e, 0xffa8, 0xffe3, 0x001f, 0x005b, 0x0095, 0x00cd, 0x0108, 0x014c, 0x014c, 0x014c, 0x0259, 0x02c4, 0x0323, 0x0366, 
// 	0x019c, 0x041c, 0x0668, 0x079c, 0x0929, 0x0c1f, 0x0f91, 0x12e6, 0x1585, 0x1651, 0x1585, 0x12fd, 0x10f4, 0x10ed, 0x1da6, 0x1da6, 
// 	0x1da6, 0x1276, 0x12f6, 0x1363, 0x13b0, 0x277c, 0x24d5, 0x21a7, 0x201d, 0x1a95, 0x0de9, 0xfe8b, 0xef79, 0xe3af, 0xe05f, 0xe3af, 
// 	0xea4a, 0xf418, 0x0220, 0x122b, 0x122b, 0x122b, 0x1834, 0x1834, 0x1834, 0x1834, 0x000d, 0x0017, 0xffe8, 0xffd3, 0xfe37, 0xf9f1, 
// 	0xf4aa, 0xef76, 0xeb66, 0xea06, 0xeb66, 0xf018, 0xf4ce, 0xf746, 0xff20, 0xff20, 0xff20, 0x0101, 0x02e6, 0x0461, 0x0557, 0xb2e3, 
// 	0xb06e, 0xae48, 0xad10, 0xac51, 0xab3b, 0xab0a, 0xab5d, 0xac69, 0xae62, 0xb177, 0xb5c6, 0xbb40, 0xc189, 0xcd76, 0xcd76, 0xcd76, 
// 	0xd6ce, 0xd99f, 0xdb84, 0xdc9c, 0xf738, 0xf8b4, 0xfa9b, 0xfc1c, 0xfceb, 0xfcb8, 0xf997, 0xf600, 0xf21b, 0xee1d, 0xea50, 0xe707, 
// 	0xe49a, 0xe351, 0xdc1a, 0xdc1a, 0xdc1a, 0xe84a, 0xea75, 0xec48, 0xed84, 0xc548, 0xc5e1, 0xc63d, 0xc659, 0xc662, 0xc67b, 0xc6be, 
// 	0xc705, 0xc72b, 0xc6ff, 0xc643, 0xc4c1, 0xc26a, 0xbf75, 0xb5d4, 0xb5d4, 0xb5d4, 0xb694, 0xb5fe, 0xb5c9, 0xb5c1, 0x0334, 0x020f, 
// 	0x0111, 0x027f, 0x0409, 0x03bd, 0x054a, 0x07e0, 0x09b5, 0x0a24, 0x09cf, 0x0890, 0x0725, 0x0668, 0x05ce, 0x0567, 0x0530, 0x051e, 
// 	0x0522, 0x0532, 0x0541, 0x0706, 0x084a, 0x09fe, 0x0ba0, 0x0d93, 0x0d83, 0x08bc, 0x026e, 0xfd7b, 0xfabb, 0xf99a, 0xfb4f, 0xfde0, 
// 	0xfeab, 0xfee9, 0xffaf, 0x018a, 0x0312, 0x0436, 0x04f8, 0x0563, 0x0a1b, 0x0531, 0xffc4, 0xfc81, 0xf742, 0xeed8, 0xebc3, 0xee2e, 
// 	0xf4e3, 0xfbdf, 0x0157, 0x05e1, 0x09cf, 0x0d07, 0x10b9, 0x129f, 0x11b4, 0x1038, 0x0ea9, 0x0d62, 0x0c97, 0x1a79, 0x1e47, 0x21bd, 
// 	0x23bf, 0x25e2, 0x27f9, 0x286f, 0x26e8, 0x23dc, 0x20d8, 0x1eb8, 0x1e3e, 0x1df2, 0x1c05, 0x1850, 0x1630, 0x1756, 0x1886, 0x195f, 
// 	0x19c3, 0x19d2, 0x0606, 0x061b, 0x0613, 0x060e, 0x05ec, 0x0591, 0x051d, 0x04bb, 0x0496, 0x04f7, 0x05ac, 0x062d, 0x0625, 0x05e7, 
// 	0x05ea, 0x06a6, 0x079f, 0x087d, 0x0973, 0x0a58, 0x0b00, 0x0110, 0x0189, 0x0200, 0x023b, 0x022c, 0x01bf, 0x012c, 0x009c, 0x003d, 
// 	0x0032, 0x0062, 0x009b, 0x00d9, 0x0121, 0x0150, 0x012a, 0x0110, 0x0155, 0x01b9, 0x0220, 0x0271, 0xaaa1, 0xa72a, 0xa3f7, 0xa263, 
// 	0xa2bf, 0xa584, 0xa948, 0xacf0, 0xaf5f, 0xafa8, 0xae6d, 0xacef, 0xab5a, 0xa980, 0xa81c, 0xa845, 0xa852, 0xa6bd, 0xa49e, 0xa278, 
// 	0xa0cd, 0xfa66, 0xfb2c, 0xfbe1, 0xfbe1, 0xfc1a, 0xfd91, 0x0080, 0x056a, 0x0c35, 0x0e4f, 0x0b79, 0x0680, 0x03a8, 0x01f3, 0xff3c, 
// 	0xfe2a, 0xfeb0, 0xff28, 0xff8c, 0xffd6, 0x0004, 0xfa0e, 0xfa61, 0xfa84, 0xfa4f, 0xf95f, 0xf7da, 0xf5ef, 0xf249, 0xeb85, 0xe71f, 
// 	0xe576, 0xe79b, 0xec2a, 0xf17b, 0xf7f6, 0xfa52, 0xf94a, 0xf87d, 0xf7e8, 0xf784, 0xf74c, 0xf963, 0xf786, 0xf5f0, 0xf4a9, 0xf1e2, 
// 	0xee3f, 0xe8d1, 0xe148, 0xdf29, 0xe3cc, 0xea30, 0xf0f5, 0xf292, 0xeef9, 0xed2a, 0xee04, 0xeee5, 0xef17, 0xeeeb, 0xeea2, 0xee6b, 
// 	0x1a2b, 0x176a, 0x141c, 0x129d, 0x1737, 0x219e, 0x35fb, 0x47f0, 0x4c08, 0x43ab, 0x3644, 0x2a7f, 0x27a1, 0x2ae7, 0x2c25, 0x2bca, 
// 	0x2d09, 0x2ea3, 0x3036, 0x317e, 0x3256, 0xf9c9, 0xf969, 0xf8ee, 0xf8b0, 0xf87f, 0xf82c, 0xf821, 0xf870, 0xf8b4, 0xf908, 0xf987, 
// 	0xfa4c, 0xfb72, 0xff60, 0x0321, 0x01e2, 0xff80, 0xfe2b, 0xfccd, 0xfb95, 0xfab6, 0xff5d, 0xffbb, 0xff7e, 0xff4e, 0xff1c, 0xfec3, 
// 	0xfe91, 0xfea6, 0xfec3, 0xfee9, 0xff1a, 0xff5a, 0xffaa, 0x006b, 0x0154, 0x0206, 0x0256, 0x0215, 0x01a3, 0x0125, 0x00be, 0xb7b4, 
// 	0xba88, 0xb930, 0xb807, 0xb6af, 0xb454, 0xb2a0, 0xb291, 0xb2ee, 0xb37a, 0xb3fc, 0xb438, 0xb3f4, 0xb14a, 0xade3, 0xac9a, 0xabb0, 
// 	0xaa1b, 0xa83f, 0xa679, 0xa524, 0x1d01, 0x0da4, 0xfff4, 0xf8d3, 0xf45c, 0xf03b, 0xec70, 0xe922, 0xe674, 0xe4b1, 0xe674, 0xebef, 
// 	0xf383, 0xfe0e, 0x157d, 0x1735, 0x1a08, 0x3381, 0x34d9, 0x3356, 0x3153, 0x0352, 0x0521, 0x069d, 0x0769, 0x0743, 0x0617, 0x0480, 
// 	0x02e2, 0x01a2, 0x01a6, 0x01a2, 0xffed, 0xfdc8, 0xfc99, 0x03f1, 0x039d, 0xff55, 0xfeb7, 0x00bb, 0x00b6, 0x00a2, 0x41bc, 0x4245, 
// 	0x42c5, 0x4309, 0x437b, 0x4473, 0x459a, 0x46b9, 0x479a, 0x48b1, 0x479a, 0x41a5, 0x3a0e, 0x3542, 0x4939, 0x4c5d, 0x4d78, 0x3f56, 
// 	0x3fe4, 0x41a3, 0x42e4, 0xff9b, 0xff8c, 0xff7e, 0xff79, 0xff7d, 0xff84, 0xff8c, 0xff87, 0xff6c, 0xff46, 0xff1f, 0xff02, 0xfefb, 
// 	0xfeff, 0xf947, 0xfaa1, 0xfea6, 0xfeb1, 0xfec9, 0xfee6, 0xfefd, 0x1510, 0x17dc, 0x1a12, 0x1942, 0x12be, 0x0555, 0xf54f, 0xea66, 
// 	0xe64e, 0xe540, 0xe6bc, 0xea3e, 0xf18b, 0xfcc1, 0x0a86, 0x1938, 0x242f, 0x28af, 0x2ba8, 0x2d68, 0x2e3b, 0x3f36, 0x3f18, 0x3efe, 
// 	0x3ef6, 0x3f09, 0x3f30, 0x3f60, 0x3f88, 0x3fa5, 0x3fc0, 0x3fd1, 0x3fd2, 0x3fb8, 0x3f84, 0x3868, 0x3924, 0x3e72, 0x3e5e, 0x3e59, 
// 	0x3e5e, 0x3e66, 0x8d4e, 0x8e33, 0x8e18, 0xf31f, 0x1062, 0xeaeb, 0xe71e, 0xe5c6, 0xe49b, 0xe32b, 0xe155, 0xdef5, 0xdb4d, 0x433a, 
// 	0x13f8, 0x0f4d, 0xeb9f, 0x6a7e, 0x6ab5, 0x6ab5, 0x6ab5, 0x0038, 0xfc16, 0xf2f1, 0x4f6e, 0x34af, 0x3324, 0x332a, 0x3268, 0x3122, 
// 	0x2ff3, 0x2f7b, 0x305a, 0x332a, 0x4779, 0x48ee, 0x59a3, 0x5de0, 0x07b2, 0xfe90, 0xfe90, 0xfe90, 0x3f5c, 0x37d1, 0x3ba1, 0xa749, 
// 	0xc63e, 0xa8a7, 0xa591, 0xa61c, 0xa73c, 0xa83d, 0xa8ac, 0xa81c, 0xa591, 0x1955, 0xe335, 0xdb9c, 0xb87c, 0x30a2, 0x3afd, 0x3afd, 
// 	0x3afd, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0xffff, 0x0000, 
// 	0x0159, 0xfc7d, 0x0205, 0xffff, 0xffff, 0xffff, 0x107f, 0x0bc3, 0x0818, 0x071f, 0x07a8, 0x082c, 0x08d6, 0x0996, 0x0a5a, 0x0b12, 
// 	0x0baa, 0x0c12, 0x0c39, 0x0c39, 0x0c39, 0x0567, 0x03ab, 0x0c94, 0x0c39, 0x0c39, 0x0c39, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 
// 	0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0cc4, 0xfa4f, 0xed31, 0x0147, 0x0147, 0x0147, 
// 	0xfa3f, 0xf541, 0xef00, 0xfbb3, 0xf993, 0xf4c6, 0xee0e, 0xebad, 0xf3e7, 0x0984, 0x27d8, 0x49fe, 0x6b0c, 0x861b, 0x9d22, 0x9593, 
// 	0x9311, 0xa3a6, 0xa93a, 0xb5f7, 0xbcef, 0xfc01, 0x0208, 0xfa51, 0xc73a, 0xcca3, 0xd0d5, 0xd66c, 0xdc4a, 0xe14e, 0xe565, 0xe934, 
// 	0xecb3, 0xefd7, 0xf297, 0xf0ab, 0xf1d4, 0x1275, 0x2403, 0x240c, 0x310a, 0x3a37, 0xf9ff, 0xf9e2, 0x0997, 0x05aa, 0x05b1, 0x05ff, 
// 	0x0665, 0x067c, 0x05d8, 0x0444, 0x020f, 0xff99, 0xfd44, 0xfb71, 0x071c, 0x1ff6, 0x2f70, 0x3058, 0x2eee, 0x3436, 0x40f8, 0xaf8a, 
// 	0x0dbd, 0xa256, 0xa429, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 
// 	0xa256, 0xa256, 0xa256, 0xa256, 0xbb99, 0xbb19, 0xc82e, 0xc932, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 
// 	0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0x23d8, 0xa90b, 0x0f90, 0x0c93, 0x0f90, 0x0f90, 0x0f90, 
// 	0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0000, 0x479e, 
// 	0x3b06, 0xffff, 0x0000, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0xffff, 
// 	0xffff, 0xffff, 0xffff, 0xc3f4, 0xd881, 0xd959, 0xbba6, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 
// 	0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0x0147, 0xbfe2, 0xbe14, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 
// 	0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0xabfb, 0xaa7f, 0xa970, 
// 	0xa8fa, 0xa890, 0xa7d8, 0xa722, 0xa6fe, 0xa79e, 0xa8e6, 0xaaec, 0xad8f, 0xb05a, 0xb295, 0xb379, 0xb379, 0xb379, 0xb379, 0xb379, 
// 	0xb379, 0xb379, 0xc862, 0xc97d, 0xca5f, 0xcae5, 0xcb27, 0xcbaf, 0xcc36, 0xcc3e, 0xcbb9, 0xcb03, 0xca46, 0xc9a5, 0xc935, 0xc8f7, 
// 	0xc8e4, 0xc8e4, 0xc8e4, 0xc8e4, 0xc8e4, 0xc8e4, 0xc8e4, 0x126f, 0x142e, 0x156f, 0x1606, 0x1659, 0x16f6, 0x1777, 0x16cb, 0x1492, 
// 	0x1141, 0x0d02, 0x0837, 0x0394, 0x0012, 0xfeb1, 0xfeb1, 0xfeb1, 0xfeb1, 0xfeb1, 0xfeb1, 0xfeb1, 0xbae3, 0xb6f0, 0xb35c, 0xb17c, 
// 	0xafe0, 0xacd8, 0xa9b4, 0xa978, 0xacee, 0xb27e, 0xb9b4, 0xc1c6, 0xc973, 0xcf28, 0xd15d, 0xd15d, 0xd15d, 0xd15d, 0xd15d, 0xd15d, 
// 	0xd15d, 0xcac2, 0xcb98, 0xcc21, 0xcc81, 0xcc70, 0xcc69, 0xcc4b, 0xcbe2, 0xcb23, 0xca41, 0xc969, 0xc8bf, 0xc854, 0xc823, 0xc817, 
// 	0xc817, 0xc817, 0xc817, 0xc817, 0xc817, 0xc817, 0xff36, 0xff67, 0xffa8, 0xffbd, 0xffd3, 0xffff, 0x0018, 0xff21, 0xfc9e, 0xf8d2, 
// 	0xf3dd, 0xee2e, 0xe8ab, 0xe489, 0xe2ef, 0xe2ef, 0xe2ef, 0xe2ef, 0xe2ef, 0xe2ef, 0xe2ef, 0x2e58, 0x343d, 0x381b, 0x3986, 0x59a2, 
// 	0xfe8d, 0xfeb8, 0xff04, 0xff6e, 0xffdd, 0x0031, 0x0044, 0xfffb, 0xff6a, 0x0677, 0xfffa, 0x00b2, 0x006a, 0x0073, 0x0073, 0x0073, 
// 	0x4f18, 0x4f81, 0x4e33, 0x495c, 0x4441, 0x33c0, 0x33b2, 0x3420, 0x34f5, 0x3618, 0x3772, 0x38ec, 0x3a6e, 0x3be2, 0x405f, 0x454d, 
// 	0x4d23, 0x60b7, 0x6d54, 0x6d54, 0x6d54, 0xd4f0, 0xd472, 0xd58f, 0xdcb8, 0x07a5, 0xbbeb, 0xbceb, 0xbd5b, 0xbd5e, 0xbd09, 0xbc61, 
// 	0xbb6e, 0xba40, 0xb912, 0xc01c, 0xb41b, 0xaeab, 0xab93, 0xae92, 0xae92, 0xae92, 0xbd37, 0xbc63, 0xb9c3, 0x3251, 0x30b1, 0xf6a3, 
// 	0xf5f5, 0xf4f2, 0xf3aa, 0xf1fd, 0xf070, 0xedb0, 0xea2f, 0xe93b, 0x5bd4, 0x0d9e, 0xfd14, 0x8101, 0x7de8, 0x7de8, 0x7de8, 0x1df0, 
// 	0x1a3c, 0x19f1, 0x5cad, 0x4c83, 0x2e41, 0x2be0, 0x2bc6, 0x2d0b, 0x2ec9, 0x303b, 0x315c, 0x3334, 0x3716, 0x440c, 0x44de, 0x51e8, 
// 	0x0a30, 0xfa43, 0xfa43, 0xfa43, 0x5b3a, 0x5402, 0x50d0, 0xd496, 0xe3e6, 0xbea2, 0xbf34, 0xbdcf, 0xbb17, 0xb77d, 0xb42f, 0xb02d, 
// 	0xaac2, 0xa5df, 0x1383, 0xbefe, 0xa587, 0x2296, 0x2fcc, 0x2fcc, 0x2fcc, };

// JointIndex gIronKnuckleSkelGironknucklehorizontalattackanimAnimJointIndices[30] = {
// 	{ 0x0000, 0x0006, 0x001b, },
// 	{ 0x0030, 0x0045, 0x005a, },
// 	{ 0x006f, 0x0084, 0x0099, },
// 	{ 0x00ae, 0x00c3, 0x00d8, },
// 	{ 0x0001, 0x0000, 0x00ed, },
// 	{ 0x0102, 0x0117, 0x012c, },
// 	{ 0x0141, 0x0156, 0x016b, },
// 	{ 0x0000, 0x0001, 0x0180, },
// 	{ 0x0195, 0x01aa, 0x01bf, },
// 	{ 0x0001, 0x0002, 0x0000, },
// 	{ 0x01d4, 0x01e9, 0x01fe, },
// 	{ 0x0213, 0x0228, 0x023d, },
// 	{ 0x0001, 0x0001, 0x0000, },
// 	{ 0x0001, 0x0001, 0x0000, },
// 	{ 0x0252, 0x0267, 0x027c, },
// 	{ 0x0291, 0x02a6, 0x02bb, },
// 	{ 0x02d0, 0x02e5, 0x02fa, },
// 	{ 0x0000, 0x0000, 0x0001, },
// 	{ 0x0000, 0x0000, 0x0001, },
// 	{ 0x030f, 0x0324, 0x0339, },
// 	{ 0x034e, 0x0363, 0x0378, },
// 	{ 0x0003, 0x0004, 0x0005, },
// 	{ 0x038d, 0x03a2, 0x03b7, },
// 	{ 0x03cc, 0x03e1, 0x03f6, },
// 	{ 0x040b, 0x0420, 0x0435, },
// 	{ 0x044a, 0x045f, 0x0474, },
// 	{ 0x0001, 0x0000, 0x0001, },
// 	{ 0x0001, 0x0000, 0x0001, },
// 	{ 0x0001, 0x0000, 0x0001, },
// 	{ 0x0000, 0x0001, 0x0001, },
// };

// AnimationHeader gIronKnuckleSkelGironknucklehorizontalattackanimAnim = { { 21 }, gIronKnuckleSkelGironknucklehorizontalattackanimAnimFrameData, gIronKnuckleSkelGironknucklehorizontalattackanimAnimJointIndices, 6 };

s16 gIronKnuckleSkelGironknucklehorizontalattackanimAnimFrameData[1371] = {
	0x0000, 0xffff, 0x4000, 0xa3a8, 0xf5d5, 0x0bfc, 0x0cea, 0x0cdb, 0x0cce, 0x0cc2, 0x0cb8, 0x0cb0, 0x0ca8, 0x0ca2, 0x0c9c, 0x0c96, 
	0x0c90, 0x0c8b, 0x0c85, 0x0c86, 0x0c8d, 0x0c8d, 0x0beb, 0x0c78, 0x0c6a, 0x0c5c, 0x0c52, 0xfe62, 0xfe81, 0xfea6, 0xfed1, 0xff02, 
	0xff36, 0xff6e, 0xffa8, 0xffe3, 0x001f, 0x005b, 0x0095, 0x00cd, 0x0108, 0x014c, 0x014c, 0x13a1, 0x0259, 0x02c4, 0x0323, 0x0366, 
	0x019c, 0x041c, 0x0668, 0x079c, 0x0929, 0x0c1f, 0x0f91, 0x12e6, 0x1585, 0x1651, 0x1585, 0x12fd, 0x10f4, 0x10ed, 0x1da6, 0x1da6, 
	0x1da6, 0x1276, 0x12f6, 0x1363, 0x13b0, 0x277c, 0x24d5, 0x21a7, 0x201d, 0x1a95, 0x0de9, 0xfe8b, 0xef79, 0xe3af, 0xe05f, 0xe3af, 
	0xea4a, 0xf418, 0x0220, 0x122b, 0x122b, 0x122b, 0x1834, 0x1834, 0x1834, 0x1834, 0x000d, 0x0017, 0xffe8, 0xffd3, 0xfe37, 0xf9f1, 
	0xf4aa, 0xef76, 0xeb66, 0xea06, 0xeb66, 0xf018, 0xf4ce, 0xf746, 0xff20, 0xff20, 0xff20, 0x0101, 0x02e6, 0x0461, 0x0557, 0xb2e3, 
	0xb06e, 0xae48, 0xad10, 0xac51, 0xab3b, 0xab0a, 0xab5d, 0xac69, 0xae62, 0xb177, 0xb5c6, 0xbb40, 0xc189, 0xcd76, 0xcd76, 0xc6a1, 
	0xd6ce, 0xd99f, 0xdb84, 0xdc9c, 0xf738, 0xf8b4, 0xfa9b, 0xfc1c, 0xfceb, 0xfcb8, 0xf997, 0xf600, 0xf21b, 0xee1d, 0xea50, 0xe707, 
	0xe49a, 0xe351, 0xdc1a, 0xdc1a, 0xe321, 0xe84a, 0xea75, 0xec48, 0xed84, 0xc548, 0xc5e1, 0xc63d, 0xc659, 0xc662, 0xc67b, 0xc6be, 
	0xc705, 0xc72b, 0xc6ff, 0xc643, 0xc4c1, 0xc26a, 0xbf75, 0xb5d4, 0xb5d4, 0xbdab, 0xb694, 0xb5fe, 0xb5c9, 0xb5c1, 0x0334, 0x020f, 
	0x0111, 0x027f, 0x0409, 0x03bd, 0x054a, 0x07e0, 0x09b5, 0x0a24, 0x09cf, 0x0890, 0x0725, 0x0668, 0x05ce, 0x0567, 0x051d, 0x051e, 
	0x0522, 0x0532, 0x0541, 0x0706, 0x084a, 0x09fe, 0x0ba0, 0x0d93, 0x0d83, 0x08bc, 0x026e, 0xfd7b, 0xfabb, 0xf99a, 0xfb4f, 0xfde0, 
	0xfeab, 0xfee9, 0xffaf, 0xfd28, 0x0312, 0x0436, 0x04f8, 0x0563, 0x0a1b, 0x0531, 0xffc4, 0xfc81, 0xf742, 0xeed8, 0xebc3, 0xee2e, 
	0xf4e3, 0xfbdf, 0x0157, 0x05e1, 0x09cf, 0x0d07, 0x10b9, 0x129f, 0x1f5a, 0x1038, 0x0ea9, 0x0d62, 0x0c97, 0xffff, 0x0000, 0x0000, 
	0x0000, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 0x0000, 0x0000, 0x0588, 0xffff, 0xffff, 
	0x0000, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0x0000, 0xffff, 0x0000, 0xffff, 
	0xffff, 0x0000, 0xfffa, 0xffff, 0x0000, 0xffff, 0x0000, 0x1a79, 0x1e47, 0x21bd, 0x23bf, 0x25e2, 0x27f9, 0x286f, 0x26e8, 0x23dc, 
	0x20d8, 0x1eb8, 0x1e3e, 0x1df2, 0x1c05, 0x1850, 0x1630, 0x0498, 0x1886, 0x195f, 0x19c3, 0x19d2, 0x0606, 0x061b, 0x0613, 0x060e, 
	0x05ec, 0x0591, 0x051d, 0x04bb, 0x0496, 0x04f7, 0x05ac, 0x062d, 0x0625, 0x05e7, 0x05ea, 0x06a6, 0x079f, 0x087d, 0x0973, 0x0a58, 
	0x0b00, 0x0110, 0x0189, 0x0200, 0x023b, 0x022c, 0x01bf, 0x012c, 0x009c, 0x003d, 0x0032, 0x0062, 0x009b, 0x00d9, 0x0121, 0x0150, 
	0x012a, 0x0110, 0x0155, 0x01b9, 0x0220, 0x0271, 0xaaa1, 0xa72a, 0xa3f7, 0xa263, 0xa2bf, 0xa584, 0xa948, 0xacf0, 0xaf5f, 0xafa8, 
	0xae6d, 0xacef, 0xab5a, 0xa980, 0xa81c, 0xa845, 0xa852, 0xa6bd, 0xa49e, 0xa278, 0xa0cd, 0xfa66, 0xfb2c, 0xfbe1, 0xfbe1, 0xfc1a, 
	0xfd91, 0x0080, 0x056a, 0x0c35, 0x0e4f, 0x0b79, 0x0680, 0x03a8, 0x01f3, 0xff3c, 0xfe2a, 0xfdfa, 0xff28, 0xff8c, 0xffd6, 0x0004, 
	0xfa0e, 0xfa61, 0xfa84, 0xfa4f, 0xf95f, 0xf7da, 0xf5ef, 0xf249, 0xeb85, 0xe71f, 0xe576, 0xe79b, 0xec2a, 0xf17b, 0xf7f6, 0xfa52, 
	0xfd44, 0xf87d, 0xf7e8, 0xf784, 0xf74c, 0xf963, 0xf786, 0xf5f0, 0xf4a9, 0xf1e2, 0xee3f, 0xe8d1, 0xe148, 0xdf29, 0xe3cc, 0xea30, 
	0xf0f5, 0xf292, 0xeef9, 0xed2a, 0xee04, 0xe2e7, 0xef17, 0xeeeb, 0xeea2, 0xee6b, 0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 
	0xffff, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0xffff, 0x0000, 0x0000, 0xff78, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 
	0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 0x017c, 
	0x0000, 0x0000, 0x0000, 0xffff, 0x1a2b, 0x176a, 0x141c, 0x129d, 0x1737, 0x219e, 0x35fb, 0x47f0, 0x4c08, 0x43ab, 0x3644, 0x2a7f, 
	0x27a1, 0x2ae7, 0x2c25, 0x2bca, 0x3754, 0x2ea3, 0x3036, 0x317e, 0x3256, 0xf9c9, 0xf969, 0xf8ee, 0xf8b0, 0xf87f, 0xf82c, 0xf821, 
	0xf870, 0xf8b4, 0xf908, 0xf987, 0xfa4c, 0xfb72, 0xff60, 0x0321, 0x01e2, 0xff80, 0xfe2b, 0xfccd, 0xfb95, 0xfab6, 0xff5d, 0xffbb, 
	0xff7e, 0xff4e, 0xff1c, 0xfec3, 0xfe91, 0xfea6, 0xfec3, 0xfee9, 0xff1a, 0xff5a, 0xffaa, 0x006b, 0x0154, 0x0206, 0x0256, 0x0215, 
	0x01a3, 0x0125, 0x00be, 0xb7b4, 0xba88, 0xb930, 0xb807, 0xb6af, 0xb454, 0xb2a0, 0xb291, 0xb2ee, 0xb37a, 0xb3fc, 0xb438, 0xb3f4, 
	0xb14a, 0xade3, 0xac9a, 0xabb0, 0xaa1b, 0xa83f, 0xa679, 0xa524, 0x1d01, 0x0da4, 0xfff4, 0xf8d3, 0xf45c, 0xf03b, 0xec70, 0xe922, 
	0xe674, 0xe4b1, 0xe674, 0xebef, 0xf383, 0xfe0e, 0x157d, 0x1735, 0x1cc3, 0x3381, 0x34d9, 0x3356, 0x3153, 0x0352, 0x0521, 0x069d, 
	0x0769, 0x0743, 0x0617, 0x0480, 0x02e2, 0x01a2, 0x01a6, 0x01a2, 0xffed, 0xfdc8, 0xfc99, 0x03f1, 0x039d, 0x0074, 0xfeb7, 0x00bb, 
	0x00b6, 0x00a2, 0x41bc, 0x4245, 0x42c5, 0x4309, 0x437b, 0x4473, 0x459a, 0x46b9, 0x479a, 0x48b1, 0x479a, 0x41a5, 0x3a0e, 0x3542, 
	0x4939, 0x4c5d, 0x53a2, 0x3f56, 0x3fe4, 0x41a3, 0x42e4, 0xff9b, 0xff8c, 0xff7e, 0xff79, 0xff7d, 0xff84, 0xff8c, 0xff87, 0xff6c, 
	0xff46, 0xff1f, 0xff02, 0xfefb, 0xfeff, 0xf947, 0xfaa1, 0xfea6, 0xfeb1, 0xfec9, 0xfee6, 0xfefd, 0x1510, 0x17dc, 0x1a12, 0x1942, 
	0x12be, 0x0555, 0xf54f, 0xea66, 0xe64e, 0xe540, 0xe6bc, 0xea3e, 0xf18b, 0xfcc1, 0x0a86, 0x1938, 0x242f, 0x28af, 0x2ba8, 0x2d68, 
	0x2e3b, 0x3f36, 0x3f18, 0x3efe, 0x3ef6, 0x3f09, 0x3f30, 0x3f60, 0x3f88, 0x3fa5, 0x3fc0, 0x3fd1, 0x3fd2, 0x3fb8, 0x3f84, 0x3868, 
	0x3924, 0x3e72, 0x3e5e, 0x3e59, 0x3e5e, 0x3e66, 0x8d4e, 0x8e33, 0x8e18, 0xf31f, 0x1062, 0xeaeb, 0xe71e, 0xe5c6, 0xe49b, 0xe32b, 
	0xe155, 0xdef5, 0xdb4d, 0x433a, 0x13f8, 0x0f4d, 0x5b8d, 0x6a7e, 0x6ab5, 0x6ab5, 0x6ab5, 0x0038, 0xfc16, 0xf2f1, 0x4f6e, 0x34af, 
	0x3324, 0x332a, 0x3268, 0x3122, 0x2ff3, 0x2f7b, 0x305a, 0x332a, 0x4779, 0x48ee, 0x59a3, 0x2324, 0x07b2, 0xfe90, 0xfe90, 0xfe90, 
	0x3f5c, 0x37d1, 0x3ba1, 0xa749, 0xc63e, 0xa8a7, 0xa591, 0xa61c, 0xa73c, 0xa83d, 0xa8ac, 0xa81c, 0xa591, 0x1955, 0xe335, 0xdb9c, 
	0x243e, 0x30a2, 0x3afd, 0x3afd, 0x3afd, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 
	0xffff, 0x0000, 0xffff, 0x0000, 0x0159, 0xfc91, 0x0205, 0xffff, 0xffff, 0xffff, 0x107f, 0x0bc3, 0x0818, 0x071f, 0x07a8, 0x082c, 
	0x08d6, 0x0996, 0x0a5a, 0x0b12, 0x0baa, 0x0c12, 0x0c39, 0x0c39, 0x0c39, 0x0567, 0x01a7, 0x0c94, 0x0c39, 0x0c39, 0x0c39, 0x0147, 
	0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0cc4, 0xfc61, 
	0xed31, 0x0147, 0x0147, 0x0147, 0xfa3f, 0xf541, 0xef00, 0xfbb3, 0xf993, 0xf4c6, 0xee0e, 0xebad, 0xf3e7, 0x0984, 0x27d8, 0x49fe, 
	0x6b0c, 0x861b, 0x9d22, 0x9593, 0x9311, 0xa3a6, 0xa93a, 0xb5f7, 0xbcef, 0xfc01, 0x0208, 0xfa51, 0xc73a, 0xcca3, 0xd0d5, 0xd66c, 
	0xdc4a, 0xe14e, 0xe565, 0xe934, 0xecb3, 0xefd7, 0xf297, 0xf0ab, 0xf1d4, 0x1275, 0x2403, 0x240c, 0x310a, 0x3a37, 0xf9ff, 0xf9e2, 
	0x0997, 0x05aa, 0x05b1, 0x05ff, 0x0665, 0x067c, 0x05d8, 0x0444, 0x020f, 0xff99, 0xfd44, 0xfb71, 0x071c, 0x1ff6, 0x2f70, 0x3058, 
	0x2eee, 0x3436, 0x40f8, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xffff, 0xffff, 0x01cf, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 
	0xffff, 0xffff, 0xffff, 0x0000, 0xffff, 0x0000, 0x0000, 0xffff, 0xf08e, 0xffff, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 0x0000, 
	0xffff, 0xffff, 0xffff, 0x0000, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0xffff, 0x0137, 0xffff, 0xffff, 
	0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xffff, 0x01cf, 0x0000, 0xffff, 0x0000, 0xffff, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 
	0xffff, 0xffff, 0x0000, 0xffff, 0x0000, 0x0000, 0xffff, 0xf08e, 0xffff, 0xffff, 0xffff, 0x0000, 0xffff, 0xffff, 0x0000, 0xffff, 
	0xffff, 0xffff, 0x0000, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0xffff, 0x0137, 0xffff, 0xffff, 0x0000, 
	0xffff, 0xaf8a, 0x0dbd, 0xa256, 0xa429, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 
	0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xa256, 0xbb99, 0xbb19, 0xc82e, 0xc932, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 
	0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0xc82e, 0x23d8, 0xa90b, 0x0f90, 0x0c93, 0x0f90, 
	0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 0x0f90, 
	0x0000, 0x479e, 0x3b06, 0xffff, 0x0000, 0x0000, 0xffff, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0xffff, 0xffff, 
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xc3f4, 0xd881, 0xd959, 0xbba6, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 
	0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0xbc7b, 0x0147, 0xbfe2, 0xbe14, 0x0147, 0x0147, 0x0147, 
	0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0x0147, 0xabfb, 
	0xaa7f, 0xa970, 0xa8fa, 0xa890, 0xa7d8, 0xa722, 0xa6fe, 0xa79e, 0xa8e6, 0xaaec, 0xad8f, 0xb05a, 0xb295, 0xb379, 0xb379, 0xb379, 
	0xb379, 0xb379, 0xb379, 0xb379, 0xc862, 0xc97d, 0xca5f, 0xcae5, 0xcb27, 0xcbaf, 0xcc36, 0xcc3e, 0xcbb9, 0xcb03, 0xca46, 0xc9a5, 
	0xc935, 0xc8f7, 0xc8e4, 0xc8e4, 0xc8e4, 0xc8e4, 0xc8e4, 0xc8e4, 0xc8e4, 0x126f, 0x142e, 0x156f, 0x1606, 0x1659, 0x16f6, 0x1777, 
	0x16cb, 0x1492, 0x1141, 0x0d02, 0x0837, 0x0394, 0x0012, 0xfeb1, 0xfeb1, 0xfeb1, 0xfeb1, 0xfeb1, 0xfeb1, 0xfeb1, 0xbae3, 0xb6f0, 
	0xb35c, 0xb17c, 0xafe0, 0xacd8, 0xa9b4, 0xa978, 0xacee, 0xb27e, 0xb9b4, 0xc1c6, 0xc973, 0xcf28, 0xd15d, 0xd15d, 0xd15d, 0xd15d, 
	0xd15d, 0xd15d, 0xd15d, 0xcac2, 0xcb98, 0xcc21, 0xcc81, 0xcc70, 0xcc69, 0xcc4b, 0xcbe2, 0xcb23, 0xca41, 0xc969, 0xc8bf, 0xc854, 
	0xc823, 0xc817, 0xc817, 0xc817, 0xc817, 0xc817, 0xc817, 0xc817, 0xff36, 0xff67, 0xffa8, 0xffbd, 0xffd3, 0xffff, 0x0018, 0xff21, 
	0xfc9e, 0xf8d2, 0xf3dd, 0xee2e, 0xe8ab, 0xe489, 0xe2ef, 0xe2ef, 0xe2ef, 0xe2ef, 0xe2ef, 0xe2ef, 0xe2ef, 0x2e58, 0x343d, 0x381b, 
	0x3986, 0x59a2, 0xfe8d, 0xfeb8, 0xff04, 0xff6e, 0xffdd, 0x0031, 0x0044, 0xfffb, 0xff6a, 0x0677, 0xfffa, 0x00b2, 0x006a, 0x0073, 
	0x0073, 0x0073, 0x4f18, 0x4f81, 0x4e33, 0x495c, 0x4441, 0x33c0, 0x33b2, 0x3420, 0x34f5, 0x3618, 0x3772, 0x38ec, 0x3a6e, 0x3be2, 
	0x405f, 0x454d, 0x4d23, 0x60b7, 0x6d54, 0x6d54, 0x6d54, 0xd4f0, 0xd472, 0xd58f, 0xdcb8, 0x07a5, 0xbbeb, 0xbceb, 0xbd5b, 0xbd5e, 
	0xbd09, 0xbc61, 0xbb6e, 0xba40, 0xb912, 0xc01c, 0xb41b, 0xaeab, 0xab93, 0xae92, 0xae92, 0xae92, 0xbd37, 0xbc63, 0xb9c3, 0x3251, 
	0x30b1, 0xf6a3, 0xf5f5, 0xf4f2, 0xf3aa, 0xf1fd, 0xf070, 0xedb0, 0xea2f, 0xe93b, 0x5bd4, 0x0d9e, 0xfd14, 0x8101, 0x7de8, 0x7de8, 
	0x7de8, 0x1df0, 0x1a3c, 0x19f1, 0x5cad, 0x4c83, 0x2e41, 0x2be0, 0x2bc6, 0x2d0b, 0x2ec9, 0x303b, 0x315c, 0x3334, 0x3716, 0x440c, 
	0x44de, 0x51e8, 0x0a30, 0xfa43, 0xfa43, 0xfa43, 0x5b3a, 0x5402, 0x50d0, 0xd496, 0xe3e6, 0xbea2, 0xbf34, 0xbdcf, 0xbb17, 0xb77d, 
	0xb42f, 0xb02d, 0xaac2, 0xa5df, 0x1383, 0xbefe, 0xa587, 0x2296, 0x2fcc, 0x2fcc, 0x2fcc, };

JointIndex gIronKnuckleSkelGironknucklehorizontalattackanimAnimJointIndices[30] = {
	{ 0x0000, 0x0006, 0x001b, },
	{ 0x0030, 0x0045, 0x005a, },
	{ 0x006f, 0x0084, 0x0099, },
	{ 0x00ae, 0x00c3, 0x00d8, },
	{ 0x00ed, 0x0102, 0x0117, },
	{ 0x012c, 0x0141, 0x0156, },
	{ 0x016b, 0x0180, 0x0195, },
	{ 0x01aa, 0x01bf, 0x01d4, },
	{ 0x01e9, 0x01fe, 0x0213, },
	{ 0x0001, 0x0002, 0x0000, },
	{ 0x0228, 0x023d, 0x0252, },
	{ 0x0267, 0x027c, 0x0291, },
	{ 0x0001, 0x0001, 0x0000, },
	{ 0x0001, 0x0001, 0x0000, },
	{ 0x02a6, 0x02bb, 0x02d0, },
	{ 0x02e5, 0x02fa, 0x030f, },
	{ 0x0324, 0x0339, 0x034e, },
	{ 0x0363, 0x0378, 0x038d, },
	{ 0x03a2, 0x03b7, 0x03cc, },
	{ 0x03e1, 0x03f6, 0x040b, },
	{ 0x0420, 0x0435, 0x044a, },
	{ 0x0003, 0x0004, 0x0005, },
	{ 0x045f, 0x0474, 0x0489, },
	{ 0x049e, 0x04b3, 0x04c8, },
	{ 0x04dd, 0x04f2, 0x0507, },
	{ 0x051c, 0x0531, 0x0546, },
	{ 0x0001, 0x0000, 0x0001, },
	{ 0x0001, 0x0000, 0x0001, },
	{ 0x0001, 0x0000, 0x0001, },
	{ 0x0000, 0x0001, 0x0001, },
};

AnimationHeader gIronKnuckleSkelGironknucklehorizontalattackanimAnim = { { 21 }, gIronKnuckleSkelGironknucklehorizontalattackanimAnimFrameData, gIronKnuckleSkelGironknucklehorizontalattackanimAnimJointIndices, 6 };


#define FLAGS ACTOR_FLAG_4

typedef void (*EnIkDrawFunc)(struct EnIk*, PlayState*);

void EnIk_Init(Actor* thisx, PlayState* play);
void EnIk_Destroy(Actor* thisx, PlayState* play);
void EnIk_Update(Actor* thisx, PlayState* play);
void EnIk_Draw(Actor* thisx, PlayState* play);

void func_80A74714(EnIk* this);
void func_80A747C0(EnIk* this, PlayState* play);
void func_80A7492C(EnIk* this, PlayState* play);
void func_80A74AAC(EnIk* this);
void func_80A74BA4(EnIk* this, PlayState* play);
void func_80A74E2C(EnIk* this);
void func_80A74EBC(EnIk* this, PlayState* play);
void func_80A7506C(EnIk* this);
void func_80A7510C(EnIk* this, PlayState* play);
void func_80A751C8(EnIk* this);
void EnIk_SetupLowSweep(EnIk* this);
void func_80A75260(EnIk* this, PlayState* play);
void func_80A753D0(EnIk* this);
void func_80A7545C(EnIk* this, PlayState* play);
void func_80A754A0(EnIk* this);
void func_80A75530(EnIk* this, PlayState* play);
void func_80A755F0(EnIk* this);
void func_80A7567C(EnIk* this, PlayState* play);
void func_80A758B0(EnIk* this, PlayState* play);
void func_80A75A38(EnIk* this, PlayState* play);
void func_80A75FA0(Actor* thisx, PlayState* play);
void func_80A76798(Actor* thisx, PlayState* play);
void func_80A7748C(EnIk* this, PlayState* play);
void func_80A774BC(EnIk* this, PlayState* play);
void func_80A774F8(EnIk* this, PlayState* play);
void func_80A77844(EnIk* this, PlayState* play);
void func_80A779DC(EnIk* this, PlayState* play);
void func_80A77AEC(EnIk* this, PlayState* play);
void func_80A77B0C(EnIk* this, PlayState* play);
void func_80A77B3C(EnIk* this, PlayState* play);
void func_80A77ED0(EnIk* this, PlayState* play);
void func_80A77EDC(EnIk* this, PlayState* play);
void func_80A78160(EnIk* this, PlayState* play);
void func_80A781CC(Actor* thisx, PlayState* play);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 25, 80, 0, { 0, 0, 0 } },
};

static ColliderTrisElementInit sTrisElementsInit[2] = {
    {
        {
            ELEMTYPE_UNK2,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFC3FFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_NO_AT_INFO,
            OCELEM_NONE,
        },
        { { { -10.0f, 14.0f, 2.0f }, { -10.0f, -6.0f, 2.0f }, { 9.0f, 14.0f, 2.0f } } },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFC3FFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_NO_AT_INFO,
            OCELEM_NONE,
        },
        { { { -10.0f, -6.0f, 2.0f }, { 9.0f, -6.0f, 2.0f }, { 9.0f, 14.0f, 2.0f } } },
    },
};

static ColliderTrisInit sTrisInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_TRIS,
    },
    2,
    sTrisElementsInit,
};

static ColliderQuadInit sQuadInit = {
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
        { 0x20000000, 0x00, 0x40 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL | TOUCH_UNK7,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0xD),
    /* Deku stick    */ DMG_ENTRY(2, 0xF),
    /* Slingshot     */ DMG_ENTRY(1, 0xE),
    /* Explosive     */ DMG_ENTRY(2, 0xF),
    /* Boomerang     */ DMG_ENTRY(0, 0xD),
    /* Normal arrow  */ DMG_ENTRY(2, 0xE),
    /* Hammer swing  */ DMG_ENTRY(2, 0xF),
    /* Hookshot      */ DMG_ENTRY(0, 0xD),
    /* Kokiri sword  */ DMG_ENTRY(1, 0xF),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(4, 0xF),
    /* Fire arrow    */ DMG_ENTRY(2, 0xE),
    /* Ice arrow     */ DMG_ENTRY(2, 0xE),
    /* Light arrow   */ DMG_ENTRY(2, 0xE),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0xE),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0xE),
    /* Unk arrow 3   */ DMG_ENTRY(15, 0xE),
    /* Fire magic    */ DMG_ENTRY(0, 0x6),
    /* Ice magic     */ DMG_ENTRY(0, 0x6),
    /* Light magic   */ DMG_ENTRY(0, 0x6),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0xF),
    /* Giant spin    */ DMG_ENTRY(4, 0xF),
    /* Master spin   */ DMG_ENTRY(2, 0xF),
    /* Kokiri jump   */ DMG_ENTRY(2, 0xF),
    /* Giant jump    */ DMG_ENTRY(8, 0xF),
    /* Master jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 1     */ DMG_ENTRY(10, 0xF),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

typedef enum {
    /* 0x00 */ ENIK_LIMB_NONE,
    /* 0x01 */ ENIK_LIMB_ROOT,
    /* 0x02 */ ENIK_LIMB_HIP_ROOT,
    /* 0x03 */ ENIK_LIMB_RIGHT_THIGH_ROOT,
    /* 0x04 */ ENIK_LIMB_RIGHT_SHIN_ROOT,
    /* 0x05 */ ENIK_LIMB_RIGHT_FOOT_ROOT,
    /* 0x06 */ ENIK_LIMB_LEFT_THIGH_ROOT,
    /* 0x07 */ ENIK_LIMB_LEFT_SHIN_ROOT,
    /* 0x08 */ ENIK_LIMB_LEFT_FOOT_ROOT,
    /* 0x09 */ ENIK_LIMB_TORSO_ROOT,
    /* 0x0A */ ENIK_LIMB_TORSO,
    /* 0x0B */ ENIK_LIMB_HEAD_ROOT,
    /* 0x0C */ ENIK_LIMB_HELMET,
    /* 0x0D */ ENIK_LIMB_HEAD,
    /* 0x0E */ ENIK_LIMB_RIGHT_UPPER_ARM_ROOT,
    /* 0x0F */ ENIK_LIMB_RIGHT_FOREARM_ROOT,
    /* 0x10 */ ENIK_LIMB_RIGHT_HAND_ROOT,
    /* 0x11 */ ENIK_LIMB_AXE_ROOT,
    /* 0x12 */ ENIK_LIMB_RIGHT_HAND,
    /* 0x13 */ ENIK_LIMB_LEFT_UPPER_ARM_ROOT,
    /* 0x14 */ ENIK_LIMB_LEFT_FOREARM_ROOT,
    /* 0x15 */ ENIK_LIMB_LEFT_HAND_ROOT,
    /* 0x16 */ ENIK_LIMB_LEFT_SHOULDER_GUARD_OUTER,
    /* 0x17 */ ENIK_LIMB_LEFT_SHOULDER_GUARD_INNER,
    /* 0x18 */ ENIK_LIMB_RIGHT_SHOULDER_GUARD_OUTER,
    /* 0x19 */ ENIK_LIMB_RIGHT_SHOULDER_GUARD_INNER,
    /* 0x1A */ ENIK_LIMB_CHEST_PLATE,
    /* 0x1B */ ENIK_LIMB_BACK_PLATE,
    /* 0x1C */ ENIK_LIMB_UK1,
    /* 0x1D */ ENIK_LIMB_UK2,
    /* 0x1E */ ENIK_LIMB_MAX
} EnIkLimb;

void EnIk_Destroy(Actor* thisx, PlayState* play) {
    EnIk* this = (EnIk*)thisx;

    if (Actor_FindNearby(play, &this->actor, ACTOR_EN_IK, ACTORCAT_ENEMY, 8000.0f) == NULL) {
        func_800F5B58();
    }

    Collider_DestroyTris(play, &this->shieldCollider);
    Collider_DestroyCylinder(play, &this->bodyCollider);
    Collider_DestroyQuad(play, &this->axeCollider);
}

void EnIk_SetupAction(EnIk* this, EnIkActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void func_80A74398(Actor* thisx, PlayState* play) {
    EnIk* this = (EnIk*)thisx;
    s32 pad;
    EffectBlureInit1 blureInit;

    thisx->update = func_80A75FA0;
    thisx->draw = func_80A76798;
    thisx->flags |= ACTOR_FLAG_10;

    Collider_InitCylinder(play, &this->bodyCollider);
    Collider_SetCylinder(play, &this->bodyCollider, thisx, &sCylinderInit);
    Collider_InitTris(play, &this->shieldCollider);
    Collider_SetTris(play, &this->shieldCollider, thisx, &sTrisInit, this->shieldColliderItems);
    Collider_InitQuad(play, &this->axeCollider);
    Collider_SetQuad(play, &this->axeCollider, thisx, &sQuadInit);

    thisx->colChkInfo.damageTable = &sDamageTable;
    thisx->colChkInfo.mass = MASS_HEAVY;
    this->unk_2FC = 0;
    thisx->colChkInfo.health = 30;
    thisx->gravity = -1.0f;
    this->switchFlags = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;

    if (thisx->params == 0) {
        thisx->colChkInfo.health += 20;
        thisx->naviEnemyId = 0x34;
    } else {
        Actor_SetScale(thisx, 0.012f);
        thisx->naviEnemyId = 0x35;
        Actor_ChangeCategory(play, &play->actorCtx, thisx, ACTORCAT_ENEMY);
    }

    blureInit.p1StartColor[0] = blureInit.p1StartColor[1] = blureInit.p2StartColor[0] = blureInit.p2StartColor[1] =
        blureInit.p2StartColor[2] = blureInit.p1EndColor[0] = blureInit.p1EndColor[1] = blureInit.p2EndColor[0] =
            blureInit.p2EndColor[1] = blureInit.p2EndColor[2] = 255;

    blureInit.p2StartColor[3] = 64;
    blureInit.p1StartColor[3] = 200;
    blureInit.p1StartColor[2] = blureInit.p1EndColor[2] = 150;
    blureInit.p1EndColor[3] = blureInit.p2EndColor[3] = 0;

    blureInit.elemDuration = 8;
    blureInit.unkFlag = 0;
    blureInit.calcMode = 2;

    Effect_Add(play, &this->blureIdx, EFFECT_BLURE1, 0, 0, &blureInit);
    func_80A74714(this);

    if (this->switchFlags != 0xFF) {
        // In vanilla gameplay, Iron Knuckles are despawned based on specific flags in specific scenarios.
        // In Enemy Randomizer, this made the Iron Knuckles despawn when the same flag was set by other objects.
        // Instead, rely on the "Clear enemy room" flag when in Enemy Randomizer for Iron Knuckles that aren't Nabooru.
        if ((Flags_GetSwitch(play, this->switchFlags) && !CVar_GetS32("gRandomizedEnemies", 0)) ||
            (thisx->params != 0 && Flags_GetClear(play, play->roomCtx.curRoom.num) && CVar_GetS32("gRandomizedEnemies", 0))) {
            Actor_Kill(thisx);
        }
    } else if (thisx->params != 0 && Flags_GetClear(play, play->roomCtx.curRoom.num)) {
        Actor_Kill(thisx);
    }
}

s32 func_80A745E4(EnIk* this, PlayState* play) {
    if (((this->unk_2FB != 0) || (this->actor.params == 0)) &&
        (func_800354B4(play, &this->actor, 100.0f, 0x2710, 0x4000, this->actor.shape.rot.y) != 0) &&
        (Rand_ZeroOne() < 0.5f)) {
        func_80A755F0(this);
        return true;
    } else {
        return false;
    }
}

Actor* func_80A74674(PlayState* play, Actor* actor) {
    Actor* prop = play->actorCtx.actorLists[ACTORCAT_PROP].head;

    while (prop != NULL) {
        if ((prop == actor) || (prop->id != ACTOR_BG_JYA_IRONOBJ)) {
            prop = prop->next;
            continue;
        } else if (Actor_ActorAIsFacingAndNearActorB(actor, prop, 80.0f, 0x2710)) {
            return prop;
        }

        prop = prop->next;
    }

    return NULL;
}

void func_80A74714(EnIk* this) {
    f32 frames = Animation_GetLastFrame(&object_ik_Anim_00CD70);
    f32 frame;

    if (this->actor.params >= 2) {
        frame = frames - 1.0f;
    } else {
        frame = 0.0f;
    }

    Animation_Change(&this->skelAnime, &object_ik_Anim_00CD70, 0.0f, frame, frames, ANIMMODE_ONCE, 0.0f);
    this->unk_2F8 = 3;
    this->actionState = 0;
    this->pauseFrame = 0;
    this->actor.speedXZ = 0.0f;
    EnIk_SetupAction(this, func_80A747C0);
}

void func_80A747C0(EnIk* this, PlayState* play) {
    Vec3f sp24;

    if (this->bodyCollider.base.acFlags & AC_HIT) {
        sp24 = this->actor.world.pos;
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_ARMOR_HIT);
        sp24.y += 30.0f;
        func_8003424C(play, &sp24);
        this->skelAnime.playSpeed = 1.0f;
        // Disable miniboss music with Enemy Randomizer because the music would keep
        // playing if the enemy was never defeated, which is common with Enemy Randomizer.
        if (!CVar_GetS32("gRandomizedEnemies", 0)) {
            func_800F5ACC(NA_BGM_MINI_BOSS);
        }
    }
    if (this->skelAnime.curFrame == 5.0f) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_WAKEUP);
    }
    if (SkelAnime_Update(&this->skelAnime)) {
        this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_2;
        func_80A74AAC(this);
    }
}

void EnIk_SelectAttack(EnIk* this) {
    //EnIk_SetupLowSweep(this); return;
    if ((Rand_ZeroOne() < 0.33f)) {
        func_80A74E2C(this);
    } else if (Rand_ZeroOne() < 0.5f) {
        func_80A751C8(this);
    } else {
        EnIk_SetupLowSweep(this);
    }
}

void func_80A7489C(EnIk* this) {
    f32 frames = Animation_GetLastFrame(&object_ik_Anim_00DD50);

    this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_2;
    this->unk_2F8 = 4;
    this->actor.speedXZ = 0.0f;
    Animation_Change(&this->skelAnime, &object_ik_Anim_00DD50, 0.0f, 0.0f, frames, ANIMMODE_LOOP, 4.0f);
    EnIk_SetupAction(this, func_80A7492C);
}

void func_80A7492C(EnIk* this, PlayState* play) {
    s32 phi_a0 = (this->unk_2FB == 0) ? 0xAAA : 0x3FFC;
    s16 yawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

    if ((ABS(yawDiff) <= phi_a0) && (this->actor.xzDistToPlayer < 100.0f) &&
        (ABS(this->actor.yDistToPlayer) < 150.0f)) {
            EnIk_SelectAttack(this);
    } else if ((ABS(yawDiff) <= 0x4000) && (ABS(this->actor.yDistToPlayer) < 150.0f)) {
        func_80A74AAC(this);
    } else {
        func_80A74AAC(this);
    }
    func_80A745E4(this, play);
    SkelAnime_Update(&this->skelAnime);
}

void func_80A74AAC(EnIk* this) {
    this->unk_2F8 = 5;
    if (this->unk_2FB == 0) {
        Animation_Change(&this->skelAnime, &object_ik_Anim_00ED24, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_ik_Anim_00ED24), ANIMMODE_LOOP, -4.0f);
        this->actor.speedXZ = 0.9f;
    } else {
        Animation_Change(&this->skelAnime, &object_ik_Anim_006734, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_ik_Anim_006734), ANIMMODE_LOOP, -4.0f);
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_DASH);
        this->actor.speedXZ = 2.5f;
    }
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnIk_SetupAction(this, func_80A74BA4);
}

void func_80A74BA4(EnIk* this, PlayState* play) {
    s16 temp_t0;
    s16 temp_a1;
    s16 yawDiff;
    s16 sp30;
    s16 sp2E;
    s16 phi_a3;

    if (this->unk_2FB == 0) {
        temp_t0 = 0xAAA;
        phi_a3 = 0x320;
        sp30 = 0;
        sp2E = 0x10;
    } else {
        temp_t0 = 0x3FFC;
        phi_a3 = 0x4B0;
        sp30 = 2;
        sp2E = 9;
    }
    temp_a1 = this->actor.wallYaw - this->actor.shape.rot.y;
    if ((this->actor.bgCheckFlags & 8) && (ABS(temp_a1) >= 0x4000)) {
        temp_a1 = (this->actor.yawTowardsPlayer > 0) ? this->actor.wallYaw - 0x4000 : this->actor.wallYaw + 0x4000;
        Math_SmoothStepToS(&this->actor.world.rot.y, temp_a1, 1, phi_a3, 0);
    } else {
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, phi_a3, 0);
    }
    this->actor.shape.rot.y = this->actor.world.rot.y;
    yawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    if ((ABS(yawDiff) <= temp_t0) && (this->actor.xzDistToPlayer < 100.0f)) {
        if (ABS(this->actor.yDistToPlayer) < 150.0f) {
            EnIk_SelectAttack(this);
        }
    }
    if (func_80A74674(play, &this->actor) != NULL) {
        func_80A751C8(this);
        this->unk_2FC = 1;
    } else {
        temp_t0 = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
        if (ABS(temp_t0) > 0x2000) {
            this->unk_300--;
            if (this->unk_300 == 0) {
                func_80A754A0(this);
            }
        } else {
            this->unk_300 = 0x10;
        }
    }
    func_80A745E4(this, play);
    SkelAnime_Update(&this->skelAnime);
    if ((sp30 == (s16)this->skelAnime.curFrame) || (sp2E == (s16)this->skelAnime.curFrame)) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_WALK);
    }
}

#define OVERHEAD_SPEED 1.5f

void func_80A74E2C(EnIk* this) {
    f32 frames = Animation_GetLastFrame(&object_ik_Anim_001C28);

    this->unk_2FF = 1;
    this->unk_2F8 = 6;
    this->actor.speedXZ = 0.0f;
    this->pauseFrame = 2;
    Animation_Change(&this->skelAnime, &object_ik_Anim_001C28, OVERHEAD_SPEED, 0.0f, frames, ANIMMODE_ONCE, -4.0f);
    EnIk_SetupAction(this, func_80A74EBC);
}

void func_80A74EBC(EnIk* this, PlayState* play) {
    Vec3f sp2C;

    if (this->pauseFrame > 0) {
        this->pauseFrame--;
        this->skelAnime.playSpeed = 0.0f;
    } else {
        this->skelAnime.playSpeed = OVERHEAD_SPEED;
    }
    
    this->actor.speedXZ = 0.0f;
    if (this->skelAnime.curFrame < 15.0f){
        if (this->actor.xzDistToPlayer > 70.0f)
            this->actor.speedXZ = 6.0f;
    } else if (this->skelAnime.curFrame == 15.0f) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_SWING_AXE);
    } else if (this->skelAnime.curFrame == 21.0f) {
        sp2C.x = this->actor.world.pos.x + Math_SinS(this->actor.shape.rot.y + 0x6A4) * 70.0f;
        sp2C.z = this->actor.world.pos.z + Math_CosS(this->actor.shape.rot.y + 0x6A4) * 70.0f;
        sp2C.y = this->actor.world.pos.y;
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_HIT_GND);
        Camera_AddQuake(&play->mainCamera, 2, 0x19, 5);
        func_800AA000(this->actor.xzDistToPlayer, 0xFF, 0x14, 0x96);
        CollisionCheck_SpawnShieldParticles(play, &sp2C);
    }

    if ((this->skelAnime.curFrame > 17.0f) && (this->skelAnime.curFrame < 23.0f)) {
        this->unk_2FE = 1;
    } else {
        if (this->skelAnime.curFrame < 17.0f) {
            f32 invDist = 1.5;///(0.1f+this->actor.xzDistToPlayer*0.01);
            Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, invDist*0xDAC, 0);
            this->actor.shape.rot.y = this->actor.world.rot.y;
        }
        this->unk_2FE = 0;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        func_80A7506C(this);
    }
}

void func_80A7506C(EnIk* this) {
    f32 frames = Animation_GetLastFrame(&object_ik_Anim_0029FC);

    this->unk_2FE = 0;
    this->unk_2F9 = (s8)frames;
    this->unk_2F8 = 7;
    this->unk_2FF = this->unk_2FE;
    Animation_Change(&this->skelAnime, &object_ik_Anim_0029FC, 1.0f, 0.0f, frames, ANIMMODE_LOOP, -4.0f);
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_PULLOUT);
    EnIk_SetupAction(this, func_80A7510C);
}

void func_80A7510C(EnIk* this, PlayState* play) {
    f32 frames;

    if (SkelAnime_Update(&this->skelAnime) || (--this->unk_2F9 == 0)) {
        if (this->unk_2F8 == 8) {
            func_80A7489C(this);
        } else {
            frames = Animation_GetLastFrame(&object_ik_Anim_002538);
            this->unk_2F8 = 8;
            Animation_Change(&this->skelAnime, &object_ik_Anim_002538, 1.5f, 0.0f, frames, ANIMMODE_ONCE_INTERP, -4.0f);
        }
    }
}

void func_80A751C8(EnIk* this) {
    f32 frames = Animation_GetLastFrame(&gIronKnuckleSkelGironknucklehorizontalattackanimAnim);

    this->unk_2FF = 2;
    this->unk_300 = 0;
    this->unk_2F8 = 6;
    this->actionState = 0;
    this->pauseFrame = 5;
    this->actor.speedXZ = 0.0f;
    Animation_Change(&this->skelAnime, &gIronKnuckleSkelGironknucklehorizontalattackanimAnim, 0.0f, 0.0f, frames, ANIMMODE_ONCE_INTERP, -6.0f);
    this->unk_2FC = 0;
    EnIk_SetupAction(this, func_80A75260);
}

void EnIk_SetupLowSweep(EnIk* this) {
    f32 frames = Animation_GetLastFrame(&gIronKnuckleSkelGironknucklehorizontalattackanimAnim);

    this->unk_2FF = 2;
    this->unk_300 = 0x200;
    this->unk_2F8 = 5;
    this->actionState = 1;
    this->pauseFrame = 6;
    this->actor.speedXZ = 0.0f;
    Animation_Change(&this->skelAnime, &gIronKnuckleSkelGironknucklehorizontalattackanimAnim, 0.0f, 13.0f, frames, ANIMMODE_ONCE_INTERP, -6.0f);
    this->unk_2FC = 0;
    EnIk_SetupAction(this, func_80A75260);
}

void func_80A75260(EnIk* this, PlayState* play) {
    f32 temp_f0;
    u8 isPaused = (this->pauseFrame > 0 && (((!this->actionState) && this->skelAnime.curFrame > 0.0f ) || ((this->actionState) && this->skelAnime.curFrame > 13.0f)));
    if (!isPaused) {
        if (this->actionState) {
            this->unk_300 += 0x180;
        } else {
            this->unk_300 += 0x1C2;
        }
    }
    if (this->pauseFrame > 0 && (((!this->actionState) && this->skelAnime.curFrame > 0.0f ) || ((this->actionState) && this->skelAnime.curFrame > 13.0f))) {
        this->pauseFrame--;
        temp_f0 = 0.0f;
    } else {
        temp_f0 = Math_SinS(this->unk_300);
    }
    this->skelAnime.playSpeed = ABS(temp_f0);
    
    this->actor.speedXZ = 0.0f;
    if (this->skelAnime.curFrame < 11.0f || (this->actionState == 1 && this->skelAnime.curFrame < 18.0f)) {
        if (this->actor.xzDistToPlayer > 70.0f)
            this->actor.speedXZ = 4.0f;
    } else if (this->skelAnime.curFrame > 11.0f) {
        this->unk_2FF = 3;
    }
    if (((this->skelAnime.curFrame > 1.0f) && (this->skelAnime.curFrame < 9.0f)) ||
        ((this->skelAnime.curFrame > 13.0f) && (this->skelAnime.curFrame < 18.0f))) {
        if (this->unk_2FC == 0) {
            Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 0xDAC, 0);
            this->actor.shape.rot.y = this->actor.world.rot.y;
        }
        if (this->unk_2FE < 0) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_SWING_AXE);
        }
        this->unk_2FE = 1;
    } else {
        this->unk_2FE = 0;
    }
    if (SkelAnime_Update(&this->skelAnime)) {
        func_80A753D0(this);
    }
}

void func_80A753D0(EnIk* this) {
    f32 frames = Animation_GetLastFrame(&object_ik_Anim_003DBC);

    this->unk_2FF = this->unk_2FE = 0;
    this->unk_2F8 = 8;
    Animation_Change(&this->skelAnime, &object_ik_Anim_003DBC, 1.5f, 0.0f, frames, ANIMMODE_ONCE_INTERP, -4.0f);
    EnIk_SetupAction(this, func_80A7545C);
}

void func_80A7545C(EnIk* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        func_80A7489C(this);
        func_80A745E4(this, play);
    }
}

void func_80A754A0(EnIk* this) {
    f32 frames = Animation_GetLastFrame(&gIronKnuckleSkelGironknucklehorizontalattackanimAnim);

    this->unk_2F8 = 1;
    this->unk_2FF = 3;
    this->actor.speedXZ = 0.0f;
    Animation_Change(&this->skelAnime, &gIronKnuckleSkelGironknucklehorizontalattackanimAnim, 0.5f, 13.0f, frames, ANIMMODE_ONCE_INTERP, -4.0f);
    EnIk_SetupAction(this, func_80A75530);
}

void func_80A75530(EnIk* this, PlayState* play) {
    Math_StepUntilS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 0xDAC);
    this->actor.shape.rot.y = this->actor.world.rot.y;
    if ((this->skelAnime.curFrame > 13.0f) && (this->skelAnime.curFrame < 18.0f)) {
        if (this->unk_2FE < 0) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_SWING_AXE);
        }
        this->unk_2FE = 1;
    } else {
        this->unk_2FE = 0;
    }
    if (SkelAnime_Update(&this->skelAnime)) {
        func_80A753D0(this);
        func_80A745E4(this, play);
    }
}

void func_80A755F0(EnIk* this) {
    f32 frames = Animation_GetLastFrame(&object_ik_Anim_00485C);

    this->unk_2FE = 0;
    this->unk_2F8 = 9;
    this->actor.speedXZ = 0.0f;
    Animation_Change(&this->skelAnime, &object_ik_Anim_00485C, 1.0f, 0.0f, frames, ANIMMODE_ONCE_INTERP, -4.0f);
    EnIk_SetupAction(this, func_80A7567C);
}

void func_80A7567C(EnIk* this, PlayState* play) {
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->shieldCollider.base);
    if (SkelAnime_Update(&this->skelAnime)) {
        if ((ABS((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y)) <= 0x4000) &&
            (this->actor.xzDistToPlayer < 100.0f) && (ABS(this->actor.yDistToPlayer) < 150.0f)) {
            EnIk_SelectAttack(this);
        } else {
            func_80A7489C(this);
        }
    }
}

void func_80A75790(EnIk* this) {
    s16 yaw;
    s16 yawDiff;

    yaw = Math_Vec3f_Yaw(&this->actor.world.pos, &this->bodyCollider.base.ac->world.pos);
    this->unk_2F8 = 0;
    yawDiff = yaw - this->actor.shape.rot.y;
    if (ABS(yawDiff) <= 0x4000) {
        Animation_Change(&this->skelAnime, &object_ik_Anim_006194, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_ik_Anim_006194), ANIMMODE_ONCE, -4.0f);
        this->actor.speedXZ = -6.0f;
    } else {
        Animation_Change(&this->skelAnime, &object_ik_Anim_0045BC, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_ik_Anim_0045BC), ANIMMODE_ONCE, -4.0f);
        this->actor.speedXZ = 6.0f;
    }
    this->unk_2FE = 0;
    EnIk_SetupAction(this, func_80A758B0);
}

void func_80A758B0(EnIk* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.speedXZ, 0.0f, 1.0f, 1.0f, 0.0f);
    if (BodyBreak_SpawnParts(&this->actor, &this->bodyBreak, play, this->actor.params + 4)) {
        this->bodyBreak.val = BODYBREAK_STATUS_FINISHED;
    }
    if (SkelAnime_Update(&this->skelAnime)) {
        if (ABS((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y)) <= 0x2000) {
            func_80A7489C(this);
            func_80A745E4(this, play);
        } else {
            func_80A754A0(this);
        }
    }
}

void func_80A7598C(EnIk* this) {
    f32 frames = Animation_GetLastFrame(&object_ik_Anim_005944);

    this->unk_2FE = 0;
    this->unk_2F8 = 2;
    this->actor.speedXZ = 0.0f;
    Animation_Change(&this->skelAnime, &object_ik_Anim_005944, 1.0f, 0.0f, frames, ANIMMODE_ONCE, -4.0f);
    this->unk_2F9 = 0x18;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_DEAD);
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_NUTS_CUTBODY);
    EnIk_SetupAction(this, func_80A75A38);
    gSaveContext.sohStats.count[COUNT_ENEMIES_DEFEATED_IRON_KNUCKLE]++;
}

void func_80A75A38(EnIk* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        if ((this->actor.colChkInfo.health == 0) && (this->unk_2F9 != 0)) {
            s32 i;
            Vec3f pos;
            Vec3f sp7C = { 0.0f, 0.5f, 0.0f };

            this->unk_2F9--;

            for (i = 0xC - (this->unk_2F9 >> 1); i >= 0; i--) {
                pos.x = this->actor.world.pos.x + Rand_CenteredFloat(120.0f);
                pos.z = this->actor.world.pos.z + Rand_CenteredFloat(120.0f);
                pos.y = this->actor.world.pos.y + 20.0f + Rand_CenteredFloat(50.0f);
                EffectSsDeadDb_Spawn(play, &pos, &sp7C, &sp7C, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1, 9, true);
            }
            if (this->unk_2F9 == 0) {
                Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0xB0);
                // Don't set flag when Iron Knuckle is spawned by Enemy Rando.
                // Instead Iron Knuckles rely on the "clear room" flag when Enemy Rando is on.
                if (this->switchFlags != 0xFF && !CVar_GetS32("gRandomizedEnemies",0)) {
                    Flags_SetSwitch(play, this->switchFlags);
                }
                Actor_Kill(&this->actor);
            }
        }
    } else if (this->skelAnime.curFrame == 23.0f) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_WALK);
    }
}

void func_80A75C38(EnIk* this, PlayState* play) {
    f32 temp_f0;
    u8 pad;
    u8 pad2;
    u8 prevHealth;
    s32 temp_v0_3;
    Vec3f sp38;

    if ((this->unk_2F8 == 3) || (this->unk_2F8 == 2)) {
        return;
    }
    if (this->shieldCollider.base.acFlags & AC_BOUNCED) {
        temp_f0 = Animation_GetLastFrame(&object_ik_Anim_00485C) - 2.0f;
        if (this->skelAnime.curFrame < temp_f0) {
            this->skelAnime.curFrame = temp_f0;
        }
        this->shieldCollider.base.acFlags &= ~AC_BOUNCED;
        this->bodyCollider.base.acFlags &= ~AC_HIT;
        return;
    }
    if (!(this->bodyCollider.base.acFlags & AC_HIT)) {
        return;
    }
    sp38 = this->actor.world.pos;
    sp38.y += 50.0f;
    Actor_SetDropFlag(&this->actor, &this->bodyCollider.info, 1);
    temp_v0_3 = this->actor.colChkInfo.damageEffect;
    this->unk_2FD = temp_v0_3 & 0xFF;
    this->bodyCollider.base.acFlags &= ~AC_HIT;

    if ((this->unk_2FD == 0) || (this->unk_2FD == 0xD) || ((this->unk_2FB == 0) && (this->unk_2FD == 0xE))) {
        if (this->unk_2FD != 0) {
            CollisionCheck_SpawnShieldParticlesMetal(play, &sp38);
        }
        return;
    }
    Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, 0xC);
    prevHealth = this->actor.colChkInfo.health;
    Actor_ApplyDamage(&this->actor);
    if (this->actor.params != 0) {
        if ((prevHealth > 10) && (this->actor.colChkInfo.health <= 10)) {
            this->unk_2FB = 1;
            BodyBreak_Alloc(&this->bodyBreak, 3, play);
        }
    } else if (this->actor.colChkInfo.health <= 10) {
        Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_BOSS);
        SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 20, NA_SE_EN_LAST_DAMAGE);
        if (this->switchFlags != 0xFF) {
            Flags_SetSwitch(play, this->switchFlags);
        }
        return;
    } else if (prevHealth == 50) {
        Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_ENEMY);
    }

    if (this->actor.colChkInfo.health == 0) {
        func_80A7598C(this);
        Enemy_StartFinishingBlow(play, &this->actor);
        return;
    }
    Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 0x7D0, 0);
    if ((this->actor.params == 0) && (Rand_ZeroOne() < 0.5f)) {
        if (ABS((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y)) > 0x2000) {
            func_80A754A0(this);
        }
    }
    if ((this->actor.params != 0) && (this->unk_2FB != 0)) {
        if ((prevHealth > 10) && (this->actor.colChkInfo.health <= 10)) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_ARMOR_OFF_DEMO);
        } else {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_DAMAGE);
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_NUTS_CUTBODY);
        }
        //func_80A75790(this);
        return;
    }
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_ARMOR_HIT);
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_IRONNACK_DAMAGE);
    CollisionCheck_SpawnShieldParticles(play, &sp38);
}

void func_80A75FA0(Actor* thisx, PlayState* play) {
    EnIk* this = (EnIk*)thisx;
    s32 pad;
    Player* player = GET_PLAYER(play);
    u8 prevInvincibilityTimer;

    this->unk_2FA = this->unk_2FB;
    func_80A75C38(this, play);
    if ((this->actor.params == 0) && (this->actor.colChkInfo.health <= 10)) {
        func_80A781CC(&this->actor, play);
        return;
    }
    this->actionFunc(this, play);
    if (this->axeCollider.base.atFlags & AT_HIT) {
        this->axeCollider.base.atFlags &= ~AT_HIT;
        if (&player->actor == this->axeCollider.base.at) {
            prevInvincibilityTimer = player->invincibilityTimer;
            if (player->invincibilityTimer <= 0) {
                if (player->invincibilityTimer < -39) {
                    player->invincibilityTimer = 0;
                } else {
                    player->invincibilityTimer = 0;
                    play->damagePlayer(play, -64);
                    this->unk_2FE = 0;
                }
            }
            func_8002F71C(play, &this->actor, 8.0f, this->actor.yawTowardsPlayer, 8.0f);
            player->invincibilityTimer = prevInvincibilityTimer;
        }
    }
    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 75.0f, 30.0f, 30.0f, 0x1D);
    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += 45.0f;
    Collider_UpdateCylinder(&this->actor, &this->bodyCollider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyCollider.base);
    if ((this->actor.colChkInfo.health > 0) && (this->actor.colorFilterTimer == 0) && (this->unk_2F8 >= 2)) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodyCollider.base);
    }
    if (this->unk_2FE > 0) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->axeCollider.base);
    }
    if (this->unk_2F8 == 9) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->shieldCollider.base);
    }
}

Gfx* func_80A761B0(GraphicsContext* gfxCtx, u8 primR, u8 primG, u8 primB, u8 envR, u8 envG, u8 envB) {
    Gfx* displayList;
    Gfx* displayListHead;

    displayList = Graph_Alloc(gfxCtx, 4 * sizeof(Gfx));
    displayListHead = displayList;

    gDPPipeSync(displayListHead++);
    if (CVar_GetS32("gCosmetics.NPC_IronKnuckles.Changed", 0)) {
        Color_RGB8 color = CVar_GetRGB("gCosmetics.NPC_IronKnuckles.Value", (Color_RGB8){primR, primG, primB});
        gDPSetPrimColor(displayListHead++, 0, 0, color.r, color.g, color.b, 255);
    } else {
        gDPSetPrimColor(displayListHead++, 0, 0, primR, primG, primB, 255);
    }
    gDPSetEnvColor(displayListHead++, envR, envG, envB, 255);
    gSPEndDisplayList(displayListHead++);

    return displayList;
}

s32 EnIk_OverrideLimbDraw3(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnIk* this = (EnIk*)thisx;

    if (limbIndex == ENIK_LIMB_TORSO) {
        if ((this->actionFunc == func_80A75260)) {
            // if (this->skelAnime.curFrame >= 13.0f)
            //     rot->z += 0x100;
            // else
            if (this->skelAnime.curFrame < 9.0f)
                rot->z -= 0x200;
            //rot->y += CVar_GetS32("gLimbY",0);
            //rot->z += CVar_GetS32("gLimbZ",0);
        }
    }

    if (limbIndex == 12) {
        if (this->actor.params != 0) {
            *dList = object_ik_DL_018E78;
        }
    } else if (limbIndex == 13) {
        if (this->actor.params != 0) {
            *dList = object_ik_DL_019100;
        }
    } else if ((limbIndex == 26) || (limbIndex == 27)) {
        if ((this->unk_2FA & 1)) {
            *dList = NULL;
        }
    } else if ((limbIndex == 28) || (limbIndex == 29)) {
        if (!(this->unk_2FA & 1)) {
            *dList = NULL;
        }
    }
    return false;
}

// unused
static Vec3f D_80A78470 = { 300.0f, 0.0f, 0.0f };

//x+ = blade (forward)
//y+ = flat (right)
//z- = tip
static Vec3f D_80A7847C[] = {
    // // { 800.0f, -200.0f, 0.0f },
    // { 800.0f, -200.0f, -5200.0f },
    // { 0.0f, 0.0f, 0.0f },
    // { -200.0f, -2200.0f, -5200.0f },
    // { -6000.0f, 2000.0f, 0000.0f },
    // // { 000.0f, 2000.0f, -10000.0f },

    { -1000.0f, 1000.0f, 1000.0f },
    { -6000.0f, 1000.0f, -7000.0f },
    { 1000.0f, 1000.0f, 1000.0f },
    { 3000.0f, 1000.0f, -6000.0f },
};

static Vec3f originalQuad[] = {
    { 800.0f, -200.0f, -5200.0f },
    { 0.0f, 0.0f, 0.0f },
    { -200.0f, -2200.0f, -5200.0f },
    { -6000.0f, 2000.0f, 0000.0f },
};

static Vec3f augmentedQuad[] = {
    { -1000.0f, -00.0f, 1000.0f },
    { -6000.0f, -00.0f, -8000.0f },
    { 1000.0f, -00.0f, 1000.0f },
    { 3000.0f, -00.0f, -7000.0f },
};

static Vec3f lowQuad[] = {
    { -1000.0f, 2000.0f, 1000.0f },
    { -6000.0f, 2000.0f, -8000.0f },
    { 1000.0f, 2000.0f, 1000.0f },
    { 6000.0f, 2000.0f, -7000.0f },
};

static Vec3f D_80A784AC[] = {
    { -3000.0, -700.0, -5000.0 },
    { -3000.0, -700.0, 2000.0 },
    { 4000.0, -700.0, 2000.0 },
};

static Vec3f D_80A784D0[] = {
    { 4000.0, -700.0, 2000.0 },
    { 4000.0, -700.0, -5000.0 },
    { -3000.0, -700.0, -5000.0 },
};

void EnIk_PostLimbDraw3(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    Vec3f spF4;
    Vec3f spE8;
    EnIk* this = (EnIk*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    if (this->unk_2FB & 1) {
        BodyBreak_SetInfo(&this->bodyBreak, limbIndex, 26, 27, 28, dList, BODYBREAK_OBJECT_DEFAULT);
    }
    if (limbIndex == 12) {
        gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        if (this->actor.params != 0) {
            gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_019E08);
        } else {
            gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016D88);
        }
    } else if (limbIndex == 17) {
        s32 i;
        Vec3f sp9C[3];
        Vec3f sp78[3];

        Vec3f* quadRef = D_80A7847C;
        if (this->actionFunc == func_80A74EBC)
            quadRef = augmentedQuad;
        //else if (this->actionFunc == func_80A75260 && (this->skelAnime.curFrame >= 13.0f))
        //    quadRef = lowQuad;

        Matrix_MultVec3f(&quadRef[0], &this->axeCollider.dim.quad[1]);
        Matrix_MultVec3f(&quadRef[1], &this->axeCollider.dim.quad[0]);
        Matrix_MultVec3f(&quadRef[2], &this->axeCollider.dim.quad[3]);
        Matrix_MultVec3f(&quadRef[3], &this->axeCollider.dim.quad[2]);
        Collider_SetQuadVertices(&this->axeCollider, &this->axeCollider.dim.quad[0], &this->axeCollider.dim.quad[1],
                                 &this->axeCollider.dim.quad[2], &this->axeCollider.dim.quad[3]);
        Matrix_MultVec3f(&quadRef[0], &spF4);
        Matrix_MultVec3f(&quadRef[1], &spE8);
        if (this->unk_2FE > 0) {
            EffectBlure_AddVertex(Effect_GetByIndex(this->blureIdx), &spF4, &spE8);
        } else if (this->unk_2FE == 0) {
            EffectBlure_AddSpace(Effect_GetByIndex(this->blureIdx));
            this->unk_2FE = -1;
        }
        if (this->unk_2F8 == 9) {
            for (i = 0; i < ARRAY_COUNT(sp78); i++) {
                Matrix_MultVec3f(&D_80A784AC[i], &sp9C[i]);
                Matrix_MultVec3f(&D_80A784D0[i], &sp78[i]);
            }

            Collider_SetTrisVertices(&this->shieldCollider, 0, &sp9C[0], &sp9C[1], &sp9C[2]);
            Collider_SetTrisVertices(&this->shieldCollider, 1, &sp78[0], &sp78[1], &sp78[2]);
        }
    }

    switch (limbIndex) {
        case 22:
            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016F88);
            break;
        case 24:
            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016EE8);
            break;
        case 26:
            if (!(this->unk_2FA & 1)) {
                gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016BE0);
            }
            break;
        case 27:
            if (!(this->unk_2FA & 1)) {
                gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016CD8);
            }
            break;
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80A76798(Actor* thisx, PlayState* play) {
    EnIk* this = (EnIk*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    if (this->actor.params == 0) {
        gSPSegment(POLY_OPA_DISP++, 0x08, func_80A761B0(play->state.gfxCtx, 245, 225, 155, 30, 30, 0));
        gSPSegment(POLY_OPA_DISP++, 0x09, func_80A761B0(play->state.gfxCtx, 255, 40, 0, 40, 0, 0));
        gSPSegment(POLY_OPA_DISP++, 0x0A, func_80A761B0(play->state.gfxCtx, 255, 255, 255, 20, 40, 30));
    } else if (this->actor.params == 1) {
        gSPSegment(POLY_OPA_DISP++, 0x08, func_80A761B0(play->state.gfxCtx, 245, 255, 205, 30, 35, 0));
        gSPSegment(POLY_OPA_DISP++, 0x09, func_80A761B0(play->state.gfxCtx, 185, 135, 25, 20, 20, 0));
        gSPSegment(POLY_OPA_DISP++, 0x0A, func_80A761B0(play->state.gfxCtx, 255, 255, 255, 30, 40, 20));
    } else if (this->actor.params == 2) {
        gSPSegment(POLY_OPA_DISP++, 0x08, func_80A761B0(play->state.gfxCtx, 55, 65, 55, 0, 0, 0));
        gSPSegment(POLY_OPA_DISP++, 0x09, func_80A761B0(play->state.gfxCtx, 205, 165, 75, 25, 20, 0));
        gSPSegment(POLY_OPA_DISP++, 0x0A, func_80A761B0(play->state.gfxCtx, 205, 165, 75, 25, 20, 0));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x08, func_80A761B0(play->state.gfxCtx, 255, 255, 255, 180, 180, 180));
        gSPSegment(POLY_OPA_DISP++, 0x09, func_80A761B0(play->state.gfxCtx, 225, 205, 115, 25, 20, 0));
        gSPSegment(POLY_OPA_DISP++, 0x0A, func_80A761B0(play->state.gfxCtx, 225, 205, 115, 25, 20, 0));
    }
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          EnIk_OverrideLimbDraw3, EnIk_PostLimbDraw3, this);

    CLOSE_DISPS(play->state.gfxCtx);
}

void EnIk_StartMusic(void) {
    func_800F5ACC(NA_BGM_MINI_BOSS);
}

void func_80A76C14(EnIk* this) {
    if (Animation_OnFrame(&this->skelAnime, 1.0f)) {
        Audio_PlaySoundGeneral(NA_SE_EN_IRONNACK_WAKEUP, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    } else if (Animation_OnFrame(&this->skelAnime, 33.0f)) {
        Audio_PlaySoundGeneral(NA_SE_EN_IRONNACK_WALK, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    } else if (Animation_OnFrame(&this->skelAnime, 68.0f) || Animation_OnFrame(&this->skelAnime, 80.0f)) {
        Audio_PlaySoundGeneral(NA_SE_EN_IRONNACK_ARMOR_DEMO, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    } else if (Animation_OnFrame(&this->skelAnime, 107.0f)) {
        Audio_PlaySoundGeneral(NA_SE_EN_IRONNACK_FINGER_DEMO, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    } else if (Animation_OnFrame(&this->skelAnime, 156.0f)) {
        Audio_PlaySoundGeneral(NA_SE_EN_IRONNACK_ARMOR_DEMO, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    } else if (Animation_OnFrame(&this->skelAnime, 188.0f)) {
        Audio_PlaySoundGeneral(NA_SE_EN_IRONNACK_WAVE_DEMO, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }
}

void func_80A76DDC(EnIk* this, PlayState* play, Vec3f* pos) {
    Audio_PlaySoundGeneral(NA_SE_EN_TWINROBA_TRANSFORM, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                           &D_801333E8);
}

void func_80A76E2C(EnIk* this, PlayState* play, Vec3f* pos) {
    static Vec3f D_80A78514[] = {
        { 1000.0, -1000.0, 1000.0 },  { 0.0, -1000.0, 0.0 },        { -1000.0, -5000.0, -4000.0 },
        { 1000.0, -5000.0, -3000.0 }, { -1000.0, 1000.0, -6000.0 }, { -1000.0, 3000.0, -5000.0 },
        { -800.0, 1000.0, -3000.0 },  { 0.0, -4000.0, -2000.0 },    { -1000.0, -2000.0, -6000.0 },
        { 1000.0, -3000.0, 0.0 },     { 2000.0, -2000.0, -4000.0 }, { -1000.0, 0.0, -6000.0 },
        { 1000.0, -2000.0, -2000.0 }, { 0.0, -2000.0, 2100.0 },     { 0.0, 0.0, 0.0 },
        { 1000.0, -1000.0, -6000.0 }, { 2000.0, 0.0, -3000.0 },     { -1000.0, -1000.0, -4000.0 },
        { 900.0, -800.0, 2700.0 },    { 720.0f, 900.0f, 2500.0f },
    };

    if (this->unk_4D4 == 0) {
        s32 pad;
        Vec3f effectVelocity = { 0.0f, 0.0f, 0.0f };
        Vec3f effectAccel = { 0.0f, 0.3f, 0.0f };
        s32 i;

        for (i = ARRAY_COUNT(D_80A78514) - 1; i >= 0; i--) {
            Color_RGBA8 primColor = { 200, 200, 200, 255 };
            Color_RGBA8 envColor = { 150, 150, 150, 0 };
            s32 temp_v0;
            Vec3f effectPos;

            Matrix_MultVec3f(&D_80A78514[i], &effectPos);
            temp_v0 = (Rand_ZeroOne() * 20.0f) - 10.0f;
            primColor.r += temp_v0;
            primColor.g += temp_v0;
            primColor.b += temp_v0;
            envColor.r += temp_v0;
            envColor.g += temp_v0;
            envColor.b += temp_v0;
            func_8002829C(play, &effectPos, &effectVelocity, &effectAccel, &primColor, &envColor,
                          (Rand_ZeroOne() * 60.0f) + 300.0f, 0);
        }

        this->unk_4D4 = 1;
        func_80A76DDC(this, play, pos);
    }
}

void func_80A77034(EnIk* this, PlayState* play) {
    Actor_UpdateBgCheckInfo(play, &this->actor, 75.0f, 30.0f, 30.0f, 5);
}

s32 func_80A7707C(EnIk* this) {
    return SkelAnime_Update(&this->skelAnime);
}

CsCmdActorAction* EnIk_GetNpcAction(PlayState* play, s32 actionIdx) {
    if (play->csCtx.state != CS_STATE_IDLE) {
        return play->csCtx.npcActions[actionIdx];
    } else {
        return NULL;
    }
}

void func_80A770C0(EnIk* this, PlayState* play, s32 actionIdx) {
    CsCmdActorAction* npcAction = EnIk_GetNpcAction(play, actionIdx);

    if (npcAction != NULL) {
        this->actor.world.pos.x = npcAction->startPos.x;
        this->actor.world.pos.y = npcAction->startPos.y;
        this->actor.world.pos.z = npcAction->startPos.z;
        this->actor.world.rot.y = this->actor.shape.rot.y = npcAction->rot.y;
    }
}

f32 EnIk_curFrame(Actor* thisx) {
    EnIk* this = (EnIk*)thisx;

    return this->skelAnime.curFrame;
}

void func_80A77148(EnIk* this) {
    this->action = 0;
    this->drawMode = 0;
    this->actor.shape.shadowAlpha = 0;
}

void func_80A77158(EnIk* this, PlayState* play) {
    Animation_Change(&this->skelAnime, &object_ik_Anim_00C114, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_ik_Anim_00C114), ANIMMODE_ONCE, 0.0f);
    func_80A770C0(this, play, 4);
    this->action = 1;
    this->drawMode = 1;
    this->actor.shape.shadowAlpha = 0xFF;
}

void func_80A771E4(EnIk* this) {
    Animation_Change(&this->skelAnime, &object_ik_Anim_00C114, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_ik_Anim_00C114), ANIMMODE_ONCE, 0.0f);
    this->action = 2;
    this->drawMode = 1;
    this->unk_4D4 = 0;
    this->actor.shape.shadowAlpha = 0xFF;
}

void func_80A77264(EnIk* this, PlayState* play, s32 arg2) {
    if ((arg2 != 0) && (EnIk_GetNpcAction(play, 4) != NULL)) {
        func_80A78160(this, play);
    }
}

void func_80A772A4(EnIk* this) {
    Audio_PlaySoundGeneral(NA_SE_EN_IRONNACK_STAGGER_DEMO, &this->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                           &D_801333E8);
}

void func_80A772EC(EnIk* this, PlayState* play) {
    static Vec3f D_80A78FA0;
    s32 pad[2];
    f32 wDest;

    SkinMatrix_Vec3fMtxFMultXYZW(&play->viewProjectionMtxF, &this->actor.world.pos, &D_80A78FA0, &wDest);
    Audio_PlaySoundGeneral(NA_SE_EN_IRONNACK_DEAD, &D_80A78FA0, 4, &D_801333E0, &D_801333E0, &D_801333E8);
}

void func_80A7735C(EnIk* this, PlayState* play) {
    s32 pad[3];
    f32 frames = Animation_GetLastFrame(&object_ik_Anim_0203D8);

    SkelAnime_InitFlex(play, &this->skelAnime, &object_ik_Skel_0205C0, NULL, this->jointTable, this->morphTable,
                       30);
    Animation_Change(&this->skelAnime, &object_ik_Anim_0203D8, 1.0f, 0.0f, frames, ANIMMODE_ONCE, 0.0f);
    this->action = 3;
    this->drawMode = 2;
    func_80A770C0(this, play, 4);
    func_80A772EC(this, play);
    this->actor.shape.shadowAlpha = 0xFF;
}

void func_80A77434(EnIk* this, PlayState* play) {
    this->action = 4;
    this->drawMode = 2;
    func_80A772A4(this);
    this->actor.shape.shadowAlpha = 0xFF;
}

void func_80A77474(EnIk* this, PlayState* play) {
    this->action = 5;
    this->drawMode = 0;
    this->actor.shape.shadowAlpha = 0;
}

void func_80A7748C(EnIk* this, PlayState* play) {
    func_80A77034(this, play);
    func_80A779DC(this, play);
}

void func_80A774BC(EnIk* this, PlayState* play) {
    func_80A7707C(this);
    func_80A77034(this, play);
    func_80A779DC(this, play);
}

void func_80A774F8(EnIk* this, PlayState* play) {
    if (EnIk_GetNpcAction(play, 4) == NULL) {
        Actor_Kill(&this->actor);
    }
}

s32 EnIk_OverrideLimbDraw2(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnIk* this = (EnIk*)thisx;

    if ((limbIndex == 13) || (limbIndex == 26) || (limbIndex == 27)) {
        if (EnIk_curFrame(&this->actor) >= 30.0f) {
            *dList = NULL;
        }
    }

    return 0;
}

void EnIk_PostLimbDraw2(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;

    OPEN_DISPS(gfxCtx);

    switch (limbIndex) {
        case 13: {
            EnIk* this = (EnIk*)thisx;

            if (EnIk_curFrame(&this->actor) < 30.0f) {
                gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(gfxCtx),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016D88);
            }
        } break;
        case 22:
            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016F88);
            break;
        case 24:
            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016EE8);
            break;
        case 26: {
            EnIk* this = (EnIk*)thisx;

            if (EnIk_curFrame(&this->actor) < 30.0f) {
                gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(gfxCtx),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016BE0);
            }
        } break;
        case 27: {
            EnIk* this = (EnIk*)thisx;

            if (EnIk_curFrame(&this->actor) < 30.0f) {
                gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(gfxCtx),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016CD8);
            }
        } break;
    }
    CLOSE_DISPS(gfxCtx);
}

void func_80A77844(EnIk* this, PlayState* play) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    SkelAnime* skelAnime = &this->skelAnime;
    s32 pad[2];

    OPEN_DISPS(gfxCtx);

    func_8002EBCC(&this->actor, play, 0);
    Gfx_SetupDL_25Opa(gfxCtx);
    Gfx_SetupDL_25Xlu(gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, func_80A761B0(gfxCtx, 245, 225, 155, 30, 30, 0));
    gSPSegment(POLY_OPA_DISP++, 0x09, func_80A761B0(gfxCtx, 255, 40, 0, 40, 0, 0));
    gSPSegment(POLY_OPA_DISP++, 0x0A, func_80A761B0(gfxCtx, 255, 255, 255, 20, 40, 30));
    SkelAnime_DrawFlexOpa(play, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                          EnIk_OverrideLimbDraw2, EnIk_PostLimbDraw2, this);

    CLOSE_DISPS(gfxCtx);
}

void func_80A779DC(EnIk* this, PlayState* play) {
    CsCmdActorAction* npcAction = EnIk_GetNpcAction(play, 4);
    u32 action;
    u32 currentNpcAction;

    if (npcAction != NULL) {
        action = npcAction->action;
        currentNpcAction = this->npcAction;
        if (action != currentNpcAction) {
            switch (action) {
                case 1:
                    func_80A77148(this);
                    break;
                case 2:
                    func_80A77158(this, play);
                    break;
                case 3:
                    func_80A771E4(this);
                    break;
                case 4:
                    func_80A78160(this, play);
                    break;
                case 5:
                    func_80A7735C(this, play);
                    break;
                case 6:
                    func_80A77434(this, play);
                    break;
                case 7:
                    func_80A77474(this, play);
                    break;
                default:
                    osSyncPrintf("En_Ik_inConfrontion_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
            }

            this->npcAction = action;
        }
    }
}

void func_80A77AEC(EnIk* this, PlayState* play) {
    func_80A779DC(this, play);
}

void func_80A77B0C(EnIk* this, PlayState* play) {
    func_80A77034(this, play);
    func_80A779DC(this, play);
}

void func_80A77B3C(EnIk* this, PlayState* play) {
    s32 sp24;

    sp24 = func_80A7707C(this);
    func_80A76C14(this);
    func_80A77034(this, play);
    func_80A779DC(this, play);
    func_80A77264(this, play, sp24);
}

static EnIkActionFunc sActionFuncs[] = {
    func_80A77AEC, func_80A77B0C, func_80A77B3C, func_80A7748C, func_80A774BC, func_80A774F8,
};

void EnIk_Update(Actor* thisx, PlayState* play) {
    EnIk* this = (EnIk*)thisx;

    if (this->action < 0 || this->action >= ARRAY_COUNT(sActionFuncs) || sActionFuncs[this->action] == NULL) {
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }

    sActionFuncs[this->action](this, play);
}

s32 EnIk_OverrideLimbDraw1(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnIk* this = (EnIk*)thisx;
    f32 curFrame;

    switch (limbIndex) {
        case 17:
            curFrame = this->skelAnime.curFrame;
            if (curFrame < 120.0f) {
                *dList = NULL;
            } else {
                func_80A76E2C(this, play, pos);
            }
            break;
        case 29:
        case 30:
            *dList = NULL;
            break;
    }

    return 0;
}

void EnIk_PostLimbDraw1(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;

    OPEN_DISPS(gfxCtx);

    switch (limbIndex) {
        case 12:
            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016D88);
            break;
        case 22:
            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016F88);
            break;
        case 24:
            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016EE8);
            break;
        case 26:
            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016BE0);
            break;
        case 27:
            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, object_ik_DL_016CD8);
            break;
    }

    CLOSE_DISPS(gfxCtx);
}

void func_80A77ED0(EnIk* this, PlayState* play) {
}

void func_80A77EDC(EnIk* this, PlayState* play) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    SkelAnime* skelAnime = &this->skelAnime;
    s32 pad[2];

    OPEN_DISPS(gfxCtx);

    func_8002EBCC(&this->actor, play, 0);
    Gfx_SetupDL_25Opa(gfxCtx);
    Gfx_SetupDL_25Xlu(gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, func_80A761B0(gfxCtx, 245, 225, 155, 30, 30, 0));
    gSPSegment(POLY_OPA_DISP++, 0x09, func_80A761B0(gfxCtx, 255, 40, 0, 40, 0, 0));
    gSPSegment(POLY_OPA_DISP++, 0x0A, func_80A761B0(gfxCtx, 255, 255, 255, 20, 40, 30));
    SkelAnime_DrawFlexOpa(play, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                          EnIk_OverrideLimbDraw1, EnIk_PostLimbDraw1, this);

    CLOSE_DISPS(gfxCtx);
}

static EnIkDrawFunc sDrawFuncs[] = { func_80A77ED0, func_80A77EDC, func_80A77844 };

void EnIk_Draw(Actor* thisx, PlayState* play) {
    EnIk* this = (EnIk*)thisx;

    if (this->drawMode < 0 || this->drawMode >= ARRAY_COUNT(sDrawFuncs) || sDrawFuncs[this->drawMode] == NULL) {
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }

    sDrawFuncs[this->drawMode](this, play);
}

void func_80A780D0(EnIk* this, PlayState* play) {
    if (this->actor.params == 0) {
        if (!(gSaveContext.eventChkInf[3] & 0x800)) {
            this->actor.update = EnIk_Update;
            this->actor.draw = EnIk_Draw;
            Actor_SetScale(&this->actor, 0.01f);
        } else {
            func_80A78160(this, play);
            EnIk_StartMusic();
        }
    }
    osSyncPrintf("En_Ik_inConfrontion_Init : %d !!!!!!!!!!!!!!!!\n", this->actor.params);
}

void func_80A78160(EnIk* this, PlayState* play) {
    this->actor.update = func_80A75FA0;
    this->actor.draw = func_80A76798;
    this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_2;
    gSaveContext.eventChkInf[3] |= 0x800;
    Actor_SetScale(&this->actor, 0.012f);
    func_80A7489C(this);
}

void func_80A781CC(Actor* thisx, PlayState* play) {
    EnIk* this = (EnIk*)thisx;

    if (!Play_InCsMode(play)) {
        this->actor.update = EnIk_Update;
        this->actor.draw = EnIk_Draw;
        // Don't initiate nabooru defeat CS in rando
        if (!(gSaveContext.n64ddFlag)) {
            Cutscene_SetSegment(play, gSpiritBossNabooruKnuckleDefeatCs);
            gSaveContext.cutsceneTrigger = 1;
            Actor_SetScale(&this->actor, 0.01f);
        } else {
        // Because no CS in rando, we hide the death of the knuckle by spawning flames and kill the actor
            if ((this->actor.colChkInfo.health <= 10)) {
                s32 i;
                Vec3f pos;
                Vec3f sp7C = { 0.0f, 0.5f, 0.0f };
                int flameAmount = 100;

                for (i = flameAmount; i >= 0; i--) {
                    pos.x = this->actor.world.pos.x + Rand_CenteredFloat(120.0f);
                    pos.z = this->actor.world.pos.z + Rand_CenteredFloat(120.0f);
                    pos.y = this->actor.world.pos.y + 20.0f + Rand_CenteredFloat(120.0f);
                    EffectSsDeadDb_Spawn(play, &pos, &sp7C, &sp7C, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1, 9,
                                            true);
                }
                Actor_Kill(&this->actor);
            }
        }
        gSaveContext.eventChkInf[3] |= 0x1000;
        func_80A7735C(this, play);
        gSaveContext.sohStats.count[COUNT_ENEMIES_DEFEATED_IRON_KNUCKLE_NABOORU]++;
    }
}

void EnIk_Init(Actor* thisx, PlayState* play) {
    EnIk* this = (EnIk*)thisx;
    s32 flag = this->actor.params & 0xFF00;

    if (((this->actor.params & 0xFF) == 0 && (gSaveContext.eventChkInf[3] & 0x1000)) ||
        (flag != 0 && Flags_GetSwitch(play, flag >> 8))) {
        Actor_Kill(&this->actor);
    } else {
        ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
        SkelAnime_InitFlex(play, &this->skelAnime, &object_ik_Skel_01E178, &object_ik_Anim_00C114,
                           this->jointTable, this->morphTable, 30);
        func_80A74398(&this->actor, play);
        func_80A780D0(this, play);
    }

    // Immediately trigger Iron Knuckle for enemy randomizer
    if (CVar_GetS32("gRandomizedEnemies", 0) && (thisx->params == 2 || thisx->params == 3)) {
        this->skelAnime.playSpeed = 1.0f;
    }
}

const ActorInit En_Ik_InitVars = {
    ACTOR_EN_IK,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_IK,
    sizeof(EnIk),
    (ActorFunc)EnIk_Init,
    (ActorFunc)EnIk_Destroy,
    (ActorFunc)EnIk_Update,
    (ActorFunc)EnIk_Draw,
    NULL,
};
