#include "OTRGlobals.h"
#include <libultraship/ResourceMgr.h>
#include <libultraship/Scene.h>
#include <Utils/StringHelper.h>
#include "global.h"
#include "vt.h"
#include <libultraship/Text.h>
#include <message_data_static.h>
#include "Enhancements/custom-message/CustomMessageManager.h"
#include "Enhancements/custom-message/CustomMessageTypes.h"

extern "C" MessageTableEntry* sNesMessageEntryTablePtr;
extern "C" MessageTableEntry* sGerMessageEntryTablePtr;
extern "C" MessageTableEntry* sFraMessageEntryTablePtr;
extern "C" MessageTableEntry* sStaffMessageEntryTablePtr;
//extern "C" MessageTableEntry* _message_0xFFFC_nes;	

MessageTableEntry* OTRMessage_LoadTable(const char* filePath, bool isNES) {
    auto file = std::static_pointer_cast<Ship::Text>(OTRGlobals::Instance->context->GetResourceManager()->LoadResource(filePath));

    if (file == nullptr)
        return nullptr;
    
    // Allocate room for an additional message
    MessageTableEntry* table = (MessageTableEntry*)malloc(sizeof(MessageTableEntry) * (file->messages.size() + 1));

    for (int i = 0; i < file->messages.size(); i++) {
        // Look for Owl Text
        if (file->messages[i].id == 0x2066) {
            // Create a new message based on the Owl Text
            char* kaeporaPatch = (char*)malloc(sizeof(char) * file->messages[i].msg.size());
            file->messages[i].msg.copy(kaeporaPatch, file->messages[i].msg.size(), 0);

            // Swap the order of yes and no in this new message
            if (filePath == "text/nes_message_data_static/nes_message_data_static") {
                kaeporaPatch[26] = 'Y';
                kaeporaPatch[27] = 'e';
                kaeporaPatch[28] = 's';
                kaeporaPatch[29] = 1;
                kaeporaPatch[30] = 'N';
                kaeporaPatch[31] = 'o';
            } else if (filePath == "text/ger_message_data_static/ger_message_data_static") {
                kaeporaPatch[30] = 'J';
                kaeporaPatch[31] = 'a';
                kaeporaPatch[32] = '!';
                kaeporaPatch[33] = 1;
                kaeporaPatch[34] = 'N';
                kaeporaPatch[35] = 'e';
                kaeporaPatch[36] = 'i';
                kaeporaPatch[37] = 'n';
            } else {
                kaeporaPatch[26] = 'O';
                kaeporaPatch[27] = 'u';
                kaeporaPatch[28] = 'i';
                kaeporaPatch[29] = 1;
                kaeporaPatch[30] = 'N';
                kaeporaPatch[31] = 'o';
                kaeporaPatch[32] = 'n';
            }

            // load data into message
            table[file->messages.size()].textId = 0x71B3;
            table[file->messages.size()].typePos = (file->messages[i].textboxType << 4) | file->messages[i].textboxYPos;
            table[file->messages.size()].segment = kaeporaPatch;
            table[file->messages.size()].msgSize = file->messages[i].msg.size();
        }

        table[i].textId = file->messages[i].id;
        table[i].typePos = (file->messages[i].textboxType << 4) | file->messages[i].textboxYPos;
        table[i].segment = file->messages[i].msg.c_str();
        table[i].msgSize = file->messages[i].msg.size();

        if (isNES && file->messages[i].id == 0xFFFC)
            _message_0xFFFC_nes = (char*)file->messages[i].msg.c_str();
    }

	return table;
}

