(1) which search algorithm seems to work best for searching routing options?
A star algorithm seems to work best.
(2) which algorithm is fastest in terms of the amount of computation time required by your program, and by how much, according to your experiments?
Theoretically, because BSF uses the plain queue structure to seach all adjacent neighbors, it should be the fastest in terms of computation. However, with heuristic function, the search space is much less compared with the normal BFS. In practice, the time spent on A* search is less than BFS.
(3) Which algorithm requires the least memory, and by how much, according to your experiments?
Both uniform cost search and A* search use priority queue to find min value of the candidates. However, with the help of the heuristic function, the candidates to be looked up in A* search algorithm are far less than uniform cost search. Therefore, A* algorithm uses less memory. Compared with typical BFS, the A* search doesn't necessarily have to explore all nearby neighbors, it uses less memory when I experimented with two points which are far from each other.
(4) Which heuristic function did you use, how good is it and how might you make it/them better?
I used distance, time, segments heuristic function. The heuristic function for distance is the direct geo-distance between two points; the heuristic function for time is the direct geo-distance divided by the maximum speed between two points. They work very well. For these conjunction points (which are missing latitude and longtitue), I used the cloest city to represent its location. If we could find more accurate location information, it will increase the precision of the search result. 


