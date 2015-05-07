#pragma once
#include "cvirtlink.h"

class cPhyLink;

class cAppChain :
	public cVirtLink
{
private:
	ID request_id;
	res_unit res_required;

public:
	vector<cPhyLink*> p_host_link_vec;

public:
	void setRequestID(ID _request_id){request_id = _request_id;}
	ID   getResquestID(void){return request_id;}

	void setResRequied(res_unit _res_required){res_required = _res_required;}
	res_unit getResRequired(void){return res_required;}

public:
	cAppChain(void);

	cAppChain(ID _id,ID _request_id, ID _endSrcPhyNodeID, ID _endDesNodeID,res_unit _res_required,cVirtNode* _endSrcPhyNodePoint = NULL,cVirtNode* _endDesNodePoint = NULL)\
		:cVirtLink(_id,_endSrcPhyNodeID,_endDesNodeID),res_required(_res_required),request_id(_request_id){}

	cAppChain(const cAppChain& _app_chain);

	cAppChain& operator=(const cAppChain& _app_chain);

	~cAppChain(void);
};

