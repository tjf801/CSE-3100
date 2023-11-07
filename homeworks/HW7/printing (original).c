#define _BSD_SOURCE
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Search TODO to find the locations where code needs to be completed

// error handling functions

void die(char *s)
{
    if (errno)
        perror(s);
    else 
        fprintf(stderr, "Error: %s\n", s);
    exit(EXIT_FAILURE);
}

void check_pthread_return(int rv, char *msg)
{
    if (rv == 0)
        return;
    errno = rv;
    die(msg);
}

#define DEFAULT_NUM_JOBS    20
#define DEFAULT_NUM_PRINTERS 2

#define print_job(x) usleep((x)*100 + 50)

typedef struct {
    unsigned char *jobs;            // buffer that keeps job info
    int     njobs_fetched;          // number of jobs that have fetched by printer
    int     njobs_max;              // number of jobs to perform
    pthread_mutex_t mutex;          // mutex to protect the queue, which is shared by all threads
} job_queue_t;

// The computers and printers can share the same structure
typedef struct {
    int id;
    job_queue_t * jq;
    int njobs;   // number of jobs printed
    pthread_t thread_id; // thread id
} printer_t;

/*************  BEIGINNING OF QUEUE *********************/
/* Impelementation of Q. Not a small, fixed-size buffer, but good enough for this assignment. */

// init q, add random max_jobs jobs, seed is set before this function
int q_init(job_queue_t *q, int max_jobs)
{
    q->njobs_fetched = 0;
    q->njobs_max = max_jobs;
    q->jobs = malloc(max_jobs);
    if (q->jobs == NULL)
        die("malloc()");
    for (int i = 0; i < max_jobs; i ++)
        q->jobs[i] = rand() % 100 + 1;
    return 0;
}

// get the number of jobs to be printed
// Return values:
//      non-negative integer:  the number of remaining jobs in the queue
int q_num_jobs(job_queue_t *q)
{
    return (q->njobs_max - q->njobs_fetched);
}

// Printer id fetches a job to print
// Check the number of jobs in the queue before calling this function!
// It is an error to fetch a job from an empty queue!
//
// Return value:
// value > 0: a value indicating the time needed for the job.
int q_fetch_job (job_queue_t *q, int id)
{
    int r = q_num_jobs(q);
    if (r <= 0) {
        fprintf(stderr, "Error: printer %2d tries to get a job "
               "after the max number of jobs have been fetched.", id);
        exit(EXIT_FAILURE);
    }
    printf("Printer  %2d fetched   job %2d\n", id, q->njobs_fetched);
    return q->jobs[q->njobs_fetched ++];
}

// This function frees memory dynamically allocated for a queue
// This function does not return a value
void q_destroy(job_queue_t *q)
{
    if (q->jobs) {
        free(q->jobs);
        q->jobs = NULL;
    }
}

/*************  END OF QUEUE *********************/

void print_printer_summary(printer_t *pprinter, int nprinters)
{
    int  total = 0;

    for (int i = 0; i < nprinters; i ++, pprinter ++) {
        printf("Printer  %2d completed %d jobs.\n", pprinter->id, pprinter->njobs);
        total += pprinter->njobs;
    }
    printf("Total completed is %d.\n", total);
}

void printer_single(printer_t *pprinter)
{
    int done = 0;
    job_queue_t *jq = pprinter->jq;

    // keep track of how many jobs are printed by this printer
    pprinter->njobs = 0;
    while (! done) {
        int     r;
        r = q_num_jobs(jq); // get the number of remaining jobs
        if (r <= 0) {       // done ?
            done = 1;
            continue;
        }
        int job = q_fetch_job(jq, pprinter->id);

        // print the job
        print_job(job);

        pprinter->njobs ++;
    }
}

/* main function for printers */
void * printer_main(void * arg)
{
    // TODO
    return arg;
}

int main(int argc, char *argv[])
{
    int num_printers = DEFAULT_NUM_PRINTERS;
    int num_jobs = DEFAULT_NUM_JOBS;
    int demo = 0;

    int i, status;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            num_printers = atoi(argv[++i]);
            if (num_printers <= 0)
                die("number of printers must be positive.");
        } else if (strcmp(argv[i], "-j") == 0 && i + 1 < argc) {
            num_jobs = atoi(argv[++i]);
            if (num_jobs <= 0)
                die("number of jobs must be positive.");
        } else if (strcmp(argv[i], "-d") == 0) {
            demo = 1;
        } else {
            fprintf(stderr, "Usage: %s [-p N] [-j N]\n", argv[0]);
            fprintf(stderr, "-p <n> : number of printers.\n");
            fprintf(stderr, "-j <n> : total number of jobs.\n");
            fprintf(stderr, "-d     : demo mode.\n");
            return 1;
        }
    }
    printf("num_printers=%d\n", num_printers);
    printf("num_jobs=%d\n", num_jobs);

    int seed = (num_jobs << 24) ^ (num_printers << 8);
    srand(seed);

    // insert necessary init and destroy functions below

    // define job_queue and initialize it
    job_queue_t job_queue;

    q_init(&job_queue, num_jobs);

    printer_t printers[num_printers];

    if (demo) {
        printf("Demo: there is only one printer.\n");
        printers[0].id = 0;
        printers[0].jq = &job_queue;
        printer_single(printers);
        q_destroy(&job_queue);
        print_printer_summary(printers, 1);
        exit(0);
    }

    /* TODO: 
     *      prepare arguments to threads
     *      create threads
     *      wait for other threads
     *  Also, properly init and desctroy mutex.
     *  */

    q_destroy(&job_queue);

    print_printer_summary(printers, num_printers);

    return 0;
}
