#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "myLib.h"


int main(int argc, char* argv[] ) {
		int sem_constructor, sem_dyer,
    sem_check1, sem_check2, sem_check3,
		//semaphores that tell assembler that the part is ready to take
    sem_assem1, sem_assem2, sem_assem3,
		//semaphores that tell check that the part is used and need to send another one
		sem_assem4, sem_assem5, sem_assem6;
		int shmem1, shmem2, shmem3, shmem4, shmem5;
		struct sharedMem *sh1, *sh2;
		struct sharedMemAss *sh3, *sh4, *sh5;
		struct dyer *dyer = malloc(sizeof(struct dyer));
    pid_t pid;
    int status = 0;
		int fdyer = 1000, sdyer = 500, tdyer = 1500,
		fcheck = 800, scheck = 700, tcheck = 900;
		double painterAverage = 0.0, assembleAverage = 0.0, timeSpent = 0.0;

    if(argc!=2){
      printf("Wrong number of arguments.Please insert 1 argument.\n");
      exit(0);
    }
    int productsNum = atoi(argv[1]);

		//Initialize Semaphores
		sem_constructor = getSemaphore();
		sem_Init(sem_constructor,1);

		sem_dyer = getSemaphore();
		sem_Init(sem_dyer,0);

    sem_check1 = getSemaphore();
		sem_Init(sem_check1,0);

    sem_check2 = getSemaphore();
		sem_Init(sem_check2,0);

    sem_check3 = getSemaphore();
		sem_Init(sem_check3,0);

    sem_assem1 = getSemaphore();
		sem_Init(sem_assem1,0);

    sem_assem2 = getSemaphore();
		sem_Init(sem_assem2,0);

    sem_assem3 = getSemaphore();
		sem_Init(sem_assem3,0);

    sem_assem4 = getSemaphore();
		sem_Init(sem_assem4,1);

    sem_assem5 = getSemaphore();
		sem_Init(sem_assem5,1);

    sem_assem6 = getSemaphore();
		sem_Init(sem_assem6,1);

		//Create a new shared memory for Con-Dyer
		shmem1 = getSharedMem();
		//Attach the shared memory segments
		sh1 = attachSharedMem(shmem1);

		//Create a new shared memory for Dye-Check
		shmem2 = getSharedMem();
		sh2 = attachSharedMem(shmem2);

		//Create a new shared memory for Check-Ass
		shmem3 = getSharedMemAss();
		sh3 = attachSharedMemAss(shmem3);

		//Create a new shared memory for Check-Ass
		shmem4 = getSharedMemAss();
		sh4 = attachSharedMemAss(shmem4);

		//Create a new shared memory for Check-Ass
		shmem5 = getSharedMemAss();
		sh5 = attachSharedMemAss(shmem5);

    for (int kid = 0; kid < 8; kid++) {
			pid = fork();
			if(pid < 0){
			    exit(EXIT_FAILURE);
			}
			else if (pid == 0){
				/* Child process */
				int randomID;
        if(kid < 3){
  				//Constructors
          for(int i=0; i<productsNum; i++){
  					sem_down(sem_constructor);
    					srand(time(NULL) ^ (getpid()<<16) - i);
    					randomID = rand() % 9000 + 1000;
  						sh1->type = kid+1;
  						sh1->id = randomID;
							sh1->time = clock();
  					sem_up(sem_dyer);
          }
          exit(EXIT_SUCCESS);
          exit(EXIT_SUCCESS);
          exit(EXIT_SUCCESS);
        }
				//Dyer
				else if(kid == 3) {
          for(int j=0; j<productsNum*3; j++){
            sem_down(sem_dyer);
							timeSpent = clock() - sh1->time;
							painterAverage += (double) timeSpent;
              dyer->id = sh1->id;
              dyer->type = sh1->type;
              switch(dyer->type){
                case 1:
									usleep(fdyer);
                  sh2->type = dyer->type;
                  sh2->id = dyer->id;
									sh2->time = sh1->time;
                  sem_up(sem_check1);
                  sem_up(sem_constructor);
                  break;
                case 2:
									usleep(sdyer);
                  sh2->type = dyer->type;
                  sh2->id = dyer->id;
                  sem_up(sem_check2);
                  sem_up(sem_constructor);
                  break;
                case 3:
									usleep(tdyer);
                  sh2->type = dyer->type;
                  sh2->id = dyer->id;
                  sem_up(sem_check3);
                  sem_up(sem_constructor);
                  break;
              }
            }
						printf("Average time waiting on dyer: %7lf \n",(painterAverage/(CLOCKS_PER_SEC*3*productsNum)));
						free(dyer);
            exit(EXIT_SUCCESS);
				  }
          //Check
          else if(kid > 3 && kid < 7){
						int *check1 = malloc(productsNum*sizeof(int));
				    int *check2 = malloc(productsNum*sizeof(int));
				    int *check3 = malloc(productsNum*sizeof(int));
            if(kid == 4) {
              for(int i=0; i<productsNum; i++){
								usleep(fcheck);
                  sem_down(sem_check1);
                    check1[i] = sh2->id;
				            sem_down(sem_assem4);
										sh3->time = sh2->time;
                    sh3->id = check1[sh3->counter];
                  sem_up(sem_assem1);
              }
            }
            else if(kid == 5) {
              for(int i=0; i<productsNum; i++){
								usleep(scheck);
                  sem_down(sem_check2);
                    check2[i] = sh2->id;
				            sem_down(sem_assem5);
                    sh4->id = check2[sh4->counter];
                  sem_up(sem_assem2);
              }
            }
            else if(kid == 6) {
              for(int i=0; i<productsNum; i++){
								usleep(tcheck);
                  sem_down(sem_check3);
                    check3[i] = sh2->id;
				            sem_down(sem_assem6);
                    sh5->id = check3[sh5->counter];
                  sem_up(sem_assem3);
              }
          }
			    free(check1);
			    free(check2);
					free(check3);
          exit(EXIT_SUCCESS);
          exit(EXIT_SUCCESS);
          exit(EXIT_SUCCESS);
        }
        //Assembler
        else if(kid == 7){
					double endTime = 0.0;
		    	int *assembler = malloc((productsNum*3)*sizeof(int));
					char *id = malloc(3*sizeof(char));
          for(int j=0; j<productsNum; j++){
						//we get the first part
            sem_down(sem_assem1);
						assembler[j*3] = sh3->id;
						sh3->counter++;
						//we get the second part
            sem_down(sem_assem2);
						assembler[j*3+1] = sh4->id;
						sh4->counter++;
						//we get the third part
            sem_down(sem_assem3);
						assembler[j*3+2] = sh5->id;
						sh5->counter++;

						//now we start assembling the product

					 sprintf(id, "%d%d%d", assembler[j*3], assembler[j*3+1], assembler[j*3+2]);

					 endTime += clock() - sh3->time;
					 
					 sem_up(sem_assem4);
           sem_up(sem_assem5);
           sem_up(sem_assem6);
          }
					printf("Average assembling time: %7lf\n",endTime/(productsNum*CLOCKS_PER_SEC));
					free(id);
					free(assembler);
          exit(EXIT_SUCCESS);
        }
		  }
			else {
				/* parent process */
			  continue;
			}
  }

    	while(wait(&status)>0);

  		// Detach and delete shared memory
			detachSharedMem(sh1);
			deleteSharedMem(shmem1);

			detachSharedMem(sh2);
			deleteSharedMem(shmem2);

			detachSharedMemAss(sh3);
			deleteSharedMem(shmem3);

			detachSharedMemAss(sh4);
			deleteSharedMem(shmem4);

			detachSharedMemAss(sh5);
			deleteSharedMem(shmem5);

		// Delete semaphores
		deleteSemaphore(sem_constructor);
		deleteSemaphore(sem_dyer);
		deleteSemaphore(sem_check1);
		deleteSemaphore(sem_check2);
		deleteSemaphore(sem_check3);
		deleteSemaphore(sem_assem1);
		deleteSemaphore(sem_assem2);
		deleteSemaphore(sem_assem3);
		deleteSemaphore(sem_assem4);
		deleteSemaphore(sem_assem5);
		deleteSemaphore(sem_assem6);

    return 0;
}
