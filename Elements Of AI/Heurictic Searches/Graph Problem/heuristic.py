import sys
from math import radians, cos, sin, asin, sqrt

from constants import *

def find_cloest_city_for_missing_intersection(graph, edgeInfo, intersection, cityToLagLng):
    if intersection not in graph:
        raise Exception('Not valid start city or end city')
    currentDistance = sys.maxint
    closestCity = ''
    for adjacent in graph[intersection]:
        if adjacent in cityToLagLng:
            distance = int(edgeInfo[(adjacent, intersection)][0])
            if distance < currentDistance:
                currentDistance = distance
                closestCity = adjacent
    return closestCity


# http://www.movable-type.co.uk/scripts/latlong.html
# https://stackoverflow.com/questions/15736995/how-can-i-quickly-estimate-the-distance-between-two-latitude-longitude-points
# https://stackoverflow.com/questions/4913349/haversine-formula-in-python-bearing-and-distance-between-two-gps-points
# http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html
def haversine_in_miles(lat1, lon1, lat2, lon2):
    """
    Calculate the great circle distance between two points
    on the earth (specified in decimal degrees)
    """
    # convert decimal degrees to radians
    lon1, lat1, lon2, lat2 = map(float, [lon1, lat1, lon2, lat2])
    lon1, lat1, lon2, lat2 = map(radians, [lon1, lat1, lon2, lat2])
    # haversine formula
    dlon = lon2 - lon1
    dlat = lat2 - lat1
    a = sin(dlat / 2) ** 2 + cos(lat1) * cos(lat2) * sin(dlon / 2) ** 2
    c = 2 * asin(sqrt(a))
    km = 6367 * c
    mile = km * 0.621
    return mile


def heuristic_cost_estimate(graph, edgeInfo, cityToLagLng, start, end, cost_function_type):
    # to verify which data set is the super set of city; I bet the graph will be
    if start not in graph or end not in graph:
        raise Exception('Not valid start city or end city')
    if start not in cityToLagLng:
        start = find_cloest_city_for_missing_intersection(graph, edgeInfo, start, cityToLagLng)
    if end not in cityToLagLng:
        end = find_cloest_city_for_missing_intersection(graph, edgeInfo, end, cityToLagLng)
    if start == end:
        return 0
    distance = haversine_in_miles(cityToLagLng[start][0], cityToLagLng[start][1],
                                  cityToLagLng[end][0], cityToLagLng[end][1])
    if cost_function_type == DISTANCES:
        return distance
    if cost_function_type == TIME:
        return distance / MAX_SPEED
