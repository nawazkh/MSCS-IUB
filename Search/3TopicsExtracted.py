#
# Created by Nawaz Hussain Khazielakha
#
from gensim.models import LdaModel
from gensim import corpora

# to generate top 60 most addressed topics in the reviews
my_lda = LdaModel.load("DataModels/lda_model_topics.lda")
i = 0
for topic in my_lda.show_topics(num_topics=60):
    print '#' + str(i) + ': ' + str(topic)#topic[1].split('*')[1]#
    i += 1
