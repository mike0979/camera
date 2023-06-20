/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: PlayThread.h
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-6-15
*  Description: brief
*  History: 
******************************************************************************/

#ifndef PLAY_THREAD_H
#define PLAY_THREAD_H

#include <QThread>
#include <QImage>
#include "opencv2/opencv.hpp"

class PlayThread : public QThread
{
	Q_OBJECT

public:
	PlayThread(cv::VideoCapture& capture);
	void run();
	void ProgessChanged(int nValue);
	void SetPlayFlag(bool bPlay);
	void SetPauseFlag(bool bPause);
	void Forward();
	void Backward();
	void Stop();
	void Play();
	void SetTimes(double dTimes);
	bool GetPlayFlag();
	void SetPos(int nPos);

signals:
	void SigDisplay(int nValue, const QString& strCurTime, const QImage& img);
	void SigFinish();

private:
	cv::VideoCapture& m_capture;
	double m_dPos;
	bool m_bPlay = false;
	bool m_bPause = false;
	QImage m_img;
	double m_speed;
	bool m_bSet = false;
};

#endif
