/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: Image.h
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-6-15
*  Description: brief
*  History: 
******************************************************************************/

#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>
#include "ui_Image.h"

class QStandardItem;

class Image : public QWidget
{
	Q_OBJECT

public:
	Image(QWidget *parent = Q_NULLPTR);
	void Display(QStandardItem* item);
	void RetranslateUi();

private:
	void resizeEvent(QResizeEvent *event);

private:
	Ui::Image ui;
	QString m_strFileName;
};

#endif

