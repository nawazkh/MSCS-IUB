//
// detect.cpp : Detect integrated circuits in printed circuit board (PCB) images.
//
// Based on skeleton code by D. Crandall, Spring 2018
//
//      jeereddy-nawazkh-pkurusal-rpochamp-a1
// jeereddy - Jeevan Reddy
// nawazkh - Nawaz Hussain K
// pkurusal - Pruthvi Raj Kurusala
// rpochamp - Rahul Pochampally
//
//

#include <SImage.h>
#include <SImageIO.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

#include <sstream>
#include <cmath>
#include <stdio.h>
#include <string>
#include <fft.h>
#include <algorithm>
#include <iterator>
#include <iomanip>

using namespace std;

// The simple image class is called SDoublePlane, with each pixel represented as
// a double (floating point) type. This means that an SDoublePlane can represent
// values outside the range 0-255, and thus can represent squared gradient magnitudes,
// harris corner scores, etc.
//
// The SImageIO class supports reading and writing PNG files. It will read in
// a color PNG file, convert it to grayscale, and then return it to you in
// an SDoublePlane. The values in this SDoublePlane will be in the range [0,255].
//
// To write out an image, call write_png_file(). It takes three separate planes,
// one for each primary color (red, green, blue). To write a grayscale image,
// just pass the same SDoublePlane for all 3 planes. In order to get sensible
// results, the values in the SDoublePlane should be in the range [0,255].
//

// Forward FFT transform: take input image, and return real and imaginary parts.
//
void fft(const SDoublePlane &input, SDoublePlane &fft_real, SDoublePlane &fft_imag){
        fft_real = input;
        fft_imag = SDoublePlane(input.rows(), input.cols());

        FFT_2D(1, fft_real, fft_imag);
}

// Inverse FFT transform: take real and imaginary parts of fourier transform, and return
//  real-valued image.
//
void ifft(const SDoublePlane &input_real, const SDoublePlane &input_imag, SDoublePlane &output_real){
        output_real = input_real;
        SDoublePlane output_imag = input_imag;

        FFT_2D(0, output_real, output_imag);
}





// Below is a helper functions that overlays rectangles
// on an image plane for visualization purpose.

// Draws a rectangle on an image plane, using the specified gray level value and line width.
//
void overlay_rectangle(SDoublePlane &input, int _top, int _left, int _bottom, int _right, double graylevel, int width)
{
        for(int w=-width/2; w<=width/2; w++) {
                int top = _top+w, left = _left+w, right=_right+w, bottom=_bottom+w;

                // if any of the coordinates are out-of-bounds, truncate them
                top = min( max( top, 0 ), input.rows()-1);
                bottom = min( max( bottom, 0 ), input.rows()-1);
                left = min( max( left, 0 ), input.cols()-1);
                right = min( max( right, 0 ), input.cols()-1);

                // draw top and bottom lines
                for(int j=left; j<=right; j++)
                        input[top][j] = input[bottom][j] = graylevel;
                // draw left and right lines
                for(int i=top; i<=bottom; i++)
                        input[i][left] = input[i][right] = graylevel;
        }
}

// DetectedBox class may be helpful!
//  Feel free to modify.
//
class DetectedBox {
public:
int row, col, width, height;
double confidence;
};

// Function that outputs the ascii detection output file
void  write_detection_txt(const string &filename, const vector<DetectedBox> &ics)
{
        ofstream ofs(filename.c_str());

        for(vector<DetectedBox>::const_iterator s=ics.begin(); s != ics.end(); ++s)
                ofs << s->row << " " << s->col << " " << s->width << " " << s->height << " " << s->confidence << endl;
}

// Function that outputs a visualization of detected boxes
void  write_detection_image(const string &filename, const vector<DetectedBox> &ics, const SDoublePlane &input)
{
        SDoublePlane output_planes[3];

        for(int p=0; p<3; p++)
        {
                output_planes[p] = input;
                for(vector<DetectedBox>::const_iterator s=ics.begin(); s != ics.end(); ++s)
                        overlay_rectangle(output_planes[p], s->row, s->col, s->row+s->height-1, s->col+s->width-1, p==2 ? 255 : 0, 2);
        }

        SImageIO::write_png_file(filename.c_str(), output_planes[0], output_planes[1], output_planes[2]);
}



