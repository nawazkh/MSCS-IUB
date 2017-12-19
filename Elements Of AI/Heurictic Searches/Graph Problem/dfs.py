from output_helper import *

# https://stackoverflow.com/questions/7375020/depth-first-graph-search-that-returns-path-to-goal
def dfs_helper(graph, current, end, visited, path):
    visited.add(current)
    if current == end:
        path.append(current)
        return path
    for adjacent in graph.get(current, []):
        if adjacent not in visited:
            p = dfs_helper(graph, adjacent, end, visited, path + [current])
            if len(p) != 0:
                return p
    return []

def dfs_path(graph, start, end):
    # to verify which data set is the super set of city; I bet the graph will be
    if start not in graph or end not in graph:
        raise Exception('Not valid start city or end city')
    if start == end:
        return []
    visited = set()
    path = []
    return dfs_helper(graph, start, end, visited, path)

def dfs(graph, edgeInfo, start, end):
    path = dfs_path(graph, start, end)
    standard_output(edgeInfo, path, start)