// Yulian Shvab 2025
// Released into the public domain; no copyright claimed
#include <Windows.h>

#include "Hooking.Patterns-master/Hooking.Patterns.h"
#include "hookFns.h"
#include "weapAnims.h"


/**
 * @namespace preCE_hooks_addr
 * @brief Contains addresses of functions and hook addresses for preCE game build.
 *
 * This namespace provides a central location for storing and memory addresses
 * that are essential for hooking and modifying the behavior of the game.
 * It includes addresses of target functions within the game's code
 *
 * @note This namespace is specific to the preCE build and should not be used with
 * other versions of the game.
 */
namespace preCE_hooks_addr{
size_t g_CPed__ProcessControl_hook_addr;
size_t g_gunFireAbortVmt_addr;
size_t g_gunReloadAbortVmt_addr;

size_t g_CTaskSimpleReloadGun__process_hook_addr;
size_t g_CTaskSimpleReloadGun__process_always_hook_addr;
size_t g_CTaskSimpleFireGun__process_hook_addr;

size_t g_CTaskSimpleNewGangDriveBy__process_vmt_addr;
size_t g_CTaskSimpleNewGangDriveBy__abort_vmt_addr;

size_t g_coverReloadFix_patch_addr;
size_t g_fireAbortFix_patch_addr;
}


/**
 * @namespace hooks_addr
 * @brief Contains addresses of functions and hook addresses for CE game build.
 *
 * This namespace provides a central location for storing and memory addresses
 * that are essential for hooking and modifying the behavior of the game.
 * It includes addresses of target functions within the game's code
 *
 * @note This namespace is specific to the CE build and should not be used with
 * other versions of the game.
 */
namespace hooks_addr {
size_t g_CPed__ProcessControl_hook_addr;

size_t g_gunFireAbortVmt_addr;
size_t g_gunReloadAbortVmt_addr;

size_t g_CTaskSimpleReloadGun__process_hook_addr;
size_t g_CTaskSimpleReloadGun__process_always_hook_addr;
size_t g_CTaskSimpleFireGun__process_hook_addr;

size_t g_coverReloadFix_patch_addr;
size_t g_fireAbortFix_patch_addr;
//size_t g_coverReloadFix_patch_addr;

size_t g_processIk_addr;
}

/**
 * @brief Initializes addresses for functions within the game's CE build executable.
 *
 * This function attempts to locate and initialize the addresses of specific functions
 * within the game's executable file. It relies on predefined patterns to identify
 * these functions in memory.
 *
 * @return int Returns an error code. 0 indicates successful initialization, while
 * any other value signifies an error during the process.
 */
