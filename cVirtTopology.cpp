#include "StdAfx.h"
#include "cVirtTopology.h"


cVirtTopology::cVirtTopology(void)
{
}


cVirtTopology::cVirtTopology(const cVirtTopology& _topo)
{
	operator=(_topo);
}

cVirtTopology& cVirtTopology::operator=(const cVirtTopology& _topo)
{
	if (this!=&_topo)
	{
		//this->pnode = _topo.pnode;
		//this->plink = _topo.plink;

		this->virtNode_vec.erase(virtNode_vec.begin(),virtNode_vec.end());
		this->virtNode_vec.assign(_topo.virtNode_vec.begin(),_topo.virtNode_vec.end());

		this->virtLink_vec.erase(virtLink_vec.begin(),virtLink_vec.end());
		this->virtLink_vec.assign(_topo.virtLink_vec.begin(),_topo.virtLink_vec.end());
	}

	return *this;
}

cVirtTopology::~cVirtTopology(void)
{
}

//int cVirtTopology::createVirtTopology(cTopology& _topo)
//{
//	vector<cPhyNode>::iterator iter_node_vec;
//	vector<cPhyLink>::iterator iter_link_vec;
//
//	virtNode_vec.erase(virtNode_vec.begin(),virtNode_vec.end());
//	virtLink_vec.erase(virtLink_vec.begin(),virtLink_vec.end());
//	
//	for (iter_node_vec = _topo.phyNode_vec.begin();iter_node_vec!=_topo.phyNode_vec.end();iter_node_vec++)
//	{
//		cVirtNode tem_virNode(iter_node_vec->getId(),iter_node_vec->getDistance(),iter_node_vec->getPreNodeID());
//
//		tem_virNode.adjacentLinkID_vec.assign(iter_node_vec->adjacentLinkID_vec.begin(),iter_node_vec->adjacentLinkID_vec.end());
//		tem_virNode.adjacentLink_vec.assign(iter_node_vec->adjacentLink_vec.begin(),iter_node_vec->adjacentLink_vec.end());
//
//		virtNode_vec.push_back(tem_virNode);
//	}
//}

cVirtLink* isDuplicate(vector<cVirtLink>& _virtLink_vec,int _endSrcPhyNodeID,int _endDesNodeID)
{
	vector<cVirtLink>::iterator iter_virtLink_vec = _virtLink_vec.begin();

	for (;iter_virtLink_vec!=_virtLink_vec.end();iter_virtLink_vec++)
	{
		if (iter_virtLink_vec->getEndSrcPhyNodeID() == _endSrcPhyNodeID && iter_virtLink_vec->getEndDesNodeID() == _endDesNodeID)
			return &(*iter_virtLink_vec);
	}
	return NULL;
}
