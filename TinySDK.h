#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <stdint.h>

namespace tinySDK
{
	// Enum Engine.EMovementMode
	enum class EMovementMode : UINT8 {
		MOVE_None = 0,
		MOVE_Walking = 1,
		MOVE_NavWalking = 2,
		MOVE_Falling = 3,
		MOVE_Swimming = 4,
		MOVE_Flying = 5,
		MOVE_Custom = 6,
		MOVE_MAX = 7
	};

	struct FVector
	{
		float                                              X;                                                        // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
		float                                              Y;                                                        // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
		float                                              Z;
	};

	struct FRotator
	{
		float                                              Pitch;                                                    // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
		float                                              Yaw;                                                      // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
		float                                              Roll;
	};

	struct UObject {
		char pad_0[0x28]; // 0x00(0x28)
	};

	template<class T>
	class TArray
	{
	public:
		T* Data;
		int Count;
		int Max;
	};

	struct AActor : UObject {
		unsigned char ActorStructSize[0x220]; // 0x28(0x220)
	};

	// Class Engine.Pawn
// Size: 0x2a8 (Inherited: 0x248)
	struct APawn : AActor {
		unsigned char ActorStructSize[0x60]; // 0x248(0x60)
	};



	// Class Engine.CharacterMovementComponent
	// Size: 0xb30 (Inherited: 0x150)
	struct UCharacterMovementComponent {
		unsigned char inheritedSizePadding[0x150];
		char pad_150[0x10]; // 0x150(0x10)
		struct ACharacter* CharacterOwner; // 0x160(0x08)
		float GravityScale; // 0x168(0x04)
		float MaxStepHeight; // 0x16c(0x04)
		float JumpZVelocity; // 0x170(0x04)
		float JumpOffJumpZFactor; // 0x174(0x04)
		float WalkableFloorAngle; // 0x178(0x04)
		float WalkableFloorZ; // 0x17c(0x04)
		char bMovementModeIsManaged : 1; // 0x180(0x01)
		char bMovementModeCalledFromManager : 1; // 0x180(0x01)
		char pad_180_2 : 6; // 0x180(0x01)
		enum class EMovementMode MovementMode; // 0x181(0x01)
		char CustomMovementMode; // 0x182(0x01)
		char NetworkSmoothingMode[0x1];
		float GroundFriction; // 0x184(0x04)
		char pad_188[0x24]; // 0x188(0x24)
		float MaxWalkSpeed; // 0x1ac(0x04)
		float MaxWalkSpeedCrouched; // 0x1b0(0x04)
		float MaxSwimSpeed; // 0x1b4(0x04)
		float MaxFlySpeed; // 0x1b8(0x04)
		float MaxCustomMovementSpeed; // 0x1bc(0x04)
		float MaxAcceleration; // 0x1c0(0x04)
		float MinAnalogWalkSpeed; // 0x1c4(0x04)
		float BrakingFrictionFactor; // 0x1c8(0x04)
		float BrakingFriction; // 0x1cc(0x04)
		float BrakingSubStepTime; // 0x1d0(0x04)
		float BrakingDecelerationWalking; // 0x1d4(0x04)
		float BrakingDecelerationFalling; // 0x1d8(0x04)
		float BrakingDecelerationSwimming; // 0x1dc(0x04)
		float BrakingDecelerationFlying; // 0x1e0(0x04)
		float AirControl; // 0x1e4(0x04)
		float AirControlBoostMultiplier; // 0x1e8(0x04)
		float AirControlBoostVelocityThreshold; // 0x1ec(0x04)
		float FallingLateralFriction; // 0x1f0(0x04)
		float CrouchedHalfHeight; // 0x1f4(0x04)
		float Buoyancy; // 0x1f8(0x04)
		float PerchRadiusThreshold; // 0x1fc(0x04)
		float PerchAdditionalHeight; // 0x200(0x04)
		struct FRotator RotationRate; // 0x204(0x0c)
		char bUseSeparateBrakingFriction : 1; // 0x210(0x01)
		char bApplyGravityWhileJumping : 1; // 0x210(0x01)
		char bUseControllerDesiredRotation : 1; // 0x210(0x01)
		char bOrientRotationToMovement : 1; // 0x210(0x01)
		char bSweepWhileNavWalking : 1; // 0x210(0x01)
		char pad_210_5 : 1; // 0x210(0x01)
		char bMovementInProgress : 1; // 0x210(0x01)
		char bEnableScopedMovementUpdates : 1; // 0x210(0x01)
		char bEnableServerDualMoveScopedMovementUpdates : 1; // 0x211(0x01)
		char bForceMaxAccel : 1; // 0x211(0x01)
		char bRunPhysicsWithNoController : 1; // 0x211(0x01)
		char bForceNextFloorCheck : 1; // 0x211(0x01)
		char bShrinkProxyCapsule : 1; // 0x211(0x01)
		char bCanWalkOffLedges : 1; // 0x211(0x01)
		char bCanWalkOffLedgesWhenCrouching : 1; // 0x211(0x01)
		char pad_211_7 : 1; // 0x211(0x01)
		char pad_212_0 : 1; // 0x212(0x01)
		char bNetworkSkipProxyPredictionOnNetUpdate : 1; // 0x212(0x01)
		char bNetworkAlwaysReplicateTransformUpdateTimestamp : 1; // 0x212(0x01)
		char bDeferUpdateMoveComponent : 1; // 0x212(0x01)
		char bEnablePhysicsInteraction : 1; // 0x212(0x01)
		char bTouchForceScaledToMass : 1; // 0x212(0x01)
		char bPushForceScaledToMass : 1; // 0x212(0x01)
		char bPushForceUsingZOffset : 1; // 0x212(0x01)
		char bScalePushForceToVelocity : 1; // 0x213(0x01)
		char pad_213_1 : 7; // 0x213(0x01)
		char pad_214[0x4]; // 0x214(0x04)
		struct USceneComponent* DeferredUpdatedMoveComponent; // 0x218(0x08)
		float MaxOutOfWaterStepHeight; // 0x220(0x04)
		float OutofWaterZ; // 0x224(0x04)
		float Mass; // 0x228(0x04)
		float StandingDownwardForceScale; // 0x22c(0x04)
		float InitialPushForceFactor; // 0x230(0x04)
		float PushForceFactor; // 0x234(0x04)
		float PushForcePointZOffsetFactor; // 0x238(0x04)
		float TouchForceFactor; // 0x23c(0x04)
		float MinTouchForce; // 0x240(0x04)
		float MaxTouchForce; // 0x244(0x04)
		float RepulsionForce; // 0x248(0x04)
		struct FVector Acceleration; // 0x24c(0x0c)
		char pad_258[0x8]; // 0x258(0x08)
		unsigned char LastUpdateRotation[0x10];
		struct FVector LastUpdateLocation; // 0x270(0x0c)
		struct FVector LastUpdateVelocity; // 0x27c(0x0c)
		float ServerLastTransformUpdateTimeStamp; // 0x288(0x04)
		float ServerLastClientGoodMoveAckTime; // 0x28c(0x04)
		float ServerLastClientAdjustmentTime; // 0x290(0x04)
		struct FVector PendingImpulseToApply; // 0x294(0x0c)
		struct FVector PendingForceToApply; // 0x2a0(0x0c)
		float AnalogInputModifier; // 0x2ac(0x04)
		char pad_2B0[0xc]; // 0x2b0(0x0c)
		float MaxSimulationTimeStep; // 0x2bc(0x04)
		int MaxSimulationIterations; // 0x2c0(0x04)
		int MaxJumpApexAttemptsPerSimulation; // 0x2c4(0x04)
		float MaxDepenetrationWithGeometry; // 0x2c8(0x04)
		float MaxDepenetrationWithGeometryAsProxy; // 0x2cc(0x04)
		float MaxDepenetrationWithPawn; // 0x2d0(0x04)
		float MaxDepenetrationWithPawnAsProxy; // 0x2d4(0x04)
		float NetworkSimulatedSmoothLocationTime; // 0x2d8(0x04)
		float NetworkSimulatedSmoothRotationTime; // 0x2dc(0x04)
		float ListenServerNetworkSimulatedSmoothLocationTime; // 0x2e0(0x04)
		float ListenServerNetworkSimulatedSmoothRotationTime; // 0x2e4(0x04)
		float NetProxyShrinkRadius; // 0x2e8(0x04)
		float NetProxyShrinkHalfHeight; // 0x2ec(0x04)
		float NetworkMaxSmoothUpdateDistance; // 0x2f0(0x04)
		float NetworkNoSmoothUpdateDistance; // 0x2f4(0x04)
		float NetworkMinTimeBetweenClientAckGoodMoves; // 0x2f8(0x04)
		float NetworkMinTimeBetweenClientAdjustments; // 0x2fc(0x04)
		float NetworkMinTimeBetweenClientAdjustmentsLargeCorrection; // 0x300(0x04)
		float NetworkLargeClientCorrectionDistance; // 0x304(0x04)
		float LedgeCheckThreshold; // 0x308(0x04)
		float JumpOutOfWaterPitch; // 0x30c(0x04)
		unsigned char currentFloor[0x94];
		enum class EMovementMode DefaultLandMovementMode; // 0x3a4(0x01)
		enum class EMovementMode DefaultWaterMovementMode; // 0x3a5(0x01)
		enum class EMovementMode GroundMovementMode; // 0x3a6(0x01)
		char bMaintainHorizontalGroundVelocity : 1; // 0x3a7(0x01)
		char bImpartBaseVelocityX : 1; // 0x3a7(0x01)
		char bImpartBaseVelocityY : 1; // 0x3a7(0x01)
		char bImpartBaseVelocityZ : 1; // 0x3a7(0x01)
		char bImpartBaseAngularVelocity : 1; // 0x3a7(0x01)
		char bJustTeleported : 1; // 0x3a7(0x01)
		char bNetworkUpdateReceived : 1; // 0x3a7(0x01)
		char bNetworkMovementModeChanged : 1; // 0x3a7(0x01)
		char bIgnoreClientMovementErrorChecksAndCorrection : 1; // 0x3a8(0x01)
		char bServerAcceptClientAuthoritativePosition : 1; // 0x3a8(0x01)
		char bNotifyApex : 1; // 0x3a8(0x01)
		char bCheatFlying : 1; // 0x3a8(0x01)
		char bWantsToCrouch : 1; // 0x3a8(0x01)
		char bCrouchMaintainsBaseLocation : 1; // 0x3a8(0x01)
		char bIgnoreBaseRotation : 1; // 0x3a8(0x01)
		char bFastAttachedMove : 1; // 0x3a8(0x01)
		char bAlwaysCheckFloor : 1; // 0x3a9(0x01)
		char bUseFlatBaseForFloorChecks : 1; // 0x3a9(0x01)
		char bPerformingJumpOff : 1; // 0x3a9(0x01)
		char bWantsToLeaveNavWalking : 1; // 0x3a9(0x01)
		char bUseRVOAvoidance : 1; // 0x3a9(0x01)
		char bRequestedMoveUseAcceleration : 1; // 0x3a9(0x01)
		char pad_3A9_6 : 1; // 0x3a9(0x01)
		char bWasSimulatingRootMotion : 1; // 0x3a9(0x01)
		char bAllowPhysicsRotationDuringAnimRootMotion : 1; // 0x3aa(0x01)
		char bHasRequestedVelocity : 1; // 0x3aa(0x01)
		char bRequestedMoveWithMaxSpeed : 1; // 0x3aa(0x01)
		char bWasAvoidanceUpdated : 1; // 0x3aa(0x01)
		char pad_3AA_4 : 2; // 0x3aa(0x01)
		char bProjectNavMeshWalking : 1; // 0x3aa(0x01)
		char bProjectNavMeshOnBothWorldChannels : 1; // 0x3aa(0x01)
		char pad_3AB[0x11]; // 0x3ab(0x11)
		float AvoidanceConsiderationRadius; // 0x3bc(0x04)
		struct FVector RequestedVelocity; // 0x3c0(0x0c)
		int32_t AvoidanceUID; // 0x3cc(0x04)
		unsigned char avoidanceMasks[0xC];
		float AvoidanceWeight; // 0x3dc(0x04)
		struct FVector PendingLaunchVelocity; // 0x3e0(0x0c)
		char pad_3EC[0xa4]; // 0x3ec(0xa4)
		float NavMeshProjectionInterval; // 0x490(0x04)
		float NavMeshProjectionTimer; // 0x494(0x04)
		float NavMeshProjectionInterpSpeed; // 0x498(0x04)
		float NavMeshProjectionHeightScaleUp; // 0x49c(0x04)
		float NavMeshProjectionHeightScaleDown; // 0x4a0(0x04)
		float NavWalkingFloorDistTolerance; // 0x4a4(0x04)
		char PostPhysicsTickFunction[0x48]; // 0x4a8(0x48)
		char pad_4F0[0x18]; // 0x4f0(0x18)
		float MinTimeBetweenTimeStampResets; // 0x508(0x04)
		char pad_50C[0x4ac]; // 0x50c(0x4ac)
		unsigned char rootMotions[0x70];
		char pad_A28[0x98]; // 0xa28(0x98)
		char RootMotionParams[0x40]; // 0xac0(0x40)
		struct FVector AnimRootMotionVelocity; // 0xb00(0x0c)
		char pad_B0C[0x24]; // 0xb0c(0x24)

	};

