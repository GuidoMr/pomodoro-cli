#include "../include/timer.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>

void testInitTimer(void) {
    Timer timer;

    assert(initTimer(25, 30, &timer) == TIMER_OK);
    assert(timer.startSeconds == 1530);
    assert(timer.state == TIMER_RUNNING);

    assert(initTimer(UINT_MAX, 60, &timer) == TIMER_ERROR_OVERFLOW);
}

void testInitTimerSeconds(void) {
    Timer timer;

    assert(initTimerSeconds(90, &timer) == TIMER_OK);
    assert(timer.startSeconds == 90);
    assert(timer.state == TIMER_RUNNING);

    assert(initTimerSeconds(UINT_MAX, &timer) == TIMER_ERROR_OVERFLOW);
}

void testInitNullPointer(void) {
    assert(initTimer(25, 30, NULL) == TIMER_ERROR_NULL_POINTER);
    assert(initTimerSeconds(90, NULL) == TIMER_ERROR_NULL_POINTER);
}

void testPauseTimer(void) {
    Timer timer;

    assert(initTimerSeconds(30, &timer) == TIMER_OK);
    assert(pauseTimer(&timer) == TIMER_OK);

    assert(timer.state == TIMER_PAUSED);
    assert(timer.remainingTime.tv_sec >= 29);
    assert(timer.remainingTime.tv_sec <= 30);
}

void testPauseAlreadyPaused(void) {
    Timer timer;

    assert(initTimerSeconds(30, &timer) == TIMER_OK);
    assert(pauseTimer(&timer) == TIMER_OK);

    assert(pauseTimer(&timer) == TIMER_ERROR_ALREADY_PAUSED);
}

void testResumeTimer(void) {
    Timer timer;

    assert(initTimerSeconds(30, &timer) == TIMER_OK);
    assert(pauseTimer(&timer) == TIMER_OK);

    assert(resumeTimer(&timer) == TIMER_OK);
    assert(timer.state == TIMER_RUNNING);
}

void testResumeAlreadyRunning(void) {
    Timer timer;

    assert(initTimerSeconds(30, &timer) == TIMER_OK);

    assert(resumeTimer(&timer) == TIMER_ERROR_ALREADY_UNPAUSED);
}

void testPercentage(void) {
    Timer timer = {
        .startSeconds = 100,
        .remainingTime = {50, 0},
        .state = TIMER_PAUSED,
    };

    int percentage;

    assert(getPercentage(&timer, &percentage) == TIMER_OK);
    assert(percentage == 50);

    timer.remainingTime.tv_sec = 75;

    assert(getPercentage(&timer, &percentage) == TIMER_OK);
    assert(percentage == 25);

    timer.remainingTime.tv_sec = 0;

    assert(getPercentage(&timer, &percentage) == TIMER_OK);
    assert(percentage == 100);
}

void testRemainingTime(void) {
    Timer timer;
    struct timespec remaining;

    assert(initTimerSeconds(60, &timer) == TIMER_OK);

    assert(remainingTime(&timer, &remaining) == TIMER_OK);

    assert(remaining.tv_sec >= 59);
    assert(remaining.tv_sec <= 60);

    // Paused timer should return its stored remaining time.
    timer.state = TIMER_PAUSED;
    timer.remainingTime.tv_sec = 25;
    timer.remainingTime.tv_nsec = 500000000;

    assert(remainingTime(&timer, &remaining) == TIMER_OK);

    assert(remaining.tv_sec == 25);
    assert(remaining.tv_nsec == 500000000);
}

void testTogglePauseTimer(void) {
    Timer timer;

    assert(initTimerSeconds(60, &timer) == TIMER_OK);
    assert(timer.state == TIMER_RUNNING);

    assert(togglePauseTimer(&timer) == TIMER_OK);
    assert(timer.state == TIMER_PAUSED);

    assert(togglePauseTimer(&timer) == TIMER_OK);
    assert(timer.state == TIMER_RUNNING);

    assert(togglePauseTimer(&timer) == TIMER_OK);
    assert(timer.state == TIMER_PAUSED);
}

void testNullPointerGetters(void) {
    struct timespec remaining;
    int value;

    assert(remainingTime(NULL, &remaining) == TIMER_ERROR_NULL_POINTER);
    assert(remainingTime(NULL, NULL) == TIMER_ERROR_NULL_POINTER);

    assert(getRemainingSeconds(NULL, &value) == TIMER_ERROR_NULL_POINTER);
    assert(getRemainingSeconds(NULL, NULL) == TIMER_ERROR_NULL_POINTER);

    assert(getRemainingMinutes(NULL, &value) == TIMER_ERROR_NULL_POINTER);
    assert(getRemainingMinutes(NULL, NULL) == TIMER_ERROR_NULL_POINTER);

    assert(getPercentage(NULL, &value) == TIMER_ERROR_NULL_POINTER);
    assert(getPercentage(NULL, NULL) == TIMER_ERROR_NULL_POINTER);

    assert(pauseTimer(NULL) == TIMER_ERROR_NULL_POINTER);
    assert(resumeTimer(NULL) == TIMER_ERROR_NULL_POINTER);
    assert(togglePauseTimer(NULL) == TIMER_ERROR_NULL_POINTER);
}

void testGetRemainingSecondsAndMinutes(void) {
    Timer timer = {
        .startSeconds = 3665,
        .remainingTime = {3665, 0},
        .state = TIMER_PAUSED,
    };

    int seconds;
    int minutes;

    assert(getRemainingSeconds(&timer, &seconds) == TIMER_OK);
    assert(seconds == 5);

    assert(getRemainingMinutes(&timer, &minutes) == TIMER_OK);
    assert(minutes == 61);
}

void testGetterNullOutputs(void) {
    Timer timer;

    assert(initTimerSeconds(60, &timer) == TIMER_OK);

    assert(getRemainingSeconds(&timer, NULL) == TIMER_ERROR_NULL_POINTER);
    assert(getRemainingMinutes(&timer, NULL) == TIMER_ERROR_NULL_POINTER);
    assert(getPercentage(&timer, NULL) == TIMER_ERROR_NULL_POINTER);
}

int main(void) {
    testInitTimer();
    testInitTimerSeconds();
    testInitNullPointer();

    testPauseTimer();
    testPauseAlreadyPaused();
    testResumeTimer();
    testResumeAlreadyRunning();

    testPercentage();
    testRemainingTime();
    testTogglePauseTimer();

    testNullPointerGetters();
    testGetRemainingSecondsAndMinutes();
    testGetterNullOutputs();

    printf("All timer tests passed!\n");
    return 0;
}

