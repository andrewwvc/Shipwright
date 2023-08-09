#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO

#include "OTRGlobals.h"
#include <libultraship/libultraship.h>
#include "soh/resource/type/Scene.h"
#include <Utils/StringHelper.h>
#include "global.h"
#include "vt.h"
#include "soh/resource/type/CollisionHeader.h"
#include <DisplayList.h>
#include "soh/resource/type/Cutscene.h"
#include "soh/resource/type/Path.h"
#include "soh/resource/type/Text.h"
#include <Blob.h>
#include <memory>
#include "soh/resource/type/scenecommand/SetCameraSettings.h"
#include "soh/resource/type/scenecommand/SetCutscenes.h"
#include "soh/resource/type/scenecommand/SetStartPositionList.h"
#include "soh/resource/type/scenecommand/SetActorList.h"
#include "soh/resource/type/scenecommand/SetCollisionHeader.h"
#include "soh/resource/type/scenecommand/SetRoomList.h"
#include "soh/resource/type/scenecommand/SetEntranceList.h"
#include "soh/resource/type/scenecommand/SetSpecialObjects.h"
#include "soh/resource/type/scenecommand/SetRoomBehavior.h"
#include "soh/resource/type/scenecommand/SetMesh.h"
#include "soh/resource/type/scenecommand/SetObjectList.h"
#include "soh/resource/type/scenecommand/SetLightList.h"
#include "soh/resource/type/scenecommand/SetPathways.h"
#include "soh/resource/type/scenecommand/SetTransitionActorList.h"
#include "soh/resource/type/scenecommand/SetSkyboxSettings.h"
#include "soh/resource/type/scenecommand/SetSkyboxModifier.h"
#include "soh/resource/type/scenecommand/SetTimeSettings.h"
#include "soh/resource/type/scenecommand/SetWindSettings.h"
#include "soh/resource/type/scenecommand/SetSoundSettings.h"
#include "soh/resource/type/scenecommand/SetEchoSettings.h"
#include "soh/resource/type/scenecommand/SetAlternateHeaders.h"
#include "SaveManager.h"

using json = nlohmann::json;

// bool operator==(const Ship::ActorSpawnEntry& lhs, const Ship::ActorSpawnEntry& rhs)
// {
//     return (lhs.actorNum == rhs.actorNum)
//     && (lhs.posX == rhs.posX)
//     && (lhs.posY == rhs.posY)
//     && (lhs.posZ == rhs.posZ)
//     && (lhs.rotX == rhs.rotX)
//     && (lhs.rotY == rhs.rotY)
//     && (lhs.rotZ == rhs.rotZ)
//     && (lhs.initVar == rhs.initVar);
// }

// void copyActorSpawn(ActorSpawnEnt& lhs, const Ship::ActorSpawnEntry& rhs) {
//     lhs.actorNum = rhs.actorNum;
//     lhs.posX = rhs.posX;
//     lhs.posY = rhs.posY;
//     lhs.posZ = rhs.posZ;
//     lhs.rotX = rhs.rotX;
//     lhs.rotY = rhs.rotY;
//     lhs.rotZ = rhs.rotZ;
// }

constexpr bool operator<(const ActorSpawnResource& lhs, const ActorSpawnResource& rhs)
{
    if (lhs.scene < rhs.scene)
        return true;
    else if (lhs.scene > rhs.scene)
        return false;
    else if (lhs.room < rhs.room)
        return true;
    else if (lhs.room > rhs.room)
        return false;
    else if (lhs.entry.actorNum < rhs.entry.actorNum)
        return true;
    else if (lhs.entry.actorNum > rhs.entry.actorNum)
        return false;
    else if (lhs.entry.posX < rhs.entry.posX)
        return true;
    else if (lhs.entry.posX > rhs.entry.posX)
        return false;
    else if (lhs.entry.posY < rhs.entry.posY)
        return true;
    else if (lhs.entry.posY > rhs.entry.posY)
        return false;
    else if (lhs.entry.posZ < rhs.entry.posZ)
        return true;
    else if (lhs.entry.posZ > rhs.entry.posZ)
        return false;
    else if (lhs.entry.rotX < rhs.entry.rotX)
        return true;
    else if (lhs.entry.rotX > rhs.entry.rotX)
        return false;
    else if (lhs.entry.rotY < rhs.entry.rotY)
        return true;
    else if (lhs.entry.rotY > rhs.entry.rotY)
        return false;
    else if (lhs.entry.rotZ < rhs.entry.rotZ)
        return true;
    else if (lhs.entry.rotZ > rhs.entry.rotZ)
        return false;
    else if (lhs.entry.initVar < rhs.entry.initVar)
        return true;
    else if (lhs.entry.initVar > rhs.entry.initVar)
        return false;
    else if (lhs.dirt < rhs.dirt)
        return true;
    else if (lhs.dirt > rhs.dirt)
        return false;

    return false;
}

void to_json(json& j, const ActorSpawnResource& p) {
    j = json{{"scene", p.scene}, {"room", p.room}, {"actorNum", p.entry.actorNum},
        {"posX", p.entry.posX}, {"posY", p.entry.posY}, {"posZ", p.entry.posZ},
        {"rotX", p.entry.rotX}, {"rotY", p.entry.rotY}, {"rotZ", p.entry.rotZ},
        {"initVar", p.entry.initVar}, {"dirt", p.dirt}};
}

void from_json(const json& j, ActorSpawnResource& p) {
    j.at("scene").get_to(p.scene);
    j.at("room").get_to(p.room);
    j.at("actorNum").get_to(p.entry.actorNum);
    j.at("posX").get_to(p.entry.posX);
    j.at("posY").get_to(p.entry.posY);
    j.at("posZ").get_to(p.entry.posZ);
    j.at("rotX").get_to(p.entry.rotX);
    j.at("rotY").get_to(p.entry.rotY);
    j.at("rotZ").get_to(p.entry.rotZ);
    j.at("dirt").get_to(p.dirt);
}

extern std::map<ActorSpawnResource,int> UsedResources;
extern std::map<int,ActorSpawnResource> TempResourceEntries;

void insertSpawnResource(int entry, int extraTime) {
    auto itt = TempResourceEntries.find(entry);
    if (itt != TempResourceEntries.end()) {
        ActorSpawnResource sw = itt->second;
        auto existing = UsedResources.insert({sw,gSaveContext.savedFrameCount+extraTime});
        if (!existing.second) {
            existing.first->second = gSaveContext.savedFrameCount+extraTime;
        }
    }
}

s32 createTempEntryPlus(PlayState* play, ActorEntry* spawn, s16 dirt) {
    ActorSpawnResource sw;
    sw.scene = play->sceneNum;
    sw.room = play->roomCtx.curRoom.num;
    sw.entry.actorNum = spawn->id;
    sw.entry.initVar = spawn->params;
    sw.entry.posX = spawn->pos.x;
    sw.entry.posY = spawn->pos.y;
    sw.entry.posZ = spawn->pos.z;
    sw.entry.rotX = spawn->rot.x;
    sw.entry.rotY = spawn->rot.y;
    sw.entry.rotZ = spawn->rot.z;
    sw.dirt = dirt;
    int entNum;
    auto node = TempResourceEntries.rbegin();
    if (node != TempResourceEntries.rend())
        entNum = node->first+1;
    else
        entNum = 0;
    auto foundVal = UsedResources.find(sw);
    if (foundVal != UsedResources.end()) {
        return -1;
    }
    TempResourceEntries.insert({entNum, sw});
    return entNum;
}

//Returns -1 if entry was found, otherwise return the entry number
s32 createTempEntry(PlayState* play, ActorEntry* spawn) {
    return createTempEntryPlus(play,spawn,0);
}

s32 isResourceUsed(PlayState* play, ActorEntry* spawn, s16 dirt) {
    ActorSpawnResource sw;
    sw.scene = play->sceneNum;
    sw.room = play->roomCtx.curRoom.num;
    sw.entry.actorNum = spawn->id;
    sw.entry.initVar = spawn->params;
    sw.entry.posX = spawn->pos.x;
    sw.entry.posY = spawn->pos.y;
    sw.entry.posZ = spawn->pos.z;
    sw.entry.rotX = spawn->rot.x;
    sw.entry.rotY = spawn->rot.y;
    sw.entry.rotZ = spawn->rot.z;
    sw.dirt = dirt;
    auto foundVal = UsedResources.find(sw);
    if (foundVal != UsedResources.end()) {
        return 1;
    } else {
        return 0;
    }
}

extern LUS::IResource* OTRPlay_LoadFile(PlayState* play, const char* fileName);
extern "C" s32 Object_Spawn(ObjectContext* objectCtx, s16 objectId);
extern "C" RomFile sNaviMsgFiles[];
s32 OTRScene_ExecuteCommands(PlayState* play, LUS::Scene* scene);

std::shared_ptr<LUS::File> ResourceMgr_LoadFile(const char* path) {
    std::string Path = path;
    if (IsGameMasterQuest()) {
        size_t pos = 0;
        if ((pos = Path.find("/nonmq/", 0)) != std::string::npos) {
            Path.replace(pos, 7, "/mq/");
        }
    }
    return LUS::Context::GetInstance()->GetResourceManager()->LoadFile(Path.c_str());
}

// Forward Declaration of function declared in OTRGlobals.cpp
std::shared_ptr<LUS::IResource> GetResourceByNameHandlingMQ(const char* path);

bool Scene_CommandSpawnList(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetStartPositionList* cmdStartPos = std::static_pointer_cast<LUS::SetStartPositionList>(cmd);
    LUS::SetStartPositionList* cmdStartPos = (LUS::SetStartPositionList*)cmd;
    ActorEntry* entries = (ActorEntry*)cmdStartPos->GetRawPointer();
// bool Scene_CommandSpawnList(PlayState* play, Ship::SceneCommand* cmd)
// {
//     Ship::SetStartPositionList* cmdStartPos = (Ship::SetStartPositionList*)cmd;

//     ActorEntry* linkSpawnEntry = nullptr;

//     if (cmdStartPos->cachedGameData != nullptr)
//     {
//         ActorEntry* entries = (ActorEntry*)cmdStartPos->cachedGameData;
//         linkSpawnEntry = &entries[play->setupEntranceList[play->curSpawn].spawn];
//     }
//     else
//     {
//         ActorEntry* entries = (ActorEntry*)malloc(sizeof(ActorEntry) * cmdStartPos->entries.size());

//         for (int i = 0; i < cmdStartPos->entries.size(); i++)
//         {
//             entries[i].id = cmdStartPos->entries[i].actorNum;
//             entries[i].params = cmdStartPos->entries[i].initVar;
//             entries[i].pos.x = cmdStartPos->entries[i].posX;
//             entries[i].pos.y = cmdStartPos->entries[i].posY;
//             entries[i].pos.z = cmdStartPos->entries[i].posZ;
//             entries[i].rot.x = cmdStartPos->entries[i].rotX;
//             entries[i].rot.y = cmdStartPos->entries[i].rotY;
//             entries[i].rot.z = cmdStartPos->entries[i].rotZ;

//             SPDLOG_INFO("Spawn {0:d} - ID: {1:x}, Parameters: {2:x}", i, (uint16_t)entries[i].id, (uint16_t)entries[i].params);
//         }

//         linkSpawnEntry = &entries[play->setupEntranceList[play->curSpawn].spawn];
//         cmdStartPos->cachedGameData = entries;
//     }

//     ActorEntry* linkEntry = play->linkActorEntry = linkSpawnEntry;

//     s16 linkObjectId;

    play->linkActorEntry = &entries[play->setupEntranceList[play->curSpawn].spawn];
    play->linkAgeOnLoad = ((void)0, gSaveContext.linkAge);
    s16 linkObjectId = gLinkObjectIds[((void)0, gSaveContext.linkAge)];

    Object_Spawn(&play->objectCtx, linkObjectId);

    return false;
}


