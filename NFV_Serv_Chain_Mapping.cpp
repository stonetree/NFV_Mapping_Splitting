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
const res_unit max_link_request_required = 30;
const res_unit max_node_request_required = 20;
const double arrival_rate = 5;
const double departure_rate = 4;

//the initial distance of each node when applying the shortest path algorithm
const int max_distance = 100000;

//The number of virtual network functions (vnf) is randomly distributed and chosen
// and the max number of vnfs for each requests is defined as max_vnf
uint const max_vnf = 5;

/************************************************************************/

/************************************************************************/
/* Global variable*/
map<ID,cVirtFuncApp*> vnf_index_map;
map<ID,cAppChain*> app_chain_index_map;
/************************************************************************/

int _tmain(int argc, _TCHAR* argv[])
{
	//Initial physical topology
	cTopology* p_phy_topology = new cTopology;
	//vector<cPhyNode>* p_phy_server_vec = new vector<cPhyNode>;
	//vector<cPhyLink>* p_phy_link_vec = new vector<cPhyLink>;
	intialPhyTopology(p_phy_topology);

	vector<cRequest>* p_requests_vec = new vector<cRequest>;
	initialRequests(p_requests_vec);
	
	//establish an mapping from the ID to the point pointing the vnf or the app chain
	establishGlobalIndex(p_requests_vec);

	multimap<double,cEvent> event_mulmap;
	initialEventList(p_requests_vec,event_mulmap);

	multimap<double,cEvent>::iterator iter_event_mulmap = event_mulmap.begin();
	for (;iter_event_mulmap != event_mulmap.end();iter_event_mulmap++)
	{
		if (ARRIVING ==iter_event_mulmap->second.getEventType())
		{
			//a request arriving
			//allocate resource
			requestAllocating(p_phy_topology,iter_event_mulmap->second.getRequestPoint());
		}
		else
		{
			//a request leaving
			//release resource
			if ((iter_event_mulmap->second.getRequestPoint())->getIsService())
			{
				releaseResource(iter_event_mulmap->second.getRequestPoint());
			}
		}
	}
	
	free(p_phy_topology);
	return 0;
}

