#include "global.h"
#include "z64elf_message.h"
#include <assert.h>

ElfMessage sChildSariaMsgs[] = {
    ELF_MSG_STRENGTH_UPG(SKIP, 3, false, 0),
    ELF_MSG_FLAG(CHECK, 0x61, false, 0x37), /* eventChkInf[3] & 0x80 */
    ELF_MSG_END(0x64),
    ELF_MSG_FLAG(CHECK, 0x62, false, 0x25), /* eventChkInf[2] & 0x20 */
    ELF_MSG_FLAG(CHECK, 0x63, false, 0x37), /* eventChkInf[3] & 0x80 */
    ELF_MSG_FLAG(CHECK, 0x65, false, 0x43), /* eventChkInf[4] & 0x8 */
    ELF_MSG_MEDALLION(CHECK, 0x66, false, ITEM_MEDALLION_FOREST),
    ELF_MSG_MEDALLION(CHECK, 0x66, false, ITEM_MEDALLION_FIRE),
    ELF_MSG_MEDALLION(CHECK, 0x66, false, ITEM_MEDALLION_WATER),
    ELF_MSG_SONG(CHECK, 0x67, false, ITEM_SONG_STORMS),
    ELF_MSG_MEDALLION(CHECK, 0x68, false, ITEM_MEDALLION_SPIRIT),
    ELF_MSG_MEDALLION(CHECK, 0x68, false, ITEM_MEDALLION_SHADOW),
    ELF_MSG_END(0x69),
};

ElfMessage sAdultSariaMsgs[] = {
    ELF_MSG_MEDALLION(CHECK, 0x6A, false, ITEM_MEDALLION_FOREST),
    ELF_MSG_MEDALLION(CHECK, 0x6B, false, ITEM_MEDALLION_FIRE),
    ELF_MSG_MEDALLION(CHECK, 0x6B, false, ITEM_MEDALLION_WATER),
    ELF_MSG_MEDALLION(CHECK, 0x6C, false, ITEM_MEDALLION_SPIRIT),
    ELF_MSG_MEDALLION(CHECK, 0x6C, false, ITEM_MEDALLION_SHADOW),
    ELF_MSG_END(0x6D),
};

u32 ElfMessage_CheckCondition(ElfMessage* msg) {
    s32 type = msg->byte0 & 0x1E;
    u16 flag;

    switch (type) {
        case (ELF_MSG_CONDITION_FLAG << 1):
            flag = 1 << (msg->byte1 & 0x0F);
            return ((msg->byte0 & 1) == 1) == ((flag & gSaveContext.eventChkInf[(msg->byte1 & 0xF0) >> 4]) != 0);
        case (ELF_MSG_CONDITION_DUNGEON_ITEM << 1):
            return ((msg->byte0 & 1) == 1) ==
                   (CHECK_DUNGEON_ITEM(msg->byte1 - ITEM_KEY_BOSS, gSaveContext.mapIndex) != 0);
        case (ELF_MSG_CONDITION_ITEM << 1):
            return ((msg->byte0 & 1) == 1) == (msg->byte3 == INV_CONTENT(msg->byte1));
        case (ELF_MSG_CONDITION_OTHER << 1):
            switch (msg->byte1 & 0xF0) {
                case (ELF_MSG_CONDITION_STRENGTH_UPG << 4):
                    return ((msg->byte0 & 1) == 1) == ((msg->byte1 & 0x0F) == CUR_UPG_VALUE(UPG_STRENGTH));
                case (ELF_MSG_CONDITION_BOOTS << 4):
                    return ((msg->byte0 & 1) == 1) ==
                           (CHECK_OWNED_EQUIP(EQUIP_TYPE_BOOTS,
                                              msg->byte3 - ITEM_BOOTS_KOKIRI + EQUIP_INV_BOOTS_KOKIRI) != 0);
                case (ELF_MSG_CONDITION_SONG << 4):
                    return ((msg->byte0 & 1) == 1) ==
                           (CHECK_QUEST_ITEM(msg->byte3 - ITEM_SONG_MINUET + QUEST_SONG_MINUET) != 0);
                case (ELF_MSG_CONDITION_MEDALLION << 4):
                    return ((msg->byte0 & 1) == 1) ==
                           (CHECK_QUEST_ITEM(msg->byte3 - ITEM_MEDALLION_FOREST + QUEST_MEDALLION_FOREST) != 0);
                case (ELF_MSG_CONDITION_MAGIC << 4):
                    return ((msg->byte0 & 1) == 1) == (((void)0, gSaveContext.isMagicAcquired) != 0);
            }
    }

    LOG_STRING("企画外 条件"); // "Unplanned conditions"
    assert(0);

    return false;
}

