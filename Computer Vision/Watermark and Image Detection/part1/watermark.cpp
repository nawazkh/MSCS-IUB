// Please refer external report for the summary of our observations
// Watermark.cpp : Add watermark to an image, or inspect if a watermark is present.
//
// Based on skeleton code by D. Crandall, Spring 2018
//
// PUT YOUR NAMES HERE
// jeereddy-nawazkh-pkurusal-rpochamp-a1
// jeereddy - Jeevan Reddy
// nawazkh - Nawaz Hussain K
// pkurusal - Pruthvi Raj Kurusala
// rpochamp - Rahul Pochampally
//
//

//Link to the header file
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <SImage.h>
#include <SImageIO.h>
#include <fft.h>
#include <stdio.h>
#include <sstream>
#include <cmath>
#include <fstream>

#include <algorithm>
#include <iterator>
#include <iomanip>

using namespace std;

// This code requires that input be a *square* image, and that each dimension
//  is a power of 2; i.e. that input.width() == input.height() == 2^k, where k
//  is an integer. You'll need to pad out your image (with 0's) first if it's
//  not a square image to begin with. (Padding with 0's has no effect on the FT!)
//
// Forward FFT transform: take input image, and return real and imaginary parts.
//
void fft(const SDoublePlane &input, SDoublePlane &fft_real, SDoublePlane &fft_imag)
{
        fft_real = input;
        fft_imag = SDoublePlane(input.rows(), input.cols());

        FFT_2D(1, fft_real, fft_imag);
}

// Inverse FFT transform: take real and imaginary parts of fourier transform, and return
//  real-valued image.
//
void ifft(const SDoublePlane &input_real, const SDoublePlane &input_imag, SDoublePlane &output_real)
{
        output_real = input_real;
        SDoublePlane output_imag = input_imag;

        FFT_2D(0, output_real, output_imag);
}

string decToBinary(int n)
{
        // array to store binary number
        int binaryNum[100000];
        stringstream sstm;
        string result;
        // counter for binary array
        int i = 0;
        while (n > 0)
        {
                // storing remainder in binary array
                binaryNum[i] = n % 2;
                n = n / 2;
                i++;
        }
        cout << fixed;
        // printing binary array in reverse order
        for (int j = i - 1; j >= 0; j--)
        {
                sstm << binaryNum[j];
        }
        result = sstm.str();
        return result;
}

//Random Number generation through seeding
string gen_random_number(int myNum)
{
        cout << "In generating the random number " << endl;
        cout << "myNum:" << myNum << endl;
        srand(myNum); // every random number generated from myNum will always be the same.
        int myrand = rand();
        cout << "First random number :" << myrand << endl;
        string result = decToBinary(myrand); //Converting the random to binary
        return result;
}

//To check if a pixel has the distance of r - the radius of watermark circle.
//It checks if a pixel lies on the circle.
bool get_distance(int centerx, int centery, int x2, int y2, int r)
{
        int value = sqrt(((x2 - centerx) * (x2 - centerx)) + ((y2 - centery) * (y2 - centery))); //sqrt((x2-x1)^2 + (y2-y1)^2)
        if (r == value)
        {
                return true;
        }
        return false;
}

// Used this function in Part 1.1
//Computes a matrix with the logarithm of fft magnitude
SDoublePlane fft_magnitude(const SDoublePlane &fft_real, const SDoublePlane &fft_imag)
{
        SDoublePlane matrixE = SDoublePlane(fft_real.rows(), fft_real.cols()); //Initiating a matrix with size equal to the rows and columns of the input
        float temp = 0;
        for (int i = 0; i < fft_real.rows(); i++)
        {
                for (int j = 0; j < fft_real.cols(); j++)
                {
                        temp = (((fft_real[i][j]) * (fft_real[i][j])) + ((fft_imag[i][j]) * (fft_imag[i][j]))); //Calculating the magnitude
                        matrixE[i][j] = log10(sqrt(temp));                                                      //Applying log to boost the value
                }
        }
        return matrixE;
}

