#include "timer.h"

#ifndef POMODORO_H
#define POMODORO_H

typedef enum {
    FOCUS,
    SHORT_BREAK,
    LONG_BREAK,
} PomodoroPhase;

typedef struct {
    Timer timer;

    int cycleNum;
    PomodoroPhase phase;

} Pomodoro;

Pomodoro initPomodoro();
void nextPomodoro(Pomodoro *pomodoro);
void pausePomodoro(Pomodoro *pomodoro);
void resumePomodoro(Pomodoro *pomodoro);
void togglePausePomodoro(Pomodoro *pomodoro);

int getTimerPercentage(const Pomodoro *pomodoro);
int getRemainingCycleMinutes(const Pomodoro *pomodoro);
int getRemainingCycleSeconds(const Pomodoro *pomodoro);
int isPomodoroPaused(const Pomodoro *pomodoro);
int getCycleNum(const Pomodoro *pomodoro);

int isPomodoroCycleFinished(const Pomodoro *pomodoro);

#endif // !POMODORO_H
#define POMODORO_H
