#pragma once
#include "cVirtLink.h"
#include "cVirtNode.h"


class cVirtTopology
{

public:
	map<ID,cVirtNode>* p_virtNode_map;
	map<ID,cVirtLink>* p_virtLink_map;

public:
	int findShortestPath(cVirtNode* _p_src_node, cVirtNode* _p_des_node,vector<cVirtNode*>& _path);
	int outputShortestPath(cVirtNode* _pnode,vector<cVirtNode*>& _path);
	int markAdjacentNode(cVirtNode* _startPoint);
public:
	cVirtTopology(void);
	cVirtTopology(const cVirtTopology& _topo);
	cVirtTopology& operator=(const cVirtTopology& _topo);
	~cVirtTopology(void);
};

