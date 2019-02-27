// B657 assignment 2 skeleton code
//
// Compile with: "make"
//
// See assignment handout for command line and project specifications.
// Team members names
// jeereddy-nawazkh-pkurusal-rpochamp-a1
// jeereddy - Jeevan Reddy
// nawazkh - Nawaz Hussain K
// pkurusal - Pruthvi Raj Kurusala
// rpochamp - Rahul Pochampally
//

//Link to the header file
#include "CImg.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Sift.h>
#include <armadillo>
#include <list>


//Use the cimg namespace to access functions easily
using namespace cimg_library;
using namespace std;
class Matches
{
public:
Matches() {
}

Matches(SiftDescriptor _descriptor1, SiftDescriptor _descriptor2, double _distance) :
        descriptor1(_descriptor1),   descriptor2(_descriptor2), distance(_distance) {
}

SiftDescriptor descriptor1, descriptor2;
double distance;
};

void draw_descriptor_image(CImg<double> image, const vector<SiftDescriptor> descriptors, const char *filename)
{
        for(unsigned int i=0; i < descriptors.size(); i++)
        {
                int tx1 = 0, ty1 = 0, tx2 = 0, ty2 = 0;
                double color_point[] = {255.0, 255.0, 0};
                for(int x=-2; x<3; x++)
                        for(int y=-2; y<3; y++)
                                if(x==0 || y==0)
                                        for(int c=0; c<3; c++) {
                                                //Find if coordinates are in workspace to draw crosshair
                                                tx1 = (descriptors[i].col + y - 1);
                                                ty1 = (descriptors[i].row + x - 1);
                                                if (tx1 >= 0 && tx1 < image.width() && ty1 >= 0 && ty1 < image.height())
                                                        image( tx1, ty1, 0, c) = color_point[c];
                                                //cout << "color_point[c]" << color_point[c] << endl;
                                        }
        }
        image.get_normalize(0,255).save(filename);
        cout << "descriptors.size()" << descriptors.size() << endl;
}
void projectiveMappingInverse(const char *fileName, const char *opFilename){
        // --------
        /* We are first softening the image to check the result.
           Using assignment 1's convolution procedure
                SImageIO::read_png_file
         */
        // SDoublePlane input_image = SImageIO::read_png_file(fileName);
        // SDoublePlane realPart, imaginaryPart;
        // fft(input_image, realPart, imaginaryPart);
        // SDoublePlane smooth_real_part = convolve_separable(realPart,)
        // softening the image using ____ kernel
        // --------


        double a[3][3] = {// projective martix
                {0.907,         0.258,          -182},
                {-0.153,        1.44,           58},
                {-0.000306,     0.000731,       1}
        };
        double a11 = 0.907,a12 = 0.258,a13 = -182, a21 = -0.153, a22 =1.44, a23 = 58, a31 = -0.000306, a32 = 0.000731, a33 = 1;

        // to calculate inverse of projective matrix, we calculate the DET(A) and Adjoint(A) first.

        //Det(A) = () + () + () - () - () - ()
        double detA = (a11*a22*a33) + (a12*a23*a31) + (a13*a21*a32) - (a11*a23*a32) - (a12*a21*a33) - (a13*a22*a31);
        //cout << "detA: " << detA << endl;
        // double AdjA[3][3] = {
        //         {(a22*a33)-(a23*a32),(a13*a32)-(a12*a33),(a12*a23)-(a13*a22)},
        //         {(a23*a31)-(a21*a33),(a11*a33)-(a13*a31),(a13*a21)-(a11*a23)},
        //         {(a21*a32)-(a22*a31),(a12*a31)-(a11*a32),(a11*a22)-(a12*a21)}
        // };
        CImg<double> inputImage(fileName),myOutputImage(inputImage);//,temp_linear(inputImage),temp_cubic(inputImage);
        // for(int i  = 0; i < inputImage.width(); i++ ) {
        //         for(int j = 0; j < inputImage.height(); j++) {
        //                 for(int c = 0; c  < 3; c++) {
        //                         temp_linear(i,j,0,c) = inputImage.linear_atXY(i,j,0,c);
        //                         temp_cubic(i,j,0,c) = inputImage.cubic_atXY(i,j,0,c);
        //                         cout << "inputImage(x,y,0,c): " << inputImage(i,j,0,c) <<  endl;
        //                         cout << "inputImage.linear_atXY(i,j,0,c): " << inputImage.linear_atXY(i,j,0,c) << endl;
        //                         cout << "inputImage.inputImage.cubic_atXY(i,j,0,c): " << inputImage.cubic_atXY(i,j,0,c) << endl;
        //
        //                 }
        //                 //break;
        //         }//break;
        // }

        int h = inputImage.height(), w = (int)inputImage.width();//1024=w, 681 = h

        //cout << "working" << endl;
        double x;//[inputImage.height()][inputImage.width()];
        //cout << "working" << endl;
        double y;//[inputImage.height()][inputImage.width()];
        //cout << "working" << endl;
        for(int i  = 0; i < inputImage.width(); i++ ) {
                for(int j = 0; j < inputImage.height(); j++) {
                        x = (   (((a22*a33)-(a23*a32)) * i ) +  ((  (a13*a32)-(a12*a33)  ) * j) + ((  (a12*a23)-(a13*a22)  ) * 1));// (   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                        y = (  (  (    (a23*a31)-(a21*a33)    )*i  ) + (  (   (a11*a33)-(a13*a31)   )*j  ) + (  (   (a13*a21)-(a11*a23)   )*1  )   );//(   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                        int modX = (x/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                        int modY = (y/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                        // y = y/detA;
                        //cout << "(i,j):( "<<i<<","<<j<<") : (" << x << "," << y << ")" << endl;
                        if (modY >= 0 && modY < inputImage.height() && modX >= 0 && modX < inputImage.width()) {
                                for(int c = 0; c  < 3; c++) {
                                        myOutputImage(i,j,0,c) = inputImage(modX,modY,0,c);
                                }

                        }
                        else{
                                for(int c = 0; c  < 3; c++) {
                                        myOutputImage(i,j,0,c) = 0;
                                }
                        }

                }
        }
        myOutputImage.get_normalize(0,255).save(opFilename);
}


void projectiveWarppingInversepart2(const char *book1filename, const char *book2filename, const char *opFilename){
        CImg<double> book1(book1filename),book2(book2filename), myOutputImage(book1);
        // arma::mat matA(2,2);
        // matA << 3 << -2 << arma::endr << 6 << 4 << arma::endr;
        // arma::mat matB(2,1);
        // arma::mat matC(2,2);
        // matB << 5 << arma::endr << 10 << arma::endr;
        // matC = arma::solve(matA,matB);
        // matC.print("matC:");
        // matB.print("matB:");
        // matA.print("matA:");
        int x1_dash = 318,y1_dash = 256;//final points
        int x2_dash = 534,y2_dash = 372;
        int x3_dash = 316,y3_dash = 670;
        int x4_dash = 73,y4_dash = 473;

        int x1 = 141, y1 = 131;//initial points
        int x2 = 480, y2 = 159;
        int x3 = 493, y3 = 630;
        int x4 = 64, y4 = 601;

        arma::mat matB(8,1);
        arma::mat matA(8,8);
        arma::mat matC(8,1);
        matB << x1_dash << arma::endr
             << y1_dash << arma::endr
             << x2_dash << arma::endr
             << y2_dash << arma::endr
             << x3_dash << arma::endr
             << y3_dash << arma::endr
             << x4_dash << arma::endr
             << y4_dash << arma::endr;
        //matB.print("matrixB is:");
        matA << x1 << y1 << 1 << 0 << 0 << 0 << (-1 * x1 * x1_dash) << (-1 * y1 * x1_dash) << arma::endr
             << 0 << 0 << 0 << x1 << y1 << 1 << (-1 * x1 * y1_dash) << (-1 * y1 * y1_dash) << arma::endr
             << x2 << y2 << 1 << 0 << 0 << 0 << (-1 * x2 * x2_dash) << (-1 * y2 * x2_dash) << arma::endr
             << 0 << 0 << 0 << x2 << y2 << 1 << (-1 * x2 * y2_dash) << (-1 * y2 * y2_dash) << arma::endr
             << x3 << y3 << 1 << 0 << 0 << 0 << (-1 * x3 * x3_dash) << (-1 * y3 * x3_dash) << arma::endr
             << 0 << 0 << 0 << x3 << y3 << 1 << (-1 * x3 * y3_dash) << (-1 * y3 * y3_dash) << arma::endr
             << x4 << y4 << 1 << 0 << 0 << 0 << (-1 * x4 * x4_dash) << (-1 * y4 * x4_dash) << arma::endr
             << 0 << 0 << 0 << x4 << y4 << 1 << (-1 * x4 * y4_dash) << (-1 * y4 * y4_dash) << arma::endr;
        //matA.print("matrixA is:");
        matC = arma::solve(matA,matB);
        matC.print("Transformation Matrix is:");
        double a11 = matC(0,0),a12 = matC(1,0),a13 = matC(2,0), a21 = matC(3,0), a22 =matC(4,0), a23 = matC(5,0), a31 = matC(6,0), a32 = matC(7,0), a33 = 1;
        double detA = (a11*a22*a33) + (a12*a23*a31) + (a13*a21*a32) - (a11*a23*a32) - (a12*a21*a33) - (a13*a22*a31);
        //cout << "detA: " << detA << endl;
        double x,y;

        for(int i  = 0; i < book2.width(); i++ ) {
                for(int j = 0; j < book2.height(); j++) {
                        x = (   (((a22*a33)-(a23*a32)) * i ) +  ((  (a13*a32)-(a12*a33)  ) * j) + ((  (a12*a23)-(a13*a22)  ) * 1));// (   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                        y = (  (  (    (a23*a31)-(a21*a33)    )*i  ) + (  (   (a11*a33)-(a13*a31)   )*j  ) + (  (   (a13*a21)-(a11*a23)   )*1  )   );//(   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                        int modX = floor(x/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                        int modY = floor(y/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                        // y = y/detA;
                        //cout << "(i,j):( "<<i<<","<<j<<") : (" << x << "," << y << ")" << endl;
                        if (modY >= 0 && modY < book2.height() && modX >= 0 && modX < book2.width()) {
                                for(int c = 0; c  < 3; c++) {
                                        myOutputImage(i,j,0,c) = book2(modX,modY,0,c);
                                }

                        }
                        else{
                                for(int c = 0; c  < 3; c++) {
                                        myOutputImage(i,j,0,c) = 0;
                                }
                        }

                }
        }
        myOutputImage.get_normalize(0,255).save(opFilename);
}

