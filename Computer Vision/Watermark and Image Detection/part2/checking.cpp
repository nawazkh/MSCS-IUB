//
// detect.cpp : Detect integrated circuits in printed circuit board (PCB) images.
//
// Based on skeleton code by D. Crandall, Spring 2018
//
// PUT YOUR NAMES HERE
//
//

#include <SImage.h>
#include <SImageIO.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

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

// Below is a helper functions that overlays rectangles
// on an image plane for visualization purpose.

// Draws a rectangle on an image plane, using the specified gray level value and line width.
//
void overlay_rectangle(SDoublePlane &input, int _top, int _left, int _bottom, int _right, double graylevel, int width)
{
    for (int w = -width / 2; w <= width / 2; w++)
    {
        int top = _top + w, left = _left + w, right = _right + w, bottom = _bottom + w;

        // if any of the coordinates are out-of-bounds, truncate them
        top = min(max(top, 0), input.rows() - 1);
        bottom = min(max(bottom, 0), input.rows() - 1);
        left = min(max(left, 0), input.cols() - 1);
        right = min(max(right, 0), input.cols() - 1);

        // draw top and bottom lines
        for (int j = left; j <= right; j++)
            input[top][j] = input[bottom][j] = graylevel;
        // draw left and right lines
        for (int i = top; i <= bottom; i++)
            input[i][left] = input[i][right] = graylevel;
    }
}

// DetectedBox class may be helpful!
//  Feel free to modify.
//
class DetectedBox
{
public:
    int row, col, width, height;
    double confidence;
};

// Function that outputs the ascii detection output file
void write_detection_txt(const string &filename, const vector<DetectedBox> &ics)
{
    ofstream ofs(filename.c_str());

    for (vector<DetectedBox>::const_iterator s = ics.begin(); s != ics.end(); ++s)
        ofs << s->row << " " << s->col << " " << s->width << " " << s->height << " " << s->confidence << endl;
}

// Function that outputs a visualization of detected boxes
void write_detection_image(const string &filename, const vector<DetectedBox> &ics, const SDoublePlane &input)
{
    SDoublePlane output_planes[3];

    for (int p = 0; p < 3; p++)
    {
        output_planes[p] = input;
        for (vector<DetectedBox>::const_iterator s = ics.begin(); s != ics.end(); ++s)
            overlay_rectangle(output_planes[p], s->row, s->col, s->row + s->height - 1, s->col + s->width - 1, p == 2 ? 255 : 0, 2);
    }

    SImageIO::write_png_file(filename.c_str(), output_planes[0], output_planes[1], output_planes[2]);
}

// The rest of these functions are incomplete. These are just suggestions to
// get you started -- feel free to add extra functions, change function
// parameters, etc.

