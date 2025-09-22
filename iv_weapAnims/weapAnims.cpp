// Yulian Shvab 2025
// Released into the public domain; no copyright claimed
#include "weapAnims.h"

#include <Windows.h>
#include <stdio.h>

#include "hookFns.h"

#define WEAPON_FIRE_HASH 0xDF8E89EB
#define WEAPON_RELOAD_HASH 0xE84461E7
#define WEAPON_AMMO_HASH 0xE6FC1366
#define WEAPON_EMPTY_RELOAD_HASH 0x905509FA

#define RELOAD_PRIORITY 0

// forward declarations 
class fvfDrawData;
class CPtrNodeDouble;
struct fragInst_obj1;
struct phInst;
namespace rage {
class crmtObserver;
class crAnimation;
}

// classes
struct CAnimPlayer {
	size_t** __vmt;
	DWORD m_dwFlags;
	DWORD m_dwPriority; // original name: m_Priority
	DWORD _fc;
	DWORD m_nAnimAssociationId;
	DWORD m_nAnimId;
	DWORD m_dwAnimHash;
	DWORD _f1c;
	void(__cdecl* m_pfnCallback)(CAnimPlayer*, int);
	void* m_pArgForCallback;
	int m_taskPoolHandle;
	float _f2c;
	float _f30;
	float _f34;
	DWORD _f38;
	float m_fBlendOutDelta;
	rage::crAnimation* m_pAnimation;
	WORD m_wActual; // 1 or 0
	WORD m_dwFlags2;
	DWORD _f48;
	float m_fAnimCurrentTime;
	float m_fAnimCurrentTimeOld;
	float m_fSpeed;
	float m_fBlendAmount;
	float _f5c; // old?
	float _f60;
	DWORD _f64;
	float _f68;
	DWORD _f6c;
	float _f70;
	DWORD _f74;
	float _f78;
	float _f7c;
	float _f80;
	rage::crmtObserver* _f84;

	char getAnimEventTime(int mask, float* time, float start, float end) {
		return ((char(__thiscall*)(CAnimPlayer * pThis, int mask, float* time, float start, float end))(g_CAnimPlayer__getAnimEventTime))(this, mask, time, start, end);
	}

};

struct CAnimBlender_obj3 {
	int _f0;
	CAnimPlayer m_animPlayer;
	CAnimBlender_obj3* m_pNext;
	CAnimBlender_obj3* m_pPrev;
};

struct CAnimBlender {

	__forceinline CAnimPlayer* blendAnim(rage::crAnimation* pAnim, int flags, int layerIndex, float _unk, int _unk2, int _unk3, const char* pszWadName, const char* pszAnimName, int wadId, int animHash) {
		return ((CAnimPlayer * (__thiscall*)(CAnimBlender*, rage::crAnimation*, int, int, float, int, int, const char*, const char*, int, int))(g_blendAnim_addr))
			(this, pAnim, flags, layerIndex, _unk, _unk2, _unk3, pszWadName, pszAnimName, wadId, animHash);
	}

	__forceinline CAnimPlayer* findAnimInBlend(rage::crAnimation* pAnim) {
		return ((CAnimPlayer * (__thiscall*)(CAnimBlender*, rage::crAnimation*))(g_CAnimBlender__findAnimInBlend_addr))(this, pAnim);
	}


	inline void setAnimPlayerTimeByLayer(int layerIndex, float val) {
		auto pObj = *(CAnimBlender_obj3**)((BYTE*)this + 0x1A28);

		if (pObj) {
			while (true) {
				if (pObj->m_animPlayer.m_wActual != 1)
					continue;
				if (pObj->m_animPlayer.m_dwPriority == layerIndex) {
					pObj->m_animPlayer.m_fAnimCurrentTime = val;
					pObj->m_animPlayer.m_fAnimCurrentTimeOld = val;
					//break;
				}
				pObj = pObj->m_pNext;

				if (!pObj)
					break;
			}
		}
	}

};

__forceinline rage::crAnimation* getAnimByIdAndHash(int id, DWORD hash) { return ((rage::crAnimation * (__cdecl*)(int, DWORD))(g_getAnimByIdAndHash_addr))(id, hash); }

struct CEntity {
	BYTE __0[0x24];
	DWORD m_dwFlags;
	BYTE __28[0x6];
	WORD m_wModelIndex;
	BYTE __30[0x4];
	fvfDrawData* m_pDrawHandler;
	phInst* m_pPhysics;
	BYTE __3c[0x34];

	__forceinline phInst* getPhysicsInstance() { return ((phInst * (__thiscall*)(CEntity*))(*(void***)this)[160 / sizeof size_t])(this); }
};

