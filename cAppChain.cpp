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
		is_split = _app_chain.is_split;

		p_host_link_vec.clear();
		p_host_link_vec.assign(_app_chain.p_host_link_vec.begin(),_app_chain.p_host_link_vec.end());
	}

	return *this;
}
