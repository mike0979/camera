#include "PlayThread.h"
#include "Utils.h"
#include <QTime>

using namespace utilfunc;

#define SPEED 30

PlayThread::PlayThread(cv::VideoCapture& capture) : m_capture(capture)
{}

//////////////////////////////////////////////////////////////////////////////// 
// Function: run
// Description: 线程执行函数
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::run()
{
	while (true)
	{
		if (!m_bPlay)
		{
			m_capture.set(cv::CAP_PROP_POS_FRAMES, m_dPos);
			Play();
			break;
		}

		// 暂停响应
		if (!m_bPause)
		{
			// 点击进度条响应 变化起始位置
			m_capture.set(cv::CAP_PROP_POS_FRAMES, m_dPos);
			Play();
		}

		if (m_bSet)
		{
			m_bSet = false;
			m_capture.set(cv::CAP_PROP_POS_FRAMES, m_dPos);
			Play();
		}

		msleep(m_speed);
	}
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: ProgessChanged
// Description: 进度条调整
// Parameters:int nValue
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::ProgessChanged(int nValue)
{
	m_dPos = nValue;
	m_bSet = true;
	if (!m_bPlay)
	{
		m_capture.set(cv::CAP_PROP_POS_FRAMES, m_dPos);
		Play();
	}
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: SetPlayFlag
// Description: 设置播放标志
// Parameters:bool bPlay
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::SetPlayFlag(bool bPlay)
{
	m_bPlay = bPlay;
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: SetPauseFlag
// Description: 设置暂停标志
// Parameters:bool bPause
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::SetPauseFlag(bool bPause)
{
	m_bPause = bPause;
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: Forward
// Description: 快进
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::Forward()
{
	// 快进3秒
	m_dPos += 3 * m_capture.get(CV_CAP_PROP_FPS);
	auto totalFrame = m_capture.get(CV_CAP_PROP_FRAME_COUNT);
	if (m_dPos > totalFrame - 1)
	{
		m_dPos = totalFrame - 1;
	}

	m_bSet = true;
	if (!m_bPlay)
	{
		m_capture.set(cv::CAP_PROP_POS_FRAMES, m_dPos);
		Play();
	} 
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: Backward
// Description: 快退
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::Backward()
{
	// 快退三秒
	m_dPos -= 3 * m_capture.get(CV_CAP_PROP_FPS);

	m_bSet = true;
	if (!m_bPlay)
	{
		m_capture.set(cv::CAP_PROP_POS_FRAMES, m_dPos);
		Play();
	}
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: Stop
// Description: 停止
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::Stop()
{
	m_dPos = 1;
	m_bPlay = false;
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: Play
// Description: 播放
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::Play()
{
	// 设置进度条
	cv::Mat frame;
	m_capture >> frame;
	
	if (frame.empty())
	{
		m_bPlay = false;
		emit SigFinish();
		return;
	}

	Mat2Image(frame, m_img);
	m_dPos = m_capture.get(CV_CAP_PROP_POS_FRAMES);

	// 设置时间
	QTime n(0, 0, 0);                // n = 00:00:00
	auto msec = m_capture.get(CV_CAP_PROP_POS_MSEC);
	auto time = n.addMSecs(msec);
	auto curTime = time.toString("hh:mm:ss");

	emit SigDisplay(m_dPos, curTime, m_img);
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: SetTimes
// Description: 设置播放速度
// Parameters:double dTimes
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::SetTimes(double dTimes)
{
	m_speed = qRound(SPEED / dTimes);
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: GetPlayFlag
// Description: 获取播放标志
// Return: bool
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
bool PlayThread::GetPlayFlag()
{
	return m_bPlay;
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: SetPos
// Description: 设置位置
// Parameters:int nPos
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::SetPos(int nPos)
{
	m_dPos = nPos;
}

