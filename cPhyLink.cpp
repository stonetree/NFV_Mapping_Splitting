#include "StdAfx.h"
#include "cPhyLink.h"


cPhyLink& cPhyLink::operator=(const cPhyLink& _link)
{
	if(this!=&_link)
	{
		//this->id = _link.id;

		//this->endSrcPhyNodeID = _link.endSrcPhyNodeID;
		//this->endDesNodeID = _link.endDesNodeID;

		//this->endSrcPhyNode = _link.endSrcPhyNode;
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
