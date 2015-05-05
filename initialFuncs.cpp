#include "stdafx.h"
#include "common.h"
#include "cPhyNode.h"
#include "cPhyLink.h"
#include "cTopology.h"
#include "cEvent.h"

#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"


int intialPhyTopology(vector<cPhyNode>* _p_phy_server_vec,vector<cPhyLink>* _p_phy_link_vec,cTopology* _p_phy_topology)
{
	const gsl_rng_type * T;
	gsl_rng * r;
	uint index_phy_server;
	uint i,j;
	double uniform_ran_num;
	ID id;

	gsl_rng_env_setup();
	gsl_rng_default_seed = 10;
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

	
	//Initial physical servers
	for (index_phy_server = 0, id = 1;index_phy_server < total_phy_server;index_phy_server++,id++)
	{
		_p_phy_server_vec->push_back(cPhyNode(id,phy_server_capacity,phy_server_capacity));
	}
	
	//Initial physical links between physical links
	//The circle of each node is forbidden
	id = 1;
	for (i = 0;i<total_phy_server - 1;i++)
	{
		for (j=i+1;j<total_phy_server;j++)
		{
			uniform_ran_num = gsl_rng_uniform(r);
			if (uniform_ran_num < 0.5)
			{
				//establish a link between node i and j;
				cPhyLink tem_phy_link = cPhyLink(id,phy_link_capacity,phy_link_capacity);
				tem_phy_link.setEndSrcPhyNode((&((*_p_phy_server_vec)[i])));
				tem_phy_link.setEndSrcPhyNodeID(i+1);
				tem_phy_link.setEndDesNode((&((*_p_phy_server_vec)[j])));
				tem_phy_link.setEndDesNodeID(j+1);
				_p_phy_link_vec->push_back(tem_phy_link);
				id++;
			}
		}
	}
	
	vector<cPhyLink>::iterator iter_phy_link;
	for (iter_phy_link = _p_phy_link_vec->begin();iter_phy_link!=_p_phy_link_vec->end();iter_phy_link++)
	{
	}
	
	gsl_rng_free(r);

	return 0;
}

int initialRequests(vector<cRequest>* _p_requests)
{

	const gsl_rng_type * T;
	gsl_rng * r,*r_vnf_type,*r_vnf_resource_required,*r_app_chain_resource_required,*r_interval_time,*r_duration_time;
	uint reques_index = 0;
	ID vnf_id = 1;
	ID app_chain_id = 1;
	res_unit vnf_resource_required = 0;
	res_unit app_chain_resource_required = 0;
	int vnf_num = 0;
	int vnf_index= 0;
	int vnf_types=0;
	double arrive_time = 0;
	double duraiton_time = 0;
	double interval_time = 0;

	gsl_rng_env_setup();
	gsl_rng_default_seed = 10;
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);
	r_vnf_type = gsl_rng_alloc(T);
	r_vnf_resource_required = gsl_rng_alloc(T);
	r_app_chain_resource_required = gsl_rng_alloc(T);
	r_interval_time = gsl_rng_alloc(T);
	r_duration_time = gsl_rng_alloc(T);

	for (reques_index = 0;reques_index < total_requests;reques_index++)
	{
		//initial each vnf
		duraiton_time = gsl_ran_exponential(r_duration_time,1/departure_rate);
		cRequest tem_request = cRequest((ID)(reques_index+1),arrive_time,duraiton_time);
		interval_time = gsl_ran_exponential(r_interval_time,1/arrival_rate);
		arrive_time += interval_time;
		vnf_num = gsl_rng_uniform_int(r,max_vnf) + 1;
		
		//initial vnfs for the request indexed by requests_index;
		for (vnf_index = 0;vnf_index < vnf_num;vnf_index++)
		{
			vnf_types = gsl_rng_uniform_int(r_vnf_type,total_vnf_types);
			vnf_resource_required = 0;
			while(vnf_resource_required<1)
			{
				vnf_resource_required = (res_unit)(gsl_rng_uniform(r_vnf_resource_required)*max_node_request_required);
			}
			tem_request.vir_func_app.push_back(cVirtFuncApp(vnf_id,(ID)(reques_index+1),vnf_types,vnf_resource_required));
			vnf_id++;
		}

		//initial each app chain
		vector<cVirtFuncApp>::iterator iter_vnf;
		for (iter_vnf = tem_request.vir_func_app.begin();iter_vnf != tem_request.vir_func_app.end() - 1;iter_vnf++)
		{
			vector<cVirtFuncApp>::iterator iter_next_vnf = iter_vnf + 1;
			app_chain_resource_required = 0;
			while (app_chain_resource_required<1)
			{
				app_chain_resource_required = (res_unit)(gsl_rng_uniform(r_vnf_resource_required)*max_link_request_required);
			}

			cAppChain tem_app_chain = cAppChain(app_chain_id,(ID)(reques_index+1),iter_vnf->getId(),iter_next_vnf->getId(),app_chain_resource_required);
			tem_request.app_chain.push_back(tem_app_chain);
			app_chain_id++;
		}

		_p_requests->push_back(tem_request);
	}




	gsl_rng_free(r);
	gsl_rng_free(r_vnf_type);
	gsl_rng_free(r_vnf_resource_required);
	gsl_rng_free(r_app_chain_resource_required);
	gsl_rng_free(r_interval_time);
	gsl_rng_free(r_duration_time);

	return 0;
}

int establishGlobalIndex(vector<cRequest>* _p_requests_vec)
{
	vector<cRequest>::iterator iter_request_vec;
	vector<cVirtFuncApp>::iterator iter_vnf_vec;
	vector<cAppChain>::iterator iter_app_chain_vec;

	vnf_index_map.clear();
	app_chain_index_map.clear();

	for (iter_request_vec = _p_requests_vec->begin();iter_request_vec != _p_requests_vec->end();iter_request_vec++)
	{
		for (iter_vnf_vec = iter_request_vec->vir_func_app.begin();iter_vnf_vec != iter_request_vec->vir_func_app.end();iter_vnf_vec++)
		{
			vnf_index_map.insert(make_pair(iter_vnf_vec->getId(),&(*iter_vnf_vec)));
		}

		for (iter_app_chain_vec = iter_request_vec->app_chain.begin();iter_app_chain_vec != iter_request_vec->app_chain.end();iter_app_chain_vec++)
		{
			app_chain_index_map.insert(make_pair(iter_app_chain_vec->getId(),&(*iter_app_chain_vec)));
		}
	}
	
	return 0;
}

int initialEventList(vector<cRequest>* _p_requests_vec,multimap<double,cEvent>& _event_vec)
{
	_event_vec.clear();

	vector<cRequest>::iterator iter_request_vec = _p_requests_vec->begin();
	for (;iter_request_vec != _p_requests_vec->end();iter_request_vec++)
	{
		_event_vec.insert(make_pair(iter_request_vec->getArrivalTime(),cEvent(ARRIVING,&(*iter_request_vec))));
		_event_vec.insert(make_pair(iter_request_vec->getArrivalTime()+iter_request_vec->getDuringTime(),cEvent(LEAVING,&(*iter_request_vec))));
	}
	
	return 0;
}