#ifndef __CCAM
#define __CCAM

#define NUMBER_OF_VECTORS_FOR_AVERAGE   2
#define CAM_NUM_TARGET_HISTORY          4

class CAutomobile;
class CPed;

class CCam
{
public:
    bool				bBelowMinDist;						// 0x0 //used for follow ped mode
    bool				bBehindPlayerDesired;				// 0x1 //used for follow ped mode
    bool				m_bCamLookingAtVector;				// 0x2
    bool				m_bCollisionChecksOn;				// 0x3
    bool				m_bFixingBeta;						// 0x4 //used for camera on a string
    bool				m_bTheHeightFixerVehicleIsATrain;	// 0x5
    bool				LookBehindCamWasInFront;			// 0x6
    bool				LookingBehind;						// 0x7
    bool				LookingLeft;						// 0x8
    bool				LookingRight;						// 0x9
    bool				ResetStatics;						// 0xA //for interpolation type stuff to work
    bool				Rotating;							// 0xB

    short				Mode;								// 0xC // CameraMode
    unsigned int		m_uiFinishTime;						// 0x10
    
    int					m_iDoCollisionChecksOnFrameNum;		// 0x14
    int					m_iDoCollisionCheckEveryNumOfFrames;	// 0x18
    int					m_iFrameNumWereAt;					// 0x1C
    int					m_iRunningVectorArrayPos;			// 0x20
    int					m_iRunningVectorCounter;			// 0x24
    int					DirectionWasLooking;				// 0x28
    
    float   			f_max_role_angle;					// 0x2C //=DEGTORAD(5.0f);    
    float   			f_Roll;								// 0x30 //used for adding a slight roll to the camera in the
    float   			f_rollSpeed;						// 0x34 //camera on a string mode
    float   			m_fSyphonModeTargetZOffSet;			// 0x38
    float   			m_fAmountFractionObscured;			// 0x3C
    float   			m_fAlphaSpeedOverOneFrame;			// 0x40
    float   			m_fBetaSpeedOverOneFrame;			// 0x44
    float   			m_fBufferedTargetBeta;				// 0x48
    float   			m_fBufferedTargetOrientation;		// 0x4C
    float   			m_fBufferedTargetOrientationSpeed;	// 0x50
    float   			m_fCamBufferedHeight;				// 0x54
    float   			m_fCamBufferedHeightSpeed;			// 0x58
    float   			m_fCloseInPedHeightOffset;			// 0x5C
    float   			m_fCloseInPedHeightOffsetSpeed;		// 0x60
    float   			m_fCloseInCarHeightOffset;			// 0x64
    float   			m_fCloseInCarHeightOffsetSpeed;		// 0x68
    float   			m_fDimensionOfHighestNearCar;		// 0x6C
    float   			m_fDistanceBeforeChanges;			// 0x70
    float   			m_fFovSpeedOverOneFrame;				// 0x74
    float   			m_fMinDistAwayFromCamWhenInterPolating;	// 0x78
    float   			m_fPedBetweenCameraHeightOffset;		// 0x7C
    float   			m_fPlayerInFrontSyphonAngleOffSet;		// 0x80
    float   			m_fRadiusForDead;					// 0x84
    float   			m_fRealGroundDist;					// 0x88 //used for follow ped mode
    float   			m_fTargetBeta;						// 0x8C
    float   			m_fTimeElapsedFloat;				// 0x90
    float   			m_fTilt;							// 0x94
    float   			m_fTiltSpeed;						// 0x98

    float   			m_fTransitionBeta;					// 0x9C
    float   			m_fTrueBeta;						// 0xA0
    float   			m_fTrueAlpha;						// 0xA4
    float   			m_fInitialPlayerOrientation;		// 0xA8 //used for first person