const std::map<u16, std::map<u16, std::vector<std::tuple<int, int, LUS::ActorEntry>>>> sceneActorOverrides = {
    { 0x01, { // Dodongo's Cavern
        { 0x03, {
            { -1, 4, { ACTOR_EN_ITEM00, 4266,100,-1575, 0, -21299, 0, 0x100+(uint16_t)ITEM00_HEART_PIECE  }},
        } },
    } },
    { 0x02, { // Jabu Jabu's Belly
        { 0x0d, {
            //{ -1, 3, { ACTOR_EN_ITEM00, -1150, -1113, -2248, 0, 0, 0, 0x106 }},
        } },
    } },
    { 0x04, { // Fire Temple
        { 0x08, {
            { -1, 2, { 0xA, 1944,4681,-393, 0,24394,0, 0x07CD }},
        } },
        { 0x0A, {
            { -1, -1, { ACTOR_EN_ZF, -1925,2800,780, 0,24394,0, 0xFFFF }},
            { -1, -1, { ACTOR_EN_ZF, -1740,2800,-1060, 0,24394,0, 0xFFFF }},
        } },
    } },
    { 0x8, {//Well
        { 0x00, {
            { -1, 32, { ACTOR_ELF_MSG2, -627, 97, -1553,  2,0,1, (0x3F<<8)|0xB3 }},
            { -1, 34, { ACTOR_ELF_MSG2, -986, 59, 87,  2,0,1, (0x3F<<8)|0xB4 }},
            { -1, -1, { ACTOR_ELF_MSG2, 1186, 97, -1500,  2,0,1, (0x3F<<8)|0xB6 }},
            { -1, -1, { ACTOR_ELF_MSG2, 865, 40, 305,  2,0,1, (0x3F<<8)|0xB8 }},
            { -1, -1, { ACTOR_ELF_MSG2, 865, 40, 115,  2,0,1, (0x3F<<8)|0xB7 }},
            { -1, -1, { ACTOR_ELF_MSG2, 0, 80, -745,  2,0,1, (0x3F<<8)|0xB9 }},
        } },
        { 0x02, {
            { -1, -1, { ACTOR_ELF_MSG2, -2145, 10, -615,  2,0,1, (0x3F<<8)|0xB5 }},
        } },
    } },
    { 0x55, { // Kokiri Forest
        { 0x00, {//Village
            //{ -1, -1, { ACTOR_EN_ITEM00, 1297, 240, -553, 0, 0, 0, 0x0800+(uint16_t)ITEM00_HEART_PIECE }},
            //{ -1, -1, { ACTOR_EN_TEST, 70, -80, 675, 0, 0, 0, 0xFFFF }},
            //{ -1, -1, { ACTOR_EN_WOOD02, 0, -80, 870, 0, 0, 0, 0x0011 }},
            //{ -1, 72, { ACTOR_EN_ITEM00, 0, -80, 870, 0, 0, 0, 0x100+(uint16_t)ITEM00_HEART_PIECE }},
            //{ -1, -1, { ACTOR_EN_SA, -110, -80, 950, 0, 0, 0, 0x0}},
            { 0, -1, { ACTOR_OBJ_MAKEKINSUTA, -367,53,-770, 0, 0, 1, 0x0 }}, { 1, -1, { ACTOR_OBJ_MAKEKINSUTA, -367,53,-770, 0, 0, 1, 0x0 }},
            { 0, -1, { ACTOR_OBJ_MAKEKINSUTA, -459,55,-818, 0, 0, 1, 0x0 }}, { 1, -1, { ACTOR_OBJ_MAKEKINSUTA, -459,55,-818, 0, 0, 1, 0x0 }},
            { 0, -1, { ACTOR_OBJ_MAKEKINSUTA, -521,56,-773, 0, 0, 1, 0x0 }}, { 1, -1, { ACTOR_OBJ_MAKEKINSUTA, -521,56,-773, 0, 0, 1, 0x0 }},
            { -1, -1, { 0x77, 355,0,1095, 1, 0, 0, 0x20 }},
            { -1, -1, { 0x77, 860,185,-475, 2, 0, 0, 0x20 }},
            { -1, -1, { 0x77, -565,120,880, 3, 0, 0, 0x20 }},
            { -1, -1, { 0x77, 500,120,105, 4, 0, 0, 0x20 }},
            { -1, -1, { 0x77, -916,60,-935, 7, 0, 0, 0x20 }},
            //{ -1, -1, { ACTOR_EN_ITEM00, 0, -80, 870, 0, 0, 0, 0x100+(uint16_t)ITEM00_HEART_PIECE }},
        } },
        { 0x01, {//Deku Tree
            { -1, -1, { 0x77, 4365,-155,-2365, 5, 0, 0, 0x20 }},
            { -1, -1, { 0x77, 3900,-180,-190, 6, 0, 0, 0x20 }},
        } },
    } },
    { SCENE_LINK_HOME, {
        { 0x00, {
            //{ -1, -1, { ACTOR_EN_ITEM00, 0, 0, 0, 0, 0, 0, 0x100+(uint16_t)ITEM00_HEART_PIECE}},
            { -1, -1, { ACTOR_EN_SA, 100,0,1, 0,-12350,0, 0x0}},
            { 0, -1, { ACTOR_EN_WONDER_TALK2, -92,25,-8, 0,15350,0, 0x8000|(0x3D<<6)|0x3f /*0x8abf*/}},
            { 1, -1, { ACTOR_EN_WONDER_TALK2, -92,25,-8, 0,15350,0, 0x8000|(0x3D<<6)|0x3f /*0x8abf*/}},
            { 2, -1, { ACTOR_EN_WONDER_TALK2, -92,25,-8, 0,15350,0, 0x8000|(0x3E<<6)|0x3f /*0x8abf*/}},
            { 3, -1, { ACTOR_EN_WONDER_TALK2, -92,25,-8, 0,15350,0, 0x8000|(0x3E<<6)|0x3f /*0x8abf*/}},
        } },
    } },
    { 0x29, {//Saria's
        { 0x00, {
            //{ -1, -1, { ACTOR_EN_ITEM00, 0, 0, 0, 0, 0, 0, 0x100+(uint16_t)ITEM00_HEART_PIECE}},
            { -1, -1, { ACTOR_EN_WONDER_ITEM, -79,0,-151, 0,0,1, 0x1241}},
        } },
    } },
    { SCENE_SPOT10, { // Lost Woods
        { 0x5, {//Bridge
            { -1, -1, { ACTOR_EN_KO, -1150,-360,1152, 0,1200,0, 0xff00 }},
            { -1, -1, { ACTOR_OBJ_MAKEKINSUTA, -1140,-10,1208, 0, 0, 2, 0x0 }},
        } },
    } },
    { 0x09, { // Ice Cavern
        { 0x09, {
            //{ -1, 9, { ACTOR_EN_ITEM00, 366, 213, -2036, 0, 0, 0, 0x406 }},
        } },
    } },
    { SCENE_SPOT00, { // Hyrule Field
        { 0x00, {
            //{ 2, -1, { 0x77, 4870, -160, 8506, 0, 0, 0, 0x11 }}, //{ 3, -1, { 0x77, 4870, -160, 8506, 0, 0, 0, 0x11 }},
            { 2, -1, { 0x00A7, 116,192,6206, 0, 0, 0, 0x18a4 }}, { 3, -1, { 0x00A7, 116,192,6206, 0, 0, 0, 0x18a4 }},
            { 0, -1, { ACTOR_EN_GO2,  3173,-20,880,  0,-20000,0,  0x03e0 | (13<<10)}}, { 1, -1, { ACTOR_EN_GO2,  3173,-20,880,  0,-20000,0,  0x03e0 | (13<<10)}},
            { 2, -1, { ACTOR_EN_HORSE, 100,0,2000, 0,0,0, 0x800B} }, { 3, -1, { ACTOR_EN_HORSE, 100,0,2000, 0,0,0, 0x800B} },
            {2, -1, { ACTOR_EN_MB, -4010,-300,1700, 0,8104,0, 0xffff}},
            {3, -1, { ACTOR_EN_MB, -4010,-300,1700, 0,8104,0, 0xffff}},
            {2, -1, { ACTOR_EN_MB, -3157,-300,945, 0,8104,0, 0xffff}},
            {3, -1, { ACTOR_EN_MB, -3157,-300,945, 0,8104,0, 0xffff}},
            {2, -1, { ACTOR_EN_MB, -5245,-300,2000, 0,-1321,0, 0xffff}},
            {3, -1, { ACTOR_EN_MB, -5245,-300,2000, 0,-1321,0, 0xffff}},
        } },
    } },
    { SCENE_MARKET_DAY, { // Castle Town Square - Day
        { 0x00, {
            { -1, -1, { ACTOR_EN_GO2, -288,0,188, 0,0x4000,0, 0x03ed | 0x10 | (0x0<<10)}},
        } },
    } },
    { SCENE_MARKET_NIGHT, { // Castle Town Square - Night
        { 0x00, {
            { -1, -1, { ACTOR_EN_GO2, 298,0,234, 0,-0x4000,0, 0x03ed | 0x10 | (0x1<<10)}},
        } },
    } },
    { SCENE_MARKET_RUINS, { // Castle Town Square - Ruined
        { 0x00, {
            { -1, -1, { ACTOR_EN_GO2, -288,0,188, 0,0x4000,0, 0x03ed | 0x10 | (0x0<<10)}},
        } },
    } },
    { 0x3E, { // Grotto
        { 0x05, { // Octorok Grotto
            //{ -1, 8, { ACTOR_EN_ITEM00, 32, -129, 852, 0, 0, 0, 0x406 }},
        } },
        { 0x07, {
            { -1, -1, { ACTOR_EN_ITEM00, 1180,100,800, 0,0,0, 0x100+(uint16_t)ITEM00_HEART_PIECE  }},
        } },
    } },
    { SCENE_MALON_STABLE, { //Ranch
        { 0x00, {
            { -1, -1,  {ACTOR_EN_NIW, 0,0,0, 0,0x4000,0, 0x10}},
        } },
    } },
    { SCENE_SOUKO, {
        { 0x00, {
            { -1, -1, { ACTOR_BG_JYA_KANAAMI, -382,200,115, 0,-0x4000,0, 0x0 }},
            { -1, -1, { ACTOR_EN_A_OBJ, -430,415,0, 0,-0x4000,0, 0x8 }},
            { -1, -1, { ACTOR_EN_A_OBJ, -510,415,0, 0,-0x4000,0, 0x8 }},
            { -1, -1, { ACTOR_EN_A_OBJ, -590,415,0, 0,-0x4000,0, 0x8 }},
            { -1, -1, { ACTOR_EN_ITEM00, -590,495,0, 0,0,0, 0x400+(uint16_t)ITEM00_HEART_PIECE  }},
        } },
        { 0x02, {
            { 0, -1,  {ACTOR_EN_MA1, 1556,164,-160, 0,0,0, 0x0}}, { 1, -1,  {ACTOR_EN_MA1, 1556,164,-160, 0,0,0, 0x0}},
        } },
    } },
    { SCENE_SPOT20, { //Ranch
        { 0x00, {
            { 0, -1,  {ACTOR_EN_MA1, 1236,0,-2371, 0,0,0, 0x1}}, { 1, -1,  {ACTOR_EN_MA1, 1236,0,-2371, 0,0,0, 0x1}},
        } },
    } },
    { 0x37, {//Impa's
        { 0x00, {
            { -1, -1, { ACTOR_EN_WONDER_TALK2, 142,20,210, 0,15350,0, 0x8000|(0x3F<<6)|0x3f }},
            { -1, -1, { ACTOR_EN_WONDER_TALK2, 142,20,67,  0,15350,0, 0x8000|(0x40<<6)|0x3f }},
        } },
    } },
    { 0x52, { // Kakariko
        { 0x00, {
            //{ -1, 18, { 0x95, -18,800,1800, 0,-32768,0, 0xb140 }},
            { 1, 24, { ACTOR_EN_SW, 5,755,-100, 0,0,0, 0xB104 }},
        } },
    } },
    { SCENE_SPOT18, {//Goron City
        { 0x00, {
            { -1, -1, { ACTOR_EN_GO2, -1232,520,-1190, 0,0,0, 0x03eb | 0x10 | (0x0<<10)}},
        } },
        { 0x03, {
            { -1, -1, { ACTOR_EN_GO2, 1030,480,-380, 0,0,0, 0x03eb | 0x10 | (0x1<<10)}},
            { 2, -1, { ACTOR_EN_GO2, /*-20,-3,330,*/ 520,399,565,  0,-17295,0, /*0xffe0*/ 0x03e0 | (0x0<<10)}},
            { 3, -1, { ACTOR_EN_GO2, /*-20,-3,330,*/ 520,399,565,  0,-17295,0, /*0xffe0*/ 0x03e0 | (0x0<<10)}},
        } },
    } },
    { 0x60, {//Goron Trail
        { 0x00, {
            { 0, -1, { ACTOR_EN_GO2, /*-20,-3,330,*/ -8,3230,-4129,  0,0,0, /*0xffe0*/ 0x03e0 | (11<<10)}},
            { 1, -1, { ACTOR_EN_GO2, /*-20,-3,330,*/ -8,3230,-4129,  0,0,0, /*0xffe0*/ 0x03e0 | (11<<10)}},
            { 2, -1, { ACTOR_EN_GO2, -282,1258,-1585, 0,-4915,0, 0x03e5 | (0x1<<10)}},
            { 3, -1, { ACTOR_EN_GO2, -282,1258,-1585, 0,-4915,0, 0x03e5 | (0x1<<10)}},
            { 2, -1, { ACTOR_EN_GO2, -522,1264,-1560, 0,-4915,0, 0x03e5 | (0x2<<10)}},
            { 3, -1, { ACTOR_EN_GO2, -522,1264,-1560, 0,-4915,0, 0x03e5 | (0x2<<10)}},
            //{ 2, -1, { ACTOR_EN_GO2, /*-20,-3,330,*/ 520,399,565,  0,-17295,0, /*0xffe0*/ 0x03e0 | (0x3<<10)}},
            //{ 3, -1, { ACTOR_EN_GO2, /*-20,-3,330,*/ 520,399,565,  0,-17295,0, /*0xffe0*/ 0x03e0 | (0x3<<10)}},
        } },
    } },
    { 0x61, {//Mountian Crater
        { 0x01, {
            { 0, 34, { ACTOR_EN_SHOPNUTS, -853,769,1196, 0,0,0, 0xB}},
            { 1, 34, { ACTOR_EN_SHOPNUTS, -853,769,1196, 0,0,0, 0xB}},
        } },
    } },
    { 0x57, { // Lake Hylia
        { 0x00, {
            { -1, -1, { ACTOR_EN_RU1, -918,-1336,3560, 0,0x7FFF,0, 0xB }},
        } },
    } },
    { SCENE_SPOT03, { // Zora's River
        { 0x00, {
            { 0, 57, { ACTOR_EN_MS, -582,375,-438, 0,16384,0, 0xFFFF }},
            { 0, 60, { ACTOR_OBJ_BOMBIWA, 838,210,-194, 0,-20771,1, 0x0002 }},
            { 0, 62, { ACTOR_OBJ_BOMBIWA, -464,375,-558, 0,-25850,1, 0x0008 }},
            { 0, 63, { ACTOR_OBJ_BOMBIWA, -533,375,-594, 0,16384,1, 0x0009 }},
            { 0, 64, { ACTOR_OBJ_BOMBIWA, -594,375,-617, 0,0,1, 0x000A }},
            { 0, -1, { ACTOR_OBJ_BOMBIWA, -452,375,-480, 0,-25850,1, 0x000B }},
            { 1, 57, { ACTOR_EN_MS, -582,375,-438, 0,16384,0, 0xFFFF }},
            { 1, 60, { ACTOR_OBJ_BOMBIWA, 838,210,-194, 0,-20771,1, 0x0002 }},
            { 1, 62, { ACTOR_OBJ_BOMBIWA, -464,375,-558, 0,-25850,1, 0x0008 }},
            { 1, 63, { ACTOR_OBJ_BOMBIWA, -533,375,-594, 0,16384,1, 0x0009 }},
            { 1, 64, { ACTOR_OBJ_BOMBIWA, -594,375,-617, 0,0,1, 0x000A }},
            { 1, -1, { ACTOR_OBJ_BOMBIWA, -452,375,-480, 0,-25850,1, 0x000B }},
        } },
    } },
    { 0x59, { // Zora's Fountain
        { 0x00, {
            { 2, -1, { ACTOR_EN_GOROIWA, 1243,322,100, 0,-20771,1, 0x1D00 }},{ 3, -1, { ACTOR_EN_GOROIWA, 1243,322,100, 0,-20771,1, 0x1D00 }},
            { 2, 34, { ACTOR_BG_SPOT08_ICEBLOCK, 255,20,-285, 	0,16384,0, 0x1A }}, { 3, 34, { ACTOR_BG_SPOT08_ICEBLOCK, 255,20,-285, 	0,16384,0, 0x1A }},
            { 2, -1, { ACTOR_OBJ_MAKEKINSUTA, -253,32,-834, 0, 0, 3, 0x3 }}, { 2, -1, { ACTOR_OBJ_MAKEKINSUTA, 253,32,-834, 0, 0, 3, 0x3 }},
        } },
    } },
    { 0x5A, {//Gerudo Valley
        { 0x00, {
            { 0, 32, { ACTOR_BG_UMAJUMP, -391,-70,-98, 	-0x000,16384,0, 0x2}},
            { 1, 32, { ACTOR_BG_UMAJUMP, -391,-70,-98, 	-0x000,16384,0, 0x2}},
            { 0, -1, { ACTOR_EN_GO2,  2626,-240,447,  0,-22910,0,  0x03e0 | (12<<10)}},
            { 1, -1, { ACTOR_EN_GO2,  2626,-240,447,  0,-22910,0,  0x03e0 | (12<<10)}},
        } },
    } },
    { 0xC, { // Gerudo Rooms
        { 0x00, {
            { 0, -1, { ACTOR_OBJ_KIBAKO2, 529,0,-3556, 0,0,0, 0xFFFF }},
            { 1, -1, { ACTOR_OBJ_KIBAKO2, 529,0,-3556, 0,0,0, 0xFFFF }},
        } },
    } },
    { 0x5D, { // Gerudo Fortress
        { 0x00, {
            { 0, -1, { ACTOR_EN_NIW, 1525,834,-2020, 0,16384,0, 0xFFFF }},
            { 1, -1, { ACTOR_EN_NIW, 1525,834,-2020, 0,16384,0, 0xFFFF }},
            { 0, -1, { ACTOR_BG_UMAJUMP, 190,1103,-2915, 0x4000,880,0, 0xFFFF }},
            { 0, -1, { ACTOR_BG_UMAJUMP, 190,1103,-2915, 0x4000,880+0x8000,0, 0xFFFF }},
            { 1, -1, { ACTOR_BG_UMAJUMP, 190,1103,-2915, 0x4000,880,0, 0xFFFF }},
            { 1, -1, { ACTOR_BG_UMAJUMP, 190,1103,-2915, 0x4000,880+0x8000,0, 0xFFFF }},
            { 0, 21, { 0xA, 926,1042,-2512, 0,-16384,0, 0x7c1 }},
            { 1, 21, { 0xA, 926,1042,-2512, 0,-16384,0, 0x7c1 }},
        } },
    } },
    { SCENE_SPOT13, { //Desert Wasteland
        { 0x00, {
            { -1, 4, { ACTOR_EN_SW, 637,-393,-2536, 16384,0,0, 0xB602 }},
        } },
        { 0x01, {
            { -1, -1, { ACTOR_EN_HATA, 3315,-261, 2244, 0,0,0, 0xFFFF }},
            { -1, -1, { ACTOR_OBJ_KIBAKO2, 3460,-435,2484, 0,7100,0, 0xFFFF }},
            { -1, -1, { ACTOR_OBJ_KIBAKO2, 3621,-392,2847, 0,7100,0, 0xFFFF }},
            { -1, -1, { ACTOR_EN_NIW, 3125,-268,2459, 0,7100,0, 0xFFFF }},
        } },
    } },
    { SCENE_SYOTES2, {
        { 0x00, {
            //{ -1, 0, { 0x5D, 0,0, 100, 0,0,0,  0 }},
            { -1, -1, { ACTOR_EN_ITEM00, 0,20,380, 0,0,0, 0x100+(uint16_t)ITEM00_HEART_PIECE  }},
            } },
    } },
    {SCENE_BMORI1, {
        {0x0, {
            {-1, -1, {ACTOR_EN_ST, 118,510,155, 0,0,0, 0x4}}
        } },
        {0x3, {
            {-1, 4, {ACTOR_EN_BB, 1387,510,-1436, 0,0x4000,0, 0xffff}},
            {-1, -1, {ACTOR_EN_BB, 1187,463,-1436, 0,-0x4000,0, 0xffff}}
        } },
        {0x8, {
            {-1, -1, {ACTOR_EN_DEKUNUTS, -1288,242,-2109, 0,25009,0, 5 << 8}},
            {-1, -1, {ACTOR_EN_DEKUNUTS, -499,242,-2680, 0,-16512,0, 5 << 8}},
        } },
        {0x11, {
            {-1, -1, {ACTOR_EN_WALLMAS, 128,-779,-1658, 0,0,0, 0x0}},
            {-1, -1, {ACTOR_EN_WALLMAS, 128,-779,-1658, 0,0,0, 0x5 | (0x10 << 8)}},
            {-1, -1, {ACTOR_EN_WALLMAS, 128,-779,-1658, 0,0,0, 0x5 | (0x11 << 8)}},
            {-1, -1, {ACTOR_EN_WALLMAS, 128,-779,-1658, 0,0,0, 0x5 | (0x12 << 8)}},
        } },
        {0x0f, {
            {-1, -1, {ACTOR_EN_WALLMAS, 1830,404,-2700, 0,0,0, 0x2 | (0x20+11 << 8)}},
            {-1, -1, {ACTOR_EN_ST, 2069,720,-3002, 0,-32768,0, 0x5}}
        } },
    } },
    {SCENE_HIDAN, {
        {0x18, {
            {-1, 0, {ACTOR_EN_FD, -2928,2920,139, 0,0,0, 0x0}}
        } },
    } },
};

