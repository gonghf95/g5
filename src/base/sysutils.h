#ifndef SYSUTILS_H_
#define SYSUTILS_H_

#include <unistd.h>

#define HTTP_OK 200
#define HTTP_RESOURCE_NOT_FOUND 404

#define ERR_EXIT(msg)       \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int setnonblocking(int fd);
// int event_add(int epollfd, int fd, unsigned int events);
int event_add(int epollfd, int fd, unsigned int events, void *data);
int event_del(int epollfd, int fd);

#endif