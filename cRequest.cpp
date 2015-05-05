#include "StdAfx.h"
#include "cRequest.h"


cRequest::cRequest(void)
{
}

cRequest::cRequest(const cRequest& _request)
{
	operator=(_request);
}

cRequest& cRequest::operator=(const cRequest& _request)
{
	if (this!=&_request)
	{
		cServChain::operator=(_request);
		
		this->id = _request.id;

		this->startingTime = _request.startingTime;
		this->durationTime = _request.durationTime;

		this->isService = _request.isService;

	}

	return *this;
}
cRequest::~cRequest(void)
{
}