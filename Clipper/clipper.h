#ifndef CLIPPER_H
#define CLIPPER_H

#include <QtWidgets/QWidget>

class QPushButton;
class QLabel;
class QScrollArea;
class QLineEdit;

class Clipper : public QWidget
{
	Q_OBJECT

public:
	Clipper(QWidget *parent = 0);
	~Clipper();
protected:
	void createSubCom();
	void loadConfig();
	void connectSignal();
	void reloadImage(const QString &fileRoute);
protected:
	virtual bool eventFilter(QObject *obj, QEvent *e) override;
	virtual void resizeEvent(QResizeEvent *e) override;
	virtual void paintEvent(QPaintEvent *e) override;
protected:
	QPushButton *areaSelectedBt;
	QPushButton *routeSelectedBt;
	QPushButton *reloadImageBt;
	QPushButton *saveFileBt;
	QLabel *prevLa;
	QLabel *leftTopLa;
	QLabel *rightBottomLa;
	QLabel *saveRouteLa;
	QLabel *bigImage;
	QLineEdit *leftTopX;
	QLineEdit *leftTopY;
	QLineEdit *rightBottomX;
	QLineEdit *rightBottomY;
	QLineEdit *saveRouteEd;
	QScrollArea *bigImageArea;
	QImage originData;
	QPixmap prevData;
	QImage bigImageData;
	QString saveRoute;
	int currentStep;
};

#endif // CLIPPER_H
