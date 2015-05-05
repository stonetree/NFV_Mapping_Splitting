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
	vector<cPhyNode*> p_phyNode_vec;
	vector<cPhyLink*> p_phyLink_vec;

public:

public:
	cTopology(void);
	cTopology(const cTopology& _topo);
	cTopology& operator=(const cTopology& _topo);
	~cTopology(void);
};

