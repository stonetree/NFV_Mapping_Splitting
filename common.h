#pragma once
#include "stdafx.h"

class cEvent;
class cPhyLink;
class cPhyNode;
class cTopology;
class cVirtFuncApp;
class cVirtNode;
class cVirtLink;
class cAppChain;
class cRequest;

/************************************************************************/
/* Configuration parameters*/
extern const uint total_vnf_types;
extern const uint total_phy_server;
extern const uint total_requests;
extern const res_unit phy_server_capacity;
extern const res_unit phy_link_capacity;
extern const uint max_vnf;
extern const res_unit max_link_request_required;
extern const res_unit max_node_request_required;
extern const double arrival_rate;
extern const double departure_rate;
/************************************************************************/

/************************************************************************/
/* Global variables*/
extern map<ID,cVirtFuncApp*> vnf_index_map;
extern map<ID,cAppChain*> app_chain_index_map;
/************************************************************************/


/************************************************************************/
/* functions*/
extern int intialPhyTopology(vector<cPhyNode>* _p_phy_server_vec,vector<cPhyLink>* _p_phy_link_vec,cTopology* _p_phy_topology);
extern int initialRequests(vector<cRequest>* _p_requests);
extern int establishGlobalIndex(vector<cRequest>* _p_requests_vec);
extern int initialEventList(vector<cRequest>* _p_requests_vec,multimap<double,cEvent>& _event_vec);
extern int requestMapping(cTopology* _topo,cRequest* _request);
extern int releaseResource(cRequest* _request);
/************************************************************************/





