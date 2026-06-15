// This code is written at BigVision LLC. It is based on the OpenCV project. It is subject to the license terms in the LICENSE file found in this distribution and at http://opencv.org/license.html

// Usage example:  ./mask_rcnn.out --video=run.mp4
//                 ./mask_rcnn.out --image=bird.jpg
#include  "stdafx.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include  "mask_rcnn.h"
#include  "ProjectPaths.h"

//const char* keys =
//"{help h usage ? | | Usage examples: \n\t\t./mask-rcnn.out --image=traffic.jpg \n\t\t./mask-rcnn.out --video=sample.mp4}"
//"{image i        |<none>| input image   }"
//"{video v       |<none>| input video   }"
//;
using namespace cv;
using namespace dnn;
using namespace std;

// Initialize the parameters
float confThreshold = 0.5; // Confidence threshold
float maskThreshold = 0.3; // Mask threshold

vector<string> classes;
vector<Scalar> colors;

// Draw the predicted bounding box
void drawBox(Mat& frame, int classId, float conf, Rect box, Mat& objectMask);

// Postprocess the neural network's output for each frame
void postprocess(Mat& frame, const vector<Mat>& outs);

int  net_init__maskRcnn(Net &  net    )
{
	if ( 1  )  
	{
		cout << "Use this script to run object detection using YOLO3 in OpenCV. " << endl;
		std::wstring classesPath = projectpaths::ResolveModelFile(L"mscoco_labels.names");
		string classesFile = projectpaths::ToUtf8(classesPath);
		ifstream ifs(classesFile.c_str());
		string line;
		while (getline(ifs, line))
			classes.push_back(line);

		// Load the colors
		std::wstring colorsPath = projectpaths::ResolveModelFile(L"colors.txt");
		string colorsFile = projectpaths::ToUtf8(colorsPath);
		ifstream colorFptr(colorsFile.c_str());
		while (getline(colorFptr, line))
		{
			char* pEnd;
			double r, g, b;
			r = strtod(line.c_str(), &pEnd);
			g = strtod(pEnd, NULL);
			b = strtod(pEnd, NULL);
			Scalar color = Scalar(r, g, b, 255.0);
			colors.push_back(Scalar(r, g, b, 255.0));
		}

		// Give the configuration and weight files for the model
		std::wstring pbtxtPath = projectpaths::ResolveModelFile(L"mask_rcnn_inception_v2_coco_2018_01_28.pbtxt");
		std::wstring weightsPath = projectpaths::ResolveModelFile(L"mask_rcnn_inception_v2_coco_2018_01_28/frozen_inference_graph.pb");
		String textGraph = projectpaths::ToUtf8(pbtxtPath);
		String modelWeights = projectpaths::ToUtf8(weightsPath);

		// Load the network
		net = readNetFromTensorflow(modelWeights, textGraph);
		net.setPreferableBackend(DNN_BACKEND_OPENCV);
		//net.setPreferableTarget(DNN_TARGET_CPU);
		net.setPreferableTarget(DNN_TARGET_OPENCL);
		cout << "finish  net_init__maskRcnn(Net &  net  ) .  " << endl;
	}


	return 0;
}


int   procImage__maskRcnn(Mat   frame, Net  net, int  id)
{
	Mat   blob;
	// Stop the program if reached end of video
	if (frame.empty()) {
		cout << "Done processing !!!frame.empty()" << endl;
		waitKey(3000);
		return  -1;
	}
	// Create a 4D blob from a frame.
	blobFromImage(frame, blob, 1.0, Size(frame.cols, frame.rows), Scalar(), true, false);

	//Sets the input to the network
	net.setInput(blob);

	// Runs the forward pass to get output from the output layers
	std::vector<String> outNames(2);
	outNames[0] = "detection_out_final";
	outNames[1] = "detection_masks";
	vector<Mat> outs_detection_maskrcnn;
	net.forward(outs_detection_maskrcnn, outNames);

	cout << "outs_detection_maskrcnn.size() = " << outs_detection_maskrcnn.size() << endl;
	cout << "outs_detection_maskrcnn[0].size() = " << outs_detection_maskrcnn[0].size() << endl;
	cout << "outs_detection_maskrcnn[0].channels() = " << outs_detection_maskrcnn[0].channels() << endl;
	cout << "outs_detection_maskrcnn[1].size() = " << outs_detection_maskrcnn[1].size() << endl;
	cout << "outs_detection_maskrcnn[1].channels() = " << outs_detection_maskrcnn[1].channels() << endl;
	//Mat show_1;
	//outs_detection_maskrcnn[1].convertTo(show_1,  CV_32FC1   );
	//cout << "outs_detection_maskrcnn[1] = " << show_1  << endl;

	// storage 
	FileStorage fs__outs_detection_maskrcnn("outs_detection_maskrcnn.xml", FileStorage::WRITE);
	fs__outs_detection_maskrcnn << "outs_detection_maskrcnn" << outs_detection_maskrcnn;
	fs__outs_detection_maskrcnn.release();


	// storage 
	Mat outs_detection_maskrcnn_1 = outs_detection_maskrcnn[1];
	FileStorage fs__outs_detection_maskrcnn_1("outs_detection_maskrcnn_1.yml", FileStorage::WRITE);
	fs__outs_detection_maskrcnn_1 << "outs_detection_maskrcnn_1" << outs_detection_maskrcnn_1;
	fs__outs_detection_maskrcnn_1.release();


	imwrite( "frame_before_postProcess__maskrcnn.jpg", frame   );
	// Extract the bounding box and mask for each of the detected objects
	postprocess(frame, outs_detection_maskrcnn);
	imwrite("frame_after_postProcess__maskrcnn.jpg", frame);




	// Put efficiency information. The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
	vector<double> layersTimes;
	double freq = getTickFrequency() / 1000;
	double t = net.getPerfProfile(layersTimes) / freq;
	cout << "Inference time for a frame : "<<  t<<  " ms" <<   endl;


	//string label = format("Mask-RCNN on 2.5 GHz Intel Core i7 CPU, Inference time for a frame : %0.0f ms", t);
	string label = format("Inference time for a frame : %0.0f ms", t);
	//putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
	putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));
	putText(frame, string("id=") + to_string(id), Point(60, 75), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255));

	// Write the frame with the detection boxes
	Mat detectedFrame;
	frame.convertTo(detectedFrame, CV_8U);
	imwrite("detectedFrame_maskrcnn_finalfinal.jpg", detectedFrame);

	imshow("detectedFrame_maskrcnn_finalfinal.jpg", frame);
	waitKey(20);
	frame.setTo(0);
	cout << "finish   procImage__maskRcnn(Mat   frame, Net  net   ) .  " << endl;
	return  0;
}

