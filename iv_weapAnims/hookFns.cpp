// Yulian Shvab 2025
// Released into the public domain; no copyright claimed
#include "hookFns.h"

CBaseModelInfo** g_ppMdlInfos = NULL;
CWeaponInfo* g_pWeapInfo = NULL;
CAnimDescriptor** g_pAnimAssociations = NULL; // ms_aAnimAssociations
size_t g_getAnimByIdAndHash_addr;
size_t g_checkCoords_addr;
size_t g_allocCoords_addr;
size_t g_allocAnimBlender_addr;
size_t g_allocNode_addr;
size_t g_blendAnim_addr;
size_t g_CAnimBlender__findAnimInBlend_addr;


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

DWORD writeDWORD(size_t addr, DWORD val) {
	setProtect(addr, sizeof val, PAGE_EXECUTE_READWRITE);

	DWORD oldVal = *(DWORD*)addr;
	*(DWORD*)addr = val;

	setProtect(addr, sizeof val, g_dwOldProtect);

	return oldVal;
}