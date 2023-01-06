#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO

#include "OTRGlobals.h"
#include <ResourceMgr.h>
#include <Scene.h>
#include <Utils/StringHelper.h>
#include "global.h"
#include "vt.h"
#include <CollisionHeader.h>
#include <DisplayList.h>
#include <Cutscene.h>
#include <Path.h>
#include <Text.h>
#include <Blob.h>

extern Ship::Resource* OTRPlay_LoadFile(PlayState* play, const char* fileName);
extern "C" s32 Object_Spawn(ObjectContext* objectCtx, s16 objectId);
extern "C" RomFile sNaviMsgFiles[];
s32 OTRScene_ExecuteCommands(PlayState* play, Ship::Scene* scene);

std::shared_ptr<Ship::OtrFile> ResourceMgr_LoadFile(const char* path) {
    std::string Path = path;
    if (IsGameMasterQuest()) {
        size_t pos = 0;
        if ((pos = Path.find("/nonmq/", 0)) != std::string::npos) {
            Path.replace(pos, 7, "/mq/");
        }
    }
    return OTRGlobals::Instance->context->GetResourceManager()->LoadFile(Path.c_str());
}

// Forward Declaration of function declared in OTRGlobals.cpp
std::shared_ptr<Ship::Resource> ResourceMgr_LoadResource(const char* path);

bool Scene_CommandSpawnList(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetStartPositionList* cmdStartPos = (Ship::SetStartPositionList*)cmd;

    ActorEntry* linkSpawnEntry = nullptr;

    if (cmdStartPos->cachedGameData != nullptr)
    {
        ActorEntry* entries = (ActorEntry*)cmdStartPos->cachedGameData;
        linkSpawnEntry = &entries[play->setupEntranceList[play->curSpawn].spawn];
    }
    else
    {
        ActorEntry* entries = (ActorEntry*)malloc(sizeof(ActorEntry) * cmdStartPos->entries.size());

        for (int i = 0; i < cmdStartPos->entries.size(); i++)
        {
            entries[i].id = cmdStartPos->entries[i].actorNum;
            entries[i].params = cmdStartPos->entries[i].initVar;
            entries[i].pos.x = cmdStartPos->entries[i].posX;
            entries[i].pos.y = cmdStartPos->entries[i].posY;
            entries[i].pos.z = cmdStartPos->entries[i].posZ;
            entries[i].rot.x = cmdStartPos->entries[i].rotX;
            entries[i].rot.y = cmdStartPos->entries[i].rotY;
            entries[i].rot.z = cmdStartPos->entries[i].rotZ;

            SPDLOG_INFO("Spawn {0:d} - ID: {1:x}, Parameters: {2:x}", i, (uint16_t)entries[i].id, (uint16_t)entries[i].params);
        }

        linkSpawnEntry = &entries[play->setupEntranceList[play->curSpawn].spawn];
        cmdStartPos->cachedGameData = entries;
    }

    ActorEntry* linkEntry = play->linkActorEntry = linkSpawnEntry;

    s16 linkObjectId;

    play->linkAgeOnLoad = ((void)0, gSaveContext.linkAge);

    linkObjectId = gLinkObjectIds[((void)0, gSaveContext.linkAge)];

    //gActorOverlayTable[linkEntry->id].initInfo->objectId = linkObjectId;
    Object_Spawn(&play->objectCtx, linkObjectId);

    return false;
}

