  CS B551 Fall 2017, Assignment #3
#
 Your names and user ids:
 Aravind Bharatha and abharath
 Nawaz Hussain K and nawazkh
 Rahul Pochampally and rpochamp

 (Based on skeleton code by D. Crandall)
#
#
####
 Report:
 For Training:
 1) We trained the data got the dictionary of words(P(w1|S), transition of words(P(N|N)) and POS probabilities (P(N)).
#
 For Simplified:
 1) We just check the dictionary of word frequency for a particular words with all POS and return the maximum one.
 2) FOr the ones we don't have any freq or in case of tie we return NOun as default.
#
 For Variable Elimination:
 We have implemented only forward elimination sequense.
 we caluclated Tow values for the previous words and summed the previous state Tow values and picked the maximum among the current state.
 si*() = arg max P(Si = si|W)
#
 For Viterbi:
 We defined a 2*2 matrix which has all the (S^2 * T)
 We calculate the value for each cell using the previous columns values and keep track of the maximum value POS.
 Our each cell is of this form [probability, PrevMaxValue]
 once we fill our veterbi matrix we backtack and print the sequence
#
 For Posterior Probability:
 We used P(S|W) = P(S) * P(W|S)
#
 References:
 Canvas Slides
 Piazza Posts

------------------------------------------
 ./ocr.py : Perform optical character recognition, usage:
     ./ocr.py train-image-file.png train-text.txt test-image-file.png
#
 Authors: (insert names here)
 Your names and user ids:
 Aravind Bharatha and abharath
 Nawaz Hussain K and nawazkh
 Rahul Pochampally and rpochamp
#
 Report:
 For Training:
 1) We trained the data got the dictionary of chars(P(w1|S), transition of chars(P(N|N)).
 2) FOr emission probabilities we used hit and miss ratio.
 3) If out 14 * 25 has more than 340 blank spaces we assign a high prbabilty forspace
 4) for other characters we divide the hit/miss and then normalize the data by dividing with the total of the sum of all the values.
   This is done to avoid the probabilites to go over 1.
 5) FOr transition prbabilities we add a high number and divide a low number to normalize the values. This is done to avoid the domination of transition in our answers.
    The idea was taken from a paper mentioned in refernces.
   We noticed that the transition probabilties were dominating so we changed to this.
#
 For Simplified:
 1) We just check the hit/miss ratio for a particular char and returned the maximum one
 2) For spaces if the no.of chars is greater than 340 we send a high probability to space
 3) We take only the top 3 emission probabilities for the given state. By doing this we got better results
#
 For Variable Elimination:
 We have implemented only forward elimination sequence. We used a matrix and formed a tow table
 we take the sum of all the Tow values of previous column and multiply with the emission and fill the consecutive column.
 3) We take only the top 3 emission probabilities for the given state. By doing this we got better results
#
 For Viterbi:
 We defined a 2*2 matrix which has all the (S^2 * T)
 We also added weights to our transition variables and giving max weight to the emission to get better results.
 We calculate the value for each cell using the previous columns values and keep track of the maximum value POS.
 Our each cell is of this form [probability, PrevMaxValue]
 once we fill our veterbi matrix we back track and print the sequence
#
#
 References:
 Canvas Slides
 Piazza Posts
 http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.141.7177&rep=rep1&type=pdf
####
