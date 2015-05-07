#pragma once

#include "stdafx.h"

#include "cPhyNode.h"
#include "cPhyLink.h"
#include "cVirtTopology.h"
#include "cVirtNode.h"
#include "cVirtLink.h"

class cTopology
{
private:
	//cPhyNode* pnode;
	//cPhyLink* plink;

public:
	map<ID,cPhyNode>* p_phyNode_map;
	map<pair<ID,ID>,cPhyLink>* p_phyLink_map;

public:
	int findShortestPath(cPhyNode* _p_src_node, cPhyNode* _p_des_node,vector<cPhyNode*>& _path);
	int outputShortestPath(cPhyNode* _pnode,vector<cPhyNode*>& _path);
	int markAdjacentNode(cPhyNode* _startPoint);
	int updateAvailableLink(cAppChain& _app_chain);

public:
	cTopology(void);
	cTopology(const cTopology& _topo);
	cTopology& operator=(const cTopology& _topo);
	~cTopology(void);
};