// The rest of these functions are incomplete. These are just suggestions to
// get you started -- feel free to add extra functions, change function
// parameters, etc.

// To create gaussian filters
SDoublePlane filter_creation(double sigma)
{

        int center = (int)(3.0 * sigma);
        SDoublePlane kernel(1, 2 * center + 1);
        double sigma2 = sigma * sigma;
        for (int i = 0; i < 2 * center + 1; i++)
        {
                double r = center - i;
                kernel[0][i] = (float)exp(-0.5 * (r * r) / sigma2);
                // cout << kernel[0][i] << endl;
        }
        return kernel;
}

// handle borders of the image_edges
int flippedImage(int M, int x){
        if(x < 0)
        {
                return -x - 1;
        }
        if(x >= M)
        {
                return 2*M - x - 1;
        }
        return x;
}

// Convolve an image with a separable convolution kernel
//
SDoublePlane convolve_separable(const SDoublePlane &input, const SDoublePlane &row_filter, const SDoublePlane &col_filter)
{
        SDoublePlane output(input.rows(), input.cols());
        SDoublePlane temp(input.rows(), input.cols());

        int lengthOfKernel_row = row_filter.rows()/2;
        int lengthOfKernel_column = col_filter.rows()/2;

        //flipping the image () mirror image) to handle the edges for only 3 X 3 kernel
        float sum = 0;
        int x1 = 0, y1 = 0;
        for(int y = 0; y < input.rows(); y++) {
                for(int x = 0; x < input.cols(); x++) {
                        sum = 0.0;
                        for(int i = ((-1) * lengthOfKernel_row); i <= lengthOfKernel_row; i++) {
                                y1 = flippedImage(input.rows(), y - i);
                                sum = sum + (row_filter[(i + lengthOfKernel_row)][0] * input[y1][x]);
                        }
                        temp[y][x] = sum;
                }
        }
        sum = 0;
        for(int y = 0; y < input.rows(); y++) {
                for(int x = 0; x < input.cols(); x++) {
                        sum = 0.0;
                        for(int i = ((-1)*lengthOfKernel_column); i <= lengthOfKernel_column; i++) {
                                x1 = flippedImage(input.cols(), x - i);
                                //cout << "x1" << x1 << endl;
                                sum = sum + col_filter[i + lengthOfKernel_column][0] * temp[y][x1];
                        }
                        output[y][x] = sum;
                }
        }

        return output;
}

// Convolve an image with a  convolution kernel
//
SDoublePlane convolve_general(const SDoublePlane &input, const SDoublePlane &filter)
{
        SDoublePlane output(input.rows(), input.cols());

        // Convolution code here
        // we are cropping the picture. not considering the edges of the image.
        int lengthOfKernel = filter.rows()/2;
        float sum;
        for(int y = lengthOfKernel; y < input.rows() - lengthOfKernel; y++) {
                for(int x = lengthOfKernel; x < input.cols() - lengthOfKernel; x++) {
                        sum = 0.0;
                        for(int k = (-1)*lengthOfKernel; k <= lengthOfKernel; k++) {
                                for(int j = (-1)*lengthOfKernel; j <=lengthOfKernel; j++) {
                                        sum = sum + filter[j+1][k+1]*input[y - j][x - k];
                                }
                        }
                        output[y][x] = sum;
                }
        }

        return output;
}


// Apply a sobel operator to an image, returns the result
//
SDoublePlane sobel_gradient_filter(const SDoublePlane &input, int choice)
{
        SDoublePlane output(input.rows(), input.cols());

        // Implement a sobel gradient estimation filter with 1-d filters
        SDoublePlane xKernel_v(3, 1);
        xKernel_v[0][0] = 1;
        xKernel_v[1][0] = 2;
        xKernel_v[2][0] = 1;

        SDoublePlane xKernel_h(3, 1);
        xKernel_h[0][0] = 1;
        xKernel_h[1][0] = 0;
        xKernel_h[2][0] = -1;

        SDoublePlane yKernel_v(3, 1);
        yKernel_v[0][0] = 1;
        yKernel_v[1][0] = 0;
        yKernel_v[2][0] = -1;

        SDoublePlane yKernel_h(3, 1);
        yKernel_h[0][0] = 1;
        yKernel_h[1][0] = 2;
        yKernel_h[2][0] = 1;

        if(choice == 0) {
                output = convolve_separable(input, xKernel_v, xKernel_h); //SobelX
        }else{
                output = convolve_separable(input, yKernel_v, yKernel_h); //SobelY
        }
        //SImageIO::write_png_file("sobelXX.png", sobelX, sobelX, sobelX);
        return output;
}

