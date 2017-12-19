from output_helper import *

def bfs_helper(graph, start, end):
    # to verify which data set is the super set of city; I bet the graph will be
    if start not in graph or end not in graph:
        raise Exception('Not valid start city or end city')
    if start == end:
        return []
    visited = set()
    queue = []
    queue.append((start, [start]))
    visited.add(start)
    while queue:
        (node, path) = queue.pop(0)
        visited.add(node)
        if node == end:
            return path
        for adjacent in graph.get(node, []):
            if adjacent not in visited:
                queue.append((adjacent, path + [adjacent]))

def bfs(graph, edgeInfo, start, end):
    path = bfs_helper(graph, start, end)
    standard_output(edgeInfo, path, start)