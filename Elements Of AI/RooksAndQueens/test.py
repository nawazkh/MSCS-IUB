def count_on_left_right_diagonal(board, row, col):
    r1 = row
    c1 = col
    r2 = row
    c2 = col
    N = 6
    strtr = 0
    strtc = 0
    while r1 < N and c1 < N:
        print "row and col are:", r1,c1

        strtr = strtr + board[r1][c1]
        print strtr
        r1 += 1
        c1 += 1
    while r2 >= 0 and c2 >= 0:
        print "row and col are:", r2,c2
        strtc = strtc + board[r2][c2]
        print strtc
        r2 -= 1
        c2 -= 1
    print "total elements in this diagonal from left to right is:", strtc + strtr - 1

def count_on_right_left_diagonal(board, row, col):
    r1 = row
    c1 = col
    r2 = row
    c2 = col
    N = 6
    strtr = 0
    strtc = 0
    while r1 >= 0 and c1 < N:
        print "row and col are:", r1,c1

        strtr = strtr + board[r1][c1]
        print strtr
        r1 -= 1
        c1 += 1
    while r2 < N and c2 >= 0:
        print "row and col are:", r2,c2
        strtc = strtc + board[r2][c2]
        print strtc
        r2 += 1
        c2 -= 1
    print "total elements in this diagonal from left to right is:", strtc + strtr - 1

board = [[1, 0, 0, 0, 0, 0], [0, 1, 0, 0, 0, 0], [0, 0, 1, 0, 0, 0], [0, 0, 0, 1, 0, 0], [0, 0, 0, 0, 1, 0], [0, 0, 0, 0, 0, 1]]
roww = 1
coll = 1
count_on_left_right_diagonal(board,roww,coll)
count_on_right_left_diagonal(board,roww,coll)