extern "C" void OTRMessage_Init()
{
    sNesMessageEntryTablePtr = OTRMessage_LoadTable("text/nes_message_data_static/nes_message_data_static", true);
    sGerMessageEntryTablePtr = OTRMessage_LoadTable("text/ger_message_data_static/ger_message_data_static", false);
    sFraMessageEntryTablePtr = OTRMessage_LoadTable("text/fra_message_data_static/fra_message_data_static", false);

	auto file2 = std::static_pointer_cast<Ship::Text>(OTRGlobals::Instance->context->GetResourceManager()->LoadResource("text/staff_message_data_static/staff_message_data_static"));

	sStaffMessageEntryTablePtr = (MessageTableEntry*)malloc(sizeof(MessageTableEntry) * file2->messages.size());

	for (int i = 0; i < file2->messages.size(); i++)
	{
		sStaffMessageEntryTablePtr[i].textId = file2->messages[i].id;
		sStaffMessageEntryTablePtr[i].typePos = (file2->messages[i].textboxType << 4) | file2->messages[i].textboxYPos;
		sStaffMessageEntryTablePtr[i].segment = file2->messages[i].msg.c_str();
		sStaffMessageEntryTablePtr[i].msgSize = file2->messages[i].msg.size();
	}

    CustomMessageManager::Instance->AddCustomMessageTable(customMessageTableID);
    CustomMessageManager::Instance->AddCustomMessageTable(questMessageTableID);
    CustomMessageManager::Instance->CreateGetItemMessage(
        customMessageTableID, (GetItemID)TEXT_GS_NO_FREEZE, ITEM_SKULL_TOKEN,
        { 
            TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
            "You got a %rGold Skulltula Token%w!&You've collected %r{{gsCount}}%w tokens&in total!\x0E\x3C",
            "Du erhälst ein %rGoldene&Skulltula-Symbol%w! Du hast&insgesamt %r{{gsCount}}%w symbol gesammelt!\x0E\x3C",
            "Vous obtenez un %rSymbole de&Skulltula d'or%w! Vous avez&collecté %r{{gsCount}}%w symboles en tout!\x0E\x3C"
        }
    );
    CustomMessageManager::Instance->CreateGetItemMessage(
        customMessageTableID, (GetItemID)TEXT_GS_FREEZE, ITEM_SKULL_TOKEN,
        { 
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "You got a %rGold Skulltula Token%w!&You've collected %r{{gsCount}}%w tokens&in total!",
          "Du erhälst ein %rGoldene&Skulltula-Symbol%w! Du hast&insgesamt %r{{gsCount}}%w symbol gesammelt!",
          "Vous obtenez un %rSymbole de&Skulltula d'or%w! Vous avez&collecté %r{{gsCount}}%w symboles en tout!"
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        customMessageTableID, TEXT_BUY_BOMBCHU_10_DESC,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "\x08%rBombchu  (10 pieces)  99 Rupees&%wThis looks like a toy mouse, but&it's actually a self-propelled time&bomb!\x09\x0A",
          "\x08%rKrabbelmine  10 Stück  99 Rubine&%wDas ist eine praktische Zeitbombe,&die Du als Distanzwaffe&einsetzen kannst!\x09\x0A",
          "\x08%rMissile  10 unités  99 Rubis&%wProfilée comme une souris&mécanique, cette arme est &destructrice!!!\x09\x0A",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        customMessageTableID, TEXT_BUY_BOMBCHU_10_PROMPT,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "\x08" "Bombchu  10 pieces   99 Rupees\x09&&\x1B%gBuy&Don't buy%w",
          "\x08Krabbelmine  10 Stück  99 Rubine\x09&&\x1B%gKaufen!&Nicht kaufen!%w",
          "\x08Missiles  10 unités   99 Rubis\x09&&\x1B%gAcheter&Ne pas acheter%w",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x0170,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "It's nice that you are dropping by the&village again. I've heard the twins have&been arranging something special!\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x0171,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "\x0F, you've been collecting a lot of&things in your place, haven't you?&Maybe I should come around sometime.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x0176,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "The guys here know a lot of things.&But surely, there has to be so much&more outside the forest to discover.^\x0F, I want you you to teach&me about the world. Talk to me&about any places you discover.^I'm just so curious.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x0172,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "The shopkeeper doesn't do a very& good job of hiding everything in&the store. Tee hee!\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x0173,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "I shouldn't be admitting this to you,&but even I sometimes get the twins&confused with the other girls.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x0174,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "You probably shouldn't let&Mido know I'm talking to you like this...\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x0175,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "Make yourself at home.&I don't mind if other people&clean things up a bit for me.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x8000,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Hey, you know, it's so strange what&happened to the Great Deku Tree.^Things haven't been&growing in the forrest lately,&but hopefully that will change.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x8001,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Tee hee hee, a lot of Kokiri&are good at hiding secrets.^But some aren't as&good as it as others.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x8002,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Hey, wouldn't it be funny, if \x14\x03someone\x14\x01&played a prank on Mido...^Saria is maybe a bit too nice to him&for his own good.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x1074,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "You know, I wish the vegitation&around here had gotten&more water years ago.^If that had happened, there's no doubt&that things would have grown&a mighty lot greater, and the&forest would be a better place for it.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x8004,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "As it is, plants are barely&growing at all, and we're&only getting weeds.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x8005,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Things are growing somewhat,&but there are still places&that could be better...",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x8006,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "I've spent years tending to&the plants in this forest,&and would you look&at what's happened.^Bountiful foliage all around,&all displaying the result of&putting in the effort to&make things grow right.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x8007,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "You bet I'm proud! In fact,&since I'm feeling so generous,^I'm happy to share some of&the life the forest has in excess.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x8886,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "You know, I think there are places&in the lost woods that no Kokiri&now remembers how to get to...&\x0D\x14\x03...except one...^Oops, I shouldn't have said that...\x07\x88\x87",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x8887,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "This is no longer a secret&to everyone anymore!",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, 0x8888,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "I'll give you this, if you&promise to never let anyone know.",
          "",
          "",
        }
    );
}