struct CDynamicEntity : CEntity {
	BYTE __70[0x4];
	CPtrNodeDouble* m_pNode;
	CAnimBlender* m_pAnimBlender;
	BYTE __7c[0x84];
	fragInst_obj1* _f100;

	__forceinline fragInst_obj1* checkCoords() { return ((fragInst_obj1*(__thiscall*)(CEntity*))(g_checkCoords_addr))(this); }
	__forceinline int allocCoords() { return ((int(__thiscall*)(CEntity*))(g_allocCoords_addr))(this); }
	__forceinline void* allocAnimBlender(void* _b) { return ((void* (__thiscall*)(CDynamicEntity*, void*))(g_allocAnimBlender_addr))(this, _b); }
	__forceinline void allocNode() { ((void(__thiscall*)(CDynamicEntity*))(g_allocNode_addr))(this); }

	/**
 * @brief Plays an animation on the object. Works on any dynamic entity. But do not use on cars without additional patching.
 * This will lead to the game not processing the wheel matrix while there is at least one animation in the blend.
 *
 * @param animId The ID of the #ad container.
 * @param hash The Jenkins hash code of the animation.
 * @param _unk An unknown float value.
 * @param bLoop A flag indicating whether to loop the animation.
 * @param bDontClearAnimLayerAfterPlaying A flag indicating whether to clear the animation layer after the animation finishes.
 * @param layerIndex The index of the animation layer.
 * @param speed The animation playback speed.
 *
 */
	void playAnim(int animId, DWORD hash, float _unk, char bLoop, char bDontClearAnimLayerAfterPlaying, int layerIndex, float speed) {

		rage::crAnimation* pAnim = NULL;

		if (auto pDrawHandler = m_pDrawHandler) // pDrawHandler = m_pDrawHandler
			if (auto pDrawable = *(BYTE**)pDrawHandler) // pDrawable = pDrawHandler->m_pDrawable
				if (auto pSkelData = *(DWORD**)((BYTE**)pDrawable + 0xc)) // pSkelData = pDrawable->m_pSkeleton
					if (pAnim = getAnimByIdAndHash(animId, hash))
						; // ok
					else return;
				else return;
			else return;
		else return;

		int flags = 0;
		if (bLoop)
			flags = 0x20;
		if (!bDontClearAnimLayerAfterPlaying)
			flags |= 0x8000;

		if (!checkCoords())
			allocCoords();

		if (!m_pAnimBlender)
			allocAnimBlender(NULL);

		if (!m_pNode)
			allocNode();


		CAnimPlayer* pAnimPlayer = m_pAnimBlender->blendAnim(pAnim, flags, layerIndex, _unk, -1, -1, NULL, NULL, animId, hash);

		if (pAnimPlayer)
			pAnimPlayer->m_fSpeed = speed;

	}
};

struct CPhysical : CDynamicEntity { };

struct CObject : CPhysical { };

struct CBaseModelInfo {
	BYTE __0[0x60];
};

struct CWeaponModelInfo : CBaseModelInfo {
	DWORD m_dwWeaponInfoId;
	DWORD m_aWeaponBoneIndices[6];
	DWORD _f7c;
};

struct CWeaponInfo {
	DWORD m_dwWeaponInfoId;
	int m_nSlot;
	DWORD m_dwFireType;
	DWORD m_dwDamageType;
	DWORD m_dwGroup;
	float m_fTargetRange;
	float m_fWeaponRange;
	DWORD m_statType;
	DWORD m_dwFlags;
	DWORD m_nWeaponModelHash;
	DWORD m_dwAnimGroup;
	float m_fFirerate;
	float m_fBlindfirerate;
	float m_fAccuracy;
	float m_fAccuracyfps;
	DWORD field_3C;
	float _f40[4]; // rage::Vector3
	float _f50[4]; // rage::Vector3
	float m_fPitch;
	float m_fStanding;
	float m_fDucked;
	float m_fScale;
	DWORD m_dwRumbleDuration;
	float m_fRumbleIntensity;
	DWORD m_dwRegenTime;
	BYTE __7c[0x4];
	WORD m_wAMMoonStreet;
	WORD m_wBase;
	WORD m_wFps;
	WORD m_wClipSize;
	WORD m_wAmmoMax;
	DWORD m_dwTimeBetweenShots;
	float m_fForce;
	DWORD m_dwTime;
	DWORD m_dwFastTime;
	DWORD m_dwCrouchTime;
	DWORD _fa0;
	DWORD _fa4;
	DWORD _fa8;
	DWORD _fac;
	float _fb0;
	BYTE __b4[0xc];
	float _fc0[4]; // rage::Vector3
	float _fd0;
	float _fd4;
	float _fd8;
	DWORD m_dwAnimMeleeGroup1;
	DWORD m_dwAnimMeleeGroup2;
	DWORD m_dwMuzzleFxHash;
	DWORD m_dwShellFxHash;
	DWORD m_dwTrailFxHash;
	float m_fNetworkPlayerMod;
	float m_fNetworkPedMod;
	float m_fAccuracyTime;
	int m_dwPellets;
	BYTE __100[0xc];
	DWORD _f10c;
};

