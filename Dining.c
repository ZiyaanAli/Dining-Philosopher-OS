#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <term.h>
#include <curses.h>
#include <stdlib.h>

unsigned int sleep(unsigned int seconds);

#define N 5
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

enum s{
 EATING , HUNGRY , THINKING	//it works like true or false ENUM (1 or 0)
};
int state[N];
struct Philosopher{
    char * name;
    int id;
};
struct Philosopher phil[5]={{"Riyan",0},{"Maredia",1},{"Amik",2},{"Barkat",3},{"Ali",4}};
sem_t mutex;
sem_t S[5];
 void test(int phnum)
{
    if (state[phnum] == HUNGRY
        && state[LEFT] != EATING
        && state[RIGHT] != EATING) {
        // state that eating
        state[phnum] = EATING;
        sleep(2);
    printf("Philosopher %s , %d Picking %d and %d down . \n",phil[phnum].name,
           phnum + 1, LEFT + 1, phnum + 1);
           printf("Philosopher %s , id : %d is Eating .\n",phil[phnum].name, phnum + 1);
        // during takefork
        // used to wake up hungry philosophers
        // during putfork
        sem_post(&S[phnum]);
    }
}
// take up chopsticks
void PickUpChopSticks(int phnum)
{
 
    sem_wait(&mutex);
 
    // state that hungry
    state[phnum] = HUNGRY;
 
    printf("Philosopher %s , id : %d is Hungry\n",phil[phnum].name, phnum + 1);
 
    // eat if neighbours are not eating
    test(phnum);
 
    sem_post(&mutex);
 
    // if unable to eat wait to be signalled
    sem_wait(&S[phnum]);
 
    sleep(1);
}
 
// put down chopsticks
void PutChopsticksDown(int phnum)
{
 
    sem_wait(&mutex);
 
    // state that thinking
    state[phnum] = THINKING;

    printf("Philosopher %s , %d putting fork %d and %d down\n",phil[phnum].name,
           phnum + 1, LEFT + 1, phnum + 1);
           printf("Philosopher %s , id : %d is thinking\n",phil[phnum].name, phnum + 1);
    test(LEFT);
    test(RIGHT);
 
    sem_post(&mutex);
}
 
void* philospher(int* num)
{
 
    while (1) {
 
        int* i = num;
 
        sleep(1);
 
        PickUpChopSticks(*i) ;
 
        sleep(0);

        PutChopsticksDown(*i);
    }
}
int main()
{

    int i;
    pthread_t thread_id[N];
 
    // initialize the semaphores
    sem_init(&mutex, 0, 1);
 
    for (i = 0; i < N; i++)
 
        sem_init(&S[i], 0, 0);
   
    system("setterm -bold off");
 

    for (i = 0; i < N; i++) {
 
        // create philosopher processes
        pthread_create(&thread_id[i], NULL, (void*)philospher, &phil[i].id);
 
        printf("Philosopher %s , id : %d is thinking\n",phil[i].name, i + 1);
    }
 

    for (i = 0; i < N; i++)
  pthread_join(thread_id[i], NULL);

}
