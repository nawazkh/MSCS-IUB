#
# Created by Nawaz Hussain Khazielakha
#
from pymongo import MongoClient
from gensim.models import LdaModel
from gensim import corpora

my_dictionary = corpora.Dictionary.load("DataModels/dictionary.dict")
my_lda = LdaModel.load("DataModels/lda_model_topics.lda")

corpus_collection = MongoClient("mongodb://localhost:27017/")["yelp_attempt3"]["Corpus"]

i=0
corpus_cursor = corpus_collection.find()
for review in corpus_cursor:
             # assume there's one document per line, tokens separated by whitespace
             i=i+1
             # display the words which are matching with the ones present in then
             # dictionary and then calculate their probability distribution
             # using my_lda
             print my_lda[my_dictionary.doc2bow(review["words"])]
             if i ==20:
                 break;
