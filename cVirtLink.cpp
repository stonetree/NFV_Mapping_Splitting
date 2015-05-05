#include "StdAfx.h"
#include "cVirtLink.h"


cVirtLink::cVirtLink(void)
{
}


cVirtLink::~cVirtLink(void)
{
}

cVirtLink::cVirtLink(const cVirtLink& _link)
{
	operator=(_link);
}

cVirtLink& cVirtLink::operator=(const cVirtLink& _link)
{
	if(this!=&_link)
	{
		this->id = _link.id;

		this->endSrcPhyNodeID = _link.endSrcPhyNodeID;
		this->endDesNodeID = _link.endDesNodeID;

		this->endSrcPhyNode = _link.endSrcPhyNode;
		this->endDesNode = _link.endDesNode;

	}

	return *this;
}

