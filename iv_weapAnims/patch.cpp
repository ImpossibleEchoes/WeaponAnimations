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

	p = hook::pattern("56 57 BE ? ? ? ? BF 3B 00 00 00 8D 64 24 00 8B CE E8 ? ? ? ? 81 C6 10 01 00 00 4F 79 F0 68 ? ? ? ? E8 ? ? ? ? 83 C4 04 5F 5E C3 ");
	if (!p.empty())
		g_pWeapInfo = *(CWeaponInfo**)p.get_first(1 + 1 + 1);
	else
		result |= 1 << 2;
	//p = hook::pattern("8B F8 69 FF ? ? ? ? 81 C7 ? ? ? ? 89 07 8B 73 1C 85 F6 74 42 ");
	//if (!p.empty())
	//	g_pWeapInfo = *(tWeapInfo**)p.get_first(10);
	//else
	//	result |= 1 << 2;

	p = hook::pattern("A1 ? ? ? ? 03 C7 50 53 E8 ? ? ? ? 83 C4 08 85 C0 74 15 46 83 C7 58 3B 35 ? ? ? ? 7C DF 5F 5B 83 C8 FF 5E C2 04 00 ");
	if (!p.empty())
		g_pAnimAssociations = *(CAnimDescriptor***)p.get_first(1);
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
	if (!p.empty())
		hooks_addr::g_gunFireAbortVmt_addr = (size_t)(*(size_t**)p.get_first(2) + 5); // 0 for aim event
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
		g_pAnimAssociations = *(CAnimDescriptor***)p.get_first(1);
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
	if (!p.empty())
		preCE_hooks_addr::g_gunFireAbortVmt_addr = (size_t)(*(size_t**)p.get_first(0x36 + 2) + 5); // 0 for aim event
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
		processFire((CPed*)this);
		return ret;
	}

};

void init() {
	origcall = setFnAddrInCallOpcode(hooks_addr::g_CPed__ProcessControl_hook_addr, getThisCallFuncAddres(&tFirePed::processControl));

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

struct tReloadPed {

	int run(tReloadPed* ped) {
		auto ret = ((int(__thiscall*)(void*, tReloadPed*))(origCall))(this, ped);
		processReload((CPed*)ped);
		return ret;

	}
};



void init() {
	size_t addr = hooks_addr::g_CTaskSimpleReloadGun__process_hook_addr;
	origCall = setFnAddrInCallOpcode(addr, getThisCallFuncAddres(&tReloadPed::run));
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

		processReloadAbort((CPed*)ped);

		return ((char(__thiscall*)(void*, tReloadAbortPed*, int, tReloadAbortEvent*))(g_gunReloadAbort_origCall_addr))(this, ped, _b, _c);
	}

};

void init() {
	size_t addr = hooks_addr::g_gunReloadAbortVmt_addr;

	g_gunReloadAbort_origCall_addr = writeDWORD(addr, getThisCallFuncAddres(&tReloadAbortTask::run));

}

} // reload_abort
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
		processFire((CPed*)this);
		return ret;
	}

};

void init() {
	origcall = setFnAddrInCallOpcode(preCE_hooks_addr::g_CPed__ProcessControl_hook_addr, getThisCallFuncAddres(&tFirePed::processControl));
}

} // fire

namespace reload {

size_t origCall;

struct tReloadPed {

	int run(tReloadPed* ped) {
		auto ret = ((int(__thiscall*)(void*, tReloadPed*))(origCall))(this, ped);
		processReload((CPed*)ped);
		return ret;

	}
};



void init() {
	size_t addr = preCE_hooks_addr::g_CTaskSimpleReloadGun__process_hook_addr;
	origCall = setFnAddrInCallOpcode(addr, getThisCallFuncAddres(&tReloadPed::run));
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

		processReloadAbort((CPed*)ped);

		return ((char(__thiscall*)(void*, tReloadAbortPed*, int, tReloadAbortEvent*))(g_gunReloadAbort_origCall_addr))(this, ped, _b, _c);
	}

};

void init() {
	size_t addr = preCE_hooks_addr::g_gunReloadAbortVmt_addr;

	g_gunReloadAbort_origCall_addr = writeDWORD(addr, getThisCallFuncAddres(&tReloadAbortTask::run));

}

} // reload_abort

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

		MessageBoxA(NULL, msg, NULL, 0x10);
	}
	else {
#ifdef GTA_IV_PRE_CE
		patchMem_preCE();
#else
		patchMem();
#endif

	}

}