int initAddrs() {
	int result = 0;
	//hook::pattern p("56 8B F1 8B 86 ? ? ? ? 83 B8 ? ? ? ? ? 0F 85 ? ? ? ? F6 86 ? ? ? ? ? 0F 85 ? ? ? ? 80 BE ? ? ? ? ? 0F 85 ? ? ? ? E8 ? ? ? ? "); //
	//if (!p.empty())
	//	g_CPed__ProcessControl_addr = (size_t)p.get_first();
	//else
	//	result |= 1;

	//p = hook::pattern("8B 04 B5 ? ? ? ? 66 C7 44 24 ? ? ? 85 C0 0F 84 ? ? ? ? 8B 80 ? ? ? ? 56 83 F8 02 74 45 83 C0 F0 83 F8 01 8D 44 24 10 50 77 1C E8 ? ? ? ? 85 C0 0F 84 ? ? ? ? 50 FF 74 24 18 8B CF 56 E8 ? ? ? ? ");
	//hook::pattern p("8B 04 B5 ? ? ? ? 66 C7 44 24 ? ? ? 85 C0 0F 84 ? ? ? ? 8B 80");
	hook::pattern p("8B 04 85 ? ? ? ? 8B 80 ? ? ? ? 8B 40 04 83 F8 FF 0F 84 ? ? ? ? 50 8B CF E8 ? ? ? ? F3 0F 10 40 ? F3 0F 11 44 24 ? F3 0F 10 40 ? F3 0F 11 44 24 ? F3 0F 10 40 ? ");
	if (!p.empty())
		g_ppMdlInfos = *(CBaseModelInfo***)p.get_first(3);
	else
		result |= 1 << 1;

	p = hook::pattern("05 ? ? ? ? C3 B8 ? ? ? ? C3 ");
	if (!p.empty())
		g_pWeapInfo = *(CWeaponInfo**)p.get_first(1);
	else
		result |= 1 << 2;
	//p = hook::pattern("8B F8 69 FF ? ? ? ? 81 C7 ? ? ? ? 89 07 8B 73 1C 85 F6 74 42 ");
	//if (!p.empty())
	//	g_pWeapInfo = *(tWeapInfo**)p.get_first(10);
	//else
	//	result |= 1 << 2;

	p = hook::pattern("A1 ? ? ? ? 03 C7 50 53 E8 ? ? ? ? 83 C4 08 85 C0 74 15 46 83 C7 58 3B 35 ? ? ? ? 7C DF 5F 5B 83 C8 FF 5E C2 04 00 ");
	if (!p.empty())
		g_pAnimAssociations = *(CAnimAssociations__AnimData***)p.get_first(1);
	else
		result |= 1 << 3;

	p = hook::pattern("FF 74 24 08 FF 74 24 08 E8 ? ? ? ? 83 C4 04 8B C8 E8 ? ? ? ? C3");
	if (!p.empty())
		g_getAnimByIdAndHash_addr = (size_t)p.get_first();
	else
		result |= 1 << 4;

	p = hook::pattern("56 8B F1 8B 06 FF 90 ? ? ? ? 85 C0 74 15 8B 06 8B CE FF 90 ? ? ? ? 8B 10 8B C8 5E FF A2 E0 00 00 00 ");
	if (!p.empty())
		g_checkCoords_addr = (size_t)p.get_first();
	else
		result |= 1 << 5;

	p = hook::pattern("56 8B F1 8B 06 FF 90 ? ? ? ? 85 C0 0F 85 ? ? ? ? 8B 46 34 85 C0 0F 84 ? ? ? ? 57 8B 38 85 FF 0F 84 ? ? ? ? 8B 7F 0C 85 FF 74 7E 53 0F B7 5F 14 6A 28 E8 ? ? ? ? 83 C4 04 85 C0 74 0C 6A 01 53 8B C8 E8");
	if (!p.empty())
		g_allocCoords_addr = (size_t)p.get_first();
	else
		result |= 1 << 6;

	p = hook::pattern("56 8B F1 8B 0D ? ? ? ? E8 ? ? ? ? 85 C0 74 1A FF 74 24 08 8B C8 56 E8 ? ? ? ? 80 8E ? ? ? ? ? 89 46 78 5E C2 04 00 ");
	if (!p.empty())
		g_allocAnimBlender_addr = (size_t)p.get_first();
	else
		result |= 1 << 7;

	p = hook::pattern("56 8B F1 83 7E 74 00 75 3B 8B 0D ? ? ? ? E8 ? ? ? ? 8B D0 85 D2 74 04 89 32 EB 02 ");
	if (!p.empty())
		g_allocNode_addr = (size_t)p.get_first();
	else
		result |= 1 << 8;

	p = hook::pattern("83 EC 14 8B 44 24 1C 8B 91 ? ? ? ? C1 E8 08 25 ? ? ? ? 56 57 89 44 24 10 33 C0 33 FF 89 4C 24 08 89 44 24 18 89 44 24 14 85 D2 0F 84 ? ? ? ? EB 0B ");
	if (!p.empty())
		g_blendAnim_addr = (size_t)p.get_first();
	else
		result |= 1 << 9;



	// hooks

	p = hook::pattern("E8 ? ? ? ? 8B 8E ? ? ? ? 85 C9 74 06 8B 01 56 FF 50 04 ");
	if (!p.empty())
		hooks_addr::g_CPed__ProcessControl_hook_addr = (size_t)p.get_first();
	else
		result |= 1 << 10;
	//p = hook::pattern("56 8B F1 E8 ? ? ? ? 8B 8E ? ? ? ? 85 C9 74 06 8B 01 56 FF 50 04 ");
	//if (!p.empty())
	//	g_CPed__ProcessControl_hook_addr = (size_t)p.get_first(3);
	//else
	//	result |= 1 << 10;

	p = hook::pattern("C7 06 ? ? ? ? 89 0A F3 0F 11 46 ? 89 86 ? ? ? ? 85 C9 74 06 52 E8 ? ? ? ? ");
	if (!p.empty()) {
		size_t* vmt = *(size_t**)p.get_first(2);

		hooks_addr::g_CTaskSimpleFireGun__process_hook_addr = (size_t)(vmt + 0x44 / sizeof(size_t));
		hooks_addr::g_gunFireAbortVmt_addr = (size_t)(*(size_t**)p.get_first(2) + 5); // 0 for aim event
	}
	else
		result |= 1 << 11;

	p = hook::pattern("8B 81 ? ? ? ? 85 C0 74 2F 56 8B 74 24 08 90 66 83 78 ? ? 8D 48 04 8B 80 ? ? ? ? 75 05 8B 51 40 EB 02 ");
	if (!p.empty())
		g_CAnimBlender__findAnimInBlend_addr = (size_t)p.get_first();
	else
		result |= 1 << 12;


	p = hook::pattern("56 8B F1 E8 ? ? ? ? 8B 4C 24 08 F3 0F 10 44 24 ? 8B C1 F3 0F 11 46 ? F3 0F 10 44 24 ? C1 E8 1B C7 06 ? ? ? ? 89 4E 60 C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? F3 0F 11 46 ? C7 ");
	if (!p.empty())
		hooks_addr::g_gunReloadAbortVmt_addr = (size_t)(*(size_t**)p.get_first(0x22 + 2) + 5); // пятая функция
	else
		result |= 1 << 13;

	p = hook::pattern("55 8B CF C7 47 ? ? ? ? ? E8 ? ? ? ? 6A 01 8B CF C7 46 ? ? ? ? ? E8 ? ? ? ? E9 ? ? ? ? ");
	if (!p.empty())
		hooks_addr::g_CTaskSimpleReloadGun__process_hook_addr = (size_t)p.get_first(10);
	else
		result |= 1 << 14;


	p = hook::pattern("8B C1 56 8B 70 14 85 F6 74 1E 8B B6 ? ? ? ? 85 F6 74 1F 8B 08 83 C1 03 8D 14 49 8B 4E 18 3B 0C 90 75 0F 8B C6 5E C3 ");
	if (!p.empty())
		g_CPedWeapons__getWeaponData = (size_t)p.get_first();
	else
		result |= 1 << 15;

	p = hook::pattern("83 EC 10 53 55 8B D9 56 66 83 7B ? ? 57 89 5C 24 14 0F 85 ? ? ? ? 8B 4B 40 85 C9 0F 84 ? ? ? ? 6A 00 68 ? ? ? ? E8 ? ? ? ? ");
	if (!p.empty())
		g_CAnimPlayer__getAnimEventTime = (size_t)p.get_first();
	else
		result |= 1 << 18;

	p = hook::pattern("C7 06 ? ? ? ? 85 C9 74 1D 51 C7 04 24 ? ? ? ? E8 ? ? ? ? 8B 4E 48 56 E8 ? ? ? ? ");
	if (!p.empty()) {
		size_t* vmt = *(size_t**)p.get_first(2);

		hooks_addr::g_CTaskSimpleReloadGun__process_always_hook_addr = (size_t)(vmt + 0x44 / sizeof(size_t));
	}
	else
		result |= 1 << 19;


	p = hook::pattern("3D ? ? ? ? 75 1D 8D 8E ? ? ? ? E8 ? ? ? ? 85 C0 74 0E 83 78 1C 00 74 08 56 8B C8 E8 ? ? ? ? ");
	if (!p.empty())
		hooks_addr::g_coverReloadFix_patch_addr = (size_t)p.get_first(1);

	else
		result |= 1 << 20;

	p = hook::pattern("85 C9 74 38 81 C1 ? ? ? ? E8 ? ? ? ? 8B F0 85 F6 74 27 FF 76 18 E8 ? ? ? ? ");
	if (!p.empty())
		hooks_addr::g_fireAbortFix_patch_addr = (size_t)p.get_first(0);

	else
		result |= 1 << 21;

	p = hook::pattern("E8 ? ? ? ? 89 44 24 20 85 C0 0F 84 ? ? ? ? 8B CF E8 ? ? ? ? 8B F0 32 DB 89 74 24 24 83 FE 03 7D 47 8B 17 8D 44 24 30 50 8B CF FF 92 ? ? ? ? ");
	if (!p.empty())
		g_CPed__getPad2 = getFnAddrInCallOpcode((size_t)p.get_first());

	else
		result |= 1 << 22;

	p = hook::pattern("E8 ? ? ? ? EB 23 8B 46 40 8B 48 38 ");
	if (!p.empty()) {

		hooks_addr::g_processIk_addr = (size_t)p.get_first();

	}
	else
		result |= 1 << 23;

	p = hook::pattern("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8B 46 40 8B 80 ? ? ? ? 83 B8 ? ? ? ? ? ");
	if (!p.empty())
		g_CVehicle__isDriver = getFnAddrInCallOpcode((size_t)p.get_first());

	else
		result |= 1 << 24;

	p = hook::pattern("55 8B EC 83 E4 F0 81 EC ? ? ? ? 80 7D 08 00 56 8B F1 57 8B 46 40 0F BF 40 2E 8B 0C 85 ? ? ? ? ");
	if (!p.empty())
		g_CIkManager__setHandPos = (size_t)p.get_first();

	else
		result |= 1 << 25;

	p = hook::pattern("56 8B F1 8B 06 FF 90 ? ? ? ? 85 C0 74 19 8B 06 8B CE FF 90 ? ? ? ? 8B 10 8B C8 FF 92 ? ? ? ? 8B 40 04 5E ");
	if (!p.empty())
		g_CDynamicEntity__getSkeletonData = (size_t)p.get_first();

	else
		result |= 1 << 26;

	p = hook::pattern("83 EC 1C 53 8B D9 8B 03 FF 90 ? ? ? ? 85 C0 74 16 8B 03 ");
	if (!p.empty())
		g_CDynamicEntity__getBoneMatrixInWorldSpace = (size_t)p.get_first();

	else
		result |= 1 << 26;

	p = hook::pattern("57 8B B9 ? ? ? ? 85 FF 74 5F 53 8A 5C 24 10 55 8B 6C 24 10 ");
	if (!p.empty())
		g_CAnimBlender__getPlayerByAnimId = (size_t)p.get_first();

	else
		result |= 1 << 27;


	return result;
}

