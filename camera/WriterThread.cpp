/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: WriterThread.cpp
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-6-15
*  Description: brief
*  History: 
******************************************************************************/

#include "WriterThread.h"

WriterThread::WriterThread(cv::VideoCapture& capture, cv::VideoWriter& writer) : m_capture(capture), m_writer(writer)
{

}

//////////////////////////////////////////////////////////////////////////////// 
// Function: run
// Description: 线程运行函数
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void WriterThread::run()
{
	cv::Mat frame;

	while (m_bRecord)
	{
		m_capture >> frame;
		if (frame.empty())
		{
			break;
		}

		cv::cvtColor(frame, frame, CV_BGR2RGB);
		m_writer << frame;
		msleep(30);
	}
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: SetRecordFlag
// Description: 设置是否录制
// Parameters:bool bRecord
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void WriterThread::SetRecordFlag(bool bRecord)
{
	m_bRecord = bRecord;
}

