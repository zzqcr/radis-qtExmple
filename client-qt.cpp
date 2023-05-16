#include <iostream>
using namespace std;
#include "client-qt.h"
#include "ThreadPool.h"
void getCallback(redisAsyncContext *, void * r, void * privdata) {

    redisReply * reply = static_cast<redisReply *>(r);
    ClientQt * ex = static_cast<ClientQt *>(privdata);
    if (reply == nullptr || ex == nullptr) return;

    // Receive the message of radis and throw it to the thread pool for processing
    auto fptr = ThreadPool::instance()->submit([=](){
        return reply->str;
    });

    cout << "getCallback: " << fptr.get() << endl;
}

void sendCallback(redisAsyncContext *, void * r, void * privdata) {

    redisReply * reply = static_cast<redisReply *>(r);
    if (reply == nullptr) return;

    cout << "sendCallback: " << reply->str <<endl;
}

void disconnectCallback(const struct redisAsyncContext* ctx, int status)
{
    cout << "disconnectCallback: " << ctx->errstr << endl;
}
void connectCallback(const struct redisAsyncContext* ctx, int status)
{
    cout << "connectCallback: " << ctx->errstr << endl;
}


void ClientQt::run() {

    cout << "run start"<< endl;
    static int num =0;
    num++;
    asyncSend(QString("set key %1").arg(num),sendCallback);
    sendCommand(QString("set key2 %1").arg(num));
    auto reply = sendCommand("GET key2");
    cout << "reply: " << reply->str<<endl;
    asyncGet("GET key",getCallback);
}

void ClientQt::connect(QString ip, int port, int timeout)
{
    asyncConnect(ip,port);
    syncConnect(ip,port,timeout);
}

void ClientQt::asyncConnect(QString ip,int port)
{
    m_actx = redisAsyncConnect(ip.toStdString().c_str(), port);
    redisAsyncSetConnectCallback(m_actx,connectCallback);
    redisAsyncSetDisconnectCallback(m_actx,disconnectCallback);
    m_adapter.setContext(m_actx);
    if (m_actx->err) {
        cerr << "Error: " << m_actx->errstr << endl;
        redisAsyncFree(m_actx);
        emit finished();
    }
}

void ClientQt::syncConnect(QString ip, int port, int timeout)
{
    timeval tv;
    tv.tv_sec = timeout;
    m_ctx = redisConnectWithTimeout(ip.toStdString().c_str(), port,tv);
    if(m_ctx->err){
        cerr << "Error: " << m_ctx->errstr << endl;
        redisFree(m_ctx);
        emit finished();
    }
}

void ClientQt::asyncSend(QString command,redisCallbackFn *fn)
{
    redisAsyncCommand(m_actx, fn, NULL, command.toStdString().c_str());
}

void ClientQt::asyncGet(QString command, redisCallbackFn *fn)
{

    redisAsyncCommand(m_actx, fn, this, command.toStdString().c_str());

}

redisReply * ClientQt::sendCommand(QString command)
{
    auto r = redisCommand(m_ctx,command.toStdString().c_str());
    redisReply * reply = static_cast<redisReply *>(r);
    return reply;
}

void ClientQt::checkConnect()
{

}

