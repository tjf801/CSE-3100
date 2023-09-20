//In this assignment, we write code to solve the cloest-pair problem.
//Given n (n > 1) points on a plan (a 2 dimentional surface), 
//find the cloest pair of points, i.e., the pair of points
//that has the shortest distance.

//If you learned CSE3500, you know we can solve this problem in O(n log n) time
//But that is not the focus of this assignment. 
//An algorithm with O(n^2) running time works fine for this assignment.

//The purpose of this assignment is to practice using structures.
//We use a structure t_point to represent a point on a plane.
//We use another structure t_pair to represent a pair of points.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define N 10000

typedef struct Point
{
	int id;         // id of the point
	double x, y;    // coordinates of the point
}t_point;

typedef struct Pair
{
	t_point p1, p2;
}t_pair;

//function to calculate the squared distance between two points
//pointed by p1 and p2 respectively
double squared_dist(t_point *p1, t_point *p2)
{
	return (p1->x - p2->x)*(p1->x - p2->x) + (p1->y - p2->y)*(p1->y - p2->y);
}
//function that returns the pointer to the pair of points that has the shortest distance
t_pair *shortest_dist_pair(int n, t_point points[])
{
	int i, j;
	double d;
	t_pair *p_pair;
	p_pair = malloc(sizeof(t_pair));
	if(p_pair == NULL)
	{
		perror("Cannot allocate memory.\n");
		exit(-1);
	}

	double min_d = squared_dist(&(points[0]), &(points[1]));
	p_pair -> p1 = points[0];
	p_pair -> p2 = points[1];
	// TODO fill in the code








    return p_pair;
}
//Do not change the following code
int main(int argc, char *argv[])
{
	t_point points[N];
	if(argc !=2)
	{
		printf("Usage: %s n\n", argv[0]);
		return -1;
	}
	int n = atoi(argv[1]);
	assert(n > 1 && n <= N);
 
	int i;

	//randomly generate n points
	srand(12345);
	for(i=0; i<n; i++)
	{
		points[i].id = i;
		points[i].x = ((double)rand())/RAND_MAX;
		points[i].y = ((double)rand())/RAND_MAX;
	}

	t_pair *p_pair;

	p_pair = shortest_dist_pair(n, points);

        // make sure the first point has smaller id
        if (p_pair->p1.id > p_pair->p2.id) {
            t_point tmp;
            tmp = p_pair->p1; 
            p_pair->p1 = p_pair->p2; 
            p_pair->p2 = tmp;
        }

	printf("%05d %lf %lf\n", p_pair->p1.id, p_pair->p1.x, p_pair->p1.y);
	printf("%05d %lf %lf\n", p_pair->p2.id, p_pair->p2.x, p_pair->p2.y);

        free(p_pair);
	return 0;
}
