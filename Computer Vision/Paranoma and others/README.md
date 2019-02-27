Assignment 2 Report
Team members:
jeereddy-nawazkh-pkurusal-rpochamp-a2
jeereddy - Jeevan Reddy
nawazkh - Nawaz Hussain K
pkurusal - Pruthvi Raj Kurusala
rpochamp - Rahul Pochampally

Please refer to Assignment2Report.docx for our findings.

Part 1: Custom Billboards (Image Warping and Homogeneous Matrix)

./a2 part1 poster_input.png
1.	Utilized the Inverse Wrapping methodology to avoid potential aliasing and holes in the new output Image.
o	Given matrix to be used in the projective mapping is
0.907	0.258	-182
-0.153	1.44	58
-0.000306	0.000731	1
o	We are force ceiling the generated pixel coordinates in the expectation of obtaining smoother image.
o	Pseudo code for inverse wrapping:
 For every pixel x0 in g(x0)
 1. Compute the source location x = h^(x0)
 2. Resample f(x) at location x and copy to g(x0)
•	Observations: Output image has sharpness as that of original image. Straight lines are preserved due to inverse mapping. However, the transformed lines have saw-toothed edges as the pixels have not been smoothened.
2.	Using Armadillo Library to calculate the linear equations to generate projective transformation matrix.
o	reference: https://github.com/lsolanka/armadillo/blob/master/examples/example1.cpp
3.	We have hard coded the transformed coordinates of the billboard. The output is as expected.
4.	Our Observations:
The lincoln_wrapped image generated has sharp uneven edges to the column pillars. This can be removed by applying the transformation on the smoothened image. Internal CImg’s blurring function was used to test our hypothesis.


Part 2: Blending

How to run:

./a2 part2 apple.png orange.png mask.png

Output file name: blended.png

The first step in forming the composite image is to calculate the gaussian pyramids of the input images and the mask image. The gaussian level Gi is calculated by convolving Gi-1 with a gaussian kernel k which is given in the text and then down sampling it to half it’s resolution, i.e., if size of Gi-1 is (a x b) then size of Gi is (a/2 x b/2). Basically, Gi-1 is the down-sampled blurred version of Gi. We used the function “get_resize” for down-sampling the images. The output we got using the kernel was not satisfactory as there was a kind of patched layer when using the given kernel. So, to solve this I used the “get_blur” function from the “CImg” library where the variance of the kernel can be passed as a parameter. By trial and error, for variance (here variable ‘sigma’ is used in the code) value 4.0 we got the desired output. Also, we have normalized the mask gaussian images to (0,1) in the code using “normalize(0,1)”.(The images shown below are normalized to (0,255) for the images to be visible).      

The second step is to calculate the Laplacian pyramids of the gaussian pyramids by using the formula
Li = Gi – Gi+1 for i=0 to 4 where Gi+1 is expanded to the size of Gi and then subtracted and L5 = G5. Generally, normalization to (0,255) scale is preferred after calculating the Laplacian to get the grey image but we chose to process the images without normalizing and normalize for the final composite image because this approach is giving us the better blended image than normalizing at each intermediate level. In this method we are not losing the intensities of the input image but in the other way we are getting a darker blended image. Below shown are the Laplacian pyramids for the input images.


The third step is to calculate the Laplacian of the composite image using the Laplacian images of the input images and the gaussian images of the mask using the formula
	(Li)composite = (Gi)mask * (Li)input1 + (1- (Gi)mask) * (Li)input2
We faced the most difficulty in this step only as there were some complexities here. To do the 1-Gi  in the above step, we defined a unit image of appropriate size where the values of the image with 1 since we normalized the Gaussian of mask to (0,1). Next, to perform the multiplication we need to do point to point matrix multiplication for which we used “mul” function. After doing all this we found the Laplacian of the composite images to be greyscale image unlike the color images of the input images above. From further debugging we found that we need all the images to have 3 channels. Since the given input mask image was having 1 channel (found from using the function “imagename.spectrum()”) unlike the other 2 input images which had 3 channels, information was getting lost while calculating using the above formula. So, we had to create a new image using the given mask image such that it has the same pixel values in all the 3 channels thereby not changing the original mask. This has been achieved by iterating through the image using the function “cimg_forXYC”. Below shown are the Laplacian pyramids of the composite image.

The fourth and final step is to calculate the gaussian pyramid from the Laplacian pyramid of the composite image. This is done by using
		 G5= L5
and 		Gi= Gi+1 + Li from i=4 to 0 where Gi+1 is up-sampled to the size of Li and then added. This stage the images are calculated from the top of the pyramid i.e., from 5th to 0th level. At each level of calculating the gaussian the image is normalized to the scale of (0,255). The final blended image is the image obtained at G0. Below are the final Gaussian pyramid images obtained.

The final blended image is the last image shown above.


Output Blended Image:

Part 3: Image Matching and RANSAC

./a2 part3 image_src.png image_dst.png
1.	For Image eiffel_18.jpg eiffel_19.jpg, the SIFT descriptor ratio value below 0.68 gave good results. The following is the output image:



2.	Using The RANSAC algorithm on the above SIFT Pairs, With a threshold of 0 to 120 (Euclidean distance between transformed SIFT descriptors of image1 vs SIFT descriptors of Image 2) gave us an output of


3.	The accuracy of the SIFT descriptor matching mainly depends on the SIFT descriptor ratio value below which filters out the initial outliers. (At times we also observed the removal of genuine in-liners.
a.	Second, outliers removal happens at the threshold fixing and comparing.
b.	In our case, threshold of 10 provides us best result.
4.	Other images and their RANSAC outputs:
We observed a few outliers in the output file even after filtering and thresholding. However, the outliers decreased by considerable amount:




Part 4: Creating Multi-image panaroma:

./a2 part4 panorama_1.jpg panorama_2.jpg panorama_3.jpg
Initially we found out the similar sift descriptors by RANSAC first panorama and second panorama and they are converted to be in the same coordinate system by projective transformation.
Next, applying the same sift descriptor between transformed first panorama and the second panorama, we get the common points in both. Here we have created the mask required for the blending based on the sift descriptor that was obtained.

Mask Generator: By taking one the sift descriptor of left image we found the breaking point from black to white.
Mask Width = first image descriptor length from left + second image descriptor length to right
				(black part)				(white part)

This process is continued for images from left to right.

Transforming first half and second half:
