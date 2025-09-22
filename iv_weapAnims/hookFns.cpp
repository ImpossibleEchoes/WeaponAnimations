// Yulian Shvab 2025
// Released into the public domain; no copyright claimed
#include "hookFns.h"

CBaseModelInfo** g_ppMdlInfos = NULL;
CWeaponInfo* g_pWeapInfo = NULL;
CAnimAssociations__AnimData** g_pAnimAssociations = NULL; // ms_aAnimAssociations
size_t g_getAnimByIdAndHash_addr;
size_t g_checkCoords_addr;
size_t g_allocCoords_addr;
size_t g_allocAnimBlender_addr;
size_t g_allocNode_addr;
size_t g_blendAnim_addr;
size_t g_CAnimBlender__findAnimInBlend_addr;

size_t g_CPedWeapons__getWeaponData;
size_t g_CPBuffer__get;
size_t g_CPBuffer__set;
size_t g_CAnimPlayer__getAnimEventTime;
size_t g_getWeaponByType;
size_t g_CPed__getPad2;
size_t g_CVehicle__isDriver;
size_t g_CIkManager__setHandPos;
size_t g_CDynamicEntity__getSkeletonData;
size_t g_CDynamicEntity__getBoneMatrixInWorldSpace;
size_t g_CAnimBlender__getPlayerByAnimId;

DWORD g_dwOldProtect;

bool setProtect(size_t addr, size_t size, DWORD newProtect) { 
	size = (size + 0xfff) & ~0xfff;
	return VirtualProtect((void*)addr, size, newProtect, &g_dwOldProtect);
}

size_t setFnAddrInCallOpcode(size_t callPos, size_t pfn) {

	BYTE* patch = (BYTE*)callPos + 1;

	auto retVal = (*(DWORD*)patch + (callPos + 5));
	setProtect(callPos, 5, PAGE_EXECUTE_READWRITE);
	*(DWORD*)patch = (pfn - (callPos + 5)); // для x64 использовать батут при необходимости
	setProtect(callPos, 5, g_dwOldProtect);

	return retVal;
}

size_t getFnAddrInCallOpcode(size_t callPos) {
	return (*(DWORD*)((BYTE*)callPos + 1) + (callPos + 5));
}

DWORD writeDWORD(size_t addr, DWORD val) {
	setProtect(addr, sizeof val, PAGE_EXECUTE_READWRITE);

	DWORD oldVal = *(DWORD*)addr;
	*(DWORD*)addr = val;

	setProtect(addr, sizeof val, g_dwOldProtect);

	return oldVal;
}

void makeNop(size_t addr, size_t size) {

	//DWORD oldVal = *(DWORD*)addr;
	//*(DWORD*)addr = val;

	//return oldVal;

	setProtect(addr, size, PAGE_EXECUTE_READWRITE);

	memset((void*)addr, 0x90, size);

	setProtect(addr, size, g_dwOldProtect);

}