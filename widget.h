#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include <QVariant>
#include <QThread>

#include "hidapi.h"

typedef struct hid_device_info* devInfo;
Q_DECLARE_METATYPE(devInfo)

namespace Ui {
class Widget;
class WorkerThread;
}

class WorkerThread : public QThread
{
    Q_OBJECT
    void run();
public:
    bool Exiting;
    hid_device **hidDevice;
    bool ifBoardLenON;
    int ticks;

signals:
    void resultReady(const quint8 *report, quint32 len);
    void testDisconnection();
    void setBLed(bool);
};

class Widget : public QWidget
{
    Q_OBJECT
    WorkerThread *workerThread;
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
    Ui::Widget *ui;
    devInfo listEnumDevs;
    hid_device *hidDevice;

    void startWorkInAThread();
    bool openDevice(uchar *path);
    bool closeDevice();
    bool setChanges(quint8 mask); //ID 2
    int getCurrent(); //ID 4
    bool setValue(quint8 mask,quint8 val); //ID 6
public slots:
    void makeListOfDevs();
    void testDisconnect();
    bool setBoardLed(bool stat); //ID 8
private slots:
    void onRecvFromHID(const quint8 *report, quint32 len);
    void onConnect();
    void onDisconnect();
    void onListNotEmpty();
    void onListEmpty();
    void testConnect();
    void onClose();
    bool setBTReset(bool stat); //ID 6
    bool setBTKey(bool stat); //ID 6
    bool setBoardChanges(quint8 mask); //ID 2
    bool getBoardChanges(); //ID 2
signals:
    void monConnect();
    void monDisonnect();
    void monListNotEmpty();
    void monListEmpty();

};



#endif // DIALOG_H
