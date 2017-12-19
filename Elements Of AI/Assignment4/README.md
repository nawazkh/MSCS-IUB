
# Report
Detailed Report available in the FinalReport.pdf file.
Below is the report for the implementation of the algorithm
I) Knn algorithm:
   We are finding the euclidean distance between each and every pixel of the test image and with all the pixels of corresponding train images(10k).
   Then we sort the all the errors in ascending order and pick the top 11.
   We got an accuracy about 70%.
   Findings:
   1) We initially implemented using lists and the whole algorithm was taking about 26min to complete. Then we switched and used numpy to calculate the eculidiean distance
   which reduced the time to 7mins.
   2) We have experimented with values of k. We have tested with the following values of k = 5, 9, 11 and 21.
   We observed that the accuracy and time where increasing as increase the value of K. We have decided using the k=11 as it is has accuracy about 70% and time is not as bad as k=21.

 II) Adaboost algorithm:
   Training:
   1) We have taken 191 * 192 decision stumps in our adaboost algorithm and we assigned random pixels (r,g,b) values to these decision stumps.
   2) We then noticed the pattern of these decision stumps for 0, 90, 180 and 270 orientation of images. Our pattern was to see if the pixel1 value is greater than pixel2.
   3) After training we have saved decision stump weights for all the 4 classifiers in our adaboost_model.txt.
   4) We have tried altering the number of decision stumps to be taken and noticed if we take fewer stumps then the accuracy of prediction varies a lot with each run and pixels chosen are random.
   5) We then decided to take almost all the decision stump values close  to 192 * 192.

   Testing:
   1) We use the adabosst_model.txt to fetch the edge weights of all the 4 orientations. Check whether their corresponding random pixel (r,g,b) values match the condition.
   2) If yes we add the decision stump value and If no we subtract the decision stump value then we pick the maximum value among the 4 and predict the output.

 III) Neural Net algorithm:
   Training:
   1) Our neural network has one layer with 5 nodes and the output layer has 4 nodes as there are four classifiers.
   2) We have implemented forward propagation algorithm and adjust the weights using the backward propagation algorithm.
   3) Each image in our train data uses forward algorithm uses fwd propagation and updates the weight for each neuron by calculating error.
   4) We had to normalize our pixel data to make our sigmoid function return proper values.
   5) We have stored all the weights for a neuron as a dictionary in the nnet_model.txt.
   6) Our training took about 15 hours to generate a file.
   7) We ran the code by changing the no.of nodes in hidden layer to 16 and reducing the no.of iterations to 100, 500 and 1000 and observed the best results are for no.of iterations as 100.
   8) We chose a high learning rate alpha as 0.5 and reduced the no.of iterations to 100.
   9) At the end we chose 5 nodes in hidden layer, alpha = 0.5, iterations = 100.
   Testing:
   1) We use the weights calculated during training and run our feed forward algorithm. We then use the 4 answers from the output laywr and take the maximum from it.
   2) We then print it to the output.txt file.

 IV) Best Algorithm:
   Our best algorithm is neural network.

 To train the Implementation run the following command in python2 installed terminal:
      time python orient.py train train-data.txt <yourchoice>_model.txt <yourchoice>

 To Test the accuracy, run the following command in python2 installed terminal:
      time python orient.py test test-data.txt <yourchoice>_model.txt <your choice>

        Your choice:
          1) adaboost
          2) nearest
          3) nnet
          4) best