SDoublePlane gradeSobel(SDoublePlane &xSobel, SDoublePlane & ySobel){
        SDoublePlane output(xSobel.rows(), xSobel.cols());
        for(int i = 0; i < xSobel.rows(); i++) {
                for(int j = 0; j < ySobel.cols(); j++) {
                        output[i][j] = abs(xSobel[i][j]) - ySobel[i][j];
                        if(output[i][j] > 220) {
                                output[i][j] = 255;
                        }
                        else{
                                output[i][j] = 0;
                        }

                }
        }
        return output;
}

SDoublePlane thickifyLines(SDoublePlane & input){
        SDoublePlane output(input.rows(), input.cols());
        for(int i = 5; i < input.rows() - 5; i++) {
                for(int j = 5; j < input.cols() - 5; j++) {
                        if(input[i][j] == 255) {
                                output[i][j] = 255;
                                if(input[i-1][j] == 0) {
                                        output[i-1][j] = 255;
                                }
                                // if(input[i-2][j] == 0) {
                                //         output[i-2][j] = 255;
                                // }
                                // if(input[i-3][j] == 0) {
                                //         output[i-2][j] = 255;
                                // }
                                // if(input[i-4][j] == 0) {
                                //         output[i-2][j] = 255;
                                // }

                                // if(input[i+1][j] == 0) {
                                //         output[i+1][j] = 255;
                                // }
                                // if(input[i+2][j] == 0) {
                                //         output[i+2][j] = 255;
                                // }
                                // if(input[i+3][j] == 0) {
                                //         output[i+2][j] = 255;
                                // }
                                // if(input[i+4][j] == 0) {
                                //         output[i+2][j] = 255;
                                // }

                                if(input[i][j-1] == 0) {
                                        output[i][j-1] = 255;
                                }
                                // if(input[i][j-2] == 0) {
                                //         output[i][j-2] = 255;
                                // }
                                // if(input[i][j-3] == 0) {
                                //         output[i][j-2] = 255;
                                // }
                                // if(input[i][j-4] == 0) {
                                //         output[i][j-2] = 255;
                                // }
                                // if(input[i][j+1] == 0) {
                                //         output[i][j+1] = 255;
                                // }
                                // if(input[i][j+2] == 0) {
                                //         output[i][j+2] = 255;
                                // }
                                // if(input[i][j+3] == 0) {
                                //         output[i][j+2] = 255;
                                // }
                                // if(input[i][j+4] == 0) {
                                //         output[i][j+2] = 255;
                                // }
                        }
                        else{
                                output[i][j] = 0;
                        }
                }
        }
        return output;
}

// getting binary image_edges
SDoublePlane getBinaryImage(SDoublePlane &input){
        SDoublePlane output(input.rows(),input.cols());
        for(int i = 0; i < input.rows(); i++) {
                for(int j = 0; j < input.cols(); j++) {
                        if(input[i][j] > 200) {
                                output[i][j] = 255;
                        }
                        else{
                                output[i][j] = 0;
                        }

                }
        }
        return output;
}

