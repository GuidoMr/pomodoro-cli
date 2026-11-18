#include "../include/timer.h"
#include <time.h>

Timer initTimer(int minutes, int seconds) {
    int totalSeconds = minutes * 60 + seconds;
    return initTimerSeconds(totalSeconds);
}


Timer initTimerSeconds(int seconds) { 
    Timer timer;
    resetTimerSeconds(seconds, &timer);
    return timer;
}

void resetTimer(int minutes, int seconds, Timer *timer) {
    int totalSeconds = minutes * 60 + seconds;

    resetTimerSeconds(totalSeconds, timer);
}

// Returns the current time.
struct timespec getNow() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now;
}

void resetTimerSeconds(int seconds, Timer *timer) {
    timer -> startSeconds = seconds;
    timer -> state = TIMER_RUNNING;

    timer -> endTime.tv_sec = getNow().tv_sec + seconds;
    timer -> endTime.tv_nsec = getNow().tv_nsec;
}

struct timespec remainingTime(const Timer *timer) {
    struct timespec remaining;
    if(timer->state == TIMER_PAUSED) {
	remaining = timer->remainingTime;
	return remaining;
    }
	
    remaining.tv_sec = timer->endTime.tv_sec - getNow().tv_sec;
    remaining.tv_nsec = timer->endTime.tv_nsec - getNow().tv_nsec;

    if (remaining.tv_nsec >= 1000000000L) {
        remaining.tv_sec++;
        remaining.tv_nsec -= 1000000000L;
    }

    if(remaining.tv_sec == 0 && remaining.tv_nsec == 0L) {
	remaining.tv_sec = 0;
	remaining.tv_nsec = 0;
    }

    return remaining;
}

int getRemainingSeconds(const Timer *timer) { 
    return remainingTime(timer).tv_sec % 60;
}
int getRemainingMinutes(const Timer *timer) {
   return remainingTime(timer).tv_sec / 60;
}
int getPercentage(const Timer *timer) {
    if (timer -> startSeconds <= 0) return 100;
    return ((timer->startSeconds - remainingTime(timer).tv_sec) * 100)
           / timer->startSeconds;
}

void togglePauseTimer(Timer *timer) {
    if(timer -> state == TIMER_PAUSED) resumeTimer(timer);
    else pauseTimer(timer);
}

void pauseTimer(Timer *timer) {
    if(timer -> state == TIMER_PAUSED) {
	return;
    }
    timer -> remainingTime = remainingTime(timer);
    timer -> state = TIMER_PAUSED;
}

void resumeTimer(Timer *timer) {
    if(timer -> state == TIMER_RUNNING) {
	return;
    }
    struct timespec now = getNow();

    timer->endTime.tv_sec =
        now.tv_sec + timer->remainingTime.tv_sec;

    timer->endTime.tv_nsec =
        now.tv_nsec + timer->remainingTime.tv_nsec;

    if (timer->endTime.tv_nsec >= 1000000000L) {
        timer->endTime.tv_sec++;
        timer->endTime.tv_nsec -= 1000000000L;
    }

    timer -> state = TIMER_RUNNING;
}

int isTimerPaused(const Timer *timer) { 
    return (timer -> state == TIMER_PAUSED);
}
