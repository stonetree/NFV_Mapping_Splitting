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
extern const res_unit min_link_request_required;
extern const res_unit max_node_request_required;
extern const res_unit min_node_request_required;
extern double arrival_rate;
extern const double departure_rate;
extern const int max_distance;
extern const bool splitting;
extern const uint max_splitting_piece;
/************************************************************************/

/************************************************************************/
/* output metrics*/
extern double accepted_rate;
/************************************************************************/

/************************************************************************/
/* Global variables*/
extern map<ID,cVirtFuncApp*> vnf_index_map;
extern map<ID,cAppChain*> app_chain_index_map;
extern res_unit total_node_resource_used;
extern res_unit total_link_resource_used;
/************************************************************************/

/************************************************************************/
/* functions*/
extern int intialPhyTopology(cTopology* _p_phy_topology);
extern int initialRequests(list<cRequest>* _p_requests);
extern int establishGlobalIndex(list<cRequest>* _p_requests_list);
extern int initialEventList(list<cRequest>* _p_requests_list,multimap<double,cEvent>& _event_vec);
extern int requestAllocating(cTopology* _topo,cRequest* _request);
//extern int requestAllocatingwithSplitting(cTopology* _topo,cRequest* _request);
extern int releaseResource(cTopology* _topo,cRequest* _request);
extern int outputResults(list<cRequest>* _p_requests_list, cTopology* _topo,double _last_time);
extern void updateTopologyInfo(cTopology* _topo);
/************************************************************************/