// Used this function in Part 1.2
SDoublePlane remove_interference(const SDoublePlane &input)
{
        SDoublePlane realPart, imaginaryPart, output_image;
        fft(input, realPart, imaginaryPart);
        SDoublePlane matrixE = SDoublePlane(realPart.rows(), realPart.cols());
        float temp = 0;

        for (int i = 0; i < realPart.rows(); i++)
        {
                for (int j = 0; j < realPart.cols(); j++)
                {
                        temp = (((realPart[i][j]) * (realPart[i][j])) + ((imaginaryPart[i][j]) * (imaginaryPart[i][j])));
                        matrixE[i][j] = log10(sqrt(temp)); //Calculating magnitude
                        if (matrixE[i][j] >= 0.12)         //0.12 is the threshold for removing noise which has been put as "HI" in the real domain
                        {
                                cout << "matrixE[i][j] : " << matrixE[i][j] << " : (i,j) : (" << i << "," << j << ")" << endl;
                                //Making all the values 0 except in row number 256 if the value in the matrix is
                                //greater than the threshold prescribed.
                                //256 because the dc component lies in that row and that should not be messed with
                                //as it effects the total image.
                                if ((i < 256 || i > 256))
                                {
                                        cout << i << " : " << j << endl;
                                        cout << "realPart[i][j] : " << realPart[i][j] << endl;
                                        realPart[i][j] = 0.00;
                                        imaginaryPart[i][j] = 0.00;
                                        cout << "realPart[i][j] : " << realPart[i][j] << endl;
                                }
                        }
                }
        }

        for (int i = 0; i < realPart.rows(); i++)
        {
                for (int j = 0; j < realPart.cols(); j++)
                {

                        realPart[i][j] = realPart[i][j] * 4.60625; //4.60625 is obtained by testing for about 2 hours
                                                                   // which makes the image to same harmonic by boosting
                                                                   // the values
                        imaginaryPart[i][j] = imaginaryPart[i][j] * 4.60625;
                }
        }
        // }

        realPart[256][256] = realPart[256][256] * 1.47078125; //1.47078125 is a similar term like above that makes it same harmonic
        imaginaryPart[256][256] = imaginaryPart[256][256] * 1.47078125;
        output_image = SDoublePlane(realPart.rows(), realPart.cols());
        ifft(realPart, imaginaryPart, output_image);
        return output_image;
}

