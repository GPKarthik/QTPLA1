#ifndef VIEWWORKERTHREAD_H
#define VIEWWORKERTHREAD_H

#include <QThread>
#include <QFutureWatcher>
#include "customitem.h"

enum EFileType
{
	eImage = 0,

	eEmf
};

class ViewWorkerThread : public QThread
{
	Q_OBJECT

public:
	ViewWorkerThread(QObject *parent);
	~ViewWorkerThread();

	void run();

	void stopLoading();

	void pauseLoading();

	void resumeLoading();

	void clear();

public slots:
	void prepareImage(QList<QGraphicsItem*> items);

	void updateItemImage(int index);

signals:
	void updateItem(CustomItem* );

private:
	QFutureWatcher<int> *m_pImageLoadThreadWatcher;

	QVector<CustomItem*> m_ItemsNotLoaded;
};

#endif // VIEWWORKERTHREAD_H
