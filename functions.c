#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "myLib.h"

//Semaphore down operation, using semop
int sem_down(int sem_id) {
   struct sembuf sem_d;
   sem_d.sem_num = 0;
   sem_d.sem_op = -1;
   sem_d.sem_flg = 0;
   if (semop(sem_id,&sem_d,1) == -1) {
       perror("Semaphore Down Operation ");
       return -1;
   }
   return 0;
}

//Semaphore up operation, using semop
int sem_up(int sem_id) {
   struct sembuf sem_d;
   sem_d.sem_num = 0;
   sem_d.sem_op = 1;
   sem_d.sem_flg = 0;
   if (semop(sem_id,&sem_d,1) == -1) {
       perror("Semaphore Up Operation ");
       return -1;
   }
   return 0;
}

//Semaphore Init - set a semaphore's value to val
void sem_Init(int sem_id, int val) {
   union semun arg;
   arg.val = val;
   if (semctl(sem_id,0,SETVAL,arg) == -1) {
       perror("Semaphore Setting Value ");
       exit(0);
   }
}

int getSemaphore() {
    int semId;
    if((semId = semget((key_t)IPC_PRIVATE,1,0600|IPC_CREAT)) < 0){
      perror("Semaphore Creation");
			semctl(semId,0,IPC_RMID);
			exit(0);
    }
    return semId;
}

int getSharedMem() {
  int shmem;

  if((shmem=shmget(IPC_PRIVATE, sizeof(struct sharedMem),0666| IPC_CREAT)) < 0)
  {
    perror("Error in input shared memory creation");
    shmctl(shmem,IPC_RMID,0);
    exit(0);
  }
  return shmem;
}

int getSharedMemAss() {
  int shmem;

  if((shmem=shmget(IPC_PRIVATE, sizeof(struct sharedMemAss),0666| IPC_CREAT)) < 0)
  {
    perror("Error in input shared memory creation");
    shmctl(shmem,IPC_RMID,0);
    exit(0);
  }
  return shmem;
}

struct sharedMem* attachSharedMem(int shmem) {
   struct sharedMem *sh;
   sh = (struct sharedMem*)shmat(shmem,(struct sharedMem*)0,0);
   if (sh == (struct sharedMem*)-1 ) {
     perror("Error in shared memory attach");
     shmctl(shmem,IPC_RMID,0);
     exit(0);
   }
   return sh;
}

struct sharedMemAss* attachSharedMemAss(int shmem) {
   struct sharedMemAss *sh;
   sh = (struct sharedMemAss*)shmat(shmem,(struct sharedMemAss*)0,0);
   if (sh == (struct sharedMemAss*)-1 ) {
     perror("Error in shared memory attach");
     shmctl(shmem,IPC_RMID,0);
     exit(0);
   }
   return sh;
}

void deleteSemaphore(int sem) {
  if (semctl(sem,0,IPC_RMID) == -1) {
    perror("Error in semaphore deleting");
      exit(0);
  }
}

void detachSharedMem(struct sharedMem* sh) {
    shmdt((char*)sh);
}

void detachSharedMemAss(struct sharedMemAss* sh) {
    shmdt((char*)sh);
}

void deleteSharedMem(int shmem) {
  if(shmctl(shmem,IPC_RMID,0) == -1){
      perror("Failed To Delete INPUT Shared Memory");
    exit(0);
    }
}

int combine(int a,int b) {
  int times = 1;
   while (times <= b)
      times *= 10;
   return a*times + b;
}
