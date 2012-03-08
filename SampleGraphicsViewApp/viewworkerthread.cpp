#include "viewworkerthread.h"
#include "customgraphicsview.h"
#include <QtConcurrentMap>
#include <QImageReader>
#include <QImage>
#include <Windows.h>

int readEmf(CustomItem* item)
{
	item->setProcessing(EProcessingState::eProcessing);

	QImage image;

	int image_width = 0;

	int image_height = 0;

	do 
	{
		FnConvertFPage2Bitmap* pfnConvertFPage2Bitmap = NULL;

		//Check handle.
		if (NULL == hXPSConvert)
		{
			break;
		}

		//Get ConvertFPage2Bitmap() pointer.
		pfnConvertFPage2Bitmap = 
			(FnConvertFPage2Bitmap*)GetProcAddress(hXPSConvert, 
												   "ConvertFPage2Bitmap");

		//Validate function pointer.
		if (NULL == pfnConvertFPage2Bitmap)
		{
			break;
		}

		//Path the image to be stored
		QString strImagePath = strExtractedFolder + QString("Image%1.png").arg(item->itemNo());

		//Convert UNICODE to ASCII string
		QByteArray bImagePath = strImagePath.toLocal8Bit();

		CONST CHAR* chImagePath = bImagePath.data();

		QString strPage = item->imagePath();

		//Replace all '/' with '\'
		strPage.replace("/", "\\");

		//Remove any preceeding '\'.
		if (true == strPage.startsWith("\\"))
		{
			strPage.remove(0, 1);
		}

		//Fixed page path.
		QString strPagePath = strExtractedFolder + strPage;

		//Convert UNICODE to ASCII string
		QByteArray bPagePath = strPagePath.toLocal8Bit();

		CONST CHAR* chFixedPagePath = bPagePath.data();

		//Job Container path
		QByteArray bJobFolder = strExtractedFolder.toLocal8Bit();

		CONST CHAR* chJobFolder = bJobFolder.data();

		//Convert Fixed page to bitmap
		if (false == pfnConvertFPage2Bitmap(chJobFolder,
											chFixedPagePath,
											chImagePath,
											96,
											96))
		{
			break;
		}

	    QString strImageFile(chImagePath);

		//Load image.
	    QImageReader imageReader(strImageFile);

	    QSize size;

		int nMaxImageSze = 1000;

		image_width = nMaxImageSze;
		image_height = nMaxImageSze;

		if (imageReader.supportsOption(QImageIOHandler::Size))
		{
			size = imageReader.size();
			image_width = size.width();
			image_height = size.height();
		}

		double ratio = (double)image_width / (double)image_height;

		if (image_width <= image_height)
		{
			image_height = nMaxImageSze;
			image_width = ratio * image_height;
		}
		else
		{
			image_width = nMaxImageSze;
			image_height = image_width / ratio;
		}

		imageReader.setScaledSize(QSize(image_width, image_height));

		image = imageReader.read();
	} while (0);

	if (image.isNull())
	{
		QImage imageIcon(QLatin1String("Image.png"));
		image = imageIcon;
		image = image.scaled(image_width, image_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	item->setPixmap(QPixmap::fromImage(image));

	item->setProcessing(EProcessingState::eProcessed);

	return 0;
}

ViewWorkerThread::ViewWorkerThread(QObject *parent)
	: QThread(parent)
{
}

ViewWorkerThread::~ViewWorkerThread()
{
	if (m_pImageLoadThreadWatcher->isRunning())
	{
		m_pImageLoadThreadWatcher->cancel();
	}

	m_pImageLoadThreadWatcher->waitForFinished();

	delete m_pImageLoadThreadWatcher;
	m_pImageLoadThreadWatcher = NULL;
}

void ViewWorkerThread::run()
{
	m_pImageLoadThreadWatcher = new QFutureWatcher<int>(this);

	connect(m_pImageLoadThreadWatcher, SIGNAL(resultReadyAt(int)), this, SLOT(updateItemImage(int)));

	exec();
}

void ViewWorkerThread::prepareImage(QList<QGraphicsItem*> items)
{
	stopLoading();

	QVector<CustomItem*> itemsNotLoaded;

	QVector<CustomItem*> itemsBeingProcessed;

	//Find all items whose images are not loaded.
	for (int index = 0; index < items.count(); index++)
	{
		CustomItem* item = dynamic_cast<CustomItem*> (items[index]);

		if ((true == item->pixmap().isNull())
						||
			((nItemWidth > (item->pixmap().width() + CustomItem::getItemMargin()))
						&&
			 (nItemHeight > (item->pixmap().height() + CustomItem::getItemMargin()))))
		{
			if (EProcessingState::eNone == item->processingState())
			{
				itemsNotLoaded.push_front(item);
			}
			else if (EProcessingState::eProcessed == item->processingState())
			{
				item->updateItem();

				item->setProcessing(EProcessingState::eRenderred);
			}
		}
		else if ((EProcessingState::eNone != item->processingState()) 
									&&
				(EProcessingState::eRenderred != item->processingState()))
		{
			item->updateItem();

			item->setProcessing(EProcessingState::eRenderred);
		}
	}

	CustomGraphicsView* parentView = dynamic_cast<CustomGraphicsView*> (parent());

	QList<QGraphicsItem *> viewItems = parentView->items();

	//Clear images from items that are not currently viewed. 
	//(To optimize memory consumption)
	for (int index = 0; index < viewItems.count(); index++)
	{
		if (!items.contains(viewItems[index]))
		{
			CustomItem* item = dynamic_cast<CustomItem*> (viewItems[index]);

			item->setPixmap(0);

			item->setProcessing(EProcessingState::eNone);
		}
	}

	if (itemsNotLoaded.isEmpty())
	{
		return;
	}

	m_ItemsNotLoaded.clear();

	m_ItemsNotLoaded = itemsNotLoaded;

	//Load images.
	m_pImageLoadThreadWatcher->setFuture(QtConcurrent::mapped(m_ItemsNotLoaded, readEmf));
}

void ViewWorkerThread::stopLoading()
{
	if (m_pImageLoadThreadWatcher == NULL)
	{
		return;
	}

	if (true == m_pImageLoadThreadWatcher->isRunning())
	{
		m_pImageLoadThreadWatcher->cancel();

		m_pImageLoadThreadWatcher->waitForFinished();
	}
}

void ViewWorkerThread::pauseLoading()
{
	if (m_pImageLoadThreadWatcher == NULL)
	{
		return;
	}

	if (m_pImageLoadThreadWatcher->isRunning())
	{
		m_pImageLoadThreadWatcher->pause();
	}
}

void ViewWorkerThread::resumeLoading()
{
	if (m_pImageLoadThreadWatcher == NULL)
	{
		return;
	}

	if (m_pImageLoadThreadWatcher->isPaused())
	{
		m_pImageLoadThreadWatcher->resume();
	}
}

void ViewWorkerThread::updateItemImage(int index)
{
	m_ItemsNotLoaded[index]->updateItem();

	m_ItemsNotLoaded[index]->setProcessing(EProcessingState::eRenderred);
}

void ViewWorkerThread::clear()
{
	m_ItemsNotLoaded.clear();
}