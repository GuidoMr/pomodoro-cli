#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "../include/timer.h"
#include "../include/pomodoro.h"

struct termios original;

volatile sig_atomic_t isRunning = 1;

void handleSignal(int sig) {
    (void)sig;
    isRunning = 0;
}

void enableRawMode() {
    if(tcgetattr(STDIN_FILENO, &original) == -1) {
	perror("Failed to get terminal attributes.");
	exit(EXIT_FAILURE);
    }

    struct termios raw = original;

    // Disable input echo and canonical mode.
    // Characters are made available as they are typed (no need for line delimiter).
    raw.c_lflag &= ~(ECHO | ICANON);

    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
	perror("Failed to enable raw terminal mode.");
	exit(EXIT_FAILURE);
    }
}

void disableRawMode() {
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &original) == -1) {
	perror("Failed to restore terminal attributes.");
    }
}

void handleChildStatus(pid_t pid, int status) {
    if(WIFEXITED(status)) {
	if(WEXITSTATUS(status) != 0) {
	    fprintf(stderr, "Child %ld failed with exid code %d\n", 
		    (long)pid, WEXITSTATUS(status));
	}
    } else if (WIFSIGNALED(status)) {
	fprintf(stderr, "Child %ld terminated by signal %d\n",
		(long)pid, WTERMSIG(status));
    } else {
	fprintf(stderr, "Child %ld terminated unexpectedly\n", (long)pid);
    }
}

void reapChildren(int mode) {
    int status;
    pid_t result;

    while (1) {
        result = waitpid(-1, &status, mode);

        if (result > 0) {
	    handleChildStatus(result, status);
            continue;
        }

	// Interrupted before collecting child.
        if (result == -1 && errno == EINTR) {
            continue;
        }

        if (result == -1 && errno != ECHILD) {
            perror("Failed to wait for child processes");
        }

	// errno == ECHILD -> no more children to wait for.
        break;
    }
}

void waitForChildren(void) {
    reapChildren(0);
}

void checkChildren(void) {
    reapChildren(WNOHANG);
}


void notifySystem(const char* message) {
    pid_t pid = fork();

    if (pid == -1) {
	perror("Failed to fork sound process");
        return;
    }
    if (pid == 0) {
	execlp("paplay", "paplay",
		"assets/finished.wav", (char *)NULL);
	perror("Failed to run paplay");
	_exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
	perror("Failed to fork notification process");
	return;
    }

    if (pid == 0) {
	execlp("notify-send", "notify-send",
		"Pomodoro", message, (char *)NULL);
	perror("Failed to run notify-send");
	_exit(EXIT_FAILURE);
    }
}

void cleanup() {
    disableRawMode();

    printf("\033[2J");     // Clear screen
    printf("\033[?25h");   // Show cursor
    fflush(stdout);

    waitForChildren();
}

int main(void) {
    struct pollfd fds = {
	.fd = STDIN_FILENO,
	.events = POLLIN
    };

    struct sigaction sa = {0};

    sa.sa_handler = handleSignal;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1 ||
	    sigaction(SIGTERM, &sa, NULL) == -1) {
	perror("sigaction failed");
	return EXIT_FAILURE;
    }

    int messageSent = 0;

    // INITIALIZE 

    enableRawMode();
    atexit(cleanup);
    printf("\033[2J");     // Clear screen
    printf("\033[?25l");   // Hide cursor
			   //
    Pomodoro pomodoro;
    if(initPomodoro(&pomodoro) != POMODORO_OK) { 
	fprintf(stderr, "Failed to initialize pomodoro");
	_exit(EXIT_FAILURE);
    }

    // MAIN LOOP
   
    while(isRunning) {
	checkChildren();
	// PRINT MENU

	printf("\033[H"); // Move cursor to top-left
	printf("|         POMODORO         |\n");
	printf("----------------------------\n");
	printf("| P - Pause/Unpause        |\n");
	printf("| N - Next                 |\n");
        printf("| Q - Quit                 |\n");

	printf("---\n");
	int timerPercentage;
	if (getTimerPercentage(&pomodoro, &timerPercentage) != POMODORO_OK) {
	    fprintf(stderr, "Failed to get timer percentage\n");
	    _exit(EXIT_FAILURE);
	} 

	for (int i = 0; i < 10; i++) {
	    if (i < timerPercentage / 10) printf("=");
	    else printf("-");
	}
	printf("\n---\n");

	int isPhaseFinished;
	if (isPomodoroPhaseFinished(&pomodoro, &isPhaseFinished) != POMODORO_OK) {
	    fprintf(stderr, "Failed to get pomodoro phase status\n");
	    _exit(EXIT_FAILURE);
	} 

	if(isPhaseFinished){
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

	int minutes;
	if (getRemainingPhaseMinutes(&pomodoro, &minutes) != POMODORO_OK) {
	    fprintf(stderr, "Failed to get remaining pomodoro phase minutes\n");
	    _exit(EXIT_FAILURE);
	}

	int seconds;
	if (getRemainingPhaseSeconds(&pomodoro, &seconds) != POMODORO_OK) {
	    fprintf(stderr, "Failed to get remaining pomodoro phase seconds\n");
	    _exit(EXIT_FAILURE);
	}

	else printf("%02d:%02d\n", minutes, seconds);

	printf("                                                                                        ");
	switch(getPhase(&pomodoro)) {
	    case FOCUS:
		printf("\rFOCUS\n");
		break;
	    case SHORT_BREAK:
		printf("\rSHORT BREAK\n");
		break;
	    case LONG_BREAK:
		printf("\rLONG BREAK\n");
	}

	fflush(stdout);

	// HANDLE INPUT

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

    // CLEANUP
    
    cleanup();

    return 0;
}

