// Yulian Shvab 2025
// Released into the public domain; no copyright claimed
#pragma once
#include <Windows.h>

#define STATIC_ASSERT_EXPR(expr) static_assert(expr, "Failed: " STR(expr))

// forward declarations 
struct CBaseModelInfo;
struct CWeaponInfo;
struct CAnimAssociations__AnimData;

/**
 * Addresses of functions and memory objects from the game
 * Used in weapAnims.cpp and initialized in patch.cpp
 */
extern CBaseModelInfo** g_ppMdlInfos;
extern CWeaponInfo* g_pWeapInfo;
extern CAnimAssociations__AnimData** g_pAnimAssociations; // ms_aAnimAssociations
extern size_t g_getAnimByIdAndHash_addr;
extern size_t g_checkCoords_addr;
extern size_t g_allocCoords_addr;
extern size_t g_allocAnimBlender_addr;
extern size_t g_allocNode_addr;
extern size_t g_blendAnim_addr;
extern size_t g_CAnimBlender__findAnimInBlend_addr;

extern size_t g_CPedWeapons__getWeaponData;
extern size_t g_CPBuffer__get;
extern size_t g_CPBuffer__set;
extern size_t g_CAnimPlayer__getAnimEventTime;
extern size_t g_getWeaponByType;
extern size_t g_CPed__getPad2;
extern size_t g_CVehicle__isDriver;
extern size_t g_CIkManager__setHandPos;
extern size_t g_CDynamicEntity__getSkeletonData;
extern size_t g_CDynamicEntity__getBoneMatrixInWorldSpace;
extern size_t g_CAnimBlender__getPlayerByAnimId;


/**
 * @brief Writes a DWORD value to the specified memory address.
 *
 * This function writes a DWORD value to the specified memory address. It first
 * disables memory protection on the page containing the address, writes the
 * value, and then restores the original memory protection.
 *
 * @param addr The memory address to write to.
 * @param val The DWORD value to write.
 * @return The original DWORD value at the specified address.
 */
DWORD writeDWORD(size_t addr, DWORD val);

/**
 * @brief Sets the function address in a call opcode and returns the old address.
 *
 * This function modifies the call opcode at the specified position to
 * jump to the provided function address. It calculates the relative
 * address from the call instruction to the target function and overwrites the
 * existing address in the opcode.
 *
 * @param callPos The address of the call opcode.
 * @param pfn The address of the function to call.
 * @return The old address that was originally in the call opcode, adjusted to
 * be an absolute address.
 */
size_t setFnAddrInCallOpcode(size_t callPos, size_t pfn);
size_t getFnAddrInCallOpcode(size_t callPos);

/**
 * @brief Retrieves the address of a class member function.
 * This function takes a reference to a class member function and returns its address.
 * @tparam T The type of the class member function.
 * @param func A reference to the class member function.
 * @return The address of the class member function.
 */
template<typename T> size_t getThisCallFuncAddres(T func) { return (size_t)(void*&)func; }

void makeNop(size_t addr, size_t size);
