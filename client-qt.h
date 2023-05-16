#ifndef __HIREDIS_EXAMPLE_QT_H
#define __HIREDIS_EXAMPLE_QT_H

#include <qtAdapter.h>
#include <WinSock2.h>
class ClientQt : public QObject {

Q_OBJECT

public:
    ClientQt(QObject * parent = 0):QObject(parent) {}
    void connect(QString ip,int port=6379,int timeout =1);
    void asyncSend(QString command,redisCallbackFn *fn =NULL);
    void asyncGet(QString command,redisCallbackFn *fn);
    redisReply * sendCommand(QString command);
    void checkConnect();

protected:
    void asyncConnect(QString ip,int port =6379);
    void syncConnect(QString ip,int port =6379,int timeout =1);
signals:
    void finished();

public slots:
    void run();

private:
    void finish() { emit finished(); }

private:
    redisAsyncContext * m_actx;
    redisContext * m_ctx;
    RedisQtAdapter m_adapter;

friend
    void getCallback(redisAsyncContext *, void *, void *);
};

#endif /* !__HIREDIS_EXAMPLE_QT_H */
