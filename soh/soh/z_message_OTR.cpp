#include "OTRGlobals.h"
#include <libultraship/libultraship.h>
#include "soh/resource/type/Scene.h"
#include <Utils/StringHelper.h>
#include "global.h"
#include "vt.h"
#include "soh/resource/type/Text.h"
#include <message_data_static.h>
#include "Enhancements/custom-message/CustomMessageManager.h"
#include "Enhancements/custom-message/CustomMessageTypes.h"
#include "src/overlays/actors/ovl_En_Hy/z_en_hy.h"

extern "C" MessageTableEntry* sNesMessageEntryTablePtr;
extern "C" MessageTableEntry* sGerMessageEntryTablePtr;
extern "C" MessageTableEntry* sFraMessageEntryTablePtr;
extern "C" MessageTableEntry* sStaffMessageEntryTablePtr;
extern std::unordered_map<uint16_t, uint16_t>* textIDSubstitutionTable;
//extern "C" MessageTableEntry* _message_0xFFFC_nes;	

MessageTableEntry* OTRMessage_LoadTable(const char* filePath, bool isNES) {
    auto file = std::static_pointer_cast<LUS::Text>(LUS::Context::GetInstance()->GetResourceManager()->LoadResource(filePath));

    if (file == nullptr)
        return nullptr;
    
    // Allocate room for an additional message
    // OTRTODO: Should not be malloc'ing here. It's fine for now since we check elsewhere that the message table is
    // already null.
    MessageTableEntry* table = (MessageTableEntry*)malloc(sizeof(MessageTableEntry) * (file->messages.size() + 1));

    for (size_t i = 0; i < file->messages.size(); i++) {
        // Look for Owl Text
        if (file->messages[i].id == 0x2066) {
            // Create a new message based on the Owl Text
            uint32_t kaeporaMsgSize = file->messages[i].msg.size();
            // OTRTODO: Should not be malloc'ing here. It's fine for now since we check elsewhere that the message table
            // is already null.
            char* kaeporaOg = (char*)malloc(sizeof(char) * kaeporaMsgSize);
            char* kaeporaPatch = (char*)malloc(sizeof(char) * kaeporaMsgSize);
            file->messages[i].msg.copy(kaeporaOg, kaeporaMsgSize, 0);
            file->messages[i].msg.copy(kaeporaPatch, kaeporaMsgSize, 0);

            size_t colorPos = file->messages[i].msg.find(QM_GREEN);
            size_t newLinePos = colorPos + file->messages[i].msg.substr(colorPos + 1).find(CTRL_NEWLINE) + 1;
            size_t endColorPos = newLinePos + file->messages[i].msg.substr(newLinePos).find(CTRL_COLOR);
            size_t NoLength = newLinePos - (colorPos + 1);
            size_t YesLength = endColorPos - (newLinePos + 1);
            // Swap the order of yes and no in this new message
            size_t yes = 0;
            while (yes < YesLength) {
                kaeporaPatch[colorPos + yes + 1] = kaeporaOg[newLinePos + yes + 1];
                yes++;
            }
            kaeporaPatch[colorPos + yes + 1] = CTRL_NEWLINE;
            size_t no = 0;
            while (no < NoLength) {
                kaeporaPatch[colorPos + yes + 2 + no] = kaeporaOg[colorPos + 1 + no];
                no++;
            }

            // load data into message
            table[file->messages.size()].textId = 0x71B3;
            table[file->messages.size()].typePos = (file->messages[i].textboxType << 4) | file->messages[i].textboxYPos;
            table[file->messages.size()].segment = kaeporaPatch;
            table[file->messages.size()].msgSize = kaeporaMsgSize;
        }

        table[i].textId = file->messages[i].id;
        table[i].typePos = (file->messages[i].textboxType << 4) | file->messages[i].textboxYPos;
        table[i].segment = file->messages[i].msg.c_str();
        table[i].msgSize = file->messages[i].msg.size();

        if (isNES && file->messages[i].id == 0xFFFC)
            _message_0xFFFC_nes = (char*)file->messages[i].msg.c_str();
    }

    // Assert that the first message starts at the first text ID
    assert(table[0].textId == 0x0001);

    return table;
}

#define MakeNormalMsgEng(txtID, text) \
    CustomMessageManager::Instance->CreateMessage(\
        questMessageTableID, (txtID) ,\
        CustomMessage(\
          (text) ,\
          "",\
          "",\
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM\
        )\
    );

#define MakeBlueMsgEng(txtID, text) \
    CustomMessageManager::Instance->CreateMessage(\
        questMessageTableID, (txtID) ,\
        CustomMessage(\
          (text) ,\
          "",\
          "",\
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM\
        )\
    );