// Apply an edge detector to an image, returns the binary edge map
//
vector<DetectedBox> find_edges(const SDoublePlane &input,const SDoublePlane &mytemplate, double thresh,vector<DetectedBox> &ics)
{
        SDoublePlane output(input.rows(), input.cols());
        if(!( ( (mytemplate.rows()) * (mytemplate.cols()) ) >= ((input.rows())*(input.cols())*0.015) ) ) {
                return ics;
        }

        if((input.rows()) <= (mytemplate.rows()+20) || input.cols() <= (mytemplate.cols()+20)) {
                return ics;
        }



        /* --- starting here ---*/
        //vector<DetectedBox> ics;
        DetectedBox s;
        cout << "input.rows(): " << input.rows() << " input.cols(): " << input.cols() << endl;
        cout << "mytemplate.rows(): " << mytemplate.rows() << " mytemplate.cols(): " << mytemplate.cols() << endl;
        SDoublePlane crossCo(input.rows(), input.cols());

        //int n = template1.rows() * template1.cols();
        int tempX = mytemplate.rows() / 2;
        int tempY = mytemplate.cols() / 2;
        int ii, jj;
        int productSum, tempSum, imageSum, tempSqSum, imageSqSum, tempSumSq, imageSumSq;



        productSum = 0, tempSum = 0, imageSum = 0, tempSqSum = 0, imageSqSum = 0, tempSumSq = 0, imageSumSq = 0;
        for (int ai = 0; ai < mytemplate.rows(); ai++)
        {
                for (int aj = 0; aj < mytemplate.cols(); aj++)
                {
                        //if(mytemplate[ai][aj] != 0){
                        tempSqSum += mytemplate[ai][aj] * mytemplate[ai][aj];
                        //}

                }
        }
        float lowerbound = (tempSqSum * 0.88125);
        float upperbound = (tempSqSum * 1.1);
        int limit_i =0;

        for (int i = tempX; i < input.rows() - tempX; i = i + 1)// chnge it to  = input.rows() - tempX
        {
                for (int j = tempY; j < input.cols() - tempY; j = j + 2)
                {
                        productSum = 0, tempSum = 0, imageSum = 0, tempSqSum = 0, imageSqSum = 0, tempSumSq = 0, imageSumSq = 0;
                        for (int ai = -tempX; ai < tempX; ai++)
                        {
                                for (int aj = -tempY; aj < tempY; aj++)
                                {
                                        // if(input[i + ai][j + aj] != 0) {
                                        //
                                        // }
                                        //input.rows() - tempX - 1 + tempX -1
                                        //tempX + -tempX
                                        //tempX-1 + tempX
                                        productSum += input[i + ai][j + aj] * mytemplate[ai + tempX][aj + tempY];
                                        // tempSum += template1[ai + tempX][aj + tempY];
                                        // imageSum += output[i + ai][j + aj];
                                        // tempSqSum += mytemplate[ai + tempX][aj + tempY] * mytemplate[ai + tempX][aj + tempY];
                                        //imageSqSum += input[i + ai][j + aj] * input[i + ai][j + aj];
                                }
                        }
                        if(productSum >= lowerbound && productSum <= upperbound && limit_i == 0) {
                                ii = i;
                                jj = j;
                                limit_i = i;
                                cout << "000" << endl;
                                cout << "limit_i"<< limit_i << endl;
                                cout << "productSum"<< productSum << endl;
                                cout << "(i,j)s : "<< i << "," << j << ")"<< endl;
                                cout << "000" << endl;
                                s.row = ii - mytemplate.rows() / 2;
                                s.col = jj - mytemplate.cols() / 2;
                                s.width = mytemplate.cols();
                                s.height = mytemplate.rows();
                                s.confidence = thresh;
                                ics.push_back(s);
                        }
                        else if(productSum >= lowerbound && productSum <= upperbound && limit_i != 0)
                        {
                                if(i >= limit_i-1 && i <= limit_i+20) {
                                        cout << "(i,j)s : "<< i << "," << j << ")"<< endl;
                                }
                                else{
                                        ii = i;
                                        jj = j;
                                        limit_i = i;
                                        cout << limit_i << endl;
                                        cout << "limit_i"<< limit_i << endl;
                                        cout << "productSum"<< productSum << endl;
                                        cout << "(i,j)s : "<< i << "," << j << ")"<< endl;
                                        cout << limit_i << endl;
                                        s.row = ii - mytemplate.rows() / 2;
                                        s.col = jj - mytemplate.cols() / 2;
                                        s.width = mytemplate.cols();
                                        s.height = mytemplate.rows();
                                        s.confidence = thresh;
                                        ics.push_back(s);
                                }
                        }
                }
        }


        //cout << "coordinates: " << ii << "->" << jj << endl;
        //SImageIO::write_png_file("crossCo.png", crossCo, crossCo, crossCo);
        return ics;

}

