#pragma once

#include "cVirtLink.h"
#include "cVirtNode.h"

class cVirtNode
{
private:
	ID id;
	ID preNodeID;
	cVirtNode* preNodePoint;

public:
	list<cVirtLink*> adjacentLink_list;
	list<ID> adjacentLinkID_list;

public:
	void setId(ID _id) {id = _id;}
	ID   getId(void) const {return id;}

	void setPreNodeID(ID _id){preNodeID = _id;}
	ID   getPreNodeID(void){return preNodeID;}

	void setPreNodePoint(cVirtNode* _pnode){preNodePoint = _pnode;}
	cVirtNode* getPreNodePoint(void){return preNodePoint;}

public:
	cVirtNode(void);
	cVirtNode(ID _id,ID _preNodeID = 0, cVirtNode* _preNodePoint = NULL):id(_id),preNodeID(_preNodeID),preNodePoint(_preNodePoint){}
	cVirtNode(const cVirtNode& _node);
	cVirtNode& operator=(const cVirtNode& _node);
	~cVirtNode(void);
};

