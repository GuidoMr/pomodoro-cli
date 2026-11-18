#include "../include/pomodoro.h"
#include "../include/timer.h"

#define FOCUS_TIME 20
#define SHORT_BREAK_TIME 5
#define LONG_BREAK_TIME 10

Pomodoro initPomodoro() { 
    Pomodoro pomodoro = { 
	.timer = initTimer(FOCUS_TIME, 0),
	.cycleNum = 0,
	.phase = FOCUS,
    };
    return pomodoro;
}

void startFocus(Pomodoro *pomodoro) {
    pomodoro->phase = FOCUS;
    resetTimer(FOCUS_TIME, 0, &pomodoro->timer);
}

void startShortBreak(Pomodoro *pomodoro) {
    pomodoro->phase = SHORT_BREAK;
    resetTimer(SHORT_BREAK_TIME, 0, &pomodoro->timer);
}

void startLongBreak(Pomodoro *pomodoro) {
    pomodoro->phase = LONG_BREAK;
    resetTimer(LONG_BREAK_TIME, 0, &pomodoro->timer);
}

void nextPomodoro(Pomodoro *pomodoro) {
    switch (pomodoro->phase) {
	case FOCUS:
	    if (pomodoro->cycleNum == 3) {
		pomodoro -> cycleNum++;
		startLongBreak(pomodoro);
	    } else {
		startShortBreak(pomodoro);
	    }
	    break;

	case SHORT_BREAK:
	    pomodoro->cycleNum++;
	    startFocus(pomodoro);
	    break;

	case LONG_BREAK:
	    pomodoro->cycleNum = 0;
	    startFocus(pomodoro);
	    break;
    }
}

void togglePausePomodoro(Pomodoro *pomodoro) {
    togglePauseTimer(&pomodoro -> timer);
}
PomodoroPhase getPhase(const Pomodoro *pomodoro) { 
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

