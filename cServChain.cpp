#include "StdAfx.h"
#include "cServChain.h"


cServChain::cServChain(void)
{
}


cServChain::~cServChain(void)
{
}

cServChain& cServChain::operator=(const cServChain& _service_chain)
{
	if (this!=&_service_chain)
	{
		vir_func_app.clear();
		vir_func_app.assign(_service_chain.vir_func_app.begin(),_service_chain.vir_func_app.end());

		app_chain.clear();
		app_chain.assign(_service_chain.app_chain.begin(),_service_chain.app_chain.end());
	}

	return *this;
}