#include "tcpserver.h"

TcpServer::TcpServer(int port, EventLoop *eventLoop, ThreadPool *threadPool) : port(port)
{
    this->eventLoop = eventLoop;
    this->threadPool = threadPool;

    this->socketfd = socket(AF_INET, SOCK_STREAM, 0);       //创建监听socket
    if(this->socketfd < 0)
    {
        std::cerr << "创建监听socket失败！" << std::endl;

        exit(1);
    }else 
    {
        std::cout << "创建监听socket成功！" << std::endl;
    }

    //设置端口复用
    int opt = 1;
    int setSocketOptResult = setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    if(setSocketOptResult < 0)
    {
        std::cerr << "设置端口复用失败!" << std::endl;
        exit(1);
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int bindResult = bind(this->socketfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    if(bindResult < 0)
    {
        std::cerr << "绑定监听socket失败！" << std::endl;

        exit(1);
    }else
    {
        std::cout << "绑定监听socket成功！" << std::endl;
    }

    this->eventLoop->registerServerEvent(this->socketfd, this);     //将this注册进事件循环的服务事件中
}

TcpServer::~TcpServer()
{
    for(std::map<int, TcpSocket *>::iterator iter = this->clientMap.begin(); iter != this->clientMap.end(); ++iter)
    {
        delete iter->second;
        this->clientMap.erase(iter);
    }
}

bool TcpServer::startListen(int listenCount)
{
    int listenResult =  listen(this->socketfd, listenCount);
    if(listenResult < 0)
    {
        return false;
    }

    return true;
}

void TcpServer::registerCallbackNewClientConnection(std::function<void (TcpSocket *)> callbackNewClientConnection)
{
    this->callbackNewClientConnection = std::move(callbackNewClientConnection);
}

void TcpServer::handlerAccept()
{
    struct sockaddr_in clientaddr;
    socklen_t sockaddrLen = sizeof(clientaddr);
    //保存新连接的客户端的信息
    int clientfd = accept(this->socketfd, (sockaddr *)&clientaddr, &sockaddrLen);
    if(clientfd < 0)
    {
        std::cerr << "接受到无效的客户端连接！" << std::endl;
    }else
    {
        std::cout << "客户端[" << inet_ntoa(clientaddr.sin_addr) <<"|" << clientaddr.sin_port << "]连接到服务器" << std::endl;
        //使用通信socket初始化一个通信处理类
        TcpSocket *tcpClient = new TcpSocket(clientfd, this->eventLoop, this->threadPool);

        //将通信对象保存到通信对象（客户端对象）映射
        this->clientMap[clientfd] = tcpClient;
        //将通信对象注册到事件循环的通信事件中
        if(this->eventLoop->registerSocketEvent(clientfd, tcpClient))
        {
            this->callbackNewClientConnection(tcpClient);       //调用新客户端连接回调函数并将通信对象传入
        }
    }
}