#ifndef HANDLER_H
#define HANDLER_H

/**
 * 作用：监听对象基类，作为接口使用，供监听类和通信类提供统一调用接口（在事件循环类中通过事件处理函数调用）
*/
class Handler
{
public:
    Handler();
    ~Handler();
    virtual void handlerAccept();       //将被TcpServer类实现
    virtual void handlerRead();         //将被TcpSocket类实现
    virtual void handlerWrite();        //将被TcpSocket类实现
protected:
    int socketfd;       //维护的文件描述符
};

#endif