#ifndef TASK_H_
#define TASK_H_

#include <string>
#include <map>

class threadpool;

class task
{
public:
    enum LINE_STATUS
    {
        LINE_AGAIN,
        LINE_OK,
        LINE_BAD,
        LINE_REACH_HEADER_END,
        LINE_REACH_BODY_END
    };

    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_BODY
    };

    enum PARSE_STATUS
    {
        PARSE_OK,

        PARSE_COMPLETED,
    };

    task(int epfd, int fd);
    virtual ~task();

    virtual void exec();

    int fd() const { return m_fd; }

private:
    friend class threadpool;

    bool parse();
    LINE_STATUS parse_line(std::string &line);
    LINE_STATUS parse_requestline(const std::string &line); // 解析请求行
    LINE_STATUS parse_header(const std::string &line);      // 解析请求头

    void do_post();
    void do_get();

    int m_epfd;
    int m_fd;

    std::string m_data;
    std::string m_content;
    int m_content_length;

    unsigned int m_last_pos; // 当前解析
    CHECK_STATE m_check_state;
    std::map<std::string, std::string> m_header; // http头部信息
    std::string m_method;
    std::string m_path;
    std::string m_http_version;
};
#endif