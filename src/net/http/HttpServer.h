#ifndef NET_HTTP_HTTPSERVER_H
#define NET_HTTP_HTTPSERVER_H

#include "src/net/TcpServer.h"

#include <string>

namespace net
{

class HttpResponse;
class HttpRequest;

class HttpServer : noncopyable
{
public:
	typedef std::function<void (const HttpRequest&,
									HttpResponse*)> HttpCallback;
	
	HttpServer(EventLoop* loop,
				const InetAddress& listenAddr,
				const std::string& name,
				TcpServer::Option option = TcpServer::kNoReusePort);
	
	EventLoop* getLoop() const { return server_.getLoop(); }

	void setHttpCallback(const HttpCallback& cb)
	{ 
		httpCallback_ = cb; 
	}

	void setThreadNum(int numThreads)
	{
		server_.setThreadNum(numThreads);
	}

	void start();
private:
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn,
					Buffer* buf,
					Timestamp reveiveTime);
	void onRequest(const TcpConnectionPtr&, const HttpRequest&);

	TcpServer server_;
	HttpCallback httpCallback_;
};

} // namespace net

#endif
