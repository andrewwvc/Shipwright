/*
 * File: z_bg_spot08_iceblock.c
 * Overlay: ovl_Bg_Spot08_Iceblock
 * Description: Floating ice platforms
 */

#include "z_bg_spot08_iceblock.h"
#include "objects/object_spot08_obj/object_spot08_obj.h"

#define FLAGS 0
#define MIN_SIZE_INC 0.003f

void BgSpot08Iceblock_Init(Actor* thisx, GlobalContext* globalCtx);
void BgSpot08Iceblock_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgSpot08Iceblock_Update(Actor* thisx, GlobalContext* globalCtx);
void BgSpot08Iceblock_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgSpot08Iceblock_SetupFloatNonrotating(BgSpot08Iceblock* this);
void BgSpot08Iceblock_FloatNonrotating(BgSpot08Iceblock* this, GlobalContext* globalCtx);
void BgSpot08Iceblock_SetupFloatRotating(BgSpot08Iceblock* this);
void BgSpot08Iceblock_FloatRotating(BgSpot08Iceblock* this, GlobalContext* globalCtx);
void BgSpot08Iceblock_SetupFloatOrbitingTwins(BgSpot08Iceblock* this);
void BgSpot08Iceblock_FloatOrbitingTwins(BgSpot08Iceblock* this, GlobalContext* globalCtx);
void BgSpot08Iceblock_SetupNoAction(BgSpot08Iceblock* this);

const ActorInit Bg_Spot08_Iceblock_InitVars = {
    ACTOR_BG_SPOT08_ICEBLOCK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT08_OBJ,
    sizeof(BgSpot08Iceblock),
    (ActorFunc)BgSpot08Iceblock_Init,
    (ActorFunc)BgSpot08Iceblock_Destroy,
    (ActorFunc)BgSpot08Iceblock_Update,
    (ActorFunc)BgSpot08Iceblock_Draw,
    NULL,
};

void BgSpot08Iceblock_SetupAction(BgSpot08Iceblock* this, BgSpot08IceblockActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void BgSpot08Iceblock_InitDynaPoly(BgSpot08Iceblock* this, GlobalContext* globalCtx, CollisionHeader* collision,
                                   s32 flags) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&this->dyna, flags);
    CollisionHeader_GetVirtual(collision, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, colHeader);
    if (this->dyna.bgId == BG_ACTOR_MAX) {
        // "Warning: move BG registration failed"
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", __FILE__, __LINE__,
                     this->dyna.actor.id, this->dyna.actor.params);
    }
}

// Sets params to 0x10 (medium, nonrotating) if not in the cases listed.
void BgSpot08Iceblock_CheckParams(BgSpot08Iceblock* this) {
    switch (this->dyna.actor.params & 0xFF) {
        case 0xFF:
            this->dyna.actor.params = 0x10;
            break;
        default:
            // "Error: arg_data setting error"
            osSyncPrintf("Error : arg_data 設定ミスです。(%s %d)(arg_data 0x%04x)\n", __FILE__, __LINE__,
                         this->dyna.actor.params);
            this->dyna.actor.params = 0x10;
            break;
        case 1:
        case 4:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x14:
        case 0x20:
        case 0x23:
        case 0x24:
            break;
    }
}

void BgSpot08Iceblock_Bobbing(BgSpot08Iceblock* this) {
    this->bobOffset = (Math_SinS(this->bobPhaseSlow) * 4.0f) + (Math_SinS(this->bobPhaseFast) * 3.0f);
}

void BgSpot08Iceblock_SinkUnderPlayer(BgSpot08Iceblock* this) {
    f32 target;
    f32 step;

    switch (this->dyna.actor.params & 0xF0) {
        case 0:
            step = 0.15f;
            break;
        case 0x10:
            step = 0.2f;
            break;
        case 0x20:
            step = 0.4f;
            break;
    }

    // Sink under Player's weight if standing on it
    target = (func_80043548(&this->dyna) ? -4.0f : 0.0f);

    Math_StepToF(&this->sinkOffset, target, step);
}

void BgSpot08Iceblock_SetWaterline(BgSpot08Iceblock* this) {
    this->dyna.actor.world.pos.y = this->sinkOffset + this->bobOffset + this->dyna.actor.home.pos.y;
}

void BgSpot08Iceblock_MultVectorScalar(Vec3f* dest, Vec3f* v, f32 scale) {
    dest->x = v->x * scale;
    dest->y = v->y * scale;
    dest->z = v->z * scale;
}