// CAnimAssociations::CAnimDescriptor
struct CAnimAssociations__AnimDescriptor;
struct CAnimAssociations__NameHashPair;

struct CAnimAssociations__AnimData {

	char m_szAnimGroupName[24];
	int _f18;
	int _f1c;
	char m_szAnimFileName[24];
	DWORD m_dwdwAnimCount;
	const char** ppszNames;
	CAnimAssociations__NameHashPair* pHashNamePairs;
	CAnimAssociations__AnimDescriptor* pAnimDescriptor;
	DWORD m_dwAnimationId;
	BOOL m_bIsResident;
	DWORD _f50;
	int _f54;
};

struct CWeaponData;

struct CPedWeaponMgr {
	__forceinline CWeaponData* getWeaponData() {
		return ((CWeaponData * (__thiscall*)(CPedWeaponMgr*))(g_CPedWeapons__getWeaponData))(this);
	}
};

// class rage::ioValue
struct ioValue {
	struct __declspec(align(4)) History {
		BYTE m_nbValue;
		void* _f4; // unk ptr
	};

	BYTE _f4;
	BYTE _f5;
	BYTE m_nbValue;
	BYTE m_nbLastValue;
	BYTE m_nbHistoryIndex;
	BYTE _f9;
	BYTE _fa;
	BYTE _fb;
	History* m_pHistory; // => num = 0x40

	__forceinline bool isPressed() {
		return (_f4 ^ m_nbValue) > 0x3F;
	}

	__forceinline bool isJustPressed() {
		return m_nbLastValue <= 0x3F && isPressed();
	}

	__forceinline virtual ~ioValue() { } // только один виртуальный метод
};

struct CPad {
	BYTE __0[0x2698];
	ioValue m_aValues[187];
};

struct CVehicle {

	__forceinline bool isDriver(CPed* pPed) { return ((bool(__thiscall*)(CVehicle*, CPed*))(g_CVehicle__isDriver))(this, pPed); }
};

struct CIkManager {

};

struct CPed {
#ifdef GTA_IV_PRE_CE
	BYTE __0[0x29B];
	DWORD m_dwFlags; // ped flags 5
	DWORD m_dwFlags2; // ped flags 6
	BYTE __2A4[0xC];
	//BYTE __2a0[0x10];

	// CPedWeaponMgr start here
	// in CE the structure fields are different
	BYTE __2B0[0x2c];

	CObject* m_pWeapObject;
#else
	BYTE __0[0x29B];
	DWORD m_dwFlags; // ped flags 5
	DWORD m_dwFlags2; // ped flags 6
	BYTE __2A4[0xC];

	// CPedWeaponMgr start here
	DWORD m_curWeapSlot;
	BYTE __2B4[0x10];

	CObject* m_pWeapObject;

	BYTE __2C8[0x868];
	CVehicle* m_pVehRef; // +B30
	BYTE __B34[0x7C];
	CIkManager m_ikMgr;
	
#endif // GTA_IV_PRE_CE



	bool isAttackPressed() {
		CPad* pad = ((CPad * (__thiscall*)(CPed * pThis))(g_CPed__getPad2))(this);
		if (!pad)
			return false;
		return pad->m_aValues[4].isJustPressed();
	}

};

#ifdef GTA_IV_PRE_CE

#else
static_assert((offsetof(CPed, m_pWeapObject) == 0x2C4), "0");
static_assert((offsetof(CPed, m_pVehRef) == 0xB30), "0");
static_assert((offsetof(CPed, m_ikMgr) == 0xBB0), "0");
#endif

enum eReloadingFlags {
	RELOADING_FLAGS_SKIP_FINAL_RELOAD = 1,
	RELOADING_FLAGS_IS_SHOTGUN = 1 << 1,
	//RELOADING_FLAGS_CANCEL_SHOTGUN_RELOAD = 1 << 2,
	RELOADING_FLAGS_IS_LAST_RELOAD_CYCLE = 1 << 3,
};

struct CTaskSimpleReloadGun {
	void* m_pParent;
	void* m_pSubTask;
	DWORD m_dwTaskFlags;
	DWORD _f10;

	int _f14;
	BYTE __18[24];
	int _f30;
	int _f34;
	int _f38;
	int _f3C;
	float _f40;
	float _f44;
	CAnimPlayer* m_pPedReloadAnimPlayer;
	int _f4C;
	char _f50;
	int m_state;
	int m_stateOld;
	int _f5C;
	int _f60;
	int _f64;

