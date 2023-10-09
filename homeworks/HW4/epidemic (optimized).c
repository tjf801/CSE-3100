#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

enum TYPE {S, I, R};

//idx returns an integer to be used for hashing
//this integer should be unique for every x, y pair in your grid
static inline int idx(int x, int y, int k) {
    // assert(-k <= x && x <= k && -k <= y && y <= k);
    return (2 * k + 1) * (x + k) + (y + k);
}

typedef struct Host {
    int id;
    int x, y;
    int t;
    enum TYPE type;
} THost;

//summary prints out the proportions of different host types.
void print_summary(int m, THost *hosts) {
    int S_n, I_n, R_n;
    
    S_n = I_n = R_n = 0;
    for (int i = 0; i < m; i++) {
        S_n += (hosts[i].type == S);
        I_n += (hosts[i].type == I);
        R_n += (hosts[i].type == R);
    }
    
    if (I_n == 0) {
        printf("    S        I        R\n");
        printf("%lf %lf %lf\n", (double)S_n/(S_n + I_n + R_n),
        (double)I_n/(S_n + I_n + R_n), (double)R_n/(S_n + I_n + R_n));
    }
}

// one_round
_Bool one_round(const int m, THost *hosts, const int k, const int T, _Bool * const infected_squares) {
    memset(infected_squares, 0, (2*k+1) * (2*k+1));
    unsigned int num_infected = 0;
    for (int i = 0; i < m; i++) {
        if (hosts[i].type == I) {
            infected_squares[idx(hosts[i].x, hosts[i].y, k)] = 1;
            num_infected++;
        }
    }
    
    //S -> I and I -> R
    for (int i = 0; i < m; i++) {
        const unsigned int dir = rand() & 0b11u; // 0: up, 1: right, 2: down, 3: left
        
        if (hosts[i].type == S) {
            // finish the following line of code
            if (infected_squares[idx(hosts[i].x, hosts[i].y, k)]) {
                hosts[i].type = I;
                num_infected++;
            }
        } else if (hosts[i].type == I) {
            hosts[i].t += 1;
            if (hosts[i].t == T) {
                hosts[i].type = R;
                num_infected--;
            }
        } else continue;
        
        switch (dir) {
            case 0: hosts[i].y = (hosts[i].y + k + 1) % (2*k+1) - k; break;
            case 1: hosts[i].x = (hosts[i].x + k + 1) % (2*k+1) - k; break;
            case 2: hosts[i].y = (hosts[i].y - k - 1) % (2*k+1) + k; break;
            case 3: hosts[i].x = (hosts[i].x - k - 1) % (2*k+1) + k; break;
            default: __builtin_unreachable();
        }
    }
    
    if (num_infected > 0) return 1;
    
    print_summary(m, hosts);
    return 0;
}

int main(int argc, char *argv[]) {
    if(argc != 5) {
        printf("Usage: %s k m T N\n", argv[0]);
        return 0;
    }
    
    int k = atoi(argv[1]);
    int m = atoi(argv[2]);
    int T = atoi(argv[3]);
    // int N = atoi(argv[4]);
    
    assert(k >= 0 && k <= 1000);
    assert(m >= 1 && m <= 100000);
    assert(T >= 1);
    // assert(N > 0 && N <= 100000);
    srand(12345);
    
    //initialize hosts
    THost hosts[m];
    hosts[0] = (THost) { .type = I };
    for (int i = 1; i < m; ++i)
        hosts[i] = (THost) {
            .id = i,
            .x = rand() % (2*k + 1) - k,
            .y = rand() % (2*k + 1) - k,
            .type = S
        };
    
    _Bool * infected_squares = calloc((2*k+1) * (2*k+1), sizeof(_Bool));
    
    //simulation
    while (one_round(m, hosts, k, T, infected_squares));
    
    free(infected_squares);
    
    return 0;
}
