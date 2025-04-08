# IPC---Coin-Game

Coin Toss Game with IPC (Inter-Process Communication)
Overview
This project simulates a coin toss game in which multiple players take turns tossing a coin until they get "tail." Each player is represented by a separate process, and Inter-Process Communication (IPC) is used to collect the results of each toss. The game consists of multiple rounds, and the winner of each round is determined by the player who accumulates the most heads. The total scores across all rounds are tracked, and the overall winner is determined at the end.

The project uses pipes for communication between the parent process and the child processes representing the players. The program simulates multiple players, each running in a separate process, and uses forking to create these processes.
Features
Multiple Players: The game can handle multiple players (up to a defined limit).

Multiple Rounds: The game can run multiple rounds, with the winner determined by the player with the most heads in each round.

Inter-Process Communication (IPC): The game uses pipes for communication between the parent process and the child processes.

Forking: Each player is simulated by a child process created using fork().

Random Coin Toss: The coin toss results are randomly generated, and the process stops when a "tail" is tossed.