	DWORD m_ammoInClip;
	DWORD m_numAmmo;
	BYTE m_flags;

	static size_t ms_process_origcall;

	__forceinline int setState(int newState) {
		_f50 = 1;
		m_stateOld = m_state;
		m_state = newState;
		return m_stateOld;
	}


	virtual ~CTaskSimpleReloadGun() { }

	virtual CTaskSimpleReloadGun* clone() { return nullptr; }
	virtual bool m8() { return true; }
	virtual int getTaskType() { return -1; } // name from unk rage game with gta style structs
	virtual void m10() { }
	virtual bool makeAbortable(void* pPed, BOOL bUseSmoothBlend, void* pEvent) { return false; }

};


__forceinline int CPBuffer__get(void* buff);
__forceinline int CPBuffer__set(void* buff, int val);

struct CWeaponData {
	char __0[20];
	int _f14;
	int m_dwInfoIndex;
	int _f1C;
	int _f20;
	char _f24;
	int _f28;
	int _f2C;
	int _f30;
	int _f34;
	int _f38;
	int _f3C;
	int _f40;
	int _f44;
	int _f48;
	int _f4C;
	char _f50;
	int _f54;
	char _f58;
#ifdef GTA_IV_PRE_CE
	void* m_secbuffArray1;
	void* m_secbuffArray2;

	__forceinline int getNumAmmoInClip() { return CPBuffer__get(&m_secbuffArray1); }
	__forceinline void setNumAmmoInClip(int num) { CPBuffer__set(&m_secbuffArray1, num); }
	__forceinline int getTotalNumAmmo() { return CPBuffer__get(&m_secbuffArray2); }
	__forceinline void setTotalNumAmmo(int num) { CPBuffer__set(&m_secbuffArray2, num); }

#else
	int m_nNumAmmoInClip;
	int m_nTotalNumAmmo;

	__forceinline int getNumAmmoInClip() { return m_nNumAmmoInClip; }
	__forceinline void setNumAmmoInClip(int num) { m_nNumAmmoInClip = num; }
	__forceinline int getTotalNumAmmo() { return m_nTotalNumAmmo; }
	__forceinline void setTotalNumAmmo(int num) { m_nTotalNumAmmo = num; }
#endif



};


/**
 * @brief Retrieves the animation descriptor for a given model index.
 *
 * This function returns a pointer to the animation descriptor associated with the specified model index.
 * The animation descriptor contains information about the available animations for the model.
 *
 * @param modelIndex The index of the model for which to retrieve the animation descriptor.
 * @return A pointer to the animation descriptor.
 */
CAnimAssociations__AnimData* getAnimDescriptor(size_t modelIndex) {
	CWeaponModelInfo* pInfo = (CWeaponModelInfo*)g_ppMdlInfos[modelIndex];
	CWeaponInfo* pWeapInfo = g_pWeapInfo + pInfo->m_dwWeaponInfoId;
	return *g_pAnimAssociations + pWeapInfo->m_dwAnimGroup;
}

/**
 * @brief Retrieves the animation ID of the #ad container.
 * for a specific model, identified by its index.
 * @param modelIndex The index of the model for which to retrieve the animation ID.
 * @return The animation ID of the #ad container for the specified model.
 */
DWORD getAnimId(size_t modelIndex) { return getAnimDescriptor(modelIndex)->m_dwAnimationId; }

void processReload(CTaskSimpleReloadGun* pTask, CPed* ped) {
	if (ped->m_pWeapObject) {
		if(pTask->m_ammoInClip > 0 || !getAnimByIdAndHash(getAnimId(ped->m_pWeapObject->m_wModelIndex), WEAPON_EMPTY_RELOAD_HASH))
			ped->m_pWeapObject->playAnim(getAnimId(ped->m_pWeapObject->m_wModelIndex), WEAPON_RELOAD_HASH, 1000.f, false, false, 0, 1.f);
		else
			ped->m_pWeapObject->playAnim(getAnimId(ped->m_pWeapObject->m_wModelIndex), WEAPON_EMPTY_RELOAD_HASH, 1000.f, false, false, 0, 1.f);
	}
}

void processFireAbort(CPed* ped) {
	if (ped && ped->m_pWeapObject && ped->m_pWeapObject->m_pAnimBlender) {

		auto animId = getAnimId(ped->m_pWeapObject->m_wModelIndex);

		rage::crAnimation* pAnimation = getAnimByIdAndHash(animId, WEAPON_FIRE_HASH);

		if (pAnimation) {
			CAnimPlayer* pAnimPlayer = ped->m_pWeapObject->m_pAnimBlender->findAnimInBlend(pAnimation);

			if (pAnimPlayer) {
				pAnimPlayer->m_fAnimCurrentTime = 1.f;
				pAnimPlayer->m_fAnimCurrentTimeOld = 1.f;
			}
		}
	}
}

