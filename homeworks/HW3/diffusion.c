#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Point3D { int x, y, z; } Point3D_t;

// small helper function for 3D array indexing
static inline size_t grid_offset(unsigned int n, Point3D_t p) {
	return (p.x+n-1)*(2*n-1)*(2*n-1) + (p.y+n-1)*(2*n-1) + (p.z+n-1);
}

// simulate where a particle ends up after n random steps in 3D space.
Point3D_t one_particle(const unsigned int n) {
	int current_x = 0, current_y = 0, current_z = 0;
	for (int _ = 0; _ < n; ++_) {
		switch (rand() % 6) {
			case 0: --current_x; break;
			case 1: ++current_x; break;
			case 2: --current_y; break;
			case 3: ++current_y; break;
			case 4: ++current_z; break;
			case 5: --current_z; break;
		}
	}
	return (Point3D_t) { current_x, current_y, current_z };
}

// return the fraction of particles that lie at most distance r*n from the origin
double density(const unsigned int * const grid, const int n, const double r) {
    const double L = n * r;
	unsigned int num_particles = 0;
	
	// NOTE: ideally this should not be calculated, and instead should just be
	//       passed in from the caller (it'll be equal to `m`)
	unsigned int total_particles = 0;
	
	Point3D_t p;
	for (p.x = 1-n; p.x < n; ++p.x)
	for (p.y = 1-n; p.y < n; ++p.y)
	for (p.z = 1-n; p.z < n; ++p.z) {
		total_particles += grid[grid_offset(n, p)];
		if (p.x*p.x + p.y*p.y + p.z*p.z <= L*L) {
			num_particles += grid[grid_offset(n, p)];
		}
	}
	
	return (double)num_particles / total_particles;
}

//use this function to print results
void print_result(const unsigned int n, const unsigned int * const grid) {
    printf("radius density\n");
    for(int k = 1; k <= 20; k++)
        printf("%.2lf   %lf\n", 0.05*k, density(grid, n, 0.05*k));
}

// n is the size of the grid (-n, n)
// m is the number of particles to simulate
void diffusion(const unsigned int n, const size_t m) {
	// unsigned int particle_grid[2*n-1][2*n-1][2*n-1] = ...;
	unsigned int *particle_grid = calloc((2*n-1)*(2*n-1)*(2*n-1), sizeof(unsigned int));
	
	// populate the grid with all final positions of particles
	for(int i = 1; i <= m; i++) particle_grid[grid_offset(n, one_particle(n))]++;
	
	// print results
	print_result(n, particle_grid);
	
	// done with particle_grid
	free(particle_grid);
}

int main(int argc, char *argv[]) {
	if(argc != 3) {
		printf("Usage: %s n m\n", argv[0]);
		return 0; 
	}
	
	unsigned int n = abs(atoi(argv[1]));
	unsigned int m = abs(atoi(argv[2]));
	assert(n - 1 <= 49);
	assert(m - 1 <= 999999);
	
	srand(12345);
	diffusion(n, m);
	
	return 0;
}
