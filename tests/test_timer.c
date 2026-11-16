#include "../include/timer.h"
#include <assert.h>
#include <stdio.h>

void testInitTimer() {
    Timer timer = initTimer(25, 30);

    assert(timer.start_seconds == 1530);
}

void testInitTimerSeconds() {
    Timer timer = initTimerSeconds(90);

    assert(timer.start_seconds == 90);
}
void testResetTimer() {
    Timer timer = initTimer(60, 0);

    resetTimer(25, 0, &timer);

    assert(timer.start_seconds == 1500);
}

void testResetTimerSeconds() {
    Timer timer = initTimer(60, 0);

    resetTimerSeconds(120, &timer);

    assert(timer.start_seconds == 120);
}

void testPauseTimer(void) {
    Timer timer = initTimerSeconds(30);

    pauseTimer(&timer);

    assert(timer.state == TIMER_PAUSED);
    assert(timer.remainingTime.tv_sec >= 29);
    assert(timer.remainingTime.tv_sec <= 30);
}

void testResumeTimer(void) {
    Timer timer = initTimerSeconds(30);

    timer.remainingTime.tv_sec = 15;
    timer.remainingTime.tv_nsec = 0;
    timer.state = TIMER_PAUSED;

    resumeTimer(&timer);

    assert(timer.state == TIMER_RUNNING);
    assert(timer.remainingTime.tv_sec >= 14);
    assert(timer.remainingTime.tv_sec <= 15);
}

void testPercentage() {
    Timer timer = {
	.start_seconds = 100,
	.remainingTime = {50, 0},
	.state = TIMER_PAUSED,
    };
    assert(getPercentage(&timer) == 50);

    timer.remainingTime.tv_sec = 75;
    assert(getPercentage(&timer) == 25);

    timer.remainingTime.tv_sec = 0;
    assert(getPercentage(&timer) == 100);
}

void testRemainingTime(void) {
    Timer timer = initTimerSeconds(60);

    struct timespec remaining = remainingTime(&timer);

    assert(remaining.tv_sec >= 59);
    assert(remaining.tv_sec <= 60);

    // Paused timer should return its stored remaining time.
    timer.state = TIMER_PAUSED;
    timer.remainingTime.tv_sec = 25;
    timer.remainingTime.tv_nsec = 500000000;

    remaining = remainingTime(&timer);

    assert(remaining.tv_sec == 25);
    assert(remaining.tv_nsec == 500000000);
}

void testTogglePauseTimer(void) {
    Timer timer = initTimerSeconds(60);

    assert(timer.state == TIMER_RUNNING);

    togglePauseTimer(&timer);
    assert(timer.state == TIMER_PAUSED);

    togglePauseTimer(&timer);
    assert(timer.state == TIMER_RUNNING);

    togglePauseTimer(&timer);
    assert(timer.state == TIMER_PAUSED);
}

// Other functions are simple getters.

int main(void) {
    testInitTimer();
    testInitTimerSeconds();
    testResetTimer();

    testPauseTimer();
    testResumeTimer();

    testPercentage();

    testRemainingTime();
    testTogglePauseTimer();

    printf("All tests passed!\n");
    return 0;
}
