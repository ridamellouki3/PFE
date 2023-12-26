#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

using namespace std;

struct SharedM
{
  int integer;
  char arr[1024];
  bool dataRead;
  
};

void writeData(int Shm_id, char *str, int i)
{
  struct SharedM *shm_at = (struct SharedM *)shmat(Shm_id, NULL, 0);
  if (shm_at->dataRead)
    {
        cout << "You can't enter anything" << endl;
        exit(EXIT_FAILURE);
    }
    if (shm_at == (SharedM *)-1)
    {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  shm_at->integer = i;
  strncpy(shm_at->arr, str, sizeof(shm_at->arr));

  shm_at->dataRead = true ;
}

int main()
{
  key_t key = ftok("Sharedmemory.txt", 1);
  if (key == -1)
    {
    perror("ftok");
    exit(EXIT_FAILURE);
  }
  int Shm_id = shmget(key, sizeof(SharedM), 0666 | IPC_CREAT);
  if (Shm_id == -1)
    {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  int i = 9;
  const char *newMessage = "Hello from the writer ! ";
  writeData(Shm_id, const_cast<char *>(newMessage), i);

  return 0;
}
