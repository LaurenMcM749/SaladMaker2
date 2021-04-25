//Salad = 80 g tomatoes, 50 g peppers, 30 g onions

//1 Chef - manages the salad creation, supplies ingredients, coordinates making salad with help of saladmakers
//3 Saladmakers - await for ingredients to be delivered to their workbench by chef, receive items not available to them, 
//weigh all 3 ingredients before put on cutting board, chop the vegetabls, create the salad

//How It works
// n = max number of salads produced
// Saladmaker has: 2/3 tomatoes weighing 100 grams each, 1/2 peppers 80 grams each, 1/2 onions 60 grams each
// Chef has all ingredient at disposal at all times, continuously replenished
// Saladmaker 1 - Onions
// Saladmaker 2 - Tomatoes
// Saladmaker 3 - Peppers
// Shared memory = workbench: Every saladmaker awaits for his 2 missing vegetables to be placed on the bench so he can:
// 1) have his chance to take them (CS)
// 2) do his independent weighing (reading of current weight)
// 3) chop the ingredient (segment part of the memory)
// Chef - gives vegetabales to saladmakers, but only 2 types at a time, and must wait for the specific salad that needs those 2 to do its CS
// Purge all shared memory and semaphores

//End Output
// 1. Num salads produced
// 2. For each saladmaker: sum weight of each ingredient used overall, total time spent making salads, total time spent waiting for
// vegetables to be delivered, a temporal log of timeline operation (append-only file)
// 3. Listings of time periods 2+ saladmakers busy at the same time




// 1. Chef creates shared memory segment and can be read/write by saladmaker
// shmget() - Create shared segment - returns id of segment
// schmat() - attach segment, pass in id
// schmctl() - ??

//Source - randomc numbers:https://www.geeksforgeeks.org/generating-random-number-range-c/


#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

// Generates and prints 'count' random
// numbers in range [lower, upper].
int printRandoms(int lower, int upper, int count)                         
{
    int i;
    for (i = 0; i < count; i++) {
        int num = (rand() %(upper - lower + 1)) + lower;
        return num;
    }
}