/**
 * @brief Initializes addresses for functions within the game's preCE build executable.
 *
 * This function attempts to locate and initialize the addresses of specific functions
 * within the game's executable file. It relies on predefined patterns to identify
 * these functions in memory.
 *
 * @return int Returns an error code. 0 indicates successful initialization, while
 * any other value signifies an error during the process.
 */
int initAddrs_preCE() {
	int result = 0;

	hook::pattern p("8B 14 8D ? ? ? ? 8B 8A ? ? ? ? 6A 01 6A 00 6A 00 6A 00 50 53 8D 44 24 28"); // from lc_iv
	if (!p.empty())
		g_ppMdlInfos = *(CBaseModelInfo***)p.get_first(3);
	else
		result |= 1 << 1;

	p = hook::pattern("56 B9 ? ? ? ? BE ? ? ? ? EB 03 8D 49 00 E8 ? ? ? ? 81 C1 ? ? ? ? 83 EE 01 79 F0 68 ? ? ? ? E8 ? ? ? ? 83 C4 04 5E ");
	if (!p.empty())
		g_pWeapInfo = *(CWeaponInfo**)p.get_first(1 + 1);
	else
		result |= 1 << 2;
	//p = hook::pattern("8B F8 69 FF ? ? ? ? 81 C7 ? ? ? ? 89 07 8B 73 1C 85 F6 74 42 ");
	//if (!p.empty())
	//	g_pWeapInfo = *(tWeapInfo**)p.get_first(10);
	//else
	//	result |= 1 << 2;

	p = hook::pattern("B9 ? ? ? ? E8 ? ? ? ? 8B F0 8B 44 24 10 8B D6 2B D0 8D A4 24 ? ? ? ? ");
	if (!p.empty())
		g_pAnimAssociations = *(CAnimAssociations__AnimData***)p.get_first(1);
	else
		result |= 1 << 3;

	p = hook::pattern("8B 44 24 08 8B 4C 24 04 50 51 E8 ? ? ? ? 83 C4 04 8B C8 E8 ? ? ? ? C3");
	if (!p.empty())
		g_getAnimByIdAndHash_addr = (size_t)p.get_first();
	else
		result |= 1 << 4;

	p = hook::pattern("56 8B F1 8B 06 8B 90 ? ? ? ? FF D2 85 C0 74 19 8B 06 8B 90 ? ? ? ? 8B CE FF D2 8B 10 8B C8 8B 82 ? ? ? ? 5E ");
	if (!p.empty())
		g_checkCoords_addr = (size_t)p.get_first();
	else
		result |= 1 << 5;

	//p = hook::pattern("E8 ? ? ? ? 83 7E 78 00 75 09 6A 00 8B CE E8 ? ? ? ? 83 7E 74 00 ");
	//if (!p.empty()) {
	//	size_t addr = (size_t)p.get_first();
	//	g_allocCoords_addr = *(DWORD*)(addr + 1) + addr + 5;
	//}
	//else 
	//	result |= 1 << 6;
	p = hook::pattern("56 8B F1 8B 06 8B 90 ? ? ? ? FF D2 85 C0 0F 85 ? ? ? ? 8B 46 34 85 C0 0F 84 ? ? ? ? 8B 00 85 C0 0F 84 ? ? ? ? 57 ");
	if (!p.empty())
		g_allocCoords_addr = (size_t)p.get_first();
	else
		result |= 1 << 6;

	p = hook::pattern("56 8B F1 8B 0D ? ? ? ? E8 ? ? ? ? 85 C0 74 1B 8B 4C 24 08 51 56 8B C8 E8 ? ? ? ? 80 8E ? ? ? ? ? 89 46 78 5E ");
	if (!p.empty())
		g_allocAnimBlender_addr = (size_t)p.get_first();
	else
		result |= 1 << 7;

	p = hook::pattern("56 8B F1 83 7E 74 00 75 3A 8B 0D ? ? ? ? E8 ? ? ? ? 85 C0 74 04 89 30 EB 02 ");
	if (!p.empty())
		g_allocNode_addr = (size_t)p.get_first();
	else
		result |= 1 << 8;

	p = hook::pattern("83 EC 0C 53 8B 5C 24 18 55 56 C1 EB 08 57 8B B9 ? ? ? ? 33 ED 80 E3 01 85 FF 89 4C 24 18 89 6C 24 10 89 6C 24 14 0F 84 ? ? ? ? 0F 57 C0 ");
	if (!p.empty())
		g_blendAnim_addr = (size_t)p.get_first();
	else
		result |= 1 << 9;

	p = hook::pattern("8B 89 ? ? ? ? 33 C0 85 C9 74 2E 57 8B 7C 24 08 56 66 83 79 ? ? ");
	if (!p.empty())
		g_CAnimBlender__findAnimInBlend_addr = (size_t)p.get_first();
	else
		result |= 1 << 12;

	// hooks
	//p = hook::pattern("56 8B F1 8B 86 ? ? ? ? 83 B8 ? ? ? ? ? 0F 85 ? ? ? ? F6 86 ? ? ? ? ? 0F 85 ? ? ? ? 80 BE ? ? ? ? ? 0F 85 ? ? ? ? E8 ? ? ? ? 84 C0 ");
	//if (!p.empty())
	//	preCE_hooks_addr::g_CPed__ProcessControl_addr = (size_t)p.get_first();
	//else
	//	result |= 1;
	p = hook::pattern("56 8B F1 E8 ? ? ? ? 8B 8E ? ? ? ? 85 C9 74 08 8B 01 8B 50 04 56 FF D2 ");
	if (!p.empty())
		preCE_hooks_addr::g_CPed__ProcessControl_hook_addr = (size_t)p.get_first(3);
	else
		result |= 1 << 10;

	p = hook::pattern("55 8B EC 83 E4 F0 83 EC 18 53 56 8B F1 E8 ? ? ? ? 66 8B 55 14 8B 4D 08 F3 0F 10 45 ? 66 89 56 40 8B 55 10 33 DB 3B CB 8D 46 14 F3 0F 11 46 ? F3 0F 10 45 ? C7 06 ? ? ? ? 89 08 F3 0F 11 46 ? 89 96 ? ? ");
	if (!p.empty()) {
		size_t* vmt = *(size_t**)p.get_first(0x36 + 2);

		preCE_hooks_addr::g_CTaskSimpleFireGun__process_hook_addr = (size_t)(vmt + 0x44 / sizeof(size_t));
		preCE_hooks_addr::g_gunFireAbortVmt_addr = (size_t)(*(size_t**)p.get_first(0x36 + 2) + 5); // 0 for aim event
	}
	else
		result |= 1 << 11;
	p = hook::pattern("C7 06 ? ? ? ? 89 4E 60 F3 0F 11 46 ? 74 03 89 46 4C ");
	if (!p.empty())
		preCE_hooks_addr::g_gunReloadAbortVmt_addr = (size_t)(*(size_t**)p.get_first(2) + 5);
	else
		result |= 1 << 13;

	p = hook::pattern("55 8B CE 89 5E 64 E8 ? ? ? ? 6A 01 8B CE C7 47 ? ? ? ? ? E8 ? ? ? ? E9 ? ? ? ? ");
	if (!p.empty())
		preCE_hooks_addr::g_CTaskSimpleReloadGun__process_hook_addr = (size_t)p.get_first(6);
	else
		result |= 1 << 14;


	p = hook::pattern("8B 41 2C 85 C0 74 1E 8B 80 ? ? ? ? 85 C0 74 20 8B 51 18 83 C2 05 56 8B 70 18 8D 14 52 3B 34 91 5E ");
	if (!p.empty())
		g_CPedWeapons__getWeaponData = (size_t)p.get_first();
	else
		result |= 1 << 15;

	p = hook::pattern("E8 ? ? ? ? 89 07 5F B0 01 5E C3 5F 32 C0 5E C3 ");
	if (!p.empty())
		g_CPBuffer__get = getFnAddrInCallOpcode((size_t)p.get_first());
	else
		result |= 1 << 16;

	p = hook::pattern("E8 ? ? ? ? 83 C7 01 83 C3 04 3B 3D ? ? ? ? 7C EA 33 C0 39 35 ? ? ? ? 7E 2B B9 ? ? ? ? EB 09 ");
	if (!p.empty())
		g_CPBuffer__set = getFnAddrInCallOpcode((size_t)p.get_first());
	else
		result |= 1 << 17;

	p = hook::pattern("83 EC 10 53 55 56 57 8B F9 66 83 7F ? ? 89 7C 24 1C 0F 85 ? ? ? ? 8B 4F 40 ");
	if (!p.empty())
		g_CAnimPlayer__getAnimEventTime = (size_t)p.get_first();
	else
		result |= 1 << 18;

	p = hook::pattern("C7 06 ? ? ? ? 89 4E 60 F3 0F 11 46 ? 74 03 89 46 4C ");
	if (!p.empty()) {
		size_t* vmt = *(size_t**)p.get_first(2);

		preCE_hooks_addr::g_CTaskSimpleReloadGun__process_always_hook_addr = (size_t)(vmt + 0x44 / sizeof(size_t));
	}
	else
		result |= 1 << 19;

	p = hook::pattern("3D ? ? ? ? 75 1D 8D 8F ? ? ? ? E8 ? ? ? ? 85 C0 74 0E 83 78 1C 00 74 08 57 8B C8 E8 ? ? ? ? ");
	if (!p.empty())
		preCE_hooks_addr::g_coverReloadFix_patch_addr = (size_t)p.get_first(1);

	else
		result |= 1 << 20;

	p = hook::pattern("85 C0 74 3A 8D 88 ? ? ? ? E8 ? ? ? ? 8B F0 85 F6 74 29 8B 46 18 50 ");
	if (!p.empty())
		preCE_hooks_addr::g_fireAbortFix_patch_addr = (size_t)p.get_first(0);

	else
		result |= 1 << 21;
	
	p = hook::pattern("E8 ? ? ? ? F6 46 68 01 8B D8 57 8B CE 74 07 E8 ? ? ? ? EB 05 ");
	if (!p.empty())
		g_CPed__getPad2 = getFnAddrInCallOpcode((size_t)p.get_first());

	else
		result |= 1 << 22;

	p = hook::pattern("C7 06 ? ? ? ? E8 ? ? ? ? 8B 4C 24 0C 0F 57 C0 8B 54 24 18 F3 0F 10 4C 24 ? 89 4E 20 F3 0F 11 46 ? F3 0F 11 46 ? F3 0F 11 46 ? ");
	if (!p.empty()) {
		size_t* vmt = *(size_t**)p.get_first(2);

		preCE_hooks_addr::g_CTaskSimpleNewGangDriveBy__process_vmt_addr = (size_t)(vmt + 0x44 / sizeof(size_t));
		preCE_hooks_addr::g_CTaskSimpleNewGangDriveBy__abort_vmt_addr = (size_t)(vmt + 0x14 / sizeof(size_t));
	}
	else
		result |= 1 << 23;


	return result;
}

