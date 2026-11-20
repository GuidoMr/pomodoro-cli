#include "timer.h"

#ifndef POMODORO_H
#define POMODORO_H

// Stores the possible pomodoro phases.
typedef enum {
    FOCUS,
    SHORT_BREAK,
    LONG_BREAK,
} PomodoroPhase;

typedef enum {
    POMODORO_OK,
    POMODORO_ERROR_NOT_INITIALIZED,
    POMODORO_ERROR_NULL_POINTER,
    POMODORO_ERROR_TIMER
} PomodoroError;

// All the data needed for the pomodoro app.
typedef struct {
    // The timer.
    Timer timer;

    // The number in the cycle (FOCUS + PAUSE = one cycle). (0 -> 1 -> 2 -> 3 -> 4 -> 0).
    int cycleNum;

    // The phase of the pomodoro app (FOCUS, SHORT BREAK or LONG_BREAK).
    PomodoroPhase phase;

    // 0 if not initialized, 1 otherwise.
    int isInitialized;

} Pomodoro;

/**
 * Initializes an instance of the pomodoro app.
 * @param pomodoro A pointer to the instance of Pomodoro to be created.
 * @return an error code (POMODORO_OK if no error).
 */
PomodoroError initPomodoro(Pomodoro *pomodoro);

/**
 * Updates the pomodoro app to the next phase in the cycle (or next cycle).
 * @param pomodoro A pointer to the instance of pomodoro.
 * @return an error code (POMODORO_OK if no error).
 */
PomodoroError nextPomodoro(Pomodoro *pomodoro);

/**
 * Toggles the pause function on the pomodoro timer, pausing the app.
 * @param pomodoro A pointer to the instance of pomodoro.
 * @return an error code (POMODORO_OK if no error).
 */
PomodoroError togglePausePomodoro(Pomodoro *pomodoro);

/**
 * Gets the phase an instance of pomodoro is at.
 * @param pomodoro A pointer to the instance of pomodoro.
 * @return The phase - 0 = FOCUS, 1 = SHORT_BREAK and 2 = LONG_BREAK.
 */
PomodoroPhase getPhase(const Pomodoro *pomodoro);

/**
 * Gets the percentage of the time that has already passed.
 * @param pomodoro A pointer to the instance of pomodoro.
 * @param timerPercentage An output parameter for the percentage.
 * @return an error code (POMODORO_OK if no error).
 */
PomodoroError getTimerPercentage(const Pomodoro *pomodoro, int *timerPercentage);

/**
 * Gets the number of minutes left in the current phase.
 * @param pomodoro A pointer to the instance of pomodoro.
 * @param Output parameter for the number of minutes.
 * @return an error code (POMODORO_OK if no error).
 */
PomodoroError getRemainingPhaseMinutes(const Pomodoro *pomodoro, int *remainingMinutes);

/**
 * Gets the number of seconds left in the current phase.
 * @param pomodoro A pointer to the instance of pomodoro.
 * @param Output parameter for the number of seconds.
 * @return an error code (POMODORO_OK if no error).
 */
PomodoroError getRemainingPhaseSeconds(const Pomodoro *pomodoro, int *remainingSeconds);

/**
 * Weather or not a pomodoro instance is paused.
 * @param pomodoro A pointer to the instance of pomodoro.
 * @return 1 if paused, 0 otherwise.
 */
int isPomodoroPaused(const Pomodoro *pomodoro);
/**
 * Gets the cycle number on a pomodoro instance.
 * @param pomodoro A pointer to the instance of pomodoro.
 * @return The cycle number.
 */
int getCycleNum(const Pomodoro *pomodoro);

/**
 * Weather or not the current phase in a pomodoro instance is finished.
 * @param pomodoro A pointer to the instance of pomodoro.
 * @param ifPhaseFinished Output parameter - 1 if finished, 0 otherwise.
 * @return an error code (POMODORO_OK if no error).
 */
PomodoroError isPomodoroPhaseFinished(const Pomodoro *pomodoro, int *isPhaseFinished);

#endif // !POMODORO_H
