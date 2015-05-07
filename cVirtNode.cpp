#include "StdAfx.h"
#include "cVirtNode.h"

cVirtNode::cVirtNode(void)
{
}

cVirtNode::cVirtNode(const cVirtNode& _node)
{
	operator=(_node);
}

cVirtNode& cVirtNode::operator=(const cVirtNode& _node)
{
	if (this!= &_node)
	{
		this->id = _node.id;
		this->preNodeID = _node.preNodeID;
		this->preNodePoint = _node.preNodePoint;
		this->nodeDistance = _node.nodeDistance;

		this->adjacent_link_map.clear();
		this->adjacent_link_map.insert(_node.adjacent_link_map.begin(),_node.adjacent_link_map.end());
	}

	return *this;
}

cVirtNode::~cVirtNode(void)
{
}