// Used in Part 1.3 -- add watermark N to image
SDoublePlane mark_image(int myNum, string inputFile, string outputFile)
{

        // "000" in 0003561850 is being taken as octal number by the compiler and error- "invalid digit '8' in octal constant" being thrown.
        // int len = length(myNum);
        /*-----------------------------------------------------*/
        // cout << "Just checking if this is even working or not";
        cout << "-------------------------" << endl;
        cout << "In: " << inputFile << "  Out: " << outputFile << endl;
        // i have changed the input string to 16bits of 1's so that when i boost the spectogram
        // i can see if we are marking the right perimeter.
        //string l="1111111111111111";
        string l = gen_random_number(myNum); // string of the binary vector
        cout << "-------------------------" << endl;
        cout << "l: " << l << endl;
        int binLen = l.length(); //length of binary vector
        SDoublePlane input_image = SImageIO::read_png_file(inputFile.c_str());
        // c_str() converts std::String to a "C" type string from a C++ one.
        /*--------------------------*/
        // we have the gray scale image in input_image
        // need to generate its fourier transform
        SDoublePlane realPart, imaginaryPart, beforeWaterReal, beforeWaterImaginary;
        //cout << "(8  & (8-1))" << (8  & (8-1)) << endl;
        fft(input_image, realPart, imaginaryPart);
        beforeWaterReal = realPart;
        beforeWaterImaginary = imaginaryPart;
        /*--------------------------*/
        // FFT is done, we need to inject the watermark
        char char_array[binLen + 1];
        strcpy(char_array, l.c_str());
        // for (int i=0; i<binLen; i++)
        // {
        //   cout << (int)char_array[i]-48;
        // }cout << endl;

        const int xcenter = realPart.rows() / 2; // Calculating the centre of image
        const int ycenter = realPart.rows() / 2;
        float alpha = 6.5; //0.65;
        int r, space;

        // to implement a cirle

        bool is_radius;
        space = 2; // so the space between two vector values is 2 blocks
        int j_count = 0, space_count = 0;
        if (binLen % 2 == 0)
        { // l is even
                r = 2 * ((binLen * (1 + space)) / 3.14);
        }
        else
        {
                r = 2 * (((binLen + 1) * (1 + space)) / 3.14); //2 * ( ((binLen + 1) * (1 + space))/3.14 );
        }

        cout << "binLen:" << binLen << endl;
        cout << "r:" << r << endl;
        cout << "space:" << space << endl;
        cout << "-------------------------" << endl;
        // for 2nd quadrant
        for (int i = (xcenter - 1); i >= 0; i--)
        {
                for (int j = 0; j < ycenter; j++)
                {
                        is_radius = get_distance(xcenter, ycenter, i, j, r); //get_distance(int centerx, int centery, int x2, int y2, int r){
                        if (is_radius && space_count == 0)
                        {
                                realPart[i][j] = realPart[i][j] + (alpha * abs(realPart[i][j]) * ((int)char_array[j_count] - 48));
                                j_count++;
                                space_count++;
                                break;
                        }
                        else if (is_radius && (is_radius != 0 && space_count < space))
                        {
                                space_count++;
                                break;
                        }
                        else if (is_radius && (is_radius != 0 && space_count == space))
                        {
                                space_count = 0;
                                break;
                        }
                }
                if (j_count == (binLen / 2))
                {
                        break;
                }
        }

        //for 1st quadrant
        for (int i = 0; i < xcenter; i++)
        {
                for (int j = (ycenter); j < (2 * ycenter); j++)
                {
                        is_radius = get_distance(xcenter, ycenter, i, j, r); //get_distance(int centerx, int centery, int x2, int y2, int r){
                        if (is_radius && space_count == 0)
                        {
                                realPart[i][j] = realPart[i][j] + (alpha * abs(realPart[i][j]) * ((int)char_array[j_count] - 48));
                                j_count++;
                                space_count++;
                                break;
                        }
                        else if (is_radius && (is_radius != 0 && space_count < space))
                        {
                                space_count++;
                                break;
                        }
                        else if (is_radius && (is_radius != 0 && space_count == space))
                        {
                                space_count = 0;
                                break;
                        }
                }
                if (j_count == (binLen))
                {
                        break;
                }
        }

        //4th quadrant
        j_count = 0;
        space_count = 0;
        for (int i = (xcenter + 1); i < (2 * xcenter); i++)
        {
                for (int j = (2 * ycenter); j > ycenter; j--)
                {
                        is_radius = get_distance(xcenter, ycenter, i, j, r); //get_distance(int centerx, int centery, int x2, int y2, int r){
                        if (is_radius && space_count == 0)
                        {
                                realPart[i][j] = realPart[i][j] + (alpha * abs(realPart[i][j]) * ((int)char_array[j_count] - 48));
                                j_count++;
                                space_count++;
                                break;
                        }
                        else if (is_radius && (is_radius != 0 && space_count < space))
                        {
                                space_count++;
                                break;
                        }
                        else if (is_radius && (is_radius != 0 && space_count == space))
                        {
                                space_count = 0;
                                break;
                        }
                }
                if (j_count == (binLen / 2))
                {
                        break;
                }
        }

        //3rd quadrant
        for (int i = (2 * xcenter); i > xcenter; i--)
        {
                for (int j = (ycenter); j >= 0; j--)
                {
                        is_radius = get_distance(xcenter, ycenter, i, j, r); //get_distance(int centerx, int centery, int x2, int y2, int r){
                        if (is_radius && space_count == 0)
                        {
                                realPart[i][j] = realPart[i][j] + (alpha * abs(realPart[i][j]) * ((int)char_array[j_count] - 48));
                                j_count++;
                                space_count++;
                                break;
                        }
                        else if (is_radius && (space_count != 0 && space_count < space))
                        {
                                space_count++;
                                break;
                        }
                        else if (is_radius && (space_count != 0 && space_count == space))
                        {
                                space_count = 0;
                                break;
                        }
                }
                if (j_count == (binLen))
                {
                        break;
                }
        }

        //to implement square
        //
        // if(binLen % 2 == 0){// l is even
        //   r = (3*binLen)/2;
        //   space = (2*r)/binLen;
        // }
        // else{
        //   r = (2*(binLen+1));
        //   space = (r)/(binLen+1);
        // }
        // cout << "binLen:" << binLen << endl;
        // cout << "r:" << r << endl;
        // cout << "space:" << space << endl;
        // cout << "-------------------------" <<endl;
        // for(int i = 0,j=0; i<(2*r) ;i = i+space,j++){
        //   // cout << realPart[xcenter - r][ycenter - r + i] <<"::::" << ((int)char_array[j] - 48);
        //   // cout << "||||";
        //   realPart[xcenter - r][ycenter - r + i] = realPart[xcenter - r][ycenter - r + i] + (alpha * abs(realPart[xcenter - r][ycenter - r + i])*((int)char_array[j] - 48));
        //   realPart[xcenter + r][ycenter + r - i] = realPart[xcenter + r][ycenter + r - i] + (alpha * abs(realPart[xcenter + r][ycenter + r - i])*((int)char_array[j] - 48));
        //   realPart[xcenter - r + i][ycenter + r] = realPart[xcenter - r + i][ycenter + r] + (alpha * abs(realPart[xcenter - r + i][ycenter + r])*((int)char_array[j] - 48));
        //   realPart[xcenter + r - i][ycenter - r] = realPart[xcenter + r - i][ycenter - r] + (alpha * abs(realPart[xcenter + r - i][ycenter - r])*((int)char_array[j] - 48));
        //
        //   imaginaryPart[xcenter - r][ycenter - r + i] = imaginaryPart[xcenter - r][ycenter - r + i] + (alpha * abs(imaginaryPart[xcenter - r][ycenter - r + i])*((int)char_array[j] - 48));
        //   imaginaryPart[xcenter + r][ycenter + r - i] = imaginaryPart[xcenter + r][ycenter + r - i] + (alpha * abs(imaginaryPart[xcenter + r][ycenter + r - i])*((int)char_array[j] - 48));
        //   imaginaryPart[xcenter - r + i][ycenter + r] = imaginaryPart[xcenter - r + i][ycenter + r] + (alpha * abs(imaginaryPart[xcenter - r + i][ycenter + r])*((int)char_array[j] - 48));
        //   imaginaryPart[xcenter + r - i][ycenter - r] = imaginaryPart[xcenter + r - i][ycenter - r] + (alpha * abs(imaginaryPart[xcenter + r - i][ycenter - r])*((int)char_array[j] - 48));
        // }
        /*--------------------------*/
        SDoublePlane output_image, after_watermark_real, after_watermark_imaginary, plot_watermark;
        after_watermark_real = realPart;
        after_watermark_imaginary = imaginaryPart;
        //beforeWaterReal
        //beforeWaterImaginary
        ifft(realPart, imaginaryPart, output_image);
        cout << "-------------------------" << endl;
        // plot the watermarked image
        SImageIO::write_png_file(outputFile.c_str(), output_image, output_image, output_image);

        /*--------------------------*/
        // testing the presence of watermark
        //Generating the png of watermark
        SDoublePlane temp_real(after_watermark_real.rows(), after_watermark_real.cols());
        SDoublePlane temp_imag(after_watermark_real.rows(), after_watermark_real.cols());
        for (int i = 0; i < after_watermark_real.rows(); i++)
        {
                for (int j = 0; j < after_watermark_real.cols(); j++)
                {
                        temp_real[i][j] = abs(after_watermark_real[i][j]) - abs(beforeWaterReal[i][j]);
                        temp_imag[i][j] = abs(after_watermark_imaginary[i][j]) - abs(beforeWaterImaginary[i][j]);
                        // if(temp[i][j] < 0.00 || temp[i][j] > 0.00){
                        //   cout << "(i,j) : (" << i << "," << j << "): " << temp[i][j] << endl;
                        //   temp[i][j] = log10(sqrt(temp[i][j] * temp[i][j]));
                        // }
                }
        }
        plot_watermark = fft_magnitude(temp_real, temp_imag);
        cout << endl;

        // rendering the watermarks below
        string modifiedPNG = "temp.png";
        string modifiedPNG2 = "temp2.png";
        // Frequency plot is done.
        cout << "Watermark frequency plot PNG : " << modifiedPNG << endl;
        SImageIO::write_png_file(modifiedPNG.c_str(), plot_watermark, plot_watermark, plot_watermark);
        ifft(temp_real, temp_imag, plot_watermark);
        // actual image is done.
        cout << "Watermark image : " << modifiedPNG2 << endl;
        SImageIO::write_png_file(modifiedPNG2.c_str(), plot_watermark, plot_watermark, plot_watermark);
        cout << "-------------------------" << endl;
        return output_image;

        /*--------------------------*/
}

