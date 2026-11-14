#include <assert.h>
#include <stdio.h>
#include "../include/pomodoro.h"

void testInitPomodoro() {
    Pomodoro pomodoro = initPomodoro();
    assert(pomodoro.timer.start_seconds == 20*60);
    assert(pomodoro.cycleNum == 0);
    assert(pomodoro.phase == FOCUS);
}

void testNextPomodoro() {
    Pomodoro pomodoro = initPomodoro();
    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.start_seconds == 5*60);
    assert(pomodoro.cycleNum == 0);
    assert(pomodoro.phase == SHORT_BREAK);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.start_seconds == 20 * 60);
    assert(pomodoro.cycleNum == 1);
    assert(pomodoro.phase == FOCUS);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.start_seconds == 5*60);
    assert(pomodoro.cycleNum == 1);
    assert(pomodoro.phase == SHORT_BREAK);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.start_seconds == 20 * 60);
    assert(pomodoro.cycleNum == 2);
    assert(pomodoro.phase == FOCUS);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.start_seconds == 5*60);
    assert(pomodoro.cycleNum == 2);
    assert(pomodoro.phase == SHORT_BREAK);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.start_seconds == 20 * 60);
    assert(pomodoro.cycleNum == 3);
    assert(pomodoro.phase == FOCUS);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.start_seconds == 10*60);
    assert(pomodoro.cycleNum == 4);
    assert(pomodoro.phase == LONG_BREAK);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.start_seconds == 20 * 60);
    assert(pomodoro.cycleNum == 0);
    assert(pomodoro.phase == FOCUS);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.start_seconds == 5 * 60);
    assert(pomodoro.cycleNum == 0);
    assert(pomodoro.phase == SHORT_BREAK);
}

void testPausePomodoro() {
    Pomodoro pomodoro = initPomodoro();

    pausePomodoro(&pomodoro);

    assert(pomodoro.timer.state == TIMER_PAUSED);
}

void testResumePomodoro() {
    Pomodoro pomodoro = initPomodoro();

    pausePomodoro(&pomodoro);
    resumePomodoro(&pomodoro);

    assert(pomodoro.timer.state == TIMER_RUNNING);
}

// other functions are simple getters.

int main() {
    testInitPomodoro();
    testNextPomodoro();
    testPausePomodoro();
    testResumePomodoro();
    printf("All pomodoro tests passed!\n");
} 
