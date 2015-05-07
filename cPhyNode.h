#pragma once
#include "cVirtNode.h"
#include "cVirtFuncApp.h"

class cPhyLink;

class cPhyNode:
	public cVirtNode
{

private:
	res_unit capacity;
	res_unit residual;

public:
	void setCapacity(res_unit _capacity){capacity = _capacity;}
	res_unit getCapacity(void){return capacity;}

	void setResidual(res_unit _residual){residual = _residual;}
	res_unit getResidual(void){return residual;}

public:
	bool havingSufficientRes(res_unit _res_required){return _res_required <= residual;}
	int  allocateResource(cVirtFuncApp& _vnf);
	int  releaseResource(cVirtFuncApp& _vnf);

public:
	cPhyNode(void);

	cPhyNode(cVirtNode& _virNode);

	cPhyNode(ID _id,res_unit _capacity,res_unit _residual):cVirtNode(_id),capacity(_capacity),residual(_residual){}

	cPhyNode(const cPhyNode& _node){operator=(_node);}

	cPhyNode& operator=(const cPhyNode& _node);

	~cPhyNode(void);
};

