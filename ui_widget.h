/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *mLoCB;
    QComboBox *mListDevs;
    QPushButton *mBtnUpdate;
    QHBoxLayout *mLoConDis;
    QPushButton *mBtnConnect;
    QPushButton *mBtnDisconnect;
    QPushButton *mBtnExit;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *mLoCheckBox;
    QCheckBox *mChkLed;
    QCheckBox *mChkReset;
    QCheckBox *mChkKey;
    QHBoxLayout *mLoLeds;
    QGraphicsView *mLedBoard;
    QGraphicsView *mBoot1Board;
    QGraphicsView *mLed1BT;
    QGraphicsView *mLed2BT;
    QGraphicsView *mRstBT;
    QGraphicsView *mKeyBT;
    QHBoxLayout *mMasks;
    QCheckBox *chbl;
    QCheckBox *chb1;
    QCheckBox *chl1;
    QCheckBox *chl2;
    QCheckBox *chbtrst;
    QCheckBox *chbtkey;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->setWindowModality(Qt::NonModal);
        Widget->resize(550, 268);
        verticalLayoutWidget = new QWidget(Widget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 531, 251));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        mLoCB = new QHBoxLayout();
        mLoCB->setSpacing(6);
        mLoCB->setObjectName(QStringLiteral("mLoCB"));
        mListDevs = new QComboBox(verticalLayoutWidget);
        mListDevs->setObjectName(QStringLiteral("mListDevs"));

        mLoCB->addWidget(mListDevs);

        mBtnUpdate = new QPushButton(verticalLayoutWidget);
        mBtnUpdate->setObjectName(QStringLiteral("mBtnUpdate"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mBtnUpdate->sizePolicy().hasHeightForWidth());
        mBtnUpdate->setSizePolicy(sizePolicy);

        mLoCB->addWidget(mBtnUpdate);


        verticalLayout->addLayout(mLoCB);

        mLoConDis = new QHBoxLayout();
        mLoConDis->setSpacing(6);
        mLoConDis->setObjectName(QStringLiteral("mLoConDis"));
        mBtnConnect = new QPushButton(verticalLayoutWidget);
        mBtnConnect->setObjectName(QStringLiteral("mBtnConnect"));

        mLoConDis->addWidget(mBtnConnect);

        mBtnDisconnect = new QPushButton(verticalLayoutWidget);
        mBtnDisconnect->setObjectName(QStringLiteral("mBtnDisconnect"));

        mLoConDis->addWidget(mBtnDisconnect);

        mBtnExit = new QPushButton(verticalLayoutWidget);
        mBtnExit->setObjectName(QStringLiteral("mBtnExit"));

        mLoConDis->addWidget(mBtnExit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        mLoConDis->addItem(horizontalSpacer);


        verticalLayout->addLayout(mLoConDis);

        mLoCheckBox = new QVBoxLayout();
        mLoCheckBox->setSpacing(6);
        mLoCheckBox->setObjectName(QStringLiteral("mLoCheckBox"));
        mChkLed = new QCheckBox(verticalLayoutWidget);
        mChkLed->setObjectName(QStringLiteral("mChkLed"));

        mLoCheckBox->addWidget(mChkLed);

        mChkReset = new QCheckBox(verticalLayoutWidget);
        mChkReset->setObjectName(QStringLiteral("mChkReset"));

        mLoCheckBox->addWidget(mChkReset);

        mChkKey = new QCheckBox(verticalLayoutWidget);
        mChkKey->setObjectName(QStringLiteral("mChkKey"));

        mLoCheckBox->addWidget(mChkKey);


        verticalLayout->addLayout(mLoCheckBox);

        mLoLeds = new QHBoxLayout();
        mLoLeds->setSpacing(6);
        mLoLeds->setObjectName(QStringLiteral("mLoLeds"));
        mLedBoard = new QGraphicsView(verticalLayoutWidget);
        mLedBoard->setObjectName(QStringLiteral("mLedBoard"));
        mLedBoard->setEnabled(false);

        mLoLeds->addWidget(mLedBoard);

        mBoot1Board = new QGraphicsView(verticalLayoutWidget);
        mBoot1Board->setObjectName(QStringLiteral("mBoot1Board"));
        mBoot1Board->setEnabled(false);

        mLoLeds->addWidget(mBoot1Board);

        mLed1BT = new QGraphicsView(verticalLayoutWidget);
        mLed1BT->setObjectName(QStringLiteral("mLed1BT"));
        mLed1BT->setEnabled(false);

        mLoLeds->addWidget(mLed1BT);

        mLed2BT = new QGraphicsView(verticalLayoutWidget);
        mLed2BT->setObjectName(QStringLiteral("mLed2BT"));
        mLed2BT->setEnabled(false);

        mLoLeds->addWidget(mLed2BT);

        mRstBT = new QGraphicsView(verticalLayoutWidget);
        mRstBT->setObjectName(QStringLiteral("mRstBT"));
        mRstBT->setEnabled(false);
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::NoBrush);
        mRstBT->setBackgroundBrush(brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::NoBrush);
        mRstBT->setForegroundBrush(brush1);
        mRstBT->setInteractive(false);

        mLoLeds->addWidget(mRstBT);

        mKeyBT = new QGraphicsView(verticalLayoutWidget);
        mKeyBT->setObjectName(QStringLiteral("mKeyBT"));
        mKeyBT->setEnabled(false);

        mLoLeds->addWidget(mKeyBT);


        verticalLayout->addLayout(mLoLeds);

        mMasks = new QHBoxLayout();
        mMasks->setSpacing(6);
        mMasks->setObjectName(QStringLiteral("mMasks"));
        mMasks->setSizeConstraint(QLayout::SetDefaultConstraint);
        mMasks->setContentsMargins(6, 6, 6, 10);
        chbl = new QCheckBox(verticalLayoutWidget);
        chbl->setObjectName(QStringLiteral("chbl"));

        mMasks->addWidget(chbl);

        chb1 = new QCheckBox(verticalLayoutWidget);
        chb1->setObjectName(QStringLiteral("chb1"));

        mMasks->addWidget(chb1);

        chl1 = new QCheckBox(verticalLayoutWidget);
        chl1->setObjectName(QStringLiteral("chl1"));

        mMasks->addWidget(chl1);

        chl2 = new QCheckBox(verticalLayoutWidget);
        chl2->setObjectName(QStringLiteral("chl2"));

        mMasks->addWidget(chl2);

        chbtrst = new QCheckBox(verticalLayoutWidget);
        chbtrst->setObjectName(QStringLiteral("chbtrst"));

        mMasks->addWidget(chbtrst);

        chbtkey = new QCheckBox(verticalLayoutWidget);
        chbtkey->setObjectName(QStringLiteral("chbtkey"));

        mMasks->addWidget(chbtkey);


        verticalLayout->addLayout(mMasks);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "HID Device", Q_NULLPTR));
        mBtnUpdate->setText(QApplication::translate("Widget", "\320\236&\320\261\320\275\320\276\320\262\320\270\321\202\321\214", Q_NULLPTR));
        mBtnConnect->setText(QApplication::translate("Widget", "&\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", Q_NULLPTR));
        mBtnDisconnect->setText(QApplication::translate("Widget", "&\320\236\321\202\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", Q_NULLPTR));
        mBtnExit->setText(QApplication::translate("Widget", "&\320\222\321\213\321\205\320\276\320\264", Q_NULLPTR));
        mChkLed->setText(QApplication::translate("Widget", "&Led on Board", Q_NULLPTR));
        mChkReset->setText(QApplication::translate("Widget", "BT &Reset", Q_NULLPTR));
        mChkKey->setText(QApplication::translate("Widget", "BT &Key", Q_NULLPTR));
        chbl->setText(QApplication::translate("Widget", "Board Led", Q_NULLPTR));
        chb1->setText(QApplication::translate("Widget", "Boot 1", Q_NULLPTR));
        chl1->setText(QApplication::translate("Widget", "BT Led 1", Q_NULLPTR));
        chl2->setText(QApplication::translate("Widget", "BT Led 2", Q_NULLPTR));
        chbtrst->setText(QApplication::translate("Widget", "BT RST", Q_NULLPTR));
        chbtkey->setText(QApplication::translate("Widget", "BT KEY", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
