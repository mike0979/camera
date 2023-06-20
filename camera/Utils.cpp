/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: Util.cpp
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-3-10
*  Description: brief
*  History: 
******************************************************************************/

#include "Utils.h"

void utilfunc::Mat2Image(const cv::Mat& src, QImage& dst)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (src.type() == CV_8UC1)
	{
		QImage img(src.cols, src.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		img.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			img.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar *pSrc = src.data;
		for (int row = 0; row < src.rows; row++)
		{
			uchar *pDest = img.scanLine(row);
			memcpy(pDest, pSrc, src.cols);
			pSrc += src.step;
		}
		dst.swap(img);
	}
	else if (src.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar *pSrc = (const uchar*)src.data;
		// Create QImage with same dimensions as input Mat
		dst = QImage(pSrc, src.cols, src.rows, src.step, QImage::Format_RGB888).copy();
	}
	else if (src.type() == CV_8UC4)
	{
		// Copy input Mat
		const uchar *pSrc = (const uchar*)src.data;
		// Create QImage with same dimensions as input Mat
		dst = QImage(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32).copy();
	}
	else
	{
		dst = QImage();
	}
}

//void utilfunc::Image2Mat(const QImage &src, cv::Mat &dst)
//{
//	switch (src.format())
//	{
//	case QImage::Format_ARGB32:
//	case QImage::Format_RGB32:
//	case QImage::Format_ARGB32_Premultiplied:
//		dst = cv::Mat(src.height(), src.width(), CV_8UC4, (void*)src.constBits(), src.bytesPerLine());
//		break;
//	case QImage::Format_RGB888:
//		dst = cv::Mat(src.height(), src.width(), CV_8UC3, (void*)src.constBits(), src.bytesPerLine());
//		break;
//	case QImage::Format_Indexed8:
//		dst = cv::Mat(src.height(), src.width(), CV_8UC1, (void*)src.constBits(), src.bytesPerLine());
//		break;
//	default:
//		break;
//	}
//}