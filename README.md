# Barcode-Reader-on-labtop-webcam

## Introduction
This is a C++ program which read barcode with laptop camera. It capture image in real time with opencv and detect any barcode that lies on the red line. Once the barcode occupy the entire screen horizontally, the program immediatly react. Then the program will analyse the binary pixel patterns and display the result on console window.

## Dependencies
- C++17
- Opencv 4.5.1

## Result

Captured image in greyscale

![alt text](https://github.com/jason2468087/Barcode-Reader/blob/master/README%20Asset/Barcode%20Detector%20greyscale%20capture.png?raw=true)

From greyscale to binary image

![alt text](https://github.com/jason2468087/Barcode-Reader/blob/master/README%20Asset/Barcode%20Detector%20binary%20capture.png?raw=true)

Captured barcode pattern

![alt text](https://github.com/jason2468087/Barcode-Reader/blob/master/README%20Asset/Barcode%20Detector%20barcode%20result.png?raw=true)

Final result

![alt text](https://github.com/jason2468087/Barcode-Reader/blob/master/README%20Asset/Barcode%20Detector%20console%20result.png?raw=true)

## Skills
- Barcode Algorithms
- RGB to greyscale
- Greyscale to binary
