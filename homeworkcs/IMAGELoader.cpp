#include "stdafx.h"
#include"ImageLoader.h"              /**< ����ͷ�ļ� */
#include <iostream>
using namespace std;
/** ���캯�� */
ImageLoader::ImageLoader()
{
	/** ��ʼ����ԱֵΪ0 */
	image = NULL;
	imageWidth = 0;
	imageHeight = 0;
}

/** �������� */
ImageLoader::~ImageLoader()
{
	FreeImage(); /**< �ͷ�ͼ������ռ�ݵ��ڴ� */
}

/** װ��һ��λͼ�ļ� */
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
	return true;         /**< �ɹ����� */
}

/** �ͷ��ڴ� */
void ImageLoader::FreeImage()
{
	/** �ͷŷ�����ڴ� */
	if (image)
	{
		cvReleaseImage(&image);
	}
}