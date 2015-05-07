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

int  cPhyNode::allocateResource(cVirtFuncApp& _vnf)
{
	residual -= _vnf.getResRequired();

	if (residual < 0)
	{
		cout<<"Error, the residual resources is less than 0 at physical node "<<id<<endl;
	}
	return 0;
}

int cPhyNode::releaseResource(cVirtFuncApp& _vnf)
{
	residual += _vnf.getResRequired();

	if (residual < 0)
	{
		cout<<"Error, the residual resources is more than the capacity at physical node "<<id<<endl;
	}
	return 0;
}