void BgSpot08Iceblock_CrossProduct(Vec3f* dest, Vec3f* v1, Vec3f* v2) {
    dest->x = (v1->y * v2->z) - (v1->z * v2->y);
    dest->y = (v1->z * v2->x) - (v1->x * v2->z);
    dest->z = (v1->x * v2->y) - (v1->y * v2->x);
}

s32 BgSpot08Iceblock_NormalizeVector(Vec3f* dest, Vec3f* v) {
    f32 magnitude;

    magnitude = Math3D_Vec3fMagnitude(v);
    if (magnitude < 0.001f) {
        dest->x = dest->y = 0.0f;
        dest->z = 1.0f;
        return false;
    } else {
        dest->x = v->x * (1.0f / magnitude);
        dest->y = v->y * (1.0f / magnitude);
        dest->z = v->z * (1.0f / magnitude);
        return true;
    }
}

static Vec3f sVerticalVector = { 0.0f, 1.0f, 0.0f };
static Vec3f sZeroVector = { 0.0f, 0.0f, 0.0f };
static f32 sInertias[] = { 1.0f / 70000000, 1.0f / 175000000, 1.0f / 700000000 };
static f32 sDampingFactors[] = { 0.96f, 0.96f, 0.98f };

static f32 sRollSins[] = {
    0.22495104f, // sin(13 degrees)
    0.22495104f, // sin(13 degrees)
    0.03489947f, // sin(2 degrees)
};

static f32 sRollCoss[] = {
    0.97437006f, // cos(13 degrees)
    0.97437006f, // cos(13 degrees)
    0.99939084f, // cos(2 degrees)
};

/**
 *  Handles all the factors that influence rolling: inertia, random oscillations, and most significantly, player weight,
 * and combines them to produce a matrix that rotates the actor to match the surface normal
 */
void BgSpot08Iceblock_Roll(BgSpot08Iceblock* this, GlobalContext* globalCtx) {
    f32 deviationFromVertSq;
    f32 stabilityCorrection;
    Vec3f surfaceNormalHorizontal;
    Vec3f playerCentroidDiff;
    Vec3f playerMoment;
    Vec3f surfaceNormalHorizontalScaled;
    Vec3f randomNutation;
    Vec3f tempVec; // reused with different meanings
    Vec3f torqueDirection;
    f32 playerCentroidDist;
    s32 rollDataIndex;
    MtxF mtx;
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);

    switch (this->dyna.actor.params & 0xFF) {
        case 0x11: // Medium nonrotating
            rollDataIndex = 0;
            break;
        case 1:
            rollDataIndex = 1; // Large nonrotating
            break;
        default:
            rollDataIndex = 2;
            break;
    }

    Math_Vec3f_Diff(&player->actor.world.pos, &this->dyna.actor.world.pos, &playerCentroidDiff);
    playerCentroidDiff.y -= (150.0f * this->dyna.actor.scale.y);
    playerCentroidDist = Math3D_Vec3fMagnitude(&playerCentroidDiff);

    randomNutation.x = (Rand_ZeroOne() - 0.5f) * (1.0f / 625);
    randomNutation.y = 0.0f;
    randomNutation.z = (Rand_ZeroOne() - 0.5f) * (1.0f / 625);

    surfaceNormalHorizontal.x = this->surfaceNormal.x;
    surfaceNormalHorizontal.y = 0.0f;
    surfaceNormalHorizontal.z = this->surfaceNormal.z;

    // If player is standing on it or holding the edge
    if (func_8004356C(&this->dyna) && (playerCentroidDist > 3.0f)) {
        Math_Vec3f_Diff(&playerCentroidDiff, &surfaceNormalHorizontal, &playerMoment);
        BgSpot08Iceblock_MultVectorScalar(&playerMoment, &playerMoment,
                                          (sInertias[rollDataIndex] * playerCentroidDist) / this->dyna.actor.scale.x);
    } else {
        playerMoment = sZeroVector;
    }

    BgSpot08Iceblock_MultVectorScalar(&surfaceNormalHorizontalScaled, &surfaceNormalHorizontal, -0.01f);

    // Add all three deviations
    Math_Vec3f_Sum(&this->normalDelta, &playerMoment, &this->normalDelta);
    Math_Vec3f_Sum(&this->normalDelta, &surfaceNormalHorizontalScaled, &this->normalDelta);
    Math_Vec3f_Sum(&this->normalDelta, &randomNutation, &this->normalDelta);

    this->normalDelta.y = 0.0f;

    Math_Vec3f_Sum(&this->surfaceNormal, &this->normalDelta, &tempVec);

    tempVec.x *= sDampingFactors[rollDataIndex];
    tempVec.z *= sDampingFactors[rollDataIndex];

    // Set up roll axis and final new angle
    if (BgSpot08Iceblock_NormalizeVector(&this->surfaceNormal, &tempVec)) {
        deviationFromVertSq = Math3D_Dist1DSq(this->surfaceNormal.z, this->surfaceNormal.x);

        // Prevent overrolling
        if (sRollSins[rollDataIndex] < deviationFromVertSq) {
            stabilityCorrection = sRollSins[rollDataIndex] / deviationFromVertSq;

            this->surfaceNormal.x *= stabilityCorrection;
            this->surfaceNormal.y = sRollCoss[rollDataIndex];
            this->surfaceNormal.z *= stabilityCorrection;
        }

        BgSpot08Iceblock_CrossProduct(&tempVec, &sVerticalVector, &this->surfaceNormal);

        if (BgSpot08Iceblock_NormalizeVector(&torqueDirection, &tempVec)) {
            this->rotationAxis = torqueDirection;
        }
    } else {
        this->surfaceNormal = sVerticalVector;
    }

    // Rotation by the angle between surfaceNormal and the vertical about rotationAxis
    Matrix_RotateAxis(Math_FAcosF(Math3D_Cos(&sVerticalVector, &this->surfaceNormal)), &this->rotationAxis,
                      MTXMODE_NEW);
    Matrix_RotateY(this->dyna.actor.shape.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);
    Matrix_Get(&mtx);
    Matrix_MtxFToYXZRotS(&mtx, &this->dyna.actor.shape.rot, 0);
}

