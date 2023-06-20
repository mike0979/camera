/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: CollectThread.h
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-6-15
*  Description: brief
*  History: 
******************************************************************************/

#ifndef COLLECTION_THREAD_H
#define COLLECTION_THREAD_H

#include <QThread>
#include "opencv2/opencv.hpp"
#include <QImage>

class CollectThread : public QThread
{
	Q_OBJECT

public:
	CollectThread(cv::VideoCapture& capture);
	void run();
	void SetCollectFlag(bool bCollect);

signals:
	void SigDisplay(const QImage& img);

private:
	cv::VideoCapture& m_capture;
	bool m_bCollect = false;
	QImage m_img;
};

#endif