	// Class Engine.Character
	// Size: 0x4e0 (Inherited: 0x2a8)
	struct ACharacter : APawn {
		struct USkeletalMeshComponent* Mesh; // 0x2a8(0x08)
		struct UCharacterMovementComponent* CharacterMovement; // 0x2b0(0x08)
		struct UCapsuleComponent* CapsuleComponent; // 0x2b8(0x08)
		char BasedMovement[0x30]; 
		char ReplicatedBasedMovement[0x30];
		float AnimRootMotionTranslationScale; // 0x320(0x04)
		char BaseTranslationOffset[0x0c];
		char BaseRotationOffset[0x10];
		float ReplicatedServerLastTransformUpdateTimeStamp; // 0x340(0x04)
		float ReplayLastTransformUpdateTimeStamp; // 0x344(0x04)
		char ReplicatedMovementMode; // 0x348(0x01)
		bool bInBaseReplication; // 0x349(0x01)
		char pad_34A[0x2]; // 0x34a(0x02)
		float CrouchedEyeHeight; // 0x34c(0x04)
		char bIsCrouched : 1; // 0x350(0x01)
		char bProxyIsJumpForceApplied : 1; // 0x350(0x01)
		char bPressedJump : 1; // 0x350(0x01)
		char bClientUpdating : 1; // 0x350(0x01)
		char bClientWasFalling : 1; // 0x350(0x01)
		char bClientResimulateRootMotion : 1; // 0x350(0x01)
		char bClientResimulateRootMotionSources : 1; // 0x350(0x01)
		char bSimGravityDisabled : 1; // 0x350(0x01)
		char bClientCheckEncroachmentOnNetUpdate : 1; // 0x351(0x01)
		char bServerMoveIgnoreRootMotion : 1; // 0x351(0x01)
		char bWasJumping : 1; // 0x351(0x01)
		char pad_351_3 : 5; // 0x351(0x01)
		char pad_352[0x2]; // 0x352(0x02)
		float JumpKeyHoldTime; // 0x354(0x04)
		float JumpForceTimeRemaining; // 0x358(0x04)
		float ProxyJumpForceStartedTime; // 0x35c(0x04)
		float JumpMaxHoldTime; // 0x360(0x04)
		int32_t JumpMaxCount; // 0x364(0x04)
		int32_t JumpCurrentCount; // 0x368(0x04)
		int32_t JumpCurrentCountPreJump; // 0x36c(0x04)
		char pad_370[0x8]; // 0x370(0x08)
		char otherSize[0x168]; // 0x378(0x168)
	};