void BgSpot08Iceblock_SpawnTwinFloe(BgSpot08Iceblock* this, GlobalContext* globalCtx) {
    s32 pad[2];
    f32 sin;
    f32 cos;

    sin = Math_SinS(this->dyna.actor.home.rot.y) * 100.0f;
    cos = Math_CosS(this->dyna.actor.home.rot.y) * 100.0f;

    if (!(this->dyna.actor.params & 0x100)) {
        Actor_SpawnAsChild(&globalCtx->actorCtx, &this->dyna.actor, globalCtx, ACTOR_BG_SPOT08_ICEBLOCK,
                           this->dyna.actor.home.pos.x, this->dyna.actor.home.pos.y, this->dyna.actor.home.pos.z,
                           this->dyna.actor.home.rot.x, this->dyna.actor.home.rot.y, this->dyna.actor.home.rot.z,
                           0x123);

        this->dyna.actor.world.pos.x += sin;
        this->dyna.actor.world.pos.z += cos;
    } else {
        this->dyna.actor.world.pos.x -= sin;
        this->dyna.actor.world.pos.z -= cos;
    }
    BgSpot08Iceblock_SetupFloatOrbitingTwins(this);
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 2200, ICHAIN_STOP),
};

f32 scaleTargets[] = {0.0f,0.05f,0.1f,0.2f};

void BgSpot08Iceblock_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot08Iceblock* this = (BgSpot08Iceblock*)thisx;
    CollisionHeader* colHeader;
    this->isThawing = 0;

    // "spot08 ice floe"
    osSyncPrintf("(spot08 流氷)(arg_data 0x%04x)\n", this->dyna.actor.params);
    BgSpot08Iceblock_CheckParams(this);

    switch (this->dyna.actor.params & 0x200) {
        case 0:
            colHeader = &gZorasFountainIcebergCol;
            break;
        case 0x200:
            colHeader = &gZorasFountainIceRampCol;
            break;
    }

    switch (this->dyna.actor.params & 0xF) {
        case 2:
        case 3:
            BgSpot08Iceblock_InitDynaPoly(this, globalCtx, colHeader, DPM_UNK3);
            break;
        default:
            BgSpot08Iceblock_InitDynaPoly(this, globalCtx, colHeader, DPM_UNK);
            break;
    }

    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        Actor_Kill(&this->dyna.actor);
        return;
    }

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);

    switch (this->dyna.actor.params & 0xF0) {
        case 0:
            Actor_SetScale(&this->dyna.actor, 0.2f);
            this->targetSize = 3;
            break;
        case 0x10:
            Actor_SetScale(&this->dyna.actor, 0.1f);
            this->targetSize = 2;
            break;
        case 0x20:
            Actor_SetScale(&this->dyna.actor, 0.05f);
            this->targetSize = 1;
            break;
    }

    if (this->dyna.actor.params & 0xF000){
        Actor_SetScale(&this->dyna.actor, MIN_SIZE_INC);
    }

    this->bobPhaseSlow = (s32)(Rand_ZeroOne() * (0xFFFF + 0.5f));
    this->bobPhaseFast = (s32)(Rand_ZeroOne() * (0xFFFF + 0.5f));
    this->surfaceNormal.y = 1.0f;
    this->rotationAxis.x = 1.0f;

    switch (this->dyna.actor.params & 0xF) {
        case 0:
        case 1:
            BgSpot08Iceblock_SetupFloatNonrotating(this);
            break;
        case 2:
            BgSpot08Iceblock_SetupFloatRotating(this);
            break;
        case 3:
            BgSpot08Iceblock_SpawnTwinFloe(this, globalCtx);
            break;
        case 4:
            BgSpot08Iceblock_SetupNoAction(this);
            break;
    }
}

