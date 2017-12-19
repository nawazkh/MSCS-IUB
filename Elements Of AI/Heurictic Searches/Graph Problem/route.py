#!/usr/bin/python
#
#
#
#code explained in ReadMe
#
#
import sys

from a_star import *
from bfs import *
from dfs import *
from parse_city import *
from parse_road import *
from ucs import *

cityToLagLng = load_city('./data/city-gps.txt')
graph, edgeInfo = load_road('./data/road-segments.txt')


start = sys.argv[1].strip('\'').strip('\"')
end = sys.argv[2].strip('\'').strip('\"')
routingAlgorithm = sys.argv[3]
costFunction = sys.argv[4] if routingAlgorithm == ASTAR else ''

if routingAlgorithm == BFS:
    bfs(graph, edgeInfo, start, end)
elif routingAlgorithm == DFS:
    dfs(graph, edgeInfo, start, end)
elif routingAlgorithm == UNIFORM:
    uniform_cost_search(graph, edgeInfo, start, end)
elif routingAlgorithm == ASTAR:
    a_star(graph, cityToLagLng, edgeInfo, start, end, costFunction)