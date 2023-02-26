#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <stdint.h>
#include <string>
#include <unordered_set>

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

	enum class EInteractionProject : UINT8 {
		Reactions = 0,
		Creatures = 1,
		NPC = 2,
		EInteractionProject_MAX = 3
	};

	enum class EnumGeneric : UINT8 {
		None = 0,
		One = 1,
		Two = 2,
		Three = 3,
		Four = 4,
		Five = 5,
		Six = 6,
		Seven = 7,
		Eight = 8,
		Nine = 9,
		Ten = 10
	};

	struct FPointer
	{
		uintptr_t Dummy;
	};

	struct FQWord
	{
		int A;
		int B;
	};

	template<class T>
	class TArray
	{
	public:
		T* Data;
		int Count;
		int Max;
	};


	class FString : public TArray<wchar_t>
	{
	public:
		FString();
		FString(const wchar_t* other);
		const wchar_t* wc_str() const;
		const char* c_str() const;
		bool IsValid() const;
		std::string ToString() const;
		std::wstring ToStringW() const;
	};


	class FUObjectItem
	{
	public:
		class UObject* Object;                                                  // 0x0000(0x0008)
		int32_t                                                    Flags;                                                   // 0x0008(0x0004)
		int32_t                                                    ClusterIndex;                                            // 0x000C(0x0004)
		int32_t                                                    SerialNumber;                                            // 0x0010(0x0004)
		unsigned char                                              pad_BXEK2KRZ3X[0x04];                                    // 0x0014(0x0004)

	public:
		bool IsUnreachable() const;
		bool IsPendingKill() const;
	};


	class TUObjectArray
	{
	private:
		static const constexpr int32_t                             NumElementsPerChunk = 64 * 1024;                         // 0x0000(0x0000)
		FUObjectItem** Objects;                                                 // 0x0000(0x0000)
		FUObjectItem* PreAllocatedObjects;                                     // 0x0000(0x0000)
	public:
		int32_t                                                    MaxElements;                                             // 0x0000(0x0000)
		int32_t                                                    NumElements;                                             // 0x0000(0x0000)
		int32_t                                                    MaxChunks;                                               // 0x0000(0x0000)
		int32_t                                                    NumChunks;                                               // 0x0000(0x0000)

	public:
		int32_t Count() const;
		int32_t Max() const;
		bool IsValidIndex(int32_t Index) const;
		FUObjectItem* GetObjectPtr(int32_t Index) const;
		UObject* GetByIndex(int32_t index) const;
		FUObjectItem* GetItemByIndex(int32_t index) const;
		UObject* operator[](int32_t i);
		const UObject* operator[](int32_t i) const;
	};

	class FNameEntryHeader
	{
	public:
		static const constexpr uint32_t                            ProbeHashBits = 5;                                       // 0x0000(0x0000)
		uint16_t                                                   bIsWide : 1;                                             // 0x0000(0x0000)
		uint16_t                                                   LowercaseProbeHash : ProbeHashBits;                      // 0x0000(0x0000)
		uint16_t                                                   Len : 10;                                                // 0x0000(0x0000)
	};


	class FNameEntry
	{
	public:
		FNameEntryHeader                                           Header;                                                  // 0x0000(0x0000)
		union
		{
			char                                                      AnsiName[1024];                                          // 0x0000(0x0000)
			wchar_t                                                   WideName[1024];                                          // 0x0000(0x0000)
		};

	public:
		int32_t GetLength() const;
		bool IsWide() const;
		int32_t GetId() const;
		std::string GetAnsiName() const;
		std::wstring GetWideName() const;
		std::string GetName() const;
	};


	class FNameEntryAllocator
	{
	private:
		uint8_t                                                    FrwLock[0x8];                                            // 0x0000(0x0000)
	public:
		static const constexpr int32_t                             Stride = 0x02;                                           // 0x0000(0x0000)
		static const constexpr int32_t                             MaxOffset = Stride * (1 << 16);                          // 0x0000(0x0000)
		int32_t                                                    CurrentBlock;                                            // 0x0000(0x0000)
		int32_t                                                    CurrentByteCursor;                                       // 0x0000(0x0000)
		uint8_t* Blocks[8192];                                            // 0x0000(0x0000)

	public:
		int32_t NumBlocks() const;
		FNameEntry* GetById(int32_t key) const;
		bool IsValidIndex(int32_t key) const;
		bool IsValidIndex(int32_t key, uint32_t block, uint16_t offset) const;
	};

	class FNamePool
	{
	public:
		FNameEntryAllocator                                        Allocator;                                               // 0x0000(0x0000)
		int32_t                                                    AnsiCount;                                               // 0x0000(0x0000)
		int32_t                                                    WideCount;                                               // 0x0000(0x0000)

	public:
		FNameEntry* GetNext(uintptr_t& nextFNameAddress, uint32_t* comparisonId) const;
		int32_t Count() const;
		bool IsValidIndex(int32_t index) const;
		FNameEntry* GetById(int32_t id) const;
		FNameEntry* operator[](int32_t id) const;
	};

	class FName
	{
	public:
		static FNamePool* GNames;                                                  // 0x0000(0x0000)
		int32_t                                                    ComparisonIndex;                                         // 0x0000(0x0000)
		int32_t                                                    Number;                                                  // 0x0000(0x0000)

	public:
		FName();
		FName(int32_t i);
		FName(const char* nameToFind);
		FName(const wchar_t* nameToFind);
		static FNamePool& GetGlobalNames();
		std::string GetNameA() const;
		std::wstring GetNameW() const;
		std::string GetName() const;
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

	template<typename KeyType, typename ValueType>
	class TPair
	{
	public:
		KeyType   Key;
		ValueType Value;
	};

	class UObject;

	class FScriptInterface
	{
	private:
		UObject* ObjectPointer;
		void* InterfacePointer;

	public:
		UObject* GetObject() const
		{
			return ObjectPointer;
		}

		UObject*& GetObjectRef()
		{
			return ObjectPointer;
		}

		void* GetInterface() const
		{
			return ObjectPointer != nullptr ? InterfacePointer : nullptr;
		}
	};

	template<class InterfaceType>
	class TScriptInterface : public FScriptInterface
	{
	public:
		InterfaceType* operator->() const
		{
			return (InterfaceType*)GetInterface();
		}

		InterfaceType& operator*() const
		{
			return *((InterfaceType*)GetInterface());
		}

		operator bool() const
		{
			return GetInterface() != nullptr;
		}

	};

	struct FText
	{
		char UnknownData[0x18];
	};

	struct FWeakObjectPtr
	{
		int32_t ObjectIndex;
		int32_t ObjectSerialNumber;
	};

	struct FStringAssetReference
	{
		FString AssetLongPathname;
	};

	template<typename TObjectID>
	class TPersistentObjectPtr
	{
	public:
		FWeakObjectPtr WeakPtr;
		int32_t TagAtLastTest;
		TObjectID ObjectID;
	};

	struct FSoftObjectPath
	{
		FName AssetPathName;
		FString SubPathString;
	};

	class FSoftObjectPtr : public TPersistentObjectPtr<FSoftObjectPath>
	{

	};

	class FAssetPtr : public TPersistentObjectPtr<FStringAssetReference>
	{

	};

	struct FGuid
	{
		uint32_t A;
		uint32_t B;
		uint32_t C;
		uint32_t D;
	};

	struct FUniqueObjectGuid
	{
		FGuid Guid;
	};

	class FLazyObjectPtr : public TPersistentObjectPtr<FUniqueObjectGuid>
	{

	};

	struct FScriptDelegate
	{
		unsigned char UnknownData[20];
	};

	struct FScriptMulticastDelegate
	{
		unsigned char UnknownData[16];
	};

	class UClass;
	
	class UObject
	{
	public:
		static class TUObjectArray* GObjects;                                                
		void* VfTable;                                                 
		int32_t                                                    Flags;                                                   
		int32_t                                                    InternalIndex;                                           
		class UClass* Class;                                                   
		FName                                                      Name;                                                    
		class UObject* Outer;                                                  

	public:
		static TUObjectArray& GetGlobalObjects();
		std::string GetName() const;
		std::string GetFullName() const;
		template<typename T>
		static T* FindObject(const std::string& name)
		{
			for (int32_t i = 0; i < UObject::GetGlobalObjects().Count(); ++i)
			{
				auto object = UObject::GetGlobalObjects().GetByIndex(i);

				if (!object)
					continue;

				if (object->GetFullName() == name)
					return static_cast<T*>(object);
			}
			return nullptr;
		}

		template<typename T>
		static T* FindObject()
		{
			auto v = T::StaticClass();
			for (int32_t i = 0; i < UObject::GetGlobalObjects().Count(); ++i)
			{
				auto object = UObject::GetGlobalObjects().GetByIndex(i);

				if (!object)
					continue;

				if (object->IsA(v))
					return static_cast<T*>(object);
			}
			return nullptr;
		}

		template<typename T>
		static std::vector<T*> FindObjects(const std::string& name)
		{
			std::vector<T*> ret;
			for (int32_t i = 0; i < UObject::GetGlobalObjects().Count(); ++i)
			{
				auto object = UObject::GetGlobalObjects().GetByIndex(i);

				if (!object)
					continue;

				if (object->GetFullName() == name)
					ret.push_back(static_cast<T*>(object));
			}
			return ret;
		}

		template<typename T>
		static std::vector<T*> FindObjects()
		{
			std::vector<T*> ret;
			auto v = T::StaticClass();
			for (int i = 0; i < UObject::GetGlobalObjects().Count(); ++i)
			{
				auto object = UObject::GetGlobalObjects().GetByIndex(i);

				if (!object)
					continue;

				if (object->IsA(v))
					ret.push_back(static_cast<T*>(object));
			}
			return ret;
		}

		static UClass* FindClass(const std::string& name);
		template<typename T>
		static T* GetObjectCasted(size_t index)
		{
			return static_cast<T*>(UObject::GetGlobalObjects().GetByIndex(index));
		}

		bool IsA(UClass* cmp) const;
		void ExecuteUbergraph(int32_t EntryPoint);
		void ProcessEvent(class UFunction* function, void* parms);
		static UClass* StaticClass();
	};

	// (0x28 - 0x30) 
	class UField : public UObject
	{
	public:
		UField* Next;
	};

	class UEnum : public UField
	{
	public:
		FString CppType; //0x0030 
		TArray<TPair<FName, uint64_t>> Names; //0x0040 
		__int64 CppForm; //0x0050 
	};

	class UStruct : public UField
	{
	public:
		UStruct* SuperField;
		UField* Children;
		int32_t PropertySize;
		int32_t MinAlignment;
		char pad_0x0048[0x40];
	};

	class UClass : public UStruct
	{
	public:
		unsigned char                                              UnknownData_Z0DU[0x180];                                 // 0x00B0(0x0180) MISSED OFFSET (PADDING)

	public:
		template<typename T>
		T* CreateDefaultObjectOfType()
		{
			return static_cast<T*>(CreateDefaultObject());
		}

		UObject* CreateDefaultObject();
		static UClass* StaticClass();
	};

	template<typename Fn>
	Fn GetVFunction(const void* instance, size_t index)
	{
		auto vtable = *static_cast<const void***>(const_cast<void*>(instance));
		return reinterpret_cast<Fn>(const_cast<void(*)>(vtable[index]));
	}


	TUObjectArray& UObject::GetGlobalObjects()
	{
		return *GObjects;
	}


	std::string UObject::GetName() const
	{
		std::string name(Name.GetName());
		if (Name.Number > 0)
			name += '_' + std::to_string(Name.Number);
		auto pos = name.rfind('/');
		if (pos == std::string::npos)
			return name;
		return name.substr(pos + 1);
	}


	std::string UObject::GetFullName() const
	{
		std::string name;
		if (Class != nullptr)
		{
			std::string temp;
			for (auto p = Outer; p; p = p->Outer)
			{
				temp = p->GetName() + "." + temp;
			}
			name = Class->GetName();
			name += " ";
			name += temp;
			name += GetName();
		}
		return name;
	}

	UClass* UObject::FindClass(const std::string& name)
	{
		return FindObject<UClass>(name);
	}

	bool UObject::IsA(UClass* cmp) const
	{
		for (auto super = Class; super; super = static_cast<UClass*>(super->SuperField))
		{
			if (super == cmp)
				return true;
		}

		return false;
	}

	void UObject::ProcessEvent(class UFunction* function, void* parms)
	{
		GetVFunction<void(*)(UObject*, class UFunction*, void*)>(this, 0x44)(this, function, parms);
	}

	template<class T, class TWeakObjectPtrBase = FWeakObjectPtr>
	class TWeakObjectPtr : public TWeakObjectPtrBase
	{
	public:
		T* Get() const
		{
			return (T*)TWeakObjectPtrBase::Get();
		}

		T& operator*() const
		{
			return *Get();
		}

		T* operator->() const
		{
			return Get();
		}

		bool IsValid()
		{
			return TWeakObjectPtrBase::IsValid();
		}

	};


	class UScriptStruct : public UStruct
	{
	public:
		char pad_0x0088[0x10]; //0x0088
	};

	struct FGameplayProperty {
		struct FName TypeName; // 0x00(0x08)
		struct FName UpdateFunctionName; // 0x08(0x08)
		struct UFunction* OnUpdatedCallback; // 0x10(0x08)
		bool bSkipUpdateOnTick; // 0x18(0x01)
		char pad_19[0x7]; // 0x19(0x07)
		struct TArray<struct UModFilter*> SupportedOnAddModFilters; // 0x20(0x10)
		struct TArray<struct UModFilter*> SupportedOnApplyModFilters; // 0x30(0x10)
		char pad_40[0x10]; // 0x40(0x10)
	};

	struct FGameplayProperty_Float : FGameplayProperty {
		float Current; // 0x50(0x04)
		float Base; // 0x54(0x04)
		float Min; // 0x58(0x04)
		float Max; // 0x5c(0x04)
		char pad_60[0x30]; // 0x60(0x30)
	};

	struct FGameplayProperty_Int : public FGameplayProperty
	{
	public:
		int32_t                                                    Current;                                                 // 0x0050(0x0004) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		int32_t                                                    Base;                                                    // 0x0054(0x0004) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		int32_t                                                    Min;                                                     // 0x0058(0x0004) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		int32_t                                                    Max;                                                     // 0x005C(0x0004) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		EnumGeneric                               RoundingType;                                            // 0x0060(0x0001) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              paddingRandom1[0x1F];                                  // 0x0061(0x001F) MISSED OFFSET (PADDING)
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

		void Jump();
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

	class UFunction : public UStruct
	{
	public:
		int32_t                                                    FunctionFlags;                                           
		int16_t                                                    RepOffset;                                               
		int8_t                                                     NumParms;                                                
		unsigned char                                              pad_YK6JXED6AT[0x01];                                    
		uint16_t                                                   ParmsSize;                                               
		uint16_t                                                   ReturnValueOffset;                                       
		uint16_t                                                   RPCId;                                                   
		uint16_t                                                   RPCResponseId;                                           
		class FProperty* FirstPropertyToInit;                                     
		class UFunction* EventGraphFunction;                                      
		int32_t                                                    EventGraphCallOffset;                                    
		unsigned char                                              pad_8SXL1DKMQG[0x04];                                    
		void* Func;                                                    
	};

	template<typename ElementType>
	class TSetElement
	{
	public:
		ElementType                                                Value;                                                   // 0x0000(0x0000)
		int32_t                                                    HashNextId;                                              // 0x0000(0x0000)
		int32_t                                                    HashIndex;                                               // 0x0000(0x0000)
	};

	template<typename Key, typename Value>
	class TMap
	{
	public:
		TArray<TSetElement<TPair<Key, Value>>>                     Data;                                                    // 0x0000(0x0000)
	private:
		uint8_t                                                    UnknownData01[0x04];                                     // 0x0000(0x0000)
		uint8_t                                                    UnknownData02[0x04];                                     // 0x0000(0x0000)
		uint8_t                                                    UnknownData03[0x08];                                     // 0x0000(0x0000)
		uint8_t                                                    UnknownData04[0x08];                                     // 0x0000(0x0000)
		uint8_t                                                    UnknownData_MayBeSize[0x04];                             // 0x0000(0x0000)
		uint8_t                                                    UnknownData_MayBeFlag[0x04];                             // 0x0000(0x0000)
		uint8_t                                                    UnknownData05[0x08];                                     // 0x0000(0x0000)
		uint8_t                                                    UnknownData06[0x08];                                     // 0x0000(0x0000)
		uint8_t                                                    UnknownData07[0x08];                                     // 0x0000(0x0000)
		uint8_t                                                    UnknownData_MayBeSize02[0x04];                           // 0x0000(0x0000)
		uint8_t                                                    UnknownData08[0x04];                                     // 0x0000(0x0000)
	};

	class AAnimationArchitect_Character : public ACharacter
	{
	public:
		unsigned char                                              padding_random1[0x10];                                   
	};

	class AAble_Character : public AAnimationArchitect_Character
	{
	public:
		unsigned char                                              pad_4F0[0x8];                                   // 0x04F0(0x0008) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		unsigned char                                              pad_4F1[0x8];                                   // 0x04F0(0x0008) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UAblAbilityComponent* AblAbilityComponent;                                     // 0x0500(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		class URagdollControlComponent* RagdollControlComponent;                                 // 0x0508(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		unsigned char                                              pad_510[0x18];                                  // 0x0510(0x0018) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UAblPhysicalBodyManager* PhysicalBodyManager;                                     // 0x0528(0x0008) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_530[0x8];                                   // 0x0530(0x0008) Fix size for supers

		bool PlayAbilityByClass(const class FName& InAbilityChannelName, class UClass* InAblAbilitClass, class AActor* InInstigator);
		bool GetAbilityWithMultipleAssetsAndTags(const struct FGameplayTagContainer& InSourceTagContainer, TArray<class UInteractionArchitectAsset*> InArchitectAssets, TArray<struct FGameplayTagContainer> InArchitectTags, EInteractionProject Project, bool* bOutIsAdditive, class UClass** OutAbilityClass);
		bool GetAbilityWithMultipleAssets(const struct FGameplayTagContainer& InSourceTagContainer, TArray<class UInteractionArchitectAsset*> InArchitectAssets, const struct FGameplayTagContainer& InArchitectTagContainer, EInteractionProject Project, bool* bOutIsAdditive, class UClass** OutAbilityClass);
		bool GetAbilityFromInteractionArchitect(const struct FGameplayTagContainer& InSourceTagContainer, class UInteractionArchitectAsset* InArchitectAsset, const struct FGameplayTagContainer& InArchitectTagContainer, EInteractionProject Project, bool* bOutIsAdditive, class UClass** OutAbilityClass);
		bool ExecuteAbilityOnChannelByClass(const class FName& ChannelName, class UClass* InAblAbilitClass, class AActor* InInstigator);
		bool ExecuteAbilityByClass(class UClass* InAblAbilitClass, class AActor* InInstigator);
		void CancelAbilityByContext(class UAblAbilityContext* InAblAbilityContext);
		void CancelAbilityByChannel(const class FName& InAbilityChannelName);
		static UClass* StaticClass();
	};

	class AAmbulatory_Character : public AAble_Character
	{
	public:
		unsigned char                                              pad_540[0x8];                                   // 0x0538(0x0008) Fix Super Size
		unsigned char                                              pad_541[0x8];                                   // 0x0540(0x0008) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UAkAudioEvent* AkEvent;                                                 // 0x0548(0x0008) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UFootPlantEffects* FootPlantEffects;                                        // 0x0550(0x0008) Edit, BlueprintVisible, ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, PersistentInstance, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       bIsBiped;                                                // 0x0558(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       bUseTurnAssist;                                          // 0x0559(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       bAbstractMobility;                                       // 0x055A(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       bTestNewMobilityMotions;                                 // 0x055B(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       bOnlyLockOnMode;                                         // 0x055C(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_55D[0x3];                                   // 0x055D(0x0003) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		float                                                      MinimumMovementSpeed;                                    // 0x0560(0x0004) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      MaximumMovementSpeed;                                    // 0x0564(0x0004) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      SlowWalkMovementSpeed;                                   // 0x0568(0x0004) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      WalkMovementSpeed;                                       // 0x056C(0x0004) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      FastWalkMovementSpeed;                                   // 0x0570(0x0004) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      JogMovementSpeed;                                        // 0x0574(0x0004) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      SprintMovementSpeed;                                     // 0x0578(0x0004) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_56D[0x4];                                   // 0x057C(0x0004) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UAmbulatory_Data* AmbulatoryData;                                          // 0x0580(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		unsigned char                                              pad_55T[0x8];                                   // 0x0588(0x0008) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		unsigned char                                              PathSpec[0x0050];
		float                                                      JumpStartHeight;                                         // 0x05E0(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		struct FVector                                             JumpStartVelocity;                                       // 0x05E4(0x000C) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		unsigned char                                              pad_57C[0x5];                                   // 0x05F0(0x0005) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		bool                                                       Debug;                                                   // 0x05F5(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_588[0x2];                                   // 0x05F6(0x0002) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class AActor* LadderActor;                                             // 0x05F8(0x0008) BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_5F0[0x28];                                  // 0x0600(0x0028) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		unsigned char                                              WorldSpeedTracker[0x000C];         // 0x0628(0x000C)                           
		unsigned char                                              pad_5F6[0x4];                                   // 0x0634(0x0004) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class FScriptMulticastDelegate                             CharacterTeleportedDelegate;                             // 0x0638(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		class UTargetTracker* LookAtTargetTracker;                                     // 0x0648(0x0008) BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UTargetTracker* FaceTargetTracker;                                       // 0x0650(0x0008) BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UTargetTracker* AimAtTargetTracker;                                      // 0x0658(0x0008) BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class USpeedModeTracker* SpeedModeTracker;                                        // 0x0660(0x0008) BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class USpeedModeTracker* SpeedModifierTracker;                                    // 0x0668(0x0008) BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		TArray<class UPrimitiveComponent*>                         WadingWaterComponents;                                   // 0x0670(0x0010) BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, ContainsInstancedReference, NativeAccessSpecifierPublic
		unsigned char                                              pad_600[0x18];                                  // 0x0680(0x0018) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		struct FVector                                             FixedWorldDirection;                                     // 0x0698(0x000C) BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_634[0x538];                                 // 0x06A4(0x0538) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		unsigned char                  LedgeComponent[0x8];                                          // 0x0BDC(0x0008) ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_680[0xB8];                                  // 0x0BE4(0x00B8) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		bool                                                       m_navLinkUsingLedge;                                     // 0x0C9C(0x0001) BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              m_linkType;                                              // 0x0C9D(0x0001) BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_6A4[0x2];                                   // 0x0C9E(0x0002) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		float                                                      m_traceWallRadius;                                       // 0x0CA0(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      m_traceWallDropRadius;                                   // 0x0CA4(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      m_traceWallForward;                                      // 0x0CA8(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      m_traceWallHeightAdjust;                                 // 0x0CAC(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      m_traceLedgeRadius;                                      // 0x0CB0(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      m_traceLedgeIntoWallAdjust;                              // 0x0CB4(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      m_traceLedgeUpAdjust;                                    // 0x0CB8(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      m_traceLedgeDownCast;                                    // 0x0CBC(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic

		//Add the proper enum structs if you want
		void StopWorldTurnInPlace();
		void StartWorldTurnInPlace(const struct FVector& WorldDirection);
		void SprintStop();
		void SprintStart(EnumGeneric SpeedModType);
		void ShowTimingMessage(const class FString& MyMessage, class UObject* WorldContextObject, float Duration);
		void SetWorldSpeedAndDirection(float WorldSpeed, const struct FVector& WorldDirection);
		void SetWorldSpeed(float WorldSpeed);
		void SetUseSpeedModifier(bool bUseSpeedModifier);
		void SetTargetSpeedAndDirection(float InAbstractSpeed, const struct FVector& InDirection, bool InSetMobilityStateBasedOnSpeed);
		void SetTargetSpeed(float InAbstractSpeed, bool InSetMobilityStateBasedOnSpeed);
		void SetSpeedMode(EnumGeneric SpeedMode, EnumGeneric SpeedModifier, EnumGeneric SpeedPriority);
		void SetMovementSpeeds(float InMin, float InMax);
		void SetMountTransform(const struct FTransform& Transform);
		void SetMountNormal(const struct FVector& Normal);
		void SetMobilityModeState(EnumGeneric InMobilityModeState);
		void SetMobilityActionState(EnumGeneric InMobilityActionState, bool bSetTargetSpeed);
		void SetLandingType(EnumGeneric InType);
		void SetJumpingType(EnumGeneric InType);
		void SetIsInIdle(bool isIdle);
		void SetInWaterVolume(bool InFlag);
		void SetDesiredDirection(const struct FVector& InWorldDirection);
		void SetClimbingLedgeState(EnumGeneric InType);
		void SetClimbingLadderState(EnumGeneric InType);
		void SetBaseSpeedMode(EnumGeneric SpeedMode, EnumGeneric SpeedModifier);
		void Set_ForceTurnAndFace(bool InFlag);
		void RevertSpeedMode(EnumGeneric SpeedPriority);
		void RevertMobilityModeState();
		void RevertMobilityActionState(bool bSetTargetSpeed);
		void RemoveWadingComponent(class UPrimitiveComponent* InWaterComponent);
		void JumpStarted(class UAnimInstance* InAnimInstance);
		void JumpFinished(class UAnimInstance* InAnimInstance);
		bool IsWadingComponent(class UPrimitiveComponent* InWaterComponent);
		bool IsUsingSpeedModifier();
		int32_t GetWadingComponentCount();
		class UPrimitiveComponent* GetWadingComponent();
		void GetTargetSpeedModifier(EnumGeneric* OutState);
		EnumGeneric GetSpeedModifierType();
		void GetPreviousMobilityActionState(EnumGeneric* OutMobilityActionState);
		struct FOdcPathSpec GetPathSpec();
		void GetMountTransform(struct FTransform* LedgeTransform);
		void GetMountNormal(struct FVector* LedgeNormal);
		void GetMobilityModeState(EnumGeneric* OutMobilityModeState);
		void GetMobilityActionStateName(class UAnimInstance* InAnimInstance, class FString* OutString);
		void GetMobilityActionState(EnumGeneric* OutMobilityActionState);
		class UTargetTracker* GetLookAtTargetTracker();
		int32_t GetLinkUsageFlags();
		void GetLedgeTransform(struct FTransform* LedgeTransform);
		void GetLedgeNormal(struct FVector* LedgeNormal);
		void GetLandingType(EnumGeneric* OutType);
		void GetJumpingType(EnumGeneric* OutType);
		bool GetIsMovementInput();
		bool GetIsInIdle();
		class UTargetTracker* GetFaceTargetTracker();
		void GetClimbingLedgeState(EnumGeneric* OutType);
		void GetClimbingLadderState(EnumGeneric* OutType);
		int32_t GetBlockageFlags();
		void GetBaseSpeedMode(EnumGeneric* OutState);
		class UTargetTracker* GetAimAtTargetTracker();
		void DestroyAttachedSceneComponent(class USceneComponent* InSceneComponent);
		float ComputeDesiredDirectionAngle(const struct FVector& InDirection, bool bInComputeDegrees);
		float ComputeAngleBetweenTargetTrackers(class UTargetTracker* SourceTracker, class UTargetTracker* DestTracker, bool bInComputeDegrees);
		float ComputeAngle(const struct FVector& InDirection, const struct FVector& InSourceForward, bool bInComputeDegrees);
		void CharacterRotatedBySupport(const struct FRotator& DeltaRotation);
		class USceneComponent* AttachSceneComponent(const struct FTransform& InTransform, const class FName& InTag);
		void ApplySlideHorizontalMovement(float WorldSpeed, const struct FVector& InWorldDirection);
		void ApplyMobilityModeStateSpeeds(EnumGeneric InMobilityModeState);
		bool AmIMovingIntoStairs();
		bool AmIMovingIntoSlopes();
		bool AmIGoingUpStairs();
		bool AmIGoingDownStairs();
		void AddWadingComponent(class UPrimitiveComponent* InWaterComponent);
		static UClass* StaticClass();
	};

	class ABase_Character : public AAmbulatory_Character
	{
	public:
		unsigned char                                              pad_CC0[0x40];                                  // 0x0CC0(0x0040) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UPhoenixManagedCharacter* ManagedCharacter;                                        // 0x0D00(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              ScratchFXHandle[0x0008];// 0x0D08(0x0008)
		class FScriptMulticastDelegate                             OnSpellSuccess;                                          // 0x0D10(0x0010) ZeroConstructor, InstancedReference, NativeAccessSpecifierPublic
		class UTraversalSettings* TraversalSettings;                                       // 0x0D20(0x0008) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      ShoulderFixupAlpha;                                      // 0x0D28(0x0004) BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      FadeOutAlphaFactor;                                      // 0x0D2C(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCharacterStateInfo* CachedCharacterStateInfo;                                // 0x0D30(0x0008) ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		unsigned char                                              pad_D38[0x3D0];                                 // 0x0D38(0x03D0) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UClothSitComponent* ClothSitComponent;                                       // 0x1108(0x0008) ExportObject, ZeroConstructor, Transient, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_1110[0x140];                                 // 0x1110(0x0140) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		float                                                      DefaultMaxHeight;                                        // 0x1250(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate
		unsigned char                                              pad_1254[0x404];                                 // 0x1254(0x0404) Fix size for supers

		void StartSettingTargetableByAiState(bool bTargetable, EnumGeneric Priority, class UObject* Provider);
		void StartSettingMaxHeightValue(float bValue, EnumGeneric Priority, class UObject* InInstigator);
		void StartSettingMaxHeightEnabledValue(bool bValue, EnumGeneric Priority, class UObject* InInstigator);
		void StartSettingLODOverride(int32_t LODOverride, class UObject* Provider);
		void RunEQS_FindStations(EnumGeneric RunMode, struct FQueryFinishedData* QueryFinishedDelegate);
		void RunEQS_FindSocialReasonings(EnumGeneric RunMode, struct FQueryFinishedData* QueryFinishedDelegate);
		void ReRegisterWithSignificanceManager();
		void ReleaseFocusFromScriptedBehavior(const class FName& InIdentifyingName);
		void PlayerSighted();
		bool Nevermind();
		bool IsTargetableByAi();
		bool Goodbye();
		class ASocialReasoning* GetSocialReasoning();
		class UObjectStateInfo* GetObjectStateInfo();
		class UCharacterStateInfo* GetCharacterStateInfo();
		void FinishSettingTargetableByAiState(EnumGeneric Priority, class UObject* Provider);
		void FinishSettingMaxHeightValue(EnumGeneric Priority, class UObject* InInstigator);
		void FinishSettingMaxHeightEnabledValue(EnumGeneric Priority, class UObject* InInstigator);
		void FinishSettingLODOverride(class UObject* Provider);
		void ExcuseMe();
		void DisableCcd();
		void ConverseWithProxy(struct FSocialResponse* SocialResponse);
		void Converse(struct FSocialResponse* SocialResponse);
		void Chat();
		void CaptureFocusForScriptedBehavior(const class FName& InIdentifyingName);
		static UClass* StaticClass();
	};

	class ABiped_Character : public ABase_Character
	{
	public:
		unsigned char                                              pad_1660[0x8];                                   // 0x1658(0x0008) Fix Super Size
		unsigned char                                              pad_16C4[0x8];                                   // 0x1660(0x0008) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UToolSetComponent* ToolSetComponent;                                        // 0x1668(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCognitionStimuliSourceComponent* CognitionStimuliSourceComponent;                         // 0x1670(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UBTCustomComponent* BehaviorComp_Primary;                                    // 0x1678(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UBTCustomComponent* BehaviorComp_Secondary;                                  // 0x1680(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UBlackboardComponent* BlackboardComp;                                          // 0x1688(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UBipedStateComponent* ObjectStateComponent;                                    // 0x1690(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UContextFilterComponent* ContextFilterComponent;                                  // 0x1698(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class URPGAbilityComponent* RPGAbilityComponent;                                     // 0x16A0(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UFacialComponent* FacialComponent;                                         // 0x16A8(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UAnimationComponent* AnimationComponent;                                      // 0x16B0(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      MAX_HEIGHT_FOR_CLIMB;                                    // 0x16B8(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      MAX_HEIGHT_FOR_HANG;                                     // 0x16BC(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      MAX_HEIGHT_FOR_JUMPWAIST;                                // 0x16C0(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_16E1[0x4];                                   // 0x16C4(0x0004) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class AActor* ledgeActor;                                              // 0x16C8(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      CrouchCapsuleHalfHeight;                                 // 0x16D0(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      CrouchCapsuleRadius;                                     // 0x16D4(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      CrouchStartCapsuleBlendDuration;                         // 0x16D8(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      CrouchEndCapsuleBlendDuration;                           // 0x16DC(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		EnumGeneric                                                ParryState;                                              // 0x16E0(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_17BC[0x7];                                   // 0x16E1(0x0007) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class FScriptMulticastDelegate                             OnSpellCast;                                             // 0x16E8(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		class FScriptMulticastDelegate                             OnLumosStart;                                            // 0x16F8(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		class FScriptMulticastDelegate                             OnLumosEnd;                                              // 0x1708(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		class FScriptMulticastDelegate                             OnAccioStart;                                            // 0x1718(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		class FScriptMulticastDelegate                             OnAccioEnd;                                              // 0x1728(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		class FScriptMulticastDelegate                             OnProtegoStart;                                          // 0x1738(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		class FScriptMulticastDelegate                             OnProtegoEnd;                                            // 0x1748(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		class FScriptMulticastDelegate                             OnProtegoDeflected;                                      // 0x1758(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		class FScriptMulticastDelegate                             OnDisillusionmentStart;                                  // 0x1768(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		class FScriptMulticastDelegate                             OnDisillusionmentEnd;                                    // 0x1778(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		unsigned char                                SpellLoadOutData[0x0030];                                        // 0x1788(0x0030) Transient, NativeAccessSpecifierPublic
		int32_t                                                    SpellCastHandle;                                         // 0x17B8(0x0004) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_18E1[0x4];                                   // 0x17BC(0x0004) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		struct FGameplayProperty_Float                             DoubleItemAbilityDurationChanceModifier;                 // 0x17C0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             RefreshAllCooldownChanceModifier;                        // 0x1850(0x0090) Edit, NativeAccessSpecifierPublic
		bool                                                       bIgnoreFallDamage;                                       // 0x18E0(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_18EC[0x3];                                   // 0x18E1(0x0003) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		float                                                      MaxAirHeight;                                            // 0x18E4(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      LastGroundHeight;                                        // 0x18E8(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_1900[0x4];                                   // 0x18EC(0x0004) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UClass* m_studentActorClassM;                                    // 0x18F0(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UClass* m_studentActorClassF;                                    // 0x18F8(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_1904[0x170];                                 // 0x1900(0x0170) MISSED OFFSET (PADDING)

		void ZeroKinematicIntegratorLinearVelocity();
		void ZeroKinematicIntegratorAngularVelocity();
		void UnEquipWand();
		void StartCrouching(class UObject* Provider, EnumGeneric SpeedMode, EnumGeneric SpeedModifier);
		void StairsStateHasChanged(EnumGeneric InLastStairsState, EnumGeneric InNewStairsState);
		void SetTargetDirection(const struct FVector& i_targetDirection);
		void SetTargetDestination(const struct FVector& i_targetDestination);
		void SetTargetComponent(class UPrimitiveComponent* i_pTargetComponent);
		void SetTargetBoneName(const class FName& i_targetBoneName);
		void SetSpellCastHandle(int32_t InSpellCastHandle);
		void SetSpell(int32_t Group, int32_t Index, class USpellToolRecord* Spell);
		void SetRightArmState(EnumGeneric InState, EnumGeneric InPriority, bool bBroadcastChange);
		void SetRayCastActor(class AActor* i_pRayCastActor);
		void SetPickupTool(class APickupTool* InPickupTool);
		void SetPartialBodyState(EnumGeneric InState);
		void SetParryState(EnumGeneric InState);
		void SetFullBodyStateLast(EnumGeneric InState);
		void SetFullBodyState(EnumGeneric InState);
		void SetFinisherOptimalRatio(float OptimalRatio, EnumGeneric FeedbackAction);
		void SetFinisher(int32_t Index, class USpellToolRecord* Spell);
		void SetCombatReadyMode(bool Flag);
		void SetAdditiveBodyState(EnumGeneric InState);
		class FName ParkourJump(const class FName& DefaultNode);
		void OnSpellCast__DelegateSignature(class AActor* Target, const struct FVector& Location, class ASpellTool* Spell, const class FName& SpellType, class AMunitionType_Base* Munition);
		void OnProtegoStart__DelegateSignature();
		void OnProtegoEnd__DelegateSignature();
		void OnProtegoDeflected__DelegateSignature(class ASpellTool* Spell);
		void OnLumosStart__DelegateSignature();
		void OnLumosEnd__DelegateSignature();
		void OnHoverDroneControlModeChanged(EnumGeneric NewMode);
		void OnEquippedChanged(class ATool* ActivatedTool, class ATool* DeactivatedTool);
		void OnDisillusionmentStart__DelegateSignature();
		void OnDisillusionmentEnd__DelegateSignature();
		void OnAccioStart__DelegateSignature(class AActor* Target, const struct FVector& Location);
		void OnAccioEnd__DelegateSignature(class AActor* Target);
		void Landed(const struct FEngine_FHitResult& Hit);
		bool IsWandEquipped();
		bool IsDead();
		bool IsCurrentLedgeAnOverhang();
		bool IsBlockedByWall();
		class AWandTool* GetWand();
		struct FVector GetToTargetDirection();
		struct FVector GetTargetDirection();
		struct FVector GetTargetDestination();
		class UPrimitiveComponent* GetTargetComponent();
		class FName GetTargetBoneName();
		bool GetSpellsByGroup(int32_t Group, TArray<class USpellToolRecord*>* ToolRecordArray);
		void GetSpellCastHandle(int32_t* OutSpellCastHandle);
		class USkeletalMeshComponent* GetSkeletalMesh();
		EnumGeneric GetRightArmState(EnumGeneric InPriority);
		class AActor* GetRayCastActor();
		float GetPutdownToolHeight();
		float GetPickupToolHeight();
		class APickupTool* GetPickupTool();
		void GetPartialBodyState(EnumGeneric* OutState);
		void GetParryState(EnumGeneric* OutState);
		struct FTransform GetOriginalLedgeTransform();
		class AActor* GetLockedOnActor();
		struct FInventoryResult GetInventoryResult();
		class AActor* GetHighlitActor();
		float GetHealth();
		void GetFullBodyStateLast(EnumGeneric* OutState);
		void GetFullBodyState(EnumGeneric* OutState);
		bool GetFinishers(TArray<class USpellToolRecord*>* ToolRecordArray);
		class FName GetCharacterID();
		class AActor* GetBestLockOnActor();
		class AActor* GetBestAutoTargetActor();
		struct FVector GetAutoTargetUpDirection();
		struct FVector GetAutoTargetFocusDirection();
		void GetAdditiveBodyState(EnumGeneric* OutState);
		void FinishCrouching(class UObject* Provider);
		void FacingTargetsListIsNowEmpty(EnumGeneric LastTargetType, EnumGeneric LastTargetID);
		void FacingNotifyOfNewBestTarget(EnumGeneric NewTargetType, EnumGeneric NewTargetID);
		void EquipWand();
		void EnablePawnInteraction(bool bEnable);
		bool DetectFallIntoWater();
		bool DetectDiveIntoWater();
		void ComputeAttack(class AActor** TargetActor);
		float ComputeAngleToDesiredDirection();
		void ClimbLedgeStart();
		void ClimbLedgeEnd();
		void CleanUpFinisherSpell();
		bool CanLandOnLedge();
		void CancelCurrentSpell(bool RemoveDisillusionment);
		void CalculateLedgePullUpState(EnumGeneric* OutState, EnumGeneric* OutType);
		void CalculateClimbingLedgeState(EnumGeneric* OutState, EnumGeneric* OutType);
		bool AdjustHealth(float change, bool bDontKill);
		static UClass* StaticClass();
	};

	bool ABiped_Character::AdjustHealth(float change, bool bDontKill)
	{
		static UFunction* fn = UObject::FindObject<UFunction>("Function Phoenix.Biped_Character.AdjustHealth");

		struct {
			float change;
			bool bDontKill;
		}params;

		params.change = change;
		params.bDontKill = bDontKill;

		UObject::ProcessEvent(fn, &params);
	}

	class ABiped_Player : public ABiped_Character
	{
	public:
		unsigned char                                              pad_1A70[0x18];                                  // 0x1A70(0x0018) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UConversationComponent* ConversationComponent;                                   // 0x1A88(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		class UCustomizableCharacterComponent* Customization;                                           // 0x1A90(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		class ASocialReasoning* PlayerSocialReasoning;                                   // 0x1A98(0x0008) ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		unsigned char                                              pad_1AA0[0x50];                                  // 0x1AA0(0x0050) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UToolSetComponent* InventoryToolSetComponent;                               // 0x1AF0(0x0008) Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, EditConst, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       bAllowFastWalk;                                          // 0x1AF8(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_1AF9[0x7];                                   // 0x1AF9(0x0007) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UCurveFloat* WalkSlowModeSpeedCurve;                                  // 0x1B00(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCurveFloat* WalkModeSpeedCurve;                                      // 0x1B08(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCurveFloat* WalkOnlyModeSpeedCurve;                                  // 0x1B10(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCurveFloat* WalkFastModeSpeedCurve;                                  // 0x1B18(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCurveFloat* WalkFastOnlyModeSpeedCurve;                              // 0x1B20(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCurveFloat* JogModeSpeedCurve;                                       // 0x1B28(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCurveFloat* JogOnlyModeSpeedCurve;                                   // 0x1B30(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCurveFloat* SprintModeSpeedCurve;                                    // 0x1B38(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCurveFloat* JogModeSpeedGovernorCurve;                               // 0x1B40(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UClass* GryffindorMaleBedAbility;                                // 0x1B48(0x0008) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UClass* GryffindorFemaleBedAbility;                              // 0x1B50(0x0008) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UClass* RavenClawMaleBedAbility;                                 // 0x1B58(0x0008) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UClass* RavenClawFemaleBedAbility;                               // 0x1B60(0x0008) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UClass* HufflePuffMaleBedAbility;                                // 0x1B68(0x0008) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UClass* HufflePuffFemaleBedAbility;                              // 0x1B70(0x0008) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UClass* SlytherinMaleBedAbility;                                 // 0x1B78(0x0008) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UClass* SlytherinFemaleBedAbility;                               // 0x1B80(0x0008) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UClass* AttackIndicatorActorClass;                               // 0x1B88(0x0008) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class APlayer_AttackIndicator* AttackIndicatorActor;                                    // 0x1B90(0x0008) ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      InnerDeadZoneMin;                                        // 0x1B98(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      InnerDeadZoneMax;                                        // 0x1B9C(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      OuterDeadZone;                                           // 0x1BA0(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		EnumGeneric                                             EdgeState;                                               // 0x1BA4(0x0001) Edit, BlueprintVisible, ZeroConstructor, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_1BA5[0x3];                                   // 0x1BA5(0x0003) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		struct FVector                                             EdgeLocation;                                            // 0x1BA8(0x000C) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		struct FVector                                             EdgeNormal;                                              // 0x1BB4(0x000C) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      EdgeDistanceToGround;                                    // 0x1BC0(0x0004) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      EdgeDistanceFromEdge;                                    // 0x1BC4(0x0004) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		struct FVector                                             EdgeImpulse;                                             // 0x1BC8(0x000C) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_1BD4[0x4];                                   // 0x1BD4(0x0004) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class USplineComponent* SyncToSplineComponent;                                   // 0x1BD8(0x0008) Edit, BlueprintVisible, ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      SyncToSplineDirection;                                   // 0x1BE0(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      SyncToSplineDistance;                                    // 0x1BE4(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UClass* DefaultIdleAbility;                                      // 0x1BE8(0x0008) Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic
		int32_t                                                    MaxChompingCabbage;                                      // 0x1BF0(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate
		int32_t                                                    MaxVenomousTentacula;                                    // 0x1BF4(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate
		bool                                                       AllowPlayerCamContorlOnOtherActorCam;                    // 0x1BF8(0x0001) BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       InvertCameraPitchControl;                                // 0x1BF9(0x0001) BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       InvertCameraYawControl;                                  // 0x1BFA(0x0001) BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       InvertMoveLeftRightControl;                              // 0x1BFB(0x0001) BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       InvertMoveForwardBackControl;                            // 0x1BFC(0x0001) BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       EnableFadeSystem;                                        // 0x1BFD(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_1BFE[0x2];                                   // 0x1BFE(0x0002) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class AFieldGuideHotSpot* PlayerInThisFieldGuideHotSpot;                           // 0x1C00(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      FinisherDamage;                                          // 0x1C08(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      ElderWandFinisherDamage;                                 // 0x1C0C(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      MaxFocus;                                                // 0x1C10(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		int32_t                                                    InitialNumBarsFilled;                                    // 0x1C14(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      FinisherFocusCost;                                       // 0x1C18(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      ManagedFinisherFocusCost;                                // 0x1C1C(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      PushFocusCost;                                           // 0x1C20(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_1C24[0x4];                                   // 0x1C24(0x0004) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		unsigned char                                   FocusMap[0x50];                                                // 0x1C28(0x0050) Edit, DisableEditOnInstance, NativeAccessSpecifierPublic
		TArray<class UMultiFX2_Base*>                              FinisherAvailableFX;                                     // 0x1C78(0x0010) Edit, ExportObject, ZeroConstructor, DisableEditOnInstance, ContainsInstancedReference, NativeAccessSpecifierPublic
		float                                                      ComboResetTime;                                          // 0x1C88(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      AdditionalComboResetTimeOnParry;                         // 0x1C8C(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		int32_t                                                    CooldownPickupFirstTierSize;                             // 0x1C90(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		int32_t                                                    CooldownPickupTierSize;                                  // 0x1C94(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		TArray<int32_t>                                            CooldownPickupCountArray;                                // 0x1C98(0x0010) Edit, ZeroConstructor, DisableEditOnInstance, NativeAccessSpecifierPublic
		float                                                      ExtendedCombatTime;                                      // 0x1CA8(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class FScriptDelegate                                      ClothTargetsStartDelegate;                               // 0x1CAC(0x0010) ZeroConstructor, Transient, InstancedReference, NoDestructor, NativeAccessSpecifierPrivate
		class FScriptDelegate                                      ClothTargetsEndDelegate;                                 // 0x1CBC(0x0010) ZeroConstructor, Transient, InstancedReference, NoDestructor, NativeAccessSpecifierPrivate
		float                                                      HealthIncreasePerLevel;                                  // 0x1CCC(0x0004) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class FName                                                LeftFootSocketName;                                      // 0x1CD0(0x0008) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class FName                                                RightFootSocketName;                                     // 0x1CD8(0x0008) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCurveFloat* LeftStickMagWalkingCurve;                                // 0x1CE0(0x0008) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCurveFloat* LeftStickMagJoggingCurve;                                // 0x1CE8(0x0008) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      BulletTimeStartSeconds;                                  // 0x1CF0(0x0004) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_1CF4[0x4];                                   // 0x1CF4(0x0004) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UCurveFloat* BulletTimeDilationCurve;                                 // 0x1CF8(0x0008) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      BulletTimeDurationSeconds;                               // 0x1D00(0x0004) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      BulletTimeDialation;                                     // 0x1D04(0x0004) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      CutBackToPlayerCamAfterTimeDialationSec;                 // 0x1D08(0x0004) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_1D0C[0x4];                                   // 0x1D0C(0x0004) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		unsigned char                                  DamageEffectBlendable[0x10];                                   // 0x1D10(0x0010) BlueprintVisible, NoDestructor, NativeAccessSpecifierPublic
		class UMaterialInstanceDynamic* DamageEffectMaterialInstance;                            // 0x1D20(0x0008) BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UCurveFloat* DamageEffectCurve;                                       // 0x1D28(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      DamageEffectDuration;                                    // 0x1D30(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      DamageEffectScale;                                       // 0x1D34(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		TArray<class UMultiFX2_Base*>                              CriticalHealthFX;                                        // 0x1D38(0x0010) Edit, ExportObject, ZeroConstructor, DisableEditOnInstance, ContainsInstancedReference, NativeAccessSpecifierPublic
		class UCurveFloat* CriticalHealthPulseTimeCurve;                            // 0x1D48(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		TArray<class UMultiFX2_Base*>                              CriticalHealthPulseFX;                                   // 0x1D50(0x0010) Edit, ExportObject, ZeroConstructor, DisableEditOnInstance, ContainsInstancedReference, NativeAccessSpecifierPublic
		float                                                      CriticalHealthFadeOutTime;                               // 0x1D60(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_1D64[0x4];                                   // 0x1D64(0x0004) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UClass* DamageDirectionClass;                                    // 0x1D68(0x0008) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      PercentHealthRecoverdPerSecond;                          // 0x1D70(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      SecondsAfterCombatBeforeRecoveryStarts;                  // 0x1D74(0x0004) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       DoDamageOnNextLanding;                                   // 0x1D78(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       UseDodgeCamera;                                          // 0x1D79(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       InHoverDrone;                                            // 0x1D7A(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       bLookAtHips;                                             // 0x1D7B(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      cameraOffsetScale;                                       // 0x1D7C(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       bLockOutPlayerCamControl;                                // 0x1D80(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       bLockOutPlayerCamControlPitch;                           // 0x1D81(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_3000[0xE];                                   // 0x1D82(0x000E) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		TArray<struct FGearItem>                                   CachedGearPieces;                                        // 0x1D90(0x0010) ZeroConstructor, Transient, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             OffenseStat;                                             // 0x1DA0(0x0090) BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             DefenseStat;                                             // 0x1E30(0x0090) BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             CooldownStat;                                            // 0x1EC0(0x0090) BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic
		struct FGameplayProperty_Int                               InventorySizeModification;                               // 0x1F50(0x0080) BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             HiddenAfterTakedownTime;                                 // 0x1FD0(0x0090) BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             StatDamageMultiplier;                                    // 0x2060(0x0090) BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             StatDamageReduction;                                     // 0x20F0(0x0090) BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             StatCooldownReduction;                                   // 0x2180(0x0090) BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             GainKnowledgeModifier;                                   // 0x2210(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             GainExperienceModifier;                                  // 0x22A0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             CompanionBondingPointsModifier;                          // 0x2330(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             VendorPricesModifier;                                    // 0x23C0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             EnemyEvadeChanceModifier;                                // 0x2450(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             EnemyProtegoChanceModifier;                              // 0x24E0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             EnemyAwarenessRateModifier;                              // 0x2570(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             CriticalSuccessChanceModifier;                           // 0x2600(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             LootDropCountModifier;                                   // 0x2690(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             LootDropRareChanceModifier;                              // 0x2720(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             StealRateModifier;                                       // 0x27B0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             CaptureToolRangeModifier;                                // 0x2840(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             CaptureToolDecayPenaltyModifier;                         // 0x28D0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             CaptureToolCaptureSpeedModifier;                         // 0x2960(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             CreatureAgroFleeDistanceModifier;                        // 0x29F0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             FocusModifier;                                           // 0x2A80(0x0090) Edit, BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             FocusOnHitModifier;                                      // 0x2B10(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             UseConsumeableForFreeChance;                             // 0x2BA0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             UsePotionForFreeChance;                                  // 0x2C30(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Int                               ObscurePerkDistance;                                     // 0x2CC0(0x0080) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             MaliciousPerkMultiplier;                                 // 0x2D40(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             ExpelliarmusLootDropChance;                              // 0x2DD0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Int                               CombatAuxPerk;                                           // 0x2E60(0x0080) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             CombatAuxPerkDuration;                                   // 0x2EE0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             CombatAuxPerkCooldown;                                   // 0x2F70(0x0090) Edit, NativeAccessSpecifierPublic
		unsigned char                                              pad_38F3[0x8];                                   // 0x3000(0x0008) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		struct FGameplayProperty_Int                               DirectAIPerk;                                            // 0x3008(0x0080) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Int                               ExtendedCompanionInventoryCapacity;                      // 0x3088(0x0080) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             PlantDamageModifier;                                     // 0x3108(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             VenomousTentaculaPoisonDamageModifier;                   // 0x3198(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             VenomousTentaculaWeakenEnemyDuration;                    // 0x3228(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             HealingCooldownGroupModifier;                            // 0x32B8(0x0090) Edit, NativeAccessSpecifierPublic
		TWeakObjectPtr<class AActor>                               LastStupefyTarget;                                       // 0x3348(0x0008) ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             CabbageVampireModifier;                                  // 0x3350(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             PlantDamageFocus;                                        // 0x33E0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             PlayerStupifyPlantDamageModifier;                        // 0x3470(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             MandrakeBonusReactionTime;                               // 0x3500(0x0090) Edit, BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             PlantSummonHealthPercent;                                // 0x3590(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             DifficultyAttackCooldownModifier;                        // 0x3620(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             DisillusionmentModifier;                                 // 0x36B0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             CrimeSceneInvestigationModifier;                         // 0x3740(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             SneakOScopeWindupModifier;                               // 0x37D0(0x0090) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Float                             SilencioRangeModifier;                                   // 0x3860(0x0090) Edit, NativeAccessSpecifierPublic
		bool                                                       bAllowEdgeCheck;                                         // 0x38F0(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       bInCombatMode;                                           // 0x38F1(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       bInCombatModeDelayed;                                    // 0x38F2(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_3904[0x5];                                   // 0x38F3(0x0005) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class AActor* FollowActor;                                             // 0x38F8(0x0008) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		int32_t                                                    CautiousMode;                                            // 0x3900(0x0004) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_3A08[0x4];                                   // 0x3904(0x0004) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		struct FGameplayProperty_Int                               BonusAncientMagicBars;                                   // 0x3908(0x0080) Edit, NativeAccessSpecifierPublic
		struct FGameplayProperty_Int                               BonusSpellLoadouts;                                      // 0x3988(0x0080) Edit, NativeAccessSpecifierPublic
		unsigned char                                              pad_3A19[0x10];                                  // 0x3A08(0x0010) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		bool                                                       InStealthMode;                                           // 0x3A18(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_3A25[0x7];                                   // 0x3A19(0x0007) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		bool                                                       bIsInvisible;                                            // 0x3A20(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       InCinematic;                                             // 0x3A21(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		EnumGeneric                                            WandLinkResult;                                          // 0x3A22(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_3A48[0x1];                                   // 0x3A23(0x0001) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		bool                                                       bHoldingProtegoParry;                                    // 0x3A24(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_3B35[0x3];                                   // 0x3A25(0x0003) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UClass* EnemyDetectionFXClass;                                   // 0x3A28(0x0008) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UAkAudioEvent* EnemyDetectionSFX;                                       // 0x3A30(0x0008) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UAkAudioEvent* EnemyAlertSFX;                                           // 0x3A38(0x0008) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class UAkAudioEvent* EnemyAggroSFX;                                           // 0x3A40(0x0008) Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_3B50[0x60];                                  // 0x3A48(0x0060) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class FScriptMulticastDelegate                             OnSpellCooldownChanged;                                  // 0x3AA8(0x0010) ZeroConstructor, InstancedReference, NativeAccessSpecifierPublic
		class FScriptMulticastDelegate                             OnFocusChanged;                                          // 0x3AB8(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		TMap<class FName, struct FSpellCooldownData>               CooldownMap;                                             // 0x3AC8(0x0050) NativeAccessSpecifierPublic
		class FScriptMulticastDelegate                             OnStationInteractionExitComplete;                        // 0x3B18(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, BlueprintCallable, NativeAccessSpecifierPublic
		struct FVector                                             DesiredFocusDirection;                                   // 0x3B28(0x000C) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		bool                                                       bUseDesiredFocusDirection;                               // 0x3B34(0x0001) Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_3B64[0x3];                                   // 0x3B35(0x0003) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UVegetationInteractionComponent* VegetationInteraction;                                   // 0x3B38(0x0008) Edit, ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		class FScriptMulticastDelegate                             OnTeleportTo;                                            // 0x3B40(0x0010) ZeroConstructor, InstancedReference, BlueprintAssignable, NativeAccessSpecifierPublic
		unsigned char                                              pad_3DE8[0x8];                                   // 0x3B50(0x0008) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UClass* LastCriticalFinisher;                                    // 0x3B58(0x0008) ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic
		float                                                      TimeoutLookAtCameraDirection;                            // 0x3B60(0x0004) Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              pad_3D10[0x26C];                                 // 0x3B64(0x026C) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UAkComponent* MotionListenerCameraOrientation;                         // 0x3DD0(0x0008) ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		class UAkComponent* MotionListenerPlayerOrientation;                         // 0x3DD8(0x0008) ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		class AActor* DamageDirectionActor;                                    // 0x3DE0(0x0008) ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		unsigned char                                              UnknownData_RJZK[0x68];                                  // 0x3DE8(0x0068) MISSED OFFSET (PADDING)

		bool UseNewConversations();
		bool UseInventoryItemByName(const class FName& InventoryObjectID, const class FName& HolderID);
		void UseFinisherFocus();
		void UpdateFinsherAvailableFX();
		void UpdateDifficulty();
		void UpdateCamMove(float DeltaTime);
		bool ToadStatueTeleport(class AActor* InDestinationActor, const struct FVector& InOffset);
		void TargetSighted(class UAISense* Sense, class AActor* Target);
		void TargetLost(class UAISense* Sense, class AActor* OldTarget);
		void TargetChanged(class UAISense* Sense, class AActor* NewTarget, class AActor* OldTarget);
		void SuspendStoryModeArmor(bool bFlag);
		bool StopPlayerInput();
		void StopMove();
		void StationInteractionExitComplete__DelegateSignature();
		class AActor* StartCriticalFinisher();
		void StartBulletTime(float Dilation, float Duration, bool DoFixedCam, float PlayerDilation, float EaseInDuration, float EaseOutDuration);
		void SpawnRockSlide(class UClass* InActorClass, float ProjectForward);
		void SpawnItemMountCallback(class UInventoryItemToolRecord* InToolRecord);
		class AActor* SpawnFaceTarget(class UClass* SpawnActorClass);
		bool ShadowBlinkTeleport(class AActor* InBlinkTarget, float InBlinkDistanceFromTarget, bool bIsGate, bool bIsStealth, struct FVector* OutBlinkLocation, struct FRotator* OutBlinkRotation);
		void SetWandStyle(const class FName& WandStyle);
		void SetWandBaseStyle(const class FName& InWandBase);
		void SetUsingElderWand(bool bUsing);
		void SetStartingMovementTime(float InStartingMovementTime);
		bool SetPlayerSocialHeritage(EnumGeneric NewHeritage);
		bool SetPlayerGenderPronoun(EnumGeneric NewGenderPronoun);
		void SetPhoenixCameraRotation(const struct FRotator& DesiredRotation);
		void SetPhoenixCameraLookAt_TimeBased(const struct FVector& LookAtPoint, float LookAtTime);
		void SetPhoenixCameraLookAt_ActorAndTime(class AActor* LookAtActor, float LookAtTime);
		void SetPhoenixCameraLookAt_Actor(class AActor* LookAtActor, float ScaleSpeed);
		void SetPhoenixCameraLookAt(const struct FVector& LookAtPoint, float ScaleSpeed);
		void SetOverrideWandStyle(const class FName& WandStyle);
		void SetMotionListener(class UAkComponent* AkComponent, bool PlayerOrientation);
		void SetManagedAM(float Value);
		void SetLastSafeInteractLocation(const struct FVector& Location);
		void SetLastEdgeState(EnumGeneric InEdgeState);
		void SetInvulnerableTime(float invulnerableTime);
		void SetInvulnerable(bool bInInvulnerable);
		void SetInteractStickyTarget(bool bSet);
		void SetInLockOnMode(bool bInFlag);
		void SetHighlitActor(class AActor* InHighlitActor);
		void SetFieldGuideHotSpot(class AFieldGuideHotSpot* InFieldGuideHotSpot);
		void SetDodgeDirection();
		void SetDisableCombatPlantHideOnSuspendEnemies(bool bInVal);
		void SetCooldownRateScale(float RateScale);
		void SetCauseOfDeath(EnumGeneric InCauseOfDeath);
		void SetCameraStyle(const class FName& CameraStyle);
		void SetAutoTargetingDirection(const struct FVector& InWorldDirection);
		void SetAccessibilitySpellToggle(bool ToggleOn);
		class AActor* Set_WandCast_TargetTrackerTargets(bool bIgnoreTargets, class AActor* InTargetOverride, const struct FVector& InTargetOverrideWorldLocation);
		void SendCriticalMunitionImpactToTarget(class AActor* TargetActor, const class FName& UniqueTag, TArray<class UInteractionArchitectAsset*> MunitionDataAssets, const struct FGameplayTagContainer& MunitionTagContainer, float Damage);
		bool SafeTeleportTo(const struct FVector& DestLocation, const struct FRotator& DestRotation, bool bIsATest, bool bNoCheck);
		bool RestorePlayerInput();
		void RestoreHealth();
		void RestartComboTimeout();
		void ResetStickTime();
		void ResetPhoenixCameraRotation(float ResetSpeed, class UCurveFloat* ResetSpeedCurve);
		void ResetComboPoints();
		void ResetCloth();
		void RequestStick(float StickX, float StickY);
		void RepairOrientation();
		void ReleasePhoenixCameraLookAt();
		void RecordSafeLocation();
		void RecordSafeInteractLocation();
		bool QuickActionInteract(int32_t Index);
		void PutPlayerIntoCinematicState(bool bFlag, bool bAlterAbilities);
		void PushDisableStack(const struct FName_GameLogicVarInt32& StackName);
		bool ProcessStick(float StickX, float StickY, float DeltaTime, class UCurveFloat* DeadZoneCurve, class UCurveFloat* SpeedCurve, class UCurveFloat* SpeedGovernorCurve, float* WorldSpeed, struct FVector* WorldDirection, EnumGeneric InterpType, float InterpSpeed, float StickMagReductionDelayTimer);
		bool ProcessFollowTarget(class AActor* InFollowActor, class UCurveFloat* SpeedCurve, float* OutWorldSpeed, struct FVector* OutWorldDirection);
		void PopDisableStack(const struct FName_GameLogicVarInt32& StackName);
		void PlayerGetOutOfBed(class AStation* BedStation, class UClass* PlayerBedAbility);
		bool PlacePlayerInBed(class AStation** BedStation, class UClass** PlayerBedAbility);
		bool PlaceOnGround(float DownDistance);
		void PauseMenuStart();
		void PauseComboTimeout();
		void OnTeleportTo__DelegateSignature(const struct FVector& PrevLocation, const struct FRotator& PrevRotation, bool bIsATest, bool bNoCheck, bool bSuccess);
		void OnSpellCooldownChanged__DelegateSignature(const class FName& SpellName, float CooldownPct);
		void OnSaveGameLoaded();
		void OnQuickItemSlotted(class UObject* InCaller, const class FName& ItemName);
		void OnNewGame(class UObject* InCaller, bool bInIsIntro);
		void OnFocusChanged__DelegateSignature(class AActor* Target, float InFocusChange);
		void OnCurtainRaised();
		void OnCharacterLoadComplete(class AActor* Actor);
		void OnBlackboardInitialized(bool bSuccess);
		void OnAboutToSaveGame();
		void OnAbilityAnimationBecameActive();
		void NotifySucessfulBlock();
		void NotifyDodgeFinished();
		void LockOnCamFlick(class UStickFlickWatcherRecord* InStickFlickWatcherRecord);
		void LockOnCameraMoving(bool bFlag);
		bool LoadInventoryItemByName(const class FName& InventoryObjectID, const class FName& HolderID);
		void LevelUp();
		void LEDDamageEffect(class AActor* InActor, class AActor* InInstigator, float InDamage, const struct FEngine_FHitResult& InHit);
		void LeafNodeSwitched();
		bool IsVenomousTentaculaAtMax();
		bool IsSwimming();
		bool IsPlayingIntro();
		bool IsPerfectRetaliate();
		bool IsManagedAMEnabled();
		bool IsLockedOn();
		bool IsInventoryItemHighlighted();
		bool IsInLockOnMode();
		bool IsHoverDroneActive();
		bool IsFinisherAvailable();
		bool IsCrawling();
		bool IsChompingCabbageAtMax();
		bool IsCameraMoving();
		bool IsAMPushAvailable();
		void InteractingWithActor(class AActor* InActor);
		bool InstaUseInventoryItem();
		bool InLockOnMode();
		bool InCombatModeDelayed();
		bool InCombatMode();
		bool IgnoreRetaliateTarget(class AActor* RetaliateTarget);
		void HighestEnemyChanged(class AActor* InEnemyActor, int32_t InLevelDiff);
		void HideReticle(bool bHide);
		bool HasMandrakeTalent();
		void HandleVenomousTentaculaCountChanged();
		bool HandleDodgeUnblockable(class AActor** OutAttacker);
		void HandleChompingCabbageCountChanged();
		void HandleActorHit(class AActor* SelfActor, class AActor* OtherActor, const struct FVector& NormalImpulse, const struct FEngine_FHitResult& Hit);
		class FName GetWandStyle();
		class FName GetWandBaseStyle();
		int32_t GetVenomousTentaculaMax();
		int32_t GetVenomousTentaculaCount();
		class ABiped_Player* GetTheBipedPlayer();
		class UCurveFloat* GetSpeedGovernorCurve();
		class UCurveFloat* GetSpeedCurve(bool* bIgnoreGovernor);
		EnumGeneric GetPlayerSocialHeritage();
		float GetPlayerMovementDuration();
		float GetPlayerLastUsedItemEffectiveness();
		EnumGeneric GetPlayerGenderPronoun();
		int32_t GetPlantCount(const class FString& trackerName);
		int32_t GetOffenseStat();
		void GetMoveStickValues(float* X, float* Y);
		class UAkComponent* GetMotionListenerPlayerOrientation();
		class UAkComponent* GetMotionListenerCameraOrientation();
		float GetMaxFocus();
		struct FVector GetLastSafeLocation();
		struct FVector GetLastSafeInteractLocation();
		bool GetIsOnAMountOrInTransition();
		int32_t GetInventorySizeModification();
		float GetHiddenAfterTakedownTime();
		bool GetGameLogicBoolVariable(const struct FName_GameLogicVarBool& BoolVar);
		float GetFocusPercent();
		float GetFocus();
		float GetFinisherDamage();
		float GetExpectedDistanceFromTarget(bool bIgnoreTargets, class AActor* InTargetOverride, bool* bFound, struct FVector* ToTarget);
		int32_t GetDefenseStat();
		float GetCurrentCriticalHealthFadeOutPercent();
		int32_t GetCooldownStat();
		float GetCooldownRateScale();
		float GetCooldownPercentByName(const class FName& SpellName);
		float GetCooldownPercent(class USpellToolRecord* InSpellToolRecord);
		int32_t GetConsecutiveEnemyHits();
		float GetCompanionsAtMaxLevel();
		float GetComboPoints();
		int32_t GetChompingCabbageMax();
		int32_t GetChompingCabbageCount();
		EnumGeneric GetCauseOfDeath();
		int32_t GetBonusSpellLoadouts();
		int32_t GetBonusAncientMagicBars();
		class UBiped_AnimInstance* GetBipedAnimInstance();
		struct FVector GetBestLockOnActor_WorldLocation();
		struct FVector GetBestLockOnActor_ScreenLocation();
		struct FVector GetBestAutoTargetActor_WorldLocation();
		float GetBaseOffenseStat();
		float GetBaseDefenseStat();
		float GetBaseCooldownStat();
		bool GetAccessibilitySpellToggle();
		void FullyResetPhoenixCamera(float ResetSpeed, class UCurveFloat* ResetSpeedCurve);
		void ForceEndDisillusionment();
		void EndCriticalFinisher(class AActor* InTargetActor);
		void EndBulletTime(float EaseOutDuration);
		void EnableManagedAM(bool Enabled);
		void DisablePlayerHealthRecovery(bool bDisable);
		void DisableCooldownPickups(bool bDisable);
		void DetectEdgeChange(const struct FVector& TestActorLocation, float TraceRadius, bool bSendEvents, bool bDebugDraw);
		void DEBUG_SetGlobalTimeDilation(class UObject* WorldContextObject, float TimeDilation);
		void DamageDirectionEffect(class AActor* InActor, class AActor* InInstigator, float InDamage, const struct FEngine_FHitResult& InHit);
		bool CreateLinkToAttackingActor(EnumGeneric ParryEventReason, bool* bOutDodgeWindow);
		void ComputeAimOffset(const class FName& BoneName, float* Pitch, float* Yaw);
		void CognitionTargetPlayerAwareStateChanged(EnumGeneric InAwareState);
		void CognitionTargetHealthChanged(class AActor* Target, float InHealthChanged, bool bIndicateHUD);
		void ClearStick();
		void ClearOverrideWandStyle();
		void ClearInvulnerable();
		void ClearCustomCameras();
		void ChangeManagedAM(float change);
		void CameraStickLastMovedChanged(bool InCameraStickMoved);
		float CalculateVendorPricesModifier(const struct FVendorTransactionInfo& Info);
		float CalculateStealRateModifier();
		float CalculateSneakOScopeWindupModifier();
		float CalculateSilencioRangeModifier();
		float CalculatePlantDamageModifier(const class FName& DbId);
		float CalculateMandrakeBonusReaction();
		struct FVector CalculateLookAtDirection();
		int32_t CalculateExtendedCompanionInventoryCapacity();
		bool AutoNavToTarget(class AActor* InTarget, float InSpeed, float DistanceFromTarget);
		bool AllowStop(float InWorldSpeed, const struct FVector& InWorldDirection, float DeltaTime, float MinTime);
		bool AllowMove(float InWorldSpeed, const struct FVector& InWorldDirection, float DeltaTime, float MinTime);
		void AdjustFocusByName(const class FName& Name, float ExternalFocusModifier);
		void AdjustFocus(float change, float ExternalFocusModifier);
		void AdjustCooldownsByCategory(float Time, EnumGeneric SpellCategory);
		void AdjustCooldown(const class FName& SpellName, float Time, bool AddIfNoneExists);
		void AdjustComboPoints(EnumGeneric ComboType, class AActor* Target);
		void AddCustomCamera(class UCameraComponent* customCam, float weight);
		static UClass* StaticClass();
	};

	void ACharacter::Jump()
	{

		static auto fn = UObject::FindObject<UFunction>("Function Engine.Character.Jump");

		struct {}params;

		UObject::ProcessEvent(fn, &params);
	}

	int32_t TUObjectArray::Count() const
	{
		return NumElements;
	}

	int32_t TUObjectArray::Max() const
	{
		return MaxElements;
	}

	bool TUObjectArray::IsValidIndex(int32_t Index) const
	{
		return Index < Count() && Index >= 0;
	}

	FUObjectItem* TUObjectArray::GetObjectPtr(int32_t Index) const
	{
		const int32_t ChunkIndex = Index / NumElementsPerChunk;
		const int32_t WithinChunkIndex = Index % NumElementsPerChunk;
		if (!IsValidIndex(Index)) return nullptr;
		if (ChunkIndex > NumChunks) return nullptr;
		if (Index > MaxElements) return nullptr;
		FUObjectItem* Chunk = Objects[ChunkIndex];
		if (!Chunk) return nullptr;
		return Chunk + WithinChunkIndex;
	}

	UObject* TUObjectArray::GetByIndex(int32_t index) const
	{
		FUObjectItem* ItemPtr = GetObjectPtr(index);
		if (!ItemPtr) return nullptr;
		return (*ItemPtr).Object;
	}

	FUObjectItem* TUObjectArray::GetItemByIndex(int32_t index) const
	{
		FUObjectItem* ItemPtr = GetObjectPtr(index);
		if (!ItemPtr) return nullptr;
		return ItemPtr;
	}

	UObject* TUObjectArray::operator[](int32_t i)
	{
		return GetByIndex(i);
	}

	const UObject* TUObjectArray::operator[](int32_t i) const
	{
		return GetByIndex(i);
	}

	int32_t FNameEntry::GetLength() const
	{
		return Header.Len;
	}

	bool FNameEntry::IsWide() const
	{
		return Header.bIsWide;
	}

	int32_t FNameEntry::GetId() const
	{
		throw std::exception("ERROR");
	}

	std::string FNameEntry::GetAnsiName() const
	{
		uint32_t len = GetLength();
		if (len > 1024) return "ERROR";
		return std::string((const char*)AnsiName, len);
	}

	std::wstring FNameEntry::GetWideName() const
	{
		uint32_t len = GetLength();
		return std::wstring((const wchar_t*)WideName, len);
	}

	std::string FNameEntry::GetName() const
	{
		return GetAnsiName();
	}

	int32_t FNameEntryAllocator::NumBlocks() const
	{
		return CurrentBlock + 1;
	}

	FNameEntry* FNameEntryAllocator::GetById(int32_t key) const
	{
		int block = key >> 16;
		int offset = (uint16_t)key;
		if (!IsValidIndex(key, block, offset))
			return reinterpret_cast<FNameEntry*>(Blocks[0] + 0); // "None"
		return reinterpret_cast<FNameEntry*>(Blocks[block] + ((uint64_t)offset * Stride));
	}

	bool FNameEntryAllocator::IsValidIndex(int32_t key) const
	{
		uint32_t block = key >> 16;
		uint16_t offset = key;
		return IsValidIndex(key, block, offset);
	}

	bool FNameEntryAllocator::IsValidIndex(int32_t key, uint32_t block, uint16_t offset) const
	{
		return (key >= 0 && block < static_cast<uint32_t>(NumBlocks()) && offset* Stride < MaxOffset);
	}

	int32_t FNamePool::Count() const
	{
		return AnsiCount;
	}


	bool FNamePool::IsValidIndex(int32_t index) const
	{
		return Allocator.IsValidIndex(static_cast<int32_t>(index));
	}


	FNameEntry* FNamePool::GetById(int32_t id) const
	{
		return Allocator.GetById(id);
	}


	FNameEntry* FNamePool::operator[](int32_t id) const
	{
		return GetById(id);
	}


	FName::FName()
	{
		ComparisonIndex = 0;
		Number = 0;
	}


	FName::FName(int32_t i)
	{
		ComparisonIndex = i;
		Number = 0;
	}

	FName::FName(const char* nameToFind)
	{
		Number = 0;
		static std::unordered_set<int> cache;
		for (auto i : cache)
		{
			if (GetGlobalNames()[i]->GetAnsiName() == nameToFind)
			{
				ComparisonIndex = i;
				return;
			}
		}
	}


	FName::FName(const wchar_t* nameToFind)
	{
		Number = 0;
		static std::unordered_set<int> cache;
		for (auto i : cache)
		{
			if (GetGlobalNames()[i]->GetWideName() == nameToFind)
			{
				ComparisonIndex = i;
				return;
			}
		}
	}


	FNamePool& FName::GetGlobalNames()
	{
		return *GNames;
	}


	std::string FName::GetNameA() const
	{
		return GetGlobalNames()[ComparisonIndex]->GetAnsiName();
	}


	std::wstring FName::GetNameW() const
	{
		return GetGlobalNames()[ComparisonIndex]->GetWideName();
	}

	std::string FName::GetName() const
	{
		return GetNameA();
	}

	class UScriptViewportClient : public UObject
	{
	public:
		unsigned char                                              padding1[0x10];                                  // 0x0028(0x0010) MISSED OFFSET (PADDING)
	};

	class UGameViewportClient : public UScriptViewportClient
	{
	public:
		unsigned char                                              UnknownData_9DM8[0x8];                                   // 0x0038(0x0008) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UConsole* ViewportConsole;                                         // 0x0040(0x0008) ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		TArray<struct FDebugDisplayProperty>                       DebugProperties;                                         // 0x0048(0x0010) ZeroConstructor, NativeAccessSpecifierPublic
		unsigned char                                              UnknownData_KXXF[0x10];                                  // 0x0058(0x0010) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		int32_t                                                    MaxSplitscreenPlayers;                                   // 0x0068(0x0004) ZeroConstructor, Config, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic
		unsigned char                                              UnknownData_YPWH[0xC];                                   // 0x006C(0x000C) MISSED OFFSET (FIX SPACE BETWEEN PREVIOUS PROPERTY)
		class UWorld* World;                                                   // 0x0078(0x0008) ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		class UGameInstance* GameInstance;                                            // 0x0080(0x0008) ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected
		unsigned char                                              UnknownData_KJQL[0x2D8];                                 // 0x0088(0x02D8) MISSED OFFSET (PADDING)
	};

	//Essential Variables
	DWORD_PTR BaseAddress;
	HMODULE Handle;
	MODULEINFO info;
	ULevel* m_PersistentLevel;
	//SDK::ULocalPlayer* m_LocalPlayer;
	uintptr_t g_GNames;

	//Additional
	float centerX = 960.0;
	float centerY = 540.0;

	//Initializing variables
	FNamePool* FName::GNames = nullptr;
	TUObjectArray* UObject::GObjects = nullptr;    
	UWorld** m_UWorld = nullptr;
}