// Used this in Part 1.3 -- check if watermark N is in image
SDoublePlane check_image(int myNum, string inputFile)
{

        /*-----------------------------------------------------*/
        // cout << "Just checking if this is even working or not";
        cout << "-------------------------" << endl;
        cout << "In: " << inputFile << endl;
        // i have changed the input string to 16bits of 1's so that when i boost the spectogram
        // i can see if we are marking the right perimeter.
        //string l="1111111111111111";
        string l = gen_random_number(myNum); // string of the binary vector
        cout << "-------------------------" << endl;
        cout << "l: " << l << endl;
        int binLen = l.length();
        SDoublePlane input_image = SImageIO::read_png_file(inputFile.c_str()); // c_str() converts std::String to a "C" type string from a C++ one.
        /*--------------------------*/
        // we have the gray scale image in input_image
        // need to generate its fourier transform
        SDoublePlane realPart, imaginaryPart, beforeWaterReal, beforeWaterImaginary;
        //cout << "(8  & (8-1))" << (8  & (8-1)) << endl;
        fft(input_image, realPart, imaginaryPart);
        beforeWaterReal = realPart;
        beforeWaterImaginary = imaginaryPart;
        /*--------------------------*/
        /*--------------------------*/
        // FFT is done, we need to inject the watermark
        char char_array[binLen + 1];
        strcpy(char_array, l.c_str());

        const int xcenter = realPart.rows() / 2;
        const int ycenter = realPart.rows() / 2;
        float alpha = 0.65; //0.65 is obtained by testing multiple values
        int r, space;

        // to implement a circle

        bool is_radius;
        space = 2; // so the space between two vector values is 2 blocks
        int j_count = 0, space_count = 0;
        if (binLen % 2 == 0)
        { // l is even
                r = 2 * ((binLen * (1 + space)) / 3.14);
        }
        else
        {
                r = 2 * (((binLen + 1) * (1 + space)) / 3.14); //2 * ( ((binLen + 1) * (1 + space))/3.14 );
        }
        int temp_value = (2 * 3.14 * r);
        float extrated_values[temp_value + 10];
        int counter = 0;
        cout << "binLen:" << binLen << endl;
        cout << "r:" << r << endl;
        cout << "space:" << space << endl;
        cout << "-------------------------" << endl;
        //Retrieving the bins depending on the seeded N and checking the coefficient
        // for 2nd quadrant
        for (int i = (xcenter - 1); i >= 0; i--)
        {
                for (int j = 0; j < ycenter; j++)
                {
                        is_radius = get_distance(xcenter, ycenter, i, j, r); //get_distance(int centerx, int centery, int x2, int y2, int r){
                        if (is_radius && space_count == 0)
                        {
                                //realPart[i][j] = realPart[i][j] + (alpha * abs(realPart[i][j]) * ((int)char_array[j_count] - 48) );
                                extrated_values[counter] = realPart[i][j];
                                counter++;
                                j_count++;
                                space_count++;
                                break;
                        }
                        else if (is_radius && (is_radius != 0 && space_count < space))
                        {
                                // extrated_values[counter] = realPart[i][j];
                                // counter++;
                                space_count++;
                                break;
                        }
                        else if (is_radius && (is_radius != 0 && space_count == space))
                        {
                                // extrated_values[counter] = realPart[i][j];
                                // counter++;
                                space_count = 0;
                                break;
                        }
                }
                if (j_count == (binLen / 2))
                {
                        break;
                }
        }

        //for 1st quadrant
        for (int i = 0; i < xcenter; i++)
        {
                for (int j = (ycenter); j < (2 * ycenter); j++)
                {
                        is_radius = get_distance(xcenter, ycenter, i, j, r); //get_distance(int centerx, int centery, int x2, int y2, int r){
                        if (is_radius && space_count == 0)
                        {
                                // realPart[i][j] = realPart[i][j] + (alpha * abs(realPart[i][j]) * ((int)char_array[j_count] - 48) );
                                extrated_values[counter] = realPart[i][j];
                                counter++;
                                j_count++;
                                space_count++;
                                break;
                        }
                        else if (is_radius && (is_radius != 0 && space_count < space))
                        {
                                // extrated_values[counter] = realPart[i][j];
                                // counter++;
                                space_count++;
                                break;
                        }
                        else if (is_radius && (is_radius != 0 && space_count == space))
                        {
                                // extrated_values[counter] = realPart[i][j];
                                // counter++;
                                space_count = 0;
                                break;
                        }
                }
                if (j_count == (binLen))
                {
                        break;
                }
        }

        //4th quadrant
        j_count = 0;
        space_count = 0;
        for (int i = (xcenter + 1); i < (2 * xcenter); i++)
        {
                for (int j = (2 * ycenter); j > ycenter; j--)
                {
                        is_radius = get_distance(xcenter, ycenter, i, j, r); //get_distance(int centerx, int centery, int x2, int y2, int r){
                        if (is_radius && space_count == 0)
                        {
                                // realPart[i][j] = realPart[i][j] + (alpha * abs(realPart[i][j]) * ((int)char_array[j_count] - 48) );
                                extrated_values[counter] = realPart[i][j];
                                counter++;
                                j_count++;
                                space_count++;
                                break;
                        }
                        else if (is_radius && (is_radius != 0 && space_count < space))
                        {
                                // extrated_values[counter] = realPart[i][j];
                                // counter++;
                                space_count++;
                                break;
                        }
                        else if (is_radius && (is_radius != 0 && space_count == space))
                        {
                                // extrated_values[counter] = realPart[i][j];
                                // counter++;
                                space_count = 0;
                                break;
                        }
                }
                if (j_count == (binLen / 2))
                {
                        break;
                }
        }

        //3rd quadrant
        for (int i = (2 * xcenter); i > xcenter; i--)
        {
                for (int j = (ycenter); j >= 0; j--)
                {
                        is_radius = get_distance(xcenter, ycenter, i, j, r); //get_distance(int centerx, int centery, int x2, int y2, int r){
                        if (is_radius && space_count == 0)
                        {
                                // realPart[i][j] = realPart[i][j] + (alpha * abs(realPart[i][j]) * ((int)char_array[j_count] - 48) );
                                extrated_values[counter] = realPart[i][j];
                                counter++;
                                j_count++;
                                space_count++;
                                break;
                        }
                        else if (is_radius && (space_count != 0 && space_count < space))
                        {
                                // extrated_values[counter] = realPart[i][j];
                                // counter++;
                                space_count++;
                                break;
                        }
                        else if (is_radius && (space_count != 0 && space_count == space))
                        {
                                // extrated_values[counter] = realPart[i][j];
                                // counter++;
                                space_count = 0;
                                break;
                        }
                }
                if (j_count == (binLen))
                {
                        break;
                }
        }

        //Coefficient calculation
        float set1[binLen]; //has corresponding values of 1
        float set2[binLen]; //has corresponding values of 0
        int set1_count = 0;
        int set2_count = 0;
        for (int i = 0; i < binLen; i++)
        {
                if (((int)char_array[i] - 48) == 1)
                {
                        set1[set1_count] = (extrated_values[i]);
                        set1_count++;
                }
                else if (((int)char_array[i] - 48) == 0)
                {
                        set2[set2_count] = (extrated_values[i]);
                        set2_count++;
                }
        }

        float meanSet1 = 0, meanSet2 = 0, sigmaSet1 = 0, sigmaSet2 = 0, sigmaSet1Sq = 0, sigmaSet2Sq = 0;
        float temp = 0, temp2 = 0;
        for (int i = 0; i < set1_count; i++)
        {
                temp = temp + set1[i];
                sigmaSet1Sq = sigmaSet1Sq + (set1[i] * set1[i]);
        }

        sigmaSet1 = temp; //sigma x
        meanSet1 = temp / set1_count;
        float jeevanTemp1 = 0;
        for (int i = 0; i < set1_count; i++)
        {
                jeevanTemp1 = jeevanTemp1 + ((set1[i] - meanSet1) * (set1[i] - meanSet1));
        }
        float jeevanTemp11 = jeevanTemp1 / set1_count;

        temp2 = 0;
        for (int i = 0; i < set2_count; i++)
        {
                temp2 = temp2 + set2[i];
                sigmaSet2Sq = sigmaSet2Sq + (set2[i] * set2[i]);
        }
        sigmaSet2 = temp2;
        meanSet2 = temp2 / set2_count;
        float jeevanTemp2 = 0;
        for (int i = 0; i < set2_count; i++)
        {
                //cout << "set2 "<< set2[i] <<  endl;
                jeevanTemp2 = jeevanTemp2 + ((set2[i] - meanSet2) * (set2[i] - meanSet2));
        }
        float jeevanTemp22 = jeevanTemp2 / set2_count;
        //cout << "jeevanTemp22 " << jeevanTemp22 << endl;
        float mmymyy = ((sigmaSet1) * (sigmaSet1));
        float SDset1 = jeevanTemp11; //(  ( (sigmaSet1Sq) - ( (sigmaSet1) * (sigmaSet1) ) ) / (set1_count) );
        float SDset2 = jeevanTemp22; //(  ( (sigmaSet2Sq) - ( (sigmaSet2) * (sigmaSet2) ) ) / (set2_count) );

        float jeevanT_numerator = ((meanSet1) - (meanSet2));
        float jeevanT_denominator = sqrt(((SDset1) / (set1_count)) + ((SDset2) / (set2_count)));
        float pValue = (jeevanT_numerator) / (jeevanT_denominator);

        cout << "pValue: " << pValue << endl;

        if (pValue>=1.1 || pValue<=-1.1){
             cout << "WaterMark is present"<< endl;
        }
        else{
                cout << "WaterMark is not present"<< endl;
        }
        // for (int i=0; i<(binLen); i++)
        //   cout << i << ":::" << "("<<((int)char_array[i] - 48)<< ": "<< extrated_values[i] <<")"<<endl;

        cout << "-------------------------" << endl;
        return beforeWaterImaginary;
}

