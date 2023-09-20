//In this assignment, we write code to solve the cloest-pair problem.
//Given n (n > 1) points on a plan (a 2 dimentional surface), 
//find the cloest pair of points, i.e., the pair of points
//that has the shortest distance.

//If you learned CSE3500, you know we can solve this problem in O(n log n) time
//But that is not the focus of this assignment. 
//An algorithm with O(n^2) running time works fine for this assignment.

//The purpose of this assignment is to practice using structures.
//We use a structure Point_t to represent a point on a plane.
//We use another structure PointPair_t to represent a pair of points.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define N 10000

typedef struct Point {
	unsigned int id;
	double x, y;
} Point_t;

typedef struct Pair {
	Point_t p1, p2;
} PointPair_t;

// the squared distance between two points.
static inline double squared_dist(const Point_t p1, const Point_t p2) {
	return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}

// return the closest two points given in the array `points[]` (which has length `n`).
PointPair_t shortest_dist_pair(const unsigned int n, const Point_t points[]) {
	double min_d = squared_dist(points[0], points[1]);
	PointPair_t p_pair = { points[0], points[1] };
	
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			double dist = squared_dist(points[i], points[j]);
			if (dist < min_d) {
				min_d = dist;
				p_pair = (PointPair_t) { points[i], points[j] };
			}
		}
	}
	
    return p_pair;
}

//Do not change the following code
int main(int argc, char *argv[])
{
	Point_t points[N];
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
	
	PointPair_t p_pair = shortest_dist_pair(n, points);
	
	// make sure the first point has smaller id
	if (p_pair.p1.id > p_pair.p2.id) {
		Point_t tmp;
		tmp = p_pair.p1; 
		p_pair.p1 = p_pair.p2; 
		p_pair.p2 = tmp;
		// SAFETY: shortest_dist_pair always iterates s.t. p1.id < p2.id
		// if the passed in list is sorted by ids, which it is.
		__builtin_unreachable();
	}
	
	printf("%05d %lf %lf\n", p_pair.p1.id, p_pair.p1.x, p_pair.p1.y);
	printf("%05d %lf %lf\n", p_pair.p2.id, p_pair.p2.x, p_pair.p2.y);
	
	return 0;
}