const std::map<u16, std::map<u16, std::vector<std::tuple<int, int, Ship::ActorSpawnEntry>>>> sceneActorOverrides = {
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
    } },
    { 0x3e, { // Grotto
        { 0x05, { // Octorok Grotto
            //{ -1, 8, { ACTOR_EN_ITEM00, 32, -129, 852, 0, 0, 0, 0x406 }},
        } },
    } },
    { 0x55, { // Kokiri Forest
        { 0x00, {//Village
            //{ -1, -1, { ACTOR_EN_ITEM00, 1297, 240, -553, 0, 0, 0, 0x0800+(uint16_t)ITEM00_HEART_PIECE }},
            //{ -1, -1, { ACTOR_EN_TEST, 70, -80, 675, 0, 0, 0, 0xFFFF }},
            //{ -1, -1, { ACTOR_EN_WOOD02, 0, -80, 870, 0, 0, 0, 0x0011 }},
            //{ -1, 72, { ACTOR_EN_ITEM00, 0, -80, 870, 0, 0, 0, 0x100+(uint16_t)ITEM00_HEART_PIECE }},
            //{ -1, -1, { ACTOR_EN_SA, -110, -80, 950, 0, 0, 0, 0x0}},
            { 0, -1, { 0x01CF, -367,53,-770, 0, 0, 1, 0x0 }}, { 1, -1, { 0x01CF, -367,53,-770, 0, 0, 1, 0x0 }},
            { 0, -1, { 0x01CF, -459,55,-818, 0, 0, 1, 0x0 }}, { 1, -1, { 0x01CF, -459,55,-818, 0, 0, 1, 0x0 }},
            { 0, -1, { 0x01CF, -521,56,-773, 0, 0, 1, 0x0 }}, { 1, -1, { 0x01CF, -521,56,-773, 0, 0, 1, 0x0 }},
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
            { 1, 57, { ACTOR_EN_MS, 204,570,146, 0,16384,0, 0xFFFF }},
            { 1, 60, { ACTOR_OBJ_BOMBIWA, -582,375,-438, 0,-20771,1, 0x0002 }},
            { 1, 62, { ACTOR_OBJ_BOMBIWA, -464,375,-558, 0,-25850,1, 0x0008 }},
            { 1, 63, { ACTOR_OBJ_BOMBIWA, -533,375,-594, 0,16384,1, 0x0009 }},
            { 1, 64, { ACTOR_OBJ_BOMBIWA, -594,375,-617, 0,0,1, 0x000A }},
        } },
    } },
    { 0x59, { // Zora's Fountain
        { 0x00, {
            { 2, -1, { ACTOR_EN_GOROIWA, 1243,322,100, 0,-20771,1, 0x1D00 }},
            { 3, -1, { ACTOR_EN_GOROIWA, 1243,322,100, 0,-20771,1, 0x1D00 }},
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

bool Scene_CommandActorList(PlayState* play, Ship::SceneCommand* cmd) {
    Ship::SetActorList* cmdActor = (Ship::SetActorList*)cmd;

    play->numSetupActors = cmdActor->entries.size();

    if (cmdActor->cachedGameData != nullptr)
        play->setupActorList = (ActorEntry*)cmdActor->cachedGameData;
    else
    {
        if (sceneActorOverrides.find(play->sceneNum) != sceneActorOverrides.end() &&
                        sceneActorOverrides.at(play->sceneNum).find(play->roomCtx.curRoom.num) != sceneActorOverrides.at(play->sceneNum).end()) {
            auto& roomOverrides = sceneActorOverrides.at(play->sceneNum).at(play->roomCtx.curRoom.num);
            for (auto& [setup, index, entry] : roomOverrides) {
                if (setup == -1 || setup == gSaveContext.sceneSetupIndex) {
                    if (index == -1) {
                        cmdActor->entries.push_back(entry);
                    } else {
                        cmdActor->entries[index] = entry;
                    }
                }
            }
            play->numSetupActors = cmdActor->entries.size();
        }

        SPDLOG_INFO("Scene: 0x{0:x}, Room: 0x{1:x}, Setup: 0x{2:x}", (uint16_t)play->sceneNum, (uint16_t)play->roomCtx.curRoom.num, (uint32_t)gSaveContext.sceneSetupIndex);

        ActorEntry* entries = (ActorEntry*)malloc(cmdActor->entries.size() * sizeof(ActorEntry));

        for (int i = 0; i < cmdActor->entries.size(); i++)
        {
            entries[i].id = cmdActor->entries[i].actorNum;
            entries[i].pos.x = cmdActor->entries[i].posX;
            entries[i].pos.y = cmdActor->entries[i].posY;
            entries[i].pos.z = cmdActor->entries[i].posZ;
            entries[i].rot.x = cmdActor->entries[i].rotX;
            entries[i].rot.y = cmdActor->entries[i].rotY;
            entries[i].rot.z = cmdActor->entries[i].rotZ;
            entries[i].params = cmdActor->entries[i].initVar;

            SPDLOG_INFO("Entity {0:d}\t ID: 0x{1:x}, \tParams: 0x{2:x}, \tpos: {3:d},{4:d},{5:d}, \t{6:d},{7:d},{8:d}",
            i, (uint16_t)entries[i].id, (uint16_t)entries[i].params,
            entries[i].pos.x, entries[i].pos.y, entries[i].pos.z, entries[i].rot.x, entries[i].rot.y, entries[i].rot.z);
        }

        cmdActor->cachedGameData = entries;
        play->setupActorList = entries;
    }

    return false;
}

bool Scene_CommandUnused2(PlayState* play, Ship::SceneCommand* cmd)
{
    // Do we need to implement this?
    //play->unk_11DFC = SEGMENTED_TO_VIRTUAL(cmd->unused02.segment);

    return false;
}

bool Scene_CommandCollisionHeader(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetCollisionHeader* cmdCol = (Ship::SetCollisionHeader*)cmd;

    auto colRes = std::static_pointer_cast<Ship::CollisionHeader>(ResourceMgr_LoadResource(cmdCol->filePath.c_str()));

    CollisionHeader* colHeader = nullptr;

    if (colRes->CachedGameAsset != nullptr)
        colHeader = (CollisionHeader*)colRes->CachedGameAsset;
    else
    {
        colHeader = (CollisionHeader*)malloc(sizeof(CollisionHeader));

        colHeader->minBounds.x = colRes->absMinX;
        colHeader->minBounds.y = colRes->absMinY;
        colHeader->minBounds.z = colRes->absMinZ;

        colHeader->maxBounds.x = colRes->absMaxX;
        colHeader->maxBounds.y = colRes->absMaxY;
        colHeader->maxBounds.z = colRes->absMaxZ;

        colHeader->vtxList = (Vec3s*)malloc(sizeof(Vec3s) * colRes->vertices.size());
        colHeader->numVertices = colRes->vertices.size();

        for (int i = 0; i < colRes->vertices.size(); i++)
        {
            colHeader->vtxList[i].x = colRes->vertices[i].x;
            colHeader->vtxList[i].y = colRes->vertices[i].y;
            colHeader->vtxList[i].z = colRes->vertices[i].z;
        }

        colHeader->polyList = (CollisionPoly*)malloc(sizeof(CollisionPoly) * colRes->polygons.size());
        colHeader->numPolygons = colRes->polygons.size();

        for (int i = 0; i < colRes->polygons.size(); i++)
        {
            colHeader->polyList[i].type = colRes->polygons[i].type;
            colHeader->polyList[i].flags_vIA = colRes->polygons[i].vtxA;
            colHeader->polyList[i].flags_vIB = colRes->polygons[i].vtxB;
            colHeader->polyList[i].vIC = colRes->polygons[i].vtxC;
            colHeader->polyList[i].normal.x = colRes->polygons[i].a;
            colHeader->polyList[i].normal.y = colRes->polygons[i].b;
            colHeader->polyList[i].normal.z = colRes->polygons[i].c;
            colHeader->polyList[i].dist = colRes->polygons[i].d;
        }

        colHeader->surfaceTypeList = (SurfaceType*)malloc(colRes->polygonTypes.size() * sizeof(SurfaceType));

        for (int i = 0; i < colRes->polygonTypes.size(); i++)
        {
            colHeader->surfaceTypeList[i].data[0] = colRes->polygonTypes[i] >> 32;
            colHeader->surfaceTypeList[i].data[1] = colRes->polygonTypes[i] & 0xFFFFFFFF;
        }

        colHeader->cameraDataList = (CamData*)malloc(sizeof(CamData) * colRes->camData->entries.size());
        colHeader->cameraDataListLen = colRes->camData->entries.size();

        for (int i = 0; i < colRes->camData->entries.size(); i++)
        {
            colHeader->cameraDataList[i].cameraSType = colRes->camData->entries[i]->cameraSType;
            colHeader->cameraDataList[i].numCameras = colRes->camData->entries[i]->numData;

            int idx = colRes->camData->entries[i]->cameraPosDataIdx;

            colHeader->cameraDataList[i].camPosData = (Vec3s*)malloc(sizeof(Vec3s) * colRes->camData->entries[i]->numData);

            for (int j = 0; j < colRes->camData->entries[i]->numData; j++)
            {
                if (colRes->camData->cameraPositionData.size() > 0)
                {
                    colHeader->cameraDataList[i].camPosData[j].x = colRes->camData->cameraPositionData[idx + j]->x;
                    colHeader->cameraDataList[i].camPosData[j].y = colRes->camData->cameraPositionData[idx + j]->y;
                    colHeader->cameraDataList[i].camPosData[j].z = colRes->camData->cameraPositionData[idx + j]->z;
                }
                else
                {
                    colHeader->cameraDataList[i].camPosData->x = 0;
                    colHeader->cameraDataList[i].camPosData->y = 0;
                    colHeader->cameraDataList[i].camPosData->z = 0;
                }
            }
        }

        colHeader->numWaterBoxes = colRes->waterBoxes.size();
        colHeader->waterBoxes = (WaterBox*)malloc(sizeof(WaterBox) * colHeader->numWaterBoxes);

        for (int i = 0; i < colHeader->numWaterBoxes; i++)
        {
            colHeader->waterBoxes[i].xLength = colRes->waterBoxes[i].xLength;
            colHeader->waterBoxes[i].ySurface = colRes->waterBoxes[i].ySurface;
            colHeader->waterBoxes[i].xMin = colRes->waterBoxes[i].xMin;
            colHeader->waterBoxes[i].zMin = colRes->waterBoxes[i].zMin;
            colHeader->waterBoxes[i].xLength = colRes->waterBoxes[i].xLength;
            colHeader->waterBoxes[i].zLength = colRes->waterBoxes[i].zLength;
            colHeader->waterBoxes[i].properties = colRes->waterBoxes[i].properties;
        }

        colRes->CachedGameAsset = colHeader;
    }

    BgCheck_Allocate(&play->colCtx, play, colHeader);

    return false;
}

bool Scene_CommandRoomList(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetRoomList* cmdRoomList = (Ship::SetRoomList*)cmd;

    play->numRooms = cmdRoomList->rooms.size();
    play->roomList = (RomFile*)malloc(play->numRooms * sizeof(RomFile));

    for (int i = 0; i < cmdRoomList->rooms.size(); i++)
    {
        play->roomList[i].fileName = (char*)cmdRoomList->rooms[i].name.c_str();
        play->roomList[i].vromStart = cmdRoomList->rooms[i].vromStart;
        play->roomList[i].vromEnd = cmdRoomList->rooms[i].vromEnd;
    }

    return false;
}

bool Scene_CommandEntranceList(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetEntranceList* otrEntrance = (Ship::SetEntranceList*)cmd;

    if (otrEntrance->cachedGameData != nullptr)
        play->setupEntranceList = (EntranceEntry*)otrEntrance->cachedGameData;
    else
    {
        play->setupEntranceList = (EntranceEntry*)malloc(otrEntrance->entrances.size() * sizeof(EntranceEntry));
        SPDLOG_INFO("Scene: 0x{0:x}", (uint16_t)play->sceneNum);

        for (int i = 0; i < otrEntrance->entrances.size(); i++)
        {
            play->setupEntranceList[i].room = otrEntrance->entrances[i].roomToLoad;
            play->setupEntranceList[i].spawn = otrEntrance->entrances[i].startPositionIndex;
            SPDLOG_INFO("Entry {0:d}\t Spawn: 0x{1:x}, \tRoom: 0x{2:x}",
            i, play->setupEntranceList[i].spawn, play->setupEntranceList[i].room);
        }

        otrEntrance->cachedGameData = play->setupEntranceList;
    }

    return false;
}

bool Scene_CommandSpecialFiles(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetSpecialObjects* otrSpecial = (Ship::SetSpecialObjects*)cmd;

    if (otrSpecial->globalObject != 0)
        play->objectCtx.subKeepIndex = Object_Spawn(&play->objectCtx, otrSpecial->globalObject);

    if (otrSpecial->elfMessage != 0)
    {
        auto res = (Ship::Blob*)OTRPlay_LoadFile(play, sNaviMsgFiles[otrSpecial->elfMessage - 1].fileName);
        play->cUpElfMsgs = (ElfMessage*)res->data.data();
    }

    return false;
}

bool Scene_CommandRoomBehavior(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetRoomBehavior* cmdRoom = (Ship::SetRoomBehavior*)cmd;

    play->roomCtx.curRoom.behaviorType1 = cmdRoom->gameplayFlags;
    play->roomCtx.curRoom.behaviorType2 = cmdRoom->gameplayFlags2 & 0xFF;
    play->roomCtx.curRoom.lensMode = (cmdRoom->gameplayFlags2 >> 8) & 1;
    play->msgCtx.disableWarpSongs = (cmdRoom->gameplayFlags2 >> 0xA) & 1;

    return false;
}

bool Scene_CommandMeshHeader(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetMesh* otrMesh = (Ship::SetMesh*)cmd;
    
    if (otrMesh->cachedGameData != nullptr)
        play->roomCtx.curRoom.meshHeader = (MeshHeader*)otrMesh->cachedGameData;
    else
    {
        play->roomCtx.curRoom.meshHeader = (MeshHeader*)malloc(sizeof(MeshHeader));
        play->roomCtx.curRoom.meshHeader->base.type = otrMesh->meshHeaderType;
        play->roomCtx.curRoom.meshHeader->polygon0.num = otrMesh->meshes.size();

        if (otrMesh->meshHeaderType == 2)
            play->roomCtx.curRoom.meshHeader->polygon0.start = malloc(sizeof(PolygonDlist2) * play->roomCtx.curRoom.meshHeader->polygon0.num);
        else
            play->roomCtx.curRoom.meshHeader->polygon0.start = malloc(sizeof(PolygonDlist) * play->roomCtx.curRoom.meshHeader->polygon0.num);

        for (int i = 0; i < play->roomCtx.curRoom.meshHeader->polygon0.num; i++)
        {
            if (otrMesh->meshHeaderType == 2)
            {
                PolygonDlist2* arr = (PolygonDlist2*)play->roomCtx.curRoom.meshHeader->polygon0.start;
                PolygonDlist2* dlist = &arr[i];

                if (otrMesh->meshes[i].opa != "")
                {
                    auto opaFile =
                        std::static_pointer_cast<Ship::DisplayList>(ResourceMgr_LoadResource(otrMesh->meshes[i].opa.c_str()));

                    dlist->opaDL = opaFile.get();
                    dlist->opa = (Gfx*)&dlist->opaDL->instructions[0];
                }
                else
                {
                    dlist->opa = 0;
                }

                if (otrMesh->meshes[i].xlu != "")
                {
                    auto xluFile =
                        std::static_pointer_cast<Ship::DisplayList>(ResourceMgr_LoadResource(otrMesh->meshes[i].xlu.c_str()));

                    dlist->xluDL = xluFile.get();
                    dlist->xlu = (Gfx*)&dlist->xluDL->instructions[0];
                }
                else
                    dlist->xlu = 0;

                dlist->pos.x = otrMesh->meshes[i].x;
                dlist->pos.y = otrMesh->meshes[i].y;
                dlist->pos.z = otrMesh->meshes[i].z;
                dlist->unk_06 = otrMesh->meshes[i].unk_06;

                //play->roomCtx.curRoom.meshHeader->base.start = dlist;
            }
            else if (otrMesh->meshHeaderType == 1)
            {
                PolygonDlist* pType = (PolygonDlist*)malloc(sizeof(PolygonDlist));

                if (otrMesh->meshes[0].imgOpa != "")
                    pType->opa = (Gfx*)&std::static_pointer_cast<Ship::DisplayList>(ResourceMgr_LoadResource(otrMesh->meshes[0].imgOpa.c_str()))->instructions[0];
                else
                    pType->opa = 0;

                if (otrMesh->meshes[0].imgXlu != "")
                    pType->xlu = (Gfx*)&std::static_pointer_cast<Ship::DisplayList>(ResourceMgr_LoadResource(otrMesh->meshes[0].imgXlu.c_str()))->instructions[0];
                else
                    pType->xlu = 0;

                play->roomCtx.curRoom.meshHeader->polygon1.dlist = (Gfx*)pType;

                play->roomCtx.curRoom.meshHeader->polygon1.format = otrMesh->meshes[0].imgFmt;

                if (otrMesh->meshes[0].imgFmt == 1)
                {
                    play->roomCtx.curRoom.meshHeader->polygon1.single.fmt = otrMesh->meshes[0].images[0].fmt;
                    play->roomCtx.curRoom.meshHeader->polygon1.single.source =
                        (void*)(ResourceMgr_LoadFile(otrMesh->meshes[0].images[0].sourceBackground.c_str()))
                        .get()
                        ->Buffer.get();
                    play->roomCtx.curRoom.meshHeader->polygon1.single.siz = otrMesh->meshes[0].images[0].siz;
                    play->roomCtx.curRoom.meshHeader->polygon1.single.width = otrMesh->meshes[0].images[0].width;
                    play->roomCtx.curRoom.meshHeader->polygon1.single.height = otrMesh->meshes[0].images[0].height;
                    play->roomCtx.curRoom.meshHeader->polygon1.single.fmt = otrMesh->meshes[0].images[0].fmt;
                    play->roomCtx.curRoom.meshHeader->polygon1.single.mode0 = otrMesh->meshes[0].images[0].mode0;
                    play->roomCtx.curRoom.meshHeader->polygon1.single.tlutCount = otrMesh->meshes[0].images[0].tlutCount;
                }
                else
                {
                    play->roomCtx.curRoom.meshHeader->polygon1.multi.count = otrMesh->meshes[0].images.size();
                    play->roomCtx.curRoom.meshHeader->polygon1.multi.list =
                        (BgImage*)calloc(sizeof(BgImage), play->roomCtx.curRoom.meshHeader->polygon1.multi.count);

                    for (size_t i = 0; i < otrMesh->meshes[0].images.size(); i++)
                    {
                        play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].fmt = otrMesh->meshes[0].images[i].fmt;
                        play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].source =
                            (void*)(ResourceMgr_LoadFile(otrMesh->meshes[0].images[i].sourceBackground.c_str()))
                            .get()
                            ->Buffer.get();
                        play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].siz = otrMesh->meshes[0].images[i].siz;
                        play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].width = otrMesh->meshes[0].images[i].width;
                        play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].height =
                            otrMesh->meshes[0].images[i].height;
                        play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].fmt = otrMesh->meshes[0].images[i].fmt;
                        play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].mode0 = otrMesh->meshes[0].images[i].mode0;
                        play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].tlutCount =
                            otrMesh->meshes[0].images[i].tlutCount;
                        play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].unk_00 =
                            otrMesh->meshes[0].images[i].unk_00;
                        play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].unk_0C =
                            otrMesh->meshes[0].images[i].unk_0C;
                        play->roomCtx.curRoom.meshHeader->polygon1.multi.list[i].id = otrMesh->meshes[0].images[i].id;
                    }
                }
            }
            else
            {
                PolygonDlist* arr = (PolygonDlist*)play->roomCtx.curRoom.meshHeader->polygon0.start;
                PolygonDlist* dlist = &arr[i];

                if (otrMesh->meshes[i].opa != "")
                {
                    auto opaFile = std::static_pointer_cast<Ship::DisplayList>(ResourceMgr_LoadResource(otrMesh->meshes[i].opa.c_str()));

                    dlist->opaDL = opaFile.get();
                    dlist->opa = (Gfx*)&dlist->opaDL->instructions[0];
                }
                else
                    dlist->opa = 0;

                if (otrMesh->meshes[i].xlu != "")
                {
                    auto xluFile = std::static_pointer_cast<Ship::DisplayList>(ResourceMgr_LoadResource(otrMesh->meshes[i].xlu.c_str()));

                    dlist->xluDL = xluFile.get();
                    dlist->xlu = (Gfx*)&dlist->xluDL->instructions[0];
                }
                else
                    dlist->xlu = 0;

                //play->roomCtx.curRoom.meshHeader->base.start = dlist;
            }
        }

        otrMesh->cachedGameData = play->roomCtx.curRoom.meshHeader;
    }

    return false;
}

