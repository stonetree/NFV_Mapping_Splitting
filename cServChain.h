#pragma once
#include "cAppChain.h"
#include "cVirtFuncApp.h"

class cServChain
{

public:
	list<cVirtFuncApp> vir_func_app;
	list<cAppChain> app_chain;

public:
	cServChain(void);

	cServChain(const cServChain& _service_chain){operator=(_service_chain);}

	cServChain& operator=(const cServChain& _service_chain);

	~cServChain(void);
};