SDoublePlane getInputBinary(const SDoublePlane &input_image){
        //checking sobel operator
        SDoublePlane xOutput = sobel_gradient_filter(input_image,0);// 0 for X direction
        //SImageIO::write_png_file("imageSobel_XX.png", xOutput, xOutput, xOutput);
        SDoublePlane yOutput = sobel_gradient_filter(input_image,1);// 1 for Y direction
        //SImageIO::write_png_file("imageSobel_YY.png", yOutput, yOutput, yOutput);
        SDoublePlane finalSobelOP = gradeSobel(xOutput,yOutput);
        SImageIO::write_png_file("edges.png", finalSobelOP, finalSobelOP, finalSobelOP);

        // using imageSobel_justLines.png, i will thicken the lines.
        SDoublePlane thickLines = thickifyLines(finalSobelOP);
        thickLines = thickifyLines(thickLines);
        //thickLines = convolve_general(thickLines, mean_filter);
        //SImageIO::write_png_file("imageSobel_thickLines.png", thickLines, thickLines, thickLines);

        // generating binary image.
        SDoublePlane binaryImage = getBinaryImage(thickLines);
        SImageIO::write_png_file("thickLines_binary.png", binaryImage, binaryImage, binaryImage);
        /*-----------------------------*/
        return binaryImage;
}

SDoublePlane getTempleteBinary(const SDoublePlane &template_image){
        /*-------------------------------------*/
        //checking sobel operator
        SDoublePlane xOutput_temp = sobel_gradient_filter(template_image,0);// 0 for X direction
        //SImageIO::write_png_file("imageSobel_XX_template.png", xOutput_temp, xOutput_temp, xOutput_temp);
        SDoublePlane yOutput_temp = sobel_gradient_filter(template_image,1);// 1 for Y direction
        //SImageIO::write_png_file("imageSobel_YY_template.png", yOutput_temp, yOutput_temp, yOutput_temp);
        SDoublePlane finalSobelOP_temp = gradeSobel(xOutput_temp,yOutput_temp);
        //SImageIO::write_png_file("imageSobel_justLines_template.png", finalSobelOP_temp, finalSobelOP_temp, finalSobelOP_temp);

        // using imageSobel_justLines.png, i will thicken the lines.
        SDoublePlane thickLines_temp = thickifyLines(finalSobelOP_temp);
        thickLines_temp = thickifyLines(thickLines_temp);
        //thickLines_temp = convolve_general(thickLines_temp, mean_filter);
        //SImageIO::write_png_file("imageSobel_thickLines_template.png", thickLines_temp, thickLines_temp, thickLines_temp);

        // generating binary image.
        SDoublePlane binaryImage_temp = getBinaryImage(thickLines_temp);
        SImageIO::write_png_file("thickLines_binary_template.png", binaryImage_temp, binaryImage_temp, binaryImage_temp);
        /*-----------------------------*/
        return binaryImage_temp;
}