//Dynamic plant locations
/*
-250,-80,1130
1297,240,-553
-565,120,880
10,180,-10
5330,-190,-1720

Roling Goron locations
Scene: 0x62, Room: 0x3, Setup: 0x0
Entity 17	 ID: 0x1ae, 	Params: 0xfc00, 	pos: 520,399,565, 	0,-12925,0

Scene: 0x60, Room: 0x0, Setup: 0x0
Entity 40	 ID: 0x1ae, 	Params: 0xfc25, 	pos: -311,1500,-393, 	0,-4915,0

-154,1369,-1073
-273,1500,-403
-441,1460,-61
-673,1192,747
*/

bool isResourceRestored(auto val) {
   return val->second > gSaveContext.savedFrameCount;
}

bool Scene_CommandActorList(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetActorList* cmdActor = std::static_pointer_cast<LUS::SetActorList>(cmd);
    LUS::SetActorList* cmdActor = (LUS::SetActorList*)cmd;

    if (cmdActor->modificationState == 0) {
        if (sceneActorOverrides.find(play->sceneNum) != sceneActorOverrides.end() &&
                sceneActorOverrides.at(play->sceneNum).find(play->roomCtx.curRoom.num) != sceneActorOverrides.at(play->sceneNum).end()) {
            auto& roomOverrides = sceneActorOverrides.at(play->sceneNum).at(play->roomCtx.curRoom.num);
            for (auto& [setup, index, entry] : roomOverrides) {
                if (setup == -1 || setup == gSaveContext.sceneSetupIndex) {
                    if (index == -1) {
                        cmdActor->actorList.push_back(entry);
                    } else {
                        cmdActor->actorList[index] = entry;
                    }
                }
            }
            //setActorList->numActors = setActorList->actorList.size();
            //play->numSetupActors = cmdActor->entries.size();
        }

        cmdActor->modificationState = 1;
    }

    //play->numSetupActors = cmdActor->numActors;
    play->numSetupActors = cmdActor->actorList.size();

    play->setupActorList = (ActorEntry*)cmdActor->GetRawPointer();

    return false;
}

