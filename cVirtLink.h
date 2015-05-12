#pragma once

class cVirtNode;

class cVirtLink
{
protected:
	ID id;
	ID endSrcNodeID;
	ID endDesNodeID;
	cVirtNode* endSrcNode;
	cVirtNode* endDesNode;
	int linkWeight;
	bool available;
	ID parentID;

public:
	void setId(ID _id){id = _id;}
	ID getId(void) const {return id;}

	void setEndSrcNodeID(ID _id) {endSrcNodeID = _id;}
	ID   getEndSrcNodeID(void) const {return endSrcNodeID;}

	void setEndDesNodeID(ID _id) {endDesNodeID = _id;}
	ID   getEndDesNodeID(void) const {return endDesNodeID;}

	void setEndSrcNode(void* _pnode){endSrcNode = (cVirtNode*)_pnode;}
	void* getEndSrcNode(void) const {return endSrcNode;}

	void setEndDesNode(void* _pnode){endDesNode = (cVirtNode*)_pnode;}
	void* getEndDesNode(void) const {return endDesNode;}

	void setLinkWeight(int _weight){linkWeight = _weight;}
	int  getLinkWeight(void){return linkWeight;}

	void setAvailable(bool _available){available = _available;}
	bool getAvailable(void){return available;}

	void setParentID(ID _id){parentID = _id;}
	ID   getParentID(void){return parentID;}

public:
	cVirtLink(void);
	~cVirtLink(void);
	cVirtLink(ID _id, ID _endSrcNodeID, ID _endDesNodeID,void* _endSrcNodePoint = NULL,void* _endDesNodePoint = NULL,int _link_weight = 1,bool _available = true,ID _parent_id = 0)\
		:id(_id),endSrcNodeID(_endSrcNodeID),endDesNodeID(_endDesNodeID),endSrcNode((cVirtNode*)_endSrcNodePoint),endDesNode((cVirtNode*)_endDesNodePoint),linkWeight(_link_weight),available(_available),parentID(_parent_id){}
	cVirtLink(const cVirtLink& _link);
	cVirtLink & operator=(const cVirtLink& _link);
};

