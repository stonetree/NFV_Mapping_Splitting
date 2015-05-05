#include "StdAfx.h"
#include "cAppChain.h"


cAppChain::cAppChain(void)
{
}


cAppChain::~cAppChain(void)
{
}

cAppChain::cAppChain(const cAppChain& _app_chain)
{
	this->operator=(_app_chain);
}

cAppChain& cAppChain::operator=(const cAppChain& _app_chain)
{
	if (this!=&_app_chain)
	{
		cVirtLink::operator=(_app_chain);

		request_id = _app_chain.request_id;
		res_required = _app_chain.res_required;
		host_link_id = _app_chain.host_link_id;
		p_host_link = _app_chain.p_host_link;
	}

	return *this;
}
