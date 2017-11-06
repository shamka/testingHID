#include "widget.h"
#include "ui_widget.h"
#include "widget.h"
#include <windows.h>
#include <QApplication>

//const unsigned char LED_ON[]= {0x06,(1<<0),(1<<0)};
const unsigned char LED_ON[]= {0x08,0x01};
const unsigned char LED_OFF[]={0x08,0x00};
const unsigned char BT_RESET_ON[]= {0x06,(1<<6),(1<<6)};
const unsigned char BT_RESET_OFF[]={0x06,(1<<6),0x00};
const unsigned char BT_KEY_ON[]=   {0x06,(1<<7),(1<<7)};
const unsigned char BT_KEY_OFF[]=  {0x06,(1<<7),0x00};
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
    connect(ui->mBtnUpdate,SIGNAL(clicked()),this,SLOT(makeListOfDevs()));
    connect(ui->mBtnExit,SIGNAL(clicked()),this,SLOT(onClose()));

    connect(ui->mChkLed,SIGNAL(clicked(bool)),this,SLOT(setBoardLed(bool)));
    connect(ui->mChkReset,SIGNAL(clicked(bool)),this,SLOT(setBTReset(bool)));
    connect(ui->mChkKey,SIGNAL(clicked(bool)),this,SLOT(setBTKey(bool)));

    makeListOfDevs();
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

void Widget::makeListOfDevs()
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
        makeListOfDevs();
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
bool Widget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    result=result+0;
    if(eventType=="windows_generic_MSG"){
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
                makeListOfDevs();
            }
            return true;
        case WM_CLOSE:
            return (hidDevice!=NULL);
        }
    }
    return false;
}
void Widget::startWorkInAThread()
{
    workerThread = new WorkerThread();
    workerThread->hidDevice=&hidDevice;
    connect(workerThread, SIGNAL(resultReady(const quint8,quint32)), this, SLOT(onRecvFromHID(const quint8*,quint32)));
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
bool Widget::setBoardChanges(quint8 mask)
{
    quint8 buff[2]={2,mask};
    workerThread->ticks=0;
    return hid_write(hidDevice,buff,sizeof(buff));
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
        break;
    case 3:
        break;
    }
}

