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

bool operator==(const LUS::ActorEntry& lhs, const LUS::ActorEntry& rhs)
{
    return (lhs.id == rhs.id)
    && (lhs.pos.x == rhs.pos.x)
    && (lhs.pos.y == rhs.pos.y)
    && (lhs.pos.z == rhs.pos.z)
    && (lhs.rot.x == rhs.rot.x)
    && (lhs.rot.y == rhs.rot.y)
    && (lhs.rot.z == rhs.rot.z)
    && (lhs.params == rhs.params);
}

void copyActorSpawn(ActorSpawnEnt& lhs, const LUS::ActorEntry& rhs) {
    lhs.actorNum = rhs.id;
    lhs.posX = rhs.pos.x;
    lhs.posY = rhs.pos.y;
    lhs.posZ = rhs.pos.z;
    lhs.rotX = rhs.rot.x;
    lhs.rotY = rhs.rot.y;
    lhs.rotZ = rhs.rot.z;
    lhs.initVar = rhs.params;
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
    j.at("initVar").get_to(p.entry.initVar);
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

bool isResourceYetToRestore(auto val) {
   return val->second > gSaveContext.savedFrameCount;
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
    if (foundVal != UsedResources.end() && isResourceYetToRestore(foundVal)) {
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
            { -1, -1, { ACTOR_EN_ZF, -1925,2800,780, 0,24394,0, static_cast<int16_t>(0xFFFF) }},
            { -1, -1, { ACTOR_EN_ZF, -1740,2800,-1060, 0,24394,0, static_cast<int16_t>(0xFFFF) }},
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
            { 0, -1, { ACTOR_EN_WONDER_TALK2, -92,25,-8, 0,15350,0, static_cast<int16_t>(0x8000|(0x3D<<6)|0x3f) }},
            { 1, -1, { ACTOR_EN_WONDER_TALK2, -92,25,-8, 0,15350,0, static_cast<int16_t>(0x8000|(0x41<<6)|0x3f) }},
            { 2, -1, { ACTOR_EN_WONDER_TALK2, -92,25,-8, 0,15350,0, static_cast<int16_t>(0x8000|(0x3E<<6)|0x3f) }},
            { 3, -1, { ACTOR_EN_WONDER_TALK2, -92,25,-8, 0,15350,0, static_cast<int16_t>(0x8000|(0x3E<<6)|0x3f) }},
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
            { -1, -1, { ACTOR_EN_KO, -1150,-360,1152, 0,1200,0, static_cast<int16_t>(0xff00) }},
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
            { 2, -1, { ACTOR_EN_HORSE, 100,0,2000, 0,0,0, static_cast<int16_t>(0x800B)} }, { 3, -1, { ACTOR_EN_HORSE, 100,0,2000, 0,0,0, static_cast<int16_t>(0x800B)}},
            {2, -1, { ACTOR_EN_MB, -4010,-300,1700, 0,8104,0, static_cast<int16_t>(0xffff)}},
            {3, -1, { ACTOR_EN_MB, -4010,-300,1700, 0,8104,0, static_cast<int16_t>(0xffff)}},
            {2, -1, { ACTOR_EN_MB, -3157,-300,945, 0,8104,0, static_cast<int16_t>(0xffff)}},
            {3, -1, { ACTOR_EN_MB, -3157,-300,945, 0,8104,0, static_cast<int16_t>(0xffff)}},
            {2, -1, { ACTOR_EN_MB, -5245,-300,2000, 0,-1321,0, static_cast<int16_t>(0xffff)}},
            {3, -1, { ACTOR_EN_MB, -5245,-300,2000, 0,-1321,0, static_cast<int16_t>(0xffff)}},
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
            { -1, -1, { ACTOR_EN_WONDER_TALK2, 142,20,210, 0,15350,0, static_cast<int16_t>(0x8000|(0x3F<<6)|0x3f) }},
            { -1, -1, { ACTOR_EN_WONDER_TALK2, 142,20,67,  0,15350,0, static_cast<int16_t>(0x8000|(0x40<<6)|0x3f) }},
        } },
    } },
    { 0x52, { // Kakariko
        { 0x00, {
            //{ -1, 18, { 0x95, -18,800,1800, 0,-32768,0, 0xb140 }},
            { 1, 24, { ACTOR_EN_SW, 5,755,-100, 0,0,0, static_cast<int16_t>(0xB104) }},
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
            { 0, 57, { ACTOR_EN_MS, -582,375,-438, 0,16384,0, static_cast<int16_t>(0xFFFF) }},
            { 0, 60, { ACTOR_OBJ_BOMBIWA, 838,210,-194, 0,-20771,1, 0x0002 }},
            { 0, 62, { ACTOR_OBJ_BOMBIWA, -464,375,-558, 0,-25850,1, 0x0008 }},
            { 0, 63, { ACTOR_OBJ_BOMBIWA, -533,375,-594, 0,16384,1, 0x0009 }},
            { 0, 64, { ACTOR_OBJ_BOMBIWA, -594,375,-617, 0,0,1, 0x000A }},
            { 0, -1, { ACTOR_OBJ_BOMBIWA, -452,375,-480, 0,-25850,1, 0x000B }},
            { 1, 57, { ACTOR_EN_MS, -582,375,-438, 0,16384,0, static_cast<int16_t>(0xFFFF) }},
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
            { 0, -1, { ACTOR_OBJ_KIBAKO2, 529,0,-3556, 0,0,0, static_cast<int16_t>(0xFFFF) }},
            { 1, -1, { ACTOR_OBJ_KIBAKO2, 529,0,-3556, 0,0,0, static_cast<int16_t>(0xFFFF) }},
        } },
    } },
    { 0x5D, { // Gerudo Fortress
        { 0x00, {
            { 0, -1, { ACTOR_EN_NIW, 1525,834,-2020, 0,16384,0, static_cast<int16_t>(0xFFFF) }},
            { 1, -1, { ACTOR_EN_NIW, 1525,834,-2020, 0,16384,0, static_cast<int16_t>(0xFFFF) }},
            { 0, -1, { ACTOR_BG_UMAJUMP, 190,1103,-2915, 0x4000,880,0, static_cast<int16_t>(0xFFFF) }},
            { 0, -1, { ACTOR_BG_UMAJUMP, 190,1103,-2915, 0x4000,-0x7C90,0, static_cast<int16_t>(0xFFFF) }},//rot.y is 880+0x8000
            { 1, -1, { ACTOR_BG_UMAJUMP, 190,1103,-2915, 0x4000,880,0, static_cast<int16_t>(0xFFFF) }},
            { 1, -1, { ACTOR_BG_UMAJUMP, 190,1103,-2915, 0x4000,-0x7C90,0, static_cast<int16_t>(0xFFFF) }},//rot.y is 880+0x8000
            { 0, 21, { 0xA, 926,1042,-2512, 0,-16384,0, 0x7c1 }},
            { 1, 21, { 0xA, 926,1042,-2512, 0,-16384,0, 0x7c1 }},
        } },
    } },
    { SCENE_SPOT13, { //Desert Wasteland
        { 0x00, {
            { -1, 4, { ACTOR_EN_SW, 637,-393,-2536, 16384,0,0, static_cast<int16_t>(0xB602) }},
        } },
        { 0x01, {
            { -1, -1, { ACTOR_EN_HATA, 3315,-261, 2244, 0,0,0, static_cast<int16_t>(0xFFFF) }},
            { -1, -1, { ACTOR_OBJ_KIBAKO2, 3460,-435,2484, 0,7100,0, static_cast<int16_t>(0xFFFF) }},
            { -1, -1, { ACTOR_OBJ_KIBAKO2, 3621,-392,2847, 0,7100,0, static_cast<int16_t>(0xFFFF) }},
            { -1, -1, { ACTOR_EN_NIW, 3125,-268,2459, 0,7100,0, static_cast<int16_t>(0xFFFF) }},
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
            {-1, 4, {ACTOR_EN_BB, 1387,510,-1436, 0,0x4000,0, static_cast<int16_t>(0xffff)}},
            {-1, -1, {ACTOR_EN_BB, 1187,463,-1436, 0,-0x4000,0, static_cast<int16_t>(0xffff)}}
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

bool Scene_CommandActorList(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetActorList* cmdActor = std::static_pointer_cast<LUS::SetActorList>(cmd);
    LUS::SetActorList* cmdActor = (LUS::SetActorList*)cmd;
    std::vector<LUS::ActorEntry> copy = cmdActor->actorList;

    //Handles static entry overrides
    if (sceneActorOverrides.find(play->sceneNum) != sceneActorOverrides.end() &&
            sceneActorOverrides.at(play->sceneNum).find(play->roomCtx.curRoom.num) != sceneActorOverrides.at(play->sceneNum).end()) {
        auto& roomOverrides = sceneActorOverrides.at(play->sceneNum).at(play->roomCtx.curRoom.num);
        for (auto& [setup, index, entry] : roomOverrides) {
            if (setup == -1 || setup == gSaveContext.sceneSetupIndex) {
                if (index == -1) {
                    copy.push_back(entry);
                } else {
                    copy[index] = entry;
                }
            }
        }
    }

    //Handles dynamic resource persistance overrides
    ActorEntry* entries = (ActorEntry*)malloc(copy.size() * sizeof(ActorEntry));
    TempResourceEntries = {};

    for (int i = 0; i < copy.size(); i++)
    {
        entries[i].id = copy[i].id;
        entries[i].pos.x = copy[i].pos.x;
        entries[i].pos.y = copy[i].pos.y;
        entries[i].pos.z = copy[i].pos.z;
        entries[i].rot.x = copy[i].rot.x;
        entries[i].rot.y = copy[i].rot.y;
        entries[i].rot.z = copy[i].rot.z;
        entries[i].params = copy[i].params;

        if (entries[i].id == ACTOR_OBJ_TSUBO) {
            ActorSpawnResource sw;
            sw.scene = play->sceneNum;
            sw.room = play->roomCtx.curRoom.num;
            copyActorSpawn(sw.entry, copy[i]);
            sw.dirt = 0;
            TempResourceEntries.insert({i,sw});
            auto foundVal = UsedResources.find(sw);
            if (foundVal != UsedResources.end() && isResourceYetToRestore(foundVal)) {
                entries[i].params &= 0xFFE0;
                entries[i].params |= ITEM00_MAX;
            }
        } else if (entries[i].id == ACTOR_EN_TUBO_TRAP) {
            ActorSpawnResource sw;
            sw.scene = play->sceneNum;
            sw.room = play->roomCtx.curRoom.num;
            copyActorSpawn(sw.entry, copy[i]);
            sw.dirt = 0;
            TempResourceEntries.insert({i,sw});
            auto foundVal = UsedResources.find(sw);
            if (foundVal != UsedResources.end() && isResourceYetToRestore(foundVal)) {
                entries[i].params = 0x7FFF;
            }
        } else if (entries[i].id == ACTOR_EN_WONDER_ITEM && DEKU_TREE_DEAD) {
            ActorSpawnResource sw;
            sw.scene = play->sceneNum;
            sw.room = play->roomCtx.curRoom.num;
            copyActorSpawn(sw.entry, copy[i]);
            sw.dirt = 0;
            TempResourceEntries.insert({i,sw});
            auto foundVal = UsedResources.find(sw);
            if (foundVal != UsedResources.end() && isResourceYetToRestore(foundVal)) {
                entries[i].params &= 0x07FF;
                entries[i].params |= (0xA << 0xB);
            }
        } else if (entries[i].id == ACTOR_EN_ITEM00 && DEKU_TREE_DEAD) {
            ActorSpawnResource sw;
            sw.scene = play->sceneNum;
            sw.room = play->roomCtx.curRoom.num;
            copyActorSpawn(sw.entry, copy[i]);
            sw.dirt = 0;
            TempResourceEntries.insert({i,sw});
            auto foundVal = UsedResources.find(sw);
            if (foundVal != UsedResources.end() && isResourceYetToRestore(foundVal)) {
                entries[i].params &= 0xFF00;
                entries[i].params |= ITEM00_MAX;
            }
        } else if (entries[i].id == ACTOR_EN_KUSA && (entries[i].params & 0x3) == 0) {
            // ActorSpawnResource sw;
            // sw.scene = play->sceneNum;
            // sw.room = play->roomCtx.curRoom.num;
            // copyActorSpawn(sw.entry, copy[i]);
            // sw.dirt = 0;
            // TempResourceEntries.insert({i,sw});
            // auto foundVal = UsedResources.find(sw);
            // if (foundVal != UsedResources.end()) {
            //     entries[i].params &= 0xFFFC;
            //     entries[i].params |= 3;
            // }
        } else if (entries[i].id == ACTOR_OBJ_KIBAKO2) {
            ActorSpawnResource sw;
            sw.scene = play->sceneNum;
            sw.room = play->roomCtx.curRoom.num;
            copyActorSpawn(sw.entry, copy[i]);
            sw.dirt = 0;
            TempResourceEntries.insert({i,sw});
            auto foundVal = UsedResources.find(sw);
            if (foundVal != UsedResources.end() && isResourceYetToRestore(foundVal)) {
                entries[i].rot.x = 0x1C;
            }
        } else if (entries[i].id == ACTOR_BG_HAKA) {
            ActorSpawnResource sw;
            sw.scene = play->sceneNum;
            sw.room = play->roomCtx.curRoom.num;
            copyActorSpawn(sw.entry, copy[i]);
            sw.dirt = 0;
            TempResourceEntries.insert({i,sw});
            auto foundVal = UsedResources.find(sw);
            if (foundVal != UsedResources.end() && isResourceYetToRestore(foundVal)) {
                entries[i].rot.z = 1;
            }
        } else if (entries[i].id == ACTOR_EN_SKJ) {
            ActorSpawnResource sw;
            sw.scene = play->sceneNum;
            sw.room = play->roomCtx.curRoom.num;
            copyActorSpawn(sw.entry, copy[i]);
            sw.dirt = 0;
            TempResourceEntries.insert({i,sw});
            auto foundVal = UsedResources.find(sw);
            if (foundVal != UsedResources.end() && isResourceYetToRestore(foundVal)) {
                entries[i].rot.z = 0x1;
            }
        } else if (entries[i].id == ACTOR_EN_COW) {
            ActorSpawnResource sw;
            sw.scene = play->sceneNum;
            sw.room = play->roomCtx.curRoom.num;
            copyActorSpawn(sw.entry, copy[i]);
            sw.dirt = 0;
            TempResourceEntries.insert({i,sw});
            auto foundVal = UsedResources.find(sw);
            if (foundVal != UsedResources.end() && isResourceYetToRestore(foundVal)) {
                entries[i].rot.z = 0x1;
            }
        }
    }

    play->numSetupActors = copy.size();//Note that this only allows for max 256 actor entries
    play->setupActorList = (ActorEntry*)entries;//cmdActor->GetRawPointer();

    return false;
}

bool Scene_CommandUnused2(PlayState* play, LUS::ISceneCommand* cmd) {
    // OTRTODO: Do we need to implement this?
    // play->unk_11DFC = SEGMENTED_TO_VIRTUAL(cmd->unused02.segment);
    return false;
}

bool Scene_CommandCollisionHeader(PlayState* play, LUS::ISceneCommand* cmd) {
    // LUS::SetCollisionHeader* cmdCol = std::static_pointer_cast<LUS::SetCollisionHeader>(cmd);
    LUS::SetCollisionHeader* cmdCol = (LUS::SetCollisionHeader*)cmd;
    for (int ii = 0; ii < cmdCol->collisionHeader->surfaceTypes.size(); ii++) {
        cmdCol->collisionHeader->surfaceTypes[ii].data[0];
        cmdCol->collisionHeader->surfaceTypes[ii].data[1];
        if ((play->sceneNum == SCENE_HAKAANA_OUKE && (ii == 0x9 || ii == 0xA)) ||
            (play->sceneNum == 0x8 && ((cmdCol->collisionHeader->surfaceTypes[ii].data[0] >> 13) == 2))) {//Well surfaces doing spike damage become voidouts
            cmdCol->collisionHeader->surfaceTypes[ii].data[0] |= 0x14 << 24;
        }
    }

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

extern "C" void* func_800982FC(ObjectContext* objectCtx, s32 bankIndex, s16 objectId);

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
        { -1, 9,  { 1,(int8_t)255, 0,(int8_t)255,   ACTOR_EN_HOLL, 0,0,-400,  0, 0x3F }},
        { -1, 10, { 1,(int8_t)255, 0,(int8_t)255,   ACTOR_EN_HOLL, 0,0, 400,  -32768, 0x3F }},
        { -1, 11, { 0,(int8_t)255, 1,(int8_t)255,   ACTOR_EN_HOLL, 800,0, 400,  -32768, 0x3F }},
        { -1, 12, { 1,(int8_t)255, 2,(int8_t)255,   ACTOR_EN_HOLL, 400,0, -800,  16384, 0x3F }},
        { -1, 13, { 2,(int8_t)255, 3,(int8_t)255,   ACTOR_EN_HOLL, 1600,0, -400,  -32768, 0x3F }},
        { -1, 14, { 7,(int8_t)255, 4,(int8_t)255,   ACTOR_EN_HOLL, 2000,0, -1600,  -16384, 0x3F }},
        { -1, 15, { 7,(int8_t)255, 8,(int8_t)255,   ACTOR_EN_HOLL, 2000,0, -2400,  16384, 0x3F | (0x7 << 6) }},
        { -1, 16, { 8,(int8_t)255, 7,(int8_t)255,   ACTOR_EN_HOLL, 1600,0, -2800,  0, 0x3F }},
        { -1, 17, { 8,(int8_t)255, 7,(int8_t)255,   ACTOR_EN_HOLL, 400,0, -2400,  -16384, 0x3F | (0x7 << 6) }},
        { -1, 18, { 7,(int8_t)255, 8,(int8_t)255,   ACTOR_EN_HOLL, 800,0, -2800,  0, 0x3F }},
        { -1, 19, { 7,(int8_t)255, 8,(int8_t)255,   ACTOR_EN_HOLL, 800,0, -2000,  -32768, 0x3F | (0x1 << 9)}},
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

    play->transiActorCtx.numActors = cmdActor->transitionActorList.size();
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