#define MakeContinuingMsgEng(txtID, text, nextID, staticVar)\
    u16 staticVar = (nextID) ;\
    static std::string staticVar##Str = text ;\
    staticVar##Str.push_back((char) 0x07); \
    staticVar##Str.push_back((char)(( staticVar >>8)&0xFF));\
    staticVar##Str.push_back((char)(( staticVar )&0xFF));\
    MakeNormalMsgEng(txtID, staticVar##Str);\


static int numFish = 0;
static std::string msgFishStr = "Now there are"+std::to_string(numFish)+"\x14\x01 of them!";

std::string provideCountingString(int num) {
    if (num == 1)
        return "...1";
    else if (num == 2)
        return "Now there are two of them!";
    else
    {
        std::string accumulation = "Now there are...&\x14\x03...1&..2^\x14\x03";
        for (int ii=3; ii<=num;ii++)
        {
            if (ii == num)
            {
                accumulation += ".\x14\x09..\x14\x01"+std::to_string(ii);
            }
            else
            {
                if ((ii-2)%3)
                    accumulation += "..."+std::to_string(ii)+"&";
                else
                    accumulation += "..."+std::to_string(ii)+"^\x14\x03";
            }
        }
        return accumulation+"\x14\x01 of them!";
    }
    return "none";
}

//For KokiriMsg+24
extern "C" void createFishString(int num)
{
    //"Now there are \x14\x4"+std::to_string(numFish)+"\x14\x1of them swishing around!";
    if (numFish != num)
    {
        u16 KokiriMsg = TextIDAllocator::Instance->getId("kokiri");
        numFish = num;
        msgFishStr = provideCountingString(numFish);
        CustomMessageManager::Instance->ReplaceMessage(
        questMessageTableID, KokiriMsg+24,
        CustomMessage(
          msgFishStr,
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM));
    }
}

static std::string msgRupeeScoreStr = "The most you've ever made is:&%r"+std::to_string(0)+" rupees%w.^What do you say?&\x1B&%gYes&No%w\x02";

extern "C" void createRupeeScoreString(int score)
{
    u16 CursedFamilyMsg = TextIDAllocator::Instance->getId("cursedfamily");
    msgRupeeScoreStr = "The most you've ever made is:&%r"+std::to_string(gSaveContext.rupeeCollectionScore)+" rupees%w.^What do you say?&\x1B&%gYes&No%w\x02";
    CustomMessageManager::Instance->ReplaceMessage(
    questMessageTableID, CursedFamilyMsg+5,
    CustomMessage(
      msgRupeeScoreStr,
      "",
      "",
      TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM));
}

extern "C" void OTRMessage_Init()
{
    TextIDAllocator::Instance->reset();
    // OTRTODO: Added a lot of null checks here so that we don't malloc the table multiple times causing a memory leak.
    // We really ought to fix the implementation such that we aren't malloc'ing new tables.
    // Once we fix the implementation, remove these NULL checks.
    if (sNesMessageEntryTablePtr == NULL) {
        sNesMessageEntryTablePtr = OTRMessage_LoadTable("text/nes_message_data_static/nes_message_data_static", true);
    }
    if (sGerMessageEntryTablePtr == NULL) {
        sGerMessageEntryTablePtr = OTRMessage_LoadTable("text/ger_message_data_static/ger_message_data_static", false);
    }
    if (sFraMessageEntryTablePtr == NULL) {
        sFraMessageEntryTablePtr = OTRMessage_LoadTable("text/fra_message_data_static/fra_message_data_static", false);
    }

    if (sStaffMessageEntryTablePtr == NULL) {
        auto file2 =
            std::static_pointer_cast<LUS::Text>(LUS::Context::GetInstance()->GetResourceManager()->LoadResource(
                "text/staff_message_data_static/staff_message_data_static"));
        // OTRTODO: Should not be malloc'ing here. It's fine for now since we check that the message table is already null.
        sStaffMessageEntryTablePtr = (MessageTableEntry*)malloc(sizeof(MessageTableEntry) * file2->messages.size());

        for (size_t i = 0; i < file2->messages.size(); i++) {
            sStaffMessageEntryTablePtr[i].textId = file2->messages[i].id;
            sStaffMessageEntryTablePtr[i].typePos =
                (file2->messages[i].textboxType << 4) | file2->messages[i].textboxYPos;
            sStaffMessageEntryTablePtr[i].segment = file2->messages[i].msg.c_str();
            sStaffMessageEntryTablePtr[i].msgSize = file2->messages[i].msg.size();
        }

        // Assert staff credits start at the first credits ID
        assert(sStaffMessageEntryTablePtr[0].textId == 0x0500);
    }

    CustomMessageManager::Instance->AddCustomMessageTable(customMessageTableID);
    CustomMessageManager::Instance->AddCustomMessageTable(questMessageTableID);
    CustomMessageManager::Instance->CreateGetItemMessage(
        customMessageTableID, (GetItemID)TEXT_GS_NO_FREEZE, ITEM_SKULL_TOKEN,
        CustomMessage("You got a %rGold Skulltula Token%w!&You've collected %r{{gsCount}}%w tokens&in total!\x0E\x3C",
                      "Ein %rGoldenes Skulltula-Symbol%w!&Du hast nun insgesamt %r{{gsCount}}&%wGoldene "
                      "Skulltula-Symbole&gesammelt!\x0E\x3C",
                      "Vous obtenez un %rSymbole de&Skulltula d'or%w! Vous avez&collecté %r{{gsCount}}%w symboles en "
                      "tout!\x0E\x3C",
                      TEXTBOX_TYPE_BLUE));
    CustomMessageManager::Instance->CreateGetItemMessage(
        customMessageTableID, (GetItemID)TEXT_GS_FREEZE, ITEM_SKULL_TOKEN,
        CustomMessage(
            "You got a %rGold Skulltula Token%w!&You've collected %r{{gsCount}}%w tokens&in total!",
            "Ein %rGoldenes Skulltula-Symbol%w!&Du hast nun insgesamt %r{{gsCount}}&%wGoldene "
            "Skulltula-Symbole&gesammelt!",
            "Vous obtenez un %rSymbole de&Skulltula d'or%w! Vous avez&collecté %r{{gsCount}}%w symboles en tout!",
            TEXTBOX_TYPE_BLUE));
    CustomMessageManager::Instance->CreateMessage(
        customMessageTableID, TEXT_BUY_BOMBCHU_10_DESC,
        CustomMessage("\x08%rBombchu  (10 pieces)  99 Rupees&%wThis looks like a toy mouse, but&it's actually a "
                      "self-propelled time&bomb!\x09\x0A",
                      "\x08%rKrabbelmine  10 Stück  99 Rubine&%wDas ist eine praktische Zeitbombe,&die Du als "
                      "Distanzwaffe&einsetzen kannst!\x09\x0A",
                      "\x08%rMissile  10 unités  99 Rubis&%wProfilée comme une souris&mécanique, cette arme est "
                      "&destructrice!!!\x09\x0A"));
    CustomMessageManager::Instance->CreateMessage(
        customMessageTableID, TEXT_BUY_BOMBCHU_10_PROMPT,
        CustomMessage("\x08"
                      "Bombchu  10 pieces   99 Rupees\x09&&\x1B%gBuy&Don't buy%w",
                      "\x08Krabbelmine  10 Stück  99 Rubine\x09&&\x1B%gKaufen!&Nicht kaufen!%w",
                      "\x08Missiles  10 unités   99 Rubis\x09&&\x1B%gAcheter&Ne pas acheter%w"));
    CustomMessageManager::Instance->CreateGetItemMessage(
        customMessageTableID, (GetItemID)TEXT_HEART_CONTAINER, ITEM_HEART_CONTAINER,
        CustomMessage(
            "You got a %rHeart Container%w!&You've collected %r{{heartContainerCount}}%w containers&in total!",
            "Ein %rHerzcontainer%w!&Du hast nun insgesamt %r{{heartContainerCount}}%w&Herzcontainer gesammelt!",
            "Vous obtenez un %rCoeur&d'Energie%w! Vous en avez&collecté %r{{heartContainerCount}}%w en tout!"));
    CustomMessageManager::Instance->CreateGetItemMessage(
        customMessageTableID, (GetItemID)TEXT_HEART_PIECE, ITEM_HEART_PIECE,
        CustomMessage("You got a %rHeart Piece%w!&You've collected %r{{heartPieceCount}}%w pieces&in total!",
                      "Ein %rHerzteil%w!&Du hast nun insgesamt %r{{heartPieceCount}}%w&Herteile gesammelt!",
                      "Vous obtenez un %rQuart de&Coeur%w! Vous en avez collecté&%r{{heartPieceCount}}%w en tout!",
                      TEXTBOX_TYPE_BLUE));
    CustomMessageManager::Instance->CreateMessage(
        customMessageTableID, TEXT_MARKET_GUARD_NIGHT,
        CustomMessage("You look bored. Wanna go out for a&walk?\x1B&%gYes&No%w",
                      "Du siehst gelangweilt aus.&Willst du einen Spaziergang machen?\x1B&%gJa&Nein%w",
                      "Tu as l'air de t'ennuyer. Tu veux&aller faire un tour?\x1B&%gOui&Non%w"));

    u16 WonderMsg = 0x0200;
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, WonderMsg+0x3D,
        CustomMessage(
          "You're looking good today \x0F!",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_MIDDLE)
    );

    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, WonderMsg+0x3E,
        CustomMessage(
          "Wow, you sure have grown!&You're still not quite&used to seeing yourself&as an adult, are you?",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_MIDDLE)
    );

    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, WonderMsg+0x3F,
        CustomMessage(
          "History of Hyrule's days.^In this land, each day,&with its characteristic Moon,&has a unique reason and purpose&for the people living under its sky.^The first day, with the biggest&and brightest Moon, is a&day of beginnings,&love and renewal.^The second, the day of the&Golden Moon, is a day of industry&and seriousness of work.^The third, the day of Moonfire,&is a day of the markets.^Many are filled with wonder at&the kinds of things that&this day brings.",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_MIDDLE)
    );

    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, WonderMsg+0x40,
        CustomMessage(
          "The fourth day, the darkest&night, a day for testing&and understanding oneself, and&also one that brings mystery.^The fifth, a day loved&by the multitudes, is a day of&festivity and celebration.^And finally the sixth,&the day of rest, one even&more loved by some.",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_MIDDLE)
    );

    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, WonderMsg+0x41,
        CustomMessage(
          "You're looking good tonight \x0F!",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_MIDDLE)
    );

    u16 ElfMsg = 0x0100;
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, ElfMsg+0xB2,
        CustomMessage(
          "\x22""Dread the touch&of we the living dead.\x22",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, ElfMsg+0xB3,
        CustomMessage(
          "\x22""Respect the ears&of us the living dead.&Sound disturbs our rest.\x22",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, ElfMsg+0xB4,
        CustomMessage(
          "\x22""Do not fear the eyes&of us living dead...&Sight is no longer with us.\x22",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, ElfMsg+0xB5,
        CustomMessage(
          "\x22""Taking out and putting away&weapons, what irksome sounds&are those to us!\x22",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, ElfMsg+0xB6,
        CustomMessage(
          "\x22""Those who tread too quickly&and hastily are most irksome&to us!\x22",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, ElfMsg+0xB7,
        CustomMessage(
          "\x22""When meeting our master,&above all, do not panic.\x22",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, ElfMsg+0xB8,
        CustomMessage(
          "\x22""Down here, even the faintest&light of the sun cannot reach.^We are thankful for that.\x22",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, ElfMsg+0xB9,
        CustomMessage(
          "\x22""Those who do not disturb us&need not worry.\x22",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, WonderMsg+0xBA,
        CustomMessage(
          "Each day is associated with an&element fitting to its character.^The first, the Etherial,&heavenly radiance and feeling.&The second, %yGold%w and&solid precious metal.^The third, %rFire%w and ash.&The forth, %gEarth%w and soil.&The fifth, %bWater%w and the living ocean.&The sixth, the %cAir%w and its inhabitants.",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_MIDDLE)
    );

    //Get Item Text
    MakeBlueMsgEng(0x810, "You received an Icon of Magic Power!&This will increase your magic capacity.&It looks awfully like an ordinary&magic jar though.");
    MakeBlueMsgEng(0x811, "You received some Horse Food!&This will add a boost&to Epona's stamina.&Carrots are delicious!");
    MakeBlueMsgEng(0x812, "You received a Heart Barrier! This&will protect one of your hearts,&if you find someone with powerful&enough magic to activate it.");
    MakeBlueMsgEng(0x813, "You received a King's Wallet!&This can carry enough money&for a king! I don't even&know how much it can carry!");
    MakeBlueMsgEng(0x814, "\x13\xA0You received the Landmine! Place&it down. Then when an enemy&approaches...KAPAW...BOOM!");
    MakeBlueMsgEng(0x815, "\x13\xA1You received Nayru's Affection!&Use it to restore 2 hearts&each time you cast it!");
    MakeBlueMsgEng(0x816, "\x13\xA2You received Din's Crucible!&Use it to forge a movable stone block!");
    MakeBlueMsgEng(0x817, "\x13\xA3You received a ring! On the&%yEquipment Subscreen%w select&the slot the ring is in with&%b\x9F%w to %requip%w or %runequip%w it.^\x13\xA3While in a %rsafe place%w, like&%byour house%w, use %y\xA7%w or %y\xA8%w while&the cursor is on a ring slot to &%ccycle between the rings you own%w.^\x13\xA3You may find and own many more&ring types than you can carry in&your slots at one time, so feel&free to swap them out!");

    MakeBlueMsgEng(0x7,  "\x13\x48You got a&%rDeku Seed Bullet Bag%w.&This bag can hold up to %y"+std::to_string(CAPACITY(UPG_BULLET_BAG,2))+"%w&slingshot bullets.");
    MakeBlueMsgEng(0x6C, "\x13\x49Your %rDeku Seed Bullet Bag%w&has become bigger!&Now you can carry more bullets!&This bag can hold %y"+std::to_string(CAPACITY(UPG_BULLET_BAG,3))+"%w bullets!");
    auto& str37 = "\x13\x00You got a %rDeku Stick%w!&On the %ySelect Item Subscreen%w,&you can set it to %y\xA7%w, %y\xA6%w or %y\xA8%w.^\x13\x00Set it to %y\xA1%w and swing it&with %y\xA1%w!^\x13\x00When you want to put it away,&stand still and press %b\x9F%w.&You can carry up to ";
    std::string s37(std::begin(str37), std::end(str37)-1);
    MakeBlueMsgEng(0x37,  s37+std::to_string(CAPACITY(UPG_STICKS,1))+" sticks,&but don't waste them.");
    auto& str90 = "\x13\x00Now you can pick up&many %rDeku Sticks%w!&You can carry up to %y";
    std::string s90(std::begin(str90), std::end(str90)-1);
    MakeBlueMsgEng(0x90, s90+std::to_string(CAPACITY(UPG_STICKS,2))+"%w of them!");
    auto& str91 = "\x13\x00Now you can pick up&even more %rDeku Sticks%w!&You can carry up to %y";
    std::string s91(std::begin(str91), std::end(str91)-1);
    MakeBlueMsgEng(0x91, s91+std::to_string(CAPACITY(UPG_STICKS,3))+"%w of them!");
    MakeBlueMsgEng(0xA7, "\x13\x01Now you can carry&many %rDeku Nuts%w!&You can hold up to %y"+std::to_string(CAPACITY(UPG_NUTS,2))+"%w nuts!");
    MakeBlueMsgEng(0xA8, "\x13\x01Now you can carry even&more %rDeku Nuts%w!You can carry&up to %y"+std::to_string(CAPACITY(UPG_NUTS,3))+"%w nuts!");
    MakeBlueMsgEng(0x56, "\x13\x4BYou upgraded your quiver to a&%rBig Quiver%w!&Now you can carry more arrows-&%y"+std::to_string(CAPACITY(UPG_QUIVER,2))+"%w in total!");
    MakeBlueMsgEng(0x57, "\x13\x4BYou upgraded your quiver to&the %rBiggest Quiver%w!&Now you can carry even more&arrows, to a maximum of %y"+std::to_string(CAPACITY(UPG_QUIVER,3))+"%w!");
    MakeBlueMsgEng(0x59, "\x13\x4EYou got a %rBig Bomb Bag%w!&Now you can carry more&Bombs, up to a maximum of %y"+std::to_string(CAPACITY(UPG_BOMB_BAG,2))+"%w!");
    MakeBlueMsgEng(0x5A, "\x13\x4FYou got the&%rBiggest Bomb Bag%w!&Now, you can carry up to&%y"+std::to_string(CAPACITY(UPG_BOMB_BAG,3))+"%w Bombs!");
    MakeBlueMsgEng(0x5E, "\x13\x56You got an Adult's Wallet!&Adults are allowed to carry a&lot of money. Now you can hold&up to %y"+std::to_string(CAPACITY(UPG_WALLET,1))+" Rupees%w.");
    MakeBlueMsgEng(0x5F, "\x13\x57You got a Giant's Wallet!&What a huge wallet!&Now you can carry up to %y"+std::to_string(CAPACITY(UPG_WALLET,2))+" Rupees%w.");

    u16 EnemyMsg = NAVI_ENEMY_CUSTOM_ID;
    //Navi Enemy Text
    MakeBlueMsgEng(0x0601, "%wGohma&%cIt's one of the parasitic monsters&inside the Deku Tree! Watch the %weye%c&carefully.^%cDifferent colors may show when it&is vulnerable to different attack types!");
    MakeBlueMsgEng(0x060C, "%wKing Dodongo&%cThis is a huge Dodongo that loves&eating %wfresh%c bomb flowers! Give it a&jolt, and finish it with your %wsword%c!");
    MakeBlueMsgEng(0x060F, "%wLizalfos&%cThese sword weilding enemies may be&able to change the angle they attack&from if you hold up your shield.^%cBut a sneaky attack to your side&can still be blocked if you&%wstop%c holding %w\xA3%c while targeting!");
    MakeBlueMsgEng(0x0610, "%wDinolfos&%cThese sword weilding enemies may be&able to change the angle they attack&from if you hold up your shield.^%cBut a sneaky attack to your side&can still be blocked if you&%wstop%c holding %w\xA3%c while targeting!");
    MakeBlueMsgEng(0x061B, "%wStalfos&%cLure it close to you and watch its&movement carefully! Attack it when&it drops its guard!^%cIt seems to be vulnerable to&%rdifferent sword techniques%c depending&on what it's doing...^%cWatch the %wcolored phantasms&%cthat appear when it attacks...");
    MakeBlueMsgEng(0x0622, "%wFlare Dancer&%cExtinguish its flaming clothes&first... though it seems to&keep them intact by spinning...");
    MakeBlueMsgEng(0x0628, "%wSpike&%cIt seems just like a stone&when it retracts its spikes...");
    MakeBlueMsgEng(0x062F, "%wDead Hand&%c\x0F, just try to stay calm...");
    MakeBlueMsgEng(0x0635, "%wIron Knuckle&%cBeware of the incredibly strong axe!&You may need to use different dodges&depending on how they attack!");
    MakeBlueMsgEng(EnemyMsg+0, "%wIron Knuckle&%cBeware of the incredibly strong axe!&You may need to use different dodges&depending on how they attack!^%cYou're an adult now, so if you're&%w\xA4 Targeting%c, hold %w\xA3%c in order to let&high swings pass over your head.");
    MakeBlueMsgEng(EnemyMsg+1, "%wOctospike&%cThese ones spit out something&interesting!");

    //Navi Quest Text
    MakeBlueMsgEng(0x015D, "%cRemember what Sheik said about&the temples of the %wSages%c.^%cOne in a %gdeep forest%c...&One on a %rhigh mountain%c...&One under a %bvast lake%c...^%cOne within the %phouse of the dead%c...&One inside a %ygoddess of the sand%c...");
    MakeBlueMsgEng(0x015E, "%cIf we're going to respect the&wishes of the Great Deku Tree&we need to start on a journey&beyond the forest.^%cIt might be tough, but you&can ask me for advice on the&%wQuest Status Subscreen%c with %y\xA5%c&and I'll try to help you.");
    MakeBlueMsgEng(0x0109, "%cIf we're really going to confront&Ganondorf we may need all&the preparation we can get.^%cIf there's any equipment we need&but don't have, we can still exit&the castle to search for it.^%cBe prepared to use things you may&not have needed to before.");
    MakeBlueMsgEng(0x010A, "%cI can hardly believe what happened.&Has the world really been ruled&by Ganondorf for seven years?");
    MakeBlueMsgEng(0x010B, "%cIf we're going to really take this&journey seriously, you may want&to ask the %wKnow-It-All Brothers%c&for more advice on things.^%cThere's a lot you don't know&and if I brush up on my&trivia I may get to show&off more of it, heh heh.");

    //Great Fairy Text
    MakeNormalMsgEng(0xE8, "Your magic power has been&enhanced! Now you can gain even&more Magic Capacity!^Collecting %gIcons of Magic%w&will allow for greater enhancement!");
    MakeNormalMsgEng(0xE9, "Your defensive power has been&enhanced! Damage inflicted by &enemies will be reduced by half.^But this only applies to&hearts protected by any&%rHeart Barriers%w you have collected.");

    //Sheik Text
    MakeNormalMsgEng(0x700F, "To find and save the sages,&you need another skill...&Head to Kakariko Village!^Then you may be able to&protect those you care about.");
    MakeNormalMsgEng(0x7010, "It looks like you have the&skills you need...^To find the people who are to&awaken as Sages, seek out those&in whom you have seen deep virtue.^Once a new Sage has been found&and awoken, you may return your&sword here and become a child&again, for a time...^But until a young sage is here&to restore this power, you must&carry that sword as your burden.");
    MakeNormalMsgEng(0x7074, "You destroyed the wicked&creatures that haunted the temple&and awakened the Sage...^But there are still other Sages&who need your help.^In order to awaken all the other&Sages, you must become even&more powerful.^You must travel through&forests...^over mountains...^under water...^and even through time...");
    MakeNormalMsgEng(0x707A, "However, so long as the evil power&in each temple remains, the Sages&cannot hear their awakening call&from the Sacred Realm...^Unfortunately, equipped as you&currently are, you cannot yet&enter any temple...^But, if you believe what I'm saying,&you should head to %rKakariko&Village%w...^Do you understand,&\x0F?");

    //Ganondorf Text
    char pName[9];
    pName[8] = 0;
    int iiC = 7;
    for (; iiC >= 0; iiC--) {
      if (gSaveContext.playerName[iiC] == 62) {
        pName[iiC] = 0;
      } else {
        break;
      }
    }

    for (; iiC >= 0; iiC--) {
      char temp = gSaveContext.playerName[iiC];

      if (temp == 62)
          temp = 32;
      else if (10 <= temp && temp <= 36)
        temp += 55;
      else if (36 <= temp && temp <= 61)
        temp += 29;
      else if (temp == 63)
        temp = 45;
      else if (temp == 64)
        temp = 46;

      pName[iiC] = temp;
    }

    std::string angryName(pName);
    static std::string angryTextEng;
    static std::string angryTextGer;
    static std::string angryTextFra;
    angryTextEng = "\x1A""\x14\x03""CURSE YOU..."+angryName+"!""\x0E""\x1E";
    angryTextGer = "\x1A""\x14\x03""ICH...&VERFLUCHE...&DICH... "+angryName+"!""\x0E""\x1E";
    angryTextFra = "\x1A""\x14\x03""JE TE MAUDIS..."+angryName+"!""\x0E""\x1E";

    MakeNormalMsgEng(0x70DC, angryTextEng);
    CustomMessageManager::Instance->ReplaceMessage(
        questMessageTableID, 0x70DC,
        CustomMessage(
          angryTextEng,
          angryTextGer,
          angryTextFra,
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM));

    u16 StoneMsg = TextIDAllocator::Instance->allocateRange("stone", 64);

    MakeBlueMsgEng(0x405, "They say that one part of&the secret directions&to a lost place is:^West&East&South&West");//Medigoron Crater Text
    MakeBlueMsgEng(0x408, "They say that one part of&the secret directions&to a lost place is:^East&North&North&East");//Ruto
    MakeBlueMsgEng(0x408, "They say that Princess Ruto's emotions&are at their highest on the&day before the full moon.");//Chicken Lady
    MakeBlueMsgEng(0x409, "Some think that the Gerudo Mask,&owned by a certian salesman,&might be useful to gain&entry to the Gerudo Fortress.^But the naive and obvious&way of using it will&likely not amount to much.");//Hold Targeting
    MakeBlueMsgEng(0x40D, "They say that the Gerudo&currently keep something special&above their highest prison cell.");//Running Man
    MakeBlueMsgEng(0x411, "They say that one part of&the secret directions&to a lost place is:^South&North&West&West");//Gerudo Valley
    MakeBlueMsgEng(0x41A, "They say that the reward&for completing the Gerudo&Training Grounds has many uses.^And experimenting, in order to&discover what kinds of things it&works on, could pay off well.");//Gerudo Colossus
    MakeBlueMsgEng(0x420, "They say that Dampe, the gravekeeper,&has a reward reserved for those&with very impresive speed.^However, he unlikely&to relinquish anything until&he gives up the ghost.");//Deku Tree
    MakeBlueMsgEng(StoneMsg+0,"There may be something special&behind falls that are %rnot%w water...");
    MakeBlueMsgEng(StoneMsg+2,"They say that many people&would like Hyrule Field to&be cleared of Peahats.");
    MakeBlueMsgEng(StoneMsg+8, "They say that dungeons may&have hidden areas that are&not even marked on any map.");
    MakeBlueMsgEng(StoneMsg+9,"They say that a level of&tension resides between the&various races in Hyrule.^The use of masks may be&the key to allow one to&better understand this.");
    MakeBlueMsgEng(StoneMsg+12, "They say that certain items, when&they are used together, may be&able to combine their abilities.^So it may pay to experiemnt&with methods of pairing things!");
    MakeBlueMsgEng(StoneMsg+20, "They say that certain shops&may be doing business&at hours that they&advertise as being closed.");
    MakeBlueMsgEng(StoneMsg+23, "They say there is a secret&that most cannot see,&in a place where&fairies no longer live.");
   // (*textIDSubstitutionTable)[StoneMsg+8] = 0x413;//Previous dungeons
    (*textIDSubstitutionTable)[StoneMsg+26] = 0x405;
    (*textIDSubstitutionTable)[0x041F] = 0x420;
    //MakeBlueMsgEng(StoneMsg+26, );//Mountain Crater Grotto
    //"They say that the Gerudo Mask,&owned by a certian salesman,&could be useful to enter the gate.^But not necessarily in the way&you might think."
    //"They say that Dampe,&the gravekeeper, has a reward reserved&for those with very impresive speed."
    //"They say that the Gerudo&currently keep something special above&their highest prison cell."
    //"They say that certain shops&may be doing business&at hours that they&advertise as being closed."

    //Deku Scrubs
    MakeNormalMsgEng(0x10E0,"\x12\x38\x82""All right! You win! In return,&I'll tell you a secret&if you pay me %r50 rupees%w!\x07\x10\xA3");
    MakeNormalMsgEng(0x10E1,"\x12\x38\x80""OK, there is path to a place&hidden in the woods that is&revealed in %bthree%w parts,&of %bfour%w directions each!^The order of the parts is&something you will have&to work out yourself!^But I can say that there&is a final hidden direction,&and that is %gSouth%w!\x0B\x02");

    u16 SariaMsg = TextIDAllocator::Instance->allocateRange("saria", 50);
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg,
        CustomMessage(
          "It's nice that you are dropping&by the village again.^I've heard the twins have been&arranging something special!\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+1,
        CustomMessage(
          "\x0F, you've been collecting&a lot of things in your place,&haven't you?^Maybe I should come&around sometime.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+2,
        CustomMessage(
          "The shopkeeper doesn't do a very&good job of hiding everything in&the store. Tee hee!\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+3,
        CustomMessage(
          "I shouldn't be admitting this to you,&but even I sometimes get the twins&confused with the other girls.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+4,
        CustomMessage(
          "You probably shouldn't let Mido&know I'm talking to you like this...\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+5,
        CustomMessage(
          "Make yourself at home.&I don't mind if other people&clean things up a bit for me.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+6,
        CustomMessage(
          "The guys here know a lot of things.&But surely, there has to be so much&more outside the forest to discover.^\x0F, I want you you to teach&me about the world. Talk to me&about any places you discover.^I'm just so curious.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+7,
        CustomMessage(
          "I can hear so many people!&You're actually in the&castle town market,&aren't you?^It's just one of those&places I've heard the birds&chattering about sometimes.^Few of them can stand such a&noisy place, but I hear they&can find nice food there!\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+8,
        CustomMessage(
          "Hmm... I never would have known&there was a place with&so much water.&It must look amazing!^Come to think of it I do&wonder what it would be&like to be a creature that&could live under water.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+9,
        CustomMessage(
          "What a strange place...&So, outside the forest,&those that die still remain,^and so they actually put&them under the ground&to hide them away?\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+10,
        CustomMessage(
          "So wow, you are in the place&where the water people live!&I think some of the Kokiri^have actually seen them before,&coming out from that deep&pool in the forest.^Have you met their princess?&I'd love to meet her myself.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+11,
        CustomMessage(
          "So, you're at the royal family's&castle, where you snuck&in before right?^Tee hee, if you're feeling a&bit naughty, maybe you could find&a way to get right inside, and&discover what's really going on.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+12,
        CustomMessage(
          "So there are places like this&that keep lots of big&creatures around, huh.^That sounds slightly scary.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+13,
        CustomMessage(
          "\x0F, what are all those creatures&making those curious sounds?^There are those ones that make&that loud MUUUOAH sound.&And then, there's something else...^I don't even know if you can&hear it, but it's there,^making this incredibly strange&high pitched noise.^What is that?\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+14,
        CustomMessage(
          "\x0F, what are all those creatures&making those curious sounds?^There are those ones that make&that loud MUUUOAH sound.&There are also those&ones that sometimes huff^and scratch and go NIIIHEHE!&And then, there's something else...^I don't even know if you can&hear it, but it's there,^making this incredibly strange&high pitched noise.^What is that?\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );

    //Saria's 'regular' speach starts here
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+15,
        CustomMessage(
          "Oh, hey \x0F, are you surprised to&see me? I did say I wanted to&come around some time, didn't I?^If it's ok with you, I'll help&myself to some of this fruit.&If there's anything on your mind,&feel free to bring it up.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+16,
        CustomMessage(
          "What are you curious about?\x1C&%gYou&Ocarina&Mido%w",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+17,
        CustomMessage(
          "I'm doing fine, as always,&that's Saria for you.&But, I have to confess,^though I shouldn't be, that I'm&somewhat jealous of how&you can come and leave the&forest whenever you want.^I know you are burdened&by so much though, and I&wish you every bit of luck.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+18,
        CustomMessage(
          "Ohh, the ocarina I gave you&is a specialty of mine.^I'm able to make instruments&with enough power that their sound&can travel to wherever it&needs to be, even far away.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+19,
        CustomMessage(
          "Oh, Mido can be a pain sometimes.&But don't worry to much about&him if you can help it.^Having said that, you might&want to know something&surprising about him, tee hee.^Mido is actually %yterrified%w of bugs.&He absolutely %r\x14\x05hates \x14\x01%wthem!\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+20,
        CustomMessage(
          "Having said that, you might&want to know something&surprising about him, tee hee.^Mido is actually %yterrified%w of bugs.&He absolutely %r\x14\x05hates \x14\x01%wthem!",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+21,
        CustomMessage(
          "Having said that, did you hear&what happened? Mido's house is&absolutely crawling with bugs! I hear&he can hardly sleep at all!",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+22,
        CustomMessage(
          "Oh, so the party isn't&happening anymore?&What a bummer.^Well, I'd hope we could&find something else to&do instead I guess.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+23,
        CustomMessage(
          "You know, you can drop&by my house anytime.^I might be there sometimes.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+24,
        CustomMessage(
          "You know, you can drop&by my house anytime.^I'm here right now.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+25,
        CustomMessage(
          "Tell Reido I'm looking&forward to the party!\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+26,
        CustomMessage(
          "Oh, hey \x0F, I'll just be&dropping over if that's&OK with you.^Looking forward to seeing you.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+27,
        CustomMessage(
          "Nice to see you here! I'd love&to hear all about your travels,&so try playing my song&when you're out and about.^I often can't help but&wish I could come along,&but still, at least I'm thankful^that this place is nice&and confortable and I'm&protected in the forest.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+28,
        CustomMessage(
          "Hey, nice to see you!^Hmm...anyway, you'll never&guesswhat a bird told me...^I heard that you were at&the lake, and the the water&princess was there with you!^I'm so jealous tee hee!",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+29,
        CustomMessage(
          "Link, there's something&I want to talk about.^Ever since I started hearing the&change in the tone of your&ocarina, I've been noticing things about you.^You havn't been acting the same since&that time and there are times where&I feel, in my heart, that you are&no longer with us in this world.^Please, \x0F, I don't want to&feel so afraid for you,&you can tell me what's going on.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+30,
        CustomMessage(
          "Rauru has shown me much wisdom and&helped me understand what it means&to be here, serving everyone.^I don't really know how long&he's realy been here,&but I suspect somehow^that he may be as ancient&as the Great Deku Tree.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+31,
        CustomMessage(
          "I'm happy I could finaly meet&Mr. Darunia. He has such striking&strength and fierce determination,^and yet he's still every bit&as jolly and full of humor&as I expected him to be.^It's an honor to be in&his company as a sage.^Make sure you tell us how&his son is doing anyway!\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+32,
        CustomMessage(
          "I was struck with so much&surprise to see Princess Ruto&awakened as a sage.^I'd always hoped to see her&some day, so it's so exciting&for me to be aquainted with&her in person, even here.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+33,
        CustomMessage(
          "Impa has told me a lot about things.&From speaking with her, I believe&that I understand now how much^Princess Zelda has had to face&and how brave she has been&for all she's been through.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+34,
        CustomMessage(
          "I'd heard tales of a far away&desert many times long ago,&but I never expected to&meet anyone from there.^Nabooru is a proud woman now&doing her part to help Hyrule&and I'm glad to accept her as&part of the circle of sages.\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM)
    );


    u16 KokiriMsg = TextIDAllocator::Instance->allocateRange("kokiri", 50);
    /*CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg,
        CustomMessage(
          "Hey, you know, it's so strange what&happened to the Great Deku Tree.^Things haven't been growing&well in the forest lately, but&hopefully that will change.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );*/
    MakeNormalMsgEng(KokiriMsg, "Hey, you know, it's so strange what&happened to the Great Deku Tree.^Things haven't been growing&well in the forest lately, but&hopefully that will change.");
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+1,
        CustomMessage(
          "Tee hee hee, a lot of Kokiri&are good at hiding secrets.^But some aren't as&good as it as others.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+2,
        CustomMessage(
          "Hey, wouldn't it be funny, if \x14\x03someone\x14\x01&played a prank on Mido...^Saria is maybe a bit too nice to him&for his own good.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+3,
        CustomMessage(
          "You know, I wish the vegetation&around here had gotten more&water back when it began budding.^If that had happened, there's no doubt&that things would have grown&a mighty lot greater, and the&forest would be a better place for it.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+4,
        CustomMessage(
          "As it is, plants are barely&growing at all, and we're&only getting weeds.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+5,
        CustomMessage(
          "Things are growing somewhat,&but there are still places&that could be better...",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+6,
        CustomMessage(
          "I've spent years tending to&the plants in this forest,&and would you look&at what's happened.^Bountiful foliage all around,&showing what results when&you put in the effort to&make things grow right.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+7,
        CustomMessage(
          "You bet I'm proud! In fact,&since I'm feeling so generous,^I'm happy to share some of&the life the forest has in excess.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    static u16 msg8 = KokiriMsg+9;
    static std::string msg8Str = "You know, I think there are places&in the lost woods that no Kokiri&now remembers how to get to...\x07";
    msg8Str.push_back((char)((msg8>>8)&0xFF));
    msg8Str.push_back((char)((msg8)&0xFF));
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+8,
        CustomMessage(
          msg8Str,
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+9,
        CustomMessage(
          "Perhaps there are forest creatures&who do remember though...",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+10,
        CustomMessage(
          "I'll give you this, if you&promise to never let anyone know.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+11,
        CustomMessage(
          "Things are mostly growing&nicely, but I still can't help&but think that maybe just one&spot ought to have grown better...",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+12,
        CustomMessage(
          "Mido always works us so hard.&I keep having to work on patching up&the crack at the back of his house.^He constantly complains about how&it lets the weather in,&but no matter what I do,&it's never good enough!",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+13,
        CustomMessage(
          "I love that each of us&Kokiri have a unique musical&note of our own!^Well, most of us at least.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+14,
        CustomMessage(
          "It's great that fairy&magic allows us to disappear&when we want to.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+15,
        CustomMessage(
          "Sometimes I wonder how powerful&our magic really is.^Maybe there are beings&far more powerful than us,&and even the Great Deku Tree.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+16,
        CustomMessage(
          "I'm probably going to go&to collect firewood from the&lost woods later on.^But you know, lately I've been&getting to think about&how strange that is.^This village used to have&enough trees that we seldom&had to do this before.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+17,
        CustomMessage(
          "Wait, what's going on at Mido's place!&Why is nobody helping him?^I swear, I'll have to kick&somebody's pants for this!",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+18,
        CustomMessage(
          "Gosh! What happened?&Mido has quite a problem on&his hands, doesn't he?",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+19,
        CustomMessage(
          "Hahaha, wow! What happened?&Mido has quite a problem on&his hands, doesn't he?&\x14\x04Tee hee hee.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+20,
        CustomMessage(
          "So, it seems that Mido has&gotten over his little problem.&Nice!",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+21,
        CustomMessage(
          "So, it seems that Mido has&gotten over his little problem.&\x14\x10Nice!",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+22,
        CustomMessage(
          "It would be nice if we&had things that could&live in the water here.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+23,
        CustomMessage(
          "Ohh, there's one swishing around.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+24,
        CustomMessage(
          msgFishStr,
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+25,
        CustomMessage(
          "What's funny about Sodo&is that she seems to&forget things a lot.^Why is she like that I wonder?&She had a much better memory& before we started having festivals.^But there's no way we're&going to stop having those now!",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+26,
        CustomMessage(
          "What's funny about Sodo&is that she seems to&forget things a lot.^Why is she like that I wonder?",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+27,
        CustomMessage(
          "I've definitely gotten beter&at taking care of things after&getting so much practice.^I wish I had more things to&practice with though.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+28,
        CustomMessage(
          "Man, this is so hard.&The trees here are&so thick and old.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    MakeNormalMsgEng(KokiriMsg+29, "I've been chopping up&wood for years,&and after doing it for so long,^I've got to say,&there's got to be,&a better way.^Please help me if you&can, your body looks&strong enough.");

    MakeNormalMsgEng(KokiriMsg+30, "I've been chopping up&wood for years,&and after doing it for so long,^I've got to proclaim,&that things can be lame,&if they stay the same.^I've been managing ok myself,&but the more help the merrier.");

    MakeNormalMsgEng(KokiriMsg+31, "I've been chopping up&wood for years,&and after doing it for&so long, I feel so strong.^Not as strong as you would be,&but I don't need to be to&hack up fresh young wood!");

    MakeNormalMsgEng(KokiriMsg+32, "I would tell Saria that I've&canceled our anniversary party,&but I'm afraid to let&her down to her face.^I know she was really&looking forward to it.");

    MakeNormalMsgEng(KokiriMsg+33, "Hey, I know you went through&the Lost Woods, and you probably&encountered Mido there,&looking out for Saria.^Mido had always given&\x0F a hard time and&I think he somewhat regretted&that once he realised \x0F^wouldn't be returning,&and especialy now that we&know he did not cause the&Great Deku Tree to wither.^But at the same time,&there seems to be&some reason why he continued&to hold a grudge against \x0F^that he has never let go of,&and I can only speculate&on why.");
    MakeNormalMsgEng(KokiriMsg+34, "Hey, I know you went through&the Lost Woods, and you probably&encountered Mido there,&looking out for Saria.^Mido had always given&\x0F a hard time and&I think he somewhat regretted&that once he realised \x0F^wouldn't be returning,&and especialy now that we&know he did not cause the&Great Deku Tree to wither.^Mido seems to have always been&jealous of \x0F for some reason,&but now I have no doubt^that he would put&aside those old feelings,&were he to meet \x0F again.");

    MakeNormalMsgEng(KokiriMsg+35, "Hey, you look like a fella&strong enough to wack down&a whole tree!^If you were so big you&could give a big wack to the&top of one, it might&just fall over.");
    MakeNormalMsgEng(KokiriMsg+36, "Ohh, I think you might look like&one of those cave people&someone was talking about.^I think he saw one of them&in the Lost Woods once.");
    MakeNormalMsgEng(0x103E, "Do you want to know how to use&the %y\xA5 Button%w? Or the %rshield%w?&\x1B%y\xA5 %gButton&Shield%w\x02");
    MakeNormalMsgEng(0x1040, "I'll teach you how to use your&%rshield%w. Pressing \xA3 will bring&it up and let you defend&against attacks to your front.^But holding \xA3 will raise your %btension%w,&which may make you look a&little blue, and stop you attacking&until you've released \xA3 for a while.^Using \xA4 targeting, holding up your&shield will let you move around while&defending your face, but may not guard&your legs from low hits.^Without using \xA4 targeting, you will&duck and can move your shield with&the %c\xAA stick%w, which could be useful&and may block some low attacks.^Are you ready for more tips?&\x1B&%gYes please!&Not Now%w\x02");
    MakeNormalMsgEng(KokiriMsg+37, "You seem to be good at attracting&life. So this seems appropriate.");
    MakeNormalMsgEng(KokiriMsg+38, "Ok, here are some advanced&%rshield%w tips!^Holding up your shield while an&enemy leaps over you may make&them difficult to see, and this&can break your \xA4 targeting lock!^So you might want to let&go of \xA3 while an enemy is&overhead if you want to&keep targeting them!^Also, some enemies may have attacks&that hit you from the side, that&will actually get around your shield!^The secret to guarding these?&\xA4 target, but %rdon't%w hold \xA3 or sidestep,&your shield will stay at your side&and guard these tricky moves.^Just remember that if your shield&is tinged %cwhite%w it can block&%corange%w tinged attacks.^Still, be careful though, as some&enemies may be able to force you&into %btension%w with their attacks!^Good luck \x0F!");


    u16 MidoMsg = TextIDAllocator::Instance->allocateRange("mido", 30);

    MakeNormalMsgEng(MidoMsg, "Ahhhhhhhhrrrrrrgh!!!^Why are there so many bugs!?^I can't stop them from&crawling everywhere!");

    MakeNormalMsgEng(MidoMsg+1,"Oh, phew.^Hmph, I'm glad you helped&out there, although I'd&expect that from any Kokiri.");

    MakeNormalMsgEng(MidoMsg+2,"Hey!!!&What do you think you're doing?^No way are you dumping&things on my floor!^The great Mido will see to it&that you are properly punished!");

    MakeNormalMsgEng(MidoMsg+3, "Actually, today Saria isn't&at her usual spot.^I heard she came around here,&but I'm honestly not sure&where she went.");

    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MidoMsg+4,
        CustomMessage(
          "I never got the opportunity&to express my gratitude to&him for everything.^So I can only hope that&you might do so in my place.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MidoMsg+5,
        CustomMessage(
          "However, I still could never&forgive him for what he did.^\x0F, I wish you could have&redeemed yourself somehow.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MidoMsg+6,
        CustomMessage(
          "Actually, today Saria isn't&at her place.^I heard she's around here somewhere,&but I'm honestly not sure&where she went.",
          "",
          "",
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM)
    );

    MakeContinuingMsgEng(MidoMsg+7, "If you really need to know, Saria&should be in her house now.", MidoMsg+8, msgM8);

    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MidoMsg+8,
        CustomMessage(
          "What business would YOU&have there though?",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_TOP)
    );

    MakeContinuingMsgEng(MidoMsg+9, "What's happening?^Everything seems to have&returned to normal...&but what about Saria?^Saria must be coming back,&I refuse to accept anything else!", MidoMsg+10, msgM10);
    MakeNormalMsgEng(MidoMsg+10,"But at least, maybe there's&a chance you could fulfill&the promise I made.^It's about someone we knew&years ago, who left us.^I promised Saria that,&if he ever appeared again,^I would tell him she had been&waiting for him,&and that he should&await her return.^His name was \x0F.");
    MakeNormalMsgEng(MidoMsg+11,"In a way, I regret&making that promise to her.^I could never bring myself to trust&\x0F, no matter how much Saria&would swear by his innocence.^I just cannot shake the feeling&in my gut that he was&responsible for taking&away everything I held dear.^But I am a man of my word&and would never back down and&break a promise I made to Saria^So, please, if you ever&encounter him on your travels,&please tell him what&Saria said, for her sake.");
    MakeNormalMsgEng(MidoMsg+12,"\x0F, how I wish you could have&shown you were deserving&of my forgiveness.");
    MakeNormalMsgEng(MidoMsg+13,"\x0F, how that name fills me&with anger! How could I&even hope of forgiving you?");
    MakeNormalMsgEng(MidoMsg+14,"\x0F, how I wish I could have&understood you.^I don't know what he was&doing, but Saria trusted him&and he always treated us well.");
    MakeNormalMsgEng(MidoMsg+15,"But now I can no longer&even speak to Saria through&her music...^All I wanted was to&just to talk to her.");//&\x14\x03one...last...time
    MakeContinuingMsgEng(MidoMsg+16,"Saria\x14\x05...?!", MidoMsg+17, msgM17);
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MidoMsg+17,
        CustomMessage(
          "Mido, it's me.&Don't worry, I'm safe, but I can no longer&come back to the forest.",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_TOP)
    );
    MakeContinuingMsgEng(MidoMsg+18,"I'm so glad you're safe,&but why aren't you coming back?", MidoMsg+19, msgM19);
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MidoMsg+19,
        CustomMessage(
          "Because Mido, I need to&aid the person who&is saving this land.^Mido,&that is \x0F.",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_TOP)
    );
    MakeNormalMsgEng(MidoMsg+20, "...^Thank you...for everything.");


    u16 GoronMsg = TextIDAllocator::Instance->allocateRange("goron", 40);

    MakeNormalMsgEng(0x3038, "I know it's around here somewhere&we just need to find it!&I'm sure I heard Dad say it&involved pulling something...");
    MakeNormalMsgEng(GoronMsg+0, "Ahhh, this is the best&spot to take a nice bath.&So warm and relaxing...");
    MakeNormalMsgEng(GoronMsg+1, "Oh, this stone at least&is warm enough to rest on.&So I'll just keep napping...");
    MakeNormalMsgEng(GoronMsg+2, "Ohh, how did I stuff that up?");
    MakeNormalMsgEng(GoronMsg+3, "Argh, why do I keep losing?");
    MakeNormalMsgEng(GoronMsg+4, "It feels good to be fast!");
    MakeNormalMsgEng(GoronMsg+5, "It feels bad to not&be fast enough!");
    MakeNormalMsgEng(GoronMsg+6, "Finaly, victory is mine!&I feel so proud of myself.&I really do.\x0B\x02");
    MakeNormalMsgEng(GoronMsg+7, "I am just so overflowing&with energy!^You must take some!\x0B\x02");
    MakeNormalMsgEng(GoronMsg+8, "Ah, I like it here.&It's so peaceful and quiet.^You know, I think it's&about time I went shopping.^I can't even remember the last&time I went, but now might be as^good as any, don't you think?&\x1B&%gYes&No%w\x02");
    MakeNormalMsgEng(GoronMsg+9, "Ok, shopping it is,&I'll try to get back soon.&At least in a couple of days.");
    MakeNormalMsgEng(GoronMsg+10, "Oh well, I'll just go&and take my bath this&evening as usual then.");
    MakeNormalMsgEng(GoronMsg+11, "Oh, our brother just went out&to get supplies.^I'm not sure from where though.");
    MakeNormalMsgEng(GoronMsg+12, "Hmm...that's strange, I just&saw brother traveling&out for trade.^But I thought I already saw&him leave earlier.");
    MakeNormalMsgEng(GoronMsg+13, "Hmmm...the market doesn't&seem to be doing too well today.^I can't buy or sell&much here now.^I'll have to come back&some other time.");
    MakeNormalMsgEng(GoronMsg+14, "Ohh, the market is so crowded.&I hardly get a chance to buy anything.");
    MakeNormalMsgEng(GoronMsg+15, "Things are quiet now.&I'm surprised nobody seems to&be letting me trade at&this time of day anymore.^Why could that be?");
    MakeNormalMsgEng(GoronMsg+16, "Wait, how did you get in&here at this time?");
    MakeNormalMsgEng(GoronMsg+17, "Some brothers are off racing&outside. It's quite a spectacle&if you want to see it.^The races are usually quite close.");
    MakeNormalMsgEng(GoronMsg+18, "Look at our bigger brother's&refined rolling.^He's matured quite a bit&since his wilder days.");
    MakeNormalMsgEng(GoronMsg+19, "I still miss the shiny&red stone that sat here.&Oh how it glowed.");
    MakeNormalMsgEng(GoronMsg+20, "Sorry! Let me express my feelings&with more refined rolling!");
    MakeNormalMsgEng(GoronMsg+21, "Our bigger brother isn't here now.&I wonder where he went.");
    MakeNormalMsgEng(GoronMsg+22, "Our bigger brother is both courteous&and tough. He tries to avoid&hitting anyone if he can help it,^and is barely phased by the&explosions of our special crop anymore.^I hear he still dislikes being&hit by them though.");
    MakeNormalMsgEng(GoronMsg+23, "Oh, I give up,&I might as well ask you&to pawn this off for me.^I'll take %g200 rupees%w for it.&\x1B&%gYes&No%w\x02");
    MakeNormalMsgEng(GoronMsg+24, "Ah, I like it here.&It's so peaceful and quiet.^You know, I think it's&about time I went shopping again.^I remember the last time I went,&and hope it goes better this time.^Should I try again?&\x1B&%gYes&No%w\x02");
    MakeNormalMsgEng(GoronMsg+25, "Oh, I can't believe it.^There was someone else there&rolling around my usual spot.^Reminded me of myself&when I was younger too.^I don't understand,&but the least I can do&is stay out here instead^to watch out for&anyone who might try to&imprison our tribe again.");
    MakeNormalMsgEng(GoronMsg+26, "Aww, too bad.");
    MakeNormalMsgEng(GoronMsg+27, "What, you don't have enough either?");
    MakeNormalMsgEng(GoronMsg+28, "We have a deal then.\x0B\x02");
    MakeNormalMsgEng(GoronMsg+29, "No, oh no!^You will never take me,&or my brothers again!&I will make sure of it!");
    MakeNormalMsgEng(GoronMsg+30, "I'm not that good at sneaking&into tight spaces! But if&you are here to sabotage&the Gerudo, then great!^Please take this for good luck!\x0B\x02");
    MakeNormalMsgEng(GoronMsg+31, "You think you're going&to capture me again?^NEVER!");
    MakeNormalMsgEng(GoronMsg+32, "I'll be standing by if you need me.&\x1B&%gDo that&You are free to leave%w\x02");
    MakeNormalMsgEng(GoronMsg+33, "Alright.");
    MakeNormalMsgEng(GoronMsg+34, "Ok, see ya back&at the mountain.");

    u16 HylianMsg = TextIDAllocator::Instance->allocateRange("hylian", 30);

    MakeNormalMsgEng(HylianMsg+0, "Oh, so I guess you're here&for the night trade then.^We normally don't let&kids get involved, but&you seem savy enough.");
    MakeNormalMsgEng(HylianMsg+1, "Here's our special night offers.\x0B\x02");
    MakeNormalMsgEng(HylianMsg+2, "I don't typically take&guests during the day.^That's why I lock my door,&as you'd know.^If you have business here,&please let that be known&ahead of time.");
    MakeNormalMsgEng(HylianMsg+3, "You know, I heard there's some&shady business going on in&some of the shops around here&at night.^Apparently there are secret deals&being done on luxury items that&aren't being offered to the public.");
    MakeNormalMsgEng(HylianMsg+4, "And how would this guy know&about what he's telling me?^He thinks he's got the&dirt on everything.");
    MakeNormalMsgEng(HylianMsg+5, "You know, I heard that the&folks at Lon Lon Ranch are&having trouble getting enough milk.^Apparently, they don't have&enough cows themselves to&provide enough supply.");
    MakeNormalMsgEng(HylianMsg+6, "I would help them, but&I can't make any milk myself!");
    MakeNormalMsgEng(HylianMsg+7, "Now there, surely there's something&they could each be used for, otherwise&why would people borrow&them in the first place?");
    MakeNormalMsgEng(HylianMsg+8, "Useless, they're useless I tell you.^There's no way even half&the masks that the mask&shop offers have any use at all.^They're just children's toys!");
    MakeNormalMsgEng(HylianMsg+9, "I'm not saying that&there MUST BE one.^I'm mearly pointing out that we&cannot rule out the possible existence&of an invisible talking cucco.^There could be one somewhere&for all we know.");
    MakeNormalMsgEng(HylianMsg+10, "I can't believe&what this guy is saying.^He believes that there is&an invisible, talking cucco!&Absurd.");
    MakeNormalMsgEng(HylianMsg+11, "You know, I heard that the&girl with the red hair from&the ranch is a terrible sleeper.");
    MakeNormalMsgEng(HylianMsg+12, "I also heard.^When she stayed at the inn,&she kept me up all&night with her singing!");
    MakeNormalMsgEng(HylianMsg+13, "You know, I'm not sure if&anyone has ever won the&Super Special Treasure Chest Game.&How do we know it's not rigged?");
    MakeNormalMsgEng(HylianMsg+14, "Surely I've never heard of&anyone winning. Shouldn't some&inspector make sure nothing&funny is going on?");
    MakeNormalMsgEng(HylianMsg+15, "Well, I do actually want to do&something about this problem, but&I'm not sure I can make the&journey to get the medicine I need.^It's embarassing to ask, but&my husband's busy, so I wonder&if anyone will escort me.");
    MakeNormalMsgEng(HylianMsg+16, "You've been an especially&good supplier this week.&I'm satisfied enough to&give you this bonus!");
    MakeNormalMsgEng(HylianMsg+17, "Traditonally, people here in this&market believed that the best day&to start business with someone&was when the moon is most %rred%w.^Now, I'm not the most superstitious&person but I know it pays to&understand a people's customs.");
    MakeNormalMsgEng(HylianMsg+18, "Here's a tip for doing business here.&If a certain kind of item&is associated with a particular&%cday of the week%w,^it may be valued more&by people on that day.&It's a useful thing to know!");
    MakeNormalMsgEng(HylianMsg+19, "Well I guess I'll be&setting off.^It's hard to walk like&this, but I'm hoping to&make it to Kakariko.^It would be nice to&get there this time.");
    MakeNormalMsgEng(HylianMsg+20, "I'm glad I could make it here.&It's such a good thing you&were there to help too.^Oh, well I wanted to get&my ointments here, but the shop I'd&normally get them from doesn't seem&to be opening up at all.");
    MakeNormalMsgEng(HylianMsg+21, "I don't know what to do.&It's a good thing I have&a friend who lives at this house.^So at least I should be able&to stay here in the meanwhile.^I have been wondering what&her son has been getting up to.^Anyway, I probably won't be back&in town until there's a wagon&train arranged to go back.^And that won't be until&next week, I'm afraid.");
    MakeNormalMsgEng(HylianMsg+22, "The woman in the yellow dress&left here a while back.^I hope things go well for her.");
    MakeNormalMsgEng(HylianMsg+23, "It's gotten too late,&I'd better head back.");
    MakeNormalMsgEng(HylianMsg+24, "Oh, I can't go&on like this.");
    MakeNormalMsgEng(0x70F0, "Oh, that's %cBlue Fire%w!&I'll buy it for %r"+std::to_string(BEGGAR_REWARD_FIRE)+" Rupees%w!&All sales final, OK?");
    MakeNormalMsgEng(0x70F1, "Oh, a %pFish%w!&I'll buy it for %r"+std::to_string(BEGGAR_REWARD_FISH)+" Rupees%w!&All sales final, OK?");
    MakeNormalMsgEng(0x70F2, "Oh, it's a cute little %gBug%w!&I'll buy it for %r"+std::to_string(BEGGAR_REWARD_BUG)+" Rupee%w!&All sales final, OK?");
    MakeNormalMsgEng(0x70F3, "Oh, a darling little %cFairy%w!&I'll buy it for %r"+std::to_string(BEGGAR_REWARD_FAIRY)+" Rupees%w!&All sales final, OK?");
    MakeNormalMsgEng(0xF000, "\x08%rPiece of Heart   300 Rupees%w&Adds a quarter of a section&to your heart container.&4 together will improve your life!\t\n\x02");
    MakeNormalMsgEng(0xF001, "\x08Piece of Heart   300 Rupees&\x09\x1B&%gBuy&Don't buy%w\x02");
    u16 RanchMsg = TextIDAllocator::Instance->allocateRange("ranch", 20);

    MakeNormalMsgEng(RanchMsg+0, "Oh, I really can't get to sleep.^Hey, fairy boy, when did&you get in here anyway?&You've been sneaking about,&haven't you?^Hmm...we could go out and play,&but you'd have to be careful&to not wake dad.^I'll see you outside.");
    MakeNormalMsgEng(RanchMsg+1, "Don't worry.^I should be able to get&out through the window.");
    MakeNormalMsgEng(RanchMsg+2, "Well, I did want to play,&but there are all these&mean birds flying around.^There's no way I can go&out there like this now.");
    MakeNormalMsgEng(RanchMsg+3, "Oh, you're getting them.&They still don't seem&to be going away though.");
    MakeNormalMsgEng(RanchMsg+4, "Oh, you're pretty good at this.&Those birds just keep comming,&but I guess I feel safer&knowing you're here.^Maybe we should call it a night.");
    MakeNormalMsgEng(RanchMsg+5, "Oh, I guess you're here to defend&us against those pesky birds.^I'll be waiting then.");
    MakeNormalMsgEng(RanchMsg+6, "\x08Hey! Let's set off.&Try following me.\016<\x02");
    MakeNormalMsgEng(RanchMsg+7, "\x08Oh, I see Epona isn't&with you yet!\016<\x02");
    MakeNormalMsgEng(RanchMsg+8, "You seem to really have a knack&for understanding horses.^Hey, fairy boy, remember the times&when we used to stay up all&night and play around the ranch?^It barely feels that long ago,&doesn't it?^Say, why don't you come&riding with me sometime?^You up for it?&\x1B&%gYes&Sorry, I'm busy.%w\x02");
    MakeNormalMsgEng(RanchMsg+9, "Great, I'll see you tomorrow,&just outside the ranch.&Make sure to take Epona.");
    MakeNormalMsgEng(RanchMsg+10, "Oh, alright.^I guess maybe we could&try for next week then.");
    MakeNormalMsgEng(RanchMsg+11, "\x08Oops. Missed that one.\016<\x02");
    MakeNormalMsgEng(RanchMsg+12, "I really enjoyed that.");
    MakeNormalMsgEng(RanchMsg+13, "It would be nice if Epona&had a special treat&once in a while.^She really deserves it,&don't you think.");

    u16 ZoraMsg = TextIDAllocator::Instance->allocateRange("zora", 30);

    MakeNormalMsgEng(0x402D, "Hey you know, ever since Ruto&returned, all she ever talks about&is the new boy she likes.^It's a bit worrying, honestly.");
    MakeNormalMsgEng(ZoraMsg+0, "Wow! I never suspected that&anyone who was not a&Zora could dive like that!^You cannot refuse this token&of my awe!\x0B\x02");
    MakeNormalMsgEng(ZoraMsg+1, "Oh, you do know a boy was&seen hanging around this domain&on the day Ruto snuck away again.^Surely you understand why people&are...suspecting things?");
    MakeNormalMsgEng(ZoraMsg+2, "Oh, you do know a boy was&seen hanging around the&shores of Lake Hylia&on the day Ruto snuck away again.^Surely you understand why people&are...suspecting things?");
    MakeNormalMsgEng(ZoraMsg+3, "I heard Ruto was treated very&roughly the last time she went out.^I would hope our princess would&deserve far more dignified&treatment from anyone.");
    MakeNormalMsgEng(ZoraMsg+4, "I heard Ruto had some scary&encounters on that night.&I hope that doens't happen to our princess again.");
    MakeNormalMsgEng(ZoraMsg+5, "I heard Ruto was made quite&unconfortable on her night away.&I would hope anyone in her company&would treat her with good courtesy.");
    MakeNormalMsgEng(ZoraMsg+6, "Apparently, Ruto's boyfriend&just abandoned her,&leaving her completely alone!^How unthinkable!");
    MakeNormalMsgEng(ZoraMsg+7, "Apparently, whoever was&supposed to meet Ruto&just never showed up at all!^How unbelievable!");
    MakeNormalMsgEng(ZoraMsg+8, "There was apparently something&unexpected that happend&which bothered Ruto the&last time she went out.^I can't say I'm aware of what&that was in particular though.");
    MakeNormalMsgEng(ZoraMsg+9, "You know what I heard.&Apparently, Ruto's date&showed up far ahead of time.^Almost like he was excessively&desperate for her.");
    MakeNormalMsgEng(ZoraMsg+10, "You know what I heard.&Apparently, Ruto's date&showed up incredibly late.^Like he had no regard&for her whatsoever!");
    MakeNormalMsgEng(ZoraMsg+11, "I heard the boy Ruto met&was acting very strange&when she showed up.^Like he was just hanging around&at first, silently watching her.^Creepy.");
    MakeNormalMsgEng(ZoraMsg+12, "Those who cannot give their&partner a propper seeing&off deserve no respect.");
    MakeNormalMsgEng(ZoraMsg+13, "Oh, so there is a Zora girl here,&none other than the&Royal Princess Ruto.^Glad to see you're keeping her safe...");
    MakeNormalMsgEng(ZoraMsg+14, "Those who do not think about&their partner's wishes&deserve no respect.");
    MakeNormalMsgEng(ZoraMsg+15, "Those who do not think about&their partner at all&deserve absolutely no respect.");
    MakeNormalMsgEng(ZoraMsg+16, "Well...she is very pretty&isn't she?^You wouldn't have&any pictures of her,&now would you?^I'd promise to not show&them to anyone else.");
    MakeNormalMsgEng(ZoraMsg+17, "I heard the boy Ruto met&was acting very strange&when she showed up.^Like he wouldn't even talk&to her even after she'd&greeted him.^Creepy.");
    MakeNormalMsgEng(ZoraMsg+18, "Ruto sure is a strange&and wild girl.^And some days she's even stranger&and wilder than usual.^It's unfitting for a princess,&but I guess girls her&age will be that way.");
    MakeNormalMsgEng(ZoraMsg+19, "The water that comes directly from&Zora's Fountain is incredibly fresh.^As long as there's nothing tainting it,&it should be the purest water&in all the land.");
    MakeNormalMsgEng(ZoraMsg+20, "Apparently, we Zoras have an&affinity for the Moon unique&to all the races in Hyrule.^I can't say I feel much for it&myself though, even if others might.");
    MakeNormalMsgEng(ZoraMsg+21, "Ohhh, I'm revived...&Was it you who saved me?&Don't be so nervous!^I...well I wanted to give&you a %bZora tunic%w to aid you&with breathing under water.^But there seems to be nothing&I can do about that right now.&So all I can do is ask you to&listen to my troubles.");

    u16 RutoMsg = TextIDAllocator::Instance->allocateRange("ruto", 30);

    MakeNormalMsgEng(RutoMsg+0, "You finally showed up!&Be a man and carry me&out to the middle of the lake!\x0B\x02");
    MakeNormalMsgEng(RutoMsg+1, "Who do you think you are&trying to leave on me?&You better pick me up again!\x0B\x02");
    MakeNormalMsgEng(RutoMsg+2, "Pssst! Hey, I don't want my&father knowing about our promise&just yet...^But maybe we could get to&know each other a little better,&say tomorrow night...^I'll sneak out to the lake,&and we can meet at nightfall,&under the full moon.^How about that?&\x1B&%gAbsolutely&Uhhh...%w\x02");
    MakeNormalMsgEng(RutoMsg+3, "Heehee, let's do this!");
    MakeNormalMsgEng(RutoMsg+4, "Ok, maybe it's a bit too soon.");
    MakeNormalMsgEng(RutoMsg+5, "\x08Oh, hello there!\016<\x02");
    MakeNormalMsgEng(RutoMsg+6, "\x08Yes, take me across&to that island&with the big tree.\016<\x02");
    MakeNormalMsgEng(RutoMsg+7, "\x08Hello Mr. Owl!\016<\x02");
    MakeNormalMsgEng(RutoMsg+8, "\x08Ok, put me down here,&let's gaze at the sky&and watch the sun rise.\016<\x02");
    MakeNormalMsgEng(RutoMsg+9, "\x08Oh, the sun is up already,&that's disappointing.\016<\x02");
    MakeNormalMsgEng(RutoMsg+10, "\x08That was nice!\016<\x02");
    MakeNormalMsgEng(RutoMsg+11, "\x08Isn't it all so beautiful?&Come closer to me.\016<\x02");
    MakeNormalMsgEng(RutoMsg+12, "That was such a wonderful night.^With us together here,&there is only one way I can&properly show you how I feel...^But I have to be back now,&otherwise father will be angry.&Please see me again soon.");
    MakeNormalMsgEng(RutoMsg+13, "We're a bit eager, aren't we?&I see you've been waiting for me.^You can start by taking me to&that island out there then.\x0B\x02");
    MakeNormalMsgEng(RutoMsg+14, "Oh, you showed up just on time!^It's all so exciting, isn't it,&the moonlit lake,&the sky, the water.^And us.^Tonight I give you the privilage&of carrying me out to the&middle of the lake.\x0B\x02");
    MakeNormalMsgEng(RutoMsg+15, "Hiding around here, weren't you?&Come on, don't be shy,&you can carry me to the&island across that bridge.\x0B\x02");
    MakeNormalMsgEng(RutoMsg+16, "Why did you leave?&Don't hesitate now,&carry me over to that island!\x0B\x02");
    MakeNormalMsgEng(RutoMsg+17, "Well I know I said I wanted to&really show my feelings&for you...but allthe gossip&about us is so embarassing already.^I'd hope that would die down&if nobody actually noticed us.");
    MakeNormalMsgEng(RutoMsg+18, "I said I wanted to&really show my feelings&for you...and now...&I truly feel I can.^Right now, I can only express&my deepest love for you,&with this.\x0B\x02");
    MakeNormalMsgEng(RutoMsg+19, "\x08Why are you over there?&I want you near me.\016<\x02");
    MakeNormalMsgEng(RutoMsg+20, "How dare you treat me&the way you did!");
    MakeNormalMsgEng(RutoMsg+21, "You know my feelings for you,&nothing more needs to be said.");
    MakeNormalMsgEng(RutoMsg+22, "That was fun,&maybe we should try&that again sometime.");
    MakeNormalMsgEng(RutoMsg+23, "Thank you for being here.&I have to go now,&so see you later.");
    MakeNormalMsgEng(RutoMsg+24, "Why did you run away from me?");
    MakeNormalMsgEng(RutoMsg+25, "Why wouldn't you talk to me?\x0B\x02");
    MakeNormalMsgEng(RutoMsg+26, "\x08Oh, the tree is in the way,&how disappointing.\016<\x02");

    u16 CursedFamilyMsg = TextIDAllocator::Instance->allocateRange("cursedfamily", 10);
    MakeNormalMsgEng(CursedFamilyMsg+0, "Our father may be telling you&that he has no need for&any help, but please^ignore his politeness.&I may have more rewards&for you yet!");
    MakeNormalMsgEng(CursedFamilyMsg+1, "I'm happy to have given&what I can, please keep&destroying the curse!");
    MakeNormalMsgEng(CursedFamilyMsg+2, "You've done such good work!&Please accept this reward!\x0B\x02");
    MakeNormalMsgEng(CursedFamilyMsg+3, "I can tell that you possess a&noble soul. How about this challenge,&let us see how much money&you can make within the time given.^For this period, I will let&you borrow my prized wallet,&on the condition that you give&it back when the time is up.^You will also not be allowed&to spend any money out of it,&only to put it in.^Make enough money and you may&see what it's like to be rich.\x0B\x02");
    MakeNormalMsgEng(CursedFamilyMsg+4, "Are you done then?&\x1B&%gYes&No%w\x02");
    MakeNormalMsgEng(CursedFamilyMsg+5, "The most you've ever made is:&%r"+std::to_string(0)+" rupees%w.^What do you say?&\x1B&%gYes&No%w\x02");
    MakeNormalMsgEng(CursedFamilyMsg+6, "It looks like you understand&something about how to&keep yourself rich.^Here's something fitting for&a budding aristocrat.\x0B\x02");

    u16 MiscMsg = TextIDAllocator::Instance->allocateRange("misc", 20);
    MakeNormalMsgEng(MiscMsg+0, "Mooooo! I feel really great&like I'm about to produce&something amazing!&Hang on!\x0B\x02");
    MakeNormalMsgEng(MiscMsg+1, "Hey did you see the large&Goron that just zoomed past?&Anyway, I still must say.&The current time is \x1F.");
    MakeNormalMsgEng(MiscMsg+2, "WHAT!?^You can actually see&and hear me, can't you?^It is of no concern to&me how you may be able to&pull off such a feat, but alas,^all I can do is bestow&apon you a fitting enough trinket,&such that you JUST BUZZ OFF&and leave me alone.");
    MakeNormalMsgEng(MiscMsg+3, "I do believe I&told you to SCRAM.");
    MakeNormalMsgEng(MiscMsg+4, "Hey, I see you're pretty&good at this. How about&trying your hand at the&Super Special Treasure Chest game?^Your choice.\x1C&%gSuper Special Game (30 Rupees)&Regular Game (10 Rupees)&Don't play%w");
    MakeNormalMsgEng(MiscMsg+5, "Sorry. I cannot serve someone&who is already borrowing something&from someone else.\x0B\x02");
    //MakeBlueMsgEng(MiscMsg+6, "It seems your time is up!\x0B\x02");
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MiscMsg+6,
        CustomMessage(
          "It seems your time is up!\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_TOP)
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MiscMsg+7,
        CustomMessage(
          "Oh no! The %rheat%w is spoiling your item!\x0B\x02",
          "",
          "",
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_TOP)
    );
    MakeNormalMsgEng(MiscMsg+8, "I'm not sure I want you&to worry, but I've had some&concerns about the lady who's&been waiting outside the gate.^She seems to be afraid to leave.&I would help her, but my&duty is to stand guard here.");

    u16 GerudoMsg = TextIDAllocator::Instance->allocateRange("gerudo", 10);
    MakeNormalMsgEng(GerudoMsg+0, "Heh, since you're clearly a master,&the only thing left is&to try for a perfect score,&if you can!\x0B\x02");
}
