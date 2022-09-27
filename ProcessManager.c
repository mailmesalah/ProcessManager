/*
 * ProcessManager.c
 *
 *  Created on: Jul 1, 2015
 *      Author: Sely
 */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include  <signal.h>

int start = 0;
int pid;
int waitt = 0;
/* This flag controls termination of the main loop. */
volatile sig_atomic_t run = 1;

void INThandler(int sig) {
	//ignores the signal
	signal(SIGINT, SIG_IGN);
	//Check for more Ctrl+C
	signal(SIGINT, INThandler);
	if (getppid() == 1) //Check if it is parent process
			{
		waitt++;
		//Creates a child process
		pid = fork();
		//Let the program run if this is first Ctrl+C
		start = 1;

	} else {

	}
}

/* The signal handler just clears the flag and re-enables itself. */
void catch_alarm(int sig) {
	run = 0;
	signal(sig, catch_alarm);
}
int main() {

	printf("\nProgram started \n");
	int status;
	//Register for signal
	signal(SIGINT, INThandler);

	start = 0;
	while (!start)
		; //Waits till at least one child process is created

	while (1) {
		if (pid < 0) {
			printf("\n Error ");
			exit(1);
		}

		if (getppid() == 1) {
			wait(&status);

			printf("\n Hello I am the parent process ");

			printf("\n My actual pid is %d %d %d \n ", getpid(), pid,
					getppid());

			//exit(1);
		} else {
			printf("\n Hello I am the child process ");
			usleep(2000000);
			/* Establish a handler for SIGALRM signals. */
			signal(SIGALRM, catch_alarm);
			/* Set an alarm to go off in a little while. */
			alarm(2);

			/* Check the flag once in a while to see when to quit. */
			while (run)
				;

			printf("\n My pid is %d %d %d ", getpid(), pid, getppid());
			//exit(0);
		}
	}
}

