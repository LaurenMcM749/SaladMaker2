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
#include <sys/time.h>
#include <time.h>

// Time source: https://levelup.gitconnected.com/8-ways-to-measure-execution-time-in-c-c-48634458d0f9

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
    sem_t *out;
    const char * tp_name;
    const char * to_name;
    const char * op_name;
    const char * t_name; 
    const char * o_name;
    const char * p_name;
    struct Veggies *veggies;
    FILE *outfile;
    outfile = fopen("outfile","a");
    time_t rawtime;
    struct tm * timeinfo;


    if (outfile == NULL){
        printf("Error opening outfile\n");
    }

    if (argc <= 1) {
     printf (" Need shared memory id. \n"); 
     exit (1); 
    }

    //Get id from command line
    sscanf ( argv [3] , "%d", &id); 
    printf ("Id is %d\n", id);

    //Attach values to memory
    veggies =  (struct Veggies *) shmat (id , NULL, 0);
    if ((int) veggies == -1)  
        perror (" Attachment .");
    else 
        printf (" Attached . Veggies\n");

    //Initialize values
    // veggies->numSalads = 0;
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

    out = sem_open("/out_name", O_CREAT, 0666, 1);
    if(out != SEM_FAILED){
        printf("Created semaphore out\n");
    }
    else if (errno == EEXIST){
        printf("Semaphore out exists already\n");
    }

    int time1 = atoi(argv[1]);
    int time2 = atoi(argv[2]);
    printf("NumSalads = %d\n", veggies->numSalads);

   
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

                while (veggies->c < veggies->numSalads)
                {
                    // outfile = fopen("outfile","a");
                    printf("TO: I'm waiting to grab tomato and onion\n");
                    //1 Log
                    sem_wait(out);
                    fprintf(outfile,"TO: I'm waiting to grab tomato and onion %s\n", asctime (timeinfo));
                    sem_post(out);
                    printf("Added pepper to the cutting board!\n");
                    // Start measuring waiting time
                    struct timeval begin, end;
                    gettimeofday(&begin, 0);
                    //TO turn to grab 
                    sem_wait(to);
                    sem_getvalue(to, &val);
                    printf("After wait: TO = %d\n", val );
                    // sleep(20);
                    // Stop measuring time and calculate the elapsed time
                    gettimeofday(&end, 0);
                    long seconds = end.tv_sec - begin.tv_sec;
                    long microseconds = end.tv_usec - begin.tv_usec;
                    double elapsed = seconds + microseconds*1e-6;
                    printf("TO Time waited: %.3f seconds.\n", elapsed);//End time
                    // fprintf( outfile, "TO wait: %lf\n",elapsed);
                    time ( &rawtime );
                    timeinfo = localtime ( &rawtime );
                    sem_wait(out);
                    fprintf (outfile, "TO time: %s", asctime (timeinfo) );
                    sem_post(out);
                    sem_getvalue(out, &val);
                    printf("TO: Out = %d\n", val);
                    veggies->TO_wait += elapsed;
                    // Start measuring working time
                    gettimeofday(&begin, 0);
                    //Grab and weigh tomato
                    sem_wait(t);
                    // sleep(10); 
                    veggies->TO_tomweight += veggies->TO_tom_dif;
                    veggies->TO_tomweight += veggies->tom1;
                    veggies->TO_tomweight += veggies->tom2;
                    veggies->TO_tomweight += veggies->tom3;
                    veggies->tom1 = 0;
                    veggies->tom2 = 0;
                    veggies->tom3 = 0;
                    printf("TO: I grabbed %d g tomato\n", veggies->TO_tomweight);
                    //2 Log
                    sem_wait(out);
                    fprintf(outfile,"TO: I grabbed %d g tomato %s\n", veggies->TO_tomweight, asctime (timeinfo));
                    sem_post(out);
                    //Grab and weigh onion
                    sem_wait(o); 
                    veggies->TO_onweight += veggies->TO_on_dif;
                    veggies->TO_onweight += veggies->on1;
                    veggies->TO_onweight += veggies->on2;
                    veggies->on1 = 0;
                    veggies->on2 = 0;
                    printf("TO: I grabbed %d g onions off workbench\n", veggies->TO_onweight);
                    //3 Log
                    sem_wait(out);
                    fprintf(outfile,"TO: I grabbed %d g onions %s\n", veggies->TO_onweight, asctime (timeinfo));
                    sem_post(out);
                    veggies->c = veggies->c + 1; 
                    sem_post(to); // ("I am done") - Now chef can issue wait
                    sleep(15); //Wait for chef to get I am done signal 
                    //This is where other processes can work in parallel
                    printf("TO: I got the tomato and the onion I needed!\n");
                    printf("TO: Time to chop and keep the difference for next time\n");
                    sleep(printRandoms(time1,time2,1));
                    //End time
                    //4 Log
                    sem_wait(out);
                    fprintf(outfile,"TO: I made the salad %s\n", asctime (timeinfo));
                    sem_post(out);
                    printf("%d salads made\n",veggies->c);
                    // Stop measuring time and calculate the working time
                    gettimeofday(&end, 0);
                    seconds = end.tv_sec - begin.tv_sec;
                    microseconds = end.tv_usec - begin.tv_usec;
                    elapsed = seconds + microseconds*1e-6;
                    printf("TO Time making: %.3f seconds.\n", elapsed);//End time
                    veggies->TO_make += elapsed;
                    sleep(10);
                }

                }  

                if (maker % 3 == 1){
                //Need TomPep
                while (veggies->c < veggies->numSalads)
                {
                    // outfile = fopen("outfile","a");
                    printf("TP: I'm waiting to grab tomato and pepper\n");
                    //1 Log
                    sem_wait(out);
                    fprintf(outfile,"TP: I'm waiting to grab tomato and pepper %s\n", asctime (timeinfo));
                    sem_post(out);
                    printf("Added onion to the cutting board!\n");
                    // Start measuring waiting time
                    struct timeval begin, end;
                    gettimeofday(&begin, 0);
                    //TP turn to get veggies
                    sem_wait(tp);
                    sem_getvalue(tp, &val);
                    printf("After wait: TP = %d\n", val );
                    // sleep(20);
                    // Stop measuring time and calculate the elapsed time
                    gettimeofday(&end, 0);
                    long seconds = end.tv_sec - begin.tv_sec;
                    long microseconds = end.tv_usec - begin.tv_usec;
                    double elapsed = seconds + microseconds*1e-6;
                    printf("TP Time waited: %.3f seconds.\n", elapsed);//End time
                    time ( &rawtime );
                    timeinfo = localtime ( &rawtime );
                    sem_wait(out);
                    fprintf ( outfile, "TP time: %s", asctime (timeinfo) );
                    sem_post(out);
                    sem_getvalue(out, &val);
                    printf("TP: Out = %d\n", val);
                    veggies->TP_wait += elapsed;
                    // Start measuring working time
                    gettimeofday(&begin, 0);
                    //Grab tomato
                    sem_wait(t); 
                    veggies->TP_tomweight += veggies->TP_tom_dif;
                    veggies->TP_tomweight += veggies->tom1;
                    veggies->TP_tomweight += veggies->tom2;
                    veggies->TP_tomweight += veggies->tom3;
                    veggies->tom1 = 0;
                    veggies->tom2 = 0;
                    veggies->tom3 = 0;
                    printf("TP: I grabbed %d g tomato\n", veggies->TP_tomweight);
                    //2 Log
                    sem_wait(out);
                    fprintf(outfile,"TP: I grabbed %d g tomato %s\n", veggies->TP_tomweight, asctime (timeinfo));
                    sem_post(out);
                    //Grab pepper
                    sem_wait(p); //Bug - stuck here - I think chef has changed to too fast so locks out
                    veggies->TP_pepweight += veggies->TP_pep_dif;
                    veggies->TP_pepweight += veggies->pep1;
                    veggies->TP_pepweight += veggies->pep2;
                    veggies->pep1 = 0;
                    veggies->pep2 = 0;
                    printf("TP: I grabbed %d g pepper\n", veggies->TP_pepweight);
                    //3 Log
                    sem_wait(out);
                    fprintf(outfile,"TP: I grabbed %d g pepper %s\n", veggies->TP_pepweight, asctime (timeinfo));
                    sem_post(out);
                    veggies->c = veggies->c + 1;  
                    sem_post(tp); // ("I am done") - Now chef can issue wait
                    sleep(15); //Wait for chef to get I am done signal 
                    printf("TP: I got the tomato and pepper I needed!\n");
                    printf("TP: Time to weigh, chop, and serve\n");
                    //Log 4
                    sem_wait(out);
                    fprintf(outfile,"TP: I made the salad %s\n", asctime (timeinfo));
                    sem_post(out);
                    //Make the salad
                    sleep(printRandoms(time1,time2,1));
                    printf("%d salads made\n",veggies->c);
                    // Stop measuring time and calculate the elapsed time
                    gettimeofday(&end, 0);
                    seconds = end.tv_sec - begin.tv_sec;
                    microseconds = end.tv_usec - begin.tv_usec;
                    elapsed = seconds + microseconds*1e-6;
                    printf("TP Time making: %.3f seconds.\n", elapsed);//End time
                    veggies->TP_make += elapsed;
                    sleep(10);
                    // fclose(outfile);
                }

                }

                if (maker % 3 == 2)
                {


                //Need TomPep
                while (veggies->c < veggies->numSalads)
                {
                    // outfile = fopen("outfile","a");
                    printf("OP: I'm waiting to grab onion and pepper\n");
                    //1 Log
                    sem_wait(out);
                    fprintf(outfile,"OP: I'm waiting to grab onion and pepper %s\n", asctime (timeinfo));
                    sem_post(out);
                    printf("Added tomato to the cutting board!\n");
                    // Start measuring waiting time
                    struct timeval begin, end;
                    gettimeofday(&begin, 0);
                    //OP turn to grab veggies
                    sem_wait(op);
                    sem_getvalue(op, &val);
                    printf("After wait: op = %d\n", val );
                    // sleep(20);
                    // Stop measuring time and calculate the waiting time
                    gettimeofday(&end, 0);
                    long seconds = end.tv_sec - begin.tv_sec;
                    long microseconds = end.tv_usec - begin.tv_usec;
                    double elapsed = seconds + microseconds*1e-6;
                    printf("OP Time waited: %.3f seconds.\n", elapsed);//End time
                   // fprintf(outfile, "OP wait: %lf\n",elapsed);
                    time ( &rawtime );
                    timeinfo = localtime ( &rawtime );
                    sem_getvalue(out, &val);
                    printf("OP: Out = %d\n", val);
                    veggies->OP_wait += elapsed;
                    // Start measuring making time
                    gettimeofday(&begin, 0);
                    //Grab onion
                    sem_wait(o); 
                    veggies->OP_onweight += veggies->OP_on_dif;
                    veggies->OP_onweight += veggies->on1;
                    veggies->OP_onweight += veggies->on2;
                    veggies->on1 = 0;
                    veggies->on2 = 0;
                    printf("OP: I grabbed %d g onions off workbench\n",  veggies->OP_onweight);
                    //2 Log
                    sem_wait(out);
                    fprintf(outfile,"TP: I grabbed %d g onions %s\n", veggies->OP_onweight, asctime (timeinfo));
                    sem_post(out);
                    //Grab pepper
                    sem_wait(p);
                    veggies->OP_pepweight += veggies->OP_pep_dif;
                    veggies->OP_pepweight += veggies->pep1;
                    veggies->OP_pepweight += veggies->pep2;
                    veggies->pep1 = 0;
                    veggies->pep2 = 0;
                    printf("OP: I grabbed %d g pepper\n", veggies->OP_pepweight);
                    //3 Log
                    sem_wait(out);
                    fprintf(outfile,"OP: I grabbed %d g pepper %s\n", veggies->OP_pepweight, asctime (timeinfo));
                    sem_post(out);
                    veggies->c = veggies->c + 1; 
                    sem_post(op); // ("I am done") - Now chef can issue wait
                    sleep(15); //Wait for chef to get I am done signal 
                    printf("Op: I got the tomato and pepper I needed!\n");
                    printf("Op: Time to weigh, chop, and serve\n");
                     //Log 4
                    sem_wait(out);
                    fprintf(outfile,"OP: I made the salad %s\n", asctime (timeinfo));
                    sem_post(out);
                    //Make the salad
                    sleep(printRandoms(time1,time2,1));
                    printf("%d salads made\n",veggies->c);
                    // Stop measuring time and calculate the waiting time
                    gettimeofday(&end, 0);
                    seconds = end.tv_sec - begin.tv_sec;
                    microseconds = end.tv_usec - begin.tv_usec;
                    elapsed = seconds + microseconds*1e-6;
                    printf("OP Time making: %.3f seconds.\n", elapsed);//End time
                    veggies->OP_make += elapsed;
                    sleep(10);
                    // fclose(outfile);
                }

                }

        }


    
        // wait();

    }

        fclose(outfile);

      
        sem_close(to);
        sem_close(tp);
        sem_close(op);
        sem_close(t);
        sem_close(o);
        sem_close(p);
        sem_close(out);

        sem_unlink("/to_name");
        sem_unlink("/tp_name");
        sem_unlink("/op_name");
        sem_unlink("/t_name");
        sem_unlink("/p_name");
        sem_unlink("/o_name");
        sem_unlink("/out_name");
       



    
    
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
    printf("DONE\n");
    return 0;



}