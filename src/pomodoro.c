#include "../include/pomodoro.h"
#include "../include/timer.h"

Pomodoro initPomodoro() { 
    Pomodoro pomodoro = { 
	.timer = initTimer(20, 0),
	.cycleNum = 0,
	.phase = FOCUS,
    };
    return pomodoro;
}

void startFocus(Pomodoro *pomodoro) {
    resetTimer(20, 0, &pomodoro->timer);
}

void startShortBreak(Pomodoro *pomodoro) {
    resetTimer(5, 0, &pomodoro->timer);
}

void startLongBreak(Pomodoro *pomodoro) {
    resetTimer(10, 0, &pomodoro->timer);
}

void nextPomodoro(Pomodoro *pomodoro) {
    if (pomodoro -> cycleNum == 4) {
	pomodoro -> cycleNum = 0;
	pomodoro -> phase = FOCUS;
	startFocus(pomodoro);
	return;
    }
    if(pomodoro -> phase == SHORT_BREAK) {
	pomodoro -> cycleNum += 1;
	pomodoro -> phase = FOCUS;
	startFocus(pomodoro);
	return;
    }

    if (pomodoro -> cycleNum == 3) {
	pomodoro -> cycleNum = 4;
	pomodoro -> phase = LONG_BREAK;
	startLongBreak(pomodoro);
	return;
    }

    pomodoro -> phase = SHORT_BREAK;
    startShortBreak(pomodoro);
}

void togglePausePomodoro(Pomodoro *pomodoro) {
    togglePauseTimer(&pomodoro -> timer);
}
int getPhase(const Pomodoro *pomodoro) { 
    return pomodoro -> phase;
}

int getTimerPercentage(const Pomodoro *pomodoro) {
    return getPercentage(&pomodoro->timer);
}

int getRemainingPhaseMinutes(const Pomodoro *pomodoro) {
    return getRemainingMinutes(&pomodoro->timer);
} 

int getRemainingPhaseSeconds(const Pomodoro *pomodoro) {
    return getRemainingSeconds(&pomodoro->timer);
}

int isPomodoroPaused(const Pomodoro *pomodoro) { 
    return (pomodoro -> timer.state == TIMER_PAUSED);
}

int getCycleNum(const Pomodoro *pomodoro) { return pomodoro->cycleNum; }

int isPomodoroPhaseFinished(const Pomodoro *pomodoro) {
    return (getRemainingPhaseSeconds(pomodoro) <= 0 && getRemainingPhaseMinutes(pomodoro) <= 0);
}

int isPomodoroFinished(const Pomodoro *pomodoro) { 
    return (getRemainingSeconds(&pomodoro -> timer) <= 0 && getRemainingMinutes(&pomodoro -> timer) <= 0);
}

int getCycleCount(const Pomodoro *pomodoro) { 
    return pomodoro -> cycleNum;
}
