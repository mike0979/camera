/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: Video.cpp
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-6-15
*  Description: brief
*  History: 
******************************************************************************/

#include "Video.h"
#include "Utils.h"
#include <QTimer>
#include <QTime>
#include <QStandardItem>
#include "PlayThread.h"
#include <QMessageBox>

using namespace cv;
using namespace utilfunc;
using namespace std;

Video::Video(QWidget *parent /*= Q_NULLPTR*/)
{
	ui.setupUi(this);
	connect(ui.sliderProgess, SIGNAL(sliderMoved(int)), this, SLOT(OnProgessChanged(int)));
	connect(ui.sliderProgess, SIGNAL(sliderReleased()), this, SLOT(OnMouseRelease()));
	connect(ui.btnForward, SIGNAL(clicked()), this, SLOT(OnForward()));
	connect(ui.btnBackward, SIGNAL(clicked()), this, SLOT(OnBackward()));
	connect(ui.btnStart, SIGNAL(clicked()), this, SLOT(OnStart()));
	connect(ui.btnPause, SIGNAL(clicked()), this, SLOT(OnPause()));
	connect(ui.btnStop, SIGNAL(clicked()), this, SLOT(OnStop()));
	connect(ui.comboBoxSpeed, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(OnSpeed(const QString&)));

	m_playThread = make_shared<PlayThread>(m_capture);
	connect(m_playThread.get(), SIGNAL(SigDisplay(int, const QString&, const QImage&)), 
		this, SLOT(OnDisPlay(int, const QString&, const QImage&)), Qt::QueuedConnection);
	connect(m_playThread.get(), SIGNAL(SigFinish()), this, SLOT(OnFinish()), Qt::QueuedConnection);
}

Video::~Video()
{
	m_playThread->SetPlayFlag(false);
	m_playThread->wait();
	m_capture.release();
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnProgessChanged
// Description: 进度条数值变化
// Parameters:int nValue
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::OnProgessChanged(int nValue)
{
	m_playThread->SetPauseFlag(true);
	m_playThread->ProgessChanged(nValue);
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnStart
// Description: 播放
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::OnStart()
{
	m_bPause = false;
	m_playThread->SetPlayFlag(true);
	m_playThread->SetPauseFlag(false);
	m_playThread->start();
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnPause
// Description: 暂停
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::OnPause()
{
	m_bPause = true;
	m_playThread->SetPauseFlag(true);
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnForward
// Description: 快进
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::OnForward()
{
	m_playThread->Forward();
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnBackward
// Description: 快退
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::OnBackward()
{
	m_playThread->Backward();
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnStop
// Description: 停止
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::OnStop()
{
	m_playThread->Stop();
	m_playThread->wait();
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnDisPlay
// Description: 显示播放的视频
// Parameters:int nValue
// Parameters:const QString & strCurTime
// Parameters:const QImage & img
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::OnDisPlay(int nValue, const QString& strCurTime, const QImage& img)
{
	ui.sliderProgess->setValue(nValue);
	ui.labelTime->setText(strCurTime + '/' + m_strTotalTime);
	m_img = img;
	ui.label->setPixmap(QPixmap::fromImage(img).scaled(ui.label->width(), ui.label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnMouseRelease
// Description: 鼠标释放后
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::OnMouseRelease()
{
	if (!m_bPause)
	{
		OnStart();
	}
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnSpeed
// Description: 设置播放速度
// Parameters:const QString & text
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::OnSpeed(const QString& text)
{
	m_playThread->SetTimes(text.toDouble());
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnFinish
// Description: 播放结束
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::OnFinish()
{
	m_bPause = true;
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: Open
// Description: 打开视频
// Parameters:QStandardItem * item
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::Open(QStandardItem* item)
{
	auto ret = m_playThread->GetPlayFlag();
	if (ret)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please close the previous video first"));
		return;
	}

	auto data = item->data();
	auto fileName = data.toString();
	m_capture.open(fileName.toLocal8Bit().data());
	auto totalFrame = m_capture.get(CV_CAP_PROP_FRAME_COUNT);
	ui.sliderProgess->setMaximum(totalFrame - 1);

	QTime n(0, 0, 0);                // n = 00:00:00
	int nTotalTime = totalFrame / m_capture.get(CV_CAP_PROP_FPS);
	auto time = n.addSecs(nTotalTime);
	m_strTotalTime = time.toString("hh:mm:ss");

	m_playThread->SetPlayFlag(true);
	ui.comboBoxSpeed->setCurrentIndex(3);
	m_playThread->SetTimes(1);
	m_playThread->start();
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: RetranslateUi
// Description: 更新语言
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::RetranslateUi()
{
	ui.retranslateUi(this);
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: resizeEvent
// Description: 界面大小改变
// Parameters:QResizeEvent * event
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::resizeEvent(QResizeEvent *event)
{
	ui.label->setPixmap(QPixmap::fromImage(m_img).scaled(ui.label->width(), ui.label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: closeEvent
// Description: 窗口关闭事件
// Parameters:QCloseEvent * event
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Video::closeEvent(QCloseEvent *event)
{
	m_playThread->SetPlayFlag(false);
	m_playThread->wait();
	m_capture.release();
	m_playThread->SetPos(1);
}
