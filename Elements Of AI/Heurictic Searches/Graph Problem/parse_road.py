from constants import *

# use adjacent list to represent the graph
# read high way data
def load_road(filePath):
    graph = {}
    edgeInfo = {}
    with open(filePath) as f:
        maxSpeed = 0
        for line in f:
            try:
                [source, destination, length, speedLimit, name] = line.split()
            except ValueError as err:
                [source, destination, length, name] = line.split()
                speedLimit = MAX_SPEED
            edgeInfo[(source, destination)] = (length, speedLimit, name)
            edgeInfo[(destination, source)] = (length, speedLimit, name)
            maxSpeed = max(maxSpeed, speedLimit)
            if source not in graph:
                graph[source] = set()
            if destination not in graph:
                graph[destination] = set()
            graph[source].add(destination)
            graph[destination].add(source)
    return graph, edgeInfo