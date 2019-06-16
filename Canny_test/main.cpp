#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

/*************Variable*************/
//原图 灰度图 目标图
Mat g_srcImage, g_srcGrayImage, g_dstImage;

//Canny
Mat g_cannyDetectedEdges;
int g_cannyLowThreshold = 1; //TrackBar

//Sobel
Mat g_sobelGradient_X, g_sobelGradient_Y;
Mat g_sobelAbsGradient_X, g_sobelAbsGradient_Y;
int g_sobelKernelSize = 1; //TrackBar

//Scharr
Mat g_scharrGradient_X, g_scharrGradient_Y;
Mat g_scharrAbsGradient_X, g_scharrAbsGradient_Y;

/*************Function*************/
static void on_Canny(int, void*); //Canny callback function
static void on_Sobel(int, void*); //Sobel callback function
void Scharr(); //Scharr Function

int main()
{
    g_srcImage = imread("../Images/church01.jpg");
    if(!g_srcImage.data){
        cout << "Failed to read the image!" << endl;
        return false;
    }

    namedWindow("srcImage");
    imshow("srcImage", g_srcImage);

    g_dstImage.create(g_srcImage.size(), g_srcImage.type());

    cvtColor(g_srcImage, g_srcGrayImage, COLOR_BGR2GRAY);

    namedWindow("CannyDetection", WINDOW_KEEPRATIO);
    namedWindow("SobelDetection", WINDOW_KEEPRATIO);
    namedWindow("ScharrDetection", WINDOW_KEEPRATIO);

    createTrackbar("Param:", "CannyDetection", &g_cannyLowThreshold, 120, on_Canny);
    createTrackbar("Param:", "SobelDetection", &g_sobelKernelSize, 3, on_Sobel);

    on_Canny(0, 0);
    on_Sobel(0, 0);

    Scharr();

    waitKey();

    return 0;
}

//Canny
void on_Canny(int, void*)
{
    blur(g_srcGrayImage, g_cannyDetectedEdges, Size(3, 3));

    Canny(g_cannyDetectedEdges, g_cannyDetectedEdges, g_cannyLowThreshold, g_cannyLowThreshold * 3, 3);

    g_dstImage = Scalar::all(0);

    g_srcImage.copyTo(g_dstImage, g_cannyDetectedEdges);

    imshow("CannyDetection", g_dstImage);
}

//Sobel
void on_Sobel(int, void*)
{
    Sobel(g_srcImage, g_sobelGradient_X, CV_16S, 1, 0, (2 * g_sobelKernelSize + 1), 1, 1, BORDER_DEFAULT);
    convertScaleAbs(g_sobelGradient_X, g_sobelAbsGradient_X);

    Sobel(g_srcImage, g_sobelGradient_Y, CV_16S, 0, 1, (2 * g_sobelKernelSize + 1), 1, 1, BORDER_DEFAULT);
    convertScaleAbs(g_sobelGradient_Y, g_sobelAbsGradient_Y);

    addWeighted(g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, g_dstImage);

    imshow("SobelDetection", g_dstImage);
}

//Scharr
void Scharr()
{
    Scharr(g_srcImage, g_scharrGradient_X, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(g_scharrGradient_X, g_scharrAbsGradient_X);

    Scharr(g_srcImage, g_scharrGradient_Y, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(g_scharrGradient_Y, g_scharrAbsGradient_Y);

    addWeighted(g_scharrAbsGradient_X, 0.5, g_scharrAbsGradient_Y, 0.5, 0, g_dstImage);

    imshow("ScharrDetection", g_dstImage);
}
