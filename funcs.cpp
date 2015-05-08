#include "stdafx.h"
#include "common.h"
#include "cPhyNode.h"
#include "cPhyLink.h"
#include "cTopology.h"
#include "cRequest.h"
#include "cVirtLink.h"

/************************************************************************/
/* Algorithm adjustable parameters*/

//If the value is set to be 1, physical nodes will be ranked according to the
//residual resources of themselves and their adjacent links. Otherwise only the residual
//resources of physical nodes will be considered.
uint node_link_joing_ranking = 1;
/************************************************************************/

cPhyLink* nodetolink(cTopology* _topo,cPhyNode* _p_src_node,cPhyNode* _p_des_node)
{
	map<pair<ID,ID>,cPhyLink>::iterator find_phy_link_map;
	find_phy_link_map = _topo->p_phyLink_map->find(make_pair(_p_src_node->getId(),_p_des_node->getId()));
	if (find_phy_link_map == _topo->p_phyLink_map->end())
	{
		find_phy_link_map = _topo->p_phyLink_map->find(make_pair(_p_des_node->getId(),_p_src_node->getId()));
		if (find_phy_link_map == _topo->p_phyLink_map->end())
		{
			return NULL;
		}
	}

	return &(find_phy_link_map->second);
}

int nodeRanking(cTopology* _topo, multimap<res_unit, cPhyNode*>& _node_ranking_mulmap)
{
	res_unit residual_resource_ranking = 0;

	map<ID,cPhyNode>::iterator iter_phy_node_map = _topo->p_phyNode_map->begin();
	map<pair<ID,ID>,cPhyLink>::iterator iter_phy_link_map = _topo->p_phyLink_map->begin();

	for (;iter_phy_node_map != _topo->p_phyNode_map->end();iter_phy_node_map++)
	{
		residual_resource_ranking = 0;
		residual_resource_ranking += iter_phy_node_map->second.getResidual();

		if (1 == node_link_joing_ranking)
		{
			map<pair<ID,ID>,cVirtLink*>::iterator iter_adjacent_link_id_point_map = iter_phy_node_map->second.adjacent_link_map.begin();
			for (;iter_adjacent_link_id_point_map != iter_phy_node_map->second.adjacent_link_map.end();iter_adjacent_link_id_point_map++)
			{
				residual_resource_ranking += ((cPhyLink*)(iter_adjacent_link_id_point_map->second))->getResidual();
			}
		}

		_node_ranking_mulmap.insert(make_pair(residual_resource_ranking,&(iter_phy_node_map->second)));
	}
	return 0;
}

int requestRearranging(cRequest* _request,multimap<res_unit,cVirtFuncApp*>& _vnf_ranking_mulmap)
{
	list<cVirtFuncApp>::iterator iter_vnf_vec = _request->vir_func_app.begin();
	for (;iter_vnf_vec != _request->vir_func_app.end();iter_vnf_vec++)
	{
		_vnf_ranking_mulmap.insert(make_pair(iter_vnf_vec->getResRequired(),&(*iter_vnf_vec)));
	}
	
	return 0;
}



int nodeMapping(cRequest* _request,cTopology* _topo)
{

	multimap<res_unit,cPhyNode*> node_ranking_mulmap;

	//rearrange the nodes with the ascending order according to their residual resources.
	nodeRanking(_topo,node_ranking_mulmap);

	
	//rearrange the vnfs belonging to the current request with the ascending order according to number of resources they required.
	multimap<res_unit,cVirtFuncApp*> vnf_ranking_mulmap;
	requestRearranging(_request,vnf_ranking_mulmap);
	
	multimap<res_unit,cPhyNode*>::reverse_iterator riter_phy_node = node_ranking_mulmap.rbegin();

	multimap<res_unit,cVirtFuncApp*>::reverse_iterator riter_vnf = vnf_ranking_mulmap.rbegin();
	while (riter_phy_node != node_ranking_mulmap.rend())	
	{
		if(riter_vnf != vnf_ranking_mulmap.rend())
		{
			if (!riter_phy_node->second->havingSufficientRes(riter_vnf->second->getResRequired()))
			{
				//having not sufficient resources for current vnf
				//check the next physical node
				riter_phy_node++;
			}
			else
			{
				riter_vnf->second->setHostServerPoint(riter_phy_node->second);
				riter_vnf->second->setHostServerID(riter_phy_node->second->getId());
				riter_vnf++;

				//vnfs belonging to the same request can not be map onto the same physical node
				riter_phy_node++;
			}
		}
		else
		{
			//have found enough node resources for the current request.
			return 0;
		}
	}
	
	//have not found enough resources for the current request
	return 1;
}