int main (int argc, char ** argv){

    struct Veggies{
        int numSalads;
        int c;
        int tom1;
        int tom2;
        int tom3;
        int pep1;
        int pep2;
        int on1;
        int on2;
        int TO_tomweight;
        int TP_tomweight;
        int TO_onweight;
        int OP_onweight;
        int TP_pepweight;
        int OP_pepweight;
        int TO_tom_dif;
        int TP_tom_dif;
        int OP_on_dif;
        int TO_on_dif;
        int TP_pep_dif;
        int OP_pep_dif;
        double TO_wait;
        double TO_make;
        double TP_wait;
        double TP_make;
        double OP_make;
        double OP_wait;
    };

    int id= 0;
    int z = 0;
    int val = 0;
    int err=0;
    int *mem;
    int retval;
    int t_val;
    int o_val;
    int p_val;
    int to_val;
    int op_val;
    int tp_val;
    sem_t *t;
    sem_t *o;
    sem_t *p;
    sem_t *to; 
    sem_t *tp;
    sem_t *op;
    const char * tp_name;
    const char * to_name;
    const char * op_name;
    const char * t_name; 
    const char * o_name;
    const char * p_name;
    struct Veggies *veggies;
    int x;

    // Make shared memory segment
    // key - IPC_PRVIATE, size = 10, flag = 0666
    id = shmget (IPC_PRIVATE, 1000000, 0666) ; 

    if (id == -1) 
        perror (" Creation ");
    else 
        printf (" Allocated: %d\n" , id);

    //Attach struct to memory
    veggies =  (struct Veggies *) shmat (id , NULL, 0);
    if ((int) veggies == -1)  
        perror (" Attachment .");
    else 
        printf (" Attached . Veggies\n");

    //Initialize values
    veggies->numSalads = 3;
    veggies->c = 0;
    veggies->TO_tomweight=0;
    veggies->TP_tomweight=0;
    veggies->TO_onweight=0;
    veggies->OP_onweight=0;
    veggies->TP_pepweight=0;
    veggies->OP_pepweight=0;
    veggies->TO_tom_dif=0;
    veggies->TP_tom_dif=0;
    veggies->OP_on_dif=0;
    veggies->TO_on_dif=0;
    veggies->TP_pep_dif=0;
    veggies->OP_pep_dif=0;
    veggies->TO_wait=0;
    veggies->TO_make=0;
    veggies->TP_wait=0;
    veggies->TP_make=0;
    veggies->OP_make=0;
    veggies->OP_wait=0;

    //Make new memory for c

    //Attach semaphore to memory (might need to bring back)

    sem_unlink("/to_name");
    sem_unlink("/tp_name");
    sem_unlink("/op_name");
    sem_unlink("/t_name");
    sem_unlink("/p_name");
    sem_unlink("/o_name");

    // Create named semaphores

    to = sem_open("/to_name", O_CREAT, 0666, 0);

    if(to != SEM_FAILED){
        printf("Created semaphore to\n");
    }
    else if (errno == EEXIST){
        printf("Semaphore to exists already\n");
    }

    tp = sem_open("/tp_name", O_CREAT, 0666, 0);
    if(tp != SEM_FAILED){
        printf("Created semaphore tp\n");
    }
    else if (errno == EEXIST){
        printf("Semaphore tp exists already\n");
    }

    op = sem_open("/op_name", O_CREAT, 0666, 0);

    if( op != SEM_FAILED){
        printf("Created semaphore op\n");
    }
    else if (errno == EEXIST){
        printf("Semaphore op exists already\n");
    }

    t = sem_open("/t_name", O_CREAT, 0666, 0);

    if(t != SEM_FAILED){
        printf("Created semaphore t\n");
    }
    else if (errno == EEXIST){
        printf("Semaphore to exists already\n");
    }

    p = sem_open("/p_name", O_CREAT, 0666, 0);

    if(p != SEM_FAILED){
        printf("Created semaphore p\n");
    }
    else if (errno == EEXIST){
        printf("Semaphore to exists already\n");
    }

    o = sem_open("/o_name", O_CREAT, 0666, 0);

    if(o != SEM_FAILED){
        printf("Created semaphore o\n");
    }
    else if (errno == EEXIST){
        printf("Semaphore to exists already\n");
    }

    printf (" Start other process . >"); 

    getchar();


    for(int i = 0; i < 3; i++){

        unsigned seed = time(0);
        srand(seed);

        // int r = rand();
        int r = 3;
        printf("R = %d\n", r);

        if (r % 3 == 0)
        {
        //Saladmaker that needs Tomato and Onion
        sem_post(to); 
        printf("I notified the TO saladmaker to grab them\n"); //To is allowed to grab
        sleep(5);
        sem_post(t); // Change t = 1
        //Random to decide weight of tomatoes in range 60-100
        sleep(1);
        seed = time(0);
        srand(seed);
        veggies->tom1 = printRandoms(60,100,1);
        printf("Tom1 = %d\n",veggies->tom1);
        sleep(1);
        srand(time(0));
        veggies->tom2 = printRandoms(60,100,1);
        printf("Tom2 = %d\n",veggies->tom2);
        sleep(1);
        //Random to decide if 3 tomatoes
        seed = time(0);
        srand(seed);
        x = rand();
        if(x % 2 == 0){
            veggies->tom3 = printRandoms(60,100,1);
            printf("Tom3 = %d\n",veggies->tom3);   
        }
        printf("I put tomatoes on the workbench\n");
        sleep(25);
        printf("Tom1 = %d\n",veggies->tom1);
        printf("Tom2 = %d\n",veggies->tom2);
        printf("Tom3 = %d\n",veggies->tom3);
        sem_post(o); // Change t = 1
        //Put [1-2] onions of [40-80] g each
        veggies->on1 = 40;
        veggies->on2 = 60;
        printf("On1 = %d\n",veggies->on1);
        veggies->tom2 = 80;
        printf("On2 = %d\n",veggies->on2);
        printf("I put onions on the workbench\n");
        sleep(10);
        printf("On1 = %d\n",veggies->on1);
        printf("On2 = %d\n",veggies->on2);
        sem_wait(to);
        sem_getvalue(to, &val);
        printf("The TO saladmaker got what they needed: %d\n", val);

        }

        if (r % 3 == 1)
        {
        //Saladmaker that needs tomato and pepper
        sem_post(tp); //Change tp = 1
        printf("I notified the TP saladmaker tp grab them\n"); //tp is allowed tp grab
        sleep(5);
        sem_post(t); // Change t = 1
        //Put [2-3] tomatoes of [60-100] g each
        veggies->tom1 = 60;
        printf("Tom1 = %d\n",veggies->tom1);
        veggies->tom2 = 80;
        printf("Tom2 = %d\n",veggies->tom2);
        veggies->tom3 = 100;
        printf("Tom3 = %d\n",veggies->tom3);
        //tom1 = [60-100]
        //if rand = 1 --> tom3 = [60-100], else tom3 = 0
        printf("I put tomatoes on the workbench\n");
        sleep(15);
        printf("Tom1 = %d\n",veggies->tom1);
        printf("Tom2 = %d\n",veggies->tom2);
        printf("Tom3 = %d\n",veggies->tom3);
        sem_post(p); // Change t = 1
        //Put [1-2] peppers of [60-100] g each
        veggies->pep1 = 60;
        printf("Pep1 = %d\n",veggies->pep1);
        veggies->pep2 = 80;
        printf("Pep2 = %d\n",veggies->pep2);
        //if rand = 1 --> tom3 = [60-100], else tom3 = 0
        printf("I put peppers on the workbench\n");
        sleep(10);
        printf("Pep1 = %d\n",veggies->pep1);
        printf("Pep2 = %d\n",veggies->pep2);
        sem_wait(tp);
        sem_getvalue(tp, &val);
        printf("The tp saladmaker got what they needed: %d\n", val);
        }

        if (r % 3 == 2)
        {
        //Saladmaker that needs onions and pepper
        sem_post(op); //Change to = 1
        printf("I notified the OP saladmaker to grab them\n"); //To is allowed to grab
        sleep(5);
        sem_post(o); // Change t = 1
        //Put [1-2] onions of [40-80] g each
        veggies->on1 = 40;
        veggies->on2 = 60;
        printf("On1 = %d\n",veggies->on1);
        veggies->tom2 = 80;
        printf("On2 = %d\n",veggies->on2);
        printf("I put onions on the workbench\n");
        sleep(15);
        printf("On1 = %d\n",veggies->on1);
        printf("On2 = %d\n",veggies->on2);
        sem_post(p); // Change t = 1
        //Put [1-2] peppers of [60-100] g each
        veggies->pep1 = 60;
        printf("Pep1 = %d\n",veggies->pep1);
        veggies->pep2 = 80;
        printf("Pep2 = %d\n",veggies->pep2);
        //if rand = 1 --> tom3 = [60-100], else tom3 = 0
        printf("I put peppers on the workbench\n");
        sleep(10);
        printf("Pep1 = %d\n",veggies->pep1);
        printf("Pep2 = %d\n",veggies->pep2);
        sem_wait(op); // Change to = 0 again after Saladmaker says I am finished
        printf("The op saladmaker got what they needed\n");
            
        }


    }

    printf("Total number of salads made: %d\n",veggies->c);
    printf("---------------\n");
    printf("TO stats:\n");
    printf("TO_Tom: %d, TO_On: %d\n", veggies->TO_tomweight, veggies->TO_onweight);
    printf("TO- Time Wait: %lf, TO- Time Make: %lf\n", veggies->TO_wait, veggies->TO_make);
    printf("---------------\n");
    printf("TP stats:\n");
    printf("TO_Tom: %d, TP_Pep: %d\n", veggies->TP_tomweight, veggies->TP_pepweight);
    printf("TP- Time Wait: %lf, TP- Time Make: %lf\n", veggies->TP_wait, veggies->TP_make);
    printf("---------------\n");
    printf("OP stats:\n");
    printf("OP_Pep: %d, OP_On: %d\n", veggies->OP_pepweight, veggies->OP_onweight);
    printf("OP- Time Wait: %lf, OP- Time Make: %lf\n", veggies->OP_wait, veggies->OP_make);
    printf("---------------");
    printf("Temporal log accessible in file outfile\n");
    //Time making
    //Time waiting
    //Time salads were working in parallel
    //Temporal log


    sem_close(to);
    sem_close(tp);
    sem_close(op);
    sem_close(t);
    sem_close(o);
    sem_close(p);

    sem_unlink("/to_name");
    sem_unlink("/tp_name");
    sem_unlink("/op_name");
    sem_unlink("/t_name");
    sem_unlink("/p_name");
    sem_unlink("/o_name");

    // printf ("mem is now %d\n", * mem ); /* Print out new value */

    err = shmctl (id , IPC_RMID , NULL);  //Remove segment 
    
    if (err == -1) 
     perror ("Removal.");
    else 
     printf ("Removed. %d\n", (int)(err));
    
    return 0;

}