u32 func_8006BE88(ElfMessage** msgp) {
    u32 temp = true;

    while (((*msgp)->byte0 & 0xE0) == (ELF_MSG_TYPE_UNK_1 << 5)) {
        if (!ElfMessage_CheckCondition(*msgp)) {
            temp = false;
        }
        *msgp += 1;
    }

    if (temp) {
        return ElfMessage_CheckCondition(*msgp);
    } else {
        return false;
    }
}

u32 func_8006BF1C(ElfMessage** msgp) {
    ElfMessage* msg = *msgp;
    u32 sp44[10];
    s32 temp1 = 0;
    s32 temp2 = 0;
    s32 temp3;

    do {
        sp44[temp2] = ElfMessage_CheckCondition(msg);
        temp1 += sp44[temp2];
        temp2++;
        msg++;
    } while ((msg->byte0 & 0xE0) == (ELF_MSG_TYPE_UNK_2 << 5));

    if (temp1 == 0) {
        return false;
    }

    temp3 = Rand_ZeroFloat(temp1);
    for (temp1 = 0; temp1 < temp2; temp1++) {
        if (sp44[temp1]) {
            if (temp3 > 0) {
                temp3--;
            } else {
                return true;
            }
        }
        *msgp += 1;
    }

    return false;
}

u16 ElfMessage_GetTextFromMsgs(ElfMessage* msg) {
    while (true) {
        switch (msg->byte0 & 0xE0) {
            case (ELF_MSG_TYPE_CHECK << 5):
                if (ElfMessage_CheckCondition(msg)) {
                    return msg->byte2 | 0x100;
                }
                break;
            case (ELF_MSG_TYPE_UNK_1 << 5):
                if (func_8006BE88(&msg)) {
                    return msg->byte2 | 0x100;
                }
                break;
            case (ELF_MSG_TYPE_UNK_2 << 5):
                if (func_8006BF1C(&msg)) {
                    return msg->byte2 | 0x100;
                }
                break;
            case (ELF_MSG_TYPE_SKIP << 5):
                if (ElfMessage_CheckCondition(msg)) {
                    msg += msg->byte2;
                    msg--;
                }
                break;
            case (ELF_MSG_TYPE_END << 5):
                return msg->byte2 | 0x100;
            default:
                LOG_STRING("企画外 条件"); // "Unplanned conditions"
                assert(0);
        }
        msg++;
    }
}

#define NO_ADULT_SPECIAL_MSG 5

