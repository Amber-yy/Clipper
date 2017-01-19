#include "clipper.h"

#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QLineEdit>
#include <QResizeEvent>
#include <QFile>
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>

const int len = 8;

#include <QDebug>

Clipper::Clipper(QWidget *parent): QWidget(parent),currentStep(0)
{
	createSubCom();
	loadConfig();
	connectSignal();
}

Clipper::~Clipper()
{

}

void Clipper::createSubCom()
{
	try
	{
		areaSelectedBt = new QPushButton(u8"选择区域",this);
		routeSelectedBt = new QPushButton(u8"更改", this);
		reloadImageBt = new QPushButton(u8"重选文件", this);
		saveFileBt = new QPushButton(u8"保存文件", this);
		prevLa = new QLabel(this);
		leftTopLa = new QLabel(u8"左上角",this);
		rightBottomLa = new QLabel(u8"右下角", this);
		saveRouteLa = new QLabel(u8"保存路径", this);
		bigImage = new QLabel(this);

		leftTopX = new QLineEdit(this);
		leftTopY = new QLineEdit(this);
		rightBottomX = new QLineEdit(this);
		rightBottomY = new QLineEdit(this);
		saveRouteEd = new QLineEdit(this);

		bigImageArea = new QScrollArea(this);
	}
	catch (std::bad_alloc &)
	{
		abort();
	}
}

void Clipper::loadConfig()
{
	setMinimumSize(740,500);
	bigImageArea->setWidget(bigImage);
	prevLa->setScaledContents(true);
}

void Clipper::connectSignal()
{
	bigImage->installEventFilter(this);
	prevLa->installEventFilter(this);

	connect(reloadImageBt, &QPushButton::clicked, [this]() 
	{
		QUrl fileRoute = QFileDialog::getOpenFileUrl(this);
		reloadImage(fileRoute.toLocalFile());
	}
	);

	connect(areaSelectedBt, &QPushButton::clicked, [this]() 
	{
		leftTopX->clear();
		leftTopY->clear();
		rightBottomX->clear();
		rightBottomY->clear();
		currentStep = 0;
	}
	);

	connect(routeSelectedBt, &QPushButton::clicked, [this]() 
	{
		saveRoute =std::move(QFileDialog::getExistingDirectory(this));
		saveRouteEd->setText(saveRoute);
	}
	);

	connect(saveFileBt, &QPushButton::clicked, [this]() 
	{
		if (leftTopX->text().isEmpty() || leftTopY->text().isEmpty() || rightBottomX->text().isEmpty() || rightBottomY->text().isEmpty())
		{
			QMessageBox::warning(this, u8"错误", u8"请先选择要截取的区域");
			return;
		}

		QFile saveFile(saveRouteEd->text());
		saveFile.open(QIODevice::WriteOnly);
		if (!saveFile.isOpen())
		{
			QMessageBox::warning(this, u8"错误", u8"路径无效");
			return;
		}

		int x = leftTopX->text().toInt();
		int y = leftTopY->text().toInt();
		int w = rightBottomX->text().toInt() - x;
		int h = rightBottomY->text().toInt() - y;

		originData.copy(x,y,w,h).save(&saveFile);

		QMessageBox::warning(this, u8"成功", u8"保存完成");
	}
	);


}

void Clipper::reloadImage(const QString &fileRoute)
{
	originData.load(fileRoute);
	prevData.load(fileRoute);

	int w = originData.width();
	int h = originData.height();
	bigImageData= std::move(QImage(w*len+ w+1, h*len + h+ 1,originData.format()));
	bigImage->resize(w*len + w + 1, h*len + h + 1);

	QPainter painter(&bigImageData);
	painter.setPen(Qt::black);
	for (int i = 0; i <=w; i++)
	{
		painter.drawLine(i*(len+1),0,i*(len+1), h*len + h + 1);
	}
	for (int i = 0; i <= h; i++)
	{
		painter.drawLine(0, i*(len+1), w*len + w + 1,i*(len+1));
	}

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			painter.fillRect(i*(len+1)+1,j*(len+1)+1,len,len,QBrush(originData.pixelColor(i,j)));
		}
	}

	update();
}

bool Clipper::eventFilter(QObject * obj, QEvent * e)
{
	if (e->type() != QEvent::Paint&&e->type() != QEvent::MouseButtonPress)
	{
		return false;
	}

	if (e->type() == QEvent::MouseButtonPress&&obj==bigImage)
	{
		QMouseEvent *pos = dynamic_cast<QMouseEvent *>(e);
		QLineEdit *x, *y;
		if (currentStep == 0)
		{
			currentStep++;
			x = leftTopX;
			y = leftTopY;
		}
		else if (currentStep == 1)
		{
			currentStep=0;
			x = rightBottomX;
			y = rightBottomY;

			saveRouteEd->setText(saveRoute+ QString::number(QTime::currentTime().hour())+ QString::number(QTime::currentTime().minute())+QString::number(QTime::currentTime().second())+".png");
		}

		x->setText(QString::number(pos->x() / (len + 1)));
		y->setText(QString::number(pos->y() / (len + 1)));
		
		e->accept();
		return true;
	}

	if (obj == prevLa)
	{
		QPainter painter(prevLa);
		painter.drawPixmap(prevLa->rect(),prevData);
		e->accept();
		return true;
	}
	else if(obj==bigImage)
	{
		QPainter painter(bigImage);
		painter.drawImage(bigImage->rect(),bigImageData);
		e->accept();
		return true;
	}

	return false;
}

void Clipper::resizeEvent(QResizeEvent * e)
{
	QSize s = e->size();
	bigImageArea->setGeometry(5,5,s.width()-255,s.height()-10);
	prevLa->setGeometry(s.width() - 245,5,240,s.height()/3);
	areaSelectedBt->setGeometry(s.width() - 245,10+ s.height() / 3,80,25);

	leftTopLa->setGeometry(s.width() - 245,  50+ s.height() / 3,60,30);
	rightBottomLa->setGeometry(s.width() - 245, 155 + s.height() / 3, 60, 30);

	leftTopX->setGeometry(s.width() - 175, 50 + s.height() / 3, 60, 30);
	leftTopY->setGeometry(s.width() - 105, 50 + s.height() / 3, 60, 30);
	rightBottomX->setGeometry(s.width() - 175, 155 + s.height() / 3, 60, 30);
	rightBottomY->setGeometry(s.width() - 105, 155 + s.height() / 3, 60, 30);

	saveRouteLa->setGeometry(s.width() - 245, 50 + s.height() *2/ 3, 60, 25);
	saveRouteEd->setGeometry(s.width() - 175, 50 + s.height() * 2 / 3, 130, 25);
	routeSelectedBt->setGeometry(s.width() - 45, 50 + s.height() * 2 / 3, 40, 25);
	reloadImageBt->setGeometry(s.width() - 245, 100 + s.height() * 2 / 3, 100, 30);
	saveFileBt->setGeometry(s.width() - 105, 100 + s.height() * 2 / 3, 100, 30);

}

void Clipper::paintEvent(QPaintEvent * e)
{

}
