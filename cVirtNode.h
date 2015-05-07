#pragma once

#include "common.h"
#include "cVirtLink.h"
#include "cVirtNode.h"

class cVirtNode
{
protected:
	ID id;
	ID preNodeID;
	cVirtNode* preNodePoint;
	int nodeDistance;

public:
	map<pair<ID,ID>,cVirtLink*> adjacent_link_map;

public:
	void setId(ID _id) {id = _id;}
	ID   getId(void) const {return id;}

	void setPreNodeID(ID _id){preNodeID = _id;}
	ID   getPreNodeID(void){return preNodeID;}

	void setPreNodePoint(cVirtNode* _pnode){preNodePoint = _pnode;}
	cVirtNode* getPreNodePoint(void){return preNodePoint;}

	void setNodeDistance(int _distance){nodeDistance = _distance;}
	int  getNodeDistance(void){return nodeDistance;}

public:
	cVirtNode(void);
	cVirtNode(ID _id,ID _preNodeID = 0, cVirtNode* _preNodePoint = NULL,int _node_distance = max_distance):id(_id),preNodeID(_preNodeID),preNodePoint(_preNodePoint),nodeDistance(_node_distance){}
	cVirtNode(const cVirtNode& _node);
	cVirtNode& operator=(const cVirtNode& _node);
	~cVirtNode(void);
};

