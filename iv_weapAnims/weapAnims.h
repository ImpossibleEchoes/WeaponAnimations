// Yulian Shvab 2025
// Released into the public domain; no copyright claimed
#pragma once

struct CPed;

/**
 * @brief Triggers the fire animation for the weapon currently held by the specified ped.
 *
 * @param ped A pointer to the CPed object representing the ped who is firing.
 *
 * @return void This function does not return any value.
 */
void processFire(CPed* ped);

/**
 * @brief Triggers the reload animation for the ped's weapon.
 *
 * @param ped A pointer to the CPed object representing the ped who is reloading their weapon.
 *
 * @return void This function does not return any value.
 */
void processReload(CPed* ped);

/**
 * @brief Aborts the firing animation of a weapon held by a ped.
 *
 * This function stops the currently playing firing animation for the weapon
 * that the specified ped is holding. It is typically used when the ped
 * interrupts or cancels their firing action.
 *
 * @param ped Pointer to the CPed object.
 *
 * @return void
 */
void processFireAbort(CPed* ped);

/**
 * @brief Aborts the reloading animation of a weapon held by a ped.
 *
 * This function stops the currently playing reloading animation for the weapon
 * that the specified ped is holding. It is typically used when the ped
 * interrupts or cancels their reloading action.
 *
 * @param ped Pointer to the CPed object.
 *
 * @return void
 */
void processReloadAbort(CPed* ped);
