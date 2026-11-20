#include <stddef.h>
#include <time.h>
#ifndef TIMER_H
#define TIMER_H

// Store possible timer states - paused and running.
typedef enum {
    TIMER_RUNNING,
    TIMER_PAUSED
} TimerState;

typedef enum {
    TIMER_OK,
    TIMER_ERROR_OVERFLOW,
    TIMER_ERROR_NULL_POINTER,
    TIMER_ERROR_GETTIME,
    TIMER_ERROR_ALREADY_PAUSED,
    TIMER_ERROR_ALREADY_UNPAUSED,
} TimerError;

// The timer type.
typedef struct {
    // The time set to end the timer, should it not be paused.
    struct timespec endTime;
    // The remaining time on the timer **WHEN IT IS PAUSED**.
    struct timespec remainingTime;
    
    // The number of seconds the timer starts at.
    unsigned int startSeconds;
    // The timer state - running or paused.
    TimerState state;
} Timer;

/**
 * Resets a timer to a new time.
 * @param minutes Number of minutes for the timer.
 * @param seconds Number of seconds for the timer.
 * @param timer A pointer to the timer to be created.
 * @return an error code (TIMER_OK if no error).
 */
TimerError initTimer(unsigned int minutes, unsigned int seconds, Timer* timer);

/**
 * Resets a timer to a new time.
 * @param seconds Number of seconds for the timer.
 * @param timer A pointer to the timer to be reset.
 * @return an error code (TIMER_OK if no error).
 */
TimerError initTimerSeconds(unsigned int seconds, Timer* timer);

/**
 * Gets the remaining time on the timer.
 * @param timer The timer.
 * @param remaining A pointer to where remaining will be written.
 * @param remaining Output parameter for the remaining time (seconds and nanoseconds).
 * @return an error code (TIMER_OK if no error).
 */ 
TimerError remainingTime(const Timer* timer, struct timespec *remaining);

/**
 * Gets the remaining seconds on the timer (just the seconds from the remianing time).
 * @param timer The timer.
 * @param remainingSeconds Output parameter for the remaining seconds.
 * @return an error code (TIMER_OK if no error).
 */
TimerError getRemainingSeconds(const Timer* timer, int *remainingSeconds);

/**
 * Gets the remaining minutes on the timer (just the minutes from the remianing time).
 * @param timer The timer.
 * @param remainingSeconds Output parameter for the remaining minutes.
 * @return an error code (TIMER_OK if no error).
 */
TimerError getRemainingMinutes(const Timer* timer, int *remainingMinutes);

/**
 * Gets the percentage of time that has already passed from the timer.
 * @param timer The timer.
 * @param percentage Output parameter for the percentage of time that has already passed.
 * @return an error code (TIMER_OK if no error).
 */
TimerError getPercentage(const Timer* timer, int* percentage);

/**
 * Pauses the timer if unpaused, unpauses if paused
 * @param timer The timer.
 * @return an error code (TIMER_OK if no error).
 */
TimerError togglePauseTimer(Timer* timer);

/**
 * Pauses the timer.
 * @param timer The timer.
 * @return an error code (TIMER_OK if no error).
 */
TimerError pauseTimer(Timer* timer);

/**
 * Resumes the timer after.
 * @param timer The timer.
 * @return an error code (TIMER_OK if no error).
 */
TimerError resumeTimer(Timer* timer);

/**
 * Weather or not the timer is paused.
 * @param timer The timer.
 * @return 1 if paused, 0 otherwise.
 */
int isTimerPaused(const Timer* timer);

#endif // !TIMER_H
