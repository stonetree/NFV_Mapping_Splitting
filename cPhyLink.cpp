#include "StdAfx.h"
#include "cPhyLink.h"


cPhyLink& cPhyLink::operator=(const cPhyLink& _link)
{
	if(this!=&_link)
	{
		//this->id = _link.id;

		//this->endSrcNodeID = _link.endSrcNodeID;
		//this->endDesNodeID = _link.endDesNodeID;

		//this->endSrcNode = _link.endSrcNode;
		//this->endDesNode = _link.endDesNode;

		cVirtLink::operator=(_link);

		res_capacity = _link.res_capacity;
		res_residual = _link.res_residual;

	}

	return *this;
}

cPhyLink::~cPhyLink(void)
{
}

int cPhyLink::allcateResource(cAppChain& _app_chain)
{
	res_residual -= _app_chain.getResRequired();
	if (res_residual < 0)
	{
		cout<<"Error!!! The number of residual resources is less than 0 at physical link "<<id<<endl;
		cout<<"The ID of app chain is "<<_app_chain.getResquestID()<<" "<<"and the request ID is "<<_app_chain.getResquestID()<<endl;
		exit(0);
	}
	return 0;
}

int cPhyLink::releaseResource(cAppChain& _app_chain)
{
	res_residual += _app_chain.getResRequired();
	if (res_residual > res_capacity)
	{
		cout<<"Error!!! The number of residual resources is more than the capacity"<<endl;
		cout<<"The ID of app chain is "<<_app_chain.getResquestID()<<" "<<"and the request ID is "<<_app_chain.getResquestID()<<endl;
		exit(0);
	}
	return 0;
}