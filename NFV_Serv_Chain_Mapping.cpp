// NFV_Serv_Chain_Mapping.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cPhyNode.h"
#include "cPhyLink.h"
#include "cTopology.h"
#include "common.h"
#include "cEvent.h"

/************************************************************************/
/* Configuration parameters*/
const uint total_vnf_types = 5;
const uint total_phy_server = 10;
const uint total_requests = 100;
const res_unit phy_server_capacity = 100;
const res_unit phy_link_capacity = 100;
const res_unit max_link_request_required = 110;
const res_unit min_link_request_required = 30;
const res_unit max_node_request_required = 130;
const res_unit min_node_request_required = 50;
double arrival_rate = 5;
const double departure_rate = 4;

//whether the resource splitting is used or not
//true->used;false->not used
const bool splitting = false;

//maximum number of sub function pieces for each vnf after being split
const uint max_splitting_piece = 2;

//the initial distance of each node when applying the shortest path algorithm
const int max_distance = 100000;

//The number of virtual network functions (vnf) is randomly distributed and chosen
// and the max number of vnfs for each requests is defined as max_vnf
uint const max_vnf = 5;
/************************************************************************/

/************************************************************************/
/* output metrics*/
double accepted_rate = 0;
/************************************************************************/

/************************************************************************/
/* Global variable*/
map<ID,cVirtFuncApp*> vnf_index_map;
map<ID,cAppChain*> app_chain_index_map;
res_unit total_node_resource_used = 0;
res_unit total_link_resource_used = 0;
/************************************************************************/

int _tmain(int argc, _TCHAR* argv[])
{
	ID request_id;
	for (arrival_rate = 4; arrival_rate <= 10; arrival_rate++)
	{
		total_node_resource_used = 0;
		total_link_resource_used = 0;
		//Initial physical topology
		cTopology* p_phy_topology = new cTopology;
		//vector<cPhyNode>* p_phy_server_vec = new vector<cPhyNode>;
		//vector<cPhyLink>* p_phy_link_vec = new vector<cPhyLink>;
		intialPhyTopology(p_phy_topology);

		list<cRequest>* p_requests_list = new list<cRequest>;
		initialRequests(p_requests_list);

		//establish an mapping from the ID to the point pointing the vnf or the app chain
		establishGlobalIndex(p_requests_list);

		multimap<double, cEvent> event_mulmap;
		initialEventList(p_requests_list, event_mulmap);

		multimap<double, cEvent>::iterator iter_event_mulmap = event_mulmap.begin();
		for (; iter_event_mulmap != event_mulmap.end(); iter_event_mulmap++)
		{
			request_id = iter_event_mulmap->second.getRequestPoint()->getId();
			if (ARRIVING == iter_event_mulmap->second.getEventType())
			{
				//a request arriving
				//allocate resource
				requestAllocating(p_phy_topology, iter_event_mulmap->second.getRequestPoint());
				//requestAllocatingwithSplitting(p_phy_topology,iter_event_mulmap->second.getRequestPoint());
			}
			else
			{
				//release the resources occupied by the request currently leaving
				//first should check whether the request leaving event is triggered by an already served request.
				if ((iter_event_mulmap->second.getRequestPoint())->getIsService())
				{
					releaseResource(p_phy_topology, iter_event_mulmap->second.getRequestPoint());
				}
			}
			
			updateTopologyInfo(p_phy_topology);
		}

		iter_event_mulmap--;
		double last_time = iter_event_mulmap->first;

		outputResults(p_requests_list, p_phy_topology,last_time);

		free(p_phy_topology);
	}

	return 0;
}


