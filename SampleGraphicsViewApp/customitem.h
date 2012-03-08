#ifndef CUSTOMITEM_H
#define CUSTOMITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QReadWriteLock>

extern int nItemWidth;
extern int nItemHeight;

enum EProcessingState
{
	eNone = 0,

	eProcessing,

	eProcessed,

	eRenderred
};

class CustomItem : public QGraphicsItem
{
public:
	CustomItem(const QString& strImagePath, 
			   const int& nItem,
			   QGraphicsItem* parent = 0);

	~CustomItem();

	virtual QRectF boundingRect() const;

	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

	static void setTransformationMode(Qt::TransformationMode mode);

	void setPos(qreal x, qreal y);

	QPixmap pixmap() const;

	void setPixmap(const QPixmap& pixmap);

	const QString& imagePath();

	const int& itemNo();

	static const int& getItemMargin();

	void setProcessing(EProcessingState eState);

	EProcessingState processingState();

	void updateItem();

protected:
	void hoverEnterEvent (QGraphicsSceneHoverEvent * event);

	void hoverLeaveEvent (QGraphicsSceneHoverEvent * event);

private:
	QPixmap m_pixmap;

	static int const m_nMargin = 10;

	QRect m_imageRect;

	QString m_strPath;

	static Qt::TransformationMode m_transformationMode;

	QReadWriteLock lock;

	bool m_bHovered;

	int m_nItem;

	EProcessingState m_eProcessingState;
};

#endif // CUSTOMITEM_H