void BgSpot08Iceblock_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot08Iceblock* this = (BgSpot08Iceblock*)thisx;

    if (thisx->parent && (((BgSpot08Iceblock*)(thisx->parent))->dyna.actor.params & 0xF) == 0x3) {
        ((BgSpot08Iceblock*)(thisx->parent))->dyna.actor.params |= 0x100;
        //((BgSpot08Iceblock*)(actor1->child))->dyna.actor.world.rot.y += 0x8000;
    }

    if (thisx->child)
        (thisx->child)->parent = NULL;

    if (thisx->parent)
        (thisx->parent)->child = NULL;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
}

void BgSpot08Iceblock_SetupFloatNonrotating(BgSpot08Iceblock* this) {
    BgSpot08Iceblock_SetupAction(this, BgSpot08Iceblock_FloatNonrotating);
}

void BgSpot08Iceblock_FloatNonrotating(BgSpot08Iceblock* this, GlobalContext* globalCtx) {
    BgSpot08Iceblock_Bobbing(this);
    BgSpot08Iceblock_SinkUnderPlayer(this);
    BgSpot08Iceblock_SetWaterline(this);
    this->dyna.actor.shape.rot.y = this->dyna.actor.home.rot.y;
    BgSpot08Iceblock_Roll(this, globalCtx);
}

void BgSpot08Iceblock_SetupFloatRotating(BgSpot08Iceblock* this) {
    BgSpot08Iceblock_SetupAction(this, BgSpot08Iceblock_FloatRotating);
}

void BgSpot08Iceblock_FloatRotating(BgSpot08Iceblock* this, GlobalContext* globalCtx) {
    BgSpot08Iceblock_Bobbing(this);
    BgSpot08Iceblock_SinkUnderPlayer(this);
    BgSpot08Iceblock_SetWaterline(this);
    this->dyna.actor.world.rot.y = this->dyna.actor.world.rot.y + 0x190;
    this->dyna.actor.shape.rot.y = this->dyna.actor.world.rot.y;
    BgSpot08Iceblock_Roll(this, globalCtx);
}

void BgSpot08Iceblock_SetupFloatOrbitingTwins(BgSpot08Iceblock* this) {
    BgSpot08Iceblock_SetupAction(this, BgSpot08Iceblock_FloatOrbitingTwins);
}

