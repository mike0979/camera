/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: Camera.h
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-6-15
*  Description: brief
*  History: 
******************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#include <QtWidgets/QWidget>
#include "ui_Camera.h"
#include "opencv2/opencv.hpp"
#include <memory>
#include <QTranslator>

class QStandardItemModel;
class QTimer;
class Video;
class Image;
class WriterThread;
class CollectThread;

class Camera : public QWidget
{
	Q_OBJECT

public:
	Camera(QWidget *parent = Q_NULLPTR);
	~Camera();

private:
	void resizeEvent(QResizeEvent *event);

private slots:
	void OnOpenCamera();
	void OnStartRecord();
	void OnEndRecord();
	void OnScreenShots();
	void OnDoubleClickedVedio(const QModelIndex& index);
	void OnDoubleClickedImage(const QModelIndex& index);
	void OnCloseCamera();
	void OnDisPlay(const QImage& img);
	void OnLanguage(int nIndex);

private:
	Ui::cameraClass ui;
	cv::VideoCapture m_capture;
	cv::VideoWriter m_writer;
	std::shared_ptr<QStandardItemModel> m_itemVedio;
	std::shared_ptr<QStandardItemModel> m_itemImage;
	QString m_strFileName;
	std::shared_ptr<Video> m_display;
	std::shared_ptr<Image> m_image;
	std::shared_ptr<WriterThread> m_writerThread;
	std::shared_ptr<CollectThread> m_collectThread;
	QTranslator m_translator;
	QImage m_img;
};


#endif