/**
 * @brief Namespace containing hooks for the CE game build.
 *
 * This namespace provides a set of functions that allow for the modification
 * and interception of game behavior. These hooks are specifically designed for the CE build
 * It is important to note that the use of these hooks may have unintended
 * consequences and should be used with caution.
 */

struct CPed;
struct CVehicle;
namespace hooks {

/**
 * @brief Namespace for fire-related hooks.
 *
 * This namespace contains hooks for fire events.
 * It triggers the corresponding 'weapon_fire' animation from the local #ad animation container.
 */
namespace fire {

size_t origcall; // оригинальный адрес функции
struct tFirePed {
	char processControl() {
		auto ret = ((char(__thiscall*)(tFirePed*))(origcall))(this);
		//processFire((CPed*)this);
		return ret;
	}

	bool run(tFirePed* ped) {
		preProcessFire((CTaskSimpleFireGun*)this, (CPed*)ped);
		auto ret = ((bool(__thiscall*)(void*, tFirePed*))(origcall))(this, ped);
		postProcessFire((CTaskSimpleFireGun*)this, (CPed*)ped);

		return ret;
	}


};

void init() {
	//origcall = setFnAddrInCallOpcode(hooks_addr::g_CPed__ProcessControl_hook_addr, getThisCallFuncAddres(&tFirePed::processControl));
	origcall = writeDWORD(hooks_addr::g_CTaskSimpleFireGun__process_hook_addr, getThisCallFuncAddres(&tFirePed::run));

}

} // fire

/**
 * @brief Namespace for reload-related hooks.
 *
 * This namespace contains hooks for reload events.
 * It triggers the corresponding 'weapon_reload' animation from the local #ad animation container.
 */
namespace reload {

size_t origCall;
size_t origCall_always;

struct tReloadPed {

