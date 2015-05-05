#pragma once
#include "cvirtlink.h"

class cPhyLink;

class cAppChain :
	public cVirtLink
{
private:
	ID request_id;
	res_unit res_required;
	ID host_link_id;
	cPhyLink* p_host_link;

public:
	void setRequestID(ID _request_id){request_id = _request_id;}
	ID   getResquestID(void){return request_id;}

	void setResRequied(res_unit _res_required){res_required = _res_required;}
	res_unit getResRequired(void){return res_required;}

	void setHostLinkID(ID _host_id){host_link_id = _host_id;}
	ID   getHostLinkID(void){return host_link_id;}

	void setHostLinkPoint(cPhyLink* _p_host_link){p_host_link = _p_host_link;}
	cPhyLink* getHostLinkPoint(void){return p_host_link;}

public:
	cAppChain(void);

	cAppChain(ID _id,ID _request_id, ID _endSrcPhyNodeID, ID _endDesNodeID,res_unit _res_required,cVirtNode* _endSrcPhyNodePoint = NULL,cVirtNode* _endDesNodePoint = NULL)\
		:cVirtLink(_id,_endSrcPhyNodeID,_endDesNodeID),res_required(_res_required),request_id(_request_id),\
	host_link_id(0),p_host_link(NULL){}

	cAppChain(const cAppChain& _app_chain);

	cAppChain& operator=(const cAppChain& _app_chain);

	~cAppChain(void);
};

