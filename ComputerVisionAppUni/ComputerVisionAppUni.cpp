#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

bool compareArr(int a[], int b[], int l)
{
	int i, res = 0;
	for (i = 0; i < l; i++)
	{
		if (a[i] != b[i])
		{
			res = a[i] - b[i];
			break;
		}
	}
	if (res)
	{
		return false;
	}
	else
	{
		return true;
	}
}

////////////////////////////////////////////////////////////////
// main - initialises OpenCV and captures an image and changes it
////////////////////////////////////////////////////////////////
int main()
{
	CvCapture* capture = 0;

	// Declare variable
	int prevDit = 0;
	int i = 0;
	int j = 0;
	int binaryArr[100] = {};
	int lengthArr[100] = {};
	int barcodeBinaryArr[100] = {};
	int barcodeParityArr[6] = {};
	int barcodeArr[13] = {};
	int unitLength = 0;
	int currentBarcodeBinaryInt = 0;
	int currentBarcodeInt = 1;

	// Barcode database
	int p0[6] = { 1,1,1,1,1,1 };
	int p1[6] = { 1,1,0,1,0,0 };
	int p2[6] = { 1,1,0,0,1,0 };
	int p3[6] = { 1,1,0,0,0,1 };
	int p4[6] = { 1,0,1,1,0,0 };
	int p5[6] = { 1,0,0,1,1,0 };
	int p6[6] = { 1,0,0,0,1,1 };
	int p7[6] = { 1,0,1,0,1,0 };
	int p8[6] = { 1,0,1,0,0,1 };
	int p9[6] = { 1,0,0,1,0,1 };
	int o0[7] = { 0,0,0,1,1,0,1 };
	int o1[7] = { 0,0,1,1,0,0,1 };
	int o2[7] = { 0,0,1,0,0,1,1 };
	int o3[7] = { 0,1,1,1,1,0,1 };
	int o4[7] = { 0,1,0,0,0,1,1 };
	int o5[7] = { 0,1,1,0,0,0,1 };
	int o6[7] = { 0,1,0,1,1,1,1 };
	int o7[7] = { 0,1,1,1,0,1,1 };
	int o8[7] = { 0,1,1,0,1,1,1 };
	int o9[7] = { 0,0,0,1,0,1,1 };
	int e0[7] = { 0,1,0,0,1,1,1 };
	int e1[7] = { 0,1,1,0,0,1,1 };
	int e2[7] = { 0,0,1,1,0,1,1 };
	int e3[7] = { 0,1,0,0,0,0,1 };
	int e4[7] = { 0,0,1,1,1,0,1 };
	int e5[7] = { 0,1,1,1,0,0,1 };
	int e6[7] = { 0,0,0,0,1,0,1 };
	int e7[7] = { 0,0,1,0,0,0,1 };
	int e8[7] = { 0,0,0,1,0,0,1 };
	int e9[7] = { 0,0,1,0,1,1,1 };
	int r0[7] = { 1,1,1,0,0,1,0 };
	int r1[7] = { 1,1,0,0,1,1,0 };
	int r2[7] = { 1,1,0,1,1,0,0 };
	int r3[7] = { 1,0,0,0,0,1,0 };
	int r4[7] = { 1,0,1,1,1,0,0 };
	int r5[7] = { 1,0,0,1,1,1,0 };
	int r6[7] = { 1,0,1,0,0,0,0 };
	int r7[7] = { 1,0,0,0,1,0,0 };
	int r8[7] = { 1,0,0,1,0,0,0 };
	int r9[7] = { 1,1,1,0,1,0,0 };

	// Initializes capturing video from camera and Creates window
	Mat frame;
	VideoCapture cap;
	Size size;

	int deviceID = 0;
	int apiID = cv::CAP_ANY;
	cap.open(deviceID, apiID);

	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}

	cap.read(frame);
	size = frame.size();

	/*cvNamedWindow("Camera image", 1);
	IplImage* frame = 0;
	frame = cvQueryFrame(capture);*/

	// Print details of image
	cout << "image width =" << size.width << " height =" << size.height;
	//cout << " depth =" << frame->depth << " channels =" << frame->nChannels << endl;

	int mode = 1;
	do {
		// Grabs and returns a frame from camera
		cap.read(frame);

		// Convert image into grey scale
		int max = 0;
		int min = 255;
		for (int y = 0; y < size.height; y++)
		{
			for (int x = 0; x < size.width; x++)
			{
				uchar *row = frame.ptr<uchar>(y);

				int gray = (row[x * 3] + row[x * 3 + 1] + row[x * 3 + 2]) / 3;
				row[x * 3] = gray;
				row[x * 3 + 1] = gray;
				row[x * 3 + 2] = gray;

				if (gray > max){max = gray;}
				if (gray < min) {min = gray;}
			}
		}

		// Normalise image

		for (int y = 0; y < size.height; y++)
		{
			for (int x = 0; x < size.width; x++)
			{
				uchar *row = frame.ptr<uchar>(y);

				if (y == size.height / 2 - 1)
				{
					row[x * 3] = 0;
					row[x * 3 + 1] = 0;
					row[x * 3 + 2] = 255;
				}
				else
				{
					row[x * 3] = (row[x * 3] - min)*(255 - 0) / (max - min) + 0;
					row[x * 3 + 1] = (row[x * 3+1] - min)*(255 - 0) / (max - min) + 0;
					row[x * 3 + 2] = (row[x * 3+2] - min)*(255 - 0) / (max - min) + 0;
				}
			}
		}

		// Shows the resulting image in the window

		imshow("Normalised greyscale view", frame);
		
		// Turn Image into binary
		for (int y = 0; y < size.height; y++)
		{
			for (int x = 0; x < size.width; x++)
			{
				uchar *row = frame.ptr<uchar>(y);//(uchar*)(frame->imageData + frame->widthStep * y);

				//int gray = (row[x * 3] + row[x * 3 + 1] + row[x * 3 + 2]) / 3;
				if (row[x * 3] > 127)
				{
					row[x * 3] = 255;
					row[x * 3 + 1] = 255;
					row[x * 3 + 2] = 255;
				}
				else
				{
					row[x * 3] = 0;
					row[x * 3 + 1] = 0;
					row[x * 3 + 2] = 0;
				}

				if (y == size.height / 2 - 1)
				{
					row[x * 3] = 0;
					row[x * 3 + 1] = 0;
					row[x * 3 + 2] = 255;
				}
			}
		}

		// Shows the resulting image in the window
		imshow("Binary view", frame);

		// Turn image into lines
		for (int y = 0; y < size.height; y++)
		{
			for (int x = 0; x < size.width; x++)
			{
				uchar *row = frame.ptr<uchar>(y);//(uchar*)(frame->imageData + frame->widthStep * y);
				uchar *row_ = frame.ptr<uchar>(size.height / 2);//(uchar*)(frame->imageData + frame->widthStep * (size.height) / 2);

				row[x * 3] = row_[x * 3];
				row[x * 3 + 1] = row_[x * 3 + 1];
				row[x * 3 + 2] = row_[x * 3 + 2];
			}
		}

		//Clear variable
		prevDit = 0;
		i = 0;
		j = 0;
		unitLength = 0;
		currentBarcodeInt = 1;
		currentBarcodeBinaryInt = 0;
		for (int n = 0; n < 100; n++)
		{
			binaryArr[n] = 0;
			lengthArr[n] = 0;
			barcodeBinaryArr[n] = 0;
			barcodeArr[n] = 0;
		}

		// Build pixel array
		for (int x = 0; x < size.width; x++)
		{
			uchar *row = frame.ptr<uchar>(0);//(uchar*)(frame->imageData + frame->widthStep * 0);
			if ((int)row[x * 3] == 0)
			{
				if (prevDit == 0)
				{
					lengthArr[j] = i;
					j = j + 1;
					i = 0;
				}
				prevDit = 1;
				i++;
			}
			else
			{
				if (prevDit == 1)
				{
					lengthArr[j] = i;
					j = j + 1;
					i = 0;
				}
				prevDit = 0;
				i++;
			}
		}
		unitLength = (lengthArr[1] + lengthArr[2] + lengthArr[3] + lengthArr[j - 1] + lengthArr[j - 2] + lengthArr[j - 3]) / 6;


		// Build Barcode Array
		bool isBlack = true;
		for (int n = 1; n < j; n++)
		{
			int unitNum = 0;
			if (lengthArr[n] < unitLength*1.7)
			{
				unitNum = 1;
			}
			else if (lengthArr[n] >= unitLength * 1.7 && lengthArr[n] < unitLength*2.7)
			{
				unitNum = 2;
			}
			else if (lengthArr[n] >= unitLength * 2.7 && lengthArr[n] < unitLength*3.7)
			{
				unitNum = 3;
			}
			else if (lengthArr[n] > unitLength*3.7)
			{
				unitNum = 4;
			}
			for (int m = 0; m < unitNum; m++)
			{
				if (isBlack)
				{
					barcodeBinaryArr[currentBarcodeBinaryInt] = 1;
				}
				else
				{
					barcodeBinaryArr[currentBarcodeBinaryInt] = 0;
				}
				currentBarcodeBinaryInt++;
			}
			if (isBlack)
			{
				isBlack = false;
			}
			else
			{
				isBlack = true;
			}

		}

		int key = cv::waitKey(5);
		key = (key == 255) ? -1 : key;
		if (key == 49)
		{
			mode = 1;
		}
		else if (key == 50)
		{
			mode = 2;
		}

	} while ('q' != waitKey(10) && currentBarcodeBinaryInt != 95);

	// Decode Left Barcode
	currentBarcodeInt = 1;
	for (int n = 3; n < 45; n += 7)
	{
		int digitBinary[7] = {};
		for (int m = 0; m < 7; m++)
		{
			digitBinary[m] = barcodeBinaryArr[n + m];
		}
		if (compareArr(digitBinary, o0, 7))
		{
			barcodeArr[currentBarcodeInt] = 0;
			barcodeParityArr[currentBarcodeInt - 1] = 1;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, o1, 7))
		{
			barcodeArr[currentBarcodeInt] = 1;
			barcodeParityArr[currentBarcodeInt - 1] = 1;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, o2, 7))
		{
			barcodeArr[currentBarcodeInt] = 2;
			barcodeParityArr[currentBarcodeInt - 1] = 1;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, o3, 7))
		{
			barcodeArr[currentBarcodeInt] = 3;
			barcodeParityArr[currentBarcodeInt - 1] = 1;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, o4, 7))
		{
			barcodeArr[currentBarcodeInt] = 4;
			barcodeParityArr[currentBarcodeInt - 1] = 1;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, o5, 7))
		{
			barcodeArr[currentBarcodeInt] = 5;
			barcodeParityArr[currentBarcodeInt - 1] = 1;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, o6, 7))
		{
			barcodeArr[currentBarcodeInt] = 6;
			barcodeParityArr[currentBarcodeInt - 1] = 1;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, o7, 7))
		{
			barcodeArr[currentBarcodeInt] = 7;
			barcodeParityArr[currentBarcodeInt - 1] = 1;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, o8, 7))
		{
			barcodeArr[currentBarcodeInt] = 8;
			barcodeParityArr[currentBarcodeInt - 1] = 1;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, o9, 7))
		{
			barcodeArr[currentBarcodeInt] = 9;
			barcodeParityArr[currentBarcodeInt - 1] = 1;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, e0, 7))
		{
			barcodeArr[currentBarcodeInt] = 0;
			barcodeParityArr[currentBarcodeInt - 1] = 0;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, e1, 7))
		{
			barcodeArr[currentBarcodeInt] = 1;
			barcodeParityArr[currentBarcodeInt - 1] = 0;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, e2, 7))
		{
			barcodeArr[currentBarcodeInt] = 2;
			barcodeParityArr[currentBarcodeInt - 1] = 0;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, e3, 7))
		{
			barcodeArr[currentBarcodeInt] = 3;
			barcodeParityArr[currentBarcodeInt - 1] = 0;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, e4, 7))
		{
			barcodeArr[currentBarcodeInt] = 4;
			barcodeParityArr[currentBarcodeInt - 1] = 0;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, e5, 7))
		{
			barcodeArr[currentBarcodeInt] = 5;
			barcodeParityArr[currentBarcodeInt - 1] = 0;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, e6, 7))
		{
			barcodeArr[currentBarcodeInt] = 6;
			barcodeParityArr[currentBarcodeInt - 1] = 0;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, e7, 7))
		{
			barcodeArr[currentBarcodeInt] = 7;
			barcodeParityArr[currentBarcodeInt - 1] = 0;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, e8, 7))
		{
			barcodeArr[currentBarcodeInt] = 8;
			barcodeParityArr[currentBarcodeInt - 1] = 0;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, e9, 7))
		{
			barcodeArr[currentBarcodeInt] = 9;
			barcodeParityArr[currentBarcodeInt - 1] = 0;
			currentBarcodeInt++;
		}
	}

	// Decode First digit
	for (int a = 0 ; a < 6;a++)
	{
		cout << barcodeParityArr[a] << " ";
	}
	cout << endl;
	if (compareArr(barcodeParityArr, p0, 6))
	{
		barcodeArr[0] = 0;
	}
	else if (compareArr(barcodeParityArr, p1, 6))
	{
		barcodeArr[0] = 1;
	}
	else if (compareArr(barcodeParityArr, p2, 6))
	{
		barcodeArr[0] = 2;
	}
	else if (compareArr(barcodeParityArr, p3, 6))
	{
		barcodeArr[0] = 3;
	}
	else if (compareArr(barcodeParityArr, p4, 6))
	{
		barcodeArr[0] = 4;
	}
	else if (compareArr(barcodeParityArr, p5, 6))
	{
		barcodeArr[0] = 5;
	}
	else if (compareArr(barcodeParityArr, p6, 6))
	{
		barcodeArr[0] = 6;
	}
	else if (compareArr(barcodeParityArr, p7, 6))
	{
		barcodeArr[0] = 7;
	}
	else if (compareArr(barcodeParityArr, p8, 6))
	{
		barcodeArr[0] = 8;
	}
	else if (compareArr(barcodeParityArr, p9, 6))
	{
		barcodeArr[0] = 9;
	}

	// Decode Right Barcode
	for (int n = 50; n < 92; n += 7)
	{
		int digitBinary[7] = {};
		for (int m = 0; m < 7; m++)
		{
			digitBinary[m] = barcodeBinaryArr[n + m];
		}
		if (compareArr(digitBinary, r0, 7))
		{
			barcodeArr[currentBarcodeInt] = 0;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, r1, 7))
		{
			barcodeArr[currentBarcodeInt] = 1;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, r2, 7))
		{
			barcodeArr[currentBarcodeInt] = 2;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, r3, 7))
		{
			barcodeArr[currentBarcodeInt] = 3;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, r4, 7))
		{
			barcodeArr[currentBarcodeInt] = 4;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, r5, 7))
		{
			barcodeArr[currentBarcodeInt] = 5;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, r6, 7))
		{
			barcodeArr[currentBarcodeInt] = 6;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, r7, 7))
		{
			barcodeArr[currentBarcodeInt] = 7;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, r8, 7))
		{
			barcodeArr[currentBarcodeInt] = 8;
			currentBarcodeInt++;
		}
		else if (compareArr(digitBinary, r9, 7))
		{
			barcodeArr[currentBarcodeInt] = 9;
			currentBarcodeInt++;
		}
	}

	// Print data
	cout << "Unit Length: " << unitLength << endl;

	cout << endl << "Total length:" << j;
	cout << endl << "Scanned Barcode Binary Arr length:" << currentBarcodeBinaryInt;
	cout << endl << "Scanned Barcode Binary Arr:" << endl;
	for (int n = 0; n < 45; n++)
	{
		cout << barcodeBinaryArr[n] << " ";
	}
	cout << endl << "Barcode Parity:" << endl;
	for (int n = 0; n < 6; n++)
	{
		if (barcodeParityArr[n] == 0)
		{
			cout << "G" << " ";
		}
		else
		{
			cout << "L" << " ";
		}
	}
	cout << endl << endl << endl << "Barcode is:" << endl;
	for (int n = 0; n < 13; n++)
	{
		cout << barcodeArr[n] << " ";
	}


	// Print the Barcode
	while (true)
	{
		imshow("Camera image", frame);
		int key = cv::waitKey(5);
		key = (key == 255) ? -1 : key;
		if (key == 113)
		{
			break;
		}
	}
	cout << "end";
	//tidy up

	// Releases the CvCapture structure
	cap.release();
	// Destroys all the HighGUI windows
	//cvDestroyAllWindows();

	return 0;

} //end of main 