	int run(tReloadPed* ped) {
		auto ret = ((int(__thiscall*)(void*, tReloadPed*))(origCall))(this, ped);
		processReloadZeroState((CTaskSimpleReloadGun*)this, (CPed*)ped);
		processReload((CTaskSimpleReloadGun*)this, (CPed*)ped);
		return ret;

	}

	bool runAlways(tReloadPed* ped) {
		preProcessShotgunReload((CTaskSimpleReloadGun*)this, (CPed*)ped);
		auto ret = ((bool(__thiscall*)(void*, tReloadPed*))(origCall_always))(this, ped);
		postProcessShotgunReload((CTaskSimpleReloadGun*)this, (CPed*)ped);
		return ret;
	}
};


void init() {
	size_t addr = hooks_addr::g_CTaskSimpleReloadGun__process_hook_addr;
	origCall = setFnAddrInCallOpcode(addr, getThisCallFuncAddres(&tReloadPed::run));
	addr = hooks_addr::g_CTaskSimpleReloadGun__process_always_hook_addr;
	origCall_always = writeDWORD(addr, getThisCallFuncAddres(&tReloadPed::runAlways));
}

} // reload

/**
 * @brief Namespace for fire abort hooks.
 *
 * This namespace contains hooks for interrupting the fire animation at specific game moments.
 */
namespace fire_abort {
size_t g_gunFireAbort_origCall_addr;

struct tFireAbortPed;
struct tFireAbortEvent;

struct tFireAbortTask {

