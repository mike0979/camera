#include "PlayThread.h"
#include "Utils.h"
#include <QTime>

using namespace utilfunc;

#define SPEED 30

PlayThread::PlayThread(cv::VideoCapture& capture) : m_capture(capture)
{}

//////////////////////////////////////////////////////////////////////////////// 
// Function: run
// Description: �߳�ִ�к���
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

		// ��ͣ��Ӧ
		if (!m_bPause)
		{
			// �����������Ӧ �仯��ʼλ��
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
// Description: ����������
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
// Description: ���ò��ű�־
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
// Description: ������ͣ��־
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
// Description: ���
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::Forward()
{
	// ���3��
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
// Description: ����
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::Backward()
{
	// ��������
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
// Description: ֹͣ
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
// Description: ����
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::Play()
{
	// ���ý�����
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

	// ����ʱ��
	QTime n(0, 0, 0);                // n = 00:00:00
	auto msec = m_capture.get(CV_CAP_PROP_POS_MSEC);
	auto time = n.addMSecs(msec);
	auto curTime = time.toString("hh:mm:ss");

	emit SigDisplay(m_dPos, curTime, m_img);
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: SetTimes
// Description: ���ò����ٶ�
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
// Description: ��ȡ���ű�־
// Return: bool
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
bool PlayThread::GetPlayFlag()
{
	return m_bPlay;
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: SetPos
// Description: ����λ��
// Parameters:int nPos
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void PlayThread::SetPos(int nPos)
{
	m_dPos = nPos;
}

