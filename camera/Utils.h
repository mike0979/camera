/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: Utils.h
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-3-10
*  Description: brief
*  History: 
******************************************************************************/

#ifndef UTIL_H
#define UTIL_H

#include <QImage>
#include "opencv2/opencv.hpp"

namespace utilfunc
{
	// Mat转换成QImage
	void Mat2Image(const cv::Mat& src, QImage& dst);

	// QImage转换成Mat
	//void Image2Mat(const QImage &image, cv::Mat &mat);
}

#endif 
