#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{

int n, i, j, temp;
int max = 0;
int min = 0;
int count = 0;
int nCount = 0;
int size = 3;
int fd[2];
pid_t pid;

if (argc == 1) {
	fprintf(stderr,"Usage: ./hw1 <starting value>\n");
	return -1;
}

n = atoi(argv[1]); //  n is the input starting value

if (pipe(fd) == -1) {
fprintf(stderr,"Pipe failed");
return 1;
}

pid = fork();

if (pid < 0) {
fprintf(stderr, "Fork Failed");
return 1;
}

if (pid > 0) {
	close(fd[WRITE_END]);
	read(fd[READ_END], &n, sizeof(n));
	wait(NULL);
	if(n == 495){
		printf("Result from child process 1: %d\n",n);
		close(fd[READ_END]);
		exit(getpid());
	}
}

else {
	close(fd[READ_END]);
	int numberArray[size];
	do {
		// Starting value is entered into an integer array for sorting
		for(i = 0; i < size; i++) {
			numberArray[i] = n % 10;
			n /= 10;
		}
		// Bubble Sort Block
		for(i = 0; i < size - 1; i++) {
			for(j = 0; j < size - i - 1; j++) {
				if(numberArray[j] > numberArray[j + 1]) {
					temp = numberArray[j];
					numberArray[j] = numberArray[j + 1];
					numberArray[j + 1] = temp;
				}
			}
		}
		// Min and Max values are determined for use in the Kaprekar Process
		for(i = 0; i < size; i++) {
			min = (min * 10) + numberArray[i];
		}
		for(i = 2; i >= 0 ; i--) {
			max = (max * 10) + numberArray[i];
		}
		memset(numberArray, 0, size);
		n = max - min;
		count++;
		printf("Child process %d: %dth: %d - %d = %d\n", getpid(), count, max, min, n);
		max = 0;
		min = 0;
		if(n==495){
			nCount++;
		}
	} while(nCount<2);

	write(fd[WRITE_END], &n, sizeof(n));
	close(fd[WRITE_END]);

}
return 0;
}
