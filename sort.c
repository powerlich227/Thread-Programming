
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>

pthread_t thread[10];
sem_t semaphore;
sem_t barrier;
sem_t size;
int count = 0;	  // counter of threads to the barrier
int a[100] = {0}; // sorting numbers
int b[100] = {0}; // sempahores initial value
int n;            // the number of sorting numbers
//int numsem;       // the number of semaphore
int m = 0;        // the counter of numbers per line
char *mode;

void *sort(void *arg)
{
    int id = (intptr_t)arg;

    int num_groups;
    int group_size;
    int gindex;
    int mindex;
    int group_start;
    int group_end;
    int data1;
    int data2;

    for (int q = 0; q < log(n) / log(2); q++) {
        for (int p = 0; p < log(n) / log(2); p++) {

	    sem_wait(&size);

            num_groups = pow(2, p);
            group_size = n / num_groups;
            gindex = id / (group_size / 2);
            mindex = id % (group_size / 2);
            group_start = gindex * group_size;
            group_end = (gindex + 1) * group_size - 1;
            data1 = group_start + mindex;
            data2 = group_end - mindex;

            if (a[data1] > a[data2]) {
                int temp = a[data1];
                a[data1] = a[data2];
                a[data2] = temp;
            }

            // when each thread passes the barrier each time, you need to print out a message
            if (strcmp(mode, "-o") == 0) {
                printf("Thread %d finished stage %d phase %d\n", id, (q + 1), (p + 1));	
	    }

	    sem_wait(&semaphore);
	    count = count + 1;

            if (count == n/2) {
                sem_post(&barrier);

	    	// In the observation mode, we need to print out the list of integers being sorted after each phase.
                if (strcmp(mode, "-o") == 0) {
            	    m = 0;
            	    for (int j = 0; j < n; j++) {
            		printf("%d ", a[j]);            
                	m++;
                	if(m % 8 == 0 || m == n) {
                    		printf("\n");
                	}
		    }
		}	
	    }
	    
	    sem_post(&semaphore);
	    sem_wait(&barrier);
	    sem_wait(&semaphore);
	    count = count - 1;
	    sem_post(&barrier);

	    if (count == 0) {
	    	sem_wait(&barrier);
		for (int i = 0; i < n/2; i++) {
		     sem_post(&size);
		}
	    }

	    sem_post(&semaphore);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    char *filename = argv[1];
    mode = argv[2];
    //char *semainit = argv[2];
    //printf("Filename: %s\n", filename);
    //printf("Semaphore Initialize: %s\n", semainit);
    //printf("Mode: %s\n", mode);

    FILE *fp1 = fopen(filename, "r");
    if (NULL == fp1) {
        printf("Error: Failed to open input.txt! \n");
        perror("fopen");
        return 0;
    }

    /*
    FILE *fp2 = fopen(semainit, "r");
    if (NULL == fp2) {
        printf("Error: Failed to open sema.init! \n");
        perror("fopen");
        return 0;
    }*/

    // Read the number of integers in the input list, N;
    char buffer[16];
    fgets(buffer, 16, fp1);
    n = atoi(buffer);

    /*
    fgets(buffer, 16, fp2);
    numsem = atoi(buffer);
    */

    while (n != 0) {

        m = 0;
        int i = 0;
        // Read N integer numbers and store them in an array;
        for (i = 0; i < n; i++) {
            fgets(buffer, 16, fp1);
            a[i] = atoi(buffer);

            // In both regular and observation mode, we need to print the initial list before the threads start.    
            printf("%d ", a[i]);

            // If N >= 8, you should print 8 numbers per line; otherwise, all numbers in one line.
            m++;
            if(m % 8 == 0 || m == n) {
                printf("\n");
            }
        }
        
        // Create and initialize the semaphores necessary for synchronization;
        sem_init(&semaphore, 0, 1);
	    sem_init(&barrier, 0, 0);
	    sem_init(&size, 0, n/2);
	
	// Create and initialize the semaphores from sema_init.txt
        /*for (i = 0; i < numsem; i++) {
            fgets(buffer, 16, fp2);
            b[i] = atoi(buffer);
            sem_init(&semaphore[i], 0, b[i]);
        }*/

        // Create N/2 threads to sort the array using balanced sort algorithm;
        for (i = 0; i < n/2; i++) {
            pthread_create(&thread[i], NULL, sort, (void *)(intptr_t) i);
        }

        // Wait for all the threads to finish;
        for (i = 0; i < n/2; i++) {
            pthread_join(thread[i], NULL);
        }
        
        // Print the array of sorted integers;
        for (i = 0; i < n; i++) {
            printf("%d ", a[i]);
            m++;
            if(m % 8 == 0 || m == n) {
                printf("\n");
            }
        } 
        printf("------------------------------\n");

        sem_destroy(&semaphore);
	    sem_destroy(&barrier);
	    sem_destroy(&size);

        // Read the number of integers in the input list, N;
        fgets(buffer, 16, fp1);
        n = atoi(buffer);
	
	/*fgets(buffer, 16, fp2);
	numsem = atoi(buffer);*/
    }

    fclose(fp1);
    //fclose(fp2);

    return 0;
}
