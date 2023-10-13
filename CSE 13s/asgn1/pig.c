#include "names.h" //my own files

#include <assert.h>
#include <stdio.h>
#include <stdlib.h> //system files
#define SEED 2023

//Dice Enum
typedef enum { SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER } Position;

//list Positions in a constant array
const Position pig[7] = {
    SIDE, //0
    SIDE, //0
    RAZORBACK, //10
    TROTTER, //10
    SNOUTER, //15
    JOWLER, //5
    JOWLER, //5
};

//Create function dice roll that equates dice rolls to numeric values
int dice_roll(int pos) {
    assert(pos >= 0 && pos <= 6);

    Position p = pig[pos];
    if (p == SIDE) {
        return 0;
    }
    if (p == RAZORBACK) {
        return 10;
    }
    if (p == TROTTER) {
        return 10;
    }
    if (p == SNOUTER) {
        return 15;
    }
    if (p == JOWLER) {
        return 5;
    }

    return 0;
}
int main(void) {
    // Player initialization
    int players;

    // User input
    printf("Number of players (2 to 10)? ");
    int players_result = scanf("%d", &players);

    // Input out of range
    if (players_result < 1 || players < 2 || players > 10) {
        fprintf(stderr, "Invalid number of players. Using 2 instead.\n");
        players = 2;
    }

    // Choose Random Seed
    printf("Random-number seed? ");
    int seed_assignment;
    scanf("%d", &seed_assignment);
    // Invalid Seed
    if (seed_assignment < 1) {
        fprintf(stderr, "Invalid seed. Using 2023 instead.\n");
        seed_assignment = SEED;
    }

    // Name Array
    const char *player_name[] = {
        "Margaret Hamilton",
        "Katherine Johnson",
        "Joy Buolamwini",
        "Grace Hopper",
        "Adele Goldberg",
        "Annie Easley",
        "Jeannette Wing",
        "Mary Kenneth Keller",
        "Megan Smith",
        "Radia Perlman",
    };

    // score array
    int *score = malloc(players * sizeof(int));
    for (int i = 0; i < players; i++)
        score[players] = 0;

    // Pseudorandom Numbers ERROR
    srandom(seed_assignment);
    // srand(seed_assignment);

    // Output Loops
    int win = 0;
    while (win == 0) // continue until someone whiles
    {
        for (int i = 0; i < players; i++) // loop through all players
        {
            printf("%s\n", player_name[i]);

            int random_roll = 1;
            while (random_roll != 0) // roll until 0
            {
                //call created function dice_roll
                int random_roll = dice_roll(random() % 7);

                if (random_roll == 0) {
                    printf(" rolls %d, has %d\n", random_roll, score[i]);
                    break;
                } else {
                    score[i] += random_roll;
                    printf(" rolls %d, has %d\n", random_roll, score[i]);
                    if (score[i] >= 100) {
                        printf("%s won!\n", player_name[i]);
                        win = 1;
                        return 0;
                    }
                }
            }
        }
    }
    free(score);
}