int storeTopoInfo(cTopology* _topo,vector<res_unit>* _p_store_residual)
{
	map<pair<ID,ID>,cPhyLink>::iterator iter_phy_link_map = _topo->p_phyLink_map->begin();
	for (;iter_phy_link_map != _topo->p_phyLink_map->end();iter_phy_link_map++)
	{
		_p_store_residual->push_back(iter_phy_link_map->second.getResidual());
	}
	
	return 0;
}

int restoreTopoInfo(cTopology* _topo,vector<res_unit>* _p_store_residual)
{
	
	map<pair<ID,ID>,cPhyLink>::iterator iter_phy_link_map = _topo->p_phyLink_map->begin();
	vector<res_unit>::iterator iter_store_residual = _p_store_residual->begin();
	while(iter_store_residual != _p_store_residual->end())
	{
		iter_phy_link_map->second.setResidual(*iter_store_residual);
		iter_phy_link_map++;
		iter_store_residual++;
	}

	if (iter_phy_link_map != _topo->p_phyLink_map->end())
	{
		cout<<"Error!!! The stored data doesn't match\n"<<endl;
	}
	
	return 0;
}

int updateResidualResource(cTopology* _topo,cAppChain& _app_chain)
{
	vector<cPhyLink*>::iterator iter_p_phy_link_vec = _app_chain.p_host_link_vec.begin();
	for (;iter_p_phy_link_vec != _app_chain.p_host_link_vec.end();iter_p_phy_link_vec++)
	{
		(*iter_p_phy_link_vec)->allcateResource(_app_chain);
	}
	
	return 0;
}

int updateHostLink(cTopology* _topo,vector<cPhyLink*>& _host_link,vector<cPhyNode*>& _path)
{
	_host_link.clear();
	cPhyLink* p_phy_link = NULL;
	map<pair<ID,ID>,cPhyLink>::iterator find_phy_link_map;
	vector<cPhyNode*>::iterator iter_p_host_node_vec = _path.begin();
	vector<cPhyNode*>::iterator iter_p_next_host_node_vec = iter_p_host_node_vec + 1;
	while (iter_p_next_host_node_vec != _path.end())
	{
		p_phy_link = nodetolink(_topo,*iter_p_host_node_vec,*iter_p_next_host_node_vec); 
		if (p_phy_link == NULL)
		{			
			cout<<"Error! Can not find the physical link that should exist in the found path"<<endl;
			exit(0);
		}
		_host_link.push_back(p_phy_link);
		iter_p_host_node_vec++;
		iter_p_next_host_node_vec++;
	}
	
	return 0;
}