void processReloadAbort(CTaskSimpleReloadGun* pTask, CPed* ped) {
	if (ped && ped->m_pWeapObject && ped->m_pWeapObject->m_pAnimBlender) {
		auto animId = getAnimId(ped->m_pWeapObject->m_wModelIndex);

		rage::crAnimation* pAnimation = getAnimByIdAndHash(animId, WEAPON_RELOAD_HASH);

		if (pAnimation) {
			CAnimPlayer* pAnimPlayer = ped->m_pWeapObject->m_pAnimBlender->findAnimInBlend(pAnimation);

			if (pAnimPlayer) {
				pAnimPlayer->m_fAnimCurrentTime = 1.f;
				pAnimPlayer->m_fAnimCurrentTimeOld = 1.f;
			}
		}

		auto pPedWeap = (CPedWeaponMgr*)((BYTE*)ped + 0x2B0);

		auto pWeapData = pPedWeap->getWeaponData();
		if (pWeapData)
			pWeapData->_f1C = 0;
	}

}

__forceinline int CPBuffer__get(void* buff) {
	return ((int(__thiscall*)(void*))(g_CPBuffer__get))(buff);
}
__forceinline int CPBuffer__set(void* buff, int val) {
	return ((int(__thiscall*)(void*, int))(g_CPBuffer__set))(buff, val);
}

__forceinline CWeaponInfo* getWeaponByType(int index) {
	//return ((CWeaponInfo * (__cdecl*)(int))(g_getWeaponByType))(index);

	return g_pWeapInfo + index;
}

void processReloadZeroState(CTaskSimpleReloadGun* pTask, CPed* pPed) {
	if (pTask->m_pPedReloadAnimPlayer) {
		float fWhere = 0;
		if (pTask->m_pPedReloadAnimPlayer->getAnimEventTime(1 << 31, &fWhere, 0.f, 1.f) && pTask->m_pPedReloadAnimPlayer->getAnimEventTime(1 << 30, &fWhere, 0.f, 1.f)) {
			pTask->m_flags |= RELOADING_FLAGS_IS_SHOTGUN;
		}
	}
}

void preProcessShotgunReload(CTaskSimpleReloadGun* pTask, CPed* pPed) {
	if (pTask->m_state == 0 && pTask->m_stateOld == 0) {
		auto pPedWeap = (CPedWeaponMgr*)((BYTE*)pPed + 0x2B0);

		auto pWeapData = pPedWeap->getWeaponData();

		auto ammoInClip = pWeapData->getNumAmmoInClip();
		auto numAmmo = pWeapData->getTotalNumAmmo();

		pTask->m_ammoInClip = ammoInClip;
		pTask->m_numAmmo = numAmmo;

		pTask->m_flags = 0;

		//pTask->m_bSkipFinalReload = ammoInClip > 0;
		if (ammoInClip > 0)
			pTask->m_flags |= RELOADING_FLAGS_SKIP_FINAL_RELOAD;
		
	}
	else {
		if (pTask->m_pPedReloadAnimPlayer) {
			float fWhere = 0;
			if (pTask->m_pPedReloadAnimPlayer->getAnimEventTime(1 << 31, &fWhere, 0.f, 1.f) && pTask->m_pPedReloadAnimPlayer->getAnimEventTime(1 << 30, &fWhere, 0.f, 1.f) ) {

				auto ammoInClip = pTask->m_ammoInClip;
				auto numAmmo = pTask->m_numAmmo;

				auto pPedWeap = (CPedWeaponMgr*)((BYTE*)pPed + 0x2B0);

				auto pWeapData = pPedWeap->getWeaponData();

				auto currAmmoInClip = pWeapData->getNumAmmoInClip();
				auto currNumAmmo = pWeapData->getTotalNumAmmo();

				if (currAmmoInClip != ammoInClip || currNumAmmo != numAmmo) {
					pWeapData->setNumAmmoInClip(ammoInClip);
					pWeapData->setTotalNumAmmo(numAmmo);
				}
			}
		}
	}

	if (pPed && pTask->m_flags & RELOADING_FLAGS_IS_SHOTGUN && !(pTask->m_flags & RELOADING_FLAGS_IS_LAST_RELOAD_CYCLE)) {
		if (pPed->isAttackPressed())
			pTask->m_flags |= RELOADING_FLAGS_IS_LAST_RELOAD_CYCLE;
	}
}

