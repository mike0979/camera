/******************************************************************************
*  Copyright (C), 2017-2019,Advanced Technology Institute of Suzhou. 
*  THIS IS AN UNPUBLISHED WORK CONTAINING CONFIDENTIAL AND PROPRIETARY 
*   
*******************************************************************************
*  File Name: Image.cpp
*  Author: cg.zhang@atisz.ac.cn
*  Version: 1.0.0
*  Date: 2020-6-15
*  Description: brief
*  History: 
******************************************************************************/

#include "Image.h"
#include <QStandardItem>

Image::Image(QWidget *parent /*= Q_NULLPTR*/)
{
	ui.setupUi(this);
}

//////////////////////////////////////////////////////////////////////////////// 
// Function: Display
// Description: ÏÔÊ¾Í¼Æ¬
// Parameters:QStandardItem * item
// Return: void
// Remarks: 
////////////////////////////////////////////////////////////////////////////////
void Image::Display(QStandardItem* item)
{
	auto data = item->data();
	m_strFileName = data.toString();
	ui.label->setPixmap(QPixmap(m_strFileName).scaled(ui.label->width(), ui.label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Image::RetranslateUi()
{
	ui.retranslateUi(this);
}

void Image::resizeEvent(QResizeEvent *event)
{
	ui.label->setPixmap(QPixmap(m_strFileName).scaled(ui.label->width(), ui.label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

