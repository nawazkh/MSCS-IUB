from bfs import *
from heuristic import *
from priority_queue import *
from output_helper import *

def a_star_helper(graph, cityToLagLng, edgeInfo, start, end, cost_function_type):
    # to verify which data set is the super set of city; I bet the graph will be
    if start not in graph or end not in graph:
        raise Exception('Not valid start city or end city')
    if start == end:
        return []
    if cost_function_type == SEGMENTS:
        return bfs_helper(graph, start, end)
    visited = set()
    priority_queue = PriorityQueue()
    priority_queue.insert(start, [start], 0,
                          heuristic_cost_estimate(graph, edgeInfo, cityToLagLng, start, end,
                                                  cost_function_type))
    while priority_queue:
        node, path, curValue, fScore = priority_queue.remove()
        visited.add(node)
        if node == end:
            return path
        for adjacent in graph.get(node, []):
            if adjacent not in visited:
                priority_queue.insert(adjacent, path + [adjacent],
                                      curValue + float(edgeInfo.get((node, adjacent))[0]),
                                      curValue + float(edgeInfo.get((node, adjacent))[0]) + (
                                          heuristic_cost_estimate(graph, edgeInfo, cityToLagLng,
                                                                  adjacent, end,
                                                                  cost_function_type))
                                      )

def a_star(graph, cityToLagLng, edgeInfo, start, end, cost_function_type):
    path = a_star_helper(graph, cityToLagLng, edgeInfo, start, end, cost_function_type)
    standard_output(edgeInfo, path, start)