	// Class Engine.Controller
// Size: 0x2c0 (Inherited: 0x248)
	struct AController : AActor {
		char pad_248[0x8]; // 0x248(0x08)
		struct APlayerState* PlayerState; // 0x250(0x08)
		char pad_258[0x8]; // 0x258(0x08)
		char OnInstigatedAnyDamage[0x10];
		char StateName[0x08];
		struct APawn* Pawn; // 0x278(0x08)
		char pad_280[0x8]; // 0x280(0x08)
		struct ACharacter* Character; // 0x288(0x08)
		struct USceneComponent* TransformComponent; // 0x290(0x08)
		char pad_298[0x18]; // 0x298(0x18)
		char ControlRotation[0x0c]; 
		char bAttachToPawn : 1; // 0x2bc(0x01)
		char pad_2BC_1 : 7; // 0x2bc(0x01)
		char pad_2BD[0x3]; // 0x2bd(0x03)
	};

	// Class Engine.PlayerController
	// Size: 0x5b0 (Inherited: 0x2c0)
	struct APlayerController : AController {
		struct UPlayer* Player; // 0x2c0(0x08)
		struct APawn* AcknowledgedPawn; // 0x2c8(0x08)
		struct UInterpTrackInstDirector* ControllingDirTrackInst; // 0x2d0(0x08)
		struct AHUD* MyHUD; // 0x2d8(0x08)
		struct APlayerCameraManager* PlayerCameraManager; // 0x2e0(0x08)
		struct APlayerCameraManager* PlayerCameraManagerClass; // 0x2e8(0x08)
		bool bAutoManageActiveCameraTarget; // 0x2f0(0x01)
		char pad_2F1[0x3]; // 0x2f1(0x03)
		struct FRotator TargetViewRotation; // 0x2f4(0x0c)
		char pad_300[0xc]; // 0x300(0x0c)
		float SmoothTargetViewRotationSpeed; // 0x30c(0x04)
		char pad_310[0x8]; // 0x310(0x08)
		struct TArray<struct AActor*> HiddenActors; // 0x318(0x10)
		struct TArray<struct UPrimitiveComponent> HiddenPrimitiveComponents; // 0x328(0x10)
		char pad_338[0x4]; // 0x338(0x04)
		float LastSpectatorStateSynchTime; // 0x33c(0x04)
		struct FVector LastSpectatorSyncLocation; // 0x340(0x0c)
		struct FRotator LastSpectatorSyncRotation; // 0x34c(0x0c)
		int32_t ClientCap; // 0x358(0x04)
		char pad_35C[0x4]; // 0x35c(0x04)
		struct UCheatManager* CheatManager; // 0x360(0x08)
		struct UCheatManager* CheatClass; // 0x368(0x08)
		struct UPlayerInput* PlayerInput; // 0x370(0x08)
		struct TArray<struct FActiveForceFeedbackEffect> ActiveForceFeedbackEffects; // 0x378(0x10)
		char pad_388[0x80]; // 0x388(0x80)
		char pad_408_0 : 4; // 0x408(0x01)
		char bPlayerIsWaiting : 1; // 0x408(0x01)
		char pad_408_5 : 3; // 0x408(0x01)
		char pad_409[0x3]; // 0x409(0x03)
		char NetPlayerIndex; // 0x40c(0x01)
		char pad_40D[0x3b]; // 0x40d(0x3b)
		struct UNetConnection* PendingSwapConnection; // 0x448(0x08)
		struct UNetConnection* NetConnection; // 0x450(0x08)
		char pad_458[0xc]; // 0x458(0x0c)
		float InputYawScale; // 0x464(0x04)
		float InputPitchScale; // 0x468(0x04)
		float InputRollScale; // 0x46c(0x04)
		char bShowMouseCursor : 1; // 0x470(0x01)
		char bEnableClickEvents : 1; // 0x470(0x01)
		char bEnableTouchEvents : 1; // 0x470(0x01)
		char bEnableMouseOverEvents : 1; // 0x470(0x01)
		char bEnableTouchOverEvents : 1; // 0x470(0x01)
		char bForceFeedbackEnabled : 1; // 0x470(0x01)
		char pad_470_6 : 2; // 0x470(0x01)
		char pad_471[0x3]; // 0x471(0x03)
		float ForceFeedbackScale; // 0x474(0x04)
		float HapticFeedbackScale; // 0x478(0x04)
		char pad_47C[0x4]; // 0x47c(0x04)
		struct TArray<struct FKey> ClickEventKeys; // 0x480(0x10)
		int MouseAndChannels;
		float HitResultTraceDistance; // 0x494(0x04)
		uint16_t SeamlessTravelCount; // 0x498(0x02)
		uint16_t LastCompletedSeamlessTravelCount; // 0x49a(0x02)
		char pad_49C[0x74]; // 0x49c(0x74)
		struct UInputComponent* InactiveStateInputComponent; // 0x510(0x08)
		char pad_518_0 : 2; // 0x518(0x01)
		char bShouldPerformFullTickWhenPaused : 1; // 0x518(0x01)
		char pad_518_3 : 5; // 0x518(0x01)
		char pad_519[0x17]; // 0x519(0x17)
		struct UTouchInterface* CurrentTouchInterface; // 0x530(0x08)
		char pad_538[0x50]; // 0x538(0x50)
		struct ASpectatorPawn* SpectatorPawn; // 0x588(0x08)
		char pad_590[0x4]; // 0x590(0x04)
		bool bIsLocalPlayerController; // 0x594(0x01)
		char pad_595[0x3]; // 0x595(0x03)
		struct FVector SpawnLocation; // 0x598(0x0c)
		char pad_5A4[0xc]; // 0x5a4(0x0c)
	};

