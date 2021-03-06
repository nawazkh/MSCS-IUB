# Yelp_Dataset_Challenge

You need the following packages to run this whole project(in modules):
1. Python Version - 2.7.12
2. MongoDB Version - 3.4
3. pymongo(install using pip)
4. gensim(install using pip)
5. nltk(install using pip)
6. Run import nltk and nltk.download() in a python shell
7. jupyter
8. Run jupyter nbextension enable --py --sys-prefix widgetsnbextension
9. gmaps
10. Run jupyter nbextension enable --py gmaps

To run the project:
1. 1LoadCorpus.py : loads the corpus with reviews and stars of the project
2. 2findTop60Topics.py : finds top 60 most referred nouns and plural nouns
3. 3TopicsExtracted.py : Topics extracted are displayed
4. 4businessrRatings.py : generates and normalizes the rating given for each business and review.
5. PopulateClassificationAndBusiness.py : populates reference tables for matching of the normalized ratings and businesses.
6. 6DisplayGeneres.py : Displays the genre to be found
7. 7Gmaps_HeatMap.ipynb : plot the most popular areas serving the searched cuisine with an given rating
8. 8CheckAccuracy.ipynb : plot the heat map of the restaurant which actually serve the dishes having a high rating
9. 9GeneratePlotForTopTopics.py : Generate the bar graph of the topics most popular. That is which have a rating more than 1.5

Agenda:
1. extract the most referred nouns and plurals from the reviews from the users
    visiting the city Charlotte.
2. Based on the rating/stars of each restaurant, we calculate the popularity/weight
    of a review and also the weight of the plural nouns/nouns extracted from the
    reviews.
3. Display the Restaurants with topic(given by the user) having highest rating
    in the charlotte map.

# Approach:
1) All the businesses belonging to the city = “Charlotte” were uploaded in the database. All the
reviews addressing the businesses in the city charlotte were uploaded in the DB.
2) Reviews were tokenized, and nouns and plural nouns were extracted as bag of words for each
review and stored in another separate collection.
3) Natural Language Toolkit was used for the lexical analysis and parts of speech tagging of the
reviews.
4) Corpora package of Genism Library is used to retain frequently occurring words from the
review.
5) Generated the probability distribution of the bag of all the filtered words using Allocation (gensim.models.LdaModel)
6) Top Sixty topics with their probability distribution are collected. These are the
keywords(frequent words in the review) which will be proposed to the user.
7) All the words are stored in dictionary with their corresponding id value and probability value.

![top cuisines](https://github.com/nawazkh/MSCS-IUB/blob/master/Search/probilityMoreThan1point5.png)


# Approach:
○ For each tokenized word from the review, we calculate its weight of it using the rating of the
restaurant.
○ Frequency count for each word is also calculated. For each business, the words (NN and
NNS) are weighted as per the ratings given to each business.
○ HeatMap is plotted for a sample Cuizine (say Sandwich and Salad) with a sample rating (say
2). This heatmap represents the places in Charlotte which serve Cuisines ( Sandwich and
Salad) and have a rating more than the given sample rating.
○ This HeatMap can be used by the user to judge the places which serve most popular (sample)

![Sandwich popularity](https://github.com/nawazkh/MSCS-IUB/blob/master/Search/SaladDistribution.png)

![Sandwich predicted](https://github.com/nawazkh/MSCS-IUB/blob/master/Search/Salad_Accuracy.png)
cuisines. The user can now chose a place strategically, if they want to setup their own business with the sample cuisine or not.
