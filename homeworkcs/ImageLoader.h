#include<opencv\cv.h>
#include<opencv2\highgui.hpp>
#ifndef __CBMPLOADER_H__
#define __CBMPLOADER_H__


/** 位图载入类 */
class ImageLoader
{
public:
	ImageLoader();
	~ImageLoader();

	bool LoadImage(char *filename); /**< 装载一个bmp文件 */
	void FreeImage();                /**< 释放图像数据 */

	unsigned int ID;                 /**< 生成纹理的ID号 */
	int imageWidth;                  /**< 图像宽度 */
	int imageHeight;                 /**< 图像高度 */
	IplImage  *image;            /**< 指向图像数据的指针 */
};

#endif //__CBMPLOADER_H__