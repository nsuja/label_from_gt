#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
}

using namespace cv;
using namespace std;

Mat src;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/** @function main */
int main( int argc, char** argv )
{
	int has_objects = 0;
	if(argc != 3) {
		printf("Argument error! %s <input> <output>\n", argv[0]);
		return -1;
	}
	/// Load source image and convert it to gray
	src = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	int width = src.size().width;
	int height = src.size().height;
	Mat thre;
	threshold(src, thre, 200, 255, THRESH_BINARY);

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny(thre, canny_output, thresh, thresh*2, 3 );
	/// Find contours
	findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );

	/// Draw contours
	for( int i = 0; i< contours.size(); i++ ) {
	//	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	//	drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );

		approxPolyDP( Mat(contours[i]), contours_poly[i], 1, true );
		boundRect[i] = boundingRect( Mat(contours_poly[i]) );
		has_objects = 1;
	}
//	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
//	for( int i = 0; i< contours.size(); i++ ) {
//		//Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//		Scalar color = Scalar(0, 0, 255);
//		Scalar color_contour = Scalar(255, 255,255);
//		drawContours( drawing, contours_poly, i, color_contour, 1, 8, vector<Vec4i>(), 0, Point() );
//		rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, CV_FILLED, 8, 0 );
//	}
	if(has_objects) {
		std::string dirname = argv[2];
		std::string filename = basename(argv[1]);
		filename.resize(filename.size() - 4);
		std::string outFileName = dirname+"/"+filename+".txt";

		//int fd = open(outFileName.c_str(), O_WRONLY|O_CREAT|O_TRUNC, 0666);
		ofstream myfile;
		myfile.open (outFileName.c_str());
		for( int i = 0; i< contours.size(); i++ ) {
			char cadena[512];
			float x, y, w, h;
			x = (boundRect[i].x + boundRect[i].width/2.0)/(float)width;
			y = (boundRect[i].y + boundRect[i].height/2.0)/(float)height;
			w = boundRect[i].width/(float)width;
			h = boundRect[i].height/(float)height;
			sprintf(cadena, "2 %f %f %f %f\n", x, y, w, h);
			myfile<<cadena;
		}
		myfile.close();
	}


	return 0;
}
