#!/usr/bin/env python
# nrooks.py : Solve the N-Rooks problem!
# D. Crandall, 2016
# Updated by Zehua Zhang, 2017
#
# The N-rooks problem is: Given an empty NxN chessboard, place N rooks on the board so that no rooks
# can take any other, i.e. such that no two rooks share the same row or column.

import sys
from collections import deque
import time

# Count # of pieces in given row
def count_on_row(board, row):
#    print "count_on_row"
    strtr = sum( board[row] )
#    print strtr
    return strtr

# Count # of pieces in given column
def count_on_col(board, col):
#    print "count_on_col"
    tes = sum( [ row[col] for row in board ] )
#    print tes
    return tes

# Count total # of pieces on board
def count_pieces(board):
#    print "count_pieces"
    strtemp = sum([ sum(row) for row in board ] )
#    print strtemp
    return strtemp

# Return a string with the board rendered in a human-friendly format
def printable_board(board):
#    print "printable_board"
    return "\n".join([ " ".join([ "R" if col else "_" for col in row ]) for row in board])

# Add a piece to the board at the given position, and return a new board (doesn't change original)
def add_piece(board, row, col):
#    print board
    #print row
    #print col
    #print "add_piece"
    stringTemp = board[0:row] + [board[row][0:col] + [1,] + board[row][col+1:]] + board[row+1:]
    #print stringTemp
    return stringTemp

# Get list of successors of given board state
def successors2(board):
#    print "successors"
#    print board
    tempList=[]
    itemsOnBoard = count_pieces(board)
    itemsOnRow = 0
#    print "hi"
#    print tempList
    for r in range(0,N):
        itemsOnRow = count_on_row(board,r)
        for c in range(0,N):
            if(board[r][c] == 1):#implies if 1 is at [1,1] do not re add onto it
                pass
            else:       #add it to the fringe
                if(itemsOnBoard <= N-1 ):# to remove adding of more than 3 items
                    #print "in new else" , count_pieces(board)#if the item is present a row, exclude that row
                    if(itemsOnRow == 0):#if the item not present in that row, then add item
                        if(count_on_col(board,c) == 0):#if item not present in that column, then add item
                            #print "count_on_row(board,r)",count_on_row(board,r),"count_on_col(board,c)",count_on_col(board,c)
                            tempList.append(add_piece(board, r, c))
                        else:# ignore expansion if the column already has an item
                            pass
                    else:#if an item already there at row r, ignore expansion
                        pass
                else:
                    #print "excluding expanding of state space if the number of items are more than N"
                    pass
    return tempList

def successors(board):
#    print "successors()"
    return [ add_piece(board, r, c) for r in range(0, N) for c in range(0,N) ]

# check if board is a goal state
def is_goal(board):
#    print "is_goal"
    return count_pieces(board) == N and \
        all( [ count_on_row(board, r) <= 1 for r in range(0, N) ] ) and \
        all( [ count_on_col(board, c) <= 1 for c in range(0, N) ] )

# Solve n-rooks!
def solve(initial_board):
#    print "solve"
    fringe = deque([initial_board])
#    print fringe
    while len(fringe) > 0:
        #print "hi"
        #print fringe
        for s in successors2( fringe.popleft() ):#used https://docs.python.org/2/tutorial/datastructures.html
            #print "fringe popped"
            #print s
            #print "---"
            if is_goal(s):
                return(s)

            fringe.append(s)
#            print fringe
    return False

# This is N, the size of the board. It is passed through command line arguments.
N = int(sys.argv[1])
#print "Initial params set"
# The board is stored as a list-of-lists. Each inner list is a row of the board.
# A zero in a given square indicates no piece, and a 1 indicates a piece.
initial_board = [[0]*N]*N
print ("Starting from initial board:\n" + printable_board(initial_board) + "\n\nLooking for solution...\n")
tempTime = int(time.time()*1000000)
solution = solve(initial_board)
print (printable_board(solution) if solution else "Sorry, no solution found. :(")
tempTime2 = int(time.time()*1000000)
#print "Time Elapsed in milliseconds for",N," rooks using queue",tempTime2 - tempTime