	// Class Engine.Player
	// Size: 0x48 (Inherited: 0x28)
	struct UPlayer : UObject {
		char pad_28[0x8]; // 0x28(0x08)
		struct APlayerController* PlayerController; // 0x30(0x08)
		int32_t CurrentNetSpeed; // 0x38(0x04)
		int32_t ConfiguredInternetSpeed; // 0x3c(0x04)
		int32_t ConfiguredLanSpeed; // 0x40(0x04)
		char pad_44[0x4]; // 0x44(0x04)
	};

	// Class Engine.LocalPlayer
	// Size: 0x258 (Inherited: 0x48)
	struct ULocalPlayer : UPlayer {
		char pad_48[0x28]; // 0x48(0x28)
		struct UGameViewportClient* ViewportClient; // 0x70(0x08)
		char pad_78[0x1c]; // 0x78(0x1c)
		char AspectRatioAxisConstraint[0x01]; // 0x94(0x01)
		char pad_95[0x3]; // 0x95(0x03)
		struct APlayerController* PendingLevelPlayerControllerClass; // 0x98(0x08)
		char bSentSplitJoin : 1; // 0xa0(0x01)
		char pad_A0_1 : 7; // 0xa0(0x01)
		char pad_A1[0x17]; // 0xa1(0x17)
		int32_t ControllerId; // 0xb8(0x04)
		char pad_BC[0x19c]; // 0xbc(0x19c)
	};