// bool Scene_CommandActorList(PlayState* play, Ship::SceneCommand* cmd) {
//     Ship::SetActorList* cmdActor = (Ship::SetActorList*)cmd;

//     play->numSetupActors = cmdActor->numActors;
//     //if (cmdActor->cachedGameData != nullptr)
//     //    play->setupActorList = (ActorEntry*)cmdActor->cachedGameData;
//     //else
//     {
//         if (cmdActor->cachedGameData == nullptr) {
//             if (sceneActorOverrides.find(play->sceneNum) != sceneActorOverrides.end() &&
//                             sceneActorOverrides.at(play->sceneNum).find(play->roomCtx.curRoom.num) != sceneActorOverrides.at(play->sceneNum).end()) {
//                 auto& roomOverrides = sceneActorOverrides.at(play->sceneNum).at(play->roomCtx.curRoom.num);
//                 for (auto& [setup, index, entry] : roomOverrides) {
//                     if (setup == -1 || setup == gSaveContext.sceneSetupIndex) {
//                         if (index == -1) {
//                             cmdActor->entries.push_back(entry);
//                         } else {
//                             cmdActor->entries[index] = entry;
//                         }
//                     }
//                 }
//                 play->numSetupActors = cmdActor->entries.size();
//             }
//         }

//         SPDLOG_INFO("Scene: 0x{0:x}, Room: 0x{1:x}, Setup: 0x{2:x}", (uint16_t)play->sceneNum, (uint16_t)play->roomCtx.curRoom.num, (uint32_t)gSaveContext.sceneSetupIndex);

//         ActorEntry* entries = (ActorEntry*)malloc(cmdActor->entries.size() * sizeof(ActorEntry));
//         TempResourceEntries = {};

//         for (int i = 0; i < cmdActor->entries.size(); i++)
//         {
//             entries[i].id = cmdActor->entries[i].actorNum;
//             entries[i].pos.x = cmdActor->entries[i].posX;
//             entries[i].pos.y = cmdActor->entries[i].posY;
//             entries[i].pos.z = cmdActor->entries[i].posZ;
//             entries[i].rot.x = cmdActor->entries[i].rotX;
//             entries[i].rot.y = cmdActor->entries[i].rotY;
//             entries[i].rot.z = cmdActor->entries[i].rotZ;
//             entries[i].params = cmdActor->entries[i].initVar;

//             if (entries[i].id == ACTOR_OBJ_TSUBO) {
//                 ActorSpawnResource sw;
//                 sw.scene = play->sceneNum;
//                 sw.room = play->roomCtx.curRoom.num;
//                 copyActorSpawn(sw.entry, cmdActor->entries[i]);
//                 sw.dirt = 0;
//                 TempResourceEntries.insert({i,sw});
//                 auto foundVal = UsedResources.find(sw);
//                 if (foundVal != UsedResources.end() && isResourceRestored(foundVal)) {
//                     entries[i].params &= 0xFFE0;
//                     entries[i].params |= ITEM00_MAX;
//                 }
//             } else if (entries[i].id == ACTOR_EN_WONDER_ITEM && DEKU_TREE_DEAD) {
//                 ActorSpawnResource sw;
//                 sw.scene = play->sceneNum;
//                 sw.room = play->roomCtx.curRoom.num;
//                 copyActorSpawn(sw.entry, cmdActor->entries[i]);
//                 sw.dirt = 0;
//                 TempResourceEntries.insert({i,sw});
//                 auto foundVal = UsedResources.find(sw);
//                 if (foundVal != UsedResources.end()) {
//                     entries[i].params &= 0x07FF;
//                     entries[i].params |= (0xA << 0xB);
//                 }
//             } else if (entries[i].id == ACTOR_EN_ITEM00 && DEKU_TREE_DEAD) {
//                 ActorSpawnResource sw;
//                 sw.scene = play->sceneNum;
//                 sw.room = play->roomCtx.curRoom.num;
//                 copyActorSpawn(sw.entry, cmdActor->entries[i]);
//                 sw.dirt = 0;
//                 TempResourceEntries.insert({i,sw});
//                 auto foundVal = UsedResources.find(sw);
//                 if (foundVal != UsedResources.end()) {
//                     entries[i].params &= 0xFF00;
//                     entries[i].params |= ITEM00_MAX;
//                 }
//             } else if (entries[i].id == ACTOR_EN_KUSA && (entries[i].params & 0x3) == 0) {
//                 // ActorSpawnResource sw;
//                 // sw.scene = play->sceneNum;
//                 // sw.room = play->roomCtx.curRoom.num;
//                 // copyActorSpawn(sw.entry, cmdActor->entries[i]);
//                 // sw.dirt = 0;
//                 // TempResourceEntries.insert({i,sw});
//                 // auto foundVal = UsedResources.find(sw);
//                 // if (foundVal != UsedResources.end()) {
//                 //     entries[i].params &= 0xFFFC;
//                 //     entries[i].params |= 3;
//                 // }
//             } else if (entries[i].id == ACTOR_OBJ_KIBAKO2) {
//                 ActorSpawnResource sw;
//                 sw.scene = play->sceneNum;
//                 sw.room = play->roomCtx.curRoom.num;
//                 copyActorSpawn(sw.entry, cmdActor->entries[i]);
//                 sw.dirt = 0;
//                 TempResourceEntries.insert({i,sw});
//                 auto foundVal = UsedResources.find(sw);
//                 if (foundVal != UsedResources.end()) {
//                     entries[i].rot.x = 0x1C;
//                 }
//             } else if (entries[i].id == ACTOR_EN_SKJ) {
//                 ActorSpawnResource sw;
//                 sw.scene = play->sceneNum;
//                 sw.room = play->roomCtx.curRoom.num;
//                 copyActorSpawn(sw.entry, cmdActor->entries[i]);
//                 sw.dirt = 0;
//                 TempResourceEntries.insert({i,sw});
//                 auto foundVal = UsedResources.find(sw);
//                 if (foundVal != UsedResources.end()) {
//                     entries[i].rot.z = 0x1;
//                 }
//             } else if (entries[i].id == ACTOR_EN_COW) {
//                 ActorSpawnResource sw;
//                 sw.scene = play->sceneNum;
//                 sw.room = play->roomCtx.curRoom.num;
//                 copyActorSpawn(sw.entry, cmdActor->entries[i]);
//                 sw.dirt = 0;
//                 TempResourceEntries.insert({i,sw});
//                 auto foundVal = UsedResources.find(sw);
//                 if (foundVal != UsedResources.end()) {
//                     entries[i].rot.z = 0x1;
//                 }
//             }

//             SPDLOG_INFO("Entity {0:d}\t ID: 0x{1:x}, \tParams: 0x{2:x}, \tpos: {3:d},{4:d},{5:d}, \t{6:d},{7:d},{8:d}",
//             i, (uint16_t)entries[i].id, (uint16_t)entries[i].params,
//             entries[i].pos.x, entries[i].pos.y, entries[i].pos.z, entries[i].rot.x, entries[i].rot.y, entries[i].rot.z);
//         }
//     }

//     return false;
// }

bool Scene_CommandUnused2(PlayState* play, LUS::ISceneCommand* cmd) {
    // OTRTODO: Do we need to implement this?
    // play->unk_11DFC = SEGMENTED_TO_VIRTUAL(cmd->unused02.segment);
    return false;
}

bool Scene_CommandCollisionHeader(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetCollisionHeader* cmdCol = std::static_pointer_cast<LUS::SetCollisionHeader>(cmd);
    LUS::SetCollisionHeader* cmdCol = (LUS::SetCollisionHeader*)cmd;
    BgCheck_Allocate(&play->colCtx, play, (CollisionHeader*)cmdCol->GetRawPointer());

    return false;
}

bool Scene_CommandRoomList(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetRoomList* cmdRoomList = std::static_pointer_cast<LUS::SetRoomList>(cmd);
    LUS::SetRoomList* cmdRoomList = (LUS::SetRoomList*)cmd;

    play->numRooms = cmdRoomList->numRooms;
    play->roomList = (RomFile*)cmdRoomList->GetRawPointer();

    return false;
}

bool Scene_CommandEntranceList(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetEntranceList* otrEntrance = std::static_pointer_cast<LUS::SetEntranceList>(cmd);
    LUS::SetEntranceList* otrEntrance = (LUS::SetEntranceList*)cmd;
    play->setupEntranceList = (EntranceEntry*)otrEntrance->GetRawPointer();

    return false;
}

bool Scene_CommandSpecialFiles(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetSpecialObjects* specialCmd = std::static_pointer_cast<LUS::SetSpecialObjects>(cmd);
    LUS::SetSpecialObjects* specialCmd = (LUS::SetSpecialObjects*)cmd;

    if (specialCmd->specialObjects.globalObject != 0) {
        play->objectCtx.subKeepIndex = Object_Spawn(&play->objectCtx, specialCmd->specialObjects.globalObject);
    }

    if (specialCmd->specialObjects.elfMessage != 0) {
        auto res = 
            (LUS::Blob*)OTRPlay_LoadFile(play, sNaviMsgFiles[specialCmd->specialObjects.elfMessage - 1].fileName);
        play->cUpElfMsgs = (ElfMessage*)res->Data.data();
    }

    return false;
}

bool Scene_CommandRoomBehavior(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetRoomBehavior* cmdRoom = std::static_pointer_cast<LUS::SetRoomBehavior>(cmd);
    LUS::SetRoomBehavior* cmdRoom = (LUS::SetRoomBehavior*)cmd;

    play->roomCtx.curRoom.behaviorType1 = cmdRoom->roomBehavior.gameplayFlags;
    play->roomCtx.curRoom.behaviorType2 = cmdRoom->roomBehavior.gameplayFlags2 & 0xFF;
    play->roomCtx.curRoom.lensMode = (cmdRoom->roomBehavior.gameplayFlags2 >> 8) & 1;
    play->msgCtx.disableWarpSongs = (cmdRoom->roomBehavior.gameplayFlags2 >> 0xA) & 1;

    return false;
}

bool Scene_CommandMeshHeader(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetMesh* otrMesh = static_pointer_cast<LUS::SetMesh>(cmd);
    LUS::SetMesh* otrMesh = (LUS::SetMesh*)cmd;
    play->roomCtx.curRoom.meshHeader = (MeshHeader*)otrMesh->GetRawPointer();

    return false;
}

// bool Scene_CommandCollisionHeader(PlayState* play, Ship::SceneCommand* cmd)
// {
//     Ship::SetCollisionHeader* cmdCol = (Ship::SetCollisionHeader*)cmd;

//     auto colRes = std::static_pointer_cast<Ship::CollisionHeader>(ResourceMgr_LoadResource(cmdCol->filePath.c_str()));

