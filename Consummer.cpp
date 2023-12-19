#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
using namespace std;

struct SharedM
{
    int integer;
    char arr[1024];
    bool dataRead;
};

void readData(int Shm_id)
{
    struct SharedM *shm_at = (struct SharedM *)shmat(Shm_id, NULL, 0);
    if ((void *)shm_at == (void *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    if (shm_at->dataRead)
    {
        int number = shm_at->integer;
        char message[1024];
        strncpy(message, shm_at->arr, sizeof(message));
        shm_at->dataRead = false ;
        cout << "Read data Integer: " << number << ", Message: " << message << endl;
        cout << "Data is read!!" << endl;
    }
    else
    {
        cout << "No Data to read,waiting...";
        exit(EXIT_FAILURE);
    }
}

int main()
{
    key_t key = ftok("Sharedmemory.txt", 1);
    if (key == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    int Shm_id = shmget(key, sizeof(SharedM ), 0666);
    if (Shm_id == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    readData(Shm_id);

    return 0;
}
