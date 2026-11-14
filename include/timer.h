#include <stddef.h>
#include <time.h>
#ifndef TIMER_H
#define TIMER_H

typedef enum {
    TIMER_IDLE,
    TIMER_RUNNING,
    TIMER_PAUSED
} TimerState;

typedef struct {
    struct timespec end_time;
    struct timespec remainingTime;
    
    int start_seconds;
    TimerState state;
} Timer;

Timer initTimer(int minutes, int seconds);
Timer initTimerSeconds(int seconds);
void resetTimer(int minutes, int seconds, Timer* timer);
void resetTimerSeconds(int seconds, Timer* timer);

struct timespec remainingTime(const Timer* timer);
int getRemainingSeconds(const Timer* timer);
int getRemainingMinutes(const Timer* timer);
int getPercentage(const Timer* timer);

void pauseTimer(Timer* timer);
void resumeTimer(Timer* timer);

int isTimerPaused(const Timer* timer);

#endif // !TIMER_H
#define TIMER_H