	char run(tFireAbortPed* ped, int _b, tFireAbortEvent* _c) {
		processFireAbort((CPed*)ped);
		return ((char(__thiscall*)(void*, tFireAbortPed*, int, tFireAbortEvent*))(g_gunFireAbort_origCall_addr))(this, ped, _b, _c);
	}

};

void init() {
	size_t addr = hooks_addr::g_gunFireAbortVmt_addr;
	g_gunFireAbort_origCall_addr = writeDWORD(addr, getThisCallFuncAddres(&tFireAbortTask::run));

}

} // fire_abort

/**
 * @brief Namespace for reload abort hooks.
 *
 * This namespace contains hooks for interrupting the reload animation at specific game moments.
 */
namespace reload_abort {
size_t g_gunReloadAbort_origCall_addr;

struct tReloadAbortPed;
struct tReloadAbortEvent;

struct tReloadAbortTask {

	char run(tReloadAbortPed* ped, int _b, tReloadAbortEvent* _c) {

		processReloadAbort((CTaskSimpleReloadGun*)_c, (CPed*)ped);

		//ped = nullptr;

		auto ret = ((char(__thiscall*)(void*, tReloadAbortPed*, int, tReloadAbortEvent*))(g_gunReloadAbort_origCall_addr))(this, ped, _b, _c);

		return ret;
	}

};

void init() {
	size_t addr = hooks_addr::g_gunReloadAbortVmt_addr;

	g_gunReloadAbort_origCall_addr = writeDWORD(addr, getThisCallFuncAddres(&tReloadAbortTask::run));

}

} // reload_abort

#ifndef GTA_IV_PRE_CE

namespace weap_ik {

struct CIkMgr {
	BYTE __0[0x40];
	CPed* m_pPedRef;