//
// This main file just outputs a few test images. You'll want to change it to do
//  something more interesting!
//
int main(int argc, char *argv[])
{
        if(!(argc == 2))
        {
                cerr << "usage: " << argv[0] << " input_image" << endl;
                return 1;
        }
        // if(!(argc == 3))
        // {
        //         cerr << "usage: " << argv[0] << " input_image" << " template_image" << endl;
        //         return 1;
        // }

        string input_filename(argv[1]);
        //string template_filename(argv[2]);
        SDoublePlane input_image= SImageIO::read_png_file(input_filename.c_str());
        //fft(input_image,input_realPart,input_imagPart);



        // test step 2 by applying mean filters to the input image
        SDoublePlane mean_filter(3,3);
        for(int i=0; i<3; i++)
                for(int j=0; j<3; j++)
                        mean_filter[i][j] = 1/9.0;
        // SDoublePlane output_image = convolve_general(input_image, mean_filter);
        // // checking the convolve_general
        // SImageIO::write_png_file("imageOriginalGray.png", input_image, input_image, input_image);
        // SImageIO::write_png_file("imageMean_general.png", output_image, output_image, output_image);
        //
        // //checking the convolve_seperable
        // double sigma = 3;
        // SDoublePlane gKernel = filter_creation(sigma);
        // SDoublePlane output_image_seperable = convolve_general(input_image, mean_filter);
        // SImageIO::write_png_file("imageMean_seperable.png", output_image_seperable, output_image_seperable, output_image_seperable);
        // /*-------------------------------------*/
        SDoublePlane inputBinary = getInputBinary(input_image);
        // /*-------------------------------------*/
        // //checking sobel operator
        // SDoublePlane xOutput_temp = sobel_gradient_filter(template_image,0);// 0 for X direction
        // //SImageIO::write_png_file("imageSobel_XX_template.png", xOutput_temp, xOutput_temp, xOutput_temp);
        // SDoublePlane yOutput_temp = sobel_gradient_filter(template_image,1);// 1 for Y direction
        // //SImageIO::write_png_file("imageSobel_YY_template.png", yOutput_temp, yOutput_temp, yOutput_temp);
        // SDoublePlane finalSobelOP_temp = gradeSobel(xOutput_temp,yOutput_temp);
        // //SImageIO::write_png_file("imageSobel_justLines_template.png", finalSobelOP_temp, finalSobelOP_temp, finalSobelOP_temp);
        //
        // // using imageSobel_justLines.png, i will thicken the lines.
        // SDoublePlane thickLines_temp = thickifyLines(finalSobelOP_temp);
        // thickLines_temp = thickifyLines(thickLines_temp);
        // //thickLines_temp = convolve_general(thickLines_temp, mean_filter);
        // //SImageIO::write_png_file("imageSobel_thickLines_template.png", thickLines_temp, thickLines_temp, thickLines_temp);
        //
        // // generating binary image.
        // SDoublePlane binaryImage_temp = getBinaryImage(thickLines_temp);
        // SImageIO::write_png_file("thickLines_binary_template.png", binaryImage_temp, binaryImage_temp, binaryImage_temp);
        // /*-----------------------------*/


        //cout << "hi";
        //from here use the templates
        string temp = "templates/temp";
        vector<DetectedBox> linesOutput;
        for(int i = 1; i <= 25; i++) {
                cout << "----------" << endl;
                stringstream ss;
                ss << i;
                string str = ss.str();
                temp = temp+str+".png";
                //temp = temp+std::to_string(13)+".png";
                SDoublePlane template_image= SImageIO::read_png_file(temp.c_str());
                cout << "template file: " << temp << endl << endl;
                SDoublePlane templateBinary = getTempleteBinary(template_image);
                linesOutput = find_edges(inputBinary,templateBinary,70,linesOutput);
                temp = "templates/temp";
                write_detection_txt("detected.txt", linesOutput);
                write_detection_image("detected.png", linesOutput, input_image);
                cout << "----------" << endl;
                //break;

        }
        write_detection_txt("detected.txt", linesOutput);
        write_detection_image("detected.png", linesOutput, input_image);


        // SDoublePlane template_image= SImageIO::read_png_file(template_filename.c_str());
        // SDoublePlane templateBinary = getTempleteBinary(template_image);
        //write_detection_txt("detected.txt", linesOutput);
        //write_detection_image("detected.png", linesOutput, input_image);



        //using the thickened lines, I will find for lines
        // vector<DetectedBox> linesOutput = find_edges(binaryImage,binaryImage_temp,70);
        //vector<DetectedBox> ics;
        //ics.push_back(linesOutput);
        //ics.push_back(s2);

        // randomly generate some detected ics -- you'll want to replace this
        //  with your ic detection code obviously!

        // for(int i=0; i<10; i++)
        // {
        //         DetectedBox s;
        //         s.row = rand() % input_image.rows();
        //         s.col = rand() % input_image.cols();
        //         s.width = 20;
        //         s.height = 20;
        //         s.confidence = rand();
        //         ics.push_back(s);
        // }

        // write_detection_txt("detected.txt", linesOutput);
        // write_detection_image("detected.png", linesOutput, input_image);
}
