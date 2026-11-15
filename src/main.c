#include <asm-generic/errno.h>
#include<stdio.h>
#include"../include/timer.h"
#include "../include/pomodoro.h"
#include <termios.h>
#include <unistd.h>
#include <poll.h>

struct termios original;

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &original);

    struct termios raw = original;

    raw.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
}


int seconds;

int main(void) {
    struct pollfd fds = {
	.fd = STDIN_FILENO,
	.events = POLLIN
    };

    enableRawMode();
    printf("\033[2J");     // Clear screen
    printf("\033[?25l");   // Hide cursor
    Pomodoro pomodoro = initPomodoro();		
    int isRunning = 1;
    while(isRunning) {
	printf("\033[H"); // Move cursor to top-left
	printf("|      POMODORO       |\n");
	printf("| P - Pause/Unpause        |\n");
	printf("| N - Next                 |\n");
        printf("| Q - Quit                 |\n");

	printf("---\n");
	for (int i = 0; i < 10; i++) {
	    if (i < getTimerPercentage(&pomodoro) / 10) printf("=");
	    else printf("-");
	}
	printf("\n---\n");

	if(isPomodoroCycleFinished(&pomodoro)) printf("00:00\n");
	else printf("%02d:%02d\n", getRemainingCycleMinutes(&pomodoro), getRemainingCycleSeconds(&pomodoro));

	printf("                                                                                        ");
	switch(getPhase(&pomodoro)) {
	    case 0:
		printf("\rFOCUS\n");
		break;
	    case 1:
		printf("\rSHORT BREAK\n");
		break;
	    case 2:
		printf("\rLONG BREAK\n");
	}

	fflush(stdout);

	int result = poll(&fds, 1, 100);
	if (result > 0 && (fds.revents & POLLIN)) {
	    char c;
	    read(STDIN_FILENO, &c, 1);
	    switch(c) {
		case 'p':
		    togglePausePomodoro(&pomodoro);
		    break;
		case 'n':
		    nextPomodoro(&pomodoro);
		    break;
		case 'q':
		    isRunning = 0;
		    break;
	    }
	}
    }

    disable_raw_mode();
    printf("\033[2J");     // Clear screen
    printf("\033[?25h");   // Show cursor

    return 0;
}