//     CollisionHeader* colHeader = nullptr;

//     if (colRes->CachedGameAsset != nullptr)
//         colHeader = (CollisionHeader*)colRes->CachedGameAsset;
//     else
//     {
//         colHeader = (CollisionHeader*)malloc(sizeof(CollisionHeader));

//         colHeader->minBounds.x = colRes->absMinX;
//         colHeader->minBounds.y = colRes->absMinY;
//         colHeader->minBounds.z = colRes->absMinZ;

//         colHeader->maxBounds.x = colRes->absMaxX;
//         colHeader->maxBounds.y = colRes->absMaxY;
//         colHeader->maxBounds.z = colRes->absMaxZ;

//         int extraVerts = 0;
//         if (play->sceneNum == SCENE_BESITU) {
//             extraVerts = 4;
//         }

//         colHeader->vtxList = (Vec3s*)malloc(sizeof(Vec3s) * (colRes->vertices.size()+extraVerts));
//         colHeader->numVertices = colRes->vertices.size()+extraVerts;

//         for (int i = 0; i < colRes->vertices.size(); i++)
//         {
//             colHeader->vtxList[i].x = colRes->vertices[i].x;
//             colHeader->vtxList[i].y = colRes->vertices[i].y;
//             colHeader->vtxList[i].z = colRes->vertices[i].z;

//             if (play->sceneNum == SCENE_BESITU) {
//                 // switch (i) {
//                 //     case 0: case 3: case 4: case 7:
//                 //     case 8: case 11: case 12: case 15:
//                 //     colHeader->vtxList[i].y -= 200;
//                 //     break;
//                 //     default:
//                 //     break;
//                 // }

//                 if (colHeader->vtxList[i].y == 0)
//                     colHeader->vtxList[i].y = -200;

//                 if (colHeader->vtxList[i].y > colHeader->maxBounds.y)
//                     colHeader->maxBounds.y = colHeader->vtxList[i].y;
//                 if (colHeader->vtxList[i].y < colHeader->minBounds.y)
//                     colHeader->minBounds.y = colHeader->vtxList[i].y;
//             }
//         }

//         size_t ss = colRes->vertices.size();

//         if (play->sceneNum == SCENE_BESITU) {
//             colHeader->vtxList[ss].x = -100; colHeader->vtxList[ss].y = 0; colHeader->vtxList[ss].z = 20;
//             colHeader->vtxList[ss+1].x = 100; colHeader->vtxList[ss+1].y = 0; colHeader->vtxList[ss+1].z = 20;
//             colHeader->vtxList[ss+2].x = -100; colHeader->vtxList[ss+2].y = 80; colHeader->vtxList[ss+2].z = 200;
//             colHeader->vtxList[ss+3].x = 100; colHeader->vtxList[ss+3].y = 80; colHeader->vtxList[ss+3].z = 200;
//         }

//         for (int ii = colRes->vertices.size(); ii < colRes->vertices.size()+extraVerts; ii++){
//             if (colHeader->vtxList[ii].x > colHeader->maxBounds.x)
//                 colHeader->maxBounds.x = colHeader->vtxList[ii].x;
//             if (colHeader->vtxList[ii].x < colHeader->minBounds.x)
//                 colHeader->minBounds.x = colHeader->vtxList[ii].x;
//             if (colHeader->vtxList[ii].y > colHeader->maxBounds.y)
//                 colHeader->maxBounds.y = colHeader->vtxList[ii].y;
//             if (colHeader->vtxList[ii].y < colHeader->minBounds.y)
//                 colHeader->minBounds.y = colHeader->vtxList[ii].y;
//             if (colHeader->vtxList[ii].z > colHeader->maxBounds.z)
//                 colHeader->maxBounds.z = colHeader->vtxList[ii].z;
//             if (colHeader->vtxList[ii].z < colHeader->minBounds.z)
//                 colHeader->minBounds.z = colHeader->vtxList[ii].z;
//         }

//         int extraPolys = 0;
//         if (play->sceneNum == SCENE_BESITU)
//             extraPolys = 2;
//         colHeader->polyList = (CollisionPoly*)malloc(sizeof(CollisionPoly) * (colRes->polygons.size()+extraPolys));
//         colHeader->numPolygons = colRes->polygons.size()+extraPolys;

//         for (int i = 0; i < colRes->polygons.size(); i++)
//         {
//             colHeader->polyList[i].type = colRes->polygons[i].type;
//             colHeader->polyList[i].flags_vIA = colRes->polygons[i].vtxA;
//             colHeader->polyList[i].flags_vIB = colRes->polygons[i].vtxB;
//             colHeader->polyList[i].vIC = colRes->polygons[i].vtxC;
//             colHeader->polyList[i].normal.x = colRes->polygons[i].a;
//             colHeader->polyList[i].normal.y = colRes->polygons[i].b;
//             colHeader->polyList[i].normal.z = colRes->polygons[i].c;
//             colHeader->polyList[i].dist = colRes->polygons[i].d;

//             // if (play->sceneNum == SCENE_BESITU && i >= 34 && i < 40) {
//             //     colHeader->polyList[i].dist += 200;
//             // }
//             if (play->sceneNum == SCENE_BESITU) {
//                 if (colHeader->vtxList[colHeader->polyList[i].flags_vIA].x != (s16)round(colRes->vertices[colHeader->polyList[i].flags_vIA].x) ||
//                     colHeader->vtxList[colHeader->polyList[i].flags_vIA].y != (s16)round(colRes->vertices[colHeader->polyList[i].flags_vIA].y) ||
//                     colHeader->vtxList[colHeader->polyList[i].flags_vIA].z != (s16)round(colRes->vertices[colHeader->polyList[i].flags_vIA].z) ||
//                     colHeader->vtxList[colHeader->polyList[i].flags_vIB].x != (s16)round(colRes->vertices[colHeader->polyList[i].flags_vIB].x) ||
//                     colHeader->vtxList[colHeader->polyList[i].flags_vIB].y != (s16)round(colRes->vertices[colHeader->polyList[i].flags_vIB].y) ||
//                     colHeader->vtxList[colHeader->polyList[i].flags_vIB].z != (s16)round(colRes->vertices[colHeader->polyList[i].flags_vIB].z) ||
//                     colHeader->vtxList[colHeader->polyList[i].vIC].x != (s16)round(colRes->vertices[colHeader->polyList[i].vIC].x) ||
//                     colHeader->vtxList[colHeader->polyList[i].vIC].y != (s16)round(colRes->vertices[colHeader->polyList[i].vIC].y) ||
//                     colHeader->vtxList[colHeader->polyList[i].vIC].z != (s16)round(colRes->vertices[colHeader->polyList[i].vIC].z)) {
//                         Vec3f a1, b1, c1, oNorm;
//                         f32 dist;
//                         a1.x = colHeader->vtxList[colHeader->polyList[i].flags_vIA].x;
//                         a1.y = colHeader->vtxList[colHeader->polyList[i].flags_vIA].y;
//                         a1.z = colHeader->vtxList[colHeader->polyList[i].flags_vIA].z;
//                         b1.x = colHeader->vtxList[colHeader->polyList[i].flags_vIB].x;
//                         b1.y = colHeader->vtxList[colHeader->polyList[i].flags_vIB].y;
//                         b1.z = colHeader->vtxList[colHeader->polyList[i].flags_vIB].z;
//                         c1.x = colHeader->vtxList[colHeader->polyList[i].vIC].x;
//                         c1.y = colHeader->vtxList[colHeader->polyList[i].vIC].y;
//                         c1.z = colHeader->vtxList[colHeader->polyList[i].vIC].z;

//                         // Math3D_Vec3f_Cross(&a1,&b1,&c1);
//                         oNorm.x = colHeader->polyList[i].normal.x;
//                         oNorm.y = colHeader->polyList[i].normal.y;
//                         oNorm.z = colHeader->polyList[i].normal.z;

//                         Math3D_DefPlane(&a1,&b1, &c1,
//                                         &c1.x,&c1.y,&c1.z,&dist);

//                         f32 sign = DOTXYZ(c1,oNorm);
//                         if (sign < 0) {
//                             c1.x = -c1.x;
//                             c1.y = -c1.y;
//                             c1.z = -c1.z;
//                             dist = -dist;
//                         }

//                         colHeader->polyList[i].normal.x = c1.x*0x7FFF;
//                         colHeader->polyList[i].normal.y = c1.y*0x7FFF;
//                         colHeader->polyList[i].normal.z = c1.z*0x7FFF;
//                         colHeader->polyList[i].dist = dist;
//                 }
//             }
//         }

//         size_t pp = colRes->polygons.size();
//         if (play->sceneNum == SCENE_BESITU) {
//             colHeader->polyList[pp].type = 0x0;
//             colHeader->polyList[pp].flags_vIA = ss; colHeader->polyList[pp].flags_vIB = ss+2; colHeader->polyList[pp].vIC = ss+1;
//             colHeader->polyList[pp+1].type = 0x1;
//             colHeader->polyList[pp+1].flags_vIA = ss+3; colHeader->polyList[pp+1].flags_vIB = ss+1; colHeader->polyList[pp+1].vIC = ss+2;
//         }

//         for (int ii = colRes->polygons.size(); ii < colHeader->numPolygons; ii++){
//             Vec3f a1, b1, c1, norm;
//             f32 dist;
//             a1.x = colHeader->vtxList[colHeader->polyList[ii].flags_vIA].x;
//             a1.y = colHeader->vtxList[colHeader->polyList[ii].flags_vIA].y;
//             a1.z = colHeader->vtxList[colHeader->polyList[ii].flags_vIA].z;
//             b1.x = colHeader->vtxList[colHeader->polyList[ii].flags_vIB].x;
//             b1.y = colHeader->vtxList[colHeader->polyList[ii].flags_vIB].y;
//             b1.z = colHeader->vtxList[colHeader->polyList[ii].flags_vIB].z;
//             c1.x = colHeader->vtxList[colHeader->polyList[ii].vIC].x;
//             c1.y = colHeader->vtxList[colHeader->polyList[ii].vIC].y;
//             c1.z = colHeader->vtxList[colHeader->polyList[ii].vIC].z;

//             Math3D_DefPlane(&a1,&b1, &c1,&norm.x,&norm.y,&norm.z,&dist);
//             colHeader->polyList[ii].normal.x = norm.x*0x7FFF;
//             colHeader->polyList[ii].normal.y = norm.y*0x7FFF;
//             colHeader->polyList[ii].normal.z = norm.z*0x7FFF;
//             colHeader->polyList[ii].dist = dist;
//         }

//         int extraSurfs = 0;
//         if (play->sceneNum == SCENE_BESITU)
//             extraSurfs = 1;
//         colHeader->surfaceTypeList = (SurfaceType*)malloc((colRes->polygonTypes.size()+extraSurfs) * sizeof(SurfaceType));

//         for (int i = 0; i < colRes->polygonTypes.size(); i++)
//         {
//             colHeader->surfaceTypeList[i].data[0] = colRes->polygonTypes[i] >> 32;
//             colHeader->surfaceTypeList[i].data[1] = colRes->polygonTypes[i] & 0xFFFFFFFF;

//             if ((play->sceneNum == SCENE_HAKAANA_OUKE && (i == 0x9 || i == 0xA)) ||
//                 (play->sceneNum == 0x8 && ((colHeader->surfaceTypeList[i].data[0] >> 13) == 2))) {//Well surfaces doing spike damage become voidouts
//                 colHeader->surfaceTypeList[i].data[0] |= 0x14 << 24;
//             }
//         }

//         if (play->sceneNum == SCENE_BESITU) {
//             colHeader->surfaceTypeList[colRes->polygonTypes.size()].data[0] = 0x100;
//             colHeader->surfaceTypeList[colRes->polygonTypes.size()].data[1] = 0x0;
//         }

