#include "StdAfx.h"
#include "cPhyNode.h"

#include "cVirtNode.h"


cPhyNode::cPhyNode(void)
{
}

cPhyNode::cPhyNode(cVirtNode& _virNode):cVirtNode(_virNode)
{

}

//cPhyNode::cPhyNode(ID _id)
//{
//	id = _id;
//}

//cPhyNode::cPhyNode(const cPhyNode& _node)
//{
//	operator=(_node);
//}

cPhyNode& cPhyNode::operator=(const cPhyNode& _node)
{
	if (this!= &_node)
	{
		cVirtNode::operator=(_node);
		capacity = _node.capacity;
		residual = _node.residual;
		//this->id = _node.id;

		//this->adjacentLinkID_vec.erase(adjacentLinkID_vec.begin(),adjacentLinkID_vec.end());
		//this->adjacentLinkID_vec.assign(_node.adjacentLinkID_vec.begin(),_node.adjacentLinkID_vec.end());

		//this->adjacentLink_vec.erase(adjacentLink_vec.begin(),adjacentLink_vec.end());
		//this->adjacentLink_vec.assign(_node.adjacentLink_vec.begin(),_node.adjacentLink_vec.end());
	}

	return *this;
}

cPhyNode::~cPhyNode(void)
{
}
