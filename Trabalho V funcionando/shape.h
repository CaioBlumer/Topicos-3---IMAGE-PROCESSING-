#ifndef SHAPE_H
#define SHAPE_H

//#include"pch.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>


void getSignature(std::string filename, std::vector<int>& xSig, std::vector<int>& ySig, cv::Size sigSize);
void getSignature(cv::Mat& image, std::vector<cv::Point>& cont, std::vector<int>& xSig, std::vector<int>& ySig, cv::Size sigSize);
float sameSignature(std::vector<int>& xSig1, std::vector<int>& ySig1, std::vector<int>& xSig2, std::vector<int>& ySig2, float thres);

void detectShape(cv::VideoCapture& cap, cv::VideoWriter& video)
{
	bool endProgram = false; //Control end of program
	bool saveVideo = video.isOpened(); //Wether or not to save video
	int font = cv::FONT_HERSHEY_TRIPLEX; //Font to write text on image

	//Loading signatures of base images
	cv::Size sigSize = cv::Size(60,60); // Size to convert all signatures
	std::vector<int> objxSig[4]; //x-signature of base images
	std::vector<int> objySig[4]; //y-signature of base images
	std::string filename[4] = {
		"Circle.png",
		"Triangle.png",
		"A.png",
		"B.png"
	};
	for (int i = 0; i < 4; i++)	getSignature(filename[i], objxSig[i], objySig[i], sigSize);
	//-------------------------------------

	std::clock_t now, then; //Used to calculate fps
	now = std::clock();
	then = now;

	cv::namedWindow("out", cv::WINDOW_KEEPRATIO);
	//Render Loop
	while (!endProgram)
	{
		cv::Mat frame, out, canny;
		cap >> frame;
		out = frame.clone();

		//Binarizing image
		cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
		cv::GaussianBlur(frame, frame, cv::Size(5, 5), 0.0, 0.0);
		cv::threshold(frame, frame, 60, 255, cv::THRESH_BINARY);

		//Finding contourns
		std::vector< std::vector < cv::Point > > conts;
		cv::Canny(frame, canny, 10, 10, 3);
		cv::findContours(canny, conts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);


		for (int i = 0; i < conts.size(); i++)
		{
			if (conts.at(i).size() > 150)
			{
				//Selecting contourns with the right shape
				char type[5] = { 'S', 'C', 'T', 'A', 'B' }; //'S' - Square, 'C' - Circle, 'T' - triangle, 'A' - letter A, 'B', - letter B
				int typeNumber;
				cv::RotatedRect bRect = cv::minAreaRect(conts.at(i));
				double area = cv::contourArea(conts.at(i));
				double extent = area / bRect.size.area();
				if (std::abs(extent - 1) < 0.08) typeNumber = 0; //Selects square based on extent
				else if (std::abs(extent < 0.4)) typeNumber = -1; //Used to avoid unwanted, very long and thin contourns
				else
				{
					//Getting signatures of contourn
					std::vector<int> xSig, ySig;
					getSignature(frame, conts.at(i), xSig, ySig, sigSize);

					//Comparing signatures with the ones from the base images and selecting the one that aproximates better
					float minError = 20.0f;
					int minIndex = -1;
					for (int j = 0; j < 4; j++)
					{
						float thres;
						if (j == 0) thres = 15.0f;
						else if (j == 1) thres = 28.0f;
						else if (j == 2) thres = 15.0f;
						else if (j == 3) thres = 40.0f;
						float error = sameSignature(objxSig[j], objySig[j], xSig, ySig, thres);
						if (error != -1.0f) //If the signature is too different from the base one
						{
							if (error < minError)
							{
								minError = error;
								minIndex = j + 1;
							}
						}
					}
					typeNumber = minIndex;
				}

				//Drawing the right countorns
				if (typeNumber != -1)
				{
					//geometrical center of mass
					float xacum, yacum;
					xacum = yacum = 0;
					for (int k = 0; k < conts.at(i).size(); k++)
					{
						xacum += conts.at(i).at(k).x;
						yacum += conts.at(i).at(k).y;
					}
					xacum /= conts.at(i).size();
					yacum /= conts.at(i).size();
					cv::circle(out, cv::Point((int)xacum, (int)yacum), 5, cv::Scalar(255, 0, 0), 2); //draw center of mass
					cv::drawContours(out, conts, i, cv::Scalar(0, 0, 255), 2); //draw contourn
					std::string leter = std::string(1, type[typeNumber]);
					cv::putText(out, leter, cv::Point((int)xacum, (int)yacum), font, 3, cv::Scalar(255, 255, 255), 2); //Writing shape
				}
			}
		}

		//Writing name and fps
		std::string name = "Caio Blumer";

		std::string fps = "FPS = ";
		std::stringstream temp;
		now = std::clock();
		temp << CLOCKS_PER_SEC / (now - then);
		then = now;
		fps += temp.str();


		cv::putText(out, name, cv::Point(0, 40), font, 1.5, cv::Scalar(0, 0, 0), 1);
			cv::putText(out, fps, cv::Point(0, 80), font, 1.5, cv::Scalar(0, 0, 0), 1);


		//Showing images
		imshow("binary", frame);
		imshow("canny", canny);
		imshow("out", out);
		if (saveVideo) video.write(out);
		if (cv::waitKey(1) == 27) endProgram = true;
	}
}

