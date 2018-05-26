import os
import time
import json
import nltk
from nltk import WordNetLemmatizer
from nltk.corpus import stopwords
from pymongo import MongoClient
#
# Created by Nawaz Hussain Khazielakha
#

# get cursors of the collections
my_reviews_collection = MongoClient("mongodb://localhost:27017/")["yelp_attempt3"]["Reviews"]
my_business_collection = MongoClient("mongodb://localhost:27017/")["yelp_attempt3"]["Business"]
my_corpus_collection = MongoClient("mongodb://localhost:27017/")["yelp_attempt3"]["Corpus"]
#filtering the stopWords
stopset = set(stopwords.words('english'))
# print stopset
stopwords = {}
# categoriesList = {}
with open('stopwords.txt', 'rU') as f:
    for line in f:
        stopwords[line.strip()] = 1
mylemmatizer = WordNetLemmatizer()

# populate dataset of the business collections
# populate business before populating the reviews and Corpus
with open('/Users/nawazkh/Masters/Courses/ILS_Z534_SEARCH/dataset/business.json') as dataset:
    for line in dataset:
        data = json.loads(line)
        if 'Restaurants' in data["categories"] and data['city'] == 'Charlotte':
            my_business_collection.insert({
            "_id": data["business_id"],
            "categories": data["categories"],
            "stars": data["stars"]
            })
            #print "inserted business"
            #break
print "business inserted"
n=0
with open('/Users/nawazkh/Masters/Courses/ILS_Z534_SEARCH/dataset/review.json') as dataset:
    for line in dataset:
        data = json.loads(line)
        if my_business_collection.find({"_id": data["business_id"]}).count() !=0:
            n+=1
            #print n
            my_reviews_collection.insert({
            "reviewId": data["review_id"],
            "business": data["business_id"],
            "text": data["text"],
            "stars": data["stars"],
            "votes": data["useful"]
            })
            #print "review inserted"
            words = []
            sentences = nltk.sent_tokenize(data["text"].lower())
            # eliminating stopwords and considering Nouns and Plural Nouns mentioned in the reviews
            for sentence in sentences:
                tokens = nltk.word_tokenize(sentence)
                filteredWords = [word for word in tokens if (word not in stopwords and word not in stopset)]
                tagged_text = nltk.pos_tag(filteredWords)# this is independent of semantics of a sentence
                for word, tag in tagged_text:
                    #print word, tag
                    if tag in ['NN','NNS'] :#nouns and plural nouns extracted
                        words.append(mylemmatizer.lemmatize(word))
            #print "corpus being inserted"
            my_corpus_collection.insert({
                  "reviewId": data["review_id"],
                  "business": data["business_id"],
                  "stars": data['stars'],
                  "votes": data["useful"],
                  "text": data["text"],
                  "words": words
            })
            #break
print "review and corpus inserted"
