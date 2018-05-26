Part 1.1
We are plotting the fourier transform of the imagePNGInputImage512.png

We observed that using log10 gave visually better results than using loge.
Please refer “fft_magnitude” for code implementation.

Part 1.2
This function is used to remove the interference. The real part of fft of noise1.png contained “HI” as the noise. The log of magnitude of this image had components whose values was less than 0.12 (excluding the 256th row in the fft matrix).All those values were made to zero which removed the noise/ interference in the image.
Now, the resultant image after noise removal was not as bright and required enhancement. Hence using trial and error method we came up with 2 coefficients to boost the frequency values in fft matrix.
4.60625 was multiplied with all the values in the real matrix and imaginary matrix.
The Dc term was multiplied with 1.47078125.
We also observed  that after multiplying the above values the resultant image had frequencies in the same harmonic resulting in a image without distortion and had better brightness.
Please refer “remove_interference” for code implementation.
Part1.3
There are 2 parts to this problem.
1)	Add watermark
We generated a random number using srand() that generates a constant random number for a particular given input N.
This number was converted to binary of length l where 2l is the number of bins that had been modified to inject the watermark. The water mark bins were injected with a space of 2 bins. We handled both odd and even cases of l and calculated the radius of the circle accordingly. (Please refer the code “mark_image”)
Watermark was added to the image using the equation given in the question. We found alpha = 0.65 that helped in threshold setting for checking if watermark is present which is explained below.
2)	Check if watermark exists

An input image is passed with a parameter N. So the random number is generated again in  the similar way .
After extracting the values from the same circle, we observed that to declare if the watermark was present in an image, we needed to find out Pearson coefficient between the generated binary number and extracted values.
However, peasron coefficient can be calculated between continuous variables.
Hence we calculated 2-sample Welch’s t-test.  And using the absolute value of the resultant value, we are deciding if the watermark is present or not.

The threshold value is varying between 0.0 to 1.8 if the watermark is not present.
The threshold value is varying between 1.1 to 4.7 if the watermark is present.
(These values are absolute values)
Depending on the coefficient calculated, the code prints if the watermark with the given input number is present or not.

“We need to find the correlation between Vi and R’(x, y)
      As we know that Vi contains only 0 and 1’s (Categorical or binary values), but R’(x, y) is a continuous value. In order to find the correlation between these two we use a different method other than using Pearson correlation coefficient, i.e., two sample test.
Two sample test:
We have values (0,1) and we got their respective values of R’(x, y). We are going to take these values into 2 samples, one contains all the values of R’(x, y) when Vi = 0 and the other contains all the values of R’(x, y) when Vi=1. We need to find whether these sample values are different from each other or not. If they are different then we can say that there is a correlation between R’(x, y) and Vi and vice-versa. So, we calculate the sample means, variance and number of values for each sample. We take the difference of means of these 2 samples (meand) and calculate the effective variance (evar) of these 2 samples by Welch’s t-test.  
Null hypothesis: meand = 0 (or) there is no difference between the 2 samples (or) no correlation between Vi and R’(x, y)
Alternative Hypothesis: meand != 0 (or)  there is difference between the 2 samples (or) correlation between Vi and R’(x, y)
We assume that although this forms a t distribution with some degrees of freedom, we consider this as a normal distribution
Z = meand/evar
We need to keep a threshold for the Z value inorder to accepting or rejecting the null hypothesis.