    float   			Alpha;								// 0xAC
    float   			AlphaSpeed;							// 0xB0
    float   			FOV;								// 0xB4
    float   			FOVSpeed;							// 0xB8
    float   			Beta;								// 0xBC
    float   			BetaSpeed;							// 0xC0
    float   			Distance;							// 0xC4
    float   			DistanceSpeed;						// 0xC8
    float   			CA_MIN_DISTANCE;					// 0xCC
    float   			CA_MAX_DISTANCE;					// 0xD0
    float   			SpeedVar;							// 0xD4
    float				m_fCameraHeightMultiplier;			// 0xD8 //used by TwoPlayer_Separate_Cars_TopDown
    
    // ped onfoot zoom distance
    float				m_fTargetZoomGroundOne;				// 0xDC
    float				m_fTargetZoomGroundTwo;				// 0xE0
    float				m_fTargetZoomGroundThree;			// 0xE4
    // ped onfoot alpha angle offset
    float				m_fTargetZoomOneZExtra;				// 0xE8
    float				m_fTargetZoomTwoZExtra;				// 0xEC
    float				m_fTargetZoomTwoInteriorZExtra; 	// 0xF0 //extra one for interior
    float				m_fTargetZoomThreeZExtra;			// 0xF4
    
    float				m_fTargetZoomZCloseIn;				// 0xF8
    float				m_fMinRealGroundDist;				// 0xFC
    float				m_fTargetCloseInDist;				// 0x100

    // For targetting in cooperative mode.
    float				Beta_Targeting;						// 0x104
    float				X_Targetting, Y_Targetting;			// 0x108
    int					CarWeAreFocussingOn;				// 0x110 //which car is closer to the camera in coop mode with separate cars
    float				CarWeAreFocussingOnI;				// 0x114 //interpolated version
    
    float				m_fCamBumpedHorz;					// 0x118
    float				m_fCamBumpedVert;					// 0x11C
    int					m_nCamBumpedTime;					// 0x120
    static int			CAM_BUMPED_SWING_PERIOD;
    static int			CAM_BUMPED_END_TIME;
    static float		CAM_BUMPED_DAMP_RATE;
    static float		CAM_BUMPED_MOVE_MULT;

    CVector 			m_cvecSourceSpeedOverOneFrame;		// 0x124
    CVector 			m_cvecTargetSpeedOverOneFrame;		// 0x130
    CVector 			m_cvecUpOverOneFrame;				// 0x13C
    
    CVector 			m_cvecTargetCoorsForFudgeInter; 	// 0x148
    CVector 			m_cvecCamFixedModeVector;			// 0x154
    CVector 			m_cvecCamFixedModeSource;			// 0x160
    CVector				m_cvecCamFixedModeUpOffSet;			// 0x16C
    CVector				m_vecLastAboveWaterCamPosition;		// 0x178 //helper for when the player has gone under the water

    CVector				m_vecBufferedPlayerBodyOffset;		// 0x184

    // The three vectors that determine this camera for this frame
    CVector				Front;													// 0x190	//Direction of looking in
    CVector				Source;													// 0x19C	//Coors in world space
    CVector				SourceBeforeLookBehind;									// 0x1A8
    CVector				Up;                                                     // 0x1B4	//Just that
    CVector				m_arrPreviousVectors[NUMBER_OF_VECTORS_FOR_AVERAGE];	// 0x1C0	//used to average stuff

    CVector				m_aTargetHistoryPos[CAM_NUM_TARGET_HISTORY];			// 0x1D8
    DWORD				m_nTargetHistoryTime[CAM_NUM_TARGET_HISTORY];			// 0x208
    DWORD				m_nCurrentHistoryPoints;								// 0x218

    CEntity			*	CamTargetEntity;										// 0x21C
    float				m_fCameraDistance;										// 0x220
    float				m_fIdealAlpha;											// 0x224
    float				m_fPlayerVelocity;										// 0x228
    //CVector TempRight;
    CAutomobile		*	m_pLastCarEntered;										// 0x22C // So interpolation works
    CPed			*	m_pLastPedLookedAt;										// 0x230 // So interpolation works 
    bool				m_bFirstPersonRunAboutActive;							// 0x234
};

#endif