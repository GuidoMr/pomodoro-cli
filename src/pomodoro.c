#include "../include/pomodoro.h"
#include "../include/timer.h"

#define FOCUS_TIME 20
#define SHORT_BREAK_TIME 5
#define LONG_BREAK_TIME 10

static PomodoroError startFocus(Pomodoro *pomodoro) {
    pomodoro->phase = FOCUS;
    if (initTimer(FOCUS_TIME, 0, &pomodoro->timer) != TIMER_OK) return POMODORO_ERROR_TIMER;

    return POMODORO_OK;
}

static PomodoroError startShortBreak(Pomodoro *pomodoro) {
    pomodoro->phase = SHORT_BREAK;
    if (initTimer(SHORT_BREAK_TIME, 0, &pomodoro->timer) != TIMER_OK) return POMODORO_ERROR_TIMER;
    return POMODORO_OK;
}

static PomodoroError startLongBreak(Pomodoro *pomodoro) {
    pomodoro->phase = LONG_BREAK;
    if (initTimer(LONG_BREAK_TIME, 0, &pomodoro->timer) != TIMER_OK) return POMODORO_ERROR_TIMER;
    return POMODORO_OK;
}

PomodoroError initPomodoro(Pomodoro* pomodoro) { 
    if (pomodoro == nullptr) return POMODORO_ERROR_NULL_POINTER;

    Timer timer;
    if(initTimer(FOCUS_TIME, 0, &timer) != TIMER_OK) return POMODORO_ERROR_TIMER;

    pomodoro -> timer = timer;
    pomodoro -> cycleNum = 0;
    pomodoro -> phase = FOCUS;
    pomodoro -> isInitialized = 1;

    return POMODORO_OK;
}

PomodoroError nextPomodoro(Pomodoro *pomodoro) {
    if (pomodoro == nullptr) return POMODORO_ERROR_NULL_POINTER;
    if (!pomodoro -> isInitialized) return POMODORO_ERROR_NOT_INITIALIZED;

    PomodoroError error;
    switch (pomodoro->phase) {
	case FOCUS:
	    if (pomodoro->cycleNum == 3) {
		pomodoro -> cycleNum++;
		error = startLongBreak(pomodoro);
	    } else {
		error = startShortBreak(pomodoro);
	    }
	    break;

	case SHORT_BREAK:
	    pomodoro->cycleNum++;
	    error = startFocus(pomodoro);
	    break;

	case LONG_BREAK:
	    pomodoro->cycleNum = 0;
	    error = startFocus(pomodoro);
	    break;
    }
    return error;
}

PomodoroError togglePausePomodoro(Pomodoro *pomodoro) {
    if (pomodoro == nullptr) return POMODORO_ERROR_NULL_POINTER;
    if (!pomodoro -> isInitialized) return POMODORO_ERROR_NOT_INITIALIZED;

    if (togglePauseTimer(&pomodoro -> timer) != TIMER_OK) return POMODORO_ERROR_TIMER;
    return POMODORO_OK;
}

PomodoroError getTimerPercentage(const Pomodoro *pomodoro, int *timerPercentage) {
    if (pomodoro == nullptr || timerPercentage == nullptr) return POMODORO_ERROR_NULL_POINTER;
    if (!pomodoro -> isInitialized) return POMODORO_ERROR_NOT_INITIALIZED;

    if (getPercentage(&pomodoro->timer, timerPercentage) != TIMER_OK) return POMODORO_ERROR_TIMER;

    return POMODORO_OK;
}

PomodoroError getRemainingPhaseMinutes(const Pomodoro *pomodoro, int *remainingMinutes) {
    if (pomodoro == nullptr || remainingMinutes == nullptr) return POMODORO_ERROR_NULL_POINTER;
    if (!pomodoro -> isInitialized) return POMODORO_ERROR_NOT_INITIALIZED;

    if (getRemainingMinutes(&pomodoro->timer, remainingMinutes) != TIMER_OK) return POMODORO_ERROR_TIMER;

    return POMODORO_OK;
} 

PomodoroError getRemainingPhaseSeconds(const Pomodoro *pomodoro, int *remainingSeconds) {
    if (pomodoro == nullptr || remainingSeconds == nullptr) return POMODORO_ERROR_NULL_POINTER;
    if (!pomodoro -> isInitialized) return POMODORO_ERROR_NOT_INITIALIZED;

    if (getRemainingSeconds(&pomodoro->timer, remainingSeconds) != TIMER_OK) return POMODORO_ERROR_TIMER;

    return POMODORO_OK;
} 

int isPomodoroPaused(const Pomodoro *pomodoro) { 
    return (pomodoro -> timer.state == TIMER_PAUSED);
}

PomodoroPhase getPhase(const Pomodoro *pomodoro) { 
    return pomodoro -> phase;
}

int getCycleNum(const Pomodoro *pomodoro) { return pomodoro->cycleNum; }

PomodoroError isPomodoroPhaseFinished(const Pomodoro *pomodoro, int *isPhaseFinished) {
    if (pomodoro == nullptr || isPhaseFinished == nullptr) return POMODORO_ERROR_NULL_POINTER;
    if (!pomodoro -> isInitialized) return POMODORO_ERROR_NOT_INITIALIZED;

    int seconds;
    int minutes;

    PomodoroError r;
    r = getRemainingPhaseSeconds(pomodoro, &seconds);
    if (r != POMODORO_OK) return r;

    r = getRemainingPhaseMinutes(pomodoro, &minutes);
    if (r != POMODORO_OK) return r;

    *isPhaseFinished = (seconds <= 0 && minutes <= 0);

    return POMODORO_OK;
}

