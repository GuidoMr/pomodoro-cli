CC = gcc
CFLAGS = -Wall -Wextra -std=c23 -g -pthread


pomodoro: src/main.c src/timer.c src/pomodoro.c
	mkdir -p build/
	$(CC) $(CFLAGS) src/main.c src/timer.c src/pomodoro.c -o build/pomodoro

test_timer: tests/test_timer.c src/timer.c 
	mkdir -p build/
	$(CC) $(CFLAGS) tests/test_timer.c src/timer.c -o build/test_timer 
	build/test_timer

test_pomodoro: tests/test_pomodoro.c src/pomodoro.c src/timer.c 
	mkdir -p build/
	$(CC) $(CFLAGS) tests/test_pomodoro.c src/pomodoro.c src/timer.c -o build/test_pomodoro 
	build/test_pomodoro 

clean: 
	rm -rf build/