void BgSpot08Iceblock_FloatOrbitingTwins(BgSpot08Iceblock* this, GlobalContext* globalCtx) {
    f32 cos;
    f32 sin;

    BgSpot08Iceblock_Bobbing(this);
    BgSpot08Iceblock_SinkUnderPlayer(this);
    BgSpot08Iceblock_SetWaterline(this);

    // parent handles rotations of both
    if (!(this->dyna.actor.params & 0x100)) {
        this->dyna.actor.world.rot.y += 0x190;
        sin = Math_SinS(this->dyna.actor.world.rot.y) * 100.0f;
        cos = Math_CosS(this->dyna.actor.world.rot.y) * 100.0f;

        this->dyna.actor.world.pos.x = this->dyna.actor.home.pos.x + sin;
        this->dyna.actor.world.pos.z = this->dyna.actor.home.pos.z + cos;

        CollisionPoly* colPol;
        s32 backgroundID;
        Vec3f centerPoint;
        BgActor* bgActor = &(globalCtx->colCtx).dyna.bgActors[this->dyna.bgId];
        VEC_SET(centerPoint,bgActor->boundingSphere.center.x,bgActor->boundingSphere.center.y,bgActor->boundingSphere.center.z);
        if (BgCheck_SphVsFirstPolyImpl(&globalCtx->colCtx,0,&colPol,&backgroundID,&centerPoint,bgActor->boundingSphere.radius*(1.0f/1.1f),&this->dyna.actor,0) &&
                    !(((BgSpot08Iceblock*)this->dyna.actor.child)->dyna.bgId == backgroundID)) {
            DynaPolyActor* dyna1 = DynaPoly_GetActor(&globalCtx->colCtx,backgroundID);
            if (dyna1) {
                if (dyna1->actor.id == ACTOR_BG_SPOT08_ICEBLOCK && !((BgSpot08Iceblock*)(&dyna1->actor))->isThawing) {
                    //BgSpot08Iceblock_thaw((BgSpot08Iceblock*)(&dyna1->actor));
                    DECR(((BgSpot08Iceblock*)(&dyna1->actor))->targetSize);
                    ((BgSpot08Iceblock*)(&dyna1->actor))->isThawing = 1;
                }
            }
        }

        if (this->dyna.actor.child != NULL) {
            this->dyna.actor.child->world.pos.x = this->dyna.actor.home.pos.x - sin;
            this->dyna.actor.child->world.pos.z = this->dyna.actor.home.pos.z - cos;

            BgSpot08Iceblock* this2 = (BgSpot08Iceblock*)this->dyna.actor.child;

            CollisionPoly* colPol2;
            s32 backgroundID2;
            Vec3f centerPoint2;
            BgActor* bgActor2 = &(globalCtx->colCtx).dyna.bgActors[this2->dyna.bgId];
            VEC_SET(centerPoint2,bgActor2->boundingSphere.center.x,bgActor2->boundingSphere.center.y,bgActor2->boundingSphere.center.z);
            if (BgCheck_SphVsFirstPolyImpl(&globalCtx->colCtx,0,&colPol2,&backgroundID2,&centerPoint2,bgActor2->boundingSphere.radius*(1.0f/1.1f),&this2->dyna.actor,0) &&
                        !(this->dyna.bgId == backgroundID2)) {
                DynaPolyActor* dyna2 = DynaPoly_GetActor(&globalCtx->colCtx,backgroundID2);
                if (dyna2) {
                    if (dyna2->actor.id == ACTOR_BG_SPOT08_ICEBLOCK  && !((BgSpot08Iceblock*)(&dyna2->actor))->isThawing) {
                        //BgSpot08Iceblock_thaw((BgSpot08Iceblock*)(&dyna2->actor));
                        DECR(((BgSpot08Iceblock*)(&dyna2->actor))->targetSize);
                        ((BgSpot08Iceblock*)(&dyna2->actor))->isThawing = 1;
                    }
                }
            }
        }
    }

    this->dyna.actor.shape.rot.y = this->dyna.actor.home.rot.y;
    BgSpot08Iceblock_Roll(this, globalCtx);
}

void BgSpot08Iceblock_SetupNoAction(BgSpot08Iceblock* this) {
    BgSpot08Iceblock_SetupAction(this, NULL);
}

s8 BgSpot08Iceblock_freeze(Actor* thisx) {
    BgSpot08Iceblock* this = (BgSpot08Iceblock*)thisx;
    if (!(this->dyna.actor.params & 0x200)) {
        if (this->targetSize < 3)
            this->targetSize++;
        return 1;
    }
    return 0;
}

s8 BgSpot08Iceblock_thaw(Actor* thisx) {
    BgSpot08Iceblock* this = (BgSpot08Iceblock*)thisx;
    if (!(this->dyna.actor.params & 0x200)) {
        Audio_PlayActorSound2(this, NA_SE_EV_ICE_MELT);
        this->isThawing = 2;
        if (this->targetSize > 0)
            this->targetSize--;
        return 1;
    }
    return 0;
}

//#define ICE_WILL_MOVE 1

