#
# Created by Nawaz Hussain Khazielakha
#
from pymongo import MongoClient
import nltk
from nltk import WordNetLemmatizer
from nltk.corpus import stopwords
import gensim
from gensim import corpora
from gensim.corpora import BleiCorpus

# for corpus navigation
my_corpus_collection = MongoClient("mongodb://localhost:27017/")["yelp_attempt3"]["Corpus"]
corpus_cursor_1 = my_corpus_collection.find()
corpus_cursor_2 = my_corpus_collection.find()

my_dictionary = corpora.Dictionary(review['words'] for review in corpus_cursor_1)#All the words extracted here
#keeping only top 10000
my_dictionary.filter_extremes(keep_n=10000)
my_dictionary.compactify()
# have selected plural nouns and nouns from the review. New word ids are assigned to the words
corpora.Dictionary.save(my_dictionary,"DataModels/dictionary.dict")
ncorpus =[]
i=0
for review in corpus_cursor_2:
    # print i
    i+=1
    # ncorpus is populated only if the word from review["words"] is present in the DataModels/dictionary.dict.
    # converts a collection of words to its bag-of-words representation: a list of (word_id, word_frequency)
    ncorpus.append(my_dictionary.doc2bow(review["words"]))
# saved into a corpus.mm and Save the resulting index structure to file index_fname (or fname.index is not set).Save a corpus in the LDA-C format.
corpora.BleiCorpus.serialize("DataModels/corpus.mm",ncorpus)
dcorpus = corpora.BleiCorpus("DataModels/corpus.mm")
# id2word is a mapping from word ids (integers) to words (strings). # It is used to determine the vocabulary size, as well as for debugging and topic printing.
my_lda = gensim.models.LdaModel(dcorpus, num_topics=60, id2word=my_dictionary)
my_lda.save("DataModels/lda_model_topics.lda")
i=0
for topic in my_lda.show_topics(num_topics=60,num_words = 1):
    print '#' + str(i) + ': ' + str(topic)
    i += 1