u16 ElfMessage_GetSariaText(PlayState* play, s8 status) {
    Player* player = GET_PLAYER(play);
    ElfMessage* msgs;
    u16 SariaMsg = GetTextID("saria");

    if (!LINK_IS_ADULT) {
        if (Actor_FindNearby(play, &player->actor, ACTOR_EN_SA, 4, 800.0f) == NULL) {
             if (IS_RANDO && Randomizer_GetSettingValue(RSK_SARIA_HINT)) {
                return 0x161;
            }
            if (status == 0) {
                if (play->sceneNum == SCENE_KOKIRI_FOREST) {
                    if (isSariaAtLinksHouse())
                        return SariaMsg+26;
                    else {
                        if (gSaveContext.infTable[27] & (1<<2)) {
                            if (getDayOfCycle() % 3 == 2)
                                return SariaMsg+24;
                            else
                                return SariaMsg+23;
                        } else
                            return SariaMsg;
                    }
                } else if (play->sceneNum == SCENE_LINKS_HOUSE) {
                    gSaveContext.infTable[27] |= (1<<0);
                    gSaveContext.SariaDateDay = gSaveContext.totalDays+1;
                    gSaveContext.eventInf[3] &= ~(1<<1);
                    return SariaMsg+1;
                } else if (play->sceneNum == SCENE_KOKIRI_SHOP)
                    return SariaMsg+2;
                else if (play->sceneNum == SCENE_TWINS_HOUSE) {
                    if (gSaveContext.infTable[27] & (1<<2))
                        return SariaMsg+3;
                    else {
                        if (gSaveContext.infTable[27] & (1<<1)) {
                            gSaveContext.infTable[27] |= (1<<2);
                            return SariaMsg+22;
                        } else {
                            return SariaMsg+25;
                        }
                    }
                } else if (play->sceneNum == SCENE_MIDOS_HOUSE)
                    return SariaMsg+4;
                else if (play->sceneNum == SCENE_SARIAS_HOUSE)
                    return SariaMsg+5;
                else if (play->sceneNum == SCENE_KNOW_IT_ALL_BROS_HOUSE)
                    return SariaMsg+6;
                else if (play->sceneNum == SCENE_MARKET_DAY)
                    return SariaMsg+7;
                else if (play->sceneNum == SCENE_LAKE_HYLIA)
                    return SariaMsg+8;
                else if (play->sceneNum == SCENE_GRAVEYARD || play->sceneNum == SCENE_GRAVEKEEPERS_HUT)
                    return SariaMsg+9;
                else if (play->sceneNum == SCENE_ZORAS_DOMAIN)
                    return SariaMsg+10;
                else if (play->sceneNum == SCENE_HYRULE_CASTLE)
                    return SariaMsg+11;
                else if (play->sceneNum == SCENE_LON_LON_RANCH || play->sceneNum == SCENE_LON_LON_BUILDINGS)
                    return SariaMsg+12;
                else if (play->sceneNum == SCENE_STABLE) {
                    if (IS_DAY)
                        return SariaMsg+13;
                    else
                        return SariaMsg+14;
                }
            }

            msgs = sChildSariaMsgs;
        } else {
            return 0x0160; // Special text about Saria preferring to talk to you face-to-face
        }
    } else {
        u16 msg1 = ElfMessage_GetTextFromMsgs(sAdultSariaMsgs);
        s16 randInt = Rand_S16Offset(0, 3);
        s16 messageNumbers[NO_ADULT_SPECIAL_MSG];
        s32 ii = 0;
        msgs = sAdultSariaMsgs;
        // for (s32 ii = 0; ii < NO_ADULT_SPECIAL_MSG; ii++) {

        // }
        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_LIGHT))
            messageNumbers[ii++] = 0;
        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE))
            messageNumbers[ii++] = 1;
        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_WATER))
            messageNumbers[ii++] = 2;
        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SHADOW))
            messageNumbers[ii++] = 3;
        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT))
            messageNumbers[ii++] = 4;

        if ((!(msg1 == 0x16C) && status == 0) || randInt) {
            if (msg1 >= 0x16B && ii > 0) {
                randInt = Rand_S16Offset(0, ii);
                if (randInt >= ii)
                    randInt = ii-1;
                return SariaMsg+30+messageNumbers[randInt];
            }
        }
    }

    return ElfMessage_GetTextFromMsgs(msgs);
}

#define NO_FAIRY_SPECIAL_MSG_MAX 10

