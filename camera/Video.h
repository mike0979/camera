/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: Video.h
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-6-15
*  Description: brief
*  History: 
******************************************************************************/

#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>
#include "ui_Vedio.h"
#include "opencv2/opencv.hpp"
#include <memory>
#include <QTranslator>

class QStandardItem;
class PlayThread;

class Video : public QWidget
{
	Q_OBJECT

public:
	Video(QWidget *parent = Q_NULLPTR);
	~Video();
	void Open(QStandardItem* item);
	void RetranslateUi();

private:
	void resizeEvent(QResizeEvent *event);
	void closeEvent(QCloseEvent *event);

private slots:
	void OnProgessChanged(int nValue);
	void OnStart();
	void OnPause();
	void OnForward();
	void OnBackward();
	void OnStop();
	void OnDisPlay(int nValue, const QString& strCurTime, const QImage& img);
	void OnMouseRelease();
	void OnSpeed(const QString& text);
	void OnFinish();

private:
	Ui::Video ui;
	cv::VideoCapture m_capture;
	std::shared_ptr<QTimer> m_timer;
	QString m_strTotalTime;
	std::shared_ptr<PlayThread> m_playThread;
	bool m_bPause = false;
	QImage m_img;
};

#endif
