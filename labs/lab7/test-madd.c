#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>

#include "matrix.h"

/************************************************************
  Do not change this file 
 ************************************************************/

/* timing functions */
void    stopwatch_start();
double  stopwatch_read();

#define     DEFAULT_NROWS    6
#define     DEFAULT_NCOLS    6

static void check_pointer(void *p, char *s)
{
    if (p)
       return; 
    fprintf(stderr, "%s: Pointer is NULL.\n", s ? s : "");
    exit(EXIT_FAILURE);
}

int main(int argc, char ** argv)
{
    int     nrows = DEFAULT_NROWS;
    int     ncols = DEFAULT_NCOLS;
    int     show_time = 0;
    int     count = 0;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-t", 2) == 0) {
            show_time = atoi(&argv[i][2]);
            if (show_time <= 0) {
                fprintf(stderr, "Number of runs must be a positive integer: %s\n", &argv[i][2]);
                exit(EXIT_FAILURE);
            }
        } else if (isdigit(argv[i][0]) && count < 2) { 
            if (count == 0)
                nrows = atoi(argv[i]);
            else if (count == 1)
                ncols= atoi(argv[i]);
            count ++;
        } else {
            fprintf(stderr, "Usage: %s [nrows [ncols]] [-t]\n"
                            "<nrows> : number of rows. Default value is %d.\n"
                            "<ncols> : number of columns. Default value is %d.\n"
                            "-t      : show execution time.\n"
                            , argv[0], DEFAULT_NROWS, DEFAULT_NCOLS);
            return 1;
        }
    }

    if (! (nrows > 0 && ncols > 0)) {
        fprintf(stderr, "Both nrows and ncols must be positive integers: nrows=%d ncols=%d\n", nrows, ncols);
        exit(EXIT_FAILURE);
    }

    double  time1, time2;

    TMatrix *m, *n;

    m = newMatrix(nrows, ncols);
    check_pointer(m, "m");
    n = newMatrix(nrows, ncols);
    check_pointer(n, "n");

    fillMatrix(m);
    fillMatrix(n);

    TMatrix *t1, *t2;
    stopwatch_start();
    t1 = addMatrix(m, n);
    check_pointer(t1, "t1");
    for (int i = 1; i < show_time; i ++) {
        freeMatrix(t1);
        t1 = addMatrix(m, n);
        check_pointer(t1, "t1");
    }
    time1 = stopwatch_read();
	
    stopwatch_start();
    t2 = addMatrix_thread(m, n);
    check_pointer(t2, "t2");
    for (int i = 1; i < show_time; i ++) {
        freeMatrix(t2);
        t2 = addMatrix_thread(m, n);
        check_pointer(t2, "t2");
    }
    time2 = stopwatch_read();

    int     r = compareMatrix(t1, t2);

    freeMatrix(m);
    freeMatrix(n);
    freeMatrix(t1);
    freeMatrix(t2);

    if (r) {
        printf("%s\n", "Results do not match.");
        exit(EXIT_FAILURE);
    } else {
        printf("%s\n", "Good work!");
    }

    if (show_time) {
        double speedup = 0;
        // only computes speedup if time2 is longer than 1ms
        if (time2 > 0.001) {
            speedup = time1/time2;
        }
        time1 /= show_time;
        time2 /= show_time;
        printf("num_runs=%d time1=%.4f time2=%.4f speedup=%.4f\n", show_time, time1, time2, speedup);
    }
    return 0;
}

/*********** Implementation of a timer ****************/
#define		MAX_RUN_TIME	1000000

#define		TIMER_TYPE	ITIMER_REAL
// #define		TIMER_TYPE	ITIMER_PROF

// start a stop watch, using system timer
void stopwatch_start()
{
    struct itimerval pval;

    /* set the timer*/
    pval.it_value.tv_sec = MAX_RUN_TIME;
    pval.it_value.tv_usec = 0;
    /* No repeating */
    pval.it_interval.tv_sec = 0;     
    pval.it_interval.tv_usec = 0;
    if (setitimer(TIMER_TYPE, &pval, NULL)) {
        fprintf(stderr, "Error: setitimer() failed.\n");
        exit(EXIT_FAILURE);
    }
}

/* Returns the number of seconds since last call to start_timer() 
 * The meaning of seconds depends on the timer type.
 * */
double stopwatch_read()
{
    struct itimerval val;
    long secs, usecs;
    double temp;

    if (getitimer(TIMER_TYPE, &val)) {
        fprintf(stderr, "Error: getitimer() failed.\n");
        exit(EXIT_FAILURE);
    }
    secs = MAX_RUN_TIME - val.it_value.tv_sec - 1;
    usecs = 1000000 - val.it_value.tv_usec;

    temp = (double) secs + ((double) usecs/1000000.0);

    return (temp > 0 ? temp : 0.0 );
}