	static size_t ms_processHandIkOrigcall;

	char processHandIk() {
		auto ret = ((char(__thiscall*)(CIkMgr*))(ms_processHandIkOrigcall))(this);
		
		processWeapIk(m_pPedRef);

		return ret;
	}


};
size_t CIkMgr::ms_processHandIkOrigcall;


void init() {
//	CIkMgr::ms_processHandIkOrigcall = setFnAddrInCallOpcode(hooks_addr::g_processIk_addr, (size_t)processHandIk2);
	CIkMgr::ms_processHandIkOrigcall = setFnAddrInCallOpcode(hooks_addr::g_processIk_addr, getThisCallFuncAddres(&CIkMgr::processHandIk));
	//CIkMgr::ms_processHandIkOrigcall = setFnAddrInCallOpcode(hooks_addr::g_processIk_addr, getThisCallFuncAddres(&CIkMgr::processHandIk3));

}

}
#endif

} // hooks

/**
 * @brief Namespace containing hooks for the preCE game build.
 *
 * This namespace provides a set of functions that allow for the modification
 * and interception of game behavior. These hooks are specifically designed for the preCE build
 * It is important to note that the use of these hooks may have unintended
 * consequences and should be used with caution.
 * At the moment everything here is almost identical to the CE build.
 */
namespace preCE_hooks {


namespace fire {
size_t origcall;
struct tFirePed {
	char processControl() {
		auto ret = ((char(__thiscall*)(tFirePed*))(origcall))(this);
	//	processFire((CPed*)this);
		return ret;
	}

	bool run(tFirePed* ped) {
		preProcessFire((CTaskSimpleFireGun*)this, (CPed*)ped);
		auto ret = ((bool(__thiscall*)(void*, tFirePed*))(origcall))(this, ped);
		postProcessFire((CTaskSimpleFireGun*)this, (CPed*)ped);

		return ret;
	}


};

void init() {
	//origcall = setFnAddrInCallOpcode(preCE_hooks_addr::g_CPed__ProcessControl_hook_addr, getThisCallFuncAddres(&tFirePed::processControl));
	origcall = writeDWORD(preCE_hooks_addr::g_CTaskSimpleFireGun__process_hook_addr, getThisCallFuncAddres(&tFirePed::run));

}

} // fire

namespace reload {

size_t origCall;
size_t origCall_always;

struct tReloadPed {

	int run(tReloadPed* ped) {
		auto ret = ((int(__thiscall*)(void*, tReloadPed*))(origCall))(this, ped);
		processReloadZeroState((CTaskSimpleReloadGun*)this, (CPed*)ped);
		processReload((CTaskSimpleReloadGun*)this, (CPed*)ped);
		return ret;

	}

	bool runAlways(tReloadPed* ped) {
		preProcessShotgunReload((CTaskSimpleReloadGun*)this, (CPed*)ped);
		auto ret = ((bool(__thiscall*)(void*, tReloadPed*))(origCall_always))(this, ped);
		postProcessShotgunReload((CTaskSimpleReloadGun*)this, (CPed*)ped);
		return ret;
	}
};



void init() {
	size_t addr = preCE_hooks_addr::g_CTaskSimpleReloadGun__process_hook_addr;
	origCall = setFnAddrInCallOpcode(addr, getThisCallFuncAddres(&tReloadPed::run));
	addr = preCE_hooks_addr::g_CTaskSimpleReloadGun__process_always_hook_addr;
	origCall_always = writeDWORD(addr, getThisCallFuncAddres(&tReloadPed::runAlways));
}

} // reload

namespace fire_abort {
size_t g_gunFireAbort_origCall_addr;

struct tFireAbortPed;
struct tFireAbortEvent;

struct tFireAbortTask {

	char run(tFireAbortPed* ped, int _b, tFireAbortEvent* _c) {
		processFireAbort((CPed*)ped);
		return ((char(__thiscall*)(void*, tFireAbortPed*, int, tFireAbortEvent*))(g_gunFireAbort_origCall_addr))(this, ped, _b, _c);
	}

};

void init() {
	size_t addr = preCE_hooks_addr::g_gunFireAbortVmt_addr;
	g_gunFireAbort_origCall_addr = writeDWORD(addr, getThisCallFuncAddres(&tFireAbortTask::run));

}

} // fire_abort

namespace reload_abort {
size_t g_gunReloadAbort_origCall_addr;

struct tReloadAbortPed;
struct tReloadAbortEvent;

struct tReloadAbortTask {

