#include <assert.h>
#include <stdio.h>
#include "../include/pomodoro.h"

void testInitPomodoro(void) {
    Pomodoro pomodoro;

    assert(initPomodoro(&pomodoro) == POMODORO_OK);

    assert(pomodoro.timer.startSeconds == 20 * 60);
    assert(pomodoro.cycleNum == 0);
    assert(pomodoro.phase == FOCUS);
    assert(pomodoro.isInitialized == 1);
    assert(pomodoro.timer.state == TIMER_RUNNING);
}

void testNextPomodoro(void) {
    Pomodoro pomodoro;

    assert(initPomodoro(&pomodoro) == POMODORO_OK);

    // Focus -> Short break
    assert(nextPomodoro(&pomodoro) == POMODORO_OK);
    assert(pomodoro.timer.startSeconds == 5 * 60);
    assert(pomodoro.cycleNum == 0);
    assert(pomodoro.phase == SHORT_BREAK);

    // Short break -> Focus (cycle 1)
    assert(nextPomodoro(&pomodoro) == POMODORO_OK);
    assert(pomodoro.timer.startSeconds == 20 * 60);
    assert(pomodoro.cycleNum == 1);
    assert(pomodoro.phase == FOCUS);

    // Focus -> Short break
    assert(nextPomodoro(&pomodoro) == POMODORO_OK);
    assert(pomodoro.timer.startSeconds == 5 * 60);
    assert(pomodoro.cycleNum == 1);
    assert(pomodoro.phase == SHORT_BREAK);

    // Short break -> Focus (cycle 2)
    assert(nextPomodoro(&pomodoro) == POMODORO_OK);
    assert(pomodoro.timer.startSeconds == 20 * 60);
    assert(pomodoro.cycleNum == 2);
    assert(pomodoro.phase == FOCUS);

    // Focus -> Short break
    assert(nextPomodoro(&pomodoro) == POMODORO_OK);
    assert(pomodoro.timer.startSeconds == 5 * 60);
    assert(pomodoro.cycleNum == 2);
    assert(pomodoro.phase == SHORT_BREAK);

    // Short break -> Focus (cycle 3)
    assert(nextPomodoro(&pomodoro) == POMODORO_OK);
    assert(pomodoro.timer.startSeconds == 20 * 60);
    assert(pomodoro.cycleNum == 3);
    assert(pomodoro.phase == FOCUS);

    // Focus -> Long break
    assert(nextPomodoro(&pomodoro) == POMODORO_OK);
    assert(pomodoro.timer.startSeconds == 10 * 60);
    assert(pomodoro.cycleNum == 4);
    assert(pomodoro.phase == LONG_BREAK);

    // Long break -> Focus (cycle resets)
    assert(nextPomodoro(&pomodoro) == POMODORO_OK);
    assert(pomodoro.timer.startSeconds == 20 * 60);
    assert(pomodoro.cycleNum == 0);
    assert(pomodoro.phase == FOCUS);
}

void testTogglePausePomodoro(void) {
    Pomodoro pomodoro;

    assert(initPomodoro(&pomodoro) == POMODORO_OK);

    assert(pomodoro.timer.state == TIMER_RUNNING);

    assert(togglePausePomodoro(&pomodoro) == POMODORO_OK);
    assert(pomodoro.timer.state == TIMER_PAUSED);

    assert(togglePausePomodoro(&pomodoro) == POMODORO_OK);
    assert(pomodoro.timer.state == TIMER_RUNNING);
}

void testGetPhase(void) {
    Pomodoro pomodoro;

    assert(initPomodoro(&pomodoro) == POMODORO_OK);

    assert(getPhase(&pomodoro) == FOCUS);

    assert(nextPomodoro(&pomodoro) == POMODORO_OK);
    assert(getPhase(&pomodoro) == SHORT_BREAK);

    assert(nextPomodoro(&pomodoro) == POMODORO_OK);
    assert(getPhase(&pomodoro) == FOCUS);
}

void testGetCycleNum(void) {
    Pomodoro pomodoro;

    assert(initPomodoro(&pomodoro) == POMODORO_OK);

    assert(getCycleNum(&pomodoro) == 0);

    assert(nextPomodoro(&pomodoro) == POMODORO_OK);
    assert(nextPomodoro(&pomodoro) == POMODORO_OK);

    assert(getCycleNum(&pomodoro) == 1);
}

void testGetTimerPercentage(void) {
    Pomodoro pomodoro;
    int percentage;

    assert(initPomodoro(&pomodoro) == POMODORO_OK);

    // Manually simulate a paused timer.
    pomodoro.timer.state = TIMER_PAUSED;
    pomodoro.timer.remainingTime.tv_sec = 10 * 60;
    pomodoro.timer.remainingTime.tv_nsec = 0;

    assert(getTimerPercentage(&pomodoro, &percentage) == POMODORO_OK);
    assert(percentage == 50);

    pomodoro.timer.remainingTime.tv_sec = 5 * 60;

    assert(getTimerPercentage(&pomodoro, &percentage) == POMODORO_OK);
    assert(percentage == 75);

    pomodoro.timer.remainingTime.tv_sec = 0;

    assert(getTimerPercentage(&pomodoro, &percentage) == POMODORO_OK);
    assert(percentage == 100);
}

