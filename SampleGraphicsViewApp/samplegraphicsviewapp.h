#ifndef SAMPLEGRAPHICSVIEWAPP_H
#define SAMPLEGRAPHICSVIEWAPP_H

#include <QtGui/QMainWindow>
#include "ui_samplegraphicsviewapp.h"
#include <QGraphicsScene>

class SampleGraphicsViewApp : public QMainWindow
{
	Q_OBJECT

public:
	SampleGraphicsViewApp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SampleGraphicsViewApp();

public slots:
	void init();

	void zoom(int scalePercentage);

private:
	Ui::SampleGraphicsViewAppClass ui;

	int m_nScaleFactor;
};

#endif // SAMPLEGRAPHICSVIEWAPP_H