//         colHeader->cameraDataList = (CamData*)malloc(sizeof(CamData) * colRes->camData->entries.size());
//         colHeader->cameraDataListLen = colRes->camData->entries.size();

//         for (int i = 0; i < colRes->camData->entries.size(); i++)
//         {
//             colHeader->cameraDataList[i].cameraSType = colRes->camData->entries[i]->cameraSType;
//             colHeader->cameraDataList[i].numCameras = colRes->camData->entries[i]->numData;

//             int idx = colRes->camData->entries[i]->cameraPosDataIdx;

//             colHeader->cameraDataList[i].camPosData = (Vec3s*)malloc(sizeof(Vec3s) * colRes->camData->entries[i]->numData);

//             for (int j = 0; j < colRes->camData->entries[i]->numData; j++)
//             {
//                 if (colRes->camData->cameraPositionData.size() > 0)
//                 {
//                     colHeader->cameraDataList[i].camPosData[j].x = colRes->camData->cameraPositionData[idx + j]->x;
//                     colHeader->cameraDataList[i].camPosData[j].y = colRes->camData->cameraPositionData[idx + j]->y;
//                     colHeader->cameraDataList[i].camPosData[j].z = colRes->camData->cameraPositionData[idx + j]->z;
//                 }
//                 else
//                 {
//                     colHeader->cameraDataList[i].camPosData->x = 0;
//                     colHeader->cameraDataList[i].camPosData->y = 0;
//                     colHeader->cameraDataList[i].camPosData->z = 0;
//                 }
//             }
//         }

//         colHeader->numWaterBoxes = colRes->waterBoxes.size();
//         colHeader->waterBoxes = (WaterBox*)malloc(sizeof(WaterBox) * colHeader->numWaterBoxes);

//         for (int i = 0; i < colHeader->numWaterBoxes; i++)
//         {
//             colHeader->waterBoxes[i].xLength = colRes->waterBoxes[i].xLength;
//             colHeader->waterBoxes[i].ySurface = colRes->waterBoxes[i].ySurface;
//             colHeader->waterBoxes[i].xMin = colRes->waterBoxes[i].xMin;
//             colHeader->waterBoxes[i].zMin = colRes->waterBoxes[i].zMin;
//             colHeader->waterBoxes[i].xLength = colRes->waterBoxes[i].xLength;
//             colHeader->waterBoxes[i].zLength = colRes->waterBoxes[i].zLength;
//             colHeader->waterBoxes[i].properties = colRes->waterBoxes[i].properties;
//         }

//         colRes->CachedGameAsset = colHeader;
//     }

//     BgCheck_Allocate(&play->colCtx, play, colHeader);

//     return false;
// }

extern "C" void* func_800982FC(ObjectContext* objectCtx, s32 bankIndex, s16 objectId);

//=======
//     play->numRooms = cmdRoomList->rooms.size();
//     play->roomList = (RomFile*)malloc(play->numRooms * sizeof(RomFile));

//     for (int i = 0; i < cmdRoomList->rooms.size(); i++)
//     {
//         play->roomList[i].fileName = (char*)cmdRoomList->rooms[i].name.c_str();
//         play->roomList[i].vromStart = cmdRoomList->rooms[i].vromStart;
//         play->roomList[i].vromEnd = cmdRoomList->rooms[i].vromEnd;
//     }

//     return false;
// }

// bool Scene_CommandEntranceList(PlayState* play, Ship::SceneCommand* cmd)
// {
//     Ship::SetEntranceList* otrEntrance = (Ship::SetEntranceList*)cmd;

//     if (otrEntrance->cachedGameData != nullptr)
//         play->setupEntranceList = (EntranceEntry*)otrEntrance->cachedGameData;
//     else
//     {
//         play->setupEntranceList = (EntranceEntry*)malloc(otrEntrance->entrances.size() * sizeof(EntranceEntry));
//         SPDLOG_INFO("Scene: 0x{0:x}", (uint16_t)play->sceneNum);

//         for (int i = 0; i < otrEntrance->entrances.size(); i++)
//         {
//             play->setupEntranceList[i].room = otrEntrance->entrances[i].roomToLoad;
//             play->setupEntranceList[i].spawn = otrEntrance->entrances[i].startPositionIndex;
//             SPDLOG_INFO("Entry {0:d}\t Spawn: 0x{1:x}, \tRoom: 0x{2:x}",
//             i, play->setupEntranceList[i].spawn, play->setupEntranceList[i].room);
//         }

//         otrEntrance->cachedGameData = play->setupEntranceList;
//     }

//     return false;
// }

// bool Scene_CommandSpecialFiles(PlayState* play, Ship::SceneCommand* cmd)
// {
//     Ship::SetSpecialObjects* otrSpecial = (Ship::SetSpecialObjects*)cmd;

//     if (otrSpecial->globalObject != 0)
//         play->objectCtx.subKeepIndex = Object_Spawn(&play->objectCtx, otrSpecial->globalObject);

//     if (otrSpecial->elfMessage != 0)
//     {
//         auto res = (Ship::Blob*)OTRPlay_LoadFile(play, sNaviMsgFiles[otrSpecial->elfMessage - 1].fileName);
//         play->cUpElfMsgs = (ElfMessage*)res->data.data();
//     }

//     return false;
// }

// bool Scene_CommandRoomBehavior(PlayState* play, Ship::SceneCommand* cmd)
// {
//     Ship::SetRoomBehavior* cmdRoom = (Ship::SetRoomBehavior*)cmd;

//     play->roomCtx.curRoom.behaviorType1 = cmdRoom->gameplayFlags;
//     play->roomCtx.curRoom.behaviorType2 = cmdRoom->gameplayFlags2 & 0xFF;
//     play->roomCtx.curRoom.lensMode = (cmdRoom->gameplayFlags2 >> 8) & 1;
//     play->msgCtx.disableWarpSongs = (cmdRoom->gameplayFlags2 >> 0xA) & 1;

//     return false;
// }

// bool Scene_CommandMeshHeader(PlayState* play, Ship::SceneCommand* cmd)
// {
//     Ship::SetMesh* otrMesh = (Ship::SetMesh*)cmd;
    
//     if (otrMesh->cachedGameData != nullptr)
//         play->roomCtx.curRoom.meshHeader = (MeshHeader*)otrMesh->cachedGameData;
//     else
//     {
//         play->roomCtx.curRoom.meshHeader = (MeshHeader*)malloc(sizeof(MeshHeader));
//         play->roomCtx.curRoom.meshHeader->base.type = otrMesh->meshHeaderType;
//         play->roomCtx.curRoom.meshHeader->polygon0.num = otrMesh->meshes.size();

//         if (otrMesh->meshHeaderType == 2)
//             play->roomCtx.curRoom.meshHeader->polygon0.start = malloc(sizeof(PolygonDlist2) * play->roomCtx.curRoom.meshHeader->polygon0.num);
//         else
//             play->roomCtx.curRoom.meshHeader->polygon0.start = malloc(sizeof(PolygonDlist) * play->roomCtx.curRoom.meshHeader->polygon0.num);

//         for (int i = 0; i < play->roomCtx.curRoom.meshHeader->polygon0.num; i++)
//         {
//             if (otrMesh->meshHeaderType == 2)
//             {
//                 PolygonDlist2* arr = (PolygonDlist2*)play->roomCtx.curRoom.meshHeader->polygon0.start;
//                 PolygonDlist2* dlist = &arr[i];

//                 if (otrMesh->meshes[i].opa != "")
//                 {
//                     auto opaFile =
//                         std::static_pointer_cast<Ship::DisplayList>(ResourceMgr_LoadResource(otrMesh->meshes[i].opa.c_str()));

//                     dlist->opaDL = opaFile.get();
//                     dlist->opa = (Gfx*)&dlist->opaDL->instructions[0];
//                 }
//                 else
//                 {
//                     dlist->opa = 0;
//                 }

//                 if (otrMesh->meshes[i].xlu != "")
//                 {
//                     auto xluFile =
//                         std::static_pointer_cast<Ship::DisplayList>(ResourceMgr_LoadResource(otrMesh->meshes[i].xlu.c_str()));

//                     dlist->xluDL = xluFile.get();
//                     dlist->xlu = (Gfx*)&dlist->xluDL->instructions[0];
//                 }
//                 else
//                     dlist->xlu = 0;

//                 dlist->pos.x = otrMesh->meshes[i].x;
//                 dlist->pos.y = otrMesh->meshes[i].y;
//                 dlist->pos.z = otrMesh->meshes[i].z;
//                 dlist->unk_06 = otrMesh->meshes[i].unk_06;

//                 //play->roomCtx.curRoom.meshHeader->base.start = dlist;
//             }
//             else if (otrMesh->meshHeaderType == 1)
//             {
//                 PolygonDlist* pType = (PolygonDlist*)malloc(sizeof(PolygonDlist));

//                 if (otrMesh->meshes[0].imgOpa != "")
//                     pType->opa = (Gfx*)&std::static_pointer_cast<Ship::DisplayList>(ResourceMgr_LoadResource(otrMesh->meshes[0].imgOpa.c_str()))->instructions[0];
//                 else
//                     pType->opa = 0;

//                 if (otrMesh->meshes[0].imgXlu != "")
//                     pType->xlu = (Gfx*)&std::static_pointer_cast<Ship::DisplayList>(ResourceMgr_LoadResource(otrMesh->meshes[0].imgXlu.c_str()))->instructions[0];
//                 else
//                     pType->xlu = 0;

//                 play->roomCtx.curRoom.meshHeader->polygon1.dlist = (Gfx*)pType;

//                 play->roomCtx.curRoom.meshHeader->polygon1.format = otrMesh->meshes[0].imgFmt;

//                 if (otrMesh->meshes[0].imgFmt == 1)
//                 {
//                     play->roomCtx.curRoom.meshHeader->polygon1.single.fmt = otrMesh->meshes[0].images[0].fmt;
//                     play->roomCtx.curRoom.meshHeader->polygon1.single.source =
//                         (void*)(ResourceMgr_LoadFile(otrMesh->meshes[0].images[0].sourceBackground.c_str()))
//                         .get()
//                         ->Buffer.get();
//                     play->roomCtx.curRoom.meshHeader->polygon1.single.siz = otrMesh->meshes[0].images[0].siz;
//                     play->roomCtx.curRoom.meshHeader->polygon1.single.width = otrMesh->meshes[0].images[0].width;
//                     play->roomCtx.curRoom.meshHeader->polygon1.single.height = otrMesh->meshes[0].images[0].height;
//                     play->roomCtx.curRoom.meshHeader->polygon1.single.fmt = otrMesh->meshes[0].images[0].fmt;
//                     play->roomCtx.curRoom.meshHeader->polygon1.single.mode0 = otrMesh->meshes[0].images[0].mode0;
//                     play->roomCtx.curRoom.meshHeader->polygon1.single.tlutCount = otrMesh->meshes[0].images[0].tlutCount;
//                 }
//                 else
//                 {
//                     play->roomCtx.curRoom.meshHeader->polygon1.multi.count = otrMesh->meshes[0].images.size();
//                     play->roomCtx.curRoom.meshHeader->polygon1.multi.list =
//                         (BgImage*)calloc(sizeof(BgImage), play->roomCtx.curRoom.meshHeader->polygon1.multi.count);

