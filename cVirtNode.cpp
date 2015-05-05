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

		this->adjacentLinkID_list.erase(adjacentLinkID_list.begin(),adjacentLinkID_list.end());
		this->adjacentLinkID_list.assign(_node.adjacentLinkID_list.begin(),_node.adjacentLinkID_list.end());

		this->adjacentLink_list.erase(adjacentLink_list.begin(),adjacentLink_list.end());
		this->adjacentLink_list.assign(_node.adjacentLink_list.begin(),_node.adjacentLink_list.end());
	}

	return *this;
}

cVirtNode::~cVirtNode(void)
{
}
