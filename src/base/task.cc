#include <sys/socket.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <fcntl.h>

#include "task.h"
#include "sysutils.h"

using std::cout;
using std::endl;
using std::string;

task::task(int epfd, int fd)
    : m_epfd(epfd),
      m_fd(fd),
      m_content_length(-1),
      m_last_pos(0),
      m_check_state(CHECK_STATE_REQUESTLINE)
{
}

task::~task()
{
    event_del(m_epfd, m_fd);
    close(m_fd);
}

void task::exec()
{
    bool is_parse_completed = false;
    while (true)
    {
        char buf[2048];
        int ret = recv(m_fd, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            if (ret < 0)
            {
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                    continue;
            }
            else if (ret == 0) // 对方关闭连接
            {
                printf("peer client closed.\n");
                // event_del(m_epfd, m_fd);
                // close(m_fd);
            }
            break;
        }

        std::string nowread(buf, ret);
        m_data += nowread;

        is_parse_completed = parse();
        if (is_parse_completed)
            break;
    }

    if (!is_parse_completed)
    {
        cout << "parse error...\n";
        delete this;
        return;
    }

    if (m_method == "GET")
        do_get();
    else if (m_method == "POST")
        do_post();

    delete this;
}

bool task::parse()
{
    LINE_STATUS ret;
    string line;
    while ((ret = parse_line(line)) == LINE_OK)
    {
        switch (m_check_state)
        {
        case CHECK_STATE_REQUESTLINE:
            ret = parse_requestline(line);
            if (ret == LINE_OK)
                m_check_state = CHECK_STATE_HEADER;
            else if (ret == LINE_BAD)
                cout << "requestline error...\n";
            break;
        case CHECK_STATE_HEADER:
            ret = parse_header(line);
            if (ret == LINE_BAD)
                cout << "header error...\n";
            else if (ret == LINE_REACH_HEADER_END)
            {
                m_check_state = CHECK_STATE_BODY;
                if (m_method == "GET") // get请求说明已经处理完了
                    return true;
            }
            break;
        case CHECK_STATE_BODY:
            m_content += (line + "\r\n"); // 补回被parse_line截掉的\r\n
            // 带请求体长度就读字段长度个字节， 不带的话就直接算读完了
            if (m_content_length > 0)
            {
                if ((int)m_content.length() >= m_content_length) // 读到Content-Length个字节，结束
                    return true;
                // 这里代表下次继续读
            }
            else
                return true;
            break;
        }
    }
    return false;
}

task::LINE_STATUS task::parse_line(string &line)
{
    line.clear();
    size_t pos = m_data.find("\r\n", m_last_pos);
    if (pos == string::npos)
    {
        // 处理报文数据部分，直接返回剩下长度部分数据
        if ((m_data.length() - m_last_pos > 0) && (m_check_state == CHECK_STATE_BODY))
        {
            line = m_data.substr(m_last_pos, m_data.length() - m_last_pos);
            m_last_pos = m_data.length();
            return LINE_OK;
        }
        return LINE_AGAIN;
    }
    line = m_data.substr(m_last_pos, pos - m_last_pos);
    m_last_pos = pos + 2;
    return LINE_OK;
}

task::LINE_STATUS task::parse_requestline(const string &line)
{
    size_t pos = line.find(" ", 0);
    if (pos == string::npos)
        return LINE_BAD;
    string method = line.substr(0, pos);
    m_method = method; // 保存请求行信息

    size_t pos1 = line.find(" ", pos + 1);
    if (pos1 == string::npos)
        return LINE_BAD;
    string path = line.substr(pos, pos1 - pos);
    m_path = path;

    string version = line.substr(pos1, line.length() - pos1);
    m_http_version = version;

    return LINE_OK;
}

task::LINE_STATUS task::parse_header(const string &line)
{
    if (line.length() <= 0)
        return LINE_REACH_HEADER_END;
    size_t pos = line.find(":", 0);
    if (pos == string::npos)
        return LINE_BAD;
    string key = line.substr(0, pos);
    string val = line.substr(pos + 1, line.length() - pos);
    m_header[key] = val; // 保存请求头部信息
    if (key == "Content-Length")
        m_content_length = atoi(val.c_str());
    // cout << key << ": " << val << endl;
    return LINE_OK;
}

void task::do_get()
{
    cout << "request resource: " << m_path << ",  fd: " << m_fd << endl;

    // 删除空格
    size_t found = m_path.find(" ");
    while ((found = m_path.find(" ")) != string::npos)
        m_path.erase(found, 1);

    // 获取文件名
    string filepath;
    found = m_path.find("?");
    if (found == string::npos)
        filepath = m_path.substr(1, m_path.length() - 1);
    else
        filepath = m_path.substr(1, found - 1).c_str();

    if (filepath.empty())
        filepath = "index.html";

    // 拼接响应报文
    int offset = 0;
    char buf[1024];
    int fd = open(filepath.c_str(), O_RDONLY);
    if (fd == -1) // 资源没有找到
    {
        offset += sprintf(buf, "%s %d %s\r\n", m_http_version.substr(1, m_http_version.length() - 1).c_str(), HTTP_RESOURCE_NOT_FOUND, "NOT FOUND");
        offset += sprintf(buf + offset, "%s: %s\r\n", "Server", "hongfu's server - g5/1.0");
        send(m_fd, buf, strlen(buf), 0);
        return;
    }

    offset += sprintf(buf, "%s %d %s\r\n", m_http_version.substr(1, m_http_version.length() - 1).c_str(), HTTP_OK, "OK");
    offset += sprintf(buf + offset, "%s: %s\r\n", "Server", "hongfu's server - g5/1.0");
    offset += sprintf(buf + offset, "%s: %s\r\n\r\n", "Content-Type", "text/html;charset=utf-8");
    send(m_fd, buf, strlen(buf), 0);

    while (true)
    {
        char filebuf[1024];
        int nread = read(fd, filebuf, sizeof(filebuf));
        if (nread <= 0)
        {
            if (nread == 0)
                break;
            else if (nread == -1)
            {
                if (errno == EINTR)
                    continue;
                break;
            }
        }
        send(m_fd, filebuf, nread, 0);
    }
    close(fd);

    send(m_fd, "\r\n", strlen("\r\n"), 0);
}

void task::do_post()
{
    cout << "request resource: " << m_path << endl;
    cout << "param: \n";
    cout << m_content;
}