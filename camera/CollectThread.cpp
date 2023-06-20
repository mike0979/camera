/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: CollectThread.cpp
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-6-15
*  Description: brief
*  History: 
******************************************************************************/

#include "CollectThread.h"
#include "Utils.h"

using namespace utilfunc;

CollectThread::CollectThread(cv::VideoCapture& capture) : m_capture(capture)
{

}

//////////////////////////////////////////////////////////////////////////////// 
// Function: run
// Description: 线程执行函数
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void CollectThread::run()
{
	while (m_bCollect)
	{
		cv::Mat frame;
		m_capture >> frame;
		if (!frame.empty())
		{
			cv::cvtColor(frame, frame, CV_BGR2RGB);
			Mat2Image(frame, m_img);
			emit SigDisplay(m_img);
		}
		msleep(30);
	}
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: SetCollectFlag
// Description: 设置采集标志
// Parameters:bool bCollect
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void CollectThread::SetCollectFlag(bool bCollect)
{
	m_bCollect = bCollect;
}