void billboardWrappingInverse(const char *input_filename, const char *billboard1,const char *billboard2,const char *billboard3){
        CImg<double>inputImage(input_filename),bill1(billboard1),bill2(billboard2),bill3(billboard3),myOutputImage(bill1),myOutputImage2(bill2),myOutputImage3(bill3);
        //initial points
        //cout << "inputImage.width()" << inputImage.width() << endl;
        //cout << "inputImage.height()" << inputImage.height() << endl;
        int x1 = 0, y1 = 0;//initial points
        int x2 = 0, y2 = inputImage.height();
        int x3 = inputImage.width(), y3 = 0;
        int x4 = inputImage.width(), y4 = inputImage.height();

        int x1_dash = 102,y1_dash = 60;//final points bill board 1
        int x2_dash = 102,y2_dash = 205;
        int x3_dash = 532,y3_dash = 60;
        int x4_dash = 532,y4_dash = 205;

        arma::mat matB(8,1);
        arma::mat matA(8,8);
        arma::mat matC(8,1);
        matB << x1_dash << arma::endr
             << y1_dash << arma::endr
             << x2_dash << arma::endr
             << y2_dash << arma::endr
             << x3_dash << arma::endr
             << y3_dash << arma::endr
             << x4_dash << arma::endr
             << y4_dash << arma::endr;
        //matB.print("matrixB is:");
        matA << x1 << y1 << 1 << 0 << 0 << 0 << (-1 * x1 * x1_dash) << (-1 * y1 * x1_dash) << arma::endr
             << 0 << 0 << 0 << x1 << y1 << 1 << (-1 * x1 * y1_dash) << (-1 * y1 * y1_dash) << arma::endr
             << x2 << y2 << 1 << 0 << 0 << 0 << (-1 * x2 * x2_dash) << (-1 * y2 * x2_dash) << arma::endr
             << 0 << 0 << 0 << x2 << y2 << 1 << (-1 * x2 * y2_dash) << (-1 * y2 * y2_dash) << arma::endr
             << x3 << y3 << 1 << 0 << 0 << 0 << (-1 * x3 * x3_dash) << (-1 * y3 * x3_dash) << arma::endr
             << 0 << 0 << 0 << x3 << y3 << 1 << (-1 * x3 * y3_dash) << (-1 * y3 * y3_dash) << arma::endr
             << x4 << y4 << 1 << 0 << 0 << 0 << (-1 * x4 * x4_dash) << (-1 * y4 * x4_dash) << arma::endr
             << 0 << 0 << 0 << x4 << y4 << 1 << (-1 * x4 * y4_dash) << (-1 * y4 * y4_dash) << arma::endr;

        matC = arma::solve(matA,matB);
        //matC.print("Solution Matrix is:");
        double a11 = matC(0,0),a12 = matC(1,0),a13 = matC(2,0), a21 = matC(3,0), a22 =matC(4,0), a23 = matC(5,0), a31 = matC(6,0), a32 = matC(7,0), a33 = 1;
        double detA = (a11*a22*a33) + (a12*a23*a31) + (a13*a21*a32) - (a11*a23*a32) - (a12*a21*a33) - (a13*a22*a31);
        //cout << "detA: " << detA << endl;
        double x,y;

        for(int i  = 0; i < inputImage.width(); i++ ) {
                for(int j = 0; j < inputImage.height(); j++) {
                        x = (   (((a22*a33)-(a23*a32)) * i ) +  ((  (a13*a32)-(a12*a33)  ) * j) + ((  (a12*a23)-(a13*a22)  ) * 1));// (   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                        y = (  (  (    (a23*a31)-(a21*a33)    )*i  ) + (  (   (a11*a33)-(a13*a31)   )*j  ) + (  (   (a13*a21)-(a11*a23)   )*1  )   );//(   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                        int modX = floor(x/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                        int modY = floor(y/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                        // y = y/detA;
                        //cout << "(i,j):( "<<i<<","<<j<<") : (" << x << "," << y << ")" << endl;
                        if (modY >= 0 && modY < inputImage.height() && modX >= 0 && modX < inputImage.width() && i < bill1.width() && j < bill1.height()) {
                                for(int c = 0; c  < 3; c++) {
                                        myOutputImage(i,j,0,c) = inputImage(modX,modY,0,c);
                                }

                        }
                        // else if(i < bill2.width() && j < bill2.height()){
                        //         for(int c = 0; c  < 3; c++) {
                        //                 myOutputImage(i,j,0,c) = null;
                        //         }
                        // }

                }
        }
        string opFilename = "synthetic_billboard1.png";
        //myOutputImage+=bill2;
        myOutputImage.get_normalize(0,255).save(opFilename.c_str());

        x1_dash = 176;
        y1_dash = 53;//final points bill board 1
        x2_dash = 148;
        y2_dash = 623;
        x3_dash = 1107;
        y3_dash = 261;
        x4_dash = 1125;
        y4_dash = 702;

        arma::mat matB2(8,1);
        arma::mat matA2(8,8);
        arma::mat matC2(8,1);
        matB2 << x1_dash << arma::endr
              << y1_dash << arma::endr
              << x2_dash << arma::endr
              << y2_dash << arma::endr
              << x3_dash << arma::endr
              << y3_dash << arma::endr
              << x4_dash << arma::endr
              << y4_dash << arma::endr;
        //matB2.print("matrixB is:");
        matA2 << x1 << y1 << 1 << 0 << 0 << 0 << (-1 * x1 * x1_dash) << (-1 * y1 * x1_dash) << arma::endr
              << 0 << 0 << 0 << x1 << y1 << 1 << (-1 * x1 * y1_dash) << (-1 * y1 * y1_dash) << arma::endr
              << x2 << y2 << 1 << 0 << 0 << 0 << (-1 * x2 * x2_dash) << (-1 * y2 * x2_dash) << arma::endr
              << 0 << 0 << 0 << x2 << y2 << 1 << (-1 * x2 * y2_dash) << (-1 * y2 * y2_dash) << arma::endr
              << x3 << y3 << 1 << 0 << 0 << 0 << (-1 * x3 * x3_dash) << (-1 * y3 * x3_dash) << arma::endr
              << 0 << 0 << 0 << x3 << y3 << 1 << (-1 * x3 * y3_dash) << (-1 * y3 * y3_dash) << arma::endr
              << x4 << y4 << 1 << 0 << 0 << 0 << (-1 * x4 * x4_dash) << (-1 * y4 * x4_dash) << arma::endr
              << 0 << 0 << 0 << x4 << y4 << 1 << (-1 * x4 * y4_dash) << (-1 * y4 * y4_dash) << arma::endr;

        matC2 = arma::solve(matA2,matB2);
        //matC2.print("Solution Matrix is:");
        a11 = matC2(0,0);
        a12 = matC2(1,0);
        a13 = matC2(2,0);
        a21 = matC2(3,0);
        a22 = matC2(4,0);
        a23 = matC2(5,0);
        a31 = matC2(6,0);
        a32 = matC2(7,0);
        a33 = 1;
        detA = (a11*a22*a33) + (a12*a23*a31) + (a13*a21*a32) - (a11*a23*a32) - (a12*a21*a33) - (a13*a22*a31);
        //cout << "detA: " << detA << endl;
        // double x,y;

        for(int i  = 0; i < inputImage.width(); i++ ) {
                for(int j = 0; j < inputImage.height(); j++) {
                        x = (   (((a22*a33)-(a23*a32)) * i ) +  ((  (a13*a32)-(a12*a33)  ) * j) + ((  (a12*a23)-(a13*a22)  ) * 1));// (   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                        y = (  (  (    (a23*a31)-(a21*a33)    )*i  ) + (  (   (a11*a33)-(a13*a31)   )*j  ) + (  (   (a13*a21)-(a11*a23)   )*1  )   );//(   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                        int modX = floor(x/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                        int modY = floor(y/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                        // y = y/detA;
                        //cout << "(i,j):( "<<i<<","<<j<<") : (" << x << "," << y << ")" << endl;
                        if (modY >= 0 && modY < inputImage.height() && modX >= 0 && modX < inputImage.width() && i < bill2.width() && j < bill2.height()) {
                                for(int c = 0; c  < 3; c++) {
                                        myOutputImage2(i,j,0,c) = inputImage(modX,modY,0,c);
                                }

                        }
                        // else if(i < bill1.width() && j < bill1.height()){
                        //         for(int c = 0; c  < 3; c++) {
                        //                 myOutputImage(i,j,0,c) = null;
                        //         }
                        // }

                }
        }

        x1_dash = 984;
        y1_dash = 722;//final points bill board 1
        x2_dash = 986;
        y2_dash = 773;
        x3_dash = 1143;
        y3_dash = 733;
        x4_dash = 1146;
        y4_dash = 781;

        arma::mat matB22(8,1);
        arma::mat matA22(8,8);
        arma::mat matC22(8,1);
        matB22 << x1_dash << arma::endr
               << y1_dash << arma::endr
               << x2_dash << arma::endr
               << y2_dash << arma::endr
               << x3_dash << arma::endr
               << y3_dash << arma::endr
               << x4_dash << arma::endr
               << y4_dash << arma::endr;
        //matB2.print("matrixB is:");
        matA22 << x1 << y1 << 1 << 0 << 0 << 0 << (-1 * x1 * x1_dash) << (-1 * y1 * x1_dash) << arma::endr
               << 0 << 0 << 0 << x1 << y1 << 1 << (-1 * x1 * y1_dash) << (-1 * y1 * y1_dash) << arma::endr
               << x2 << y2 << 1 << 0 << 0 << 0 << (-1 * x2 * x2_dash) << (-1 * y2 * x2_dash) << arma::endr
               << 0 << 0 << 0 << x2 << y2 << 1 << (-1 * x2 * y2_dash) << (-1 * y2 * y2_dash) << arma::endr
               << x3 << y3 << 1 << 0 << 0 << 0 << (-1 * x3 * x3_dash) << (-1 * y3 * x3_dash) << arma::endr
               << 0 << 0 << 0 << x3 << y3 << 1 << (-1 * x3 * y3_dash) << (-1 * y3 * y3_dash) << arma::endr
               << x4 << y4 << 1 << 0 << 0 << 0 << (-1 * x4 * x4_dash) << (-1 * y4 * x4_dash) << arma::endr
               << 0 << 0 << 0 << x4 << y4 << 1 << (-1 * x4 * y4_dash) << (-1 * y4 * y4_dash) << arma::endr;

        matC22 = arma::solve(matA22,matB22);
        //matC2.print("Solution Matrix is:");
        a11 = matC22(0,0);
        a12 = matC22(1,0);
        a13 = matC22(2,0);
        a21 = matC22(3,0);
        a22 = matC22(4,0);
        a23 = matC22(5,0);
        a31 = matC22(6,0);
        a32 = matC22(7,0);
        a33 = 1;
        detA = (a11*a22*a33) + (a12*a23*a31) + (a13*a21*a32) - (a11*a23*a32) - (a12*a21*a33) - (a13*a22*a31);
        //cout << "detA: " << detA << endl;
        // double x,y;

        for(int i  = 0; i < inputImage.width(); i++ ) {
                for(int j = 0; j < inputImage.height(); j++) {
                        x = (   (((a22*a33)-(a23*a32)) * i ) +  ((  (a13*a32)-(a12*a33)  ) * j) + ((  (a12*a23)-(a13*a22)  ) * 1));// (   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                        y = (  (  (    (a23*a31)-(a21*a33)    )*i  ) + (  (   (a11*a33)-(a13*a31)   )*j  ) + (  (   (a13*a21)-(a11*a23)   )*1  )   );//(   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                        int modX = floor(x/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                        int modY = floor(y/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                        // y = y/detA;
                        //cout << "(i,j):( "<<i<<","<<j<<") : (" << x << "," << y << ")" << endl;
                        if (modY >= 0 && modY < inputImage.height() && modX >= 0 && modX < inputImage.width() && i < bill2.width() && j < bill2.height()) {
                                for(int c = 0; c  < 3; c++) {
                                        myOutputImage2(i,j,0,c) = inputImage(modX,modY,0,c);
                                }

                        }
                        // else if(i < bill1.width() && j < bill1.height()){
                        //         for(int c = 0; c  < 3; c++) {
                        //                 myOutputImage(i,j,0,c) = null;
                        //         }
                        // }

                }
        }




        string opFilename2 = "synthetic_billboard2.png";
        //myOutputImage+=bill1;
        myOutputImage2.get_normalize(0,255).save(opFilename2.c_str());


        x1_dash = 615;
        y1_dash = 287;//final points bill board 1
        x2_dash = 609;
        y2_dash = 608;
        x3_dash = 1259;
        y3_dash = 261;
        x4_dash = 1262;
        y4_dash = 605;

        arma::mat matB3(8,1);
        arma::mat matA3(8,8);
        arma::mat matC3(8,1);
        matB3 << x1_dash << arma::endr
              << y1_dash << arma::endr
              << x2_dash << arma::endr
              << y2_dash << arma::endr
              << x3_dash << arma::endr
              << y3_dash << arma::endr
              << x4_dash << arma::endr
              << y4_dash << arma::endr;
        //matB2.print("matrixB is:");
        matA3 << x1 << y1 << 1 << 0 << 0 << 0 << (-1 * x1 * x1_dash) << (-1 * y1 * x1_dash) << arma::endr
              << 0 << 0 << 0 << x1 << y1 << 1 << (-1 * x1 * y1_dash) << (-1 * y1 * y1_dash) << arma::endr
              << x2 << y2 << 1 << 0 << 0 << 0 << (-1 * x2 * x2_dash) << (-1 * y2 * x2_dash) << arma::endr
              << 0 << 0 << 0 << x2 << y2 << 1 << (-1 * x2 * y2_dash) << (-1 * y2 * y2_dash) << arma::endr
              << x3 << y3 << 1 << 0 << 0 << 0 << (-1 * x3 * x3_dash) << (-1 * y3 * x3_dash) << arma::endr
              << 0 << 0 << 0 << x3 << y3 << 1 << (-1 * x3 * y3_dash) << (-1 * y3 * y3_dash) << arma::endr
              << x4 << y4 << 1 << 0 << 0 << 0 << (-1 * x4 * x4_dash) << (-1 * y4 * x4_dash) << arma::endr
              << 0 << 0 << 0 << x4 << y4 << 1 << (-1 * x4 * y4_dash) << (-1 * y4 * y4_dash) << arma::endr;

        matC3 = arma::solve(matA3,matB3);
        //matC2.print("Solution Matrix is:");
        a11 = matC3(0,0);
        a12 = matC3(1,0);
        a13 = matC3(2,0);
        a21 = matC3(3,0);
        a22 = matC3(4,0);
        a23 = matC3(5,0);
        a31 = matC3(6,0);
        a32 = matC3(7,0);
        a33 = 1;
        detA = (a11*a22*a33) + (a12*a23*a31) + (a13*a21*a32) - (a11*a23*a32) - (a12*a21*a33) - (a13*a22*a31);
        //cout << "detA: " << detA << endl;
        // double x,y;

        for(int i  = 0; i < inputImage.width(); i++ ) {
                for(int j = 0; j < inputImage.height(); j++) {
                        x = (   (((a22*a33)-(a23*a32)) * i ) +  ((  (a13*a32)-(a12*a33)  ) * j) + ((  (a12*a23)-(a13*a22)  ) * 1));// (   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                        y = (  (  (    (a23*a31)-(a21*a33)    )*i  ) + (  (   (a11*a33)-(a13*a31)   )*j  ) + (  (   (a13*a21)-(a11*a23)   )*1  )   );//(   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                        int modX = floor(x/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                        int modY = floor(y/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                        // y = y/detA;
                        //cout << "(i,j):( "<<i<<","<<j<<") : (" << x << "," << y << ")" << endl;
                        if (modY >= 0 && modY < inputImage.height() && modX >= 0 && modX < inputImage.width() && i < bill3.width() && j < bill3.height()) {
                                for(int c = 0; c  < 3; c++) {
                                        myOutputImage3(i,j,0,c) = inputImage(modX,modY,0,c);
                                }

                        }
                        // else if(i < bill1.width() && j < bill3.height()){
                        //         for(int c = 0; c  < 3; c++) {
                        //                 myOutputImage3(i,j,0,c) = null;
                        //         }
                        // }

                }
        }
        string opFilename3 = "synthetic_billboard3.png";
        //myOutputImage+=bill1;
        myOutputImage3.get_normalize(0,255).save(opFilename3.c_str());




}

