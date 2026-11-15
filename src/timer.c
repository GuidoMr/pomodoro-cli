#include "../include/timer.h"
#include <bits/time.h>
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
    timer -> start_seconds = seconds;
    timer -> state = TIMER_RUNNING;

    timer -> end_time.tv_sec = getNow().tv_sec + seconds;
    timer -> end_time.tv_nsec = 0;
}

struct timespec remainingTime(const Timer *timer) {
    struct timespec remaining;
    if(timer->state == TIMER_PAUSED) {
	remaining = timer->remainingTime;
	return remaining;
    }
	
    remaining.tv_sec = timer->end_time.tv_sec - getNow().tv_sec;
    remaining.tv_nsec = timer->end_time.tv_nsec - getNow().tv_nsec;

    if (remaining.tv_nsec >= 1000000000L) {
        remaining.tv_sec++;
        remaining.tv_nsec -= 1000000000L;
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
    return ((timer->start_seconds - remainingTime(timer).tv_sec) * 100)
           / timer->start_seconds;
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

    timer->end_time.tv_sec =
        now.tv_sec + timer->remainingTime.tv_sec;

    timer->end_time.tv_nsec =
        now.tv_nsec + timer->remainingTime.tv_nsec;

    if (timer->end_time.tv_nsec >= 1000000000L) {
        timer->end_time.tv_sec++;
        timer->end_time.tv_nsec -= 1000000000L;
    }

    timer -> state = TIMER_RUNNING;
}

int isTimerPaused(const Timer *timer) { 
    return (timer -> state == TIMER_PAUSED);
}
