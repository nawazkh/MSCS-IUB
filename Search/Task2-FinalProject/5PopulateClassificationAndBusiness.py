#
# Created by Nawaz Hussain Khazielakha
#
import os
import time
import json
from pymongo import MongoClient
from collections import Counter,defaultdict

my_business_collection = MongoClient("mongodb://localhost:27017/")["yelp_attempt3"]["Business2"]
my_classification_collection = MongoClient("mongodb://localhost:27017/")["yelp_attempt3"]["Classification"]

with open('/Users/nawazkh/Masters/Courses/ILS_Z534_SEARCH/dataset/business.json') as dataset:
    for line in dataset:
        data = json.loads(line)
        if 'Restaurants' in data["categories"] and data['city'] == 'Charlotte':
           my_business_collection.insert({
             "_id": data["business_id"],
             "lat": data['latitude'],
             "lon": data['longitude'],
             "name": data['name']
           })
           my_classification_collection.insert({
           "b_id": data["business_id"],
           "lat": data['latitude'],
           "lon": data['longitude'],
           "name": data['name'],
           "categories": data['categories'],
           "stars": data["stars"]
           })
# for references only.
# generating 10 best generes of cuisines. Cuisines here are not words, they are represented by their word id.
my_topic_rating_collection = MongoClient("mongodb://localhost:27017/")["yelp_attempt3"]["TopicRating"]

def getBest(num):
    count =  Counter();
    ave = Counter();
    res = defaultdict()
    for topic_id in range(60):
        # if a value is present in the collection then retrive it
        result = my_topic_rating_collection.find({'ratings.'+str(topic_id):{'$exists':True}})
        temp = 0
        for a in result:
             temp = temp + int(a['ratings'][str(topic_id)])
       # print temp
        ave[topic_id] = temp/result.count()
        count[topic_id] = result.count()
    #returns valid collection
    #print count.most_common(5)
    for id in count.most_common(num):
        res[id[0]] = (id[1],ave[id[0]])
    print res

getBest(10)