double part3_calculate_distance(const SiftDescriptor descriptors1, const SiftDescriptor descriptors2){
        // returns euclidian distance between descriptors1, descriptors2
        // cout << "input_descriptors1.row()" << descriptors1.row << endl;
        // cout << "input_descriptors1.col()" << descriptors1.col << endl;
        // cout << "input_descriptors1.sigma()" << descriptors1.sigma << endl;
        // cout << "input_descriptors1.angle()" << descriptors1.angle << endl;
        // cout << "::::input_descriptors1.descriptors::::" << endl;
        int desc_size = descriptors1.descriptor.size();
        double sigma = 0;
        for(int i = 0; i < desc_size; i++) {
                // cout << descriptors1.descriptor[j] << endl;
                sigma = sigma + pow((descriptors1.descriptor[i] - descriptors2.descriptor[i]), 2.0);
        }
        return sqrt(sigma);
}

double part3_calculate_distance_between_points(double x_1, double y_1, double x_1_dash, double y_1_dash){
        // returns euclidian distance between two cartisian points.
        return sqrt((pow((x_1 - x_1_dash),2.0)) + (pow((y_1 - y_1_dash),2.0)));
}

void showlist(vector <Matches> g)
{
        for(unsigned int i = 0; i < g.size(); i++) {
                cout << g[i].descriptor1.row << ": \t: ";
                cout << g[i].descriptor1.col << ": \t: ";
                cout << g[i].descriptor2.row << ": \t: ";
                cout << g[i].descriptor2.col<< ": \t: ";
                cout << g[i].distance << endl;
                //break;
        }
}
vector<Matches> sortMyDistances(vector <Matches> g){
        int x = g.size();
        Matches lan;
        for (int i = 0; i < x; i++) {

                for (int j =i+1; j < x; j++) {
                        if (g[i].distance > g[j].distance) {
                                lan=g[i];
                                g[i]=g[j];
                                g[j]=lan;
                        }

                }

        }
        return g;

}

