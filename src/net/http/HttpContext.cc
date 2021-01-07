#include "src/net/http/HttpContext.h"
#include "src/net/Buffer.h"

using namespace net;

bool HttpContext::processRequestLine(const char* begin, const char* end)
{
	bool succeed = false;
	const char* start = begin;
	const char* space = std::find(start, end, ' ');
	if(space != end && request_.setMethod(start, space))
	{
		start = space+1;
		space = std::find(start, end, ' ');
		if(space != end)
		{
			const char* question = std::find(start, space, '?');
			if(question != space)
			{
				request_.setPath(start, question);
				request_.setQuery(question, space);
			}
			else
			{
				request_.setPath(start, space);
			}
		}
	}
}
