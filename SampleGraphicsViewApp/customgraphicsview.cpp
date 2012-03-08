#include "customgraphicsview.h"
#include <QResizeEvent>
#include <QScrollBar>
#include <QImage>
#include <QMessageBox>

QString strExtractedFolder;

HMODULE hXPSConvert = NULL; 

int nViewItem = 0;

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
	//Set scene.
	this->setScene(&m_graphicsScene);

	//Set width to default View width
	m_nWidth = this->geometry().width();

	//Set width to default View height
	m_nHeight = this->geometry().height();

	//View X-offset
	m_nOffsetX = 20;

	//View Y-offset
	m_nOffsetY = 20;

	//Gap between Items.
	m_nItemSpace = 10;

	m_nRow = 0;

	m_nColoumn = 0;

	m_nMaxColumn = 0;

	m_sceneRect.setWidth(m_nWidth);

	m_graphicsScene.setSceneRect(m_sceneRect);

	m_itemTransformation = Qt::SmoothTransformation;

	hXPSConvert = LoadLibrary(L"XpsConvertQt.dll");

	docConvertThread = new DocConverterThread(this);

	docConvertThread->start();

	connect(docConvertThread, SIGNAL(addItem(QString)), this, SLOT(addItemToScene(QString)));

	//Set Rendering hint flags of GraphicsView.
	this->setRenderHints (QPainter::TextAntialiasing);

	this->setDragMode(QGraphicsView::RubberBandDrag);

	viewThread = new ViewWorkerThread(this);

	viewThread->start();

	connect(this, SIGNAL(loadImage(QList<QGraphicsItem*>)), viewThread, SLOT(prepareImage(QList<QGraphicsItem*>)));

	connect(viewThread, SIGNAL(updateItem(CustomItem* )), this, SLOT(updateItemImage(CustomItem*)));

	//Set Cache mode flag.
	//this->setCacheMode (QGraphicsView::NoCache);

	timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), this, SLOT(updateItem()));

	timer->setSingleShot(true);

	//Set Viewport Update mode of the view.
	this->setViewportUpdateMode (QGraphicsView::SmartViewportUpdate);

	connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onScrollValueChanged(int)));
}

CustomGraphicsView::~CustomGraphicsView()
{
	if (NULL != hXPSConvert)
	{
		FreeLibrary(hXPSConvert);
		hXPSConvert = NULL;
	}

	delete timer;

	delete viewThread;

	delete docConvertThread;
}

//Clears view contents.
void CustomGraphicsView::clearView()
{
	//Clear graphics scene
	m_graphicsScene.clear();

	viewThread->clear();

	//Reset row & column of view.
	m_nRow = 0;

	m_nColoumn = 0;

	m_nMaxColumn = 0;
}

//Shows image on view.
void CustomGraphicsView::addItemToScene(QString strEmfFile)
{
	//Calculate maximum column to be displayed in a view.
	m_nMaxColumn = (m_nWidth - (m_nOffsetX + (m_nOffsetX / 2))) / (nItemWidth + m_nItemSpace);

	CustomItem* pImageLabel = new CustomItem(strEmfFile, ++nViewItem);

	pImageLabel->setPos(m_nOffsetX + (m_nColoumn * (nItemWidth + m_nItemSpace)), m_nOffsetY + (m_nRow * (nItemHeight + m_nItemSpace)));

	pImageLabel->setAcceptHoverEvents(true);

	m_graphicsScene.addItem(pImageLabel);

	if ((0 == m_nRow)
			&& 
		(0 == m_nColoumn))
	{
		m_sceneRect.setX(0);
		m_sceneRect.setY(0);
		//m_sceneRect.setWidth(m_nOffsetX + (m_nOffsetX / 2) + nItemWidth);
		m_sceneRect.setHeight(m_nOffsetY + (m_nOffsetY / 2) + nItemHeight);
	}
	else if (0 == m_nColoumn)
	{
		m_sceneRect.setHeight(m_sceneRect.height() + nItemHeight + m_nItemSpace);
	}
			
	m_graphicsScene.setSceneRect(m_sceneRect);

	if (m_nColoumn < (m_nMaxColumn - 1))
	{
		m_nColoumn++;
	}
	else
	{
		m_nRow++;

		m_nColoumn = 0;
	}

	if (m_sceneRect.height() < m_nHeight)
	{
		m_sceneRect.setHeight(m_nHeight);

		m_graphicsScene.setSceneRect(m_sceneRect);
	}

	QList<QGraphicsItem*> visibleItems = this->items(viewport()->rect());

	if ((visibleItems.contains(pImageLabel))
					&&
		(false == timer->isActive()))
	{
		emit loadImage(visibleItems);
	}
}

void CustomGraphicsView::resizeEvent(QResizeEvent* event)
{
	m_itemTransformation = Qt::FastTransformation;

	m_nWidth = event->size().width();

	m_nHeight = event->size().height();

	updateView();

	QGraphicsView::resizeEvent(event);
}

void CustomGraphicsView::updateView()
{
	int nRow = 0;

	int nColoumn = 0;

	QList<QGraphicsItem *> items = this->items();

	m_nMaxColumn = (m_nWidth - (m_nOffsetX + (m_nOffsetX / 2))) / (nItemWidth + m_nItemSpace);

	if (0 == m_nMaxColumn)
	{
		m_nMaxColumn = 1;
	}

	m_sceneRect.setWidth(m_nWidth);

	m_sceneRect.setHeight(m_nOffsetY + (m_nOffsetY / 2) + (ceil(float(items.count()) / m_nMaxColumn) * (nItemHeight + m_nItemSpace)));

	if (m_sceneRect.height() < m_nHeight)
	{
		m_sceneRect.setHeight(m_nHeight);
	}

	m_graphicsScene.setSceneRect(m_sceneRect);

	for (INT index = items.count() - 1; index >= 0; index--)
	{
		CustomItem* item = dynamic_cast<CustomItem*> (items[index]);

		item->setPos(m_nOffsetX + (nColoumn * (nItemWidth + m_nItemSpace)), m_nOffsetY + (nRow * (nItemHeight + m_nItemSpace)));

		if (nColoumn < (m_nMaxColumn - 1))
		{
			nColoumn++;
		}
		else
		{
			nRow++;

			nColoumn = 0;
		}
	}

	m_nRow = nRow;

	m_nColoumn = nColoumn;

	timer->start(500);
}

void CustomGraphicsView::setTransformationMode(bool bSliderPressed)
{
	m_itemTransformation = Qt::SmoothTransformation;

	if (true == bSliderPressed)
	{
		m_itemTransformation = Qt::FastTransformation;
	}

	CustomItem::setTransformationMode(m_itemTransformation);
}

void CustomGraphicsView::setItemSize(int nItemSize)
{
	nItemWidth = nItemSize;

	nItemHeight = nItemSize;

	updateView();
}

void CustomGraphicsView::updateItem()
{
	m_itemTransformation = Qt::SmoothTransformation;

	QList<QGraphicsItem*> visibleItems = this->items(viewport()->rect());

	emit loadImage(visibleItems);
}

void CustomGraphicsView::onScrollValueChanged(int)
{
	viewThread->stopLoading();

	timer->start(500);
}

void CustomGraphicsView::updateItemImage(CustomItem* item)
{
	item->updateItem();

	item->setProcessing(EProcessingState::eRenderred);
}