void BgSpot08Iceblock_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot08Iceblock* this = (BgSpot08Iceblock*)thisx;

    if (Rand_ZeroOne() < 0.05f) {
        this->bobIncrSlow = Rand_S16Offset(300, 100);
        this->bobIncrFast = Rand_S16Offset(800, 400);
    }

    if (thisx->scale.z < scaleTargets[this->targetSize]) {
        Actor_SetScale(thisx, thisx->scale.z+MIN_SIZE_INC);
        if (thisx->scale.z >= scaleTargets[this->targetSize])
            Actor_SetScale(thisx, scaleTargets[this->targetSize]);
        if (globalCtx->gameplayFrames % 3)
            Audio_PlayActorSound2(thisx, NA_SE_PL_FREEZE_S);
        CollisionPoly* colPol;
        s32 backgroundID;
        Vec3f centerPoint;
        BgActor* bgActor = &(globalCtx->colCtx).dyna.bgActors[this->dyna.bgId];
        VEC_SET(centerPoint,bgActor->boundingSphere.center.x,bgActor->boundingSphere.center.y,bgActor->boundingSphere.center.z);
        if (BgCheck_SphVsFirstPolyImpl(&globalCtx->colCtx,0,&colPol,&backgroundID,&centerPoint,bgActor->boundingSphere.radius*(1.0f/1.1f),thisx,0)) {
#ifdef ICE_WILL_MOVE
            Vec3f vertices[3];
            Vec3f avg;
            CollisionPoly_GetVerticesByBgId(colPol,backgroundID,&globalCtx->colCtx,vertices);
            Math_Vec3f_Sum(&vertices[0],&vertices[1],&avg);
            Math_Vec3f_Scale(&avg,0.5);
            Math_Vec3f_Sum(&avg,&vertices[2],&avg);
            Math_Vec3f_Scale(&avg,0.5);
            avg.x = centerPoint.x-avg.x;
            avg.z = centerPoint.z-avg.z;
            avg.y = 0;
            f32 abs = sqrt(SQXZ(avg));
            avg.x /= abs;
            avg.z /= abs;
            const f32 FUDGE_FACTOR = 2.0F;
            f32 distToMove = bgActor->boundingSphere.radius*(1.0f/1.1f)*(MIN_SIZE_INC/thisx->scale.z);
            distToMove *= FUDGE_FACTOR;
            centerPoint.x += avg.x*distToMove;
            centerPoint.z += avg.z*distToMove;
            if (BgCheck_SphVsFirstPolyImpl(&globalCtx->colCtx,0,&colPol,&backgroundID,&centerPoint,bgActor->boundingSphere.radius*(1.0f/1.1f),thisx,0)) {
#endif
                Actor_SetScale(thisx, thisx->scale.z-MIN_SIZE_INC);
                DECR(this->targetSize);
#ifdef ICE_WILL_MOVE
            } else {
                this->dyna.actor.world.pos.x += avg.x*distToMove;
                this->dyna.actor.world.pos.z += avg.z*distToMove;
            }
#endif
        }
    }

    if (thisx->scale.z > scaleTargets[this->targetSize]) {
        Actor_SetScale(thisx, thisx->scale.z-MIN_SIZE_INC);
        Vec3f smokePos;
        smokePos.y = thisx->world.pos.y +10.0f;
        smokePos.x = thisx->world.pos.x +thisx->scale.x*Rand_Centered()*1500.0f;
        smokePos.z = thisx->world.pos.z +thisx->scale.z*Rand_Centered()*1500.0f;
        static Vec3f velocity = {0.0f,3.0f,0.0f};
        static Vec3f accel = {0.0f,-0.02f,0.0f};
        if (this->isThawing == 2)
            EffectSsIceSmoke_Spawn(globalCtx,&smokePos,&velocity,&accel,300*(5*thisx->scale.z+0.3f));
        else
            this->isThawing = 1;
        if (thisx->scale.z <= scaleTargets[this->targetSize]){
            Actor_SetScale(thisx, scaleTargets[this->targetSize]);
            this->isThawing = 0;
            if (this->targetSize == 0)
                Actor_Kill(&this->dyna.actor);
        }
    }

    this->bobPhaseSlow += this->bobIncrSlow;
    this->bobPhaseFast += this->bobIncrFast;
    if (this->actionFunc != NULL) {
        this->actionFunc(this, globalCtx);
    }
}

void BgSpot08Iceblock_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx* dList;
    BgSpot08Iceblock* this = (BgSpot08Iceblock*)thisx;

    switch (this->dyna.actor.params & 0x200) {
        case 0:
            dList = gZorasFountainIcebergDL;
            break;
        case 0x200:
            dList = gZorasFountainIceRampDL;
            break;
    }

    Gfx_DrawDListOpa(globalCtx, dList);
}
