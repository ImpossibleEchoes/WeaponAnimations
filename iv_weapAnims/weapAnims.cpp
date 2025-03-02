// Yulian Shvab 2025
// Released into the public domain; no copyright claimed
#include "weapAnims.h"

#include <Windows.h>
#include <stdio.h>

#include "hookFns.h"

#define WEAPON_FIRE_HASH 0xDF8E89EB
#define WEAPON_RELOAD_HASH 0xE84461E7
#define WEAPON_AMMO_HASH 0xE6FC1366

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
	DWORD m_dwLayerIndex;
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
};

struct CAnimBlender {

	__forceinline CAnimPlayer* blendAnim(rage::crAnimation* pAnim, int flags, int layerIndex, float _unk, int _unk2, int _unk3, const char* pszWadName, const char* pszAnimName, int wadId, int animHash) {
		return ((CAnimPlayer * (__thiscall*)(CAnimBlender*, rage::crAnimation*, int, int, float, int, int, const char*, const char*, int, int))(g_blendAnim_addr))
			(this, pAnim, flags, layerIndex, _unk, _unk2, _unk3, pszWadName, pszAnimName, wadId, animHash);
	}

	__forceinline CAnimPlayer* findAnimInBlend(rage::crAnimation* pAnim) {
		return ((CAnimPlayer * (__thiscall*)(CAnimBlender*, rage::crAnimation*))(g_CAnimBlender__findAnimInBlend_addr))(this, pAnim);
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
struct CAnimDescriptor {
	struct NameHashPair {
		const char* pszName;
		int dwHash;
	};

	char m_szAnimGroupName[24];
	int _f18;
	int _f1c;
	char m_szAnimFileName[24];
	DWORD m_dwdwAnimCount;
	const char** ppszNames;
	NameHashPair* pHashNamePairs;
	DWORD* m_pAnimData;
	DWORD m_dwAnimationId;
	BOOL m_bIsResident;
	DWORD _f50;
	int _f54;
};

struct CPed {
	BYTE __0[0x29b];
	DWORD m_dwFlags;
	BYTE __2a0[0x10];

	// CPedWeaponMgr start here
	// in CE the structure fields are different
#ifdef GTA_IV_PRE_CE
	BYTE __2b0[0x2c];
#else
	BYTE __2b0[0x14];
#endif // GTA_IV_PRE_CE

	CObject* m_pWeapObject;

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
CAnimDescriptor* getAnimDescriptor(size_t modelIndex) {
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

void processFire(CPed* ped) {
	if (ped->m_pWeapObject && (ped->m_dwFlags & 0x8000000) != 0)
		ped->m_pWeapObject->playAnim(getAnimId(ped->m_pWeapObject->m_wModelIndex), WEAPON_FIRE_HASH, 1000.f, false, false, 1, 1.f);
}

void processReload(CPed* ped) {
	if (ped->m_pWeapObject)
		ped->m_pWeapObject->playAnim(getAnimId(ped->m_pWeapObject->m_wModelIndex), WEAPON_RELOAD_HASH, 1000.f, false, false, 0, 1.f);
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

void processReloadAbort(CPed* ped) {
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

	}
}
