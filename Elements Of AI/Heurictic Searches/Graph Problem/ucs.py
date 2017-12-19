from output_helper import *
from priority_queue import *

# https://stackoverflow.com/questions/12806452/whats-the-difference-between-uniform-cost-search-and-dijkstras-algorithm
def uniform_cost_search_helper(graph, edgeInfo, start, end):
    # to verify which data set is the super set of city; I bet the graph will be
    if start not in graph or end not in graph:
        raise Exception('Not valid start city or end city')
    if start == end:
        return []
    visited = set()
    priority_queue = PriorityQueue()
    priority_queue.insertThree(start, [start], 0)
    while priority_queue:
        node, path, current_distance = priority_queue.remove()
        visited.add(node)
        if node == end:
            return path
        for adjacent in graph.get(node, []):
            if adjacent not in visited:
                priority_queue.insertThree(adjacent, path + [adjacent], current_distance +
                                           int(edgeInfo.get((node, adjacent))[0]))
def uniform_cost_search(graph, edgeInfo, start, end):
    path = uniform_cost_search_helper(graph, edgeInfo, start, end)
    standard_output(edgeInfo, path, start)