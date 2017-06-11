#include "stdafx.h"
#include"ImageLoader.h"              /**< 包含头文件 */
#include <iostream>
using namespace std;
/** 构造函数 */
ImageLoader::ImageLoader()
{
	/** 初始化成员值为0 */
	image = NULL;
	imageWidth = 0;
	imageHeight = 0;
}

/** 析构函数 */
ImageLoader::~ImageLoader()
{
	FreeImage(); /**< 释放图像数据占据的内存 */
}

/** 装载一个位图文件 */
bool ImageLoader::LoadImage(char *file)
{
	IplImage * temp = cvLoadImage(file);
	if (temp == NULL) {
		cout << "Load image file: "<<file<<" failed!" << endl;
		return false;
	}
	image = cvCreateImage(cvSize(temp->width, temp->height), 8, 3);

	cvConvertImage(temp, image, CV_CVTIMG_SWAP_RB);
	cvFlip(image, NULL, -1);
	cvReleaseImage(&temp);
	return true;         /**< 成功返回 */
}

/** 释放内存 */
void ImageLoader::FreeImage()
{
	/** 释放分配的内存 */
	if (image)
	{
		cvReleaseImage(&image);
	}
}