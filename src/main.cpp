#include <iostream>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <list>
#include <unistd.h>
#include <string.h>

#include "threadpool.h"
#include "sysutils.h"

using namespace std;

const int MAXEVENTSIZE = 2048;

static void do_accept(int epollfd, int listenfd)
{
    struct sockaddr_in sockaddr;
    socklen_t socklen = sizeof(sockaddr_in);
    int clientfd = accept(listenfd, (struct sockaddr *)&sockaddr, &socklen);

    setnonblocking(clientfd);

    task *newtask = new task(epollfd, clientfd);
    event_add(epollfd, clientfd, EPOLLIN | EPOLLET, newtask);

    cout << "new connection...[ip: " << inet_ntoa(sockaddr.sin_addr) << ", port: " << ntohs(sockaddr.sin_port) << "]\n";
}

// static void do_read(int epfd, int sockfd)
// {
//     char buf[1024];
//     string data;
//     while (true)
//     {
//         memset(buf, 0, sizeof(buf));
//         int ret = recv(sockfd, buf, sizeof(buf), 0);
//         if (ret <= 0)
//         {
//             if (ret < 0)
//             {
//                 if (errno == EAGAIN || errno == EWOULDBLOCK)
//                     continue;
//                 cout << strerror(errno) << ", " << errno << endl;
//                 break;
//             }
//             else if (ret == 0)
//             {
//                 event_del(epfd, sockfd);
//                 close(sockfd);
//                 break;
//             }
//         }

//         string tmp(buf, ret);
//         data += tmp;
//         cout << tmp << endl;
//     }
// }

int main(int argc, char *argv[])
{
    // if (argc != 3)
    // {
    //     fprintf(stderr, "Usage:%s ip port\n", argv[0]);
    //     return -1;
    // }

    // char *ip = argv[1];
    // int port = atoi(argv[2]);
    int port = 5188;

    int listenfd;
    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd != -1);

    int use = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &use, sizeof(use));

    sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_port = htons(port);
    // svraddr.sin_addr.s_addr = inet_addr(ip);
    svraddr.sin_addr.s_addr = ntohl(INADDR_ANY);
    int ret = bind(listenfd, (struct sockaddr *)&svraddr, sizeof(svraddr));
    assert(ret == 0);

    ret = listen(listenfd, SOMAXCONN);
    assert(ret == 0);

    int epollfd = epoll_create1(0);
    assert(epollfd > 0);

    threadpool pool(100);
    struct epoll_event clients[MAXEVENTSIZE];

    setnonblocking(listenfd);
    task *listentask = new task(epollfd, listenfd);
    ret = event_add(epollfd, listenfd, EPOLLIN | EPOLLET, listentask);
    assert(ret == 0);

    while (true)
    {
        int nready = epoll_wait(epollfd, clients, MAXEVENTSIZE, -1);
        for (int i = 0; i < nready; i++)
        {
            task *t = (task *)clients[i].data.ptr; // 对象的释放延迟到线程池执行完任务后由线程池释放
            int fd = t->fd();
            if ((fd == listenfd) && (clients[i].events & EPOLLIN))
                do_accept(epollfd, listenfd);
            else if (clients[i].events & EPOLLIN)            
                pool.add_task(t); // 调用线程处理任务
        }
    }
    close(epollfd);
    close(listenfd);
    return 0;
}