u16 sMessagePersistance[NO_FAIRY_SPECIAL_MSG_MAX];
s16 sMessagePersistanceLength = 0;
s16 sMessagePersistanceCurrent = 0;

void ElfMessage_SetPersistantElfMessages(u16 *msgArray, s16 length) {
    sMessagePersistanceLength = length;
    for (s16 ii = 0; ii < sMessagePersistanceLength; ii++) {
        sMessagePersistance[ii] = msgArray[ii];
    }
    sMessagePersistanceCurrent = 0;
}

void ElfMessage_SetPersistantElfCurrent(s16 currentVal) {
    sMessagePersistanceCurrent = currentVal;
}

void ElfMessage_ResetPersistantElfMessages() {
    ElfMessage_SetPersistantElfMessages(NULL, 0);
}

s16 ElfMessage_ComparePersistantElfMessages(u16 *msgArray, s16 length) {
    if (sMessagePersistanceLength != length)
        return 0;
    for (s16 ii = 0; ii < sMessagePersistanceLength; ii++) {
        if (sMessagePersistance[ii] != msgArray[ii])
            return 0;
    }
    return 1;
}

u16 ElfMessage_GetCUpText(PlayState* play) {
    if (play->cUpElfMsgs == NULL) {
        return 0;
    } else {
        return ElfMessage_GetTextFromMsgs(play->cUpElfMsgs);
    }
}