template <DWORD _When, DWORD _Where> __forceinline void skipAnim(CTaskSimpleReloadGun* pTask, CPed* pPed) {
	float val;
	float fWhere;
	auto pPedWeap = (CPedWeaponMgr*)((BYTE*)pPed + 0x2B0);

	auto pWeapData = pPedWeap->getWeaponData();

	auto weapInfo = getWeaponByType(pWeapData->m_dwInfoIndex);
	WORD clipSize = weapInfo->m_wClipSize;

	auto ammoInClip = pWeapData->getNumAmmoInClip();
	auto numAmmo = pWeapData->getTotalNumAmmo();


	auto pWeapObj = pPed->m_pWeapObject;
	auto pAnimBlender = pWeapObj->m_pAnimBlender;

	if (pTask->m_pPedReloadAnimPlayer->getAnimEventTime(_When, &fWhere, 0.f, 1.f)) {
		if (pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTime >= fWhere && pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTimeOld < fWhere) {
			if (pTask->m_pPedReloadAnimPlayer->getAnimEventTime(_Where, &fWhere, 0.f, 1.f)) {
				val = fWhere;

				pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTimeOld = val;
				pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTime = val;

				if (pAnimBlender)
					pAnimBlender->setAnimPlayerTimeByLayer(RELOAD_PRIORITY, val);
			}
		}
	}
}

void postProcessShotgunReload(CTaskSimpleReloadGun* pTask, CPed* pPed) {
	if (pTask->m_pPedReloadAnimPlayer) {

		float fWhere = 0;

		if (pTask->m_pPedReloadAnimPlayer->getAnimEventTime(1 << 24, &fWhere, 0.f, 1.f)) {
			float where2 = fWhere;
			if (pTask->m_pPedReloadAnimPlayer->getAnimEventTime(1 << 23, &fWhere, 0.f, 1.f)) {
				if (pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTime >= where2 && pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTime < fWhere)
					pPed->m_dwFlags2 |= 2;
			}

		}

		if (pTask->m_flags & RELOADING_FLAGS_IS_SHOTGUN) {

			pTask->m_pPedReloadAnimPlayer->getAnimEventTime(1 << 31, &fWhere, 0.f, 1.f);
			float where2 = fWhere;
			pTask->m_pPedReloadAnimPlayer->getAnimEventTime(1 << 30, &fWhere, 0.f, 1.f);

			if (pTask->m_ammoInClip == 1 && pTask && pTask->m_flags & RELOADING_FLAGS_IS_LAST_RELOAD_CYCLE && !(pTask->m_flags & RELOADING_FLAGS_SKIP_FINAL_RELOAD) &&
				pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTime >= fWhere && pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTime < where2) {

				float val = where2;


				pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTimeOld = val;
				pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTime = val;

				auto pWeapObj = pPed->m_pWeapObject;
				auto pAnimBlender = pWeapObj->m_pAnimBlender;

				if (pAnimBlender)
					pAnimBlender->setAnimPlayerTimeByLayer(RELOAD_PRIORITY, val);

				auto pPedWeap = (CPedWeaponMgr*)((BYTE*)pPed + 0x2B0);

				auto pWeapData = pPedWeap->getWeaponData();
				pWeapData->_f1C = 0;
			}
			else if (pTask->m_numAmmo == pTask->m_ammoInClip) {

				if (pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTime >= fWhere && pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTimeOld < fWhere) {
					float val = where2;

					pTask->_f64 = 0;
					//pWeapData->_f1C = 2;

					pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTimeOld = val;
					pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTime = val;

					auto pWeapObj = pPed->m_pWeapObject;
					auto pAnimBlender = pWeapObj->m_pAnimBlender;

					if (pAnimBlender)
						pAnimBlender->setAnimPlayerTimeByLayer(RELOAD_PRIORITY, val);

				}
			}
			else if (pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTime >= where2 && pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTimeOld < where2) {


				bool bOk = pTask->m_pPedReloadAnimPlayer->getAnimEventTime(1 << 30, & fWhere, 0.f, 1.f);

				float val = fWhere;

				auto pPedWeap = (CPedWeaponMgr*)((BYTE*)pPed + 0x2B0);

				auto pWeapData = pPedWeap->getWeaponData();

				auto weapInfo = getWeaponByType(pWeapData->m_dwInfoIndex);
				WORD clipSize = weapInfo->m_wClipSize;

				auto ammoInClip = pTask->m_ammoInClip;
				auto numAmmo = pTask->m_numAmmo;
				//auto ammoInClip = pWeapData->getNumAmmoInClip();
				//auto numAmmo = pWeapData->getTotalNumAmmo();


				auto pWeapObj = pPed->m_pWeapObject;
				auto pAnimBlender = pWeapObj->m_pAnimBlender;


				if (numAmmo != ammoInClip && ammoInClip < clipSize) {

					ammoInClip++;

					pWeapData->setNumAmmoInClip(ammoInClip);
					pWeapData->setTotalNumAmmo(numAmmo);

					{
						ammoInClip = pWeapData->getNumAmmoInClip();
						numAmmo = pWeapData->getTotalNumAmmo();
						pTask->m_ammoInClip = ammoInClip;
						pTask->m_numAmmo = numAmmo;
					}

					if (ammoInClip < clipSize) {

						if ((pTask->m_flags & RELOADING_FLAGS_IS_LAST_RELOAD_CYCLE) == 0) {

							pTask->_f64 = 0;
							//pWeapData->_f1C = 2;

							pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTimeOld = val;
							pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTime = val;


							if (pAnimBlender)
								pAnimBlender->setAnimPlayerTimeByLayer(RELOAD_PRIORITY, val);

							pTask->setState(2);
						}
						else {
							pWeapData->_f1C = 0;

						}
					}
				}
			}
		}

		if (pTask->m_flags & RELOADING_FLAGS_SKIP_FINAL_RELOAD) {
		//if (pTask->m_bSkipFinalReload) {
			skipAnim<1 << 29, 1 << 28>(pTask, pPed);
			skipAnim<1 << 27, 1 << 26>(pTask, pPed);
		}
		else {
			skipAnim<1 << 22, 1 << 21>(pTask, pPed);
		}

		if (pTask->m_pPedReloadAnimPlayer->getAnimEventTime(1 << 25, &fWhere, 0.f, 1.f)) {
			if (pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTime >= fWhere && pTask->m_pPedReloadAnimPlayer->m_fAnimCurrentTimeOld < fWhere) {

				float val = fWhere;

				auto pPedWeap = (CPedWeaponMgr*)((BYTE*)pPed + 0x2B0);

				auto pWeapData = pPedWeap->getWeaponData();

				auto weapInfo = getWeaponByType(pWeapData->m_dwInfoIndex);
				WORD clipSize = weapInfo->m_wClipSize;

				auto ammoInClip = pWeapData->getNumAmmoInClip();
				auto numAmmo = pWeapData->getTotalNumAmmo();


				auto pWeapObj = pPed->m_pWeapObject;


				if (numAmmo != ammoInClip && ammoInClip < clipSize) {


					ammoInClip++;

					pWeapData->setNumAmmoInClip(ammoInClip);
					pWeapData->setTotalNumAmmo(numAmmo);

					{
						ammoInClip = pWeapData->getNumAmmoInClip();
						numAmmo = pWeapData->getTotalNumAmmo();
						pTask->m_ammoInClip = ammoInClip;
						pTask->m_numAmmo = numAmmo;
					}
				}
			}
		}
	}
}

