#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <assert.h>

typedef struct {
	double x, y;
} point_t;

typedef struct thread_arg_tag {
    int id;
    int k, m;
    point_t *a;
    int rounds;
    point_t *centroids;
    double *p_squared_intra_cluster_dist;

} thread_arg_t;

void die(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
    if (errno)
        perror("errno");
    exit(EXIT_FAILURE);
}

void check_pthread_return(int rv, char *msg)
{
    if (rv == 0)
        return;
    errno = rv;
    die(msg);
}

void generate_samples(double x, double y, double r, point_t a[], int n)
{ 
    for(int i = 0; i < n; i++) {
        double radius = ((float)rand()/(float)(RAND_MAX))*r;
        double theta = 2*M_PI*((float)rand()/(float)(RAND_MAX));
        a[i].x = x + radius*cos(theta);
        a[i].y = y + radius*sin(theta);
    }
}
void generate_all_samples(point_t a[], int n) 
{
    srand(3100);
    generate_samples(1., 1., 0.5, a, n);
    generate_samples(2., 2., 0.5, a+n, n);
    generate_samples(4., 4., 0.5, a+2*n, n);
    generate_samples(8., 8., 0.5, a+3*n, n);
}

double dist2(point_t a, point_t b) {
    return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}

void compute_labels(point_t centroids[], int k, point_t a[], int n, int labels[])
{
    for(int i = 0; i < n; i++) {
        double min_d2 = dist2(centroids[0], a[i]);
        int label = 0;
        for(int j = 0; j < k; j++) {
            double d2 = dist2(centroids[j], a[i]);
            if(d2 < min_d2)
            {
                min_d2 = d2;
                label = j;
            }
        }
        labels[i] = label;
    }
}

void compute_centroids(point_t centroids[], int k, point_t a[], int n, int labels[])
{
    for(int j = 0; j < k; j++) {
        double x_sum, y_sum;

        x_sum = y_sum = 0.;
        int count = 0;
        for(int i = 0; i < n; i++) {
            if(labels[i]==j)
            {
                x_sum += a[i].x;
                y_sum += a[i].y;
                count ++;
            }
        }
        centroids[j].x = x_sum/count;
        centroids[j].y = y_sum/count;
    }
}

double squared_intra_cluster_dist(point_t centroids[], int k, point_t a[], int n, int labels[])
{
    double sum = 0.;
    for(int i = 0; i < n; i++)
        sum += dist2(a[i], centroids[labels[i]]);
    return sum/n;
}

void print_clusters(point_t *centroids, int k, point_t *a, int m, int *labels)
{
    printf("centroids:\n");
    for(int j = 0; j < k; j++)
        printf("%.2lf %.2lf\n", centroids[j].x, centroids[j].y);
    
    compute_labels(centroids, k, a, m, labels);
    int freq[k];
    for(int i = 0; i < k; i++)
        freq[i] = 0;

    for(int i = 0; i < m; i++)
        freq[labels[i]]++;

    for(int i = 0; i< k; i++)
        printf("%d\n", freq[i]);
}

double clusters(point_t a[], int labels[], int m, int k, int trials, int rounds)
{
    point_t centroids[k], best_centroids[k];
    double best_d;

    for(int i = 0; i < trials; i++)
    {
        for(int j = 0; j < k; j++) {
            int idx = rand() % m;
            centroids[j].x = a[idx].x;  centroids[j].y = a[idx].y;
        }

        for(int l = 0; l < rounds; l++)
        {
            compute_labels(centroids, k, a, m, labels);
            compute_centroids(centroids, k, a, m, labels);
        }

        double d = squared_intra_cluster_dist(centroids, k, a, m, labels);
        if(i==0 || d < best_d) 
        {
            best_d = d;
            for(int j = 0; j < k; j++)
            {
                best_centroids[j].x = centroids[j].x;
                best_centroids[j].y = centroids[j].y;
            }
        } 
    }
    print_clusters(best_centroids, k, a, m, labels);
    return best_d;
}

void* thread_main(void* thread_arg) {
    thread_arg_t* arg = thread_arg;
    //TODO: finish the following statements
    unsigned int id = ;
    int k = ;
    int m = ;
    point_t *a = ;
    int rounds = ;
    point_t *centroids = ;
    double *p_squared_intra_cluster_dist = ;

    for(int j = 0; j < k; j++) {
        int idx = rand_r(&id) % m;
        centroids[j].x = a[idx].x;  centroids[j].y = a[idx].y;
    }

    int *labels = malloc(m*sizeof(int));
    assert(labels!=NULL);  

    //TODO: fill in code below

    
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Usage: %s n [demo]\n", argv[0]);
        exit(0);
    }
    int n = atoi(argv[1]);
    assert(n >= 1000 && n <= 100000);
    int k = 4;
    int m = k*n;
    point_t a[m];
    int labels[m];

    generate_all_samples(a, n);
    int trials = 20;
    int rounds = 10;

    if(argc > 2)
    {
        printf("Squared intra-cluster distance: %lf\n", clusters(a, labels, m, k, trials, rounds));
        exit(0);
    }

    int n_threads = trials;
    pthread_t thread_arr[n_threads];
    thread_arg_t thread_args[n_threads];
    point_t centroids[n_threads][k];
    double squared_intra_cluster_dist[n_threads];
    int rv;

    // Set up thread arguments
    for(int i = 0; i < n_threads; i++) {
	    thread_args[i].id = i;
        thread_args[i].k = k;
        thread_args[i].m = m;
        thread_args[i].a = a;
        thread_args[i].rounds = rounds;
        thread_args[i].centroids = (point_t *)&centroids[i];
        thread_args[i].p_squared_intra_cluster_dist = &squared_intra_cluster_dist[i];
    }

    // TODO: Create the threads


    // TODO: Join with the threads

    
    // TODO: Obtain the best clustering result and print it
    double best_d = ;
    point_t *best_centroids = ;


    print_clusters(best_centroids, k, a, m, labels);
    printf("Squared intra-cluster distance: %lf\n", best_d);

    return 0;
}