# Yelp_Dataset_Challenge

You need the following packages to run this whole project(in modules)
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

To run the project.
    1. 1LoadCorpus.py : loads the corpus with reviews and stars of the project
    2. 2findTop60Topics.py : finds top 60 most referred nouns and plural nouns
    3. 3TopicsExtracted.py : Topics extracted are displayed
    4. 4businessrRatings.py : generates and normalizes the rating given for each business and review.
    5. 5PopulateClassificationAndBusiness.py : populates reference tables for matching of the normalized ratings and businesses
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