	char run(tReloadAbortPed* ped, int _b, tReloadAbortEvent* _c) {

		processReloadAbort((CTaskSimpleReloadGun*)_c, (CPed*)ped);

		return ((char(__thiscall*)(void*, tReloadAbortPed*, int, tReloadAbortEvent*))(g_gunReloadAbort_origCall_addr))(this, ped, _b, _c);
	}

};

void init() {
	size_t addr = preCE_hooks_addr::g_gunReloadAbortVmt_addr;

	g_gunReloadAbort_origCall_addr = writeDWORD(addr, getThisCallFuncAddres(&tReloadAbortTask::run));

}

} // reload_abort

namespace driveby {

size_t g_abort_origcall;
size_t g_processPed_origcall;

struct tDriveByTask {

	char abort(void* pPed, DWORD addonFlags, void* pEvent) {
		processFireAbort((CPed*)pPed);
		return ((char(__thiscall*)(void*, void* pPed, DWORD addonFlags, void* pEvent))(g_abort_origcall))(this, pPed, addonFlags, pEvent);
	}

	char processPed(void* pPed) {
		auto ret = ((char(__thiscall*)(void*, void* pPed))(g_processPed_origcall))(this, pPed);
		postProcessFire2((CPed*)pPed);
		return ret;
	}

	
};

void init() {
	g_abort_origcall = writeDWORD(preCE_hooks_addr::g_CTaskSimpleNewGangDriveBy__abort_vmt_addr, getThisCallFuncAddres(&tDriveByTask::abort));
	g_processPed_origcall = writeDWORD(preCE_hooks_addr::g_CTaskSimpleNewGangDriveBy__process_vmt_addr, getThisCallFuncAddres(&tDriveByTask::processPed));

}

}

} // preCE_hooks

/**
 * @brief Enables a debug console.
 *
 * This function is intended to create and display a console window,
 * which can be used for debugging purposes. It allows me to
 * output diagnostic information during runtime.
 */
void enableConsole() {
	FILE* pFile = NULL;
	AllocConsole();
	SetConsoleTitleA("SHook debug console");
	freopen_s(&pFile, "CONOUT$", "w", stdout);
	freopen_s(&pFile, "CONOUT$", "w", stderr);
	freopen_s(&pFile, "CONIN$", "r", stdin);
}

/**
 * @brief Patches the game's memory by injecting hooks for preCE build.
 *
 * This function modifies the game's memory by overwriting specific instructions
 * with jump instructions to custom hook functions. It is typically called
 *
 * @note This function does not return any value.
 * @note This function does not accept any arguments.
 */
void patchMem_preCE() {
	preCE_hooks::fire::init();
	preCE_hooks::reload::init();
	preCE_hooks::fire_abort::init();
	preCE_hooks::reload_abort::init();
	preCE_hooks::driveby::init();

	makeNop(preCE_hooks_addr::g_fireAbortFix_patch_addr, 2);
	writeDWORD(preCE_hooks_addr::g_coverReloadFix_patch_addr, ~1);
}

/**
 * @brief Patches the game's memory by injecting hooks for CE build.
 *
 * This function modifies the game's memory by overwriting specific instructions
 * with jump instructions to custom hook functions. It is typically called
 *
 * @note This function does not return any value.
 * @note This function does not accept any arguments.
 */
void patchMem() {
	hooks::fire::init();
	hooks::reload::init();
	hooks::fire_abort::init();
	hooks::reload_abort::init();

	makeNop(hooks_addr::g_fireAbortFix_patch_addr, 2);
	writeDWORD(hooks_addr::g_coverReloadFix_patch_addr, ~1);

	//hooks::weap_ik::init();
}

/**
 * @brief Runs a patch.
 *
 * This function executes a patch, modifying the program's memory
 * It does not return any value and does not accept any arguments.
 */
void runPatch() {

#ifdef GTA_IV_PRE_CE
	int ret = initAddrs_preCE();
#else
	int ret = initAddrs();
#endif

	if (ret) {
		char msg[0x1ff];
		sprintf_s(msg, sizeof msg / sizeof * msg, "Unk version\nerror code: %#010x", ret);

		MessageBoxA(NULL, msg, "AnimatedWeapons", 0x10);
	}
	else {
#ifdef GTA_IV_PRE_CE
		patchMem_preCE();
#else
		patchMem();
#endif

	}

}

size_t g_hookAddr_Patch;
size_t g_hookAddr_PatchOrigcall;

int patchGame() {
	runPatch();


	return ((int(*)())(g_hookAddr_PatchOrigcall))();
}

void createPatchHook() {
	hook::pattern p("E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E9 ? ? ? ? ");
	if (!p.empty())
		g_hookAddr_Patch = (size_t)p.get_first();
	else {
		MessageBoxA(NULL, "Unk version", "AnimatedWeapons", 0x10);
		return;
	}

	g_hookAddr_PatchOrigcall = setFnAddrInCallOpcode(g_hookAddr_Patch, (size_t)patchGame);

}
