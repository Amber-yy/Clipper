#ifndef CLIPPER_H
#define CLIPPER_H

#include <QtWidgets/QWidget>
#include "ui_clipper.h"

class Clipper : public QWidget
{
	Q_OBJECT

public:
	Clipper(QWidget *parent = 0);
	~Clipper();

private:
	Ui::ClipperClass ui;
};

#endif // CLIPPER_H
