#pragma once
#ifndef __IMAGELOADER_H__
#define __IMAGELOADER_H__

#include <QObject>
#include <stb_image.h>

class ImageLoader : public QObject
{
	Q_OBJECT

public:
	ImageLoader(QObject *parent);
	~ImageLoader();
};

#endif