vector<Matches> perform_transformation_and_Sift_matching_RANSAC(vector<unsigned int> myIndexList,vector<Matches> mySortedMatches,CImg<double> image1,CImg<double> image2,double ro_ratio,double match_threshold){
        CImg<double> myOutputImage(image2);

        int x1 = mySortedMatches[myIndexList[0]].descriptor1.col;
        int y1 = mySortedMatches[myIndexList[0]].descriptor1.row;
        int x1_dash = mySortedMatches[myIndexList[0]].descriptor2.col;
        int y1_dash = mySortedMatches[myIndexList[0]].descriptor2.row;

        int x2 = mySortedMatches[myIndexList[1]].descriptor1.col;
        int y2 = mySortedMatches[myIndexList[1]].descriptor1.row;
        int x2_dash = mySortedMatches[myIndexList[1]].descriptor2.col;
        int y2_dash = mySortedMatches[myIndexList[1]].descriptor2.row;

        int x3 = mySortedMatches[myIndexList[2]].descriptor1.col;
        int y3 = mySortedMatches[myIndexList[2]].descriptor1.row;
        int x3_dash = mySortedMatches[myIndexList[2]].descriptor2.col;
        int y3_dash = mySortedMatches[myIndexList[2]].descriptor2.row;

        int x4 = mySortedMatches[myIndexList[3]].descriptor1.col;
        int y4 = mySortedMatches[myIndexList[3]].descriptor1.row;
        int x4_dash = mySortedMatches[myIndexList[3]].descriptor2.col;
        int y4_dash = mySortedMatches[myIndexList[3]].descriptor2.row;

        arma::mat matB(8,1);
        arma::mat matA(8,8);
        arma::mat matC(8,1);
        matB << x1_dash << arma::endr
             << y1_dash << arma::endr
             << x2_dash << arma::endr
             << y2_dash << arma::endr
             << x3_dash << arma::endr
             << y3_dash << arma::endr
             << x4_dash << arma::endr
             << y4_dash << arma::endr;
        //matB.print("matrixB is:");
        matA << x1 << y1 << 1 << 0 << 0 << 0 << (-1 * x1 * x1_dash) << (-1 * y1 * x1_dash) << arma::endr
             << 0 << 0 << 0 << x1 << y1 << 1 << (-1 * x1 * y1_dash) << (-1 * y1 * y1_dash) << arma::endr
             << x2 << y2 << 1 << 0 << 0 << 0 << (-1 * x2 * x2_dash) << (-1 * y2 * x2_dash) << arma::endr
             << 0 << 0 << 0 << x2 << y2 << 1 << (-1 * x2 * y2_dash) << (-1 * y2 * y2_dash) << arma::endr
             << x3 << y3 << 1 << 0 << 0 << 0 << (-1 * x3 * x3_dash) << (-1 * y3 * x3_dash) << arma::endr
             << 0 << 0 << 0 << x3 << y3 << 1 << (-1 * x3 * y3_dash) << (-1 * y3 * y3_dash) << arma::endr
             << x4 << y4 << 1 << 0 << 0 << 0 << (-1 * x4 * x4_dash) << (-1 * y4 * x4_dash) << arma::endr
             << 0 << 0 << 0 << x4 << y4 << 1 << (-1 * x4 * y4_dash) << (-1 * y4 * y4_dash) << arma::endr;
        //matA.print("matrixA is:");
        vector<Matches> myNewMatchesVector;
        if(det(matA) <= 0) {
                return myNewMatchesVector;
        }
        matC = arma::solve(matA,matB);
        //matC.print("Transformation Matrix is:");
        double a11 = matC(0,0),a12 = matC(1,0),a13 = matC(2,0), a21 = matC(3,0), a22 =matC(4,0), a23 = matC(5,0), a31 = matC(6,0), a32 = matC(7,0), a33 = 1;
        double detA = (a11*a22*a33) + (a12*a23*a31) + (a13*a21*a32) - (a11*a23*a32) - (a12*a21*a33) - (a13*a22*a31);
        //cout << "detA: " << detA << endl;
        double x,y;

        double newX, newY;
        double disdis = 0;
        for(unsigned int i = 0; i < mySortedMatches.size(); i++) {

                x = (   (((a22*a33)-(a23*a32)) * mySortedMatches[i].descriptor2.col ) +  ((  (a13*a32)-(a12*a33)  ) * mySortedMatches[i].descriptor2.row) + ((  (a12*a23)-(a13*a22)  ) * 1));
                y = (  (  (    (a23*a31)-(a21*a33)    )* mySortedMatches[i].descriptor2.col  ) + (  (   (a11*a33)-(a13*a31)   )* mySortedMatches[i].descriptor2.row ) + (  (   (a13*a21)-(a11*a23)   )*1  )   );        //(   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                newX = (x/((   (((a21*a32)-(a22*a31) )* mySortedMatches[i].descriptor2.col )  + (  (  (a12*a31)-(a11*a32)   )  * mySortedMatches[i].descriptor2.row  ) + (  (a11*a22)-(a12*a21)  ) )));
                newY = (y/((   (((a21*a32)-(a22*a31) )* mySortedMatches[i].descriptor2.col )  + (  (  (a12*a31)-(a11*a32)   )  * mySortedMatches[i].descriptor2.row  ) + (  (a11*a22)-(a12*a21)  ) )));
                // calcluate the euclidian distance between two points.
                disdis = part3_calculate_distance_between_points(newX,newY,mySortedMatches[i].descriptor1.col,mySortedMatches[i].descriptor1.row);
                //cout << "disdis" << disdis << endl;
                if(disdis >= 0 && disdis <= match_threshold) {
                        Matches temp(mySortedMatches[i].descriptor1,mySortedMatches[i].descriptor2,mySortedMatches[i].distance);
                        myNewMatchesVector.push_back(temp);
                }
        }
        return myNewMatchesVector;
}
vector<Matches> compare_descriptor_image(CImg<double> image1,CImg<double> image2,double ro_ratio,const char* opfilename){
        CImg<double> input_gray1 = image1.get_RGBtoHSI().get_channel(2);
        CImg<double> input_gray2 = image2.get_RGBtoHSI().get_channel(2);
        vector<SiftDescriptor> input_descriptors1 = Sift::compute_sift(input_gray1);
        vector<SiftDescriptor> input_descriptors2 = Sift::compute_sift(input_gray2);
        vector<Matches> matches;
        // double ro_ratio = 0.65;
        for(unsigned int i = 0; i < input_descriptors1.size(); i++) {
                SiftDescriptor s_one,s_two;
                double dis_1 = std::numeric_limits<int>::max();
                double dis_2 = std::numeric_limits<int>::max();
                //cout << "max dis1:" << dis_1 << endl;
                //cout << "max dis2:" << dis_2 << endl;
                for(unsigned int j = 0; j < input_descriptors2.size(); j++) {
                        double dis = part3_calculate_distance(input_descriptors1[i], input_descriptors2[j]);
                        //cout << "euclidian distance: " << dis << endl;
                        if(dis < dis_1) {
                                s_two = s_one;
                                dis_2 = dis_1;
                                s_one = input_descriptors2[j];
                                dis_1 = dis;
                                //cout << "here1, least dis:" << dis_1 << endl;
                        }
                        else if(dis < dis_2) {
                                s_two = input_descriptors2[j];
                                dis_2 = dis;
                                //cout << "here2, least dis:" << dis_2 << endl;
                        }

                }
                //cout << dis_1 << endl;
                //cout << dis_2 << endl;
                double iratio = dis_1/dis_2;
                //cout << "iratio" << iratio << endl;
                if(s_two.row != 0 && iratio <= ro_ratio) {// s_two not null is not checked here
                        //matches[i] = {{input_descriptors1[i],s_one},dis_1};

                        Matches temp(input_descriptors1[i],s_one,dis_1);
                        matches.push_back(temp);
                        //cout << dis_1 << endl;
                        //showlist(temp);
                }
                //break;
        }
        //showlist(matches);
        vector<Matches> sortedMatches = sortMyDistances(matches);
        //showlist(sortedMatches);
        int base_width = image1.width();
        image1.append(image2,'x',0);
        //image1.get_normalize(0,255).save(opfilename);
        // --- testing descriptors
        const unsigned char color[] = { 255,128,64 }; //{255.0, 255.0, 0};
        for(unsigned int i = 0; i < sortedMatches.size(); i++) {
                int x0 = sortedMatches[i].descriptor1.col;
                int y0 = sortedMatches[i].descriptor1.row;
                int x1 = (sortedMatches[i].descriptor2.col+base_width);
                int y1 = sortedMatches[i].descriptor2.row;
                image1.draw_line(x0,y0,x1,y1,color);
                //if(i > 4) break;
        }
        if(opfilename != NULL) {
                //cout << "in not null" << endl;
                image1.get_normalize(0,255).save(opfilename);
        }


        return sortedMatches;
}

vector<Matches> run_RANSAC(CImg<double> image1,CImg<double> image2,double match_threshold,double ro_ratio,const char* opfilename,vector<Matches> mySortedMatches){
        // need to write a function, which takes in my 4 points,
        // performs projective transformation on image_1 to produce new image_1x1_dash
        // generates a sift descriptors between them again
        // and returns me a count of SIFT descriptor pairs having distance value less than a threshold.
        // also returns me the four pairs of randomly selected SIFT descriptor points.
        // this I store it in a vector and sort it w.r.t to the count and take the last one as my final set of points which give me the best Matches

        // generating 4 random points using the size of the sortedMatches

        unsigned int selectedSize = 0;
        vector<Matches> finalDescriptors;
        int T = 200; // T has been derived from the formula T=log(1-p)/log(1-(1-e)^s)
                     // p = 0.99 ----> accuracy percentage
                     // e = 0.6 -----> assuming 60 percent outliers
                     // s = 4 -----> no. of samples
                     // gives T = 178 , we rounded off to 200
        for(int kk = 0; kk < T; kk++) {
                //cout << "-------------------" << endl;
                //cout << "kk :: " << kk << endl;
                vector<Matches> myMatchesAndCount;
                vector<unsigned int> myIndexList;
                int counter = 0;
                int index = 0;
                for (unsigned int i = 0; i < mySortedMatches.size(); i++) { // this loop selects 4 randomly generated indices which are distinct.
                        index = rand() % mySortedMatches.size();
                        // myIndexList.push_back(index);
                        // counter++;
                        if( i == 0) {
                                myIndexList.push_back(index);
                                counter++;
                        }
                        else{
                                bool found = (std::find(myIndexList.begin(), myIndexList.end(), index) != myIndexList.end());
                                if(found) {
                                        // do nothing, do not add a duplicate index.
                                }
                                else
                                {
                                        myIndexList.push_back(index);
                                        counter++;
                                }

                        }
                        if(counter == 4) {
                                break;
                        }

                }
                // 8 points generated uptill here.
                // need a function to accept those 8 points and perform a transformation on the image 1 to produce image 2.
                myMatchesAndCount = perform_transformation_and_Sift_matching_RANSAC(myIndexList,mySortedMatches,image1,image2,ro_ratio,match_threshold);
                if(myMatchesAndCount.size() > selectedSize) {
                        selectedSize = myMatchesAndCount.size();
                        finalDescriptors = myMatchesAndCount;
                        //cout << "-----------myMatchesAndCount.size()" << myMatchesAndCount.size() << endl;
                }

        }
        vector<Matches> sortedMatches = sortMyDistances(finalDescriptors);
        //showlist(sortedMatches);
        int base_width = image1.width();
        image1.append(image2,'x',0);
        //image1.get_normalize(0,255).save(opfilename);
        // --- testing descriptors
        const unsigned char color[] = { 255,128,64 }; //{255.0, 255.0, 0};
        for(unsigned int i = 0; i < sortedMatches.size(); i++) {
                int x0 = sortedMatches[i].descriptor1.col;
                int y0 = sortedMatches[i].descriptor1.row;
                int x1 = (sortedMatches[i].descriptor2.col+base_width);
                int y1 = sortedMatches[i].descriptor2.row;
                image1.draw_line(x0,y0,x1,y1,color);
                //if(i > 4) break;
        }
        if(opfilename != NULL) {
                //cout << "in not null" << endl;
                image1.get_normalize(0,255).save(opfilename);
        }
        return sortedMatches;

}

