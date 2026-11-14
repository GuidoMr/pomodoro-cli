#include<stdio.h>
#include"../include/timer.h"
#include "../include/pomodoro.h"

int seconds;

int main(void) {
    printf("\033[2J");     // Clear screen
    printf("\033[H");      // Move cursor to top-left
    printf("\033[10;5H");  // Move to row 10, column 5
    Pomodoro pomodoro = initPomodoro();		
    while(1) {
	printf("\r%d:%d\n", getRemainingCycleMinutes(&pomodoro), getRemainingCycleSeconds(&pomodoro));
	printf("\r%d%%", getTimerPercentage(&pomodoro));
	fflush(stdout);
    }

    return 0;
}

