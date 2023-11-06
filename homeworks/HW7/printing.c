#define _BSD_SOURCE
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

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

// error handling functions

void die(char *s) {
    if (errno) perror(s);
    else fprintf(stderr, "Error: %s\n", s);
    exit(EXIT_FAILURE);
}

/*************  BEIGINNING OF QUEUE *********************/
/* Impelementation of Q. Not a small, fixed-size buffer, but good enough for this assignment. */

// init q, add random max_jobs jobs, seed is set before this function
int q_init(job_queue_t *q, int max_jobs) {
    q->njobs_fetched = 0;
    q->njobs_max = max_jobs;
    q->jobs = malloc(max_jobs);
    if (q->jobs == NULL)
        die("malloc()");
    for (int i = 0; i < max_jobs; i++)
        q->jobs[i] = rand() % 100 + 1;
    if (pthread_mutex_init(&q->mutex, NULL))
        die("pthread_mutex_init()");
    return 0;
}

// get the number of jobs to be printed
// Return values:
//      non-negative integer:  the number of remaining jobs in the queue
int q_num_jobs(job_queue_t *q) {
    return (q->njobs_max - q->njobs_fetched);
}

// Printer id fetches a job to print
// Check the number of jobs in the queue before calling this function!
// It is an error to fetch a job from an empty queue!
//
// Return value:
// value > 0: a value indicating the time needed for the job.
int q_fetch_job(job_queue_t *q, int id) {
    if (q_num_jobs(q) <= 0) {
        fprintf(stderr, "Error: printer %2d tries to get a job "
               "after the max number of jobs have been fetched.", id);
        exit(EXIT_FAILURE);
    }
    printf("Printer  %2d fetched   job %2d\n", id, q->njobs_fetched);
    return q->jobs[q->njobs_fetched++];
}

// This function frees memory dynamically allocated for a queue
// This function does not return a value
void q_destroy(job_queue_t *q) {
    if (q->jobs) {
        free(q->jobs);
        q->jobs = NULL;
    }
    if (pthread_mutex_destroy(&q->mutex))
        die("pthread_mutex_destroy()");
}

/*************  END OF QUEUE *********************/

void print_printer_summary(printer_t *pprinter, int nprinters) {
    int total = 0;
    for (int i = 0; i < nprinters; i++, pprinter++) {
        printf("Printer  %2d completed %d jobs.\n", pprinter->id, pprinter->njobs);
        total += pprinter->njobs;
    }
    printf("Total completed is %d.\n", total);
}

void printer_single(printer_t *pprinter) {
    job_queue_t *jq = pprinter->jq;
    
    while (q_num_jobs(jq) > 0) {
        // print the job
        print_job(q_fetch_job(jq, pprinter->id));
        
        // keep track of how many jobs are printed by this printer
        pprinter->njobs++;
    }
}


// returns -1 if q has no more jobs left
int threaded_q_fetch_job(job_queue_t *q, int id) {
    if (pthread_mutex_lock(&q->mutex)) die("pthread_mutex_lock()");
    int fetched_job = q_num_jobs(q) > 0 ? q_fetch_job(q, id) : -1;
    if (pthread_mutex_unlock(&q->mutex)) die("pthread_mutex_lock()");
    return fetched_job;
}

/* main function for printers */
void *printer_main(printer_t *self) {
    // printer's current job: -1 means no job (done)
    int current_job = threaded_q_fetch_job(self->jq, self->id);
    
    while (current_job != -1) {
        // run job
        print_job(current_job);
        self->njobs++;
        
        // fetch next job
        current_job = threaded_q_fetch_job(self->jq, self->id);
    }
    
    return NULL;
}

int main(int argc, char *argv[]) {
    int num_printers = DEFAULT_NUM_PRINTERS;
    int num_jobs = DEFAULT_NUM_JOBS;
    int demo = 0;
    
    int i;
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
    
    // define job_queue and initialize it
    job_queue_t job_queue;
    
    q_init(&job_queue, num_jobs);
    
    printer_t printers[num_printers];
    
    if (demo) {
        printf("Demo: there is only one printer.\n");
        printers[0].id = 0;
        printers[0].jq = &job_queue;
        printers[0].njobs = 0;
        printer_single(printers);
        q_destroy(&job_queue);
        print_printer_summary(printers, 1);
        exit(0);
    }
    
    for (int i = 0; i < num_printers; ++i) {
        printers[i] = (printer_t) { .id = i, .jq = &job_queue };
        // NOTE: this means that inside of `printer_main`, `self->thread_id` may still be 0.
        if (pthread_create(&printers[i].thread_id, NULL, (void*(*)(void*))printer_main, &printers[i]))
            die("pthread_create()");
    }
    
    // wait for all printers to complete
    for (int i = 0; i < num_printers; ++i) {
        if (pthread_join(printers[i].thread_id, NULL))
            die("pthread_join()");
    }
    
    // if only there was some way to ensure that this was ran whenever job_queue went out of scope
    q_destroy(&job_queue);
    
    print_printer_summary(printers, num_printers);
    
    return 0;
}