int linkMapping(cRequest* _request,cTopology* _topo)
{
	//for convenience, we first store the number of residual resources of each physical links
	vector<res_unit>* p_store_residual  = new vector<res_unit>;
	storeTopoInfo(_topo,p_store_residual);

	//the link having the most number of resources required will be processed first
	multimap<res_unit,cAppChain*> p_app_chain_mulmap;
	list<cAppChain>::iterator iter_app_chain = _request->app_chain.begin();	
	for (;iter_app_chain != _request->app_chain.end();iter_app_chain++)
	{
		p_app_chain_mulmap.insert(make_pair(iter_app_chain->getResRequired(),&(*iter_app_chain)));
	}

	multimap<res_unit,cAppChain*>::reverse_iterator riter_p_app_chain_mulmap = p_app_chain_mulmap.rbegin();
	for (;riter_p_app_chain_mulmap != p_app_chain_mulmap.rend();riter_p_app_chain_mulmap++)
	{
		vector<cPhyNode*> path;
		_topo->updateAvailableLink(*(riter_p_app_chain_mulmap->second));
		((cVirtFuncApp*)(riter_p_app_chain_mulmap->second->getEndSrcNode()));
		cPhyNode* p_start_node = ((cVirtFuncApp*)(riter_p_app_chain_mulmap->second->getEndSrcNode()))->getHostServerPoint();
		cPhyNode* p_end_node = ((cVirtFuncApp*)(riter_p_app_chain_mulmap->second->getEndDesNode()))->getHostServerPoint();
		if (!_topo->findShortestPath(p_start_node,p_end_node,path))
		{
			//find an available path
			updateHostLink(_topo,iter_app_chain->p_host_link_vec,path);

			//update the residual resource of each physical link
			updateResidualResource(_topo,*iter_app_chain);	

			riter_p_app_chain_mulmap++;
		}
		else
		{
			//can not find an available path 
			restoreTopoInfo(_topo,p_store_residual);
			free(p_store_residual);
			return 1;
		}
	}

	//list<cVirtFuncApp>::iterator iter_vnf = _request->vir_func_app.begin();
	//list<cVirtFuncApp>::iterator iter_next_vnf = iter_vnf + 1;
	//while(iter_next_vnf != _request->vir_func_app.end())
	//{
	//	vector<cPhyNode*> path;
	//	_topo->updateAvailableLink(*iter_app_chain);
	//	cPhyNode* p_start_node = iter_vnf->getHostServerPoint();
	//	cPhyNode* p_end_node = iter_next_vnf->getHostServerPoint();
	//	if (!_topo->findShortestPath(p_start_node,p_end_node,path))
	//	{
	//		//find an available path
	//		updateHostLink(_topo,iter_app_chain->p_host_link_vec,path);

	//		//update the residual resource of each physical link
	//		updateResidualResource(_topo,*iter_app_chain);	

	//		iter_vnf++;
	//		iter_next_vnf++;
	//		iter_app_chain++;
	//	}
	//	else
	//	{
	//		//can not find an available path 
	//		restoreTopoInfo(_topo,p_store_residual);
	//		free(p_store_residual);
	//		return 1;
	//	}
	//}
	
	//restore the stored residual resource info
	restoreTopoInfo(_topo,p_store_residual);
	free(p_store_residual);
	return 0;
}

int requestMapping(cRequest* _request,cTopology* _topo)
{

	//check whether the physical network has sufficient resources to host the currently arrived request
	if (!nodeMapping(_request,_topo))
	{
		//node mapping succeeds
		if (!linkMapping(_request,_topo))
		{
			//link mapping succeeds
			return 0;
		}
	}
	
	//fail to map the currently arrived request onto the substrate network
	return 1;
}

int allocateResource(cRequest* _request)
{
	list<cVirtFuncApp>::iterator iter_vnf = _request->vir_func_app.begin();
	for (;iter_vnf != _request->vir_func_app.end();iter_vnf++)
	{
		(iter_vnf->getHostServerPoint())->allocateResource((*iter_vnf));
	}

	list<cAppChain>::iterator iter_app_chain = _request->app_chain.begin();
	for (;iter_app_chain != _request->app_chain.end();iter_app_chain++)
	{
		vector<cPhyLink*>::iterator iter_host_phy_link = iter_app_chain->p_host_link_vec.begin();
		for (;iter_host_phy_link != iter_app_chain->p_host_link_vec.end();iter_host_phy_link++)
		{
			(*iter_host_phy_link)->allcateResource((*iter_app_chain));
		}
	}
	return 0;
}

int requestAllocating(cTopology* _topo,cRequest* _request)
{

	//allocate resources to the currently arrived request
	//if success, retur 0. Otherwise return 1 due to the lack of sufficient number of residual resources;
	if (!requestMapping(_request,_topo))
	{
		//succeeds to allocate resources.
		_request->setIsService(true);
		allocateResource(_request);
		return 0;
	}

	return 1;
}

int releaseResource(cRequest* _request)
{
	return 0;
}