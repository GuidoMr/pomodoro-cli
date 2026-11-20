#include "../include/timer.h"
#include <time.h>
#include <limits.h>

TimerError initTimer(unsigned int minutes, unsigned int seconds, Timer *timer) {
    if (minutes > (UINT_MAX - seconds) / 60) return TIMER_ERROR_OVERFLOW;
    unsigned int totalSeconds = minutes * 60 + seconds;

    return initTimerSeconds(totalSeconds, timer);
}

TimerError initTimerSeconds(unsigned int seconds, Timer *timer) {
    if (timer == nullptr) return TIMER_ERROR_NULL_POINTER;
    if (seconds > INT_MAX) return TIMER_ERROR_OVERFLOW;

    timer -> startSeconds = seconds;
    timer -> state = TIMER_RUNNING;

    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) return TIMER_ERROR_GETTIME;


    timer -> endTime.tv_sec = now.tv_sec + seconds;
    timer -> endTime.tv_nsec = now.tv_nsec;
    return TIMER_OK;
}

TimerError remainingTime(const Timer *timer, struct timespec *remaining) {
    if (timer == nullptr || remaining == nullptr) return TIMER_ERROR_NULL_POINTER;

    if(timer->state == TIMER_PAUSED) {
	*remaining = timer->remainingTime;
	return TIMER_OK;
    }

    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) return TIMER_ERROR_GETTIME;
	
    remaining -> tv_sec = timer->endTime.tv_sec - now.tv_sec;
    remaining -> tv_nsec = timer->endTime.tv_nsec - now.tv_nsec;

    if (remaining -> tv_nsec < 0) {
        remaining -> tv_sec--;
        remaining -> tv_nsec += 1000000000L;
    }

    if(remaining -> tv_sec < 0) {
	remaining -> tv_sec = 0;
	remaining -> tv_nsec = 0;
    }

    return TIMER_OK;
}

TimerError getRemainingSeconds(const Timer *timer, int *remainingSeconds) { 
    if (remainingSeconds == nullptr) return TIMER_ERROR_NULL_POINTER;

    struct timespec remaining;
    TimerError r = remainingTime(timer, &remaining);
    if (r != TIMER_OK) return r;

    *remainingSeconds = remaining.tv_sec % 60;
    return TIMER_OK;
}
TimerError getRemainingMinutes(const Timer *timer, int *remainingMinutes) {
    if (remainingMinutes == nullptr) return TIMER_ERROR_NULL_POINTER;

    struct timespec remaining;
    TimerError r = remainingTime(timer, &remaining);
    if (r != TIMER_OK) return r;

    *remainingMinutes = remaining.tv_sec / 60;
    return TIMER_OK;
}
TimerError getPercentage(const Timer *timer, int *percentage) {
    if (percentage == nullptr) return TIMER_ERROR_NULL_POINTER;

    struct timespec remaining;
    TimerError r = remainingTime(timer, &remaining);
    if (r != TIMER_OK) return r;

    if (timer -> startSeconds <= 0) *percentage = 100;
    else *percentage = ((timer->startSeconds - remaining.tv_sec) * 100)
           / timer->startSeconds;

    return TIMER_OK;
}

TimerError togglePauseTimer(Timer *timer) {
    if (timer == nullptr) return TIMER_ERROR_NULL_POINTER;

    if(timer -> state == TIMER_PAUSED) resumeTimer(timer);
    else pauseTimer(timer);

    return TIMER_OK;
}

TimerError pauseTimer(Timer *timer) {
    if (timer == nullptr) return TIMER_ERROR_NULL_POINTER;

    if(timer -> state == TIMER_PAUSED) {
	return TIMER_ERROR_ALREADY_PAUSED;
    }
    struct timespec remaining;
    TimerError r = remainingTime(timer, &remaining);
    if (r != TIMER_OK) return r;

    timer -> remainingTime = remaining;
    timer -> state = TIMER_PAUSED;
    return TIMER_OK;
}

TimerError resumeTimer(Timer *timer) {
    if (timer == nullptr) return TIMER_ERROR_NULL_POINTER;

    if(timer -> state == TIMER_RUNNING) {
	return TIMER_ERROR_ALREADY_UNPAUSED;
    }

    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) return TIMER_ERROR_GETTIME;

    timer->endTime.tv_sec =
        now.tv_sec + timer->remainingTime.tv_sec;

    timer->endTime.tv_nsec =
        now.tv_nsec + timer->remainingTime.tv_nsec;

    if (timer->endTime.tv_nsec >= 1000000000L) {
        timer->endTime.tv_sec++;
        timer->endTime.tv_nsec -= 1000000000L;
    }

    timer -> state = TIMER_RUNNING;
    return TIMER_OK;
}

int isTimerPaused(const Timer *timer) { 
    return (timer -> state == TIMER_PAUSED);
}