//                     for (size_t i = 0; i < otrMesh->meshes[0].images.size(); i++)
//                     {
//                         play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].fmt = otrMesh->meshes[0].images[i].fmt;
//                         play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].source =
//                             (void*)(ResourceMgr_LoadFile(otrMesh->meshes[0].images[i].sourceBackground.c_str()))
//                             .get()
//                             ->Buffer.get();
//                         play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].siz = otrMesh->meshes[0].images[i].siz;
//                         play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].width = otrMesh->meshes[0].images[i].width;
//                         play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].height =
//                             otrMesh->meshes[0].images[i].height;
//                         play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].fmt = otrMesh->meshes[0].images[i].fmt;
//                         play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].mode0 = otrMesh->meshes[0].images[i].mode0;
//                         play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].tlutCount =
//                             otrMesh->meshes[0].images[i].tlutCount;
//                         play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].unk_00 =
//                             otrMesh->meshes[0].images[i].unk_00;
//                         play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].unk_0C =
//                             otrMesh->meshes[0].images[i].unk_0C;
//                         play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].id = otrMesh->meshes[0].images[i].id;
//                     }
//                 }
//             }
//             else
//             {
//                 PolygonDlist* arr = (PolygonDlist*)play->roomCtx.curRoom.meshHeader->polygon0.start;
//                 PolygonDlist* dlist = &arr[i];

//                 if (otrMesh->meshes[i].opa != "")
//                 {
//                     auto opaFile = std::static_pointer_cast<Ship::DisplayList>(ResourceMgr_LoadResource(otrMesh->meshes[i].opa.c_str()));

//                     dlist->opaDL = opaFile.get();
//                     dlist->opa = (Gfx*)&dlist->opaDL->instructions[0];
//                 }
//                 else
//                     dlist->opa = 0;

//                 if (otrMesh->meshes[i].xlu != "")
//                 {
//                     auto xluFile = std::static_pointer_cast<Ship::DisplayList>(ResourceMgr_LoadResource(otrMesh->meshes[i].xlu.c_str()));

//                     dlist->xluDL = xluFile.get();
//                     dlist->xlu = (Gfx*)&dlist->xluDL->instructions[0];
//                 }
//                 else
//                     dlist->xlu = 0;

//                 //play->roomCtx.curRoom.meshHeader->base.start = dlist;
//             }
//         }

//         otrMesh->cachedGameData = play->roomCtx.curRoom.meshHeader;
//     }

//     return false;
// }

// extern "C" void* func_800982FC(ObjectContext * objectCtx, s32 bankIndex, s16 objectId);

// bool Scene_CommandObjectList(PlayState* play, Ship::SceneCommand* cmd)
// {
//     Ship::SetObjectList* cmdObj = (Ship::SetObjectList*)cmd;
// >>>>>>> andrewvc/s-edition-p2

bool Scene_CommandObjectList(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetObjectList* cmdObj = static_pointer_cast<LUS::SetObjectList>(cmd);
    LUS::SetObjectList* cmdObj = (LUS::SetObjectList*)cmd;
    
    s32 i;
    s32 j;
    s32 k;
    ObjectStatus* status;
    ObjectStatus* status2;
    ObjectStatus* firstStatus;
    // s16* objectEntry = SEGMENTED_TO_VIRTUAL(cmd->objectList.segment);
    s16* objectEntry = (s16*)cmdObj->GetRawPointer();
    void* nextPtr;

    k = 0;
    // i = play->objectCtx.unk_09;
    i = 0;
    firstStatus = &play->objectCtx.status[0];
    status = &play->objectCtx.status[i];

    for (int i = 0; i < cmdObj->objects.size(); i++) {
        bool alreadyIncluded = false;

        for (int j = 0; j < play->objectCtx.num; j++) {
            if (play->objectCtx.status[j].id == cmdObj->objects[i]) {
                alreadyIncluded = true;
                break;
            }
        }

        if (!alreadyIncluded) {
            play->objectCtx.status[play->objectCtx.num++].id = cmdObj->objects[i];
            func_80031A28(play, &play->actorCtx);
        }
    }

    /*
    while (i < play->objectCtx.num) {
        if (status->id != *objectEntry) {
            status2 = &play->objectCtx.status[i];
            for (j = i; j < play->objectCtx.num; j++) {
                status2->id = OBJECT_INVALID;
                status2++;
            }
            play->objectCtx.num = i;
            func_80031A28(play, &play->actorCtx);

            continue;
        }

        i++;
        k++;
        objectEntry++;
        status++;
    }

    play->objectCtx.num = i;
    */

    return false;
}

bool Scene_CommandLightList(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetLightList* cmdLight = static_pointer_cast<LUS::SetLightList>(cmd);
    LUS::SetLightList* cmdLight = (LUS::SetLightList*)cmd;

    for (size_t i = 0; i < cmdLight->lightList.size(); i++) {
        LightContext_InsertLight(play, &play->lightCtx, (LightInfo*)&cmdLight->lightList[i]);
    }

    return false;
}

bool Scene_CommandPathList(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetPathways* cmdPath = static_pointer_cast<LUS::SetPathways>(cmd);
    LUS::SetPathways* cmdPath = (LUS::SetPathways*)cmd;
    play->setupPathList = (Path*)(cmdPath->GetPointer()[0]);

    return false;
}

const std::map<u16,  std::vector<std::tuple<int, int, LUS::TransitionActorEntry>>> sceneTransitionActorOverrides = {
    { 0x5B, { // Lost Woods
        { -1, 9, { 1,255, 0,255,   ACTOR_EN_HOLL, 0,0,-400,  0, 0x3F }},
        { -1, 10, { 1,255, 0,255,   ACTOR_EN_HOLL, 0,0, 400,  -32768, 0x3F }},
        { -1, 11, { 0,255, 1,255,   ACTOR_EN_HOLL, 800,0, 400,  -32768, 0x3F }},
        { -1, 12, { 1,255, 2,255,   ACTOR_EN_HOLL, 400,0, -800,  16384, 0x3F }},
        { -1, 13, { 2,255, 3,255,   ACTOR_EN_HOLL, 1600,0, -400,  -32768, 0x3F }},
        { -1, 14, { 7,255, 4,255,   ACTOR_EN_HOLL, 2000,0, -1600,  -16384, 0x3F }},
        { -1, 15, { 7,255, 8,255,   ACTOR_EN_HOLL, 2000,0, -2400,  16384, 0x3F | (0x7 << 6) }},
        { -1, 16, { 8,255, 7,255,   ACTOR_EN_HOLL, 1600,0, -2800,  0, 0x3F }},
        { -1, 17, { 8,255, 7,255,   ACTOR_EN_HOLL, 400,0, -2400,  -16384, 0x3F | (0x7 << 6) }},
        { -1, 18, { 7,255, 8,255,   ACTOR_EN_HOLL, 800,0, -2800,  0, 0x3F }},
        { -1, 19, { 7,255, 8,255,   ACTOR_EN_HOLL, 800,0, -2000,  -32768, 0x3F | (0x1 << 9)}},
    } },
};

bool Scene_CommandTransitionActorList(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetTransitionActorList* cmdActor = static_pointer_cast<LUS::SetTransitionActorList>(cmd);
    LUS::SetTransitionActorList* cmdActor = (LUS::SetTransitionActorList*)cmd;

    if (cmdActor->modificationState == 0) {
        if (sceneTransitionActorOverrides.find(play->sceneNum) != sceneTransitionActorOverrides.end()) {
            auto& roomOverrides = sceneTransitionActorOverrides.at(play->sceneNum);
            for (auto& [setup, index, entry] : roomOverrides) {
                if (setup == -1 || setup == gSaveContext.sceneSetupIndex) {
                    if (index == -1) {
                        cmdActor->transitionActorList.push_back(entry);
                    } else {
                        if (index < cmdActor->transitionActorList.size())
                            cmdActor->transitionActorList[index] = entry;
                        else
                            cmdActor->transitionActorList.emplace(cmdActor->transitionActorList.begin()+index,entry);
                    }
                }
            }
        }

        cmdActor->modificationState = 1;
    }

    play->transiActorCtx.numActors = cmdActor->transitionActorList.size(); //cmdActor->numTransitionActors;
    play->transiActorCtx.list = (TransitionActorEntry*)cmdActor->GetRawPointer();

    return false;
}

//Keeps track of if extra transition actors have been loaded already, avoids multi loading them if this is the case
//std::map<u16, bool> sceneTransitionLoadFlags = {
//};

// void TransitionActor_InitContext(GameState* state, TransitionActorContext* transiActorCtx) {
//    transiActorCtx->numActors = 0;
//}

bool Scene_CommandLightSettingsList(PlayState* play, LUS::ISceneCommand* cmd) {
    play->envCtx.lightSettingsList = (EnvLightSettings*)cmd->GetRawPointer();

    return false;
}

// Scene Command 0x11: Skybox Settings
bool Scene_CommandSkyboxSettings(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetSkyboxSettings* cmdSky = static_pointer_cast<LUS::SetSkyboxSettings>(cmd);
    LUS::SetSkyboxSettings* cmdSky = (LUS::SetSkyboxSettings*)cmd;

    play->skyboxId = cmdSky->settings.skyboxId;
    play->envCtx.unk_17 = play->envCtx.unk_18 = cmdSky->settings.weather;
    play->envCtx.indoors = cmdSky->settings.indoors;

    return false;
}

bool Scene_CommandSkyboxDisables(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetSkyboxModifier* cmdSky = static_pointer_cast<LUS::SetSkyboxModifier>(cmd);
    LUS::SetSkyboxModifier* cmdSky = (LUS::SetSkyboxModifier*)cmd;

    play->envCtx.sunMoonDisabled = cmdSky->modifier.sunMoonDisabled;
    play->envCtx.skyboxDisabled = cmdSky->modifier.skyboxDisabled;

    return false;
}

bool Scene_CommandTimeSettings(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetTimeSettings* cmdTime = static_pointer_cast<LUS::SetTimeSettings>(cmd);
    LUS::SetTimeSettings* cmdTime = (LUS::SetTimeSettings*)cmd;

    if ((cmdTime->settings.hour != 0xFF) && (cmdTime->settings.minute != 0xFF)) {
        gSaveContext.skyboxTime = gSaveContext.dayTime =
            ((cmdTime->settings.hour + (cmdTime->settings.minute / 60.0f)) * 60.0f) / ((f32)(24 * 60) / 0x10000);
    }

    if (cmdTime->settings.timeIncrement != 0xFF) {
        play->envCtx.timeIncrement = cmdTime->settings.timeIncrement;
    } else {
        play->envCtx.timeIncrement = 0;
    }

    if (gSaveContext.sunsSongState == SUNSSONG_INACTIVE) {
        gTimeIncrement = play->envCtx.timeIncrement;
    }

    play->envCtx.sunPos.x = -(Math_SinS(((void)0, gSaveContext.dayTime) - 0x8000) * 120.0f) * 25.0f;
    play->envCtx.sunPos.y = (Math_CosS(((void)0, gSaveContext.dayTime) - 0x8000) * 120.0f) * 25.0f;
    play->envCtx.sunPos.z = (Math_CosS(((void)0, gSaveContext.dayTime) - 0x8000) * 20.0f) * 25.0f;

    if (((play->envCtx.timeIncrement == 0) && (gSaveContext.cutsceneIndex < 0xFFF0)) ||
        (gSaveContext.entranceIndex == 0x0604)) {
        gSaveContext.skyboxTime = ((void)0, gSaveContext.dayTime);
        if ((gSaveContext.skyboxTime >= 0x2AAC) && (gSaveContext.skyboxTime < 0x4555)) {
            gSaveContext.skyboxTime = 0x3556;
        } else if ((gSaveContext.skyboxTime >= 0x4555) && (gSaveContext.skyboxTime < 0x5556)) {
            gSaveContext.skyboxTime = 0x5556;
        } else if ((gSaveContext.skyboxTime >= 0xAAAB) && (gSaveContext.skyboxTime < 0xB556)) {
            gSaveContext.skyboxTime = 0xB556;
        } else if ((gSaveContext.skyboxTime >= 0xC001) && (gSaveContext.skyboxTime < 0xCAAC)) {
            gSaveContext.skyboxTime = 0xCAAC;
        }
    }

    return false;
}

