#include "stdafx.h"
#include "Rubbish.h"

#include "Game.h"
#include "TxdStore.h"
#include "Camera.h"
#include "World.h"
#include "Weather.h"
#include "Timer.h"
#include "Vehicle.h"
#include "debugmenu_public.h"

int CRubbish::RubbishVisibility;
bool CRubbish::bRubbishInvisible;
COneSheet CRubbish::aSheets[CRubbish::NUMSHEETS];
COneSheet CRubbish::StartEmptyList, CRubbish::EndEmptyList;
COneSheet CRubbish::StartStaticsList, CRubbish::EndStaticsList;
COneSheet CRubbish::StartMoversList, CRubbish::EndMoversList;
RwTexture *CRubbish::gpRubbishTexture[4];

void
COneSheet::AddToList(COneSheet *list)
{
    this->prev = list;
    this->next = list->next;
    list->next = this;
    this->next->prev = this;
}

void
COneSheet::RemoveFromList(void)
{
    this->next->prev = this->prev;
    this->prev->next = this->next;
}

void
CRubbish::Init(void)
{
    int i;
    for (i = 0; i < NUMSHEETS; i++) {
        aSheets[i].type = 0;
        if (i == 0)
            aSheets[i].prev = &StartEmptyList;
        else
            aSheets[i].prev = &aSheets[i - 1];
        if (i + 1 >= NUMSHEETS)
            aSheets[i].next = &EndEmptyList;
        else
            aSheets[i].next = &aSheets[i + 1];
    }

    StartEmptyList.next = &aSheets[0];
    StartEmptyList.prev = NULL;
    EndEmptyList.next = NULL;
    EndEmptyList.prev = &aSheets[NUMSHEETS - 1];

    StartStaticsList.next = &EndStaticsList;
    StartStaticsList.prev = NULL;
    EndStaticsList.next = NULL;
    EndStaticsList.prev = &StartStaticsList;

    StartMoversList.next = &EndMoversList;
    StartMoversList.prev = NULL;
    EndMoversList.next = NULL;
    EndMoversList.prev = &StartMoversList;

    CTxdStore::PushCurrentTxd();
    CTxdStore::SetCurrentTxd(CTxdStore::FindTxdSlot("particle"));
    gpRubbishTexture[0] = RwTextureRead("gameleaf01_64", NULL);
    gpRubbishTexture[1] = RwTextureRead("gameleaf02_64", NULL);
    gpRubbishTexture[2] = RwTextureRead("newspaper01_64", NULL);
    gpRubbishTexture[3] = RwTextureRead("newspaper02_64", NULL);
    CTxdStore::PopCurrentTxd();
    RubbishVisibility = 255;
    bRubbishInvisible = false;
}

void
CRubbish::StirUp(CVehicle *vehicle)
{
    if ((CTimer::m_FrameCounter ^ vehicle->RandomSeed) & 3)
        return;

    CVector campos = TheCamera.GetCoords();
    CVector vehpos = vehicle->GetCoords();
    if (abs(campos.x - vehpos.x) > 20.0f ||
        abs(campos.y - vehpos.y) > 20.0f)
        return;

    CVector speedvec = vehicle->GetLinearVelocity();
    if (abs(speedvec.x) <= 0.05f && abs(speedvec.y) <= 0.05f)
        return;

    float speed = CVector(speedvec.x, speedvec.y, 0.0f).Magnitude();
    if (speed <= 0.05f)
        return;

    CVector *up = vehicle->GetMatrix()->GetUp();
    CVector *right = vehicle->GetMatrix()->GetRight();
    float speed_dot_up = speedvec.x*up->x + speedvec.y*up->y;

    CColModel *col = CModelInfo::GetModelInfo(vehicle->GetModelIndex())->GetColModel();
    float length = col->boundingBox.vecMax.y;
    float width = col->boundingBox.vecMax.x;

    COneSheet *sheet, *next;
    for (sheet = StartStaticsList.next; sheet != &EndStaticsList; sheet = next) {
        next = sheet->next;
        float dx = sheet->vec1.x - vehpos.x;
        float dy = sheet->vec1.y - vehpos.y;
        float dist_dot_up = dx * up->x + dy * up->y;
        float dist_dot_right = abs(dx*right->x + dy * right->y);

        // this is all weird
        if (speed_dot_up > 0.0f && dist_dot_up < -0.5f*length && dist_dot_up > -1.5f*length ||
            speed_dot_up <= 0.0f && dist_dot_up > 0.5f*length && dist_dot_up < 1.5f*length) {
            float s = dist_dot_right >= width ? 0.5f*speed : speed;
            if (dist_dot_right < 1.5f*width && s > 0.05f) {
                sheet->type = 2;
                sheet->movementType = s <= 0.15f ? 1 : 2;
                sheet->duration = 2000;
                float l = sqrt(speedvec.x*speedvec.x + speedvec.y*speedvec.y);
                sheet->moveOffsetX = speedvec.x / l * speed*25.0f;
                sheet->moveOffsetY = speedvec.y / l * speed*25.0f;
                sheet->moveOffsetZ = speed * 3.0f;
                sheet->startTime = CTimer::m_snTimeInMilliseconds;
                sheet->moveTargetZ = CWorld::FindGroundZFor3DCoord(
                    sheet->vec1.x + sheet->moveOffsetX,
                    sheet->vec1.y + sheet->moveOffsetY,
                    sheet->vec1.z + 3.0f, NULL, NULL) + 0.1f;
                sheet->RemoveFromList();
                sheet->AddToList(&StartMoversList);
            }
        }
    }
}

