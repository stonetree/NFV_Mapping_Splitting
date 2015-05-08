#pragma once

#include "cVirtLink.h"
#include "cRequest.h"
#include "cPhyNode.h"

class cPhyLink:
	public cVirtLink
{
private:
	res_unit res_capacity;
	res_unit res_residual;

public:
	void setCapacity(res_unit _capacity){res_capacity = _capacity;}
	res_unit getCapacity(void){return res_capacity;}

	void setResidual(res_unit _residual){res_residual = _residual;}
	res_unit getResidual(void){return res_residual;}

public:
	bool havingSufficientRes(res_unit _res_required){return _res_required <= res_residual;}
	int  allcateResource(cAppChain& _app_chain);
	int  releaseResource(cAppChain& _app_chain);

public:
	cPhyLink(void):cVirtLink(){}

	cPhyLink(ID _id, res_unit _capacity,res_unit _residual,ID _endSrcNodeID = 0, ID _endDesNodeID = 0,cPhyNode* _endSrcNode = NULL, cPhyNode* _endDesNode = NULL)\
		:cVirtLink(_id, _endSrcNodeID, _endDesNodeID, (cVirtNode*)_endSrcNode,(cVirtNode*)_endDesNode),res_capacity(_capacity),res_residual(_residual){}

	cPhyLink(const cPhyLink& _link){operator=(_link);}

	cPhyLink & operator=(const cPhyLink& _link);

	~cPhyLink(void);
};

