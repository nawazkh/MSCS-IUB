
# read city data
def load_city(filePath):
    cityToLagLng = {}
    with open(filePath) as f:
        for line in f:
            try:
                [cityName, latitude, longitude] = line.split();
                cityToLagLng[cityName] = (latitude, longitude)
            except ValueError as err:
                print("error when reading city-gps.txt: {0}".format(err))
                print line
    return cityToLagLng