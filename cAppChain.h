#pragma once
#include "cvirtlink.h"

class cPhyLink;

class cAppChain :
	public cVirtLink
{
private:
	ID request_id;
	res_unit res_required;
	bool is_split;

public:
	vector<cPhyLink*> p_host_link_vec;

public:
	void setRequestID(ID _request_id){request_id = _request_id;}
	ID   getResquestID(void){return request_id;}

	void setResRequied(res_unit _res_required){res_required = _res_required;}
	res_unit getResRequired(void){return res_required;}

	void setSplit(bool _is_split){is_split = _is_split;}
	bool isSplit(void){return is_split;}

public:
	cAppChain(void);

	cAppChain(ID _id,ID _request_id, ID _endSrcNodeID, ID _endDesNodeID,res_unit _res_required,cVirtNode* _endSrcNodePoint = NULL,cVirtNode* _endDesNodePoint = NULL)\
		:cVirtLink(_id,_endSrcNodeID,_endDesNodeID,_endSrcNodePoint,_endDesNodePoint),res_required(_res_required),request_id(_request_id),is_split(false){}

	cAppChain(const cAppChain& _app_chain);

	cAppChain& operator=(const cAppChain& _app_chain);

	~cAppChain(void);
};