	// Class Engine.GameInstance
	// Size: 0x1a8 (Inherited: 0x28)
	struct UGameInstance : UObject {
		char pad_28[0x10]; // 0x28(0x10)
		struct TArray<struct ULocalPlayer*> LocalPlayers; // 0x38(0x10)
		struct UOnlineSession* OnlineSession; // 0x48(0x08)
		struct TArray<struct UObject*> ReferencedObjects; // 0x50(0x10)
		char pad_60[0x18]; // 0x60(0x18)
		char pad_61[0x10]; // 0x60(0x18); // 0x78(0x10)
		char pad_88[0x120]; // 0x88(0x120)
	};

	struct UWorld : UObject {
		char pad_28[0x8]; // 0x28(0x08)
		struct ULevel* PersistentLevel; // 0x30(0x08)
		unsigned char padding1[0x2E8]; // 0x38(0x2E8)
		struct UGameInstance* OwningGameInstance; // 0x320(0x08)
		unsigned char padding2[0x6A8]; // 0x328(0x670)
	};

	// Class Engine.Level
// Size: 0x300 (Inherited: 0x28)
	struct ULevel : UObject {
		char pad_28[0x90]; // 0x28(0x90)
		struct UWorld* OwningWorld; // 0xb8(0x08)
		struct UModel* Model; // 0xc0(0x08)
		unsigned char padding1[0x238]; // 0xC8(0x238)
	};

	//Essential Variables
	DWORD_PTR BaseAddress;
	HMODULE Handle;
	MODULEINFO info;
	UWorld** m_UWorld;
	ULevel* m_PersistentLevel;
	//SDK::ULocalPlayer* m_LocalPlayer;
	uintptr_t g_GNames;

	//Additional
	float centerX = 960.0;
	float centerY = 540.0;

}