u16 ElfMessage_GetSpecialNaviText(PlayState* play) {
    if (play->cUpElfMsgs == NULL) {
        u16 messageNumbers[NO_FAIRY_SPECIAL_MSG_MAX];
        messageNumbers[0] = 0;
        ElfMessage_SetPersistantElfMessages(messageNumbers, 1);
        return 0;
    } else {
        u16 tempMsg = ElfMessage_GetTextFromMsgs(play->cUpElfMsgs);
        s16 randInt;
        u16 messageNumbers[NO_FAIRY_SPECIAL_MSG_MAX];

        if (0x150 <= tempMsg && tempMsg <= 0x153) {
            s32 ii = 0;
            s16 bootsObtainedIron = ((gBitFlags[ITEM_BOOTS_IRON - ITEM_BOOTS_KOKIRI] << gEquipShifts[EQUIP_TYPE_BOOTS]) & gSaveContext.inventory.equipment);
            messageNumbers[ii++] = 0x015D;//Navi reminder about Sheik's message
            if (!(gSaveContext.eventChkInf[0] & 0x4000) && LINK_IS_CHILD)
                messageNumbers[ii++] = 0x010B;//Reminder to talk to Know-it-All bros.
            if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_FOREST))
                messageNumbers[ii++] = 0x150;
            if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE))
                messageNumbers[ii++] = 0x151;
            if (!bootsObtainedIron)
                messageNumbers[ii++] = 0x152;
            else if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_WATER))
                messageNumbers[ii++] = 0x153;
            if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_FOREST) && !CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE) && !CHECK_QUEST_ITEM(QUEST_MEDALLION_WATER))
                messageNumbers[ii++] = 0x010A;

            if (ii > 0) {
                if (!ElfMessage_ComparePersistantElfMessages(messageNumbers, ii)){
                    ElfMessage_SetPersistantElfMessages(messageNumbers, ii);
                    sMessagePersistanceCurrent = Rand_S16Offset(0, ii);
                }
                tempMsg = messageNumbers[sMessagePersistanceCurrent];
            }

            return tempMsg;
        }

        if (0x154 <= tempMsg && tempMsg <= 0x15A) {
            s32 ii = 0;
            messageNumbers[ii++] = 0x015D;//Navi reminder about Sheik's message
            if (!(gSaveContext.eventChkInf[0] & 0x4000) && LINK_IS_CHILD)
                messageNumbers[ii++] = 0x010B;//Reminder to talk to Know-it-All bros.
            if (!(gSaveContext.eventChkInf[0xA]&(1<<0xA)))
                messageNumbers[ii++] = 0x154;
            else if (INV_CONTENT(ITEM_LENS) == ITEM_NONE)
                messageNumbers[ii++] = 0x155;
            else if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_SHADOW))
                messageNumbers[ii++] = 0x157;

            if (!CHECK_QUEST_ITEM(ITEM_SONG_REQUIEM - ITEM_SONG_MINUET + QUEST_SONG_MINUET))
                messageNumbers[ii++] = 0x158;
            else if (CUR_UPG_VALUE(UPG_STRENGTH) < 2)
                messageNumbers[ii++] = 0x156;
            else if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT))
                messageNumbers[ii++] = 0x15A;

            if (ii > 0) {
                if (!ElfMessage_ComparePersistantElfMessages(messageNumbers, ii)) {
                    ElfMessage_SetPersistantElfMessages(messageNumbers, ii);
                    sMessagePersistanceCurrent = Rand_S16Offset(0, ii);
                }
                tempMsg = messageNumbers[sMessagePersistanceCurrent];
            }

            return tempMsg;
        }

        if (0x145 <= tempMsg && tempMsg <= 0x14A) {
            s32 ii = 0;
            if (!(gSaveContext.eventChkInf[0] & 0x4000))
                messageNumbers[ii++] = 0x010B;
            if (!CHECK_QUEST_ITEM(ITEM_SONG_SARIA - ITEM_SONG_MINUET + QUEST_SONG_MINUET)) {
                messageNumbers[ii++] = 0x145;
                messageNumbers[ii++] = 0x146;
            } else if (CUR_UPG_VALUE(UPG_STRENGTH) < 1)
                messageNumbers[ii++] = 0x146;
            else if (!(gSaveContext.eventChkInf[2]&(1<<5)))
                messageNumbers[ii++] = 0x147;
            else if (!gSaveContext.isMagicAcquired)
                messageNumbers[ii++] = 0x148;
            else if (!(gSaveContext.eventChkInf[3]&(1<<3)))
                messageNumbers[ii++] = 0x149;

            if ((gSaveContext.eventChkInf[3]&(1<<3)) && !(gSaveContext.eventChkInf[3]&(1<<7)))
                messageNumbers[ii++] = 0x14A;

            if (ii > 0) {
                if (!ElfMessage_ComparePersistantElfMessages(messageNumbers, ii)) {
                    ElfMessage_SetPersistantElfMessages(messageNumbers, ii);
                    sMessagePersistanceCurrent = Rand_S16Offset(0, ii);
                }
                tempMsg = messageNumbers[sMessagePersistanceCurrent];
            }

            return tempMsg;
        } else if (0x14B <= tempMsg && LINK_IS_CHILD) {
            s32 ii = 0;
            if (!(gSaveContext.eventChkInf[0] & 0x4000))
                messageNumbers[ii++] = 0x010B;
            messageNumbers[ii++] = tempMsg;
            if (ii > 0) {
                if (!ElfMessage_ComparePersistantElfMessages(messageNumbers, ii)) {
                    ElfMessage_SetPersistantElfMessages(messageNumbers, ii);
                    sMessagePersistanceCurrent = Rand_S16Offset(0, ii);
                }
                tempMsg = messageNumbers[sMessagePersistanceCurrent];
            }

            return tempMsg;
        }

        messageNumbers[0] = tempMsg;
        ElfMessage_SetPersistantElfMessages(messageNumbers, 1);
        return tempMsg;
    }
}

u16 ElfMessage_SelectSpecialNaviText(PlayState* play) {
    if (sMessagePersistanceLength == 0)
        ElfMessage_GetSpecialNaviText(play);
    if (++sMessagePersistanceCurrent >= sMessagePersistanceLength)
        sMessagePersistanceCurrent = 0;
    return sMessagePersistance[sMessagePersistanceCurrent];
}
