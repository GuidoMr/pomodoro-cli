#include <stddef.h>
#include <time.h>
#ifndef TIMER_H
#define TIMER_H

// Store possible timer states - paused and running.
typedef enum {
    TIMER_RUNNING,
    TIMER_PAUSED
} TimerState;

// The timer type.
typedef struct {
    // The time set to end the timer, should it not be paused.
    struct timespec end_time;
    // The remaining time on the timer **WHEN IT IS PAUSED**.
    struct timespec remainingTime;
    
    // The number of seconds the timer starts at.
    int start_seconds;
    // The timer state - running or paused.
    TimerState state;
} Timer;

/**
 * Creates a timer.
 * @param minutes Number of minutes for the timer.
 * @param seconds Number of seconds for the timer.
 * return the timer.
 */
Timer initTimer(int minutes, int seconds);
/**
 * Creates a timer.
 * @param seconds Number of seconds for the timer.
 * @return the timer.
 */
Timer initTimerSeconds(int seconds);

/**
 * Resets a timer to a new time.
 * @param minutes Number of minutes for the timer.
 * @param seconds Number of seconds for the timer.
 * @param timer A pointer to the timer to be reset.
 */
void resetTimer(int minutes, int seconds, Timer* timer);

/**
 * Resets a timer to a new time.
 * @param seconds Number of seconds for the timer.
 * @param timer A pointer to the timer to be reset.
 */
void resetTimerSeconds(int seconds, Timer* timer);

/**
 * Gets the remaining time on the timer.
 * @param timer The timer.
 * @return the remaining time (seconds and nanoseconds).
 */ 
struct timespec remainingTime(const Timer* timer);

/**
 * Gets the remaining seconds on the timer (just the seconds from the remianing time).
 * @param timer The timer.
 * @return the remaining seconds.
 */
int getRemainingSeconds(const Timer* timer);

/**
 * Gets the remaining minutes on the timer (just the minutes from the remianing time).
 * @param timer The timer.
 * @return the remaining minutes.
 */
int getRemainingMinutes(const Timer* timer);

/**
 * Gets the percentage of time that has already passed from the timer.
 * @param timer The timer.
 * @return the percentage of time that has already passed.
 */
int getPercentage(const Timer* timer);

/**
 * Pauses the timer if unpaused, unpauses if paused
 * @param timer The timer.
 */
void togglePauseTimer(Timer* timer);

/**
 * Pauses the timer.
 * @param timer The timer.
 */
void pauseTimer(Timer* timer);

/**
 * Resumes the timer after.
 * @param timer The timer.
 */
void resumeTimer(Timer* timer);

/**
 * Wheather or not the timer is paused.
 * @param timer The timer.
 * @return 1 if paused, 0 otherwise.
 */
int isTimerPaused(const Timer* timer);

#endif // !TIMER_H
#define TIMER_H
