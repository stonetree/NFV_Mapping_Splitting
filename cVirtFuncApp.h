#pragma once
#include "cvirtnode.h"

class cPhyNode;

class cVirtFuncApp :
	public cVirtNode
{
private:
	ID request_id;
	int virtFuncApptype;
	res_unit res_required;
	bool is_split;

	ID host_server_id;
	cPhyNode* p_host_server;


public:
	void setRequestID(ID _request_id){request_id = _request_id;}
	ID   getResquestID(void){return request_id;}

	void setVirtFuncApptype(int _type){virtFuncApptype = _type;}
	int  getVirtFuncAppType(void){return virtFuncApptype;}

	void setResRequired(res_unit _res_required){res_required = _res_required;}
	res_unit getResRequired(void){return res_required;}

	void setHostServerID(ID _host_server_id){host_server_id = _host_server_id;}
	ID   getHostServerID(void){return host_server_id;}

	void setHostServerPoint(cPhyNode* _p_host_server){p_host_server = _p_host_server;}
	cPhyNode* getHostServerPoint(void){return p_host_server;}

	void setSplit(bool _split){is_split = _split;}
	bool isSplit(void){return is_split;}

public:
	cVirtFuncApp(void);

	cVirtFuncApp(ID _id,ID _request_id,int _vir_func_type,res_unit _res_required)\
		:request_id(_request_id),virtFuncApptype(_vir_func_type),res_required(_res_required),cVirtNode(_id),\
	host_server_id(0),p_host_server(NULL),is_split(false){}

	cVirtFuncApp(const cVirtFuncApp& _virtFuncApp);

	cVirtFuncApp& operator=(const cVirtFuncApp& _virtFuncApp);

	~cVirtFuncApp(void);
};