void testGetRemainingPhaseMinutes(void) {
    Pomodoro pomodoro;
    int minutes;

    assert(initPomodoro(&pomodoro) == POMODORO_OK);

    pomodoro.timer.state = TIMER_PAUSED;
    pomodoro.timer.remainingTime.tv_sec = 15 * 60;
    pomodoro.timer.remainingTime.tv_nsec = 0;

    assert(getRemainingPhaseMinutes(&pomodoro, &minutes) == POMODORO_OK);
    assert(minutes == 15);

    assert(nextPomodoro(&pomodoro) == POMODORO_OK);

    pomodoro.timer.remainingTime.tv_sec = 4 * 60;
    assert(getRemainingPhaseMinutes(&pomodoro, &minutes) == POMODORO_OK);
    assert(minutes == 4);
}

void testGetRemainingPhaseSeconds(void) {
    Pomodoro pomodoro;
    int seconds;

    assert(initPomodoro(&pomodoro) == POMODORO_OK);

    pomodoro.timer.state = TIMER_PAUSED;
    pomodoro.timer.remainingTime.tv_sec = 125;
    pomodoro.timer.remainingTime.tv_nsec = 0;

    assert(getRemainingPhaseSeconds(&pomodoro, &seconds) == POMODORO_OK);
    assert(seconds == 5);
}

void testIsPomodoroPhaseFinished(void) {
    Pomodoro pomodoro;
    int finished;

    assert(initPomodoro(&pomodoro) == POMODORO_OK);

    pomodoro.timer.state = TIMER_PAUSED;
    pomodoro.timer.remainingTime.tv_sec = 10;
    pomodoro.timer.remainingTime.tv_nsec = 0;

    assert(isPomodoroPhaseFinished(&pomodoro, &finished) == POMODORO_OK);
    assert(finished == 0);

    pomodoro.timer.remainingTime.tv_sec = 0;

    assert(isPomodoroPhaseFinished(&pomodoro, &finished) == POMODORO_OK);
    assert(finished == 1);
}

void testNullPointers(void) {
    Pomodoro pomodoro;
    int value;

    assert(initPomodoro(NULL) == POMODORO_ERROR_NULL_POINTER);

    assert(nextPomodoro(NULL) == POMODORO_ERROR_NULL_POINTER);
    assert(togglePausePomodoro(NULL) == POMODORO_ERROR_NULL_POINTER);

    assert(getTimerPercentage(NULL, &value) == POMODORO_ERROR_NULL_POINTER);
    assert(getTimerPercentage(&pomodoro, NULL) == POMODORO_ERROR_NULL_POINTER);

    assert(getRemainingPhaseMinutes(NULL, &value) == POMODORO_ERROR_NULL_POINTER);
    assert(getRemainingPhaseMinutes(&pomodoro, NULL) == POMODORO_ERROR_NULL_POINTER);

    assert(getRemainingPhaseSeconds(NULL, &value) == POMODORO_ERROR_NULL_POINTER);
    assert(getRemainingPhaseSeconds(&pomodoro, NULL) == POMODORO_ERROR_NULL_POINTER);

    assert(isPomodoroPhaseFinished(NULL, &value) == POMODORO_ERROR_NULL_POINTER);
    assert(isPomodoroPhaseFinished(&pomodoro, NULL) == POMODORO_ERROR_NULL_POINTER);
}

void testUninitializedPomodoro(void) {
    Pomodoro pomodoro = {0};
    int value;

    assert(nextPomodoro(&pomodoro) == POMODORO_ERROR_NOT_INITIALIZED);
    assert(togglePausePomodoro(&pomodoro) == POMODORO_ERROR_NOT_INITIALIZED);

    assert(getTimerPercentage(&pomodoro, &value) == POMODORO_ERROR_NOT_INITIALIZED);
    assert(getRemainingPhaseMinutes(&pomodoro, &value) == POMODORO_ERROR_NOT_INITIALIZED);
    assert(getRemainingPhaseSeconds(&pomodoro, &value) == POMODORO_ERROR_NOT_INITIALIZED);
    assert(isPomodoroPhaseFinished(&pomodoro, &value) == POMODORO_ERROR_NOT_INITIALIZED);
}

void testIsPomodoroPaused(void) {
    Pomodoro pomodoro;

    assert(initPomodoro(&pomodoro) == POMODORO_OK);

    assert(isPomodoroPaused(&pomodoro) == 0);

    assert(togglePausePomodoro(&pomodoro) == POMODORO_OK);
    assert(isPomodoroPaused(&pomodoro) == 1);

    assert(togglePausePomodoro(&pomodoro) == POMODORO_OK);
    assert(isPomodoroPaused(&pomodoro) == 0);
}

int main(void) {
    testInitPomodoro();
    testNextPomodoro();
    testTogglePausePomodoro();

    testGetPhase();
    testGetCycleNum();

    testGetTimerPercentage();
    testGetRemainingPhaseMinutes();
    testGetRemainingPhaseSeconds();
    testIsPomodoroPhaseFinished();

    testNullPointers();
    testUninitializedPomodoro();
    testIsPomodoroPaused();

    printf("All pomodoro tests passed!\n");
    return 0;
}
