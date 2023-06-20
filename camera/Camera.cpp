/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: Camera.cpp
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-6-15
*  Description: brief
*  History: 
******************************************************************************/

#include "Camera.h"
#include "Utils.h"
#include <QTimer>
#include <QFileDialog>
#include <QStandardItem>
#include <QMessageBox>
#include "Video.h"
#include "Image.h"
#include "WriterThread.h"
#include "CollectThread.h"
#include <QSettings>

using namespace utilfunc;
using namespace std;

enum Language
{
	LANGUAGE_CHINESE,
	LANGUAGE_ENGLISH,
};

#define CONFIG_PATH "./config.ini"

Camera::Camera(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.btnOpenCamera, SIGNAL(clicked()), this, SLOT(OnOpenCamera()));
	connect(ui.btnCloseCamera, SIGNAL(clicked()), this, SLOT(OnCloseCamera()));
	connect(ui.btnStartRec, SIGNAL(clicked()), this, SLOT(OnStartRecord()));
	connect(ui.btnEndRec, SIGNAL(clicked()), this, SLOT(OnEndRecord()));
	connect(ui.btnScreenShots, SIGNAL(clicked()), this, SLOT(OnScreenShots()));
	connect(ui.comboBoxLanguage, SIGNAL(currentIndexChanged(int)), this, SLOT(OnLanguage(int)));

	m_itemVedio = make_shared<QStandardItemModel>(this);
	m_itemImage = make_shared<QStandardItemModel>(this);
	ui.listViewVideo->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.listViewImage->setEditTriggers(QAbstractItemView::NoEditTriggers);

	connect(ui.listViewVideo, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnDoubleClickedVedio(const QModelIndex&)));
	connect(ui.listViewImage, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(OnDoubleClickedImage(const QModelIndex&)));

	m_writerThread = make_shared<WriterThread>(m_capture, m_writer);
	m_collectThread = make_shared<CollectThread>(m_capture);
	m_display = std::make_shared<Video>(this);
	m_image = std::make_shared<Image>(this);

	connect(m_collectThread.get(), SIGNAL(SigDisplay(const QImage&)), this, SLOT(OnDisPlay(const QImage&)), Qt::QueuedConnection);

	m_translator.load("./Translation/camera_zh.qm");
	qApp->installTranslator(&m_translator);
	ui.retranslateUi(this);
	m_display->RetranslateUi();
	m_image->RetranslateUi();
}

Camera::~Camera()
{
	m_writerThread->SetRecordFlag(false);
	m_writerThread->wait();
	m_writer.release();

	m_collectThread->SetCollectFlag(false);
	m_collectThread->wait();
	m_capture.release();
}

