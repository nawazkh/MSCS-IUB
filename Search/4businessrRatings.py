#
# Created by Nawaz Hussain Khazielakha
#
from pymongo import MongoClient
from gensim.models import LdaModel
from gensim import corpora
from collections import Counter,defaultdict
import json

my_dictionary = corpora.Dictionary.load("DataModels/dictionary.dict")
corpus = corpora.BleiCorpus("DataModels/corpus.mm")
my_lda = LdaModel.load("DataModels/lda_model_topics.lda")

my_business_collection = MongoClient("mongodb://localhost:27017/")["yelp_attempt3"]["Business"]
my_corpus_collection = MongoClient("mongodb://localhost:27017/")["yelp_attempt3"]["Corpus"]
my_rating_collection = MongoClient("mongodb://localhost:27017/")["yelp_attempt3"]["TopicRating"]

i=0
business_cursor = my_business_collection.find()
corpus_cursor = my_corpus_collection.find()

for i in range(business_cursor.count()):
        try :
             business =business_cursor.__getitem__(i)
        except Exception:
             print 'Exceptions..!!!!!!'
             continue
        # Counter() gives a dictionary
        probabilities = Counter()
        count = Counter()
        scores = Counter()
        corpus_cursor = my_corpus_collection.find({"business": business["_id"]})# selecting reviews for a business id
        for corpus in corpus_cursor:
            topics = my_lda[my_dictionary.doc2bow(corpus["words"])]# get topic probability distribution for a words
            for topic, prob in topics:# for all the sixty words we have formed in the topic, we generate its probability distribution for each business
                probabilities[topic]+=prob
                scores[topic]+=corpus["stars"]# all the stars received by that restraunt
                count[topic] += 1# number of times the topic was mentioned in all the reviews

        avgOfTopics = defaultdict()
        frequencyCount = defaultdict()
        for topics in scores.keys():
            # for a topic, calculate its total stars(topic)/topics referred in the review. This is to assign its weight
            avgOfTopics[topics] = 1.0*scores[topics]/count[topics]
            # number of time a topic was reffered in the review
            frequencyCount[topics] = count[topics]

        my_rating_collection.insert({
              "business": business["_id"],
              "ratings": json.loads(json.dumps(avgOfTopics,ensure_ascii=False)),
              "counts": json.loads(json.dumps(frequencyCount,ensure_ascii=False)),
              # for every business, we have stored business id with  weight with its
        })
        #print avgOfTopics