//maskrcnn   proc  image  
int recoImageSingle_maskRcnn_image_locate(Mat  imageInput)
{
	Net  net;
	cout << "start  net_init__maskRcnn(net   ) " << endl;
	net_init__maskRcnn(net   );
	cout << "finish  net_init__maskRcnn(net   ) " << endl;

	cout << "start  procImage__maskRcnn(imageInput, net, 0);" << endl;
	procImage__maskRcnn(imageInput, net, 0);
	cout << "finish   procImage__maskRcnn(imageInput, net, 0);  " << endl;
	return 0;
}





// For each frame, extract the bounding box and mask for each detected object
void postprocess(Mat& frame, const vector<Mat>& outs_detection_maskrcnn  )   //   outs_detection_maskrcnn
{
	//???90??
	Mat outDetections = outs_detection_maskrcnn[0];     //   1 x 1 x N x 7  ; ????????  7????????
	Mat outMasks = outs_detection_maskrcnn[1];     //    100x  90 x  15  x  15 ; ???????mask????????????????????,?????????????????
	

	// Output size of masks is NxCxHxW where
	// N - number of detected boxes
	// C - number of classes (excluding background)
	// HxW - segmentation shape
	const int numDetections = outDetections.size[2];
	const int numClasses = outMasks.size[1];

	//cout << "" << outDetections.total() << endl;
	outDetections = outDetections.reshape(1, outDetections.total() / 7);

	/*
	outs_detection_maskrcnn[1].channels() = 1
	postprocess:: outDetections = 
   [0, 73, 0.62331617, 0.771658,   0.077294856,   0.9984321,  0.18568596;
	0, 83, 0.61009222, 0.1028232,  0.060688347,   0.67229217, 0.53900695;
	0, 83, 0.36619449, 0.35507858, 0.089279458,   0.81348944, 0.51844501;
	0, 75, 0.3528243,  0.50599712, 0,             0.89858884, 0.091171123]
	 **/
	cout << "postprocess:: outDetections = " << outDetections << endl;

	for (int i = 0; i < numDetections; ++i)
	{
		float score = outDetections.at<float>(i, 2);
		if (score > confThreshold)
		{
			// Extract the bounding box
			int classId = static_cast<int>(outDetections.at<float>(i, 1));
			int left = static_cast<int>(frame.cols * outDetections.at<float>(i, 3));
			int top  = static_cast<int>(frame.rows * outDetections.at<float>(i, 4));
			int right  = static_cast<int>(frame.cols * outDetections.at<float>(i, 5));
			int bottom = static_cast<int>(frame.rows * outDetections.at<float>(i, 6));

			left = max(0, min(left, frame.cols - 1));
			top = max(0, min(top, frame.rows - 1));
			right = max(0, min(right, frame.cols - 1));
			bottom = max(0, min(bottom, frame.rows - 1));
			Rect box = Rect(left, top, right - left + 1, bottom - top + 1);

			// Extract the mask for the object
			Mat objectMask(outMasks.size[2], outMasks.size[3], CV_32F, outMasks.ptr<float>(i, classId));

			// Draw bounding box, colorize and show the mask on the image
			drawBox(frame, classId, score, box, objectMask);
		}
	}
}




// Draw the predicted bounding box, colorize and show the mask on the image
void drawBox(Mat& frame, int classId, float conf, Rect box, Mat& objectMask)
{
	//Draw a rectangle displaying the bounding box
	rectangle(frame, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), Scalar(255, 178, 50), 3);

	//Get the label for the class name and its confidence
	string label = format("%.2f", conf);
	if (!classes.empty())
	{
		CV_Assert(classId < (int)classes.size());
		label = classes[classId] + ":" + label;
	}

	//Display the label at the top of the bounding box
	int baseLine;
	Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
	box.y = max(box.y, labelSize.height);
	rectangle(frame, Point(box.x, box.y - round(1.5*labelSize.height)), Point(box.x + round(1.5*labelSize.width), box.y + baseLine), Scalar(255, 255, 255), FILLED);
	putText(frame, label, Point(box.x, box.y), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 1);

	Scalar color = colors[classId%colors.size()];

	// Resize the mask, threshold, color and apply it on the image
	resize(objectMask, objectMask, Size(box.width, box.height));
	Mat mask = (objectMask > maskThreshold);
	Mat coloredRoi = (0.3 * color + 0.7 * frame(box));
	coloredRoi.convertTo(coloredRoi, CV_8UC3);

	// Draw the contours on the image
	vector<Mat> contours;
	Mat hierarchy;
	mask.convertTo(mask, CV_8U);
	findContours(mask, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	drawContours(coloredRoi, contours, -1, color, 5, LINE_8, hierarchy, 100);
	coloredRoi.copyTo(frame(box), mask);
}