extern "C" void* func_800982FC(ObjectContext * objectCtx, s32 bankIndex, s16 objectId);

bool Scene_CommandObjectList(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetObjectList* cmdObj = (Ship::SetObjectList*)cmd;

    s32 i;
    s32 j;
    s32 k;
    ObjectStatus* status;
    ObjectStatus* status2;
    ObjectStatus* firstStatus;
    //s16* objectEntry = SEGMENTED_TO_VIRTUAL(cmd->objectList.segment);
    s16* objectEntry = (s16*)cmdObj->objects.data();
    void* nextPtr;

    k = 0;
    //i = play->objectCtx.unk_09;
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

bool Scene_CommandLightList(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetLightList* cmdLight = (Ship::SetLightList*)cmd;

    LightInfo* lightInfo = (LightInfo*)malloc(cmdLight->lights.size() * sizeof(LightInfo));

    for (int i = 0; i < cmdLight->lights.size(); i++)
    {
        lightInfo[i].type = cmdLight->lights[i].type;
        lightInfo[i].params.point.x = cmdLight->lights[i].x;
        lightInfo[i].params.point.y = cmdLight->lights[i].y;
        lightInfo[i].params.point.z = cmdLight->lights[i].z;
        lightInfo[i].params.point.radius = cmdLight->lights[i].radius;
        lightInfo[i].params.point.drawGlow = cmdLight->lights[i].drawGlow;
        lightInfo[i].params.point.color[0] = cmdLight->lights[i].r;
        lightInfo[i].params.point.color[1] = cmdLight->lights[i].g;
        lightInfo[i].params.point.color[2] = cmdLight->lights[i].b;

        LightContext_InsertLight(play, &play->lightCtx, &lightInfo[i]);
    }

    return false;
}

bool Scene_CommandPathList(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetPathways* cmdPath = (Ship::SetPathways*)cmd;

    Ship::Path* path = (Ship::Path*)ResourceMgr_LoadResource(cmdPath->paths[0].c_str()).get();
    play->setupPathList = (Path*)malloc(path->paths.size() * sizeof(Path));

    //for (int i = 0; i < cmdPath->paths.size(); i++)
    {
        for (int j = 0; j < path->paths.size(); j++)
        {
            play->setupPathList[j].count = path->paths[j].size();
            play->setupPathList[j].points = (Vec3s*)malloc(sizeof(Vec3s) * path->paths[j].size());

            for (int k = 0; k < path->paths[j].size(); k++)
            {
                play->setupPathList[j].points[k].x = path->paths[j][k].x;
                play->setupPathList[j].points[k].y = path->paths[j][k].y;
                play->setupPathList[j].points[k].z = path->paths[j][k].z;
            }
        }
    }

    return false;
}

const std::map<u16,  std::vector<std::tuple<int, int, Ship::TransitionActorEntry>>> sceneTransitionActorOverrides = {
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

//Keeps track of if extra transition actors have been loaded already, avoids multi loading them if this is the case
//std::map<u16, bool> sceneTransitionLoadFlags = {
//};

bool Scene_CommandTransitionActorList(PlayState* play, Ship::SceneCommand* cmd) {
    Ship::SetTransitionActorList* cmdActor = (Ship::SetTransitionActorList*)cmd;

    if (sceneTransitionActorOverrides.find(play->sceneNum) != sceneTransitionActorOverrides.end()/* && sceneTransitionLoadFlags.find(play->sceneNum) == sceneTransitionLoadFlags.end()*/) {
        //sceneTransitionLoadFlags.insert(std::make_pair(play->sceneNum,true));
        auto& roomOverrides = sceneTransitionActorOverrides.at(play->sceneNum);
        for (auto& [setup, index, entry] : roomOverrides) {
            if (setup == -1 || setup == gSaveContext.sceneSetupIndex) {
                if (index == -1) {
                    cmdActor->entries.push_back(entry);
                } else {
                    if (index < cmdActor->entries.size())
                        cmdActor->entries[index] = entry;
                    else
                        cmdActor->entries.emplace(cmdActor->entries.begin()+index,entry);
                }
            }
        }
    }

    play->transiActorCtx.numActors = cmdActor->entries.size();
    play->transiActorCtx.list = (TransitionActorEntry*)malloc(cmdActor->entries.size() * sizeof(TransitionActorEntry));

    for (int i = 0; i < cmdActor->entries.size(); i++)
    {
        play->transiActorCtx.list[i].sides[0].room = cmdActor->entries[i].frontObjectRoom;
        play->transiActorCtx.list[i].sides[0].effects = cmdActor->entries[i].frontTransitionReaction;
        play->transiActorCtx.list[i].sides[1].room = cmdActor->entries[i].backObjectRoom;
        play->transiActorCtx.list[i].sides[1].effects = cmdActor->entries[i].backTransitionReaction;
        play->transiActorCtx.list[i].id = cmdActor->entries[i].actorNum;
        play->transiActorCtx.list[i].pos.x = cmdActor->entries[i].posX;
        play->transiActorCtx.list[i].pos.y = cmdActor->entries[i].posY;
        play->transiActorCtx.list[i].pos.z = cmdActor->entries[i].posZ;
        play->transiActorCtx.list[i].rotY = cmdActor->entries[i].rotY;
        play->transiActorCtx.list[i].params = cmdActor->entries[i].initVar;

        SPDLOG_INFO("Transition {0:d}\t ID: 0x{1:x}, \tParams: 0x{2:x}, \tpos: {3:d},{4:d},{5:d}, \trot: {6:d},\nroomF: {7:d},{8:d}\troomB: {9:d},{10:d}",
            i, (uint16_t)cmdActor->entries[i].actorNum, cmdActor->entries[i].initVar,
            cmdActor->entries[i].posX, cmdActor->entries[i].posY, cmdActor->entries[i].posZ, cmdActor->entries[i].rotY,
            cmdActor->entries[i].frontObjectRoom, cmdActor->entries[i].frontTransitionReaction, cmdActor->entries[i].backObjectRoom, cmdActor->entries[i].backTransitionReaction);
    }

    return false;
}

//void TransitionActor_InitContext(GameState* state, TransitionActorContext* transiActorCtx) {
//    transiActorCtx->numActors = 0;
//}

bool Scene_CommandLightSettingsList(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetLightingSettings* otrLight = (Ship::SetLightingSettings*)cmd;

    play->envCtx.numLightSettings = otrLight->settings.size();
    play->envCtx.lightSettingsList = (EnvLightSettings*)malloc(play->envCtx.numLightSettings * sizeof(EnvLightSettings));

    for (int i = 0; i < otrLight->settings.size(); i++)
    {
        play->envCtx.lightSettingsList[i].ambientColor[0] = otrLight->settings[i].ambientClrR;
        play->envCtx.lightSettingsList[i].ambientColor[1] = otrLight->settings[i].ambientClrG;
        play->envCtx.lightSettingsList[i].ambientColor[2] = otrLight->settings[i].ambientClrB;

        play->envCtx.lightSettingsList[i].light1Color[0] = otrLight->settings[i].diffuseClrA_R;
        play->envCtx.lightSettingsList[i].light1Color[1] = otrLight->settings[i].diffuseClrA_G;
        play->envCtx.lightSettingsList[i].light1Color[2] = otrLight->settings[i].diffuseClrA_B;

        play->envCtx.lightSettingsList[i].light1Dir[0] = otrLight->settings[i].diffuseDirA_X;
        play->envCtx.lightSettingsList[i].light1Dir[1] = otrLight->settings[i].diffuseDirA_Y;
        play->envCtx.lightSettingsList[i].light1Dir[2] = otrLight->settings[i].diffuseDirA_Z;

        play->envCtx.lightSettingsList[i].light2Color[0] = otrLight->settings[i].diffuseClrB_R;
        play->envCtx.lightSettingsList[i].light2Color[1] = otrLight->settings[i].diffuseClrB_G;
        play->envCtx.lightSettingsList[i].light2Color[2] = otrLight->settings[i].diffuseClrB_B;

        play->envCtx.lightSettingsList[i].light2Dir[0] = otrLight->settings[i].diffuseDirB_X;
        play->envCtx.lightSettingsList[i].light2Dir[1] = otrLight->settings[i].diffuseDirB_Y;
        play->envCtx.lightSettingsList[i].light2Dir[2] = otrLight->settings[i].diffuseDirB_Z;

        play->envCtx.lightSettingsList[i].fogColor[0] = otrLight->settings[i].fogClrR;
        play->envCtx.lightSettingsList[i].fogColor[1] = otrLight->settings[i].fogClrG;
        play->envCtx.lightSettingsList[i].fogColor[2] = otrLight->settings[i].fogClrB;

        play->envCtx.lightSettingsList[i].fogNear = otrLight->settings[i].fogNear;
        play->envCtx.lightSettingsList[i].fogFar = otrLight->settings[i].fogFar;
    }

    return false;
}

// Scene Command 0x11: Skybox Settings
bool Scene_CommandSkyboxSettings(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetSkyboxSettings* cmdSky = (Ship::SetSkyboxSettings*)cmd;

    play->skyboxId = cmdSky->skyboxNumber;
    play->envCtx.unk_17 = play->envCtx.unk_18 = cmdSky->cloudsType;
    play->envCtx.indoors = cmdSky->isIndoors;

    return false;
}

bool Scene_CommandSkyboxDisables(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetSkyboxModifier* cmdSky = (Ship::SetSkyboxModifier*)cmd;

    play->envCtx.sunMoonDisabled = cmdSky->disableSunMoon;
    play->envCtx.skyboxDisabled = cmdSky->disableSky;

    return false;
}

bool Scene_CommandTimeSettings(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetTimeSettings* cmdTime = (Ship::SetTimeSettings*)cmd;

    if ((cmdTime->hour != 0xFF) && (cmdTime->min != 0xFF)) {
        gSaveContext.skyboxTime = gSaveContext.dayTime =
            ((cmdTime->hour + (cmdTime->min / 60.0f)) * 60.0f) / ((f32)(24 * 60) / 0x10000);
    }

    if (cmdTime->unk != 0xFF) {
        play->envCtx.timeIncrement = cmdTime->unk;
    }
    else {
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
        }
        else if ((gSaveContext.skyboxTime >= 0x4555) && (gSaveContext.skyboxTime < 0x5556)) {
            gSaveContext.skyboxTime = 0x5556;
        }
        else if ((gSaveContext.skyboxTime >= 0xAAAB) && (gSaveContext.skyboxTime < 0xB556)) {
            gSaveContext.skyboxTime = 0xB556;
        }
        else if ((gSaveContext.skyboxTime >= 0xC001) && (gSaveContext.skyboxTime < 0xCAAC)) {
            gSaveContext.skyboxTime = 0xCAAC;
        }
    }

    return false;
}

bool Scene_CommandWindSettings(PlayState* play, Ship::SceneCommand* cmd) {
    Ship::SetWind* cmdWind = (Ship::SetWind*)cmd;

    s8 x = cmdWind->windWest;
    s8 y = cmdWind->windVertical;
    s8 z = cmdWind->windSouth;

    play->envCtx.windDirection.x = x;
    play->envCtx.windDirection.y = y;
    play->envCtx.windDirection.z = z;

    play->envCtx.windSpeed = cmdWind->clothFlappingStrength;

    return false;
}

bool Scene_CommandExitList(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::ExitList* cmdExit = (Ship::ExitList*)cmd;

    play->setupExitList = (int16_t*)malloc(cmdExit->exits.size() * sizeof(int16_t));

    for (int i = 0; i < cmdExit->exits.size(); i++)
        play->setupExitList[i] = cmdExit->exits[i];

    return false;
}

bool Scene_CommandUndefined9(PlayState* play, Ship::SceneCommand* cmd) {
    return false;
}

bool Scene_CommandSoundSettings(PlayState* play, Ship::SceneCommand* cmd) {
    Ship::SetSoundSettings* cmdSnd = (Ship::SetSoundSettings*)cmd;

    play->sequenceCtx.seqId = cmdSnd->musicSequence;
    play->sequenceCtx.natureAmbienceId = cmdSnd->nightTimeSFX;

    if (gSaveContext.seqId == 0xFF) {
        Audio_QueueSeqCmd(cmdSnd->reverb | 0xF0000000);
    }

    return false;
}

bool Scene_CommandEchoSettings(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetEchoSettings* cmdEcho = (Ship::SetEchoSettings*)cmd;

    play->roomCtx.curRoom.echo = cmdEcho->echo;

    return false;
}

bool Scene_CommandAlternateHeaderList(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetAlternateHeaders* cmdHeaders = (Ship::SetAlternateHeaders*)cmd;

    //s32 pad;
    //SceneCmd* altHeader;

    //osSyncPrintf("\n[ZU]sceneset age    =[%X]", ((void)0, gSaveContext.linkAge));
    //osSyncPrintf("\n[ZU]sceneset time   =[%X]", ((void)0, gSaveContext.cutsceneIndex));
    //osSyncPrintf("\n[ZU]sceneset counter=[%X]", ((void)0, gSaveContext.sceneSetupIndex));

    if (gSaveContext.sceneSetupIndex != 0)
    {
        std::string desiredHeader = cmdHeaders->headers[gSaveContext.sceneSetupIndex - 1];
        Ship::Scene* headerData = nullptr;
        if (desiredHeader != "") {
            headerData = (Ship::Scene*)ResourceMgr_LoadResource(desiredHeader.c_str()).get();
        }

        if (headerData != nullptr)
        {
            OTRScene_ExecuteCommands(play, headerData);
            return true;
        }
        else
        {
            // "Coughh! There is no specified dataaaaa!"
            osSyncPrintf("\nげぼはっ！ 指定されたデータがないでええっす！");

            if (gSaveContext.sceneSetupIndex == 3)
            {
                std::string desiredHeader = cmdHeaders->headers[gSaveContext.sceneSetupIndex - 2];
                Ship::Scene* headerData = nullptr;
                if (desiredHeader != "") {
                    headerData = (Ship::Scene*)ResourceMgr_LoadResource(desiredHeader.c_str()).get();
                }

                // "Using adult day data there!"
                osSyncPrintf("\nそこで、大人の昼データを使用するでええっす！！");

                if (headerData != nullptr)
                {
                    OTRScene_ExecuteCommands(play, headerData);
                    return true;
                }
            }
        }
    }
    return false;
}

bool Scene_CommandCutsceneData(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetCutscenes* cmdCS = (Ship::SetCutscenes*)cmd;

    Ship::Cutscene* csData = (Ship::Cutscene*)ResourceMgr_LoadResource(cmdCS->cutscenePath.c_str()).get();
    play->csCtx.segment = csData->commands.data();

    //osSyncPrintf("\ngame_play->demo_play.data=[%x]", play->csCtx.segment);
    return false;
}

// Camera & World Map Area
bool Scene_CommandMiscSettings(PlayState* play, Ship::SceneCommand* cmd)
{
    Ship::SetCameraSettings* cmdCam = (Ship::SetCameraSettings*)cmd;

    YREG(15) = cmdCam->cameraMovement;
    gSaveContext.worldMapArea = cmdCam->mapHighlights;

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

bool (*sceneCommands[])(PlayState*, Ship::SceneCommand*) =
{
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

s32 OTRScene_ExecuteCommands(PlayState* play, Ship::Scene* scene)
{
    Ship::SceneCommandID cmdCode;

    for (int i = 0; i < scene->commands.size(); i++)
    {
        auto sceneCmd = scene->commands[i];

        if (sceneCmd == nullptr) // UH OH
            continue;


        cmdCode = sceneCmd->cmdID;
        //osSyncPrintf("*** Scene_Word = { code=%d, data1=%02x, data2=%04x } ***\n", cmdCode, sceneCmd->base.data1, sceneCmd->base.data2);

        if ((int)cmdCode == 0x14) {
            break;
        }

        if ((int)cmdCode <= 0x19) {
            if (sceneCommands[(int)cmdCode](play, sceneCmd))
                break;
        }
        else {
            osSyncPrintf(VT_FGCOL(RED));
            osSyncPrintf("code の値が異常です\n"); // "code variable is abnormal"
            osSyncPrintf(VT_RST);
        }

        //sceneCmd++;
    }
    return 0;
}

extern "C" s32 OTRfunc_800973FC(PlayState* play, RoomContext* roomCtx) {
    if (roomCtx->status == 1) {
        //if (!osRecvMesg(&roomCtx->loadQueue, NULL, OS_MESG_NOBLOCK)) {
        if (1)
        {
            roomCtx->status = 0;
            roomCtx->curRoom.segment = roomCtx->unk_34;
            gSegments[3] = VIRTUAL_TO_PHYSICAL(roomCtx->unk_34);

            OTRScene_ExecuteCommands(play, roomCtx->roomToLoad);
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
        roomCtx->unk_34 = (void*)ALIGN16((uintptr_t)roomCtx->bufPtrs[roomCtx->unk_30] - ((size + 8) * roomCtx->unk_30 + 7));

        osCreateMesgQueue(&roomCtx->loadQueue, &roomCtx->loadMsg, 1);
        //DmaMgr_SendRequest2(&roomCtx->dmaRequest, roomCtx->unk_34, play->roomList[roomNum].vromStart, size, 0,
                            //&roomCtx->loadQueue, NULL, __FILE__, __LINE__);

        auto roomData = ResourceMgr_LoadResource(play->roomList[roomNum].fileName);
        roomCtx->status = 1;
        roomCtx->roomToLoad = (Ship::Scene*)roomData.get();

        roomCtx->unk_30 ^= 1;

        return 1;
    }

    return 0;
}