// Convolve an image with a separable convolution kernel
//
int flippedImage(int dim, int k)
{
    if (k < 0)
        return -k - 1;
    if (k >= dim)
        return 2 * dim - k - 1;
    return k;
}
SDoublePlane convolve_separable(const SDoublePlane &input, const SDoublePlane &row_filter, const SDoublePlane &col_filter)
{
    SDoublePlane output(input.rows(), input.cols());
    SDoublePlane temp(input.rows(), input.cols());

    float sum = 0.0;
    int kCenter = row_filter.rows() / 2;
    // Convolution code here
    for (int i = 0; i < input.rows(); i++)
    {
        for (int j = 0; j < input.cols(); j++)
        {
            sum = 0.0;
            for (int k = -kCenter; k <= kCenter; k++)
            {
                sum = sum + row_filter[kCenter + k][0] * input[flippedImage(input.rows(), i - k)][j];
            }
            temp[i][j] = sum;
        }
    }

    for (int i = 0; i < input.rows(); i++)
    {
        for (int j = 0; j < input.cols(); j++)
        {
            sum = 0.0;
            for (int k = -kCenter; k <= kCenter; k++)
            {
                sum = sum + col_filter[0][kCenter + k] * temp[i][flippedImage(input.cols(), j - k)];
            }
            output[i][j] = sum;
        }
    }

    double max = 0;
    for (int i = 0; i < output.rows(); i++)
    {
        for (int j = 0; j < output.cols(); j++)
        {
            if (max < output[i][j])
            {
                max = output[i][j];
            }
        }
    }
    for (int i = 0; i < output.rows(); i++)
    {
        for (int j = 0; j < output.cols(); j++)
        {
            output[i][j] = (output[i][j] / max) * 255.0;
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
    for (int i = 1; i < input.rows() - 2; i++)
    {
        for (int j = 1; j < input.cols() - 2; j++)
        {
            double sum = 0;
            for (int u = -1; u <= 1; u++)
            {
                for (int v = -1; v <= 1; v++)
                {
                    int p = input[i][j];
                    double c = filter[u + 1][v + 1];
                    sum = sum + c * p;
                }
            }
            output[i][j] = sum;
        }
    }

    return output;
}

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

// Apply a sobel operator to an image, returns the result
//
SDoublePlane sobel_gradient_filter(const SDoublePlane &input, bool _gx)
{
    SDoublePlane output(input.rows(), input.cols());
    SDoublePlane hori(input.rows(), input.cols());
    SDoublePlane vert(input.rows(), input.cols());
    double xKernel[3][3] = {{-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};
    double yKernel[3][3] = {{-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};
    double horizontal[3][3] = {{-1, -1, 1},
        {2, 2, 2},
        {-1, -1, -1}};
    double vertical[3][3] = {{-1, 2, -1},
        {-1, 2, -1},
        {-1, 2, -1}};
    SDoublePlane xKernel_v(3, 1);
    xKernel_v[0][0] = 1;
    xKernel_v[1][0] = 2;
    xKernel_v[2][0] = 1;

    SDoublePlane xKernel_h(1, 3);
    xKernel_h[0][0] = -1;
    xKernel_h[0][1] = 0;
    xKernel_h[0][2] = 1;

    SDoublePlane yKernel_v(3, 1);
    yKernel_v[0][0] = -1;
    yKernel_v[1][0] = 0;
    yKernel_v[2][0] = 1;

    SDoublePlane yKernel_h(1, 3);
    yKernel_h[0][0] = 1;
    yKernel_h[0][1] = 2;
    yKernel_h[0][2] = 1;

    if (_gx == true)
    {
        SDoublePlane sobelX = convolve_separable(input, xKernel_v, xKernel_h);
        output = sobelX;
        SImageIO::write_png_file("sobelXX.png", sobelX, sobelX, sobelX);
    }
    else
    {
        SDoublePlane sobelY = convolve_separable(input, yKernel_v, yKernel_h);
        output = sobelY;
        SImageIO::write_png_file("sobelYY.png", sobelY, sobelY, sobelY);
    }
    // Implement a sobel gradient estimation filter with 1-d filters

    return output;
}

SDoublePlane gradientMagnitude(SDoublePlane &output_imageX, SDoublePlane &output_imageY)
{
    SDoublePlane output(output_imageX.rows(), output_imageX.cols());
    double mag = 0;
    double min = 100000000;
    double max = -1;
    for (int i = 0; i < output_imageX.rows(); i++)
    {
        for (int j = 0; j < output_imageX.cols(); j++)
        {
            mag = sqrt(((output_imageX[i][j]) * (output_imageX[i][j])) + ((output_imageY[i][j]) * (output_imageY[i][j])));
            // mag = abs((output_imageX[i][j]))+abs((output_imageX[i][j]));
            if (mag > max)
            {
                max = mag;
            }
            if(mag<min){
                min = mag;
            }
        }
    }
    for (int i = 0; i < output_imageX.rows(); i++)
    {
        for (int j = 0; j < output_imageX.cols(); j++)
        {
            mag = sqrt(((output_imageX[i][j]) * (output_imageX[i][j])) + ((output_imageY[i][j]) * (output_imageY[i][j])));
            // mag = abs((output_imageX[i][j]))+abs((output_imageX[i][j]));
            output[i][j] = ((mag-min)/(max-min))*255;
        }
    }
    SDoublePlane binary(output_imageX.rows(), output_imageX.cols());
    for (int i = 0; i < output_imageX.rows(); i++)
    {
        for (int j = 0; j < output_imageX.cols(); j++)
        {
            if (output[i][j] > 80)
            {
                binary[i][j] = 255;
            }
            else
            {
                output[i][j] = 0;
                binary[i][j] = 0;
            }
        }
    }
    return binary;
}
// Apply an edge detector to an image, returns the binary edge map
//
DetectedBox find_edges(const SDoublePlane &input,SDoublePlane templateImage, double thresh = 0)
{
    SDoublePlane output(input.rows(), input.cols());
    SDoublePlane inputMod(input.rows(), input.cols());

    // Implement an edge detector of your choice, e.g.
    // use your sobel gradient operator to compute the gradient magnitude and threshold
    double sigma = 0.5;
    SDoublePlane gKernel = filter_creation(sigma);
    SDoublePlane temp1 = templateImage;
    // cout << "7";
    //SDoublePlane output_image = convolve_general(input_image, mean_filter);
    SDoublePlane output_image = convolve_separable(input, gKernel, gKernel);
    SDoublePlane temp_convolve = convolve_separable(temp1, gKernel, gKernel);
    // cout << "8";
    SImageIO::write_png_file("gradient.png", output_image, output_image, output_image);
    SDoublePlane output_imageX = sobel_gradient_filter(output_image, true);
    SDoublePlane output_tempX = sobel_gradient_filter(temp_convolve, true);

    //SImageIO::write_png_file("sobelX.png", output_imageX, output_imageX, output_imageX);
    SDoublePlane output_imageY = sobel_gradient_filter(output_image, false);
    SDoublePlane output_tempY = sobel_gradient_filter(temp_convolve, false);
    //SImageIO::write_png_file("sobelY.png", output_imageY, output_imageY, output_imageY);
    output = gradientMagnitude(output_imageX, output_imageY);
    SDoublePlane template1 = gradientMagnitude(output_tempX, output_tempY);
    SImageIO::write_png_file("outputTemp.png", output, output, output);
    SImageIO::write_png_file("template1grad.png", template1, template1, template1);

    // SDoublePlane o = hough_transform(output);
    // output = binary;
    vector<DetectedBox> ics;
    DetectedBox s;
    SDoublePlane crossCo(input.rows(), input.cols());
    //int n = template1.rows() * template1.cols();
    int tempX = template1.rows() / 2;
    int tempY = template1.cols() / 2;
    int productSum, tempSum, imageSum, tempSqSum, imageSqSum, tempSumSq, imageSumSq;
    for (int i = tempX; i < input.rows() - tempX; i = i + 1)// chnge it to  = input.rows() - tempX
    {
        for (int j = tempY; j < input.cols() - tempY; j = j + 1)
        {
            productSum = 0, tempSum = 0, imageSum = 0, tempSqSum = 0, imageSqSum = 0, tempSumSq = 0, imageSumSq = 0;
            for (int ai = -tempX; ai < tempX; ai++)
            {
                for (int aj = -tempY; aj < tempY; aj++)
                { //input.rows() - tempX - 1 + tempX -1
                    //tempX + -tempX
                    //tempX-1 + tempX
                    productSum += output[i + ai][j + aj] * template1[ai + tempX][aj + tempY];
                    // tempSum += template1[ai + tempX][aj + tempY];
                    // imageSum += output[i + ai][j + aj];
                    tempSqSum += template1[ai + tempX][aj + tempY] * template1[ai + tempX][aj + tempY];
                    imageSqSum += output[i + ai][j + aj] * output[i + ai][j + aj];
                }
            }
            // tempSumSq = tempSum * tempSum;
            // imageSumSq = imageSum * imageSum;
            // double num = (n*productSum-tempSum*imageSum);
            // double den = sqrt((n*tempSqSum-tempSumSq)*(n*imageSqSum-imageSumSq));
            double num = productSum;
            double den = sqrt(tempSqSum*imageSqSum);
            // double cor = ((n * productSum) - (tempSum * imageSum)) / (sqrt(((n * tempSqSum * tempSqSum) - tempSumSq) * ((n * imageSqSum * imageSqSum) - imageSumSq)));
            double cor = num/den;
            crossCo[i][j] = cor;
            // if (crossCo[i][j] > 0.95&&crossCo[i][j]<1)
            // {
            //     s.row = i - template1.rows() / 2;
            //     s.col = j - template1.cols() / 2;
            //     s.width = template1.cols();
            //     s.height = template1.rows();
            //     s.confidence = rand();
            //     ics.push_back(s);
            // }
        }
    }

    double max = -1;
    int ii, jj;
    for (int i = 0; i < crossCo.rows(); i++)
    {
        for (int j = 0; j < crossCo.cols(); j++)
        {
            if (max < crossCo[i][j])
            {
                max = crossCo[i][j];
                ii = i;
                jj = j;
            }
        }
    }
    // DetectedBox s;
    // -sqrt(template1.rows()*template1.rows()+template1.cols()*template1.cols())

    s.row = ii - template1.rows() / 2;
    s.col = jj - template1.cols() / 2;
    s.width = template1.cols();
    s.height = template1.rows();
    s.confidence = rand();
    ics.push_back(s);
    cout << "corelation" << max << endl;
    cout << "coordinates: " << ii << "->" << jj << endl;
    SImageIO::write_png_file("crossCo.png", crossCo, crossCo, crossCo);
    return s;
}

SDoublePlane calcAccumulator(const SDoublePlane &input)
{
    SDoublePlane accumulator(input.rows(), input.cols());
    int m = 5, start, dx, dy, aii, ajj, n, end;
    for (int i = 0; i < input.rows(); i++)
    {
        for (int j = 0; j < input.cols(); j++)
        {
            if (input[i][j] == 255)
            {
                for (int ai = i; ai < i + (input.rows() - i) / (m - 1); ai++)
                {
                    if (ai == i)
                    {
                        start = j + 1;
                    }
                    else
                    {
                        start = j - j / (m - 1);
                    }
                    for (int aj = start; aj < j + (input.cols() - j) / (m - 1); aj++)
                    {
                        if (input[ai][aj] == 255)
                        {
                            dx = ai - i;
                            dy = aj - j;
                            if (input[i - dx][j - dy] == 0)
                            {
                                aii = ai;
                                ajj = aj;
                                n = 2;
                                end = 0;
                                while (end == 0)
                                {
                                    if (input[aii + dx][ajj + dy] == 1)
                                    {
                                        n = n + 1;
                                        aii = aii + dx;
                                        ajj = ajj + dx;
                                    }
                                    else
                                        end = 1;
                                }
                                // if (n >= m)
                                // {
                                accumulator[i][j] = accumulator[i][j] + n;
                                accumulator[aii][ajj] = accumulator[aii][ajj] + n;
                                cout << accumulator[i][j] << endl;
                                // }
                            }
                        }
                    }
                }
            }
        }
    }

    SImageIO::write_png_file("accu.png", accumulator, accumulator, accumulator);
    return accumulator;
}

SDoublePlane hough_transform(const SDoublePlane &input, const SDoublePlane &sx, const SDoublePlane &sy)
{

    SDoublePlane accumulatorX(input.rows(), input.cols());
    SDoublePlane accumulatorY(input.rows(), input.cols());
    SDoublePlane threshold_accX(input.rows(), input.cols());
    SDoublePlane threshold_accY(input.rows(), input.cols());
    SDoublePlane hori(input.rows(), input.cols());
    SDoublePlane vert(input.rows(), input.cols());
    double horizontal[3][3] = {{-1, -1, 1},
        {2, 2, 2},
        {-1, -1, -1}};
    double vertical[3][3] = {{-1, 2, -1},
        {-1, 2, -1},
        {-1, 2, -1}};
    double magX = 0.0;
    for (int i = 1; i < input.rows() - 1; i++)
    {
        for (int j = 1; j < input.cols() - 1; j++)
        {
            magX = 0.0;
            for (int a = -1; a <= 1; a++)
            {
                for (int b = -1; b <= 1; b++)
                {
                    magX += horizontal[a + 1][b + 1] * input[i + a][j + b];
                }
            }
            if (magX > 255)
            {
                magX = 255;
            }
            hori[i][j] = magX;
        }
    }

    for (int i = 1; i < input.rows() - 1; i++)
    {
        for (int j = 1; j < input.cols() - 1; j++)
        {
            magX = 0.0;
            for (int a = -1; a <= 1; a++)
            {
                for (int b = -1; b <= 1; b++)
                {
                    magX += vertical[a + 1][b + 1] * input[i + a][j + b];
                }
            }
            if (magX > 255)
            {
                magX = 255;
            }
            vert[i][j] = magX;
        }
    }
    SDoublePlane inputA(input.rows(), input.cols());
    // inputA = input;
    for (int i = 1; i < input.rows(); i++)
    {
        for (int j = 1; j < input.cols(); j++)
        {
            if (hori[i][j] > 200)
            {
                for (int k = i; k < input.rows(); k++)
                {
                    if (hori[k][j] > 200)
                    {
                        inputA[i][j]++;
                    }
                }
            }
        }
    }
    for (int i = 1; i < input.rows(); i++)
    {
        for (int j = 1; j < input.cols(); j++)
        {
            if (vert[i][j] > 200)
            {
                for (int k = j; k < input.rows(); k++)
                {
                    if (vert[i][j] > 200)
                    {
                        inputA[i][j]++;
                    }
                }
            }
        }
    }
    SImageIO::write_png_file("inputA.png", inputA, inputA, inputA);
    SImageIO::write_png_file("hori.png", hori, hori, hori);
    SImageIO::write_png_file("vert.png", vert, vert, vert);

    for (int i = 0; i < input.rows(); i++)
    {
        for (int j = 0; j < input.cols(); j++)
        {
            if (sx[i][j] > 175)
            {
                for (int k = i; k < input.rows(); k++)
                {
                    if (sx[k][j] > 150)
                    {
                        accumulatorX[i][j]++;
                    }
                }
                for (int k = j; k < input.cols(); k++)
                {
                    if (sy[i][k] > 150)
                    {
                        accumulatorY[i][j]++;
                    }
                }
            }
        }
    }

    for (int i = 0; i < input.rows(); i++)
    {
        for (int j = 0; j < input.cols(); j++)
        {
            if (sx[i][j] > 200 && accumulatorX[i][j] > 50)
            {
                for (int k = 0; k < input.cols(); k++)
                {
                    threshold_accX[i][k] = 255;
                }
            }
            if (sy[i][j] > 200 && accumulatorY[i][j] > 50)
            {
                for (int k = 0; k < input.rows(); k++)
                {
                    threshold_accY[k][j] = 255;
                }
            }
        }
    }
    SImageIO::write_png_file("accuX.png", accumulatorX, accumulatorX, accumulatorX);
    SImageIO::write_png_file("accuY.png", accumulatorY, accumulatorY, accumulatorY);
    SImageIO::write_png_file("theaccuX.png", threshold_accX, threshold_accX, threshold_accX);
    return input;
}

//
// This main file just outputs a few test images. You'll want to change it to do
//  something more interesting!
//
int main(int argc, char *argv[])
{
    if (!(argc == 2))
    {
        cerr << "usage: " << argv[0] << " input_image" << endl;
        return 1;
    }

    string input_filename(argv[1]);
    cout << argv[1] << endl;
    SDoublePlane input_image = SImageIO::read_png_file(input_filename.c_str());

    // test step 2 by applying mean filters to the input image
    SDoublePlane mean_filter(3, 3);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            mean_filter[i][j] = 1 / 9.0;
        }
    }

    // cout << "6";
    double sigma = 3;
    SDoublePlane gKernel = filter_creation(sigma);
    // cout << "7";
    //SDoublePlane output_image = convolve_general(input_image, mean_filter);
    SDoublePlane output_image = convolve_separable(input_image, gKernel, gKernel);
    // cout << "8";
    SDoublePlane output_imageX = sobel_gradient_filter(output_image, true);
    SImageIO::write_png_file("sobelX.png", output_imageX, output_imageX, output_imageX);
    SDoublePlane output_imageY = sobel_gradient_filter(output_image, false);
    SImageIO::write_png_file("sobelY.png", output_imageY, output_imageY, output_imageY);
    // SDoublePlane image_edges = find_edges(input_image, 0);
    SDoublePlane templateImage = SImageIO::read_png_file("template6.png");
    SDoublePlane templateImage1 = SImageIO::read_png_file("template1.png");

    DetectedBox s1 = find_edges(input_image,templateImage, 0);
    DetectedBox s2 = find_edges(input_image,templateImage1, 0);
    // SImageIO::write_png_file("edges.png", image_edges, image_edges, image_edges);
    // SDoublePlane out = calcAccumulator(image_edges);
    // SDoublePlane o = hough_transform(image_edges, output_imageX, output_imageY);

    //SDoublePlane output_imageXY = sobel_gradient_filter(output_imageX, false);
    //SImageIO::write_png_file("sobelXY.png", output_imageXY, output_imageXY, output_imageXY);

    // cout << "9";
    // randomly generate some detected ics -- you'll want to replace this
    //  with your ic detection code obviously!
    vector<DetectedBox> ics;
    // for (int i = 0; i < 10; i++)
    // {
    //   DetectedBox s;
    //   s.row = rand() % input_image.rows();
    //   s.col = rand() % input_image.cols();
    //   s.width = 20;
    //   s.height = 20;
    //   s.confidence = rand();
    //   ics.push_back(s);
    // }
    ics.push_back(s1);
    ics.push_back(s2);

    write_detection_txt("detected.txt", ics);
    write_detection_image("detected.png", ics, input_image);
}
