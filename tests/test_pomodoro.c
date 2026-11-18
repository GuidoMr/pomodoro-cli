#include <assert.h>
#include <stdio.h>
#include "../include/pomodoro.h"

void testInitPomodoro() {
    Pomodoro pomodoro = initPomodoro();
    assert(pomodoro.timer.startSeconds == 20*60);
    assert(pomodoro.cycleNum == 0);
    assert(pomodoro.phase == FOCUS);
}

void testNextPomodoro() {
    Pomodoro pomodoro = initPomodoro();
    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.startSeconds == 5*60);
    assert(pomodoro.cycleNum == 0);
    assert(pomodoro.phase == SHORT_BREAK);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.startSeconds == 20 * 60);
    assert(pomodoro.cycleNum == 1);
    assert(pomodoro.phase == FOCUS);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.startSeconds == 5*60);
    assert(pomodoro.cycleNum == 1);
    assert(pomodoro.phase == SHORT_BREAK);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.startSeconds == 20 * 60);
    assert(pomodoro.cycleNum == 2);
    assert(pomodoro.phase == FOCUS);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.startSeconds == 5*60);
    assert(pomodoro.cycleNum == 2);
    assert(pomodoro.phase == SHORT_BREAK);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.startSeconds == 20 * 60);
    assert(pomodoro.cycleNum == 3);
    assert(pomodoro.phase == FOCUS);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.startSeconds == 10*60);
    assert(pomodoro.cycleNum == 4);
    assert(pomodoro.phase == LONG_BREAK);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.startSeconds == 20 * 60);
    assert(pomodoro.cycleNum == 0);
    assert(pomodoro.phase == FOCUS);

    nextPomodoro(&pomodoro);
    assert(pomodoro.timer.startSeconds == 5 * 60);
    assert(pomodoro.cycleNum == 0);
    assert(pomodoro.phase == SHORT_BREAK);
}

void testTogglePausePomodoro(void) {
    Pomodoro pomodoro = initPomodoro();

    assert(pomodoro.timer.state == TIMER_RUNNING);

    togglePausePomodoro(&pomodoro);
    assert(pomodoro.timer.state == TIMER_PAUSED);

    togglePausePomodoro(&pomodoro);
    assert(pomodoro.timer.state == TIMER_RUNNING);
}

// other functions are simple getters.

int main() {
    testInitPomodoro();
    testNextPomodoro();
    testTogglePausePomodoro();
    printf("All pomodoro tests passed!\n");
} 
