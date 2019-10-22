//Union semun
union semun {
   int val;                  //value for SETVAL
   struct semid_ds *buf;     // buffer for IPC_STAT, IPC_SET
   unsigned short *array;    // array for GETALL, SETALL
};

struct sharedMem {
	int id;											//parts_id
	int type;										//type of part
  clock_t time;
};

struct sharedMemAss {
  int id;
  int counter;
  clock_t time;
};

struct dyer {
	int id;
	int type;
};


int sem_down(int );
int sem_up(int );
void sem_Init(int , int );
int getSemaphore();
int getSharedMem();
int getSharedMemAss();
struct sharedMem* attachSharedMem(int );
struct sharedMemAss* attachSharedMemAss(int );
void deleteSemaphore(int );
void detachSharedMem(struct sharedMem* );
void detachSharedMemAss(struct sharedMemAss* );
void deleteSharedMem(int );
int combine(int ,int );