static uint8 rand8(void) { return rand(); }

WRAPPER int CCullZones__FindAttributesForCoors(float x, float y, float z) { EAXJMP(0x72D970); }

static float
FindGroundForRubbish(float x, float y, float z, bool *success)
{
    float groundz = CWorld::FindGroundZFor3DCoord(x, y, z, success, NULL);
    return groundz;

    // get the water to work right :/
    //	if(CWaterLevel::GetWaterLevel(x, y, z, &waterz, false, NULL))
    //		;
}

void
CRubbish::Update(void)
{
    COneSheet *sheet;

    if (bRubbishInvisible) {
        RubbishVisibility -= 5;
        if (RubbishVisibility < 0)
            RubbishVisibility = 0;
    }
    else {
        RubbishVisibility += 5;
        if (RubbishVisibility > 255)
            RubbishVisibility = 255;
    }

    CVector campos = TheCamera.GetCoords();

    // Generate new sheets
    sheet = StartEmptyList.next;
    if (sheet != &EndEmptyList) {
        float radius = rand8() / 255.0f + 23.0f;
        float angle;
        uint8 r = rand8();
        if (r & 1)
            angle = rand8() / 255.0f * M_PI*2.0f;
        else
            angle = (r - 128) / 160.0f + TheCamera.Orientation;
        sheet->vec1.x = sin(angle)*radius + campos.x;
        sheet->vec1.y = cos(angle)*radius + campos.y;
        bool foundGround;
        sheet->vec1.z = FindGroundForRubbish(sheet->vec1.x, sheet->vec1.y, campos.z, &foundGround) + 0.1f;
        if (foundGround) {
            sheet->angle = rand8() / 255.0f * M_PI*2.0f;
            sheet->type = 1;
            if (CCullZones__FindAttributesForCoors(sheet->vec1.x, sheet->vec1.y, sheet->vec1.z) & 8)
                sheet->zoneVisible1 = 0;
            else
                sheet->zoneVisible1 = 1;
            sheet->RemoveFromList();
            sheet->AddToList(&StartStaticsList);
        }
    }

    static float movement[3][34] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

    { 0, 0.05, 0.12, 0.25, 0.42, 0.57, 0.68, 0.8, 0.86, 0.9, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1,	// XY movemnt
    0.15, 0.35, 0.6, 0.9, 1.2, 1.25, 1.3, 1.2, 1.1, 0.95, 0.8, 0.6, 0.45, 0.3, 0.2, 0.1, 0 },	// Z movement

    { 0, 0.05, 0.12, 0.25, 0.42, 0.57, 0.68, 0.8, 0.95, 1.1, 1.15, 1.18, 1.15, 1.1, 1.05, 1.03, 1,
    0.15, 0.35, 0.6, 0.9, 1.2, 1.25, 1.3, 1.2, 1.1, 0.95, 0.8, 0.6, 0.45, 0.3, 0.2, 0.1, 0 }
    };
    // Update Movers
    COneSheet *next;
    for (sheet = StartMoversList.next; sheet != &EndMoversList; sheet = next) {
        next = sheet->next;
        int t = CTimer::m_snTimeInMilliseconds - sheet->startTime;
        if (t < sheet->duration) {
            int step = 16 * t / sheet->duration;	// 16 steps
            float stepInterp = (t - sheet->duration / 16.0f * step) / (sheet->duration / 16.0f);	// interpolation along step
            float interp = (float)t / sheet->duration;	// interpolation along total animation
            float moveMult = stepInterp * movement[sheet->movementType][step + 1] +
                (1.0f - stepInterp) * movement[sheet->movementType][step];
            float moveMultZ = stepInterp * movement[sheet->movementType][step + 1 + 17] +
                (1.0f - stepInterp) * movement[sheet->movementType][step + 17];

            sheet->vec2.x = sheet->vec1.x + sheet->moveOffsetX*moveMult;
            sheet->vec2.y = sheet->vec1.y + sheet->moveOffsetY*moveMult;
            sheet->vec2.z = interp * sheet->moveTargetZ + (1.0f - interp) * sheet->vec1.z +
                sheet->moveOffsetZ*moveMultZ;
            sheet->angle += CTimer::ms_fTimeStep / 25.0f;
            if (sheet->angle > 2 * M_PI)
                sheet->angle -= 2 * M_PI;
        }
        else {
            // Move done, make static again
            sheet->vec1.x += sheet->moveOffsetX;
            sheet->vec1.y += sheet->moveOffsetY;
            sheet->vec1.z = sheet->moveTargetZ;
            sheet->type = 1;
            sheet->zoneVisible1 = sheet->zoneVisible2;
            sheet->RemoveFromList();
            sheet->AddToList(&StartStaticsList);
        }
    }

    int freq;
    if (CWeather::Wind < 0.1f)
        freq = 0x1F;
    else if (CWeather::Wind < 0.4f)
        freq = 7;
    else if (CWeather::Wind < 0.7f)
        freq = 1;
    else
        freq = 0;

    // Turn Statics into Movers
    if ((CTimer::m_FrameCounter & freq) == 0) {
        int i = rand() & (NUMSHEETS - 1);
        sheet = &aSheets[i];
        if (sheet->type == 1) {
            sheet->startTime = CTimer::m_snTimeInMilliseconds;
            sheet->duration = CWeather::Wind * 1500.0f + 1000.0f;
            sheet->moveOffsetZ = 0.2f;
            sheet->moveOffsetX = CWeather::Wind * 3.0f;
            sheet->moveOffsetY = CWeather::Wind * 3.0f;
            bool foundGround;
            sheet->moveTargetZ = FindGroundForRubbish(sheet->vec1.x + sheet->moveOffsetX, sheet->vec1.y + sheet->moveOffsetY, sheet->vec1.z, &foundGround) + 0.1f;
            if (CCullZones__FindAttributesForCoors(sheet->vec1.x + sheet->moveOffsetX, sheet->vec1.y + sheet->moveOffsetY, sheet->moveTargetZ) & 8)
                sheet->zoneVisible2 = 0;
            else
                sheet->zoneVisible2 = 1;
            if (foundGround) {
                sheet->type = 2;
                sheet->movementType = 1;
                sheet->RemoveFromList();
                sheet->AddToList(&StartMoversList);
            }
        }
    }

    // Remove sheets that are too far away
    // Actually only two per frame
    int i;
    for (i = 0; i < NUMSHEETS; i++) {
        if (aSheets[i].type != 1)
            continue;
        if (aSheets[i].vec1.DistanceXY(campos) > 24.0f) {
            aSheets[i].type = 0;
            aSheets[i].RemoveFromList();
            aSheets[i].AddToList(&StartEmptyList);
        }
    }
}

