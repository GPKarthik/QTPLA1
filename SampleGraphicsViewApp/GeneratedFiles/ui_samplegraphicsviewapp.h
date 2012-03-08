/********************************************************************************
** Form generated from reading UI file 'samplegraphicsviewapp.ui'
**
** Created: Thu Mar 8 19:39:59 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMPLEGRAPHICSVIEWAPP_H
#define UI_SAMPLEGRAPHICSVIEWAPP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>
#include <customgraphicsview.h>

QT_BEGIN_NAMESPACE

class Ui_SampleGraphicsViewAppClass
{
public:
    QAction *actionOpen;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    CustomGraphicsView *graphicsView;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSlider *horizontalSlider;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *SampleGraphicsViewAppClass)
    {
        if (SampleGraphicsViewAppClass->objectName().isEmpty())
            SampleGraphicsViewAppClass->setObjectName(QString::fromUtf8("SampleGraphicsViewAppClass"));
        SampleGraphicsViewAppClass->resize(627, 400);
        actionOpen = new QAction(SampleGraphicsViewAppClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        centralWidget = new QWidget(SampleGraphicsViewAppClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new CustomGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        QBrush brush(QColor(108, 108, 108, 255));
        brush.setStyle(Qt::SolidPattern);
        graphicsView->setBackgroundBrush(brush);

        gridLayout->addWidget(graphicsView, 0, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(165, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 1, 1, 1);

        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMinimum(200);
        horizontalSlider->setMaximum(1000);
        horizontalSlider->setSingleStep(10);
        horizontalSlider->setPageStep(50);
        horizontalSlider->setValue(200);
        horizontalSlider->setTracking(true);
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 1, 2, 1, 1);

        SampleGraphicsViewAppClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SampleGraphicsViewAppClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 627, 21));
        SampleGraphicsViewAppClass->setMenuBar(menuBar);

        retranslateUi(SampleGraphicsViewAppClass);

        QMetaObject::connectSlotsByName(SampleGraphicsViewAppClass);
    } // setupUi

    void retranslateUi(QMainWindow *SampleGraphicsViewAppClass)
    {
        SampleGraphicsViewAppClass->setWindowTitle(QApplication::translate("SampleGraphicsViewAppClass", "SampleGraphicsViewApp", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("SampleGraphicsViewAppClass", "Open", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SampleGraphicsViewAppClass: public Ui_SampleGraphicsViewAppClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMPLEGRAPHICSVIEWAPP_H
