#include "widget.h"
#include "ui_widget.h"
#include "widget.h"
#include <windows.h>
#include <QApplication>
#include <QColor>
#include <QGraphicsScene>
#include <QGraphicsView>

//const unsigned char LED_ON[]= {0x06,(1<<0),(1<<0)};
const unsigned char LED_ON[]= {0x08,0x01};
const unsigned char LED_OFF[]={0x08,0x00};
const unsigned char BT_RESET_ON[]= {0x06,(1<<6),(1<<6)};
const unsigned char BT_RESET_OFF[]={0x06,(1<<6),0x00};
const unsigned char BT_KEY_ON[]=   {0x06,(1<<7),(1<<7)};
const unsigned char BT_KEY_OFF[]=  {0x06,(1<<7),0x00};

const QColor _color_null=QColor(0,0,0,0);

const QColor _color_MN=QColor(0,255,0,255);
const QColor _color_MF=QColor(0,128,0,255);
const QColor _color_UN=QColor(192,192,0,128);
const QColor _color_UF=QColor(128,128,0,128);

quint8 reportBuff[256];
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);
    hid_init();
    listEnumDevs=NULL;
    hidDevice=NULL;

    connect(this,SIGNAL(monDisonnect()),this,SLOT(onDisconnect()));
    connect(ui->mBtnDisconnect,SIGNAL(clicked()),this,SLOT(testDisconnect()));
    connect(this,SIGNAL(monConnect()),this,SLOT(onConnect()));
    connect(ui->mBtnConnect,SIGNAL(clicked()),this,SLOT(testConnect()));
    connect(this,SIGNAL(monListEmpty()),this,SLOT(onListEmpty()));
    connect(this,SIGNAL(monListNotEmpty()),this,SLOT(onListNotEmpty()));
    connect(ui->mBtnUpdate,SIGNAL(clicked()),this,SLOT(updateDevList()));
    connect(ui->mBtnExit,SIGNAL(clicked()),this,SLOT(onClose()));

    connect(ui->mChkLed,SIGNAL(clicked(bool)),this,SLOT(setBoardLed(bool)));
    connect(ui->mChkReset,SIGNAL(clicked(bool)),this,SLOT(setBTReset(bool)));
    connect(ui->mChkKey,SIGNAL(clicked(bool)),this,SLOT(setBTKey(bool)));

    ui->mLedBoard->setScene(new QGraphicsScene());
    ui->mBoot1Board->setScene(new QGraphicsScene());
    ui->mLed1BT->setScene(new QGraphicsScene());
    ui->mLed2BT->setScene(new QGraphicsScene());
    ui->mKeyBT->setScene(new QGraphicsScene());
    ui->mRstBT->setScene(new QGraphicsScene());

    connect(ui->chbl,SIGNAL(clicked()),this,SLOT(setBoardChanges()));
    connect(ui->chb1,SIGNAL(clicked()),this,SLOT(setBoardChanges()));
    connect(ui->chl1,SIGNAL(clicked()),this,SLOT(setBoardChanges()));
    connect(ui->chl2,SIGNAL(clicked()),this,SLOT(setBoardChanges()));
    connect(ui->chbtrst,SIGNAL(clicked()),this,SLOT(setBoardChanges()));
    connect(ui->chbtkey,SIGNAL(clicked()),this,SLOT(setBoardChanges()));


    updateDevList();
    onDisconnect();
    startWorkInAThread();
}
Widget::~Widget()
{
    workerThread->quit();
    workerThread->Exiting=true;
    if(hidDevice!=NULL)
        hid_close(hidDevice);
    if(listEnumDevs!=NULL)
        hid_free_enumeration(listEnumDevs);
    hid_exit();
    workerThread->wait();
    delete ui;
}

void Widget::updateDevList()
{
    if(listEnumDevs!=NULL)
        hid_free_enumeration(listEnumDevs);
    ui->mListDevs->clear();
    listEnumDevs = hid_enumerate(1155,22337);
    devInfo _dev = listEnumDevs;
    while(_dev!=NULL){
        if(_dev->usage_page==65280){
            QString name=QString("%1: %2").arg(QString::fromWCharArray(_dev->product_string),QString::fromWCharArray(_dev->serial_number));
            ui->mListDevs->addItem(name,QVariant::fromValue(_dev));
        }
        _dev=_dev->next;
    }
    if(ui->mListDevs->count()>0){
        emit monListNotEmpty();
    }
    else{
        emit monListEmpty();
    }
}
void Widget::testConnect()
{
    if(hidDevice!=NULL){
        testDisconnect();
    }
    if(ui->mListDevs->count()==0)return;
    if(ui->mListDevs->currentData().canConvert<devInfo>()){
        devInfo dev =ui->mListDevs->currentData().value<devInfo>();
        hidDevice=hid_open_path(dev->path);
        if(hidDevice==NULL)return;
        getBoardChanges();
        emit monConnect();
    }
}
void Widget::testDisconnect()
{
    if(hidDevice!=NULL){
        hid_close(hidDevice);
        hidDevice=NULL;
        updateDevList();
    }
    emit monDisonnect();
}
void Widget::onClose()
{
    qApp->quit();
}
void Widget::onConnect()
{
    ui->mListDevs->setEnabled(false);
    ui->mBtnUpdate->setEnabled(false);
    ui->mBtnExit->setEnabled(false);
    ui->mBtnDisconnect->setEnabled(true);
    ui->mBtnConnect->setEnabled(false);
    ui->mChkKey->setEnabled(true);
    ui->mChkLed->setEnabled(true);
    ui->mChkReset->setEnabled(true);
}
void Widget::onDisconnect()
{
    ui->mListDevs->setEnabled(!false);
    ui->mBtnUpdate->setEnabled(!false);
    ui->mBtnExit->setEnabled(!false);
    ui->mBtnDisconnect->setEnabled(!true);
    ui->mBtnConnect->setEnabled(!false);
    ui->mChkKey->setEnabled(!true);
    ui->mChkLed->setEnabled(!true);
    ui->mChkReset->setEnabled(!true);

    ui->mLedBoard->scene()->setForegroundBrush(_color_null);
    ui->mBoot1Board->scene()->setForegroundBrush(_color_null);
    ui->mLed1BT->scene()->setForegroundBrush(_color_null);
    ui->mLed2BT->scene()->setForegroundBrush(_color_null);
    ui->mKeyBT->scene()->setForegroundBrush(_color_null);
    ui->mRstBT->scene()->setForegroundBrush(_color_null);
}
void Widget::onListNotEmpty()
{
    emit testDisconnect();
    ui->mBtnConnect->setEnabled(!false);
}
void Widget::onListEmpty()
{
    emit testDisconnect();
    ui->mBtnConnect->setEnabled(false);
}
bool Widget::nativeEvent(const QByteArray &, void *message, long *result)
{
    result=result+0;
    //if(eventType=="windows_generic_MSG"){
        //WINDOWS MSG
        MSG *msg = static_cast< MSG * >( message );
        switch(msg->message){
        case WM_DEVICECHANGE:
        //case 1096:
        //case 1149:
        //case 1190:
        //case 1191:
        //case 1249:
            if(hidDevice==NULL){
                updateDevList();
            }
            return true;
        case WM_CLOSE:
            return (hidDevice!=NULL);
        }
    //}
    return false;
}
void Widget::startWorkInAThread()
{
    workerThread = new WorkerThread();
    workerThread->hidDevice=&hidDevice;
    connect(workerThread, SIGNAL(resultReady(const quint8*,quint32)), this, SLOT(onRecvFromHID(const quint8*,quint32)));
    connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));
    connect(workerThread, SIGNAL(testDisconnection()), this, SLOT(testDisconnect()));
    connect(workerThread, SIGNAL(setBLed(bool)),this,SLOT(setBoardLed(bool)));
    workerThread->Exiting=false;
    workerThread->start();
}
bool Widget::setBoardLed(bool stat)
{
    if(hidDevice==NULL)return false;
    workerThread->ifBoardLenON=stat;
    workerThread->ticks=0;
    if(stat){
        return hid_write(hidDevice,LED_ON,sizeof(LED_ON));
    }
    else{
        return hid_write(hidDevice,LED_OFF,sizeof(LED_OFF));
    }
}
bool Widget::setBTKey(bool stat)
{
    if(hidDevice==NULL)return false;
    workerThread->ticks=0;
    if(stat){
        return hid_write(hidDevice,BT_KEY_ON,sizeof(BT_KEY_ON));
    }
    else{
        return hid_write(hidDevice,BT_KEY_OFF,sizeof(BT_KEY_OFF));
    }
}
bool Widget::setBoardChanges(qint16 mask)
{
    if(mask==-1){
        mask=0;
        if(ui->chbl->isChecked())mask|=1;
        if(ui->chb1->isChecked())mask|=2;
        if(ui->chl1->isChecked())mask|=16;
        if(ui->chl2->isChecked())mask|=32;
        if(ui->chbtrst->isChecked())mask|=64;
        if(ui->chbtkey->isChecked())mask|=128;
    }
    quint8 buff[2]={2,(quint8)(mask&255)};
    workerThread->ticks=0;
    hid_write(hidDevice,buff,sizeof(buff));
    getBoardChanges();
    return true;
}
bool Widget::getBoardChanges()
{
    static quint8 buff[2]={4,0};
    if(hidDevice==NULL)return false;
    workerThread->ticks=0;
    return hid_write(hidDevice,buff,sizeof(buff));
}
bool Widget::setBTReset(bool stat)
{
    if(hidDevice==NULL)return false;
    workerThread->ticks=0;
    if(stat){
        return hid_write(hidDevice,BT_RESET_ON,sizeof(BT_RESET_ON));
    }
    else{
        return hid_write(hidDevice,BT_RESET_OFF,sizeof(BT_RESET_OFF));
    }
}
void WorkerThread::run()
{
    int readed;
    ticks=0;
    while(!Exiting){
        if(*(hidDevice)==NULL){
            Sleep(1000);
        }
        else{
            try{
                readed = hid_read_timeout(*hidDevice,reportBuff,64,500);
            }
            catch(const std::exception e){
                readed=-1;
            }
            if(readed==-1){
                emit testDisconnection();
                ticks=0;
                Sleep(1000);
            }
            else if(readed>0){
                emit resultReady(reportBuff,readed);
                ticks=0;
                Sleep(128);
            }
            else{
                ticks++;
                if(ticks>30){
                    ticks=0;
                    emit setBLed(ifBoardLenON);
                }
            }
        }
    }
}
void Widget::onRecvFromHID(const quint8 *report, quint32 len)
{
    if(len==0)return;
    switch(report[0]){
    case 1:
        flag_data=(flag_data&(~flag_mask))|report[1];
        break;
    case 3:
        flag_mask=report[1];
        flag_data=report[2];
        break;
    default:return;
    }
    ui->mLedBoard->scene()->setForegroundBrush((flag_mask&1)?((flag_data&1)?_color_MN:_color_MF):((flag_data&1)?_color_UN:_color_UF));
    ui->mBoot1Board->scene()->setForegroundBrush((flag_mask&2)?((flag_data&2)?_color_MN:_color_MF):((flag_data&2)?_color_UN:_color_UF));
    ui->mLed1BT->scene()->setForegroundBrush((flag_mask&16)?((flag_data&16)?_color_MN:_color_MF):((flag_data&16)?_color_UN:_color_UF));
    ui->mLed2BT->scene()->setForegroundBrush((flag_mask&32)?((flag_data&32)?_color_MN:_color_MF):((flag_data&32)?_color_UN:_color_UF));
    ui->mRstBT->scene()->setForegroundBrush((flag_mask&64)?((flag_data&64)?_color_MN:_color_MF):((flag_data&64)?_color_UN:_color_UF));
    ui->mKeyBT->scene()->setForegroundBrush((flag_mask&128)?((flag_data&128)?_color_MN:_color_MF):((flag_data&128)?_color_UN:_color_UF));

    ui->mChkKey->setCheckState((flag_data&128)?(Qt::CheckState::Checked):(Qt::CheckState::Unchecked));
    ui->mChkReset->setCheckState((flag_data&64)?(Qt::CheckState::Checked):(Qt::CheckState::Unchecked));
    ui->mChkLed->setCheckState((flag_data&1)?(Qt::CheckState::Checked):(Qt::CheckState::Unchecked));

    ui->chbl->setCheckState((flag_mask&1)?(Qt::CheckState::Checked):(Qt::CheckState::Unchecked));
    ui->chb1->setCheckState((flag_mask&2)?(Qt::CheckState::Checked):(Qt::CheckState::Unchecked));
    ui->chl1->setCheckState((flag_mask&16)?(Qt::CheckState::Checked):(Qt::CheckState::Unchecked));
    ui->chl2->setCheckState((flag_mask&32)?(Qt::CheckState::Checked):(Qt::CheckState::Unchecked));
    ui->chbtrst->setCheckState((flag_mask&64)?(Qt::CheckState::Checked):(Qt::CheckState::Unchecked));
    ui->chbtkey->setCheckState((flag_mask&128)?(Qt::CheckState::Checked):(Qt::CheckState::Unchecked));

}

