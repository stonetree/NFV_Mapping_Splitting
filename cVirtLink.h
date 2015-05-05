#pragma once

class cVirtNode;

class cVirtLink
{
private:
	ID id;
	ID endSrcPhyNodeID;
	ID endDesNodeID;
	cVirtNode* endSrcPhyNode;
	cVirtNode* endDesNode;

public:
	void setId(ID _id){id = _id;}
	ID getId(void) const {return id;}

	void setEndSrcPhyNodeID(ID _id) {endSrcPhyNodeID = _id;}
	ID   getEndSrcPhyNodeID(void) const {return endSrcPhyNodeID;}

	void setEndDesNodeID(ID _id) {endDesNodeID = _id;}
	ID   getEndDesNodeID(void) const {return endDesNodeID;}

	void setEndSrcPhyNode(void* _pnode){endSrcPhyNode = (cVirtNode*)_pnode;}
	void* getEndSrcPhyNode(void) const {return endSrcPhyNode;}

	void setEndDesNode(void* _pnode){endDesNode = (cVirtNode*)_pnode;}
	void* getEndDesNode(void) const {return endDesNode;}

public:
	cVirtLink(void);
	~cVirtLink(void);
	cVirtLink(ID _id, ID _endSrcPhyNodeID, ID _endDesNodeID,void* _endSrcPhyNodePoint = NULL,void* _endDesNodePoint = NULL)\
		:id(_id),endSrcPhyNodeID(_endSrcPhyNodeID),endDesNodeID(_endDesNodeID),endSrcPhyNode((cVirtNode*)_endSrcPhyNodePoint),endDesNode((cVirtNode*)_endDesNodePoint){}
	cVirtLink(const cVirtLink& _link);
	cVirtLink & operator=(const cVirtLink& _link);
};