struct CTaskSimpleFireGun {
	//BYTE __0[0xF8];
	size_t __vmt;
	BYTE __4[0x64];

	CAnimPlayer* m_pPedAnimPlayer;

	//bool m_bFire;
};

void preProcessFire(CTaskSimpleFireGun* pTask, CPed* pPed) {

}

void postProcessFire(CTaskSimpleFireGun* pTask, CPed* pPed) {
	if (pPed->m_pWeapObject && /*!pTask->m_bFire && */pPed->m_dwFlags & 0x8000000)
		pPed->m_pWeapObject->playAnim(getAnimId(pPed->m_pWeapObject->m_wModelIndex), WEAPON_FIRE_HASH, 1000.f, false, false, 1, 1.f);


	if (pTask->m_pPedAnimPlayer) {
		float fWhere = 0;

		if (pTask->m_pPedAnimPlayer->getAnimEventTime(1 << 24, &fWhere, 0.f, 1.f)) {
			float where2 = fWhere;
			if (pTask->m_pPedAnimPlayer->getAnimEventTime(1 << 23, &fWhere, 0.f, 1.f)) {
				if (pTask->m_pPedAnimPlayer->m_fAnimCurrentTime >= where2 && pTask->m_pPedAnimPlayer->m_fAnimCurrentTime < fWhere)
					pPed->m_dwFlags2 |= 2;
			}

		}
	}

}

void postProcessFire2(CPed* pPed) {
	if (pPed->m_pWeapObject && /*!pTask->m_bFire && */pPed->m_dwFlags & 0x8000000)
		pPed->m_pWeapObject->playAnim(getAnimId(pPed->m_pWeapObject->m_wModelIndex), WEAPON_FIRE_HASH, 1000.f, false, false, 1, 1.f);

}

#ifndef GTA_IV_PRE_CE

struct alignas(0x10) Quaternion {
	float x, y, z, w;
};

struct alignas(0x10) Vector3 {
	float x, y, z, w;
};

struct Vec3 {
	float x, y, z;
};

