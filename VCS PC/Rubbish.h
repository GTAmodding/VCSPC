#ifndef __RUBBISH
#define __RUBBISH

struct COneSheet
{
    CVector vec1;
    CVector vec2;
    float moveTargetZ;
    int8 type;
    int8 movementType;
    int32 startTime;
    int32 duration;
    float moveOffsetZ;
    float moveOffsetX;
    float moveOffsetY;
    float angle;
    int8 zoneVisible1;
    int8 zoneVisible2;
    int8 unk72;
    int8 unk73;
    COneSheet *next;
    COneSheet *prev;

    void AddToList(COneSheet *list);
    void RemoveFromList(void);
};

class CVehicle;

class CRubbish
{
public:
    // MUST be a power of 2
    enum { NUMSHEETS = 32 };

    static int RubbishVisibility;
    static bool bRubbishInvisible;
    static COneSheet aSheets[NUMSHEETS];
    static COneSheet StartEmptyList, EndEmptyList;
    static COneSheet StartStaticsList, EndStaticsList;
    static COneSheet StartMoversList, EndMoversList;
    static RwTexture *gpRubbishTexture[4];

    static void Init(void);
    static void Update(void);
    static void StirUp(CVehicle *vehicle);
    static void Render(void);
    static void SetVisibility(bool v);
};

#endif
