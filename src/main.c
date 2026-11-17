#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include"../include/timer.h"
#include "../include/pomodoro.h"

struct termios original;

void enableRawMode() {
    if(tcgetattr(STDIN_FILENO, &original) == -1) {
	perror("Failed to get terminal attributes.");
	exit(EXIT_FAILURE);
    }

    struct termios raw = original;

    raw.c_lflag &= ~(ECHO | ICANON);

    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
	perror("Failed to enable raw terminal mode.");
	exit(EXIT_FAILURE);
    }
}

void disable_raw_mode() {
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &original) == -1) {
	perror("Failed to restore terminal attributes.");
    }
}

void waitForChildren(void) {
    int status;
    pid_t result;

    while (1) {
        result = waitpid(-1, &status, 0);

        if (result > 0) {
            continue;
        }

        if (result == -1 && errno == EINTR) {
            continue;
        }

        if (result == -1 && errno != ECHILD) {
            perror("Failed to wait for child processes");
        }

        break;
    }
}

void checkChildren(void) {
    int status;
    pid_t result;

    while ((result = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                fprintf(stderr, "Child %ld failed with exit code %d\n",
                        (long)result, WEXITSTATUS(status));
            }
        } else if (WIFSIGNALED(status)) {
	    fprintf(stderr, "Child %ld terminated by signal %d\n",
	    (long)result, WTERMSIG(status));
	}
    }

    if (result == -1 && errno != ECHILD && errno != EINTR) {
        perror("waitpid failed");
    }
}

void notifySystem(char* message) {
    pid_t pid = fork();

    if(pid == -1) {
	perror("Failed to fork sound process");
        return;
    }
    if(pid == 0) {
	execlp("paplay", "paplay",
		"assets/finished.wav", (char *)NULL);
	_exit(EXIT_FAILURE);
    }

    pid = fork();

    if(pid == -1) {
	perror("Failed to fork notification process");
	return;
    }

    if (pid == 0) {
	execlp("notify-send", "notify-send",
		"Pomodoro", message, (char *)NULL);
	_exit(EXIT_FAILURE);
    }
}

int main(void) {
    struct pollfd fds = {
	.fd = STDIN_FILENO,
	.events = POLLIN
    };

    int messageSent = 0;

    enableRawMode();
    printf("\033[2J");     // Clear screen
    printf("\033[?25l");   // Hide cursor
    Pomodoro pomodoro = initPomodoro();		
    int isRunning = 1;
    while(isRunning) {
	checkChildren();
	printf("\033[H"); // Move cursor to top-left
	printf("|         POMODORO         |\n");
	printf("----------------------------\n");
	printf("| P - Pause/Unpause        |\n");
	printf("| N - Next                 |\n");
        printf("| Q - Quit                 |\n");

	printf("---\n");
	for (int i = 0; i < 10; i++) {
	    if (i < getTimerPercentage(&pomodoro) / 10) printf("=");
	    else printf("-");
	}
	printf("\n---\n");

	if(isPomodoroPhaseFinished(&pomodoro)){
	    printf("00:00\n");
	    if(!messageSent) {
		messageSent = 1;
		switch(getPhase(&pomodoro)) {
		    case 0:
			notifySystem("Time for a break!");
			break;
		    default:
			notifySystem("Break is over. Time to focus!");
		}
	    }

	} 
	else printf("%02d:%02d\n", getRemainingPhaseMinutes(&pomodoro), getRemainingPhaseSeconds(&pomodoro));

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

	if (result == -1) {
	    if (errno == EINTR) {
		continue;
	    }

	    perror("poll failed");
	    break;
	}

	if (result > 0 && (fds.revents & POLLIN)) {
	    char c;
	    ssize_t bytesRead = read(STDIN_FILENO, &c, 1);

	    if (bytesRead == -1) {
		if (errno == EINTR) {
		    continue;
		}

		perror("read failed");
		break;
	    } 

	    if(bytesRead == 0) {
		break;
	    }

	    switch(c) {
		case 'p':
		    togglePausePomodoro(&pomodoro);
		    break;
		case 'n':
		    nextPomodoro(&pomodoro);
		    messageSent = 0;
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
    fflush(stdout);

    waitForChildren();

    return 0;
}

