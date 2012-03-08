#include "samplegraphicsviewapp.h"
#include <QFileDialog>

SampleGraphicsViewApp::SampleGraphicsViewApp(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_nScaleFactor = ui.horizontalSlider->value();

	init();

	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(zoom(int)));
}

SampleGraphicsViewApp::~SampleGraphicsViewApp()
{

}

void SampleGraphicsViewApp::init()
{
	//Clear existing view.
	ui.graphicsView->clearView();

	//Set image width and height as per the current zoom value.
	nItemWidth = m_nScaleFactor;

	nItemHeight = m_nScaleFactor;
}

void SampleGraphicsViewApp::zoom(int scalePercentage)
{
	ui.graphicsView->setTransformationMode(true);

	//Zoom value.
	m_nScaleFactor = scalePercentage; 

	ui.graphicsView->setItemSize(m_nScaleFactor);
}