#include "OTRGlobals.h"
#include <ResourceMgr.h>
#include <Scene.h>
#include <Utils/StringHelper.h>
#include "global.h"
#include "vt.h"
#include <Text.h>
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
            uint32_t kaeporaMsgSize = file->messages[i].msg.size();
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

	return table;
}

#define MakeNormalMsgEng(txtID, text) \
    CustomMessageManager::Instance->CreateMessage(\
        questMessageTableID, (txtID) ,\
        {\
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,\
          (text) ,\
          "",\
          "",\
        }\
    );

#define MakeBlueMsgEng(txtID, text) \
    CustomMessageManager::Instance->CreateMessage(\
        questMessageTableID, (txtID) ,\
        {\
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,\
          (text) ,\
          "",\
          "",\
        }\
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
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          msgFishStr,
          "",
          "",
        }
    );
    }
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
            "Ein %rGoldenes Skulltula-Symbol%w!&Du hast nun insgesamt %r{{gsCount}}%w Golende&Skulltula-Symbole gesammelt!\x0E\x3C",
            "Vous obtenez un %rSymbole de&Skulltula d'or%w! Vous avez&collecté %r{{gsCount}}%w symboles en tout!\x0E\x3C"
        }
    );
    CustomMessageManager::Instance->CreateGetItemMessage(
        customMessageTableID, (GetItemID)TEXT_GS_FREEZE, ITEM_SKULL_TOKEN,
        { 
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "You got a %rGold Skulltula Token%w!&You've collected %r{{gsCount}}%w tokens&in total!",
          "Ein %rGoldenes Skulltula-Symbol%w!&Du hast nun insgesamt %r{{gsCount}}%w Golende&Skulltula-Symbole gesammelt!",
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
    CustomMessageManager::Instance->CreateGetItemMessage(
        customMessageTableID, (GetItemID)TEXT_HEART_CONTAINER, ITEM_HEART_CONTAINER,
        {
            TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
            "You got a %rHeart Container%w!&You've collected %r{{heartContainerCount}}%w containers&in total!",
            "Ein %rHerzcontainer%w!&Du hast nun insgesamt %r{{heartContainerCount}}%w&Herzcontainer gesammelt!",
            "Vous obtenez un %rCoeur&d'Energie%w! Vous en avez&collecté %r{{heartContainerCount}}%w en tout!"
        }
    );
    CustomMessageManager::Instance->CreateGetItemMessage(
        customMessageTableID, (GetItemID)TEXT_HEART_PIECE, ITEM_HEART_PIECE,
        {
            TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
            "You got a %rHeart Piece%w!&You've collected %r{{heartPieceCount}}%w pieces&in total!",
            "Ein %rHerzteil%w!&Du hast nun insgesamt %r{{heartPieceCount}}%w&Herteile gesammelt!",
            "Vous obtenez un %rQuart de&Coeur%w! Vous en avez collecté&%r{{heartPieceCount}}%w en tout!"
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        customMessageTableID, TEXT_MARKET_GUARD_NIGHT,
        {
            TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
            "You look bored. Wanna go out for a&walk?\x1B&%gYes&No%w",
            "Du siehst gelangweilt aus.&Willst du einen Spaziergang machen?\x1B&%gJa&Nein%w",
            "Tu as l'air de t'ennuyer. Tu veux&aller faire un tour?\x1B&%gOui&Non%w",
        }
    );

    u16 WonderMsg = 0x0200;
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, WonderMsg+0x3D,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_MIDDLE,
          "You're looking good \x0F!",
          "",
          "",
        }
    );

    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, WonderMsg+0x3E,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_MIDDLE,
          "Wow, you sure have grown!&You're still not quite&used to seeing yourself&as an adult, are you?",
          "",
          "",
        }
    );

    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, WonderMsg+0x3F,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_MIDDLE,
          "History of Hyrule's days.^In this land, each day,&with its characteristic Moon,&has a unique reason and purpose&for the people living under its sky.^The first day, with the biggest&and brightest Moon, is a&day of beginnings,&love and renewal.^The second, the day of&the golden Moon, is a day of&industriousness and seriousness of work.^The third, the day of Moonfire,&is a day for testing and&understanding oneself.",
          "",
          "",
        }
    );

    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, WonderMsg+0x40,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_MIDDLE,
          "The fourth day, the darkest day,&is a day of the markets, and also one that brings mystery.^Many are filled with wonder at&the kinds of things that&tend to be brought by this day.^The fifth, a day loved&by the multitudes, is a day of&festivity and celabration.^And finally the sixth,&the day of rest, one even&more loved by some.",
          "",
          "",
        }
    );

    u16 StoneMsg = TextIDAllocator::Instance->allocateRange("stone", 64);

    MakeBlueMsgEng(0x405, "They say that one part of&the secret directions&to a lost place is:^West&East&South&West");//Medigoron Crater Text
    MakeBlueMsgEng(0x408, "They say that one part of&the secret directions&to a lost place is:^East&North&North&East");//Ruto
    MakeBlueMsgEng(0x411, "They say that one part of&the secret directions&to a lost place is:^West&East&South&West");//Gerudo Valley
    MakeBlueMsgEng(StoneMsg+0,"There may be something special&behind falls that are not water...");
    MakeBlueMsgEng(StoneMsg+8, "They say that one part of&the secret directions&to a lost place is:^South&North&WestWest");
    //MakeBlueMsgEng(StoneMsg+26, );//Mountain Crater Grotto
    //"They say that the Gerudo Mask,&owned by a certian salesman,&could be useful to gain entry.^But not necessarily in the way&you might think.
    //"They say that Dampe,&the gravekeeper, has a reward reserved&for those with very impresive speed."

    //Deku Scrubs
    MakeNormalMsgEng(0x10E0,"\x12\x38\x82""All right! You win! In return,&I'll tell you a secret&if you pay me %r50 rupees%w!\x07\x10\xA3");
    MakeNormalMsgEng(0x10E1,"\x12\x38\x80""OK, there is path to a place&hidden in the woods that is&revealed in %bthree%w parts,&of %bfour%w directions each!^The order of the parts is&something you will have&to work out yourself!^But I can say that there&is a final hidden direction,&and that is %gSouth%w!\x0B\x02");

    u16 SariaMsg = TextIDAllocator::Instance->allocateRange("saria", 30);
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "It's nice that you are dropping&by the village again.^I've heard the twins have been&arranging something special!\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+1,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "\x0F, you've been collecting&a lot of things in your place,&haven't you?^Maybe I should come&around sometime.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+2,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "The shopkeeper doesn't do a very&good job of hiding everything in&the store. Tee hee!\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+3,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "I shouldn't be admitting this to you,&but even I sometimes get the twins&confused with the other girls.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+4,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "You probably shouldn't let Mido&know I'm talking to you like this...\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+5,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "Make yourself at home.&I don't mind if other people&clean things up a bit for me.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+6,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "The guys here know a lot of things.&But surely, there has to be so much&more outside the forest to discover.^\x0F, I want you you to teach&me about the world. Talk to me&about any places you discover.^I'm just so curious.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+7,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "I can hear so many people!&You're actually in the&castle town market,&aren't you?^It's just one of those&places I've heard the birds&chattering about sometimes.^Few of them can stand such a&noisy place, but I hear they&can find nice food there!\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+8,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "Hmm... I never would have known&there was a place with&so much water.&It must look amazing!^Come to think of it I do&wonder what it would be&like to be a creture that&could live under water.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+9,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "What a strange place...&So, outside the forest,&those that die still remain,^and so they actually put&them under the ground&to hide them away?\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+10,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "So wow, you are in the place&where the water people live!&I think some of the Kokiri^have actually seen them before,&coming out from that deep&pool in the forest.^Have you met their princess?&I'd love to meet her myself.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+11,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "So, you're at the royal family's&castle, where you snuck&in before right?^Tee hee, if you're feeling a&bit naughty, maybe you could find&a way to get right inside, and&discover what's really going on.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+12,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "So there are places like this&that keep lots of big&creatures around, huh.^That sounds slightly scary.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+13,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "\x0F, what are all those creatures&making those curious sounds?^There are those ones that make&that loud MUUUOAH sound.&And then, there's something else...^I don't even know if you can&hear it, but it's there,^making this incredibly strange&high pitched noise.^What is that?\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+14,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "\x0F, what are all those creatures&making those curious sounds?^There are those ones that make&that loud MUUUOAH sound.&There are also those&ones that sometimes huff^and scratch and go NIIIHEHE!&And then, there's something else...^I don't even know if you can&hear it, but it's there,^making this incredibly strange&high pitched noise.^What is that?\x0B\x02",
          "",
          "",
        }
    );

    //Saria's 'regular' speach starts here
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+15,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Oh, hey \x0F, are you surprised to&see me? I did say I wanted to&come around some time, didn't I?^If it's ok with you, I'll help&myself to some of this fruit.&If there's anything on your mind,&feel free to bring it up.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+16,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "What are you curious about?\x1C&%gYou&Ocarina&Mido%w",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+17,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "I'm doing fine, as always,&that's Saria for you.&But, I have to confess,^though I shouldn't be, that I'm&somewhat jealous of how&you can come and leave the&forest whenever you want.^I know you are burdened&by so much though, and I&wish you every bit of luck.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+18,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Ohh, the ocarina I gave you&is a specialty of mine.^I'm able to make something&with enough power that its sound&can travel to wherever it&needs to be, even far away.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+19,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Oh, Mido can be a pain sometimes.&But don't worry to much about&him if you can help it.^Having said that, you might&want to know something&surprising about him, tee hee.^Mido is actually %yterrified%w of bugs.&He absolutely %r\x14\x05hates \x14\x01%wthem!",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+20,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Having said that, you might&want to know something&surprising about him, tee hee.^Mido is actually %yterrified%w of bugs.&He absolutely %r\x14\x05hates \x14\x01%wthem!",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+21,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Having said that, did you hear what happened?&Mido's house is absolutely crawling with bugs!&I hear he can hardly sleep at all.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+22,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "Oh, so the party isn't&happening anymore?&What a bummer.^Well, I'd hope we could&find something else to&do instead I guess.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+23,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "You know, you can drop&by my house anytime.^I might be there sometimes.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+24,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "You know, you can drop&by my house anytime.^I'm here right now.\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+25,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "Tell Reido I'm looking&forward to the party!\x0B\x02",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, SariaMsg+26,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
          "Oh, hey \x0F, I'll just be&dropping over if that's&OK with you.^Looking forward to seeing you.\x0B\x02",
          "",
          "",
        }
    );


    u16 KokiriMsg = TextIDAllocator::Instance->allocateRange("kokiri", 40);
    /*CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Hey, you know, it's so strange what&happened to the Great Deku Tree.^Things haven't been growing&well in the forest lately, but&hopefully that will change.",
          "",
          "",
        }
    );*/
    MakeNormalMsgEng(KokiriMsg, "Hey, you know, it's so strange what&happened to the Great Deku Tree.^Things haven't been growing&well in the forest lately, but&hopefully that will change.");
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+1,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Tee hee hee, a lot of Kokiri&are good at hiding secrets.^But some aren't as&good as it as others.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+2,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Hey, wouldn't it be funny, if \x14\x03someone\x14\x01&played a prank on Mido...^Saria is maybe a bit too nice to him&for his own good.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+3,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "You know, I wish the vegetation&around here had gotten more&water back when it began budding.^If that had happened, there's no doubt&that things would have grown&a mighty lot greater, and the&forest would be a better place for it.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+4,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "As it is, plants are barely&growing at all, and we're&only getting weeds.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+5,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Things are growing somewhat,&but there are still places&that could be better...",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+6,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "I've spent years tending to&the plants in this forest,&and would you look&at what's happened.^Bountiful foliage all around,&showing what results when&you put in the effort to&make things grow right.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+7,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "You bet I'm proud! In fact,&since I'm feeling so generous,^I'm happy to share some of&the life the forest has in excess.",
          "",
          "",
        }
    );
    static u16 msg8 = KokiriMsg+9;
    static std::string msg8Str = "You know, I think there are places&in the lost woods that no Kokiri&now remembers how to get to...\x07";
    msg8Str.push_back((char)((msg8>>8)&0xFF));
    msg8Str.push_back((char)((msg8)&0xFF));
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+8,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          msg8Str,
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+9,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Perhaps there are forest creatures&who do remember though...",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+10,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "I'll give you this, if you&promise to never let anyone know.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+11,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Things are mostly growing&nicely, but I still can't help&but think that maybe just one&spot ought to have grown better...",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+12,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Mido always works us so hard.&I keep having to work on patching up&the crack at the back of his house.^He constantly complains about how&it lets the weather in,&but no matter what I do,&it's never good enough!",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+13,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "I love that each of us&Kokiri have a unique musical&note of our own!^Well, most of us at least.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+14,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "It's great that fairy&magic allows us to disappear&when we want to.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+15,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Sometimes I wonder how powerful&our magic really is.^Maybe there are beings&far more powerful than us,&and even the Great Deku Tree.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+16,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "I'm probably going to go&to collect firewood from the&lost woods later on.^But you know, lately I've been&getting to think about&how strange that is.^This village used to have&enough trees that we seldom&had to do this before.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+17,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Wait, what's going on at Mido's place!&Why is nobody helping him?^I swear, I'll have to kick&somebody's pants for this!",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+18,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Gosh! What happened?&Mido has quite a problem on&his hands, doesn't he?",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+19,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Hahaha, wow! What happened?&Mido has quite a problem on&his hands, doesn't he?&\x14\x04Tee hee hee.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+20,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "So, it seems that Mido has&gotten over his little problem.&Nice!",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+21,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "So, it seems that Mido has&gotten over his little problem.&\x14\x10Nice!",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+22,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "It would be nice if we&had things that could&live in the water here.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+23,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Ohh, there's one swishing around.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+24,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          msgFishStr,
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+25,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "What's funny about Sodo&is that she seems to&forget things a lot.^Why is she like that I wonder?&She had a much better memory& before we started having festivals.^But there's no way we're&going to stop having those now!",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+26,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "What's funny about Sodo&is that she seems to&forget things a lot.^Why is she like that I wonder?",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+27,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "I've definitely gotten beter&at taking care of things after&getting so much practice.^I wish I had more things to&practice with though.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, KokiriMsg+28,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Man, this is so hard.&The trees here are&so thick and old.",
          "",
          "",
        }
    );
    MakeNormalMsgEng(KokiriMsg+29, "I've been chopping up&wood for years,&and after doing it for so long,^I've got to say,&there's got to be,&a better way.^Please help me if you&can, your body looks&strong enough.");

    MakeNormalMsgEng(KokiriMsg+30, "I've been chopping up&wood for years,&and after doing it for so long,^I've got to proclaim,&that things can be lame,&if they stay the same.^I've been managing ok myself,&but the more help the merrier.");

    MakeNormalMsgEng(KokiriMsg+31, "I've been chopping up&wood for years,&and after doing it for&so long, I feel so strong.^Not as strong as you would be,&but I don't need to be to&hack up fresh young wood!");

    MakeNormalMsgEng(KokiriMsg+32, "I would tell Saria that I've&canceled our anniversary party,&but I'm afraid to let&her down to her face.^I know she was really&looking forward to it.");

    MakeNormalMsgEng(KokiriMsg+33, "Hey, I know you went through&the Lost Woods, and you probably&encountered Mido there,&looking out for Saria.^Mido had always given&\x0F a hard time and&I think he somewhat regretted&that once he realised \x0F^wouldn't be returning,&and especialy now that we&know he did not cause the&Great Deku Tree to wither.^But at the same time,&there seems to be&some reason why he continued&to hold a grudge against \x0F^that he has never let go of,&and I can only speculate&on why.");
    MakeNormalMsgEng(KokiriMsg+34, "Hey, I know you went through&the Lost Woods, and you probably&encountered Mido there,&looking out for Saria.^Mido had always given&\x0F a hard time and&I think he somewhat regretted&that once he realised \x0F^wouldn't be returning,&and especialy now that we&know he did not cause the&Great Deku Tree to wither.^Mido seems to have always been&jealous of \x0F for some reason,&but now I have no doubt^that he would put&aside those old feelings,&were he to meet \x0F again.");


    u16 MidoMsg = TextIDAllocator::Instance->allocateRange("mido", 30);

    MakeNormalMsgEng(MidoMsg, "Ahhhhhhhhrrrrrrgh!!!^Why are there so many bugs!?^I can't stop them from&crawling everywhere!");

    MakeNormalMsgEng(MidoMsg+1,"Oh, phew.^Hmph, I'm glad you helped&out there, although I'd&expect that from any Kokiri.");

    MakeNormalMsgEng(MidoMsg+2,"Hey!!!&What do you think you're doing?^No way are you dumping&things on my floor!^The great Mido will see to it&that you are properly punished!");

    MakeNormalMsgEng(MidoMsg+3, "Actually, today Saria isn't&at her usual spot.^I heard she came around here,&but I'm honestly not sure&where she went.");

    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MidoMsg+4,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "I never got the opportunity&to express my gratitude to&him for everything.^So I can only hope that&you might do so in my place.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MidoMsg+5,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "However, I still could never&forgive him for what he did.^\x0F, I wish you could have&redeemed yourself somehow.",
          "",
          "",
        }
    );
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MidoMsg+6,
        {
          TEXTBOX_TYPE_BLACK, TEXTBOX_POS_BOTTOM,
          "Actually, today Saria isn't&at her place.^I heard she's around here somewhere,&but I'm honestly not sure&where she went.",
          "",
          "",
        }
    );

    MakeContinuingMsgEng(MidoMsg+7, "If you really need to know, Saria&should be in her house now.", MidoMsg+8, msgM8);

    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MidoMsg+8,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_TOP,
          "What business would YOU&have there though?",
          "",
          "",
        }
    );

    MakeContinuingMsgEng(MidoMsg+9, "What's happening?^Everything seems to have&returned to normal...&but what about Saria?^Saria must be coming back,&I refuse to accept anything else!", MidoMsg+10, msgM10);
    MakeNormalMsgEng(MidoMsg+10,"But at least, maybe there's&a chance you could fulfill&the promise I made.^It's about someone we knew&years ago, who left us.^I promised Saria that,&if he ever appeared again,^I would tell him she had been&waiting for him,&and that he should&await her return.^His name was \x0F.");
    MakeNormalMsgEng(MidoMsg+11,"In a way, I regret&making that promise to her.^I could never bring myself to trust&\x0F,no matter how much Saria&would swear by his innocence.^I just cannot shake the feeling&in my gut that he was&responsible for taking&away everything I held dear.^But I am a man of my word&and would never back down and&break a promise I made to Saria^So, please, if you ever&encounter him on your travels,&please tell him what&Saria said, for her sake.");
    MakeNormalMsgEng(MidoMsg+12,"\x0F, how I wish you could have&shown you were deserving&of my forgiveness.");
    MakeNormalMsgEng(MidoMsg+13,"\x0F, how that name fills me&with anger! How could I&even hope of forgiving you?");
    MakeNormalMsgEng(MidoMsg+14,"\x0F, how I wish I could have&understood you.^I don't know what he was&doing, but Saria trusted him&and he always treated us well.");
    MakeNormalMsgEng(MidoMsg+15,"But now I can no longer&even speak to Saria through&the power of her music...^All I wanted was to&just to talk to her.");//&\x14\x03one...last...time
    MakeContinuingMsgEng(MidoMsg+16,"Saria\x14\x05...?!", MidoMsg+17, msgM17);
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MidoMsg+17,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_TOP,
          "Mido, it's me.&Don't worry, I'm safe, but I can no longer&come back to the forest.",
          "",
          "",
        }
    );
    MakeContinuingMsgEng(MidoMsg+18,"I'm so glad you're safe,&but why aren't you coming back?", MidoMsg+19, msgM19);
    CustomMessageManager::Instance->CreateMessage(
        questMessageTableID, MidoMsg+19,
        {
          TEXTBOX_TYPE_BLUE, TEXTBOX_POS_TOP,
          "Because Mido, I need to&aid the person who&is saving this land.^Mido,&that is \x0F.",
          "",
          "",
        }
    );
    MakeNormalMsgEng(MidoMsg+20, "...^Thank you...for everything.");


    u16 GoronMsg = TextIDAllocator::Instance->allocateRange("goron", 30);

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
    MakeNormalMsgEng(GoronMsg+15, "Things are quiet now.&I'm surprised nobody seems to&be open at this time of day anymore.");
    MakeNormalMsgEng(GoronMsg+16, "Wait, how did you get in&here at this time?");
    MakeNormalMsgEng(GoronMsg+17, "Some brothers are off racing.&It's quite a spectacle&if you want to see it.^The races are usually quite close.");
    MakeNormalMsgEng(GoronMsg+18, "Look at our big brother's&refined rolling.^He's matured quite a bit&since his wilder days.");
    MakeNormalMsgEng(GoronMsg+19, "I still miss the shiny&red stone that sat here.&Oh how it glowed.");
    MakeNormalMsgEng(GoronMsg+20, "Sorry! Let me express my feelings&with more refined rolling!");
    MakeNormalMsgEng(GoronMsg+21, "Big brother isn't here now.&I wonder where he went.");
    MakeNormalMsgEng(GoronMsg+22, "Big brother is both courteous and&tough. He tries to avoid hitting&anyone if he can help it,^and is barely phased by the&explosions of our special crop anymore.^I hear he still dislikes being&hit by them though.");
    MakeNormalMsgEng(GoronMsg+23, "Oh, I give up,&I might as well ask you&to pawn this off for me.^I'll take %g200 rupees%w for it.&\x1B&%gYes&No%w\x02");
    MakeNormalMsgEng(GoronMsg+24, "Ah, I like it here.&It's so peaceful and quiet.^You know, I think it's&about time I went shopping again.^I remember the last time I went,&and hope it goes better this time.^Should I try again?&\x1B&%gYes&No%w\x02");
    MakeNormalMsgEng(GoronMsg+25, "Oh, you expected this to&lead to something didn't you?^Well sorry, that's what&you get for playing&the beta of this mod!");
    MakeNormalMsgEng(GoronMsg+26, "Aww, too bad.");
    MakeNormalMsgEng(GoronMsg+27, "What, you don't have enough either?");
    MakeNormalMsgEng(GoronMsg+28, "We have a deal then.\x0B\x02");

    u16 HylianMsg = TextIDAllocator::Instance->allocateRange("hylian", 30);

    MakeNormalMsgEng(HylianMsg+0, "Oh, so I guess you're here&for the night trade then.^We normally don't let&kids get involved, but&you seem savy enough.");
    MakeNormalMsgEng(HylianMsg+1, "Here's our special night offers.\x0B\x02");
    MakeNormalMsgEng(HylianMsg+2, "I don't typically take&guests during the day.^That's why I lock my door,&as you'd know.^If you have business here,&please let that be known&ahead of time.");
    MakeNormalMsgEng(0xF000, "\x08%rPiece of Heart   200 Rupees%w&Adds a quarter of a section&to your heart container.&4 together will improve your life!\t\n\x02")
    MakeNormalMsgEng(0xF001, "\x08Piece of Heart   200 Rupees&\x09\x1B&%gBuy&Don't buy%w\x02")

    u16 ZoraMsg = TextIDAllocator::Instance->allocateRange("zora", 30);

    MakeNormalMsgEng(ZoraMsg+0, "Wow! I never suspected that&anyone who was not a&Zora could dive like that!^You cannot refuse this token&of my awe!\x0B\x02");
    MakeNormalMsgEng(ZoraMsg+1, "Oh, you do know a boy was&seen hanging around this domain&on the day Ruto snuck away again.^Surely you understand why people&are...suspecting things?");
    MakeNormalMsgEng(ZoraMsg+2, "Oh, you do know a boy was&seen hanging around the&shores of Lake Hylia&on the day Ruto snuck away again.^Surely you understand why people&are...suspecting things?");
    MakeNormalMsgEng(ZoraMsg+3, "I heard Ruto was treated very&roughly the last time she went out.^I would hope our princess would&deserve far more dignified&treatment from anyone.");
    MakeNormalMsgEng(ZoraMsg+4, "I heard Ruto had some scary&encounters on that night.&I hope that doens't happen to our princess again.");
    MakeNormalMsgEng(ZoraMsg+5, "I heard Ruto was made quite&unconfortable on her night away.&I would hope anyone in her company&would treat her with good courtesy.");
    MakeNormalMsgEng(ZoraMsg+6, "Apparantly, Ruto's boyfriend&just abandoned her,&leaving her completely alone!^How unthinkable!");
    MakeNormalMsgEng(ZoraMsg+7, "Apparantly, whoever was&supposed to meet Ruto&just never showed up at all!^How unbelievable!");
    MakeNormalMsgEng(ZoraMsg+8, "There was apparantly something&unexpected that happend&which bothered Ruto the&last time she went out.^I can't say I'm aware of what&that was in particular though.");
    MakeNormalMsgEng(ZoraMsg+9, "You know what I heard.&Apparantly, Ruto's date&showed up far ahead of time.^Almost like he was excessively&desperate for her.");
    MakeNormalMsgEng(ZoraMsg+10, "You know what I heard.&Apparantly, Ruto's date&showed up incredibly late.^Like he had no regard&for her whatsoever!");
    MakeNormalMsgEng(ZoraMsg+11, "I heard the boy Ruto met&was acting very strange&when she showed up.^Like he was just hanging around&at first, silently watching her.^Creepy.");
    MakeNormalMsgEng(ZoraMsg+12, "Those who cannot give their&partner a propper seeing&off deserve no respect.");
    MakeNormalMsgEng(ZoraMsg+13, "Oh, so there is a Zora girl here,&none other than the&Royal Princess Ruto.^Glad to see you're keeping her safe...");
    MakeNormalMsgEng(ZoraMsg+14, "Those who do not think about&their partner's wishes&deserve no respect.");
    MakeNormalMsgEng(ZoraMsg+15, "Those who do not think about&their partner at all&deserve absolutely no respect.");
    MakeNormalMsgEng(ZoraMsg+16, "Well...she is very pretty&isn't she?^You wouldn't have&any pictures of her,&now would you?^I'd promise to not show&them to anyone else.");
    MakeNormalMsgEng(ZoraMsg+17, "I heard the boy Ruto met&was acting very strange&when she showed up.^Like he wouldn't even talk&to her even after she'd&greeted him.^Creepy.");

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

    u16 CursedFamilyMsg = TextIDAllocator::Instance->allocateRange("cursedfamily", 20);
    MakeNormalMsgEng(CursedFamilyMsg+0, "Our father may be telling you&that he has no need for&any help, but please^ignore his politeness.&I may have more rewards&for you yet!");
    MakeNormalMsgEng(CursedFamilyMsg+1, "I'm happy to have given&what I can, please keep&destroying the curse!");
    MakeNormalMsgEng(CursedFamilyMsg+2, "You've done such good work!&Please accept this reward!\x0B\x02");
}
