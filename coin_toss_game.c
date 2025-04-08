#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>  // Include this for wait() function

#define MAX_PLAYERS 10
#define MAX_ROUNDS 10

// Function to simulate tossing the coin (1 for heads, 0 for tails)
const char* flip_coin() {
    return rand() % 2 == 0 ? "tail" : "head";  // Randomly return "tail" or "head"
}

// Function for the child process (player) to simulate the game
void player_game(int player_id, int fd_write) {
    int heads_count = 0;

    // Simulate coin toss until we get "tail"
    while (strcmp(flip_coin(), "head") == 0) {
        heads_count++;  // Increment if it's a "head"
    }

    // Write the result (number of heads) to the pipe
    write(fd_write, &heads_count, sizeof(heads_count));
    close(fd_write);  // Close the pipe after writing
    exit(0);  // End child process
}

int main() {
    srand(time(0));  // Seed the random number generator

    int num_players, num_rounds;
    printf("Enter the number of players: ");
    scanf("%d", &num_players);

    if (num_players > MAX_PLAYERS) {
        printf("Maximum number of players is %d.\n", MAX_PLAYERS);
        return 1;
    }

    printf("Enter the number of rounds: ");
    scanf("%d", &num_rounds);

    if (num_rounds > MAX_ROUNDS) {
        printf("Maximum number of rounds is %d.\n", MAX_ROUNDS);
        return 1;
    }

    // Array to store total heads count for each player across all rounds
    int total_scores[num_players];
    for (int i = 0; i < num_players; i++) {
        total_scores[i] = 0;
    }

    // Simulate the rounds
    for (int round = 1; round <= num_rounds; round++) {
        printf("\nRound %d:\n", round);

        int pipefds[num_players][2];  // Pipe file descriptors for each player

        // Create pipes and fork processes
        for (int i = 0; i < num_players; i++) {
            if (pipe(pipefds[i]) == -1) {
                perror("Pipe creation failed");
                return 1;
            }

            pid_t pid = fork();

            if (pid == -1) {
                perror("Fork failed");
                return 1;
            }

            if (pid == 0) {
                // Child process: Simulate the player's game
                close(pipefds[i][0]);  // Close the read end of the pipe in the child
                player_game(i + 1, pipefds[i][1]);
            }
        }

        // Parent process: Collect results from child processes
        for (int i = 0; i < num_players; i++) {
            close(pipefds[i][1]);  // Close the write end in the parent

            int heads_count = 0;
            read(pipefds[i][0], &heads_count, sizeof(heads_count));  // Read the result from the pipe
            total_scores[i] += heads_count;  // Update the total score

            printf("Player %d: %d heads\n", i + 1, heads_count);
            close(pipefds[i][0]);  // Close the read end of the pipe
        }

        // Wait for all child processes to finish
        for (int i = 0; i < num_players; i++) {
            wait(NULL);
        }

        // Determine the winner of the round (highest number of heads)
        int max_heads = 0;
        for (int i = 0; i < num_players; i++) {
            if (total_scores[i] > max_heads) {
                max_heads = total_scores[i];
            }
        }

        printf("Winner of this round with %d heads: ", max_heads);
        for (int i = 0; i < num_players; i++) {
            if (total_scores[i] == max_heads) {
                printf("Player %d ", i + 1);
            }
        }
        printf("\n");
    }

    // Print the final scores after all rounds
    printf("\nFinal scores after all rounds:\n");
    for (int i = 0; i < num_players; i++) {
        printf("Player %d: %d heads\n", i + 1, total_scores[i]);
    }

    return 0;
}
