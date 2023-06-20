/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: WriterThread.h
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-6-15
*  Description: brief
*  History: 
******************************************************************************/

#ifndef WRITER_THREAD_H
#define WRITER_THREAD_H

#include <QThread>
#include "opencv2/opencv.hpp"

class WriterThread : public QThread
{
public:
	WriterThread(cv::VideoCapture& capture, cv::VideoWriter& writer);
	void run();
	void SetRecordFlag(bool bRecord);

private:
	cv::VideoCapture& m_capture;
	cv::VideoWriter& m_writer;
	bool m_bRecord = false;
};

#endif