void getSignature(std::string filename, std::vector<int>& xSig, std::vector<int>& ySig, cv::Size sigSize)
{
	cv::Mat image = cv::imread(filename);
	if (image.empty()) return;

	//Binarizing image
	cv::Mat temp = image.clone();
	cv::cvtColor(temp, temp, cv::COLOR_BGR2GRAY);
	cv::threshold(temp, temp, 50, 255, cv::THRESH_BINARY);

	//Finding contourn
	std::vector< std::vector < cv::Point > > conts;
	cv::Canny(temp, temp, 50, 200, 3);
	cv::findContours(temp, conts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	//Getting sub image based on the bounding rectangle of the contourn
	cv::Rect bRect = cv::boundingRect(conts.back());
	cv::Mat subImg = image(bRect);
	cv::resize(subImg, subImg, sigSize);
	xSig.resize(subImg.rows);
	ySig.resize(subImg.cols);

	//calculating x-signature
	for (int i = 0; i < subImg.rows; i++)
	{
		xSig.at(i) = 0;
		for (int j = 0; j < subImg.cols; j++)
			if (subImg.at<cv::Vec3b>(i, j)[0] < 50) xSig.at(i)++;
	}
	//calculating y-signature
	for (int j = 0; j < subImg.cols; j++)
	{
		ySig.at(j) = 0;
		for (int i = 0; i < subImg.rows; i++)
			if (subImg.at<cv::Vec3b>(i, j)[0] < 50) ySig.at(j)++;
	}
}

void getSignature(cv::Mat& image, std::vector<cv::Point>& cont, std::vector<int>& xSig, std::vector<int>& ySig, cv::Size sigSize)
{
	//Getting sub image based on the bounding rectangle of the contourn
	cv::Rect bRect = cv::boundingRect(cont);
	cv::Mat subImg = image(bRect);
	cv::resize(subImg, subImg, sigSize);
	xSig.resize(subImg.rows);
	ySig.resize(subImg.cols);

	//calculating x-signature
	for (int i = 0; i < subImg.rows; i++)
	{
		xSig.at(i) = 0;
		for (int j = 0; j < subImg.cols; j++)
			if (subImg.at<cv::uint8_t>(i, j) < 50) xSig.at(i)++;
	}

	//calculating y-signature
	for (int j = 0; j < subImg.cols; j++)
	{
		ySig.at(j) = 0;
		for (int i = 0; i < subImg.rows; i++)
			if (subImg.at<cv::uint8_t>(i, j) < 50) ySig.at(j)++;
	}
}

//Compare if the signatures of two contourns are the same based on a threshold value
float sameSignature(std::vector<int>& xSig1, std::vector<int>& ySig1, std::vector<int>& xSig2, std::vector<int>& ySig2, float thres)
{
	int dx = 0;
	int dy = 0;
	for (unsigned int i = 0; i < xSig1.size(); i++) dx += abs(xSig1[i] - xSig2[i]);
	for (unsigned int j = 0; j < ySig1.size(); j++) dy += abs(ySig1[j] - ySig2[j]);
	float error = (float)dx / xSig1.size() + (float)dy / ySig1.size();
	if (error < thres) return error; //same contourn
	return -1.0f; //not the same contourn
}

#endif // !SHAPE_H
