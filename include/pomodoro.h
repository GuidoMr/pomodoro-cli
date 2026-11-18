#include "timer.h"

#ifndef POMODORO_H
#define POMODORO_H

// Stores the possible pomodoro phases.
typedef enum {
    FOCUS,
    SHORT_BREAK,
    LONG_BREAK,
} PomodoroPhase;

// All the data needed for the pomodoro app.
typedef struct {
    // The timer.
    Timer timer;

    // The number in the cycle (FOCUS + PAUSE = one cycle). (0 -> 1 -> 2 -> 3 -> 4 -> 0).
    int cycleNum;

    // The phase of the pomodoro app (FOCUS, SHORT BREAK or LONG_BREAK).
    PomodoroPhase phase;

} Pomodoro;

/**
 * Initializes an instance of the pomodoro app.
 */
Pomodoro initPomodoro();

/**
 * Updates the pomodoro app to the next phase in the cycle (or next cycle).
 * @param pomodoro A pointer to the instance of pomodoro.
 */
void nextPomodoro(Pomodoro *pomodoro);

/**
 * Toggles the pause function on the pomodoro timer, pausing the app.
 * @param pomodoro A pointer to the instance of pomodoro.
 */
void togglePausePomodoro(Pomodoro *pomodoro);

/**
 * Gets the phase an instance of pomodoro is at.
 * @param pomodoro A pointer to the instance of pomodoro.
 * @return The phase - 0 = FOCUS, 1 = SHORT_BREAK and 2 = LONG_BREAK.
 */
PomodoroPhase getPhase(const Pomodoro *pomodoro);

/**
 * Gets the percentage of the time that has already passed.
 * @param pomodoro A pointer to the instance of pomodoro.
 * @return The percentage.
 */
int getTimerPercentage(const Pomodoro *pomodoro);

/**
 * Gets the number of minutes left in the current phase.
 * @param pomodoro A pointer to the instance of pomodoro.
 * @return The number of minutes.
 */
int getRemainingPhaseMinutes(const Pomodoro *pomodoro);

/**
 * Gets the number of seconds left in the current phase.
 * @param pomodoro A pointer to the instance of pomodoro.
 * @return The number of seconds.
 */
int getRemainingPhaseSeconds(const Pomodoro *pomodoro);

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
 * @return 1 if finished, 0 otherwise.
 */
int isPomodoroPhaseFinished(const Pomodoro *pomodoro);

#endif // !POMODORO_H
