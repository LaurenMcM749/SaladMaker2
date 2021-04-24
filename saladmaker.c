#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

int main (int argc, char **argv){

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
    };

    int id, err, err1, err2, err3, err4, err5, err6;
    int *mem;
    int z=0;
    int val = 0;
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

    if (argc <= 1) {
     printf (" Need shared memory id. \n"); 
     exit (1); 
    }

    //Get id from command line
    sscanf ( argv [1] , "%d", &id); 
    printf ("Id is %d\n", id);

    //Attach values to memory
    veggies =  (struct Veggies *) shmat (id , NULL, 0);
    if ((int) veggies == -1)  
        perror (" Attachment .");
    else 
        printf (" Attached . Veggies\n");

    //Initialize values
    veggies->numSalads = 3;
    veggies->c = 0;

    // Might need to attach named semaphore to memory and/or unlink

    //Create named semaphores

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

    //Set max number of salads

    // *c = 0;
    // *numSalads = 3;

   
    //Saladmaker processes
    for( int i = 0; i < 3; i++)
    {
        int saladmaker = fork();

        if (saladmaker == -1) {
            perror("Failed to fork");
            exit(1);
        }

        if (saladmaker == 0)
        {

            int maker = getpid();

                //NeedTomOn
                if (maker % 3 == 0)
                {
                int my_tomweight = 0;
                int my_onweight = 0;
                int my_pepweight = 0;
                int tom_dif = 0;
                int on_dif = 0;
                int pep_dif = 0;

                while (veggies->c < veggies->numSalads)
                {
                    printf("TO: I'm waiting to grab tomato and onion\n");
                    sem_wait(to);
                    sem_getvalue(to, &val);
                    printf("After wait: TO = %d\n", val );
                    sem_wait(t); 
                    my_tomweight += tom_dif;
                    my_tomweight += veggies->tom1;
                    my_tomweight += veggies->tom2;
                    my_tomweight += veggies->tom3;
                    veggies->tom1 = 0;
                    veggies->tom2 = 0;
                    veggies->tom3 = 0;
                    printf("TO: I grabbed %d g tomato\n",my_tomweight);
                    sem_wait(o); 
                    my_onweight += on_dif;
                    my_onweight += veggies->on1;
                    my_onweight += veggies->on2;
                    veggies->on1 = 0;
                    veggies->on2 = 0;
                    printf("TO: I grabbed %d g onions off workbench\n", my_onweight);
                    veggies->c = veggies->c + 1; 
                    sem_post(to); // ("I am done") - Now chef can issue wait 
                    //This is where other processes can work in parallel
                    printf("TO: I got the tomato and the onion I needed!\n");
                    printf("TO: Time to chop and keep the difference for next time\n");
                    // if my_tomweight > 300 {dif = my_tomweight - 300, my_tomweight = 300}
                    // TO_total_tomweight += my_tomweight 
                    //sleep(rand)
                    printf("%d salads made\n",veggies->c);
                    sleep(10);
                } 
                }  

                if (maker % 3 == 1){
                //Need TomPep
                int my_tomweight = 0;
                int my_onweight = 0;
                int my_pepweight = 0;
                int tom_dif = 0;
                int on_dif = 0;
                int pep_dif = 0;
            
                while (veggies->c < veggies->numSalads)
                {
                    printf("TP: I'm waiting to grab tomato and pepper\n");
                    sem_wait(tp);
                    sem_getvalue(tp, &val);
                    printf("After wait: TP = %d\n", val );
                    sem_wait(t); 
                    my_tomweight += tom_dif;
                    my_tomweight += veggies->tom1;
                    my_tomweight += veggies->tom2;
                    my_tomweight += veggies->tom3;
                    veggies->tom1 = 0;
                    veggies->tom2 = 0;
                    veggies->tom3 = 0;
                    printf("TP: I grabbed %d g tomato\n",my_tomweight);
                    sem_wait(p); //Bug - stuck here - I think chef has changed to too fast so locks out
                    my_pepweight += pep_dif;
                    my_pepweight += veggies->pep1;
                    my_pepweight += veggies->pep2;
                    veggies->pep1 = 0;
                    veggies->pep2 = 0;
                    printf("TP: I grabbed %d g pepper\n",my_pepweight);
                    veggies->c = veggies->c + 1;  
                    sem_post(tp); // ("I am done") - Now chef can issue wait
                    printf("TP: I got the tomato and pepper I needed!\n");
                    printf("TP: Time to weigh, chop, and serve\n");
                    printf("%d salads made\n",veggies->c);
                    sleep(10);
                }
                }

                if (maker % 3 == 2)
                {

                int my_tomweight = 0;
                int my_onweight = 0;
                int my_pepweight = 0;
                int tom_dif = 0;
                int on_dif = 0;
                int pep_dif = 0;

                //Need TomPep
                while (veggies->c < veggies->numSalads)
                {
                    printf("OP: I'm waiting to grab onion and pepper\n");
                    sem_wait(op);
                    sem_getvalue(op, &val);
                    printf("After wait: op = %d\n", val );
                    sem_wait(o); 
                    my_onweight += on_dif;
                    my_onweight += veggies->on1;
                    my_onweight += veggies->on2;
                    veggies->on1 = 0;
                    veggies->on2 = 0;
                    printf("OP: I grabbed %d g onions off workbench\n", my_onweight);
                    sem_wait(p); //Bug - stuck here - I think chef has changed to too fast so locks out
                    my_pepweight += pep_dif;
                    my_pepweight += veggies->pep1;
                    my_pepweight += veggies->pep2;
                    veggies->pep1 = 0;
                    veggies->pep2 = 0;
                    printf("OP: I grabbed %d g pepper\n",my_pepweight);
                    veggies->c = veggies->c + 1; 
                    sem_post(op); // ("I am done") - Now chef can issue wait
                    printf("op: I got the tomato and pepper I needed!\n");
                    printf("op: Time to weigh, chop, and serve\n");
                    printf("%d salads made\n",veggies->c);
                    sleep(10);
                }
                }

        }

    }



    // }

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



    
    
    // printf (" Changed mem is now %d\n", *mem );
    
    // err = shmdt (); /* Detach segment */
    
    // if (err == -1) 
    //  perror (" Detachment .");
    // else 
    //  printf (" Detachment %d\n", err );


    // err1 = shmdt (to); /* Detach segment */
    
    // if (err1 == -1) 
    //  perror (" Detachment to.");
    // else 
    //  printf (" Detachment to %d\n", err1 );


    // err2 = shmdt (tp); /* Detach segment */
    
    // if (err2 == -1) 
    //  perror (" Detachment tp .");
    // else 
    //  printf (" Detachment tp %d\n", err2 );


    // err3 = shmdt (op); /* Detach segment */
    
    // if (err3 == -1) 
    //  perror (" Detachment op.");
    // else 
    //  printf (" Detachment op %d\n", err3 );


    // err4 = shmdt (t); /* Detach segment */
    
    // if (err4 == -1) 
    //  perror (" Detachment  t.");
    // else 
    //  printf (" Detachment t %d\n", err4 );


    // err5 = shmdt (o); /* Detach segment */
    
    // if (err5 == -1) 
    //  perror (" Detachment t.");
    // else 
    //  printf (" Detachment o %d\n", err5 );


    // err6 = shmdt (p); /* Detach segment */
    
    // if (err6 == -1) 
    //  perror (" Detachment p.");
    // else 
    //  printf (" Detachment p %d\n", err6 );
    
    return 0;


    


}