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

		this->endSrcNodeID = _link.endSrcNodeID;
		this->endDesNodeID = _link.endDesNodeID;

		this->endSrcNode = _link.endSrcNode;
		this->endDesNode = _link.endDesNode;

		this->linkWeight = _link.linkWeight;
		this->available = _link.available;

		this->parentID = _link.parentID;

	}

	return *this;
}

