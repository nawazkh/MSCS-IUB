Consider a variant of 15 puzzle, but with the following important change.
Instead of sliding a single title from one cell into an empty cell,
in this variant, either one or two or three tiles may be slide left, right, up
and down in a single move.
For example, for a puzzle of this configuration:
1  2  3  4
5  6  7  8
9  10    12
13 14 15 11

The valid successors are :
1  2  3  4  
5  6  7  8  
9     10 12
13 14 15 11

1  2  3  4  
5  6  7  8  
   9 10 12
13 14 15 11

1  2  3  4  
5  6  7  8  
9  10 12
13 14 15 11

1  2     4  
5  6  3  8  
9  10 7  12
13 14 15 11

1  2     4  
5  6  3  8  
9  10 7  12
13 14 15 11

1  2  3  4  
5  6     8  
9  10 7  12
13 14 15 11

1  2  3  4  
5  6  7  8  
9  10 15 12
13 14    11


The goal is to find a short sequence of moves that restores the canonical
configuration (on the left above) given an initial board configuration.

Write a program called solver16.py that finds a solution to this problem
efficiently using A* search.

Your program should run on the command line like:

./solver16.py [input-board-filename]

where input-board-filename is a text like containing a board configuration
in a format like:
1 2 3 4
5 6 7 8
9 0 10 12
13 14 15 11

where 0 indicates the empty position. The program can output whatever you'd
like, except that the last line of output should be a representation of the
solution path you found, in this format:

[move-1] [move-2] ... [move-n]

where each move is encoded as a letter L, R, U, or D for left,
right, up, or down, respectively, followed by 1, 2, or 3 indicating the
number of tiles to move, followed by a row or column number
(indexed beginning at 1). For instance, the six successors shown above would
correspond to the following six moves
(with respect to the initial board state):

R13 R23 L13 D23 D13 U13



 put your 15 puzzle solver here!
 ------------------------------------------------------------------------------
  description of how you formulated the search problem,
  Search problem:
        For any input 15-puzzle, decide whether a goal state is possible.
        If yes, provide a least number of moves to reach the goal state.

        Goal state:
             [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 0]]

        Initial State:
            Input is any 15-puzzle satisfying the following conditions.
            1) permutation Inversion of input state and the position of '0' from
               bottom should not be even(or odd) at the same time.

        Successor Function:
            Generate 6 states by moving either 1 tile, 2 titles and 3 titles
                to 0, to left or right, up or down.
                6 successors are generated.
            All the generation of the successors take only one unit cost

        Heuristic Function : h(s) :
            - (Sum of Manhattan distance of all the tiles) / 3
            This h(s) is admissible. The cost to reach the goal node is always
            less than actual cost needed to reach the goal node
           - We choose (Sum of Manhattan distance of all the tiles) / 3
             because the sliding of three tiles is considered as one move.

        Edge Weights:
            h(s) + number of moves to reach the state
  ------------------------------------------------------------------------------
  a brief description of how your search algorithm works;
            First: Solve function is invoked.
            then "initial input board is checked whether it is solvable or not"
            if initial node is the goal node, initial state is returned
            if not:
                successors as defined in above as are generated
                and stored in a heap
                each successor is popped (cost of that state be: h(s) + travel cost)
                popped state is stored in a data structure called closed
                                             CLosed is implemented by has table.
                    if the popped state is already a visited node(i.e belongs to closed)
                    then discard popped state
                    else:
                    add it to fringe.
  ------------------------------------------------------------------------------
  -------------- discussion of any problems you faced -------------------------:
    problem is with selection of right heuristic function.
    In my implementation,
    Linear conflicts had a time lag,
    Weighted Manhattan distance was not optimal

  any assumptions: a tile of lower value is considered as higher priority
                    hence more weightage is assigned to Manhattan distance
                    of lower valued tiles
  simplications: 6 successors are generated and added to a heap in
                 the successor functoin itself.
  and/or design decisions you made:
    changed the algorithm 3 a little bit.
    successors not being checked if present in fringe or not.
  references :
  https://stackoverflow.com/questions/40781817/heuristic-function-for-solving-weighted-15-puzzle

  ------------------------------------------------------------------------------
  solved by Nawaz Hussain K, 0003561850

  ------------------------------------------------------------------------------
  accepts the input-board-filename as an argument.
  to run:
  python solver16.py <filename>
  ------------------------------------------------------------------------------
