#include <fcntl.h>
#include <sys/epoll.h>

#include "sysutils.h"

int setnonblocking(int fd)
{
    int flag = fcntl(fd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
    return flag;
}

int event_add(int epollfd, int fd, unsigned int events)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;
    return epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

int event_add(int epollfd, int fd, unsigned int events, void *data)
{
    struct epoll_event ev;
    ev.data.ptr = data;
    ev.events = events;
    return epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

int event_del(int epollfd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    return epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}