void Camera::resizeEvent(QResizeEvent *event)
{
	ui.label->setPixmap(QPixmap::fromImage(m_img).scaled(ui.label->width(), ui.label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnOpenCamera
// Description: 打开摄像头
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Camera::OnOpenCamera()
{
	if (m_capture.isOpened())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Camera is opened"));
		return;
	}

	QSettings settings(CONFIG_PATH, QSettings::IniFormat);
	auto deviceid = settings.value("Camera/deviceid").toInt();

	// 打开摄像头
	m_capture.open(deviceid);

	// 打开失败就结束程序
	if (!m_capture.isOpened())
	{
		return;
	}

	m_writer.release();
	m_collectThread->SetCollectFlag(true);
	m_collectThread->start();
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnStartRecord
// Description: 开始录制
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Camera::OnStartRecord()
{
	if (m_writer.isOpened())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please end this recording first"));
		return;
	}

	if (!m_capture.isOpened())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please open camera first"));
		return;
	}

	m_strFileName = QFileDialog::getSaveFileName(this, tr("Save Video"), "", "*.mpg");
	if (m_strFileName.isEmpty())
	{
		return;
	}
	else
	{
		// 取得图像帧(frames) 大小
		cv::Size size(static_cast<int>(m_capture.get(CV_CAP_PROP_FRAME_WIDTH)), static_cast<int>(m_capture.get(CV_CAP_PROP_FRAME_HEIGHT)));

		QSettings settings(CONFIG_PATH, QSettings::IniFormat);
		double fps = settings.value("Camera/fps").toDouble();

		// 创建并初始化视频存储对象
		m_writer.open(m_strFileName.toStdString(), CV_FOURCC('M', 'P', 'E', 'G'), fps, size);

		if (!m_writer.isOpened())
		{
			return;
		}

		m_writerThread->SetRecordFlag(true);
		m_writerThread->start();
	}
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnEndRecord
// Description: 结束录制
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Camera::OnEndRecord()
{
	m_writerThread->SetRecordFlag(false);
	m_writerThread->wait();

	if (!m_strFileName.isNull())
	{
		m_writer.release();
		cv::Mat frame;

		if (!m_capture.read(frame))
		{
			return;
		}

		QImage img;
		cv::cvtColor(frame, frame, CV_BGR2RGB);
		Mat2Image(frame, img);
		QStandardItem *item = new QStandardItem();
		item->setIcon(QIcon(QPixmap::fromImage(img)));
		QFileInfo fileInfo(m_strFileName);
		item->setText(fileInfo.fileName());
		item->setData(m_strFileName);
		m_strFileName.clear();
		m_itemVedio->appendRow(item);
		ui.listViewVideo->setModel(m_itemVedio.get());
		//ui.listViewVideo->setViewMode(QListView::IconMode);
		ui.listViewVideo->setIconSize(QSize(100, 100));
	}
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnScreenShots
// Description: 截图
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Camera::OnScreenShots()
{
	if (!m_capture.isOpened())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please open camera first"));
		return;
	}

	QString strFileName = QFileDialog::getSaveFileName(this, tr("Screen Shots"), "", "*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF )");
	if (strFileName.isEmpty())
	{
		return;
	}
	else
	{
		cv::Mat frame;
		m_capture >> frame;
		if (!frame.empty())
		{
			QImage img;
			cv::cvtColor(frame, frame, CV_BGR2RGB);
			Mat2Image(frame, img);
			QFileInfo fileInfo(strFileName);
			QString strSuffix = fileInfo.suffix();
			img.save(strFileName, strSuffix.toStdString().c_str(), 100);

			QStandardItem *item = new QStandardItem();
			item->setIcon(QIcon(QPixmap::fromImage(img)));
			item->setText(fileInfo.fileName());
			item->setData(strFileName);
			m_itemImage->appendRow(item);
			ui.listViewImage->setModel(m_itemImage.get());
			ui.listViewImage->setIconSize(QSize(100, 100));
		}
	}
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnDoubleClickedVedio
// Description: 双击视频
// Parameters:const QModelIndex & index
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Camera::OnDoubleClickedVedio(const QModelIndex& index)
{
	auto item = m_itemVedio->item(index.row(), index.column());
	m_display->Open(item);
	m_display->show();
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnDoubleClickedImage
// Description: 双击图片
// Parameters:const QModelIndex & index
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Camera::OnDoubleClickedImage(const QModelIndex& index)
{
	auto item = m_itemImage->item(index.row(), index.column());
	m_image->Display(item);
	m_image->show();
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: OnCloseCamera
// Description: 关闭相机
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Camera::OnCloseCamera()
{
	m_writerThread->SetRecordFlag(false);
	m_writerThread->wait();
	m_collectThread->SetCollectFlag(false);
	m_collectThread->wait();
	m_capture.release();
}

void Camera::OnDisPlay(const QImage& img)
{
	m_img = img;
	ui.label->setPixmap(QPixmap::fromImage(img).scaled(ui.label->width(), ui.label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Camera::OnLanguage(int nIndex)
{
	switch (nIndex)
	{
	case LANGUAGE_CHINESE:
	{
		//为应用程序安装翻译
		qApp->installTranslator(&m_translator);
		break;
	}
	case LANGUAGE_ENGLISH:
	{
		//为应用程序安装翻译
		qApp->removeTranslator(&m_translator);
		break;
	}
	default:
		break;
	}

	//刷新翻译
	ui.retranslateUi(this);
	m_display->RetranslateUi();
	m_image->RetranslateUi();
}
