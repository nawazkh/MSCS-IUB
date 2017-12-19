def standard_output(edgeInfo, path, startCity):
    if len(path) == 0:
        print("%d %.4f %s %s" %(0, 0, startCity, startCity))
    totalDistance = 0
    totalTimeInHours = 0.0
    pathInString = ''
    for i in range(0, len(path) - 1):
        edge = edgeInfo[(path[i], path[i+1])];
        totalDistance = totalDistance + float(edge[0])
        totalTimeInHours = totalTimeInHours + float(edge[0])/float(edge[1])
        pathInString = pathInString + path[i] + ' '
    pathInString = pathInString + path[-1]
    print("%d %.4f %s" %(totalDistance, totalTimeInHours, pathInString.strip()))