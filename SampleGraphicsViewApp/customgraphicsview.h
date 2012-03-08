#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QStringList>
#include <QTimer>
#include "customitem.h"
#include "viewworkerthread.h"
#include "docconverterthread.h"
#include <Windows.h>

extern QString strExtractedFolder;

extern HMODULE hXPSConvert;

class CustomGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	CustomGraphicsView(QWidget *parent);
	~CustomGraphicsView();

	void clearView();

public slots:
	void updateView();

	void setTransformationMode(bool bSliderPressed = false);

	void setItemSize(int nItemSize);

	void onScrollValueChanged(int);

	void updateItem();

	void updateItemImage(CustomItem* item);

	void addItemToScene(QString strEmfFile);

signals:
	void loadImage(QList<QGraphicsItem*> items);

protected:
	virtual void resizeEvent (QResizeEvent* event);

private:
	QGraphicsScene m_graphicsScene;

	INT m_nWidth;

	INT m_nHeight;

	INT m_nRow;

	INT m_nColoumn;

	INT m_nMaxColumn;

	INT m_nOffsetX;

	INT m_nOffsetY;

	INT m_nItemSpace;

	QRectF m_sceneRect;

	Qt::TransformationMode m_itemTransformation;

	QTimer* timer;

	DocConverterThread* docConvertThread;

	ViewWorkerThread* viewThread;
};

#endif // CUSTOMGRAPHICSVIEW_H
