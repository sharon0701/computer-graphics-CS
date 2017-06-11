#include<opencv\cv.h>
#include<opencv2\highgui.hpp>
#ifndef __CBMPLOADER_H__
#define __CBMPLOADER_H__


/** λͼ������ */
class ImageLoader
{
public:
	ImageLoader();
	~ImageLoader();

	bool LoadImage(char *filename); /**< װ��һ��bmp�ļ� */
	void FreeImage();                /**< �ͷ�ͼ������ */

	unsigned int ID;                 /**< ���������ID�� */
	int imageWidth;                  /**< ͼ���� */
	int imageHeight;                 /**< ͼ��߶� */
	IplImage  *image;            /**< ָ��ͼ�����ݵ�ָ�� */
};

#endif //__CBMPLOADER_H__