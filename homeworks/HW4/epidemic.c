#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum TYPE {S, I, R};

//idx returns an integer to be used for hashing
//this integer should be unique for every x, y pair in your grid
int idx(int x, int y, int k) {
    assert(-k <= x && x <= k && -k <= y && y <= k);
    return (2 * k + 1) * (x + k) + (y + k);
}

typedef struct Host {
    int id;
    int x, y;
    int t;
    enum TYPE type;
} THost;

typedef struct node_tag {
   THost host;
   struct node_tag * next;
} node;

//create a node whose value is a specific host
//return a pointer to the created node
node * create_node(THost host)  {
    node * new_node = malloc(sizeof(node));
    assert(new_node != NULL);
    new_node->host = host;
    new_node->next = NULL;
    return new_node;
}

//add_first() should add to the beginning of a linked list
//note the type: 'node **head'
//note that it does not return a value
void add_first(node **head, node *newnode) {
    assert(newnode->next == NULL);
    newnode->next = *head;
    *head = newnode;
}

//remove the first node from the list
// NOTE: THIS DOES NOT FREE THE NODE! IT GIVES THAT RESPONSIBILITY TO THE CALLER!
node * remove_first(node **head) {
    assert(*head != NULL);
    node *first = *head;
    *head = first->next;
    first->next = NULL;
    return first;
}

//remove all the nodes in the list
//and free all the allocated memory
void remove_all(node **head) {
    node * current = *head;
    while (current != NULL) {
        node * tmp = current->next;
        free(current);
        current = tmp;
    }
    *head = NULL;
}

//location_match checks whether a linked list contains
//one or more hosts in the same location as 'victim'
//return 1 if there is a match, 0 if not
_Bool location_match(node *head, THost victim) {
    for (node *current = head; current != NULL; current = current->next) {
        if (current->host.x == victim.x && current->host.y == victim.y && current->host.type == I) {
            return 1;
        }
    }
    return 0;
}


//hash function included for your convenience :)
unsigned hash(unsigned a) {
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

//summary prints out the proportions of different host types.
//It returns 1 if the number of infected hosts is not 0.
int summary(int m, THost hosts[static m]) {
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
    
    return I_n > 0;
}

// one_round
_Bool one_round(
    int m, THost *hosts, // THost hosts[static m]
    int n_arr, node *p_arr[static n_arr],
    int k, int T
) {
    //S -> I and I -> R
    for (int i = 0; i < m; i++) {
        if (hosts[i].type == S) {
            // finish the following line of code
            int index = hash(idx(hosts[i].x, hosts[i].y, k)) % n_arr; // warning: implicit cast from `unsigned int` to `int`
            if (location_match(p_arr[index], hosts[i])) {
                //TODO: fill in what should happen here (not long)
                hosts[i].type = I;
            }
        } else if (hosts[i].type == I) {
            //TODO: fill in what should happen here (not long)
            hosts[i].t += 1;
            if (hosts[i].t == T) hosts[i].type = R;
        }
    }
    
    //reset all linked lists
    for (int i = 0; i < n_arr; ++i) remove_all(&p_arr[i]);
    
    for (int i = 0; i < m; i++) {
        int r = rand() % 4;
        // finish the follow code
        // 0: up, 1: right, 2: down, 3: left
        switch(r) {
            case 0: hosts[i].y++; break;
            case 1: hosts[i].x++; break;
            case 2: hosts[i].y--; break;
            case 3: hosts[i].x--; break;
        }
        
        if (hosts[i].y < -k) hosts[i].y += 2*k+1;
        else if (hosts[i].y > k) hosts[i].y -= 2*k+1;
        else if (hosts[i].x < -k) hosts[i].x += 2*k+1;
        else if (hosts[i].x > k) hosts[i].x -= 2*k+1;
        
        // this is legitimately THE most brain damaged snippet of code i have ever seen.
        // this is like building a new office building, using it for a month, and then
        // tearing it down again, thousands and thousands of times. and thats not even
        // MENTIONING how shitty of a data structure linked lists are. OR the fact that
        // `idx` doesnt have any fucking hash collisions... UNTIL YOU PASS IT THROUGH
        // THE `hash` FUNCTION!!! LIKE HOLY SHIT. IF YOU ARE A FUCKING PROFESSOR WHO IS
        // TEACHING THE C PROGRAMMING LANGUAGE, YOU SHOULD PROBABLY KNOW HOW TO FUCKING
        // USE THE C PROGRAMMING LANGUAGE!
        
        //buid linked list for I hosts
        if (hosts[i].type == I) {
            node *r = create_node(hosts[i]);
            int index = hash(idx(hosts[i].x, hosts[i].y, k)) % n_arr;
            add_first(&(p_arr[index]), r);
        }
    }
    
    return summary(m, hosts);
}

int main(int argc, char *argv[]) {
    if(argc != 5) {
        printf("Usage: %s k m T N\n", argv[0]);
        return 0;
    }
    
    int k = atoi(argv[1]);
    int m = atoi(argv[2]);
    int T = atoi(argv[3]);
    int N = atoi(argv[4]);
    
    assert(k >= 0 && k <= 1000);
    assert(m >= 1 && m <= 100000);
    assert(T >= 1);
    assert(N > 0 && N <= 100000);
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
    
    // NOTE: this cannot be zero-initialized as usual b.c. its a VLA
    node *(p_arr[N]); // array of `node*`
    for (int i = 0; i < N; i++) { p_arr[i] = NULL; }
    
    node *r = create_node(hosts[0]);
    int index = hash(idx(hosts[0].x, hosts[0].y, k)) % N;
    add_first(&(p_arr[index]), r);
    
    //simulation
    while (one_round(m, hosts, N, p_arr, k, T));
    
    return 0;
}
