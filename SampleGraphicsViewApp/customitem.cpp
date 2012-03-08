#include "customitem.h"
#include <QPainter>
#include <QPalette>
#include <QPaintEvent>
#include <QDebug>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>

int nItemWidth = 0;

int nItemHeight = 0;

Qt::TransformationMode CustomItem::m_transformationMode =  Qt::FastTransformation;

CustomItem::CustomItem(const QString& strImagePath, 
					   const int& nItem,
					   QGraphicsItem* parent)
			: QGraphicsItem(parent)
{
	m_strPath = strImagePath;

	m_pixmap = 0;

	m_bHovered = false;

	m_nItem = nItem;

	m_imageRect = QRect(m_nMargin / 2 , m_nMargin / 2, nItemWidth - m_nMargin, nItemHeight - m_nMargin);

	setFlags(ItemIsSelectable | ItemClipsToShape | ItemIsPanel);

	m_eProcessingState = eNone;
}

CustomItem::~CustomItem()
{
}


void CustomItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	if (option->state & QStyle::State_MouseOver)
	{
		painter->fillRect(boundingRect(), option->palette.dark());
	}
	else if (option->state & QStyle::State_Selected)
	{
		painter->fillRect(boundingRect(), option->palette.highlight());
	}
	else
	{
		painter->fillRect(boundingRect(), option->palette.base()/*QColor(255, 255, 255)*/);
	}

	if (false == pixmap().isNull())
	{
		QPixmap modifiedImage = pixmap();

		if (Qt::FastTransformation == m_transformationMode)
		{
			modifiedImage = modifiedImage.scaled(m_imageRect.width(), m_imageRect.height(), Qt::KeepAspectRatio, m_transformationMode);
		}
		else if ((pixmap().width() < m_imageRect.width())
							&&
				 (pixmap().height() < m_imageRect.height()))
		{
			modifiedImage = modifiedImage.scaled(m_imageRect.width(), m_imageRect.height(), Qt::KeepAspectRatio, Qt::FastTransformation);
		}
		else
		{
			modifiedImage = modifiedImage.scaled(m_imageRect.width(), m_imageRect.height(), Qt::KeepAspectRatio, m_transformationMode);
		}

		painter->drawPixmap(m_imageRect.x() + (m_imageRect.width() - modifiedImage.width()) / 2, 
							m_imageRect.y() + (m_imageRect.height() - modifiedImage.height()) / 2, 
							modifiedImage.width(), 
							modifiedImage.height(), 
							modifiedImage);

	}

	painter->restore();
}

void CustomItem::setTransformationMode(Qt::TransformationMode mode)
{
	m_transformationMode = mode;
}

QRectF CustomItem::boundingRect() const
{
	return QRectF(0, 0, nItemWidth, nItemHeight);
}

void CustomItem::setPos(qreal x, qreal y)
{
	m_imageRect.setRect(m_nMargin / 2 , m_nMargin / 2, nItemWidth - m_nMargin, nItemHeight - m_nMargin);

	prepareGeometryChange();
	
	QGraphicsItem::setPos(x, y);
}

void CustomItem::setPixmap(const QPixmap& pixmap)
{
	m_pixmap = pixmap;
}

QPixmap CustomItem::pixmap() const
{
	return m_pixmap;
}

const QString& CustomItem::imagePath()
{
	return m_strPath; 
}

const int& CustomItem::getItemMargin()
{
	return m_nMargin;
}

void CustomItem::hoverEnterEvent (QGraphicsSceneHoverEvent * event)
{
	m_bHovered = true;

	QGraphicsItem::hoverEnterEvent(event);
}

void CustomItem::hoverLeaveEvent (QGraphicsSceneHoverEvent * event)
{
	m_bHovered = false;

	QGraphicsItem::hoverLeaveEvent(event);
}

const int& CustomItem::itemNo()
{
	return m_nItem;
}

void CustomItem::setProcessing(EProcessingState eState)
{
	//lock.lockForWrite();

	m_eProcessingState = eState;

	//lock.unlock();
}

EProcessingState CustomItem::processingState()
{
	//lock.lockForRead();

	EProcessingState eState = m_eProcessingState;

	//lock.unlock();

	return eState;
}

void CustomItem::updateItem()
{
	prepareGeometryChange();

	update();
}
