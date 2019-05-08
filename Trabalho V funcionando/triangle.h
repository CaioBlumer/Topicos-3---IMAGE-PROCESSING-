#ifndef TRIANGLE_H
#define TRIANGLE_H

//#include"pch.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>



void detectTriangle(cv::VideoCapture& cap, cv::VideoWriter& video);
int calculateAngle(cv::Point p0, cv::Point p1, cv::Point p2);
int triangleArea(cv::Point p0, cv::Point p1, cv::Point p2);

void detectTriangle(cv::VideoCapture& cap, cv::VideoWriter& video)
{
	bool endProgram = false; //Control end of program
	bool saveVideo = true; //Wether or not to save video
	int font = cv::FONT_HERSHEY_TRIPLEX; //Font to write text on image

	std::vector<cv::Point> letterPos = { cv::Point(-1,-1), cv::Point(-1,-1) , cv::Point(-1,-1) }; //Control continuity of sides - avoid sides changing names
	std::vector<std::string> letter = { "A", "B", "C", "Error" }; //Name of sides

	std::clock_t now, then; //Used to calculate fps
	now  = std::clock();
	then = now;

	cv::namedWindow("binary", cv::WINDOW_KEEPRATIO);
	cv::namedWindow("canny", cv::WINDOW_KEEPRATIO);
	cv::namedWindow("out", cv::WINDOW_KEEPRATIO);
	//Render Loop
	while (!endProgram)
	{
		cv::Mat frame, canny;
		cap >> frame;
		cv::Mat out = frame.clone();

		//Binarizing image
		cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
		cv::threshold(frame, frame, 60, 255, cv::THRESH_BINARY);

		//Finding contourns
		std::vector< std::vector < cv::Point > > conts; //Contourns
		cv::Canny(frame, canny, 50, 200, 3);
		cv::findContours(canny, conts, 0, 2);

		int nObjects = 0;
		std::vector<cv::Point> center(3); //Vector used to store the center of the circles (triangle vertices)

		//Selecting circle contourns
		for (int i = 0; i < conts.size(); i++)
		{
			if (conts.at(i).size() > 50)
			{
				double area = cv::contourArea(conts.at(i));
				cv::RotatedRect bRect = cv::minAreaRect(conts.at(i));
				double extent = area / bRect.size.area();

				//Filtering using extent
				//for a circle or elipse: extent = pi/4
				if (abs(extent*1.27323954474 - 1) < 0.04) //1.27323954474 ~ 4/pi, maximum error of 4%
				{
					cv::drawContours(out, conts, i, cv::Scalar(0, 0, 255), 2, 5);
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
					if (nObjects < 3)
						center.at(nObjects++) = cv::Point((int)xacum, (int)yacum);
				}
			}
		}

		//Drawing Lines and writing labels
		if (nObjects > 1)
		{
			int usedLetters[3] = { -1, -1, -1 };
			for (int i = 0; i < nObjects; i++)
			{
				cv::Point lineCenter = (center.at(i) + center.at((i + 1) % nObjects)) / 2;
				cv::line(out, center.at(i), center.at((i + 1) % nObjects), cv::Scalar(0, 255, 0), 2);

				//Keeping track of letter positions
				int minDist = 9999;
				int index = 3;
				for (int j = 0; j < 3; j++)
				{
					if (j == usedLetters[0] || j == usedLetters[1]) continue;
					int distance = abs(lineCenter.x - letterPos.at(j).x) + abs(lineCenter.y - letterPos.at(j).y);
					if (distance < minDist)
					{
						minDist = distance;
						index = j;
					}
				}
				letterPos.at(index) = lineCenter;
				usedLetters[i] = index;

				if (nObjects == 2 && i == 0)
					cv::putText(out, letter.at(index), letterPos.at(index), font, 3, cv::Scalar(255, 255, 255), 3);
				else if (nObjects == 3)
				{
					cv::putText(out, letter.at(index), letterPos.at(index), font, 3, cv::Scalar(255, 255, 255), 3);
					//Calculating angle and printing
					std::stringstream s;
					s << calculateAngle(center.at(i), center.at((i + 1) % nObjects), center.at((i + 2) % nObjects));
					std::string angle = s.str();
					cv::putText(out, angle, center.at(i), font, 3, cv::Scalar(255, 255, 255), 3);
				}
			}
		}

		//Writing name, area of triangle and fps
		std::string name = "Caio Blumer";

		std::string area = "Area of triangle = ";
		std::stringstream temp1;
		if (nObjects == 3)	temp1 << triangleArea(center.at(0), center.at(1), center.at(2));
		else temp1 << 0;
		area += temp1.str();

		std::string fps = "FPS = ";
		std::stringstream temp2;
		now = std::clock();
		temp2 << CLOCKS_PER_SEC / (now - then);
		then = now;
		fps += temp2.str();

		cv::putText(out, name, cv::Point(0, 30), font, 1.5, cv::Scalar(0, 0, 0), 1);
		cv::putText(out, area, cv::Point(0, 60), font, 1.5, cv::Scalar(0, 0, 0), 1);
		cv::putText(out, fps , cv::Point(0, 90), font, 1.5, cv::Scalar(0, 0, 0), 1);

		//Showing images
		imshow("binary", frame);
		imshow("canny", canny);
		imshow("out", out);
		if (saveVideo) video.write(out);
		if (cv::waitKey(1) == 27) endProgram = true;
	}
}

int calculateAngle(cv::Point p0, cv::Point p1, cv::Point p2)
{
	//Angulo calculado pela relacao:
	//				 <p0-p1, p0-p2>
	//  cos(angle) = ---------------
	//               |p0-p1|*|p0-p2|
	cv::Point p0p1, p0p2;
	p0p1 = p0 - p1;
	p0p2 = p0 - p2;
	double a = p0p1.dot(p0p2); // <p0p1, p0p2>  produto escalar
	double b = p0p1.dot(p0p1); // |p0p1|^2 magnitude do vetor ao quadrado
	double c = p0p2.dot(p0p2); // |p0p2|^2 magnitude do vetor ao quadrado
	double angle = (57.295779)*acos(a / sqrt(b*c)); // 57.295779 ~ 180/p, usado para conversao de radianos para graus
	return (int)round(angle);
}

int triangleArea(cv::Point p0, cv::Point p1, cv::Point p2)
{
	//Calculate area using shoelace formula
	//A = 1/2(|x0y1 + x1y2 + x2y0 - x1y0 -x2y1 -x0y2|)
	return abs(p0.x*p1.y + p1.x*p2.y + p2.x*p0.y - p1.x*p0.y - p2.x*p1.y - p0.x*p2.y)/2;
}

#endif // !TRIANGLE_H