CImg<double> generate_homographies_and_transform(vector<unsigned int> myIndexList, vector<Matches> mySortedMatches,CImg<double> image1,CImg<double> image2,string side){

        //img4.append(img4,'x',0);
        //img4.append(img4,'x',0);
        //image1.append(img4,'-x',0);
        //image1.append(img4,'x',0);

        if(side == "left") {
                CImg<double> img4(image1.width(),image1.height(),1,3,0),img5(image1.width(),image1.height(),1,3,0);
                //img5.append(img4,'y',0);
                img5.append(image1,'x',0);

                // string testFile = "transformationTest.png";
                // img5.get_normalize(0,255).save(testFile.c_str());
                CImg<double> myOutputImage(2*image1.width(),image1.height(),1,3,0);
                // myOutputImage = img4;
                arma::mat matB(8,1);
                arma::mat matA(8,8);
                arma::mat matC(8,1);
                string testFile = "transformationTest11.png";
                //img5.get_normalize(0,255).save(testFile.c_str());
                //myOutputImage(2*image1.width(),1.1*image1.height(),1,3,0);
                int x1 = mySortedMatches[myIndexList[0]].descriptor1.col;
                int y1 = mySortedMatches[myIndexList[0]].descriptor1.row;
                int x1_dash = mySortedMatches[myIndexList[0]].descriptor2.col;
                int y1_dash = mySortedMatches[myIndexList[0]].descriptor2.row;

                int x2 = mySortedMatches[myIndexList[1]].descriptor1.col;
                int y2 = mySortedMatches[myIndexList[1]].descriptor1.row;
                int x2_dash = mySortedMatches[myIndexList[1]].descriptor2.col;
                int y2_dash = mySortedMatches[myIndexList[1]].descriptor2.row;

                int x3 = mySortedMatches[myIndexList[2]].descriptor1.col;
                int y3 = mySortedMatches[myIndexList[2]].descriptor1.row;
                int x3_dash = mySortedMatches[myIndexList[2]].descriptor2.col;
                int y3_dash = mySortedMatches[myIndexList[2]].descriptor2.row;

                int x4 = mySortedMatches[myIndexList[3]].descriptor1.col;
                int y4 = mySortedMatches[myIndexList[3]].descriptor1.row;
                int x4_dash = mySortedMatches[myIndexList[3]].descriptor2.col;
                int y4_dash = mySortedMatches[myIndexList[3]].descriptor2.row;

                matB << x1_dash << arma::endr
                     << y1_dash << arma::endr
                     << x2_dash << arma::endr
                     << y2_dash << arma::endr
                     << x3_dash << arma::endr
                     << y3_dash << arma::endr
                     << x4_dash << arma::endr
                     << y4_dash << arma::endr;
                //matB.print("matrixB is:");
                matA << x1 << y1 << 1 << 0 << 0 << 0 << (-1 * x1 * x1_dash) << (-1 * y1 * x1_dash) << arma::endr
                     << 0 << 0 << 0 << x1 << y1 << 1 << (-1 * x1 * y1_dash) << (-1 * y1 * y1_dash) << arma::endr
                     << x2 << y2 << 1 << 0 << 0 << 0 << (-1 * x2 * x2_dash) << (-1 * y2 * x2_dash) << arma::endr
                     << 0 << 0 << 0 << x2 << y2 << 1 << (-1 * x2 * y2_dash) << (-1 * y2 * y2_dash) << arma::endr
                     << x3 << y3 << 1 << 0 << 0 << 0 << (-1 * x3 * x3_dash) << (-1 * y3 * x3_dash) << arma::endr
                     << 0 << 0 << 0 << x3 << y3 << 1 << (-1 * x3 * y3_dash) << (-1 * y3 * y3_dash) << arma::endr
                     << x4 << y4 << 1 << 0 << 0 << 0 << (-1 * x4 * x4_dash) << (-1 * y4 * x4_dash) << arma::endr
                     << 0 << 0 << 0 << x4 << y4 << 1 << (-1 * x4 * y4_dash) << (-1 * y4 * y4_dash) << arma::endr;


                vector<Matches> myNewMatchesVector;
                matC = arma::solve(matA,matB);
                //matC.print("Transformation Matrix is:");
                double a11 = matC(0,0),a12 = matC(1,0),a13 = matC(2,0), a21 = matC(3,0), a22 =matC(4,0), a23 = matC(5,0), a31 = matC(6,0), a32 = matC(7,0), a33 = 1;     //0, a32 = 0, a33 = 1;//
                double detA = (a11*a22*a33) + (a12*a23*a31) + (a13*a21*a32) - (a11*a23*a32) - (a12*a21*a33) - (a13*a22*a31);
                //cout << "detA: " << detA << endl;
                double x,y;

                for(int i = 0; i<img5.width(); i++ ) {
                        for(int j = 0; j < img5.height(); j++) {
                                x = (   (((a22*a33)-(a23*a32)) * i ) +  ((  (a13*a32)-(a12*a33)  ) * j) + ((  (a12*a23)-(a13*a22)  ) * 1));     // (   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                                y = (  (  (    (a23*a31)-(a21*a33)    )*i  ) + (  (   (a11*a33)-(a13*a31)   )*j  ) + (  (   (a13*a21)-(a11*a23)   )*1  )   );     //(   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                                int modX = floor(x/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                                int modY = floor(y/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                                // y = y/detA;
                                //cout << "(i,j):( "<<i<<","<<j<<") : (" << x << "," << y << ")" << endl;
                                if (modY >= 0 && modY < img5.height() && modX >= 0 && modX < img5.width() && i < myOutputImage.width() && j < myOutputImage.height() ) {
                                        for(int c = 0; c  < 3; c++) {
                                                myOutputImage(i,j,0,c) = img5(modX,modY,0,c);
                                        }

                                }
                                // else{
                                //         for(int c = 0; c  < 3; c++) {
                                //                 myOutputImage(i,j,0,c) = 0;
                                //         }
                                // }

                        }
                }

                return myOutputImage;
        }
        else if(side == "right") {
                CImg<double> img4(image2.width(),image2.height(),1,3,0),img5(image2.width(),image2.height(),1,3,0);
                string testFile2 = "transformationTest222.png";
                string testFile = "transformationTest.png";
                CImg<double> myOutputImage(2*image2.width(),image2.height(),1,3,0);



                image2.append(img5,'x',0);
                //image2.get_normalize(0,255).save(testFile2.c_str());
                //image2.append(img4,'y',0);
                //image2.get_normalize(0,255).save(testFile.c_str());

                // string testFile = "transformationTest.png";
                // img5.get_normalize(0,255).save(testFile.c_str());

                // myOutputImage = img4;
                arma::mat matB(8,1);
                arma::mat matA(8,8);
                arma::mat matC(8,1);

                //myOutputImage(2*image2.width(),1.1*image2.height(),1,3,0);
                int x1_dash = mySortedMatches[myIndexList[0]].descriptor1.col;
                int y1_dash = mySortedMatches[myIndexList[0]].descriptor1.row;
                int x1 = mySortedMatches[myIndexList[0]].descriptor2.col;
                int y1 = mySortedMatches[myIndexList[0]].descriptor2.row;

                int x2_dash = mySortedMatches[myIndexList[1]].descriptor1.col;
                int y2_dash = mySortedMatches[myIndexList[1]].descriptor1.row;
                int x2 = mySortedMatches[myIndexList[1]].descriptor2.col;
                int y2 = mySortedMatches[myIndexList[1]].descriptor2.row;

                int x3_dash = mySortedMatches[myIndexList[2]].descriptor1.col;
                int y3_dash = mySortedMatches[myIndexList[2]].descriptor1.row;
                int x3 = mySortedMatches[myIndexList[2]].descriptor2.col;
                int y3 = mySortedMatches[myIndexList[2]].descriptor2.row;

                int x4_dash = mySortedMatches[myIndexList[3]].descriptor1.col;
                int y4_dash = mySortedMatches[myIndexList[3]].descriptor1.row;
                int x4 = mySortedMatches[myIndexList[3]].descriptor2.col;
                int y4 = mySortedMatches[myIndexList[3]].descriptor2.row;

                matB << x1_dash << arma::endr
                     << y1_dash << arma::endr
                     << x2_dash << arma::endr
                     << y2_dash << arma::endr
                     << x3_dash << arma::endr
                     << y3_dash << arma::endr
                     << x4_dash << arma::endr
                     << y4_dash << arma::endr;
                //matB.print("matrixB is:");
                matA << x1 << y1 << 1 << 0 << 0 << 0 << (-1 * x1 * x1_dash) << (-1 * y1 * x1_dash) << arma::endr
                     << 0 << 0 << 0 << x1 << y1 << 1 << (-1 * x1 * y1_dash) << (-1 * y1 * y1_dash) << arma::endr
                     << x2 << y2 << 1 << 0 << 0 << 0 << (-1 * x2 * x2_dash) << (-1 * y2 * x2_dash) << arma::endr
                     << 0 << 0 << 0 << x2 << y2 << 1 << (-1 * x2 * y2_dash) << (-1 * y2 * y2_dash) << arma::endr
                     << x3 << y3 << 1 << 0 << 0 << 0 << (-1 * x3 * x3_dash) << (-1 * y3 * x3_dash) << arma::endr
                     << 0 << 0 << 0 << x3 << y3 << 1 << (-1 * x3 * y3_dash) << (-1 * y3 * y3_dash) << arma::endr
                     << x4 << y4 << 1 << 0 << 0 << 0 << (-1 * x4 * x4_dash) << (-1 * y4 * x4_dash) << arma::endr
                     << 0 << 0 << 0 << x4 << y4 << 1 << (-1 * x4 * y4_dash) << (-1 * y4 * y4_dash) << arma::endr;


                vector<Matches> myNewMatchesVector;
                matC = arma::solve(matA,matB);
                //matC.print("Transformation Matrix is:");
                double a11 = matC(0,0),a12 = matC(1,0),a13 = matC(2,0), a21 = matC(3,0), a22 =matC(4,0), a23 = matC(5,0), a31 = matC(6,0), a32 = matC(7,0), a33 = 1;                  //0, a32 = 0, a33 = 1;//
                double detA = (a11*a22*a33) + (a12*a23*a31) + (a13*a21*a32) - (a11*a23*a32) - (a12*a21*a33) - (a13*a22*a31);
                //cout << "detA: " << detA << endl;
                double x,y;

                for(int i = 0; i<image2.width(); i++ ) {
                        for(int j = 0; j < image2.height(); j++) {
                                x = (   (((a22*a33)-(a23*a32)) * i ) +  ((  (a13*a32)-(a12*a33)  ) * j) + ((  (a12*a23)-(a13*a22)  ) * 1));                  // (   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                                y = (  (  (    (a23*a31)-(a21*a33)    )*i  ) + (  (   (a11*a33)-(a13*a31)   )*j  ) + (  (   (a13*a21)-(a11*a23)   )*1  )   );                  //(   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) );
                                int modX = floor(x/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                                int modY = floor(y/((   (((a21*a32)-(a22*a31) )*i)  + (  (  (a12*a31)-(a11*a32)   )  * j  ) + (  (a11*a22)-(a12*a21)  ) )));
                                // y = y/detA;
                                //cout << "(i,j):( "<<i<<","<<j<<") : (" << x << "," << y << ")" << endl;
                                if (modY >= 0 && modY < image2.height() && modX >= 0 && modX < image2.width() && i < myOutputImage.width() && j < myOutputImage.height() ) {
                                        for(int c = 0; c  < 3; c++) {
                                                myOutputImage(i,j,0,c) = image2(modX,modY,0,c);
                                        }

                                }
                                // else{
                                //         for(int c = 0; c  < 3; c++) {
                                //                 myOutputImage(i,j,0,c) = 0;
                                //         }
                                // }

                        }
                }

                return myOutputImage;
        }

        // matB << x1_dash << arma::endr
        //      << y1_dash << arma::endr
        //      << x2_dash << arma::endr
        //      << y2_dash << arma::endr
        //      << x3_dash << arma::endr
        //      << y3_dash << arma::endr
        //      << x4_dash << arma::endr
        //      << y4_dash << arma::endr;
        // //matB.print("matrixB is:");
        // matA << x1 << y1 << 1 << 0 << 0 << 0 << (-1 * x1 * x1_dash) << (-1 * y1 * x1_dash) << arma::endr
        //      << 0 << 0 << 0 << x1 << y1 << 1 << (-1 * x1 * y1_dash) << (-1 * y1 * y1_dash) << arma::endr
        //      << x2 << y2 << 1 << 0 << 0 << 0 << (-1 * x2 * x2_dash) << (-1 * y2 * x2_dash) << arma::endr
        //      << 0 << 0 << 0 << x2 << y2 << 1 << (-1 * x2 * y2_dash) << (-1 * y2 * y2_dash) << arma::endr
        //      << x3 << y3 << 1 << 0 << 0 << 0 << (-1 * x3 * x3_dash) << (-1 * y3 * x3_dash) << arma::endr
        //      << 0 << 0 << 0 << x3 << y3 << 1 << (-1 * x3 * y3_dash) << (-1 * y3 * y3_dash) << arma::endr
        //      << x4 << y4 << 1 << 0 << 0 << 0 << (-1 * x4 * x4_dash) << (-1 * y4 * x4_dash) << arma::endr
        //      << 0 << 0 << 0 << x4 << y4 << 1 << (-1 * x4 * y4_dash) << (-1 * y4 * y4_dash) << arma::endr;





        //matA.print("matrixA is:");



}

Matches return_widest_Sift(vector<Matches> myList){
        int temp = 0;
        Matches myMatch;
        for(int i = 0; i < myList.size(); i++){
                if(myList[i].descriptor1.col > temp){
                        Matches tempppy(myList[i].descriptor1,myList[i].descriptor2,myList[i].distance);
                        myMatch = tempppy;
                }
        }
        return myMatch;
}
CImg<double> transforms_images_using_sift(CImg<double> image1,CImg<double> image2,const char* opfilename,vector<Matches> mySortedMatches,string side){
        unsigned int selectedSize = 0;
        vector<Matches> finalDescriptors;
        CImg<double> transformedImage;
        vector<Matches> myMatchesAndCount;
        vector<unsigned int> myIndexList;
        myIndexList.push_back(0);
        myIndexList.push_back(3);
        myIndexList.push_back(5);
        myIndexList.push_back(7);
        int counter = 0;
        int index = 0;

        // for (unsigned int i = 0; i < mySortedMatches.size(); i++) { // this loop selects 4 randomly generated indices which are distinct.
        //         index = rand() % mySortedMatches.size();
        //         // myIndexList.push_back(index);
        //         // counter++;
        //         if( i == 0) {
        //                 myIndexList.push_back(index);
        //                 counter++;
        //         }
        //         else{
        //                 bool found = (std::find(myIndexList.begin(), myIndexList.end(), index) != myIndexList.end());
        //                 if(found) {
        //                         // do nothing, do not add a duplicate index.
        //                 }
        //                 else
        //                 {
        //                         myIndexList.push_back(index);
        //                         counter++;
        //                 }
        //
        //         }
        //         if(counter == 4) {
        //                 break;
        //         }
        //
        // }
        if(side == "left") {
                // 8 points generated uptill here.
                // need a function to accept those 8 points and perform a transformation on the image 1 to produce image 2.
                transformedImage = generate_homographies_and_transform(myIndexList,mySortedMatches,image1,image2,side);
                //transformedImage.append(image2,'x',0);
                transformedImage.autocrop();


                //transformedImage.get_normalize(0,255).save(opfilename);


                CImg<double> tesingtesting(1000,1000,1,3,0);
                int someVariable = (1000 - transformedImage.height())/2;
                tesingtesting.draw_image(0,someVariable,transformedImage);
                tesingtesting.get_normalize(0,255).save(opfilename);
                return tesingtesting;
                //return transformedImage;
        }
        if(side == "right") {
                transformedImage = generate_homographies_and_transform(myIndexList,mySortedMatches,image1,image2,side);
                //image2.append(transformedImage,'x',0);
                transformedImage.autocrop();
                //transformedImage.get_normalize(0,255).save(opfilename);

                CImg<double> tesingtesting(1000,1000,1,3,0);
                int someVariable = (1000 - transformedImage.height())/2;
                //int leftWidth = 1000 - transformedImage.width();
                tesingtesting.draw_image(0,someVariable,transformedImage);
                tesingtesting.get_normalize(0,255).save(opfilename);
                return tesingtesting;
        }


}
CImg<double> perform_stiching(CImg<double> tgtimg,CImg<double> srcimg,CImg<double> mask,const char* opfilename){
        double sigma = 4.0;
        CImg<double> unit(mask.width(), mask.height(), 1, 3); //Unit Matrix
        unit.fill(1);
        // Calculating the Gaussian Pyramid for source image by convolution and then down-sampling
        CImg<double> G0s = srcimg;
        CImg<double> G1s = G0s.get_blur(sigma).get_resize(-50, -50);
        CImg<double> G2s = G1s.get_blur(sigma).get_resize(-50, -50);
        CImg<double> G3s = G2s.get_blur(sigma).get_resize(-50, -50);
        CImg<double> G4s = G3s.get_blur(sigma).get_resize(-50, -50);
        CImg<double> G5s = G4s.get_blur(sigma).get_resize(-50, -50);

        //Calculating the Gaussian Pyramid for target image by convolution and then down-sampling
        CImg<double> G0t = tgtimg;
        CImg<double> G1t = G0t.get_blur(sigma).get_resize(-50, -50);
        CImg<double> G2t = G1t.get_blur(sigma).get_resize(-50, -50);
        CImg<double> G3t = G2t.get_blur(sigma).get_resize(-50, -50);
        CImg<double> G4t = G3t.get_blur(sigma).get_resize(-50, -50);
        CImg<double> G5t = G4t.get_blur(sigma).get_resize(-50, -50);

        //Calculating the Gaussian Pyramid for mask by convolution and then down-sampling
        CImg<double> G0m = mask.normalize(0, 1);
        CImg<double> G1m = G0m.get_blur(sigma).get_resize(-50, -50).normalize(0, 1);
        CImg<double> G2m = G1m.get_blur(sigma).get_resize(-50, -50).normalize(0, 1);
        CImg<double> G3m = G2m.get_blur(sigma).get_resize(-50, -50).normalize(0, 1);
        CImg<double> G4m = G3m.get_blur(sigma).get_resize(-50, -50).normalize(0, 1);
        CImg<double> G5m = G4m.get_blur(sigma).get_resize(-50, -50).normalize(0, 1);

        //Saving Gaussian pyramid images of source image
        // G0s.save("G0s.png");
        // G1s.save("G1s.png");
        // G2s.save("G2s.png");
        // G3s.save("G3s.png");
        // G4s.save("G4s.png");
        // G5s.save("G5s.png");

        //Saving Gaussian pyramid images of Target image
        // G0t.save("G0t.png");
        // G1t.save("G1t.png");
        // G2t.save("G2t.png");
        // G3t.save("G3t.png");
        // G4t.save("G4t.png");
        // G5t.save("G5t.png");

        //Saving Gaussian pyramid images of mask image
        // G0m.get_normalize(0, 255).save("G0m.png");
        // G1m.get_normalize(0, 255).save("G1m.png");
        // G2m.get_normalize(0, 255).save("G2m.png");
        // G3m.get_normalize(0, 255).save("G3m.png");
        // G4m.get_normalize(0, 255).save("G4m.png");
        // G5m.get_normalize(0, 255).save("G5m.png");

        //Calculating the Laplacian Pyramid from the Gaussian pyramid for source image
        CImg<double> L0s = G0s - G1s.get_resize(G0s.width(), G0s.height());
        CImg<double> L1s = G1s - G2s.get_resize(G1s.width(), G1s.height());
        CImg<double> L2s = G2s - G3s.get_resize(G2s.width(), G2s.height());
        CImg<double> L3s = G3s - G4s.get_resize(G3s.width(), G3s.height());
        CImg<double> L4s = G4s - G5s.get_resize(G4s.width(), G4s.height());
        CImg<double> L5s = G5s;

        //Calculating the Laplacian Pyramid from the Gaussian pyramid for target image
        CImg<double> L0t = G0t - G1t.get_resize(G0t.width(), G0t.height());
        CImg<double> L1t = G1t - G2t.get_resize(G1t.width(), G1t.height());
        CImg<double> L2t = G2t - G3t.get_resize(G2t.width(), G2t.height());
        CImg<double> L3t = G3t - G4t.get_resize(G3t.width(), G3t.height());
        CImg<double> L4t = G4t - G5t.get_resize(G4t.width(), G4t.height());
        CImg<double> L5t = G5t;

        //Saving Laplacian Pyramid images of source image
        // L0s.save("L0s.png");
        // L1s.save("L1s.png");
        // L2s.save("L2s.png");
        // L3s.save("L3s.png");
        // L4s.save("L4s.png");
        // L5s.save("L5s.png");

        //Saving Laplacian Pyramid images of target image
        // L0t.save("L0t.png");
        // L1t.save("L1t.png");
        // L2t.save("L2t.png");
        // L3t.save("L3t.png");
        // L4t.save("L4t.png");
        // L5t.save("L5t.png");

        // Calculating Laplacian Pyramid of images of composite image
        // using Laplacian pyramid images of source and target image
        // and Guassian pyramid images of mask image
        CImg<double> L0c(L0t.width(), L0t.height(), L0t.depth(), L0t.spectrum());
        L0c = G0m.mul(L0s) + (unit - G0m).mul(L0t);

        CImg<double> L1c(L1t.width(), L1t.height(), L1t.depth(), L1t.spectrum());
        L1c = G1m.mul(L1s) + (unit - G1m).mul(L1t);
        CImg<double> L2c(L2t.width(), L2t.height(), L2t.depth(), L2t.spectrum());
        L2c = G2m.mul(L2s) + (unit - G2m).mul(L2t);
        CImg<double> L3c(L3t.width(), L3t.height(), L3t.depth(), L3t.spectrum());
        L3c = G3m.mul(L3s) + (unit - G3m).mul(L3t);
        CImg<double> L4c(L4t.width(), L4t.height(), L4t.depth(), L4t.spectrum());
        L4c = G4m.mul(L4s) + (unit - G4m).mul(L4t);
        CImg<double> L5c(L5t.width(), L5t.height(), L5t.depth(), L5t.spectrum());
        L5c = G5m.mul(L5s) + (unit - G5m).mul(L5t);

        //Saving Laplacian pyramid images of composite image
        // L0c.save("L0c.png");
        // L1c.save("L1c.png");
        // L2c.save("L2c.png");
        // L3c.save("L3c.png");
        // L4c.save("L4c.png");
        // L5c.save("L5c.png");

        //Calculating Gaussian pyramid images of composite image from the Laplacian pyramid images
        CImg<double> G5c = L5c;
        CImg<double> G4c = (G5c.get_resize(L4c.width(), L4c.height()) + L4c).normalize(0, 255);
        CImg<double> G3c = (G4c.get_resize(L3c.width(), L3c.height()) + L3c).normalize(0, 255);
        CImg<double> G2c = (G3c.get_resize(L2c.width(), L2c.height()) + L2c).normalize(0, 255);
        CImg<double> G1c = (G2c.get_resize(L1c.width(), L1c.height()) + L1c).normalize(0, 255);
        CImg<double> G0c = (G1c.get_resize(L0c.width(), L0c.height()) + L0c).normalize(0, 255);

        //Saving Guassian pyramid images of composite image
        // G0c.save("G0c.png");
        // G1c.save("G1c.png");
        // G2c.save("G2c.png");
        // G3c.save("G3c.png");
        // G4c.save("G4c.png");
        // G5c.save("G5c.png");

        //Output composite image is stored in "blended.png"
        CImg<double> blended = G0c;
        blended.autocrop(100);
        blended.save(opfilename);
        return blended;



}


int main(int argc, char **argv)
{
        try {
                if(argc < 3) {
                        cout << "Insufficent number of arguments; correct usage:" << endl;
                        cout << "    ./a2 part1 input_image.png" << endl;
                        return -1;
                }
                /*
                   TEST CODE - STARTS
                 */

                string part = argv[1];
                //char *imagename = argv[2];
                // CImg<double> input_image("images/part3/eiffel_18.jpg");
                // CImg<double> input_gray = input_image.get_RGBtoHSI().get_channel(2);
                // vector<SiftDescriptor> input_descriptors = Sift::compute_sift(input_gray);
                // draw_descriptor_image(input_image, input_descriptors, "input_image.png");
                /*
                   TEST CODE - ENDS
                 */
                string mypath = "images/"+part+"/";
                if(part == "part1") {
                        // Billboard
                        string myPathName = mypath+"lincoln.png";
                        //string outputFilename = "testtest.png";
                        string outputFilename = "lincoln_wrapped.png";
                        CImg<double> image1(myPathName.c_str());
                        // ---- to implement projective transformation using inverse wrapping

                        projectiveMappingInverse(myPathName.c_str(),outputFilename.c_str());
                        // ----
                        string book1 = mypath+"book1.jpg";
                        string book2 = mypath+"book2.jpg";
                        outputFilename = "book_result.png";
                        projectiveWarppingInversepart2(book1.c_str(),book2.c_str(),outputFilename.c_str());
                        // ----
                        string input_image_b = argv[2];
                        string input_image_billboard = mypath+input_image_b;
                        string billboard1 = mypath+"billboard1.jpg";
                        string billboard2 = mypath+"billboard2.png";
                        string billboard3 = mypath+"billboard3.jpg";
                        billboardWrappingInverse(input_image_billboard.c_str(),billboard1.c_str(),billboard2.c_str(),billboard3.c_str());
                }
                else if(part == "part2") {
                        if (argc != 5)
                        {
                                cout << "Incorrect arguments; correct usage" << endl;
                                cout << "./a2 part2 image_1.png image_2.png mask.png" << endl;
                        }
                        else
                        {

                                double sigma = 4.0;
                                string mypath = "images/"+part+"/";
                                string targetImage = mypath+(argv[2]);
                                string sourceImage = mypath+(argv[3]);
                                string oldMask = mypath+argv[4];
                                CImg<double> tgtimg(targetImage.c_str()); //Left side of the blended image
                                CImg<double> srcimg(sourceImage.c_str()); //Right side of the blended image
                                CImg<double> oldmask(oldMask.c_str()); //Given Mask
                                //Generating the new mask since the given mask has 1 channel
                                CImg<double> mask(oldmask.width(), oldmask.height(), 1, 3);
                                cimg_forXYC(mask, x, y, c)
                                {
                                        mask(x, y, c) = oldmask(x, y);
                                }
                                CImg<double> unit(mask.width(), mask.height(), 1, 3); //Unit Matrix
                                unit.fill(1);

                                // Calculating the Gaussian Pyramid for source image by convolution and then down-sampling
                                CImg<double> G0s = srcimg;
                                CImg<double> G1s = G0s.get_blur(sigma).get_resize(-50, -50);
                                CImg<double> G2s = G1s.get_blur(sigma).get_resize(-50, -50);
                                CImg<double> G3s = G2s.get_blur(sigma).get_resize(-50, -50);
                                CImg<double> G4s = G3s.get_blur(sigma).get_resize(-50, -50);
                                CImg<double> G5s = G4s.get_blur(sigma).get_resize(-50, -50);

                                //Calculating the Gaussian Pyramid for target image by convolution and then down-sampling
                                CImg<double> G0t = tgtimg;
                                CImg<double> G1t = G0t.get_blur(sigma).get_resize(-50, -50);
                                CImg<double> G2t = G1t.get_blur(sigma).get_resize(-50, -50);
                                CImg<double> G3t = G2t.get_blur(sigma).get_resize(-50, -50);
                                CImg<double> G4t = G3t.get_blur(sigma).get_resize(-50, -50);
                                CImg<double> G5t = G4t.get_blur(sigma).get_resize(-50, -50);

                                //Calculating the Gaussian Pyramid for mask by convolution and then down-sampling
                                CImg<double> G0m = mask.normalize(0, 1);
                                CImg<double> G1m = G0m.get_blur(sigma).get_resize(-50, -50).normalize(0, 1);
                                CImg<double> G2m = G1m.get_blur(sigma).get_resize(-50, -50).normalize(0, 1);
                                CImg<double> G3m = G2m.get_blur(sigma).get_resize(-50, -50).normalize(0, 1);
                                CImg<double> G4m = G3m.get_blur(sigma).get_resize(-50, -50).normalize(0, 1);
                                CImg<double> G5m = G4m.get_blur(sigma).get_resize(-50, -50).normalize(0, 1);

                                //Saving Gaussian pyramid images of source image
                                G0s.save("G0s.png");
                                G1s.save("G1s.png");
                                G2s.save("G2s.png");
                                G3s.save("G3s.png");
                                G4s.save("G4s.png");
                                G5s.save("G5s.png");

                                //Saving Gaussian pyramid images of Target image
                                G0t.save("G0t.png");
                                G1t.save("G1t.png");
                                G2t.save("G2t.png");
                                G3t.save("G3t.png");
                                G4t.save("G4t.png");
                                G5t.save("G5t.png");

                                //Saving Gaussian pyramid images of mask image
                                G0m.get_normalize(0, 255).save("G0m.png");
                                G1m.get_normalize(0, 255).save("G1m.png");
                                G2m.get_normalize(0, 255).save("G2m.png");
                                G3m.get_normalize(0, 255).save("G3m.png");
                                G4m.get_normalize(0, 255).save("G4m.png");
                                G5m.get_normalize(0, 255).save("G5m.png");

                                //Calculating the Laplacian Pyramid from the Gaussian pyramid for source image
                                CImg<double> L0s = G0s - G1s.get_resize(G0s.width(), G0s.height());
                                CImg<double> L1s = G1s - G2s.get_resize(G1s.width(), G1s.height());
                                CImg<double> L2s = G2s - G3s.get_resize(G2s.width(), G2s.height());
                                CImg<double> L3s = G3s - G4s.get_resize(G3s.width(), G3s.height());
                                CImg<double> L4s = G4s - G5s.get_resize(G4s.width(), G4s.height());
                                CImg<double> L5s = G5s;

                                //Calculating the Laplacian Pyramid from the Gaussian pyramid for target image
                                CImg<double> L0t = G0t - G1t.get_resize(G0t.width(), G0t.height());
                                CImg<double> L1t = G1t - G2t.get_resize(G1t.width(), G1t.height());
                                CImg<double> L2t = G2t - G3t.get_resize(G2t.width(), G2t.height());
                                CImg<double> L3t = G3t - G4t.get_resize(G3t.width(), G3t.height());
                                CImg<double> L4t = G4t - G5t.get_resize(G4t.width(), G4t.height());
                                CImg<double> L5t = G5t;

                                //Saving Laplacian Pyramid images of source image
                                L0s.save("L0s.png");
                                L1s.save("L1s.png");
                                L2s.save("L2s.png");
                                L3s.save("L3s.png");
                                L4s.save("L4s.png");
                                L5s.save("L5s.png");

                                //Saving Laplacian Pyramid images of target image
                                L0t.save("L0t.png");
                                L1t.save("L1t.png");
                                L2t.save("L2t.png");
                                L3t.save("L3t.png");
                                L4t.save("L4t.png");
                                L5t.save("L5t.png");

                                // Calculating Laplacian Pyramid of images of composite image
                                // using Laplacian pyramid images of source and target image
                                // and Guassian pyramid images of mask image
                                CImg<double> L0c(L0t.width(), L0t.height(), L0t.depth(), L0t.spectrum());
                                L0c = G0m.mul(L0s) + (unit - G0m).mul(L0t);
                                CImg<double> L1c(L1t.width(), L1t.height(), L1t.depth(), L1t.spectrum());
                                L1c = G1m.mul(L1s) + (unit - G1m).mul(L1t);
                                CImg<double> L2c(L2t.width(), L2t.height(), L2t.depth(), L2t.spectrum());
                                L2c = G2m.mul(L2s) + (unit - G2m).mul(L2t);
                                CImg<double> L3c(L3t.width(), L3t.height(), L3t.depth(), L3t.spectrum());
                                L3c = G3m.mul(L3s) + (unit - G3m).mul(L3t);
                                CImg<double> L4c(L4t.width(), L4t.height(), L4t.depth(), L4t.spectrum());
                                L4c = G4m.mul(L4s) + (unit - G4m).mul(L4t);
                                CImg<double> L5c(L5t.width(), L5t.height(), L5t.depth(), L5t.spectrum());
                                L5c = G5m.mul(L5s) + (unit - G5m).mul(L5t);

                                //Saving Laplacian pyramid images of composite image
                                L0c.save("L0c.png");
                                L1c.save("L1c.png");
                                L2c.save("L2c.png");
                                L3c.save("L3c.png");
                                L4c.save("L4c.png");
                                L5c.save("L5c.png");

                                //Calculating Gaussian pyramid images of composite image from the Laplacian pyramid images
                                CImg<double> G5c = L5c;
                                CImg<double> G4c = (G5c.get_resize(L4c.width(), L4c.height()) + L4c).normalize(0, 255);
                                CImg<double> G3c = (G4c.get_resize(L3c.width(), L3c.height()) + L3c).normalize(0, 255);
                                CImg<double> G2c = (G3c.get_resize(L2c.width(), L2c.height()) + L2c).normalize(0, 255);
                                CImg<double> G1c = (G2c.get_resize(L1c.width(), L1c.height()) + L1c).normalize(0, 255);
                                CImg<double> G0c = (G1c.get_resize(L0c.width(), L0c.height()) + L0c).normalize(0, 255);

                                //Saving Guassian pyramid images of composite image
                                G0c.save("G0c.png");
                                G1c.save("G1c.png");
                                G2c.save("G2c.png");
                                G3c.save("G3c.png");
                                G4c.save("G4c.png");
                                G5c.save("G5c.png");

                                //Output composite image is stored in "blended.png"
                                CImg<double> blended = G0c;
                                blended.save("blended.png");
                        }
                }
                else if(part == "part3") {
                        if(argc < 4) {
                                cout << "Insufficent number of arguments; correct usage:" << endl;
                                cout << "    ./a2 part3 image_src.png image_dst.png" << endl;
                                return -1;
                        }
                        string image1 = argv[2];
                        string image2 = argv[3];
                        string myPathName1 = mypath+"/"+image1;
                        string myPathName2 = mypath+"/"+image2;
                        string outputfile = "sift_matches.png";
                        cout << "image1: " << image1 << " image2: " << image2 << endl;
                        // part 1 of part 3 : where SIFT descriptors are just compared
                        CImg<double> input_image1(myPathName1.c_str()), input_image2(myPathName2.c_str());
                        double ro_ratio = 0.73;
                        vector<Matches> sortedMatchess = compare_descriptor_image(input_image1, input_image2,ro_ratio,outputfile.c_str());
                        // input_gray.get_normalize(0,255).save("part3_test.png".c_str());

                        // part 2 of part 3 : Implementing RANSAC
                        string outputfile2 = "ransac_matches.png";
                        double match_threshold = 10.0;// putting a higher value of ro so that we get more pairs of sift points
                        run_RANSAC(input_image1,input_image2,match_threshold,ro_ratio,outputfile2.c_str(),sortedMatchess);
                        //run_RANSAC(input_image1,input_image2,match_threshold,outputfile2.c_str());

                }
                else if(part == "part4") {
                        if(argc < 5) {
                                cout << "Insufficent number of arguments; correct usage:" << endl;
                                cout << "    ./a2 part4 image_1.png image_2.png image_3.png" << endl;
                                return -1;
                        }
                        string image1 = argv[2];
                        string image2 = argv[3];
                        string image3 = argv[4];
                        string myPathName1 = mypath+"/"+image1;
                        string myPathName2 = mypath+"/"+image2;
                        string myPathName3 = mypath+"/"+image3;
                        string outputfile = "panaroma.png";
                        cout << "image1: " << image1 << " image2: " << image2 << " image3:" << image3 << endl;
                        CImg<double> input_image1(myPathName1.c_str()), input_image2(myPathName2.c_str()), input_image22(myPathName2.c_str()), input_image3(myPathName3.c_str());
                        double ro_ratio = 0.73;
                        double match_threshold = 5.0;// putting a higher value of ro so that we get more pairs of sift points

                        vector<Matches> sortedMatchess1 = compare_descriptor_image(input_image1, input_image2,ro_ratio,NULL);
                        string outputfile2 = "part4_ransac_matches11.png";
                        vector<Matches> final_Sift_Matches1 = run_RANSAC(input_image1,input_image2,match_threshold,ro_ratio,NULL,sortedMatchess1);
                        string outputfile22 = "part4_ransac_matches12.png";
                        CImg<double> morphed_image =transforms_images_using_sift(input_image1,input_image2,outputfile22.c_str(),final_Sift_Matches1,"left");

                        //----------//
                        CImg<double> transImage1(morphed_image), transImage2(myPathName2.c_str());
                        vector<Matches> trans_sortedMatches1 = compare_descriptor_image(transImage1, transImage2,ro_ratio,NULL);
                        //string trans_output = "trans_output.png";
                        vector<Matches> trans_final_Sift_Matches1 = run_RANSAC(transImage1,transImage2,match_threshold,ro_ratio,NULL,trans_sortedMatches1);

                        int xtransform1 = trans_final_Sift_Matches1[3].descriptor1.col;
                        int ytransform1 = trans_final_Sift_Matches1[3].descriptor1.row;
                        int xtransform1_dash = trans_final_Sift_Matches1[3].descriptor2.col;
                        int ytransform1_dash = trans_final_Sift_Matches1[3].descriptor2.row;

                        int xtransform3 = trans_final_Sift_Matches1[trans_final_Sift_Matches1.size()-2].descriptor1.col;
                        int ytransform3 = trans_final_Sift_Matches1[trans_final_Sift_Matches1.size()-2].descriptor1.row;
                        int xtransform3_dash = trans_final_Sift_Matches1[trans_final_Sift_Matches1.size()-2].descriptor2.col;
                        int ytransform3_dash = trans_final_Sift_Matches1[trans_final_Sift_Matches1.size()-2].descriptor2.row;


                        //double xRatio = -100* (xtransform3 - xtransform1)/(xtransform3_dash - xtransform1_dash);
                        double yRatio = -100* (ytransform3 - ytransform1)/(ytransform3_dash - ytransform1_dash);
                        // string ttttt = "x";
                        transImage2.resize(-100,yRatio);
                        //
                        string resized_trans = "resized_trans.png";
                        // transImage2.get_normalize(0,255).save(resized_trans.c_str());
                        //CImg<double> merge1(outputfile22.c_str());//   outputfile22 = part4_ransac_matches12.png";
                        // merge1.append(transImage2,'x',0);
                        // merge1.get_normalize(0,255).save(resized_trans.c_str());

                        CImg<double> pano2(1000,1000,1,3,0);
                        int someVariable = (1000 - transImage2.height())/2;
                        //CImg<double> tesingtesting(transImage2.width(),merge1.height(),1,3,0);
                        //int someVariable = (merge1.height() - transImage2.height())/2;
                        pano2.draw_image(0,someVariable,transImage2);
                        pano2.get_normalize(0,255).save(resized_trans.c_str());
                        //merge1.append(tesingtesting,'x',0);
                        //merge1.get_normalize(0,255).save(resized_trans.c_str());



                        //tesingtesting.draw_image(0,someVariable,transformedImage);
                        //tesingtesting.get_normalize(0,255).save(opfilename);

                        CImg<double> pano1(morphed_image);//pano2(resized_trans.c_str());
                        vector<Matches> pano_sortedMatches1 = compare_descriptor_image(pano1, pano2,ro_ratio,NULL);
                        //string trans_output = "dantanakadantanaka.png";
                        vector<Matches> pano_final_Sift_Matches1 = run_RANSAC(pano1,pano2,match_threshold,ro_ratio,NULL,pano_sortedMatches1);
                        Matches pano_final_Sift_only_one = return_widest_Sift(pano_final_Sift_Matches1);

                        int maskLength = (pano_final_Sift_only_one.descriptor1.col + pano2.width() - pano_final_Sift_only_one.descriptor2.col - 1);
                        CImg<double> panoMask(maskLength,maskLength,1,3,255);
                        cout << "maskLength" << maskLength << endl;
                        for(int i = 0; i<pano_final_Sift_only_one.descriptor1.col; i++){
                                for(int j = 0; j< pano2.height(); j++){
                                        panoMask(i,j,0,0) = 0;
                                        panoMask(i,j,0,1) = 0;
                                        panoMask(i,j,0,2) = 0;
                                }
                        }
                        string output_pano1="output_pano1.png";

                        cout << pano_final_Sift_only_one.descriptor1.col/2 << endl;
                        CImg<double> canvas1(maskLength,maskLength,1,3,0),canvas2(maskLength,maskLength,1,3,0);
                        int widthAdjustment = (maskLength - pano1.height())/2;
                        canvas1.draw_image(0,widthAdjustment,pano1);
                        canvas2.draw_image(pano_final_Sift_only_one.descriptor1.col/2,widthAdjustment,pano2);
                        CImg<double> half_stiched = perform_stiching(canvas1,canvas2,panoMask,output_pano1.c_str());
                        //----------//
                        vector<Matches> sortedMatchess2 = compare_descriptor_image(input_image22, input_image3,ro_ratio,NULL);
                        string outputfile3 = "part4_ransac_matches21.png";
                        vector<Matches> final_Sift_Matches2 = run_RANSAC(input_image22,input_image3,match_threshold,ro_ratio,NULL,sortedMatchess2);
                        string outputfile33 = "part4_ransac_matches22.png";
                        CImg<double> morphed_image2 =transforms_images_using_sift(input_image22,input_image3,outputfile33.c_str(),final_Sift_Matches2,"right");


                        //----------//
                        CImg<double> transImage11(half_stiched), transImage22(morphed_image2);
                        vector<Matches> trans_sortedMatches11 = compare_descriptor_image(transImage11, transImage22,ro_ratio,NULL);
                        //string trans_output = "trans_output.png";
                        vector<Matches> trans_final_Sift_Matches11 = run_RANSAC(transImage11,transImage22,match_threshold,ro_ratio,NULL,trans_sortedMatches11);
                        cout << "RANSAC working" <<  endl;
                        xtransform1 = trans_final_Sift_Matches11[2].descriptor1.col;
                        ytransform1 = trans_final_Sift_Matches11[2].descriptor1.row;
                        xtransform1_dash = trans_final_Sift_Matches11[2].descriptor2.col;
                        ytransform1_dash = trans_final_Sift_Matches11[2].descriptor2.row;

                        xtransform3 = trans_final_Sift_Matches11[trans_final_Sift_Matches11.size()-2].descriptor1.col;
                        ytransform3 = trans_final_Sift_Matches11[trans_final_Sift_Matches11.size()-2].descriptor1.row;
                        xtransform3_dash = trans_final_Sift_Matches11[trans_final_Sift_Matches11.size()-2].descriptor2.col;
                        ytransform3_dash = trans_final_Sift_Matches11[trans_final_Sift_Matches11.size()-2].descriptor2.row;


                        //double xRatio = -100* (xtransform3 - xtransform1)/(xtransform3_dash - xtransform1_dash);
                        yRatio = -100* (ytransform3 - ytransform1)/(ytransform3_dash - ytransform1_dash);
                        // string ttttt = "x";
                        transImage22.resize(-100,yRatio);
                        //
                        string resized_trans22 = "resized_trans22.png";
                        // transImage22.get_normalize(0,255).save(resized_trans22.c_str());
                        //CImg<double> merge1(outputfile22.c_str());//   outputfile22 = part4_ransac_matches12.png";
                        // merge1.append(transImage22,'x',0);
                        // merge1.get_normalize(0,255).save(resized_trans22.c_str());

                        CImg<double> pano22(1000,1000,1,3,0);
                        someVariable = (1000 - transImage22.height())/2;
                        //CImg<double> tesingtesting(transImage22.width(),merge1.height(),1,3,0);
                        //int someVariable = (merge1.height() - transImage22.height())/2;
                        pano22.draw_image(0,someVariable,transImage22);
                        pano22.get_normalize(0,255).save(resized_trans22.c_str());
                        //merge1.append(tesingtesting,'x',0);
                        //merge1.get_normalize(0,255).save(resized_trans22.c_str());



                        //tesingtesting.draw_image(0,someVariable,transformedImage);
                        //tesingtesting.get_normalize(0,255).save(opfilename);

                        CImg<double> pano11(transImage11);//pano22(resized_trans22.c_str());
                        vector<Matches> pano_sortedMatches12 = compare_descriptor_image(pano11, pano22,ro_ratio,NULL);
                        cout << "working until here man! "<< endl;
                        //string trans_output = "dantanakadantanaka.png";
                        vector<Matches> pano_final_Sift_Matches12 = run_RANSAC(pano11,pano22,match_threshold,ro_ratio,NULL,pano_sortedMatches12);
                        cout << "working until here man! 2222"<< endl;
                        Matches pano_final_Sift_only_one_two = return_widest_Sift(pano_final_Sift_Matches12);
                        cout << "working until here man! 33333"<< endl;

                        //maskLength = (pano_final_Sift_only_one_two.descriptor1.col + pano22.width() - pano_final_Sift_only_one_two.descriptor2.col - 1);
                        CImg<double> panoMask1(maskLength,maskLength,1,3,255);
                        cout << "maskLength" << maskLength << endl;
                        for(int i = 0; i<pano_final_Sift_only_one_two.descriptor1.col+100; i++){
                                for(int j = 0; j< pano22.height(); j++){
                                        panoMask1(i,j,0,0) = 0;
                                        panoMask1(i,j,0,1) = 0;
                                        panoMask1(i,j,0,2) = 0;
                                }
                        }
                        string output_pano12="panoroma.png";

                        cout << pano_final_Sift_only_one_two.descriptor1.col/2 << endl;
                        CImg<double> canvas12(maskLength,maskLength,1,3,0),canvas22(maskLength,maskLength,1,3,0);
                        widthAdjustment = (maskLength - pano22.height())/2;
                        canvas12.draw_image(0,0,pano11);
                        canvas22.draw_image((100+pano_final_Sift_only_one_two.descriptor1.col)/2,widthAdjustment,pano22);
                        perform_stiching(canvas12,canvas22,panoMask1,output_pano12.c_str());
                        //----------//




                }


                // feel free to add more conditions for other parts (e.g. more specific)
                //  parts, for debugging, etc.
        }
        catch(const string &err) {
                cerr << "Error: " << err << endl;
        }
}
