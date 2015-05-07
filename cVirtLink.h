#pragma once

class cVirtNode;

class cVirtLink
{
protected:
	ID id;
	ID endSrcPhyNodeID;
	ID endDesNodeID;
	cVirtNode* endSrcPhyNode;
	cVirtNode* endDesNode;
	int linkWeight;
	bool available;

public:
	void setId(ID _id){id = _id;}
	ID getId(void) const {return id;}

	void setEndSrcNodeID(ID _id) {endSrcPhyNodeID = _id;}
	ID   getEndSrcNodeID(void) const {return endSrcPhyNodeID;}

	void setEndDesNodeID(ID _id) {endDesNodeID = _id;}
	ID   getEndDesNodeID(void) const {return endDesNodeID;}

	void setEndSrcNode(void* _pnode){endSrcPhyNode = (cVirtNode*)_pnode;}
	void* getEndSrcNode(void) const {return endSrcPhyNode;}

	void setEndDesNode(void* _pnode){endDesNode = (cVirtNode*)_pnode;}
	void* getEndDesNode(void) const {return endDesNode;}

	void setLinkWeight(int _weight){linkWeight = _weight;}
	int  getLinkWeight(void){return linkWeight;}

	void setAvailable(bool _available){available = _available;}
	bool getAvailable(void){return available;}

public:
	cVirtLink(void);
	~cVirtLink(void);
	cVirtLink(ID _id, ID _endSrcPhyNodeID, ID _endDesNodeID,void* _endSrcPhyNodePoint = NULL,void* _endDesNodePoint = NULL,int _link_weight = 1,bool _available = true)\
		:id(_id),endSrcPhyNodeID(_endSrcPhyNodeID),endDesNodeID(_endDesNodeID),endSrcPhyNode((cVirtNode*)_endSrcPhyNodePoint),endDesNode((cVirtNode*)_endDesNodePoint),linkWeight(_link_weight),available(_available){}
	cVirtLink(const cVirtLink& _link);
	cVirtLink & operator=(const cVirtLink& _link);
};

