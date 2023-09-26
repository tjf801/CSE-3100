#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Player {
    // NOTE: `.id` cannot be zero!
    unsigned int id;
    size_t loc;
    unsigned long balance;
} Player_t;

typedef struct Property {
    // `.id = 0` is the "GO" space
    size_t id;
    // `.id = 0` means no owner
    unsigned int owner_id;
    unsigned int rent;
} Property_t;

//The player p1 needs to pay the player p2 'amount' of dollars
//If p1 has enough balance, the transaction will be successful
//Otherwise, p1 will pay all of their balance to p2, and the transaction
//is not successful 
//return 1 if the transaction is successful
//Otherwise, return 0
_Bool transaction(Player_t * const p1, Player_t * const p2, const int amount) {
    if (p1->balance < amount) {
        p2->balance += p1->balance;
        p1->balance = 0;
        return 0;
    }
    p2->balance += amount;
    p1->balance -= amount;
    return 1;
}

//If one player cannot pay rent to another player, this function should return 0. 
//The rest of the players will not get chances to play when this happens
_Bool one_round(
    const size_t m, const size_t n,
    Player_t players[static m],
    Property_t properties[static n]
) {
    for (int i = 0; i < m; i++) {
        const int steps = (rand() % 6 + 1) + (rand() % 6 + 1);
        //fill in the code below
        Player_t * const current_player = &players[i];
        current_player->loc += steps;
        if (current_player->loc >= n) {
            current_player->balance += n;
            current_player->loc -= n;
        }
        Property_t * const current_property = &properties[current_player->loc];
        if (current_property->owner_id == 0) {
            current_property->owner_id = current_player->id;
        } else {
            Player_t * const owner = &players[current_property->owner_id - 1];
            assert(owner->id == current_property->owner_id);
            _Bool trans_successful = transaction(current_player, owner, current_property->rent);
            if (!trans_successful) return 0;
        }
    }
    return 1;
}

//used for printing out results
void print_result(const size_t m, Player_t const players[static m]) {
    printf("      id    balance\n");
    long total_balance = 0, max_balance = 0;
    for(int i = 0; i < m; i++) {
        total_balance += players[i].balance;
        if (players[i].balance > max_balance)
            max_balance = players[i].balance;
        printf("%8d %8ld\n", i, players[i].balance);
    }
    
    printf(
        "average: %f max: %ld, max/average = %lf\n",
        (double)total_balance/m, max_balance,
        (double)max_balance*m/total_balance
    ); 
}

//max_rounds is needed because the game may never finish
void monopoly(
    const size_t m, const size_t n,
    Player_t players[static m],
    Property_t properties[static n],
    const unsigned int max_rounds
) {
    srand(12345);
    
	unsigned int rounds;
    for (rounds = 1; rounds <= max_rounds; ++rounds)
        if (!one_round(m, n, players, properties)) break;
    
    print_result(m, players);
    printf("after %d rounds\n", rounds);
}

int main(int argc, char *argv[]) {
    if(argc != 4) {
        printf("Usage: %s m n rounds\n", argv[0]);
        return -1;
    }
    
    const size_t m = atoi(argv[1]);
    const size_t n = atoi(argv[2]);
    const unsigned int max_rounds = atoi(argv[3]);
    assert(n >= 13);
    assert(m >= 1);
    assert(max_rounds >= 1);
    
    Player_t players[m];
    Property_t properties[n];
    
    for (unsigned int i = 0; i < n; i++)
        properties[i] = (Property_t) { .id = i, .rent = i + 1 };
    
    for (unsigned int j = 1; j <= m; j++)
        players[j - 1] = (Player_t) { .id = j, .balance = n };
    
    monopoly(m, n, players, properties, max_rounds);
    
    return 0;
}