static int TempBufferIndicesStored;
static int TempBufferVerticesStored;
static RwIm3DVertex TempVertexBuffer[512];
static RwImVertexIndex TempBufferRenderIndexList[1024];

void
CRubbish::Render(void)
{
    if (RubbishVisibility == 0 || CGame::currArea > 0)
        return;

    int alphafunc;
    RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTION, &alphafunc);
    RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONALWAYS);

    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);

    int textype;
    for (textype = 0; textype < 4; textype++) {
        if (textype < 3)
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, RwTextureGetRaster(gpRubbishTexture[textype]));
        TempBufferIndicesStored = 0;
        TempBufferVerticesStored = 0;

        for (COneSheet *sheet = &aSheets[textype * (NUMSHEETS / 4)];
            sheet < &aSheets[(textype + 1) * (NUMSHEETS / 4)];
            sheet++) {
            if (sheet->type == 0)
                continue;

            int alpha = 100;
            CVector pos;
            if (sheet->type == 1) {
                pos = sheet->vec1;
                if (!sheet->zoneVisible1)
                    alpha = 0;
            }
            else {
                pos = sheet->vec2;
                if (!sheet->zoneVisible1 || !sheet->zoneVisible2) {
                    float t = (float)(CTimer::m_snTimeInMilliseconds - sheet->startTime) / sheet->duration;
                    float f1 = sheet->zoneVisible1 ? 1.0f - t : 0.0f;
                    float f2 = sheet->zoneVisible2 ? t : 0.0f;
                    alpha = 100 * (f1 + f2);
                }
            }

            float dist = pos.DistanceXY(TheCamera.GetCoords());
            if (dist > 23.0f)
                continue;
            if (dist > 20.0f)
                alpha -= alpha * (dist - 20.0f) / (23.0f - 20.0f);

            float vx1, vx2;
            float vy1, vy2;
            static float sizes[4] = { 0.4, 0.8, 0.3, 0.3 };	// all square in VCS

            vx1 = sin(sheet->angle) * sizes[textype];
            vy1 = cos(sheet->angle) * sizes[textype];
            vx2 = cos(sheet->angle) * sizes[textype];
            vy2 = -sin(sheet->angle) * sizes[textype];

            alpha = RubbishVisibility * alpha / 255;

            int i = TempBufferVerticesStored;
            RwIm3DVertexSetPos(&TempVertexBuffer[i + 0], pos.x + vx1 + vx2, pos.y + vy1 + vy2, pos.z);
            RwIm3DVertexSetPos(&TempVertexBuffer[i + 1], pos.x + vx1 - vx2, pos.y + vy1 - vy2, pos.z);
            RwIm3DVertexSetPos(&TempVertexBuffer[i + 2], pos.x - vx1 + vx2, pos.y - vy1 + vy2, pos.z);
            RwIm3DVertexSetPos(&TempVertexBuffer[i + 3], pos.x - vx1 - vx2, pos.y - vy1 - vy2, pos.z);
            RwIm3DVertexSetRGBA(&TempVertexBuffer[i + 0], 255, 255, 255, alpha);
            RwIm3DVertexSetRGBA(&TempVertexBuffer[i + 1], 255, 255, 255, alpha);
            RwIm3DVertexSetRGBA(&TempVertexBuffer[i + 2], 255, 255, 255, alpha);
            RwIm3DVertexSetRGBA(&TempVertexBuffer[i + 3], 255, 255, 255, alpha);
            RwIm3DVertexSetU(&TempVertexBuffer[i + 0], 0.0f);
            RwIm3DVertexSetV(&TempVertexBuffer[i + 0], 0.0f);
            RwIm3DVertexSetU(&TempVertexBuffer[i + 1], 1.0f);
            RwIm3DVertexSetV(&TempVertexBuffer[i + 1], 0.0f);
            RwIm3DVertexSetU(&TempVertexBuffer[i + 2], 0.0f);
            RwIm3DVertexSetV(&TempVertexBuffer[i + 2], 1.0f);
            RwIm3DVertexSetU(&TempVertexBuffer[i + 3], 1.0f);
            RwIm3DVertexSetV(&TempVertexBuffer[i + 3], 1.0f);

            TempBufferRenderIndexList[TempBufferIndicesStored++] = TempBufferVerticesStored + 0;
            TempBufferRenderIndexList[TempBufferIndicesStored++] = TempBufferVerticesStored + 1;
            TempBufferRenderIndexList[TempBufferIndicesStored++] = TempBufferVerticesStored + 2;
            TempBufferRenderIndexList[TempBufferIndicesStored++] = TempBufferVerticesStored + 1;
            TempBufferRenderIndexList[TempBufferIndicesStored++] = TempBufferVerticesStored + 3;
            TempBufferRenderIndexList[TempBufferIndicesStored++] = TempBufferVerticesStored + 2;
            TempBufferVerticesStored += 4;
        }

        if (TempBufferIndicesStored) {
            if (RwIm3DTransform(TempVertexBuffer, TempBufferVerticesStored, NULL, rwIM3D_VERTEXUV)) {
                RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, TempBufferRenderIndexList, TempBufferIndicesStored);
                RwIm3DEnd();
            }
        }
    }

    RwRenderStateSet(rwRENDERSTATEFOGENABLE, 0);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

    RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)alphafunc);
}

void
CRubbish::SetVisibility(bool v)
{
    bRubbishInvisible = !v;
}

static StaticPatcher Patcher([]() {
    Memory::InjectHook(0x7204C0, CRubbish::SetVisibility, PATCH_JUMP);
    if (DebugMenuLoad()) {
        DebugMenuAddVarBool8("Rendering", "Rubbish invisible", (int8*)&CRubbish::bRubbishInvisible, NULL);
    }
});