bool Scene_CommandWindSettings(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetWind* cmdWind = std::static_pointer_cast<LUS::SetWind>(cmd);
    LUS::SetWindSettings* cmdWind = (LUS::SetWindSettings*)cmd;

    play->envCtx.windDirection.x = cmdWind->settings.windWest;
    play->envCtx.windDirection.y = cmdWind->settings.windVertical;
    play->envCtx.windDirection.z = cmdWind->settings.windSouth;

    play->envCtx.windSpeed = cmdWind->settings.windSpeed;

    return false;
}

bool Scene_CommandExitList(PlayState* play, LUS::ISceneCommand* cmd) {
    play->setupExitList = (s16*)cmd->GetRawPointer();

    return false;
}

bool Scene_CommandUndefined9(PlayState* play, LUS::ISceneCommand* cmd) {
    return false;
}

bool Scene_CommandSoundSettings(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetSoundSettings* cmdSnd = static_pointer_cast<LUS::SetSoundSettings>(cmd);
    LUS::SetSoundSettings* cmdSnd = (LUS::SetSoundSettings*)cmd;

    play->sequenceCtx.seqId = cmdSnd->settings.seqId;
    play->sequenceCtx.natureAmbienceId = cmdSnd->settings.natureAmbienceId;

    if (gSaveContext.seqId == 0xFF) {
        Audio_QueueSeqCmd(cmdSnd->settings.reverb | 0xF0000000);
    }

    return false;
}

bool Scene_CommandEchoSettings(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetEchoSettings* cmdEcho = static_pointer_cast<LUS::SetEchoSettings>(cmd);
    LUS::SetEchoSettings* cmdEcho = (LUS::SetEchoSettings*)cmd;

    play->roomCtx.curRoom.echo = cmdEcho->settings.echo;

    return false;
}

bool Scene_CommandAlternateHeaderList(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetAlternateHeaders* cmdHeaders = static_pointer_cast<LUS::SetAlternateHeaders>(cmd);
    LUS::SetAlternateHeaders* cmdHeaders = (LUS::SetAlternateHeaders*)cmd;

    // s32 pad;
    // SceneCmd* altHeader;

    // osSyncPrintf("\n[ZU]sceneset age    =[%X]", ((void)0, gSaveContext.linkAge));
    // osSyncPrintf("\n[ZU]sceneset time   =[%X]", ((void)0, gSaveContext.cutsceneIndex));
    // osSyncPrintf("\n[ZU]sceneset counter=[%X]", ((void)0, gSaveContext.sceneSetupIndex));

    if (gSaveContext.sceneSetupIndex != 0) {
        LUS::Scene* desiredHeader =
            std::static_pointer_cast<LUS::Scene>(cmdHeaders->headers[gSaveContext.sceneSetupIndex - 1]).get();

        if (desiredHeader != nullptr) {
            OTRScene_ExecuteCommands(play, desiredHeader);
            return true;
        } else {
            // "Coughh! There is no specified dataaaaa!"
            osSyncPrintf("\nげぼはっ！ 指定されたデータがないでええっす！");

            if (gSaveContext.sceneSetupIndex == 3) {
                LUS::Scene* desiredHeader =
                    std::static_pointer_cast<LUS::Scene>(cmdHeaders->headers[gSaveContext.sceneSetupIndex - 2]).get();

                // "Using adult day data there!"
                osSyncPrintf("\nそこで、大人の昼データを使用するでええっす！！");

                if (desiredHeader != nullptr) {
                    OTRScene_ExecuteCommands(play, desiredHeader);
                    return true;
                }
            }
        }
    }
    return false;
}

bool Scene_CommandCutsceneData(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetCutscenes* cmdCS = std::static_pointer_cast<LUS::SetCutscenes>(cmd);
    LUS::SetCutscenes* cmdCS = (LUS::SetCutscenes*)cmd;

    play->csCtx.segment = cmdCS->cutscene->commands.data();

    // osSyncPrintf("\ngame_play->demo_play.data=[%x]", play->csCtx.segment);
    return false;
}

// Camera & World Map Area
bool Scene_CommandMiscSettings(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetCameraSettings* cmdCam = std::static_pointer_cast<LUS::SetCameraSettings>(cmd);
    LUS::SetCameraSettings* cmdCam = (LUS::SetCameraSettings*)cmd;

    YREG(15) = cmdCam->settings.cameraMovement;
    gSaveContext.worldMapArea = cmdCam->settings.worldMapArea;

    if ((play->sceneNum == SCENE_SHOP1) || (play->sceneNum == SCENE_SYATEKIJYOU)) {
        if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
            gSaveContext.worldMapArea = 1;
        }
    }

    if (((play->sceneNum >= SCENE_SPOT00) && (play->sceneNum <= SCENE_GANON_TOU)) ||
        ((play->sceneNum >= SCENE_ENTRA) && (play->sceneNum <= SCENE_SHRINE_R))) {
        if (gSaveContext.cutsceneIndex < 0xFFF0) {
            gSaveContext.worldMapAreaData |= gBitFlags[gSaveContext.worldMapArea];
            osSyncPrintf("０００  ａｒｅａ＿ａｒｒｉｖａｌ＝%x (%d)\n", gSaveContext.worldMapAreaData,
                gSaveContext.worldMapArea);
        }
    }
    return false;
}

bool (*sceneCommands[])(PlayState*, LUS::ISceneCommand*) = {
    Scene_CommandSpawnList,           // SCENE_CMD_ID_SPAWN_LIST
    Scene_CommandActorList,           // SCENE_CMD_ID_ACTOR_LIST
    Scene_CommandUnused2,             // SCENE_CMD_ID_UNUSED_2
    Scene_CommandCollisionHeader,     // SCENE_CMD_ID_COLLISION_HEADER
    Scene_CommandRoomList,            // SCENE_CMD_ID_ROOM_LIST
    Scene_CommandWindSettings,        // SCENE_CMD_ID_WIND_SETTINGS
    Scene_CommandEntranceList,        // SCENE_CMD_ID_ENTRANCE_LIST
    Scene_CommandSpecialFiles,        // SCENE_CMD_ID_SPECIAL_FILES
    Scene_CommandRoomBehavior,        // SCENE_CMD_ID_ROOM_BEHAVIOR
    Scene_CommandUndefined9,          // SCENE_CMD_ID_UNDEFINED_9
    Scene_CommandMeshHeader,          // SCENE_CMD_ID_MESH_HEADER
    Scene_CommandObjectList,          // SCENE_CMD_ID_OBJECT_LIST
    Scene_CommandLightList,           // SCENE_CMD_ID_LIGHT_LIST
    Scene_CommandPathList,            // SCENE_CMD_ID_PATH_LIST
    Scene_CommandTransitionActorList, // SCENE_CMD_ID_TRANSITION_ACTOR_LIST
    Scene_CommandLightSettingsList,   // SCENE_CMD_ID_LIGHT_SETTINGS_LIST
    Scene_CommandTimeSettings,        // SCENE_CMD_ID_TIME_SETTINGS
    Scene_CommandSkyboxSettings,      // SCENE_CMD_ID_SKYBOX_SETTINGS
    Scene_CommandSkyboxDisables,      // SCENE_CMD_ID_SKYBOX_DISABLES
    Scene_CommandExitList,            // SCENE_CMD_ID_EXIT_LIST
    NULL,                             // SCENE_CMD_ID_END
    Scene_CommandSoundSettings,       // SCENE_CMD_ID_SOUND_SETTINGS
    Scene_CommandEchoSettings,        // SCENE_CMD_ID_ECHO_SETTINGS
    Scene_CommandCutsceneData,        // SCENE_CMD_ID_CUTSCENE_DATA
    Scene_CommandAlternateHeaderList, // SCENE_CMD_ID_ALTERNATE_HEADER_LIST
    Scene_CommandMiscSettings,        // SCENE_CMD_ID_MISC_SETTINGS
};

s32 OTRScene_ExecuteCommands(PlayState* play, LUS::Scene* scene) {
    LUS::SceneCommandID cmdCode;

    for (int i = 0; i < scene->commands.size(); i++) {
        auto sceneCmd = scene->commands[i];

        if (sceneCmd == nullptr) // UH OH
            continue;

        cmdCode = sceneCmd->cmdId;
        // osSyncPrintf("*** Scene_Word = { code=%d, data1=%02x, data2=%04x } ***\n", cmdCode, sceneCmd->base.data1, sceneCmd->base.data2);

        if ((int)cmdCode == 0x14) {
            break;
        }

        if ((int)cmdCode <= 0x19) {
            if (sceneCommands[(int)cmdCode](play, sceneCmd.get()))
                break;
        } else {
            osSyncPrintf(VT_FGCOL(RED));
            osSyncPrintf("code の値が異常です\n"); // "code variable is abnormal"
            osSyncPrintf(VT_RST);
        }

        // sceneCmd++;
    }

    if (play->sceneNum == SCENE_BESITU) {
        play->setupExitList = (int16_t*)malloc((1) * sizeof(int16_t));
        play->setupExitList[0] = 0;
        play->numSetupActors = 2;
        play->setupActorList = (ActorEntry*)malloc(play->numSetupActors * sizeof(ActorEntry));
        play->setupActorList[0] = { ACTOR_EN_WONDER_ITEM, -79,0,-151, 0,0,1, 0x1241};
        play->setupActorList[1] = { ACTOR_EN_ITEM00, 200,-200,0, 0,0,0, 0x100+(uint16_t)ITEM00_HEART_PIECE  };
    }

    return 0;
}

extern "C" s32 OTRfunc_800973FC(PlayState* play, RoomContext* roomCtx) {
    if (roomCtx->status == 1) {
        // if (!osRecvMesg(&roomCtx->loadQueue, NULL, OS_MESG_NOBLOCK)) {
        if (1) {
            roomCtx->status = 0;
            roomCtx->curRoom.segment = roomCtx->unk_34;
            gSegments[3] = VIRTUAL_TO_PHYSICAL(roomCtx->unk_34);

            OTRScene_ExecuteCommands(play, (LUS::Scene*)roomCtx->roomToLoad);
            Player_SetBootData(play, GET_PLAYER(play));
            Actor_SpawnTransitionActors(play, &play->actorCtx);

            return 1;
        }

        return 0;
    }

    return 1;
}

extern "C" s32 OTRfunc_8009728C(PlayState* play, RoomContext* roomCtx, s32 roomNum) {
    u32 size;

    if (roomCtx->status == 0) {
        roomCtx->prevRoom = roomCtx->curRoom;
        roomCtx->curRoom.num = roomNum;
        roomCtx->curRoom.segment = NULL;
        roomCtx->status = 1;

        ASSERT(roomNum < play->numRooms);

        if (roomNum >= play->numRooms)
            return 0; // UH OH

        size = play->roomList[roomNum].vromEnd - play->roomList[roomNum].vromStart;
        roomCtx->unk_34 =
            (void*)ALIGN16((uintptr_t)roomCtx->bufPtrs[roomCtx->unk_30] - ((size + 8) * roomCtx->unk_30 + 7));

        osCreateMesgQueue(&roomCtx->loadQueue, &roomCtx->loadMsg, 1);
        // DmaMgr_SendRequest2(&roomCtx->dmaRequest, roomCtx->unk_34, play->roomList[roomNum].vromStart, size, 0,
                            //&roomCtx->loadQueue, NULL, __FILE__, __LINE__);

        auto roomData =
            std::static_pointer_cast<LUS::Scene>(GetResourceByNameHandlingMQ(play->roomList[roomNum].fileName));
        roomCtx->status = 1;
        roomCtx->roomToLoad = roomData.get();

        roomCtx->unk_30 ^= 1;

        return 1;
    }

    return 0;
}
