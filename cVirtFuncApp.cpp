#include "StdAfx.h"
#include "cVirtFuncApp.h"


cVirtFuncApp::cVirtFuncApp(void)
{
}


cVirtFuncApp::~cVirtFuncApp(void)
{
}

cVirtFuncApp::cVirtFuncApp(const cVirtFuncApp& _virtFuncApp)
{
	this->operator=(_virtFuncApp);
}

cVirtFuncApp& cVirtFuncApp::operator=(const cVirtFuncApp& _virtFuncApp)
{
	if(this!=&_virtFuncApp)
	{
		cVirtNode::operator=(_virtFuncApp);

		request_id = _virtFuncApp.request_id;
		virtFuncApptype = _virtFuncApp.virtFuncApptype;
		res_required = _virtFuncApp.res_required;
		is_split = _virtFuncApp.is_split;

		host_server_id = _virtFuncApp.host_server_id;
		p_host_server = _virtFuncApp.p_host_server;
	}

	return *this;
}