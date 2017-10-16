/*********************************************************************
 File:     chromakey.cpp

 Author:   Wei-Yu Yu

 Email address: weiyuyu@usc.edu

 Usage: program_name in.bmp background.bmp dist_threshold out1.bmp out2.bmp

 Notes:
 This program performs the chroma key operation on an input 
 using two different methods.

 Method 1 Utilize a user-defined distance threshold from the
          chromakey value as a discriminator

 Method 2 Devise a method that to determine the chromakey mask
          that doesn't require a user-input threshold

********************************************************************/

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "bmplib.h"

using namespace std;

// Prototypes
// IMPORTANT: you must exactly use these input types, function names, and 
// return types. Otherwise the grader can't test them.
void method1(unsigned char inImage[][SIZE][RGB], 
	     bool mask[][SIZE],
	     double threshold);

void method2(unsigned char inImage[][SIZE][RGB], 
	     bool mask[][SIZE]);

void replace(bool mask[][SIZE],
	     unsigned char inImage[][SIZE][RGB],
	     unsigned char bgImage[][SIZE][RGB],
	     unsigned char outImage[][SIZE][RGB]);

int main(int argc, char *argv[])
{
  // Image data array
  // Note:  DON'T use the static keyword except where we expressly say so.
  //        It puts the large array in a separate, fixed, area of memory. 
  //        It is bad practice. But useful until we have dynamic allocation.
  static unsigned char inputImage[SIZE][SIZE][RGB];
  static unsigned char bgrndImage[SIZE][SIZE][RGB];
  static unsigned char outputImage[SIZE][SIZE][RGB];
  static bool chromaMask[SIZE][SIZE];

  double threshold;

  if (argc < 6) {
    cerr << "usage: program_name in.bmp background.bmp dist_threshold " 
         << "out1.bmp out2.bmp" << endl;
    return 0;
  }
	
  if (readRGBBMP(argv[1], inputImage)) {
    cerr << "Error reading file: " << argv[1] << endl;
    return 1;
  }

  if (readRGBBMP(argv[2], bgrndImage)) {
    cout << "Error reading file: " << argv[2] << endl;
    return 1;
  }
  
  // Write code to convert the threshold (argv[3])
  //  from string format to a double and assign the 'threshold'
  threshold = atof(argv[3]);

  // Call Method 1 Function
  method1(inputImage,chromaMask, threshold);

  // Produce the output by calling replace()
  replace(chromaMask, inputImage, bgrndImage, outputImage);

  // Write the output image to a file using the filename argv[4]
  if (writeRGBBMP(argv[4], outputImage)) {
    cout << "Error writing file: " << argv[4] << endl;
    exit(1);
  }	
  writeRGBBMP(argv[4], outputImage);
  // Call Method 2 Function
  
  method2(inputImage,chromaMask);

  // Produce the output by calling replace()
  replace(chromaMask, inputImage, bgrndImage, outputImage);

  // Write the output image to a file using the filename argv[5]
  if (writeRGBBMP(argv[5], outputImage)) {
    cout << "Error writing file: " << argv[5] << endl;
    exit(1);
  }	
  writeRGBBMP(argv[5], outputImage);
  
  return 0;
}



// Use user-provided threshold for chroma-key distance
// The "output" of this method is to produce a valid mask array
//  where entries in the mask array are 1 for foreground image
//  and 0 for 'background'
void method1(unsigned char inImage[][SIZE][RGB], 
	     bool mask[][SIZE],
	     double threshold)
{
	int avg_r = 0,avg_g = 0,avg_b =0;
	double dist;
	int count = 0;

	for(int i = 0; i < SIZE; i++){
		for(int j = 0; j < 10; j++){
			avg_r += inImage[i][j][0];
			avg_g += inImage[i][j][1];
			avg_b += inImage[i][j][2];
			count ++;
		}
	}

	for(int i=0; i<15; i++){
		for(int j=0; j<SIZE; j++){
			avg_r += inImage[i][j][0];
			avg_g += inImage[i][j][1];
			avg_b += inImage[i][j][2];
			count ++;
		}
	}

	avg_r /= count;
	avg_g /= count;
	avg_b /= count;

	for (int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			dist = sqrt(pow(inImage[i][j][0]-avg_r,2)
				+pow(inImage[i][j][1]-avg_g,2)+pow(inImage[i][j][2]-avg_b,2));
			if(dist >= threshold){
				mask[i][j] = 1;
			}
			else{
				mask[i][j] = 0;
			}
		}
	}

}

// Devise a method to automatically come up with a threshold
//  for the chroma key determination
// The "output" of this method is to produce a valid mask array
//  where entries in the mask array are 1 for foreground image
//  and 0 for 'background'
void method2(unsigned char inImage[][SIZE][RGB], 
	     bool mask[][SIZE])  
{
	double dist, auto_threshold = 500, avg_dist=0;
	int count = 0;
	int default_r = inImage[0][0][0];
	int default_g = inImage[0][0][1];
	int default_b = inImage[0][0][2];

//Find average RGB distance and make sure it's not too small
	for(int i=SIZE/5; i<4*SIZE/5; i++){
		for(int j=SIZE/5; j<4*SIZE/5; j++){
			dist = sqrt(pow(inImage[i][j][0]-default_r,2)
				+pow(inImage[i][j][1]-default_g,2)+pow(inImage[i][j][2]-default_b,2));
			avg_dist += dist;
			count ++;
			
		}
	}

// Calculate average distamce
	avg_dist /= count;
	cout << avg_dist << endl;

// Setting auto_threshold to calculated distance if it is larger than avg_dist
// Attempt to find mininmum auto_threshold
	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			dist = sqrt(pow(inImage[i][j][0]-default_r,2)
				+pow(inImage[i][j][1]-default_g,2)+pow(inImage[i][j][2]-default_b,2));
			if(dist>avg_dist && dist<auto_threshold){
				auto_threshold = dist;
			}
		}
	}
	
	for (int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			dist = sqrt(pow(inImage[i][j][0]-default_r,2)
				+pow(inImage[i][j][1]-default_g,2)+pow(inImage[i][j][2]-default_b,2));
			if(dist>auto_threshold){
				mask[i][j] = 1;
			}
			else{
				mask[i][j] = 0;
			}
		}
	}
	
}

// If mask[i][j] = 1 use the input image pixel for the output image
// Else if mask[i][j] = 0 use the background image pixel
void replace(bool mask[SIZE][SIZE],
	     unsigned char inImage[SIZE][SIZE][RGB],
	     unsigned char bgImage[SIZE][SIZE][RGB],
	     unsigned char outImage[SIZE][SIZE][RGB])
{
  // Create the output image using the mask to determine
  //  whether to use the pixel from the Input or Background image
	for(int i=0; i<SIZE; i++){
		for(int j=0; j< SIZE; j++){
			if(mask[i][j]==0){
				for(int k=0; k<3; k++){
					outImage[i][j][k]=bgImage[i][j][k];
				}
			}
			else{
				for(int k=0; k<3; k++){
					outImage[i][j][k]=inImage[i][j][k];
				}
			}
		}
	}


}
