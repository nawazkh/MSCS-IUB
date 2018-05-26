#
# Created by Nawaz Hussain Khazielakha
#
from gensim.models import LdaModel
from gensim import corpora
import matplotlib.pyplot as plt; plt.rcdefaults()
import numpy as np
import matplotlib.pyplot as plt

# to generate top 60 most addressed topics in the reviews
my_lda = LdaModel.load("DataModels/lda_model_topics.lda")
i = 0
objects = []
performance = []
for topic in my_lda.show_topics(num_topics=60,num_words = 1):
   print '#' + str(i) + ': ' + str(topic)#topic[1].split('*')[1]#
   if(round(float(topic[1].split('*')[0])*10,2) > 1.5):
      objects.append(str(topic[1].split('*')[1]))
      performance.append(round(float(topic[1].split('*')[0])*10,2))
      i += 1



# #objects = ('Python', 'C++', 'Java', 'Perl', 'Scala', 'Lisp')
# print objects
# print performance
# y_pos = np.arange(len(objects))
# #performance = [10,8,6,4,2,1]
#
# plt.bar(y_pos, performance, align='center', alpha=0.5)
# plt.yticks(y_pos, objects)
# plt.xlabel('Usage')
# plt.title('Topics and their probability distribution')
#
# plt.show()

y_pos = np.arange(len(objects))

plt.barh(y_pos, performance, align='center', alpha=0.6)
plt.yticks(y_pos, objects)
plt.xlabel('Usage')
plt.title('Programming language usage')
plt.show()