struct alignas(0x10) Matrix34 {
	Vector3 a, b, c, d;
};

struct alignas(0x10) crBoneData {
	const char* pszName;
	DWORD __4;
	crBoneData* m_pNextSibling;
	crBoneData* m_pFirstChild;
	crBoneData* m_pParent;
	WORD m_wBoneIndex;
	__int16 m_wBoneId;
	__int16 m_wMirror;
	char m_nbTransFlags;
	char m_nbRotFlags;
	char m_nbScaleFlags;
	Vec3 m_vOffset;
	int m_dwHash;
	Vector3 m_vRotationEuler;
	Quaternion m_vRotationQuaternion;
	Vec3 m_vScale;
	int field_5C;
	Vector3 m_vParentModelOffset;
	Vector3 m_vOrient;
	Vector3 m_vSorient;
	Vector3 m_vTransMin;
	Vector3 m_vTransMax;
	Vector3 m_vRotMin;
	Vector3 m_vRotMax;
	int m_pJointData;
	int field_D4;
	int field_D8;
	int field_DC;
};

struct crSkeletonData {
	crBoneData* m_pBones;

	BYTE __4[0x10];
	WORD m_wNumBones;

};

enum eWeaponSlot {
	WEAPONSLOT_UNARMED,
	WEAPONSLOT_MELEE,
	WEAPONSLOT_HANDGUN,
	WEAPONSLOT_SHOTGUN,
	WEAPONSLOT_SMG,
	WEAPONSLOT_RIFLE,
	WEAPONSLOT_SNIPER,
	WEAPONSLOT_HEAVY,
	WEAPONSLOT_THROWN,
	WEAPONSLOT_SPECIAL,
	WEAPONSLOT_GIFT,
	WEAPONSLOT_PARACHUTE,
	WEAPONSLOT_DETONATOR,
};

const int g_animsToCheck[]{
	0x71,
0x79,
0x7A,
0xC5,
0xC6,
0xD1,
0xD4,
0xDA,
0xDB,
0xDC,
0xE0,
0xDE,
0xE9,
0xEA,
0xEB,
0xEC,
0xF9,
0xFA,
0x107,
0x108,
0x109,
0x10A,
0x10C,
0x10D,
0x10E,
0x10F,
0x110,
0x111,
0x112,
0x11C,
0x11D,
0x11E,
0x11F,
0x120,
0x121,
0x122,
0x123,
0x136,
};

typedef int AnimId;

void processWeapIk(CPed* pPed) {
	if (!pPed)
		return;
	if (pPed->m_curWeapSlot < WEAPONSLOT_HANDGUN || pPed->m_curWeapSlot > WEAPONSLOT_HEAVY)
		return;
	if (pPed->m_pVehRef && pPed->m_pVehRef->isDriver(pPed)) 
		return;
	if (!pPed->m_pWeapObject)
		return;

	auto pSkel = ((crSkeletonData * (__thiscall*)(CObject * _a))(g_CDynamicEntity__getSkeletonData))(pPed->m_pWeapObject);
	if (!pSkel)
		return;

	bool bOk = false;
	for (size_t i = 0; i < sizeof g_animsToCheck / sizeof * g_animsToCheck; i++) {
		if (((CAnimPlayer * (__thiscall*)(CAnimBlender*, AnimId, bool))(g_CAnimBlender__getPlayerByAnimId))(((CObject*)pPed)->m_pAnimBlender, g_animsToCheck[i], false)) {
			bOk = true;
			break;
		}
	}

	if (bOk) {

		for (size_t i = 0; i < pSkel->m_wNumBones; i++) {
			if (!strcmp(pSkel->m_pBones[i].pszName, "gun_grip2")) {


				((void(__thiscall*)(CObject * _a))(*(*(size_t**)pPed->m_pWeapObject + 0x118 / sizeof(size_t))))(pPed->m_pWeapObject);


				Matrix34 targetMtx{ 1.f,0.f ,0.f ,0.f ,
						0.f, 1.f,0.f ,0.f ,
						0.f ,0.f ,1.f ,0.f ,
						0.f ,0.f ,0.f ,0.f };


				((Matrix34 * (__thiscall*)(CObject * _a, int, Matrix34*))(g_CDynamicEntity__getBoneMatrixInWorldSpace))(pPed->m_pWeapObject, i, &targetMtx);


				Vector3 offset{ 0.f,0.f,0.f,0.f };

				auto ret = ((bool(__thiscall*)(CIkManager*, bool bIsLeft, Vector3 * pOffset, Matrix34 * pTargetMtx))(g_CIkManager__setHandPos))(&pPed->m_ikMgr, true, &offset, &targetMtx);

				break;
			}

		}
	}

}

#endif
