#pragma once
#include "cVirtLink.h"
#include "cVirtNode.h"


class cVirtTopology
{

public:
	vector<cVirtNode> virtNode_vec;
	vector<cVirtLink> virtLink_vec;

public:

public:
	cVirtTopology(void);
	cVirtTopology(const cVirtTopology& _topo);
	cVirtTopology& operator=(const cVirtTopology& _topo);
	~cVirtTopology(void);
};