int main(int argc, char **argv)
{
        try
        {
                if (argc < 4)
                {
                        cout << "Insufficent number of arguments; correct usage:" << endl;
                        cout << "    p2 problemID inputfile outputfile" << endl;
                        return -1;
                }
                string part = argv[1];
                string inputFile = argv[2];
                string outputFile = argv[3];
                SDoublePlane input_image = SImageIO::read_png_file(inputFile.c_str());
                if (part == "1.1")
                {
                        // check if you have to plot the DFT of the image or
                        // Do ifft and then plot the image
                        cout << "In part 1.1" << endl;
                        SDoublePlane realPart, imaginaryPart;
                        fft(input_image, realPart, imaginaryPart);
                        SDoublePlane output_image = fft_magnitude(realPart, imaginaryPart);
                        SImageIO::write_png_file(outputFile.c_str(), output_image, output_image, output_image);
                }
                else if (part == "1.2")
                {
                        cout << "In part 1.2" << endl;
                        SDoublePlane output_image = remove_interference(input_image);
                        SImageIO::write_png_file(outputFile.c_str(), output_image, output_image, output_image);
                }
                else if (part == "1.3")
                {
                        cout << "In part 1.3" << endl;
                        if (argc < 6)
                        {
                                cout << "Need 6 parameters for watermark part:" << endl;
                                cout << "    p2 1.3 inputfile outputfile operation N" << endl;
                                return -1;
                        }
                        string op(argv[4]);
                        string myNum(argv[5]);
                        stringstream geek(myNum);
                        int myNumx = 0;
                        geek >> myNumx;
                        if (op == "add")
                        {
                                // add watermark
                                // run with ./watermark 1.3 PNGInputImage512.png Sample24.png add 10001
                                //part1check(myNumx,inputFile,outputFile);
                                SDoublePlane modifiedFileName = mark_image(myNumx, inputFile, outputFile);
                                cout << "-------------------------" << endl;
                        }
                        else if (op == "check")
                        {
                                // check watermark
                                check_image(myNumx, inputFile);
                        }
                        else
                                throw string("Bad operation!");

                        int N = atoi(argv[5]);
                }
                else
                        throw string("Bad part!");
        }
        catch (const string &err)
        {
                cerr << "Error: " << err << endl;
        }
}
