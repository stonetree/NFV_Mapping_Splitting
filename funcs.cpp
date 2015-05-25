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

int linktonode(cTopology* _topo,cPhyLink* _p_phy_link,pair<ID,ID>& _node_pair)
{
	ID src_node_id = _p_phy_link->getEndSrcNodeID();
	ID des_node_id = _p_phy_link->getEndDesNodeID();

	map<pair<ID,ID>,cPhyLink>::iterator fiter_phy_link_map;
	fiter_phy_link_map = _topo->p_phyLink_map->find(make_pair(src_node_id,des_node_id));
	if (fiter_phy_link_map == _topo->p_phyLink_map->end())
	{
		fiter_phy_link_map = _topo->p_phyLink_map->find(make_pair(des_node_id,src_node_id));

		if (fiter_phy_link_map == _topo->p_phyLink_map->end())
		{
			cout<<"Error!!! Can not locate the physical link"<<endl;
		}
	}
	
	_node_pair = fiter_phy_link_map->first;
	return 0;
}

int nodeRanking(map<ID,cPhyNode*> _phyNode_point_map,multimap<res_unit, cPhyNode*>& _node_ranking_mulmap)
{
	_node_ranking_mulmap.clear();
	double node_residual_resource_ranking = 0;
	double link_residual_resource_ranking = 1;

	map<ID,cPhyNode*>::iterator iter_phy_node_map = _phyNode_point_map.begin();
	//map<pair<ID,ID>,cPhyLink*>::iterator iter_phy_link_map = _phyLink_point_map.begin();

	for (;iter_phy_node_map != _phyNode_point_map.end();iter_phy_node_map++)
	{
		node_residual_resource_ranking = 0;
		node_residual_resource_ranking += iter_phy_node_map->second->getResidual();

		if (1 == node_link_joing_ranking)
		{
			link_residual_resource_ranking = 0;
			map<pair<ID,ID>,cVirtLink*>::iterator iter_adjacent_link_id_point_map = iter_phy_node_map->second->adjacent_link_map.begin();
			for (;iter_adjacent_link_id_point_map != iter_phy_node_map->second->adjacent_link_map.end();iter_adjacent_link_id_point_map++)
			{
				link_residual_resource_ranking += ((cPhyLink*)(iter_adjacent_link_id_point_map->second))->getResidual();
			}
		}

		_node_ranking_mulmap.insert(make_pair(node_residual_resource_ranking * link_residual_resource_ranking,iter_phy_node_map->second));
	}
	return 0;
}

int requestRearranging(cRequest* _request,multimap<res_unit,cVirtFuncApp*>& _vnf_ranking_mulmap)
{
	_vnf_ranking_mulmap.clear();
	list<cVirtFuncApp>::iterator iter_vnf_vec = _request->vir_func_app.begin();
	for (;iter_vnf_vec != _request->vir_func_app.end();iter_vnf_vec++)
	{
		_vnf_ranking_mulmap.insert(make_pair(iter_vnf_vec->getResRequired(),&(*iter_vnf_vec)));
	}
	
	return 0;
}

int vnfMapping(cRequest* _request,map<ID,cPhyNode*>& _phy_node_point_map,map<pair<ID,ID>,cPhyLink*>& _phy_link_point_map)
{
	multimap<res_unit,cPhyNode*> node_ranking_mulmap;
	//rearrange the nodes with the ascending order according to their residual resources.
	nodeRanking(_phy_node_point_map,node_ranking_mulmap);

	//rearrange the vnfs belonging to the current request with the ascending order according to number of resources they required.
	multimap<res_unit,cVirtFuncApp*> vnf_ranking_mulmap;
	requestRearranging(_request,vnf_ranking_mulmap);

	multimap<res_unit,cPhyNode*>::iterator iter_phy_node = node_ranking_mulmap.begin();

	multimap<res_unit,cVirtFuncApp*>::reverse_iterator riter_vnf = vnf_ranking_mulmap.rbegin();

	while (iter_phy_node != node_ranking_mulmap.end())	
	{
		if(riter_vnf != vnf_ranking_mulmap.rend())
		{
			if (riter_vnf->second->getHostServerPoint() != NULL)
			{
				//The host server is already assigned for this vnf
				riter_vnf++;
			}
			else
			{
				if (!iter_phy_node->second->havingSufficientRes(riter_vnf->second->getResRequired()))
				{
					//having not sufficient resources for current vnf
					//check the next physical node
					iter_phy_node++;
				}
				else
				{
					riter_vnf->second->setHostServerPoint(iter_phy_node->second);
					riter_vnf->second->setHostServerID(iter_phy_node->second->getId());
					riter_vnf++;

					//vnfs belonging to the same request can not be map onto the same physical node
					iter_phy_node++;
				}				
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

int updateVnfInfo(cRequest* _request,cVirtFuncApp* _vnf,map<ID,cPhyNode*>& _host_node)
{
	uint splitting_count = _host_node.size();

	map<ID,cPhyNode*>::iterator iter_found_phy_node_map = _host_node.begin();

	if (splitting_count == 1)
	{
		//Set the host server info
		_vnf->setHostServerID(iter_found_phy_node_map->second->getId());
		_vnf->setHostServerPoint(iter_found_phy_node_map->second);
		return 0;
	}
	else
	{
		uint splitting_node_index = 1;
		uint splitting_link_index = 1;
		ID vnf_orignal_id = _vnf->getId();
		res_unit parent_node_resource_required = _vnf->getResRequired();
		int parent_type = _vnf->getVirtFuncAppType();
		ID parent_node_id = _vnf->getId();
		ID base_node_ID = _vnf->getId()*100;
		for (splitting_node_index = 1;iter_found_phy_node_map != _host_node.end();iter_found_phy_node_map++,splitting_node_index++)
		{
			//Create a new vnf as a sub-vnf of the current vnf
			cVirtFuncApp tem_vnf(*(_vnf));
			_request->vir_func_app.push_back(tem_vnf);
			_vnf->setSplit(true);

			//The point to the just inserted vnf
			cVirtFuncApp* p_inserted_vnf = &(*(_request->vir_func_app.rbegin()));

			//Update the info for the just inserted vnf
			p_inserted_vnf->setId((ID)(parent_node_id*100 + splitting_node_index));	
			p_inserted_vnf->setHostServerID(iter_found_phy_node_map->second->getId());
			p_inserted_vnf->setHostServerPoint(iter_found_phy_node_map->second);
			p_inserted_vnf->setParentID(parent_node_id);
			p_inserted_vnf->setResRequired(parent_node_resource_required/splitting_count);
			p_inserted_vnf->setSplit(false);



			//Split each adjacent link of the vnf to be split

			map<pair<ID,ID>,cVirtLink*> adjacent_link_point;
			adjacent_link_point.insert(p_inserted_vnf->adjacent_link_map.begin(),p_inserted_vnf->adjacent_link_map.end());
			map<pair<ID,ID>,cVirtLink*>::iterator iter_adjacent_link_point = adjacent_link_point.begin();

			//map<pair<ID,ID>,cVirtLink*>::iterator iter_adjacent_link_map = p_inserted_vnf->adjacent_link_map.begin();
			for (;iter_adjacent_link_point != adjacent_link_point.end();iter_adjacent_link_point++)
			//for (;iter_adjacent_link_map != p_inserted_vnf->adjacent_link_map.end();iter_adjacent_link_map++)
			{
				//cAppChain* point_app_chain = (cAppChain*)(iter_adjacent_link_map->second);
				cAppChain* point_app_chain = (cAppChain*)(iter_adjacent_link_point->second);
				point_app_chain->setSplit(true);

				ID app_chain_original_id = point_app_chain->getId();
				res_unit app_chain_original_resource_required = point_app_chain->getResRequired();

				cAppChain tem_app_chain(*point_app_chain);
				_request->app_chain.push_back(tem_app_chain);

				//The point to the just inserted app chain
				cAppChain* p_inserted_app_chain = &(*(_request->app_chain.rbegin()));

				//Update the info for the just inserted app chain
				p_inserted_app_chain->setId(base_node_ID+splitting_node_index);
				p_inserted_app_chain->setParentID(app_chain_original_id);
				p_inserted_app_chain->setResRequied(app_chain_original_resource_required/splitting_count);
				p_inserted_app_chain->setSplit(false);

				//Update the src node and des node info
				if (iter_adjacent_link_point->first.second == _vnf->getId())
				{
					//The link is ended at this vnf.
					p_inserted_app_chain->setEndDesNodeID(p_inserted_vnf->getId());
					p_inserted_app_chain->setEndDesNode(p_inserted_vnf);
					
					((cVirtNode*)(iter_adjacent_link_point->second->getEndSrcNode()))->adjacent_link_map.insert(make_pair(make_pair(p_inserted_app_chain->getEndSrcNodeID(),p_inserted_app_chain->getEndDesNodeID()),(cVirtLink*)p_inserted_app_chain));
				}
				else
				{
					//The link is started at this vnf
					p_inserted_app_chain->setEndSrcNodeID(p_inserted_vnf->getId());
					p_inserted_app_chain->setEndSrcNode(p_inserted_vnf);
					
					((cVirtNode*)(iter_adjacent_link_point->second->getEndDesNode()))->adjacent_link_map.insert(make_pair(make_pair(p_inserted_app_chain->getEndSrcNodeID(),p_inserted_app_chain->getEndDesNodeID()),(cVirtLink*)p_inserted_app_chain));

				}

				//Add this newly created app chain into the adjacent link list of both the src node and the des node.
	
				p_inserted_vnf->adjacent_link_map.insert(make_pair(make_pair(p_inserted_app_chain->getEndSrcNodeID(),p_inserted_app_chain->getEndDesNodeID()),(cVirtLink*)p_inserted_app_chain));

				_request->vnf_chain_map.insert(make_pair(make_pair(p_inserted_app_chain->getEndSrcNodeID(),p_inserted_app_chain->getEndDesNodeID()),p_inserted_app_chain));
					
			}
		}
	}
	
	return 0;
}

int updateAvailableNode(map<ID,cPhyNode*>& _available_node_point_map,map<ID,cPhyNode*>& _host_node_point_map)
{
	map<ID,cPhyNode*>::iterator iter_host_node_point_map = _host_node_point_map.begin();
	map<ID,cPhyNode*>::iterator fiter_phy_node_point_map;
	for (;iter_host_node_point_map != _host_node_point_map.end();iter_host_node_point_map++)
	{
		fiter_phy_node_point_map = _available_node_point_map.find(iter_host_node_point_map->first);
		if (fiter_phy_node_point_map == _available_node_point_map.end())
		{
			cout<<"Error!!! Can not remove the physical node that already used by other vnf"<<endl;
			exit(0);
		}		
		_available_node_point_map.erase(fiter_phy_node_point_map);

	}
	
	return 0;
}

int clearAdjacentLink(cRequest* _request,cVirtNode* _p_vir_node)
{
	map<pair<ID,ID>,cVirtLink*>::iterator iter_adjacent_link_map = _p_vir_node->adjacent_link_map.begin();
	map<pair<ID,ID>,cAppChain*>::iterator iter_indicate_removed_adjacent_link;

	while (iter_adjacent_link_map != _p_vir_node->adjacent_link_map.end())
	{
		iter_indicate_removed_adjacent_link = _request->vnf_chain_map.find(make_pair(iter_adjacent_link_map->first.first,iter_adjacent_link_map->first.second));
		if (iter_indicate_removed_adjacent_link == _request->vnf_chain_map.end())
		{
			//This adjacent link should be deleted
			ID src_node_id = iter_adjacent_link_map->first.first;
			ID des_node_id = iter_adjacent_link_map->first.second;
			iter_adjacent_link_map++;
			_p_vir_node->adjacent_link_map.erase(make_pair(src_node_id,des_node_id));
		}
		else
		{
			iter_adjacent_link_map++;
		}
	}
	
	
	return 0;
}

int removeSplitVnfAppChain(cRequest* _request)
{
	
	//Clean the vnf chain
	map<pair<ID,ID>,cAppChain*>::iterator iter_vnf_chain_map = _request->vnf_chain_map.begin();
	map<pair<ID,ID>,cAppChain*>::iterator iter_indicate_removed_vnf_chain;
	while (iter_vnf_chain_map != _request->vnf_chain_map.end())
	{
		if (iter_vnf_chain_map->second->isSplit())
		{
			iter_indicate_removed_vnf_chain = iter_vnf_chain_map;
			iter_vnf_chain_map++;
			_request->vnf_chain_map.erase(iter_indicate_removed_vnf_chain);
		}
		else
		{
			iter_vnf_chain_map++;
		}
	}
	
	//Clean the app chain
	list<cAppChain>::iterator iter_app_chain_list = _request->app_chain.begin();
	list<cAppChain>::iterator iter_indicate_removed_app_chain;
	while (iter_app_chain_list != _request->app_chain.end())
	{	

		if (iter_app_chain_list->isSplit())
		{
			iter_indicate_removed_app_chain = iter_app_chain_list;

			//Clean the adjacent link
			cVirtNode* src_node_point = (cVirtNode*)(iter_indicate_removed_app_chain->getEndSrcNode());
			cVirtNode* des_node_point = (cVirtNode*)(iter_indicate_removed_app_chain->getEndDesNode());
			clearAdjacentLink(_request,src_node_point);
			clearAdjacentLink(_request,des_node_point);
			
			iter_app_chain_list++;
			_request->app_chain.erase(iter_indicate_removed_app_chain);
		}
		else
		{
			iter_app_chain_list++;
		}


	}
	
	//Clean the vnf
	list<cVirtFuncApp>::iterator iter_vnf_list = _request->vir_func_app.begin();
	list<cVirtFuncApp>::iterator iter_indicate_removed_vnf;
	while(iter_vnf_list != _request->vir_func_app.end())
	{
		if (iter_vnf_list->isSplit())
		{
			iter_indicate_removed_vnf = iter_vnf_list;
			iter_vnf_list++;
			_request->vir_func_app.erase(iter_indicate_removed_vnf);
		}
		else
		{
			iter_vnf_list++;
		}
	}	
	
	return 0;
}

int onevnfMapping(cRequest* _request,cVirtFuncApp* _p_vnf,map<ID,cPhyNode*>& _available_node_point_map)
{
	uint splitting_count = 1;
	map<ID,cPhyNode*> host_node_point_map;
	
	//Find enough number of physical servers from available physical servers already used.
	for (splitting_count = 1;splitting_count <= max_splitting_piece; splitting_count++)
	{
		host_node_point_map.clear();

		multimap<res_unit,cPhyNode*> node_ranking_mulmap;
		//rearrange the nodes with the ascending order according to their residual resources.
		nodeRanking(_available_node_point_map,node_ranking_mulmap);

		multimap<res_unit,cPhyNode*>::iterator iter_phy_node = node_ranking_mulmap.begin();
		for (;iter_phy_node != node_ranking_mulmap.end();iter_phy_node++)
		{
			if (iter_phy_node->second->havingSufficientRes(_p_vnf->getResRequired()/splitting_count))
			{
				//The physical node has sufficient resources to support the vnf
				host_node_point_map.insert(make_pair(iter_phy_node->second->getId(),iter_phy_node->second));
			}

			if (host_node_point_map.size() == splitting_count)
			{
				//Enough number of physical nodes having sufficient resources is found
				//When the size of host_node_point_map is larger than 1, the vnf will be split and deployed 
				//onto multiple physical nodes.
				updateVnfInfo(_request,_p_vnf,host_node_point_map);
				updateAvailableNode(_available_node_point_map,host_node_point_map);
				removeSplitVnfAppChain(_request);
				return 0;
			}
		}
	}
	
	//Can not find enough resource from the specified set of physical servers
	return 1;
}

int vnfMappingwithSplitting(cRequest* _request,cTopology* _topo)
{

	//Since vnfs belonging to the same request can not be place on the same physical server
	//the info of all physical servers should be first stored.
	map<ID,cPhyNode*> available_used_physical_node;
	available_used_physical_node.insert(_topo->used_phyNode_point_map.begin(),_topo->used_phyNode_point_map.end());

	map<ID,cPhyNode*> available_unused_physical_node;
	map<ID, cPhyNode*>::iterator iter_unused_phyNode_point_map = _topo->unused_phyNode_point_map.begin();
	//for (; iter_unused_phyNode_point_map != _topo->unused_phyNode_point_map.end();iter_unused_phyNode_point_map++)
	//{
	//	available_unused_physical_node.insert(*iter_unused_phyNode_point_map);
	//}
	available_unused_physical_node.insert(_topo->unused_phyNode_point_map.begin(),_topo->unused_phyNode_point_map.end());


	//rearrange the vnfs belonging to the current request with the ascending order according to number of resources they required.
	multimap<res_unit,cVirtFuncApp*> vnf_ranking_mulmap;
	requestRearranging(_request,vnf_ranking_mulmap);
	multimap<res_unit,cVirtFuncApp*>::reverse_iterator riter_vnf_ranking_mulmap = vnf_ranking_mulmap.rbegin();

	for (;riter_vnf_ranking_mulmap != vnf_ranking_mulmap.rend();riter_vnf_ranking_mulmap++)
	{
		//First check the available physical servers already used.
		if (!onevnfMapping(_request,riter_vnf_ranking_mulmap->second,available_used_physical_node))
		{
			//Find enough resources from used physical server
		}
		else
		{
			//Try to find resources from unused physical servers
			if (!onevnfMapping(_request,riter_vnf_ranking_mulmap->second,available_unused_physical_node))
			{
				//Find enough resources from unused physical server
				//Check the next vnf
				continue;
			}
			else
			{
				return 1;
			}
		}

	}

	//Enough number of resources have found for the vnfs belonging to this request
	return 0;
}


int nodeMapping(cRequest* _request,cTopology* _topo)
{

	if (splitting == false)
	{
		return vnfMapping(_request,_topo->p_phyNode_point_map,_topo->p_phyLink_point_map);
	}
	else
	{
		return vnfMappingwithSplitting(_request,_topo);
	}
	
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
	static uint iterator_count = 0;

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
		iterator_count++;
		vector<cPhyNode*> path;
		_topo->updateAvailableLink(*(riter_p_app_chain_mulmap->second));
		((cVirtFuncApp*)(riter_p_app_chain_mulmap->second->getEndSrcNode()));
		cPhyNode* p_start_node = ((cVirtFuncApp*)(riter_p_app_chain_mulmap->second->getEndSrcNode()))->getHostServerPoint();
		cPhyNode* p_end_node = ((cVirtFuncApp*)(riter_p_app_chain_mulmap->second->getEndDesNode()))->getHostServerPoint();
		if (!_topo->findShortestPath(p_start_node,p_end_node,path))
		{
			//find an available path
			updateHostLink(_topo,riter_p_app_chain_mulmap->second->p_host_link_vec,path);

			//update the residual resource of each physical link
			updateResidualResource(_topo,*(riter_p_app_chain_mulmap->second));	
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

int allocateResource(cTopology* _topo,cRequest* _request)
{
	list<cVirtFuncApp>::iterator iter_vnf = _request->vir_func_app.begin();
	map<ID,cPhyNode*>::iterator fiter_used_phynode;
	map<pair<ID,ID>,cPhyLink*>::iterator fiter_used_phylink;
	for (;iter_vnf != _request->vir_func_app.end();iter_vnf++)
	{
		(iter_vnf->getHostServerPoint())->allocateResource((*iter_vnf));
		fiter_used_phynode = _topo->used_phyNode_point_map.find((iter_vnf->getHostServerID()));
		if (fiter_used_phynode == _topo->used_phyNode_point_map.end())
		{
			_topo->used_phyNode_point_map.insert(make_pair(iter_vnf->getHostServerID(),iter_vnf->getHostServerPoint()));
		}
	}

	list<cAppChain>::iterator iter_app_chain = _request->app_chain.begin();
	for (;iter_app_chain != _request->app_chain.end();iter_app_chain++)
	{
		vector<cPhyLink*>::iterator iter_host_phy_link = iter_app_chain->p_host_link_vec.begin();
		for (;iter_host_phy_link != iter_app_chain->p_host_link_vec.end();iter_host_phy_link++)
		{
			(*iter_host_phy_link)->allcateResource((*iter_app_chain));
			pair<ID,ID> node_id;
			linktonode(_topo,*iter_host_phy_link,node_id);
			fiter_used_phylink = _topo->used_phyLink_point_map.find(node_id);
			if (fiter_used_phylink == _topo->used_phyLink_point_map.end())
			{
				_topo->used_phyLink_point_map.insert(make_pair(node_id,*iter_host_phy_link));
			}
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
		cout << "The " << _request->getId() << " is accepted." << endl;
		allocateResource(_topo,_request);
		return 0;
	}

	return 1;
}

int releaseResource(cTopology* _topo,cRequest* _request)
{
	list<cVirtFuncApp>::iterator iter_vnf = _request->vir_func_app.begin();
	map<ID,cPhyNode*>::iterator fiter_used_phynode;
	map<pair<ID,ID>,cPhyLink*>::iterator fiter_used_phylink;
	for (;iter_vnf != _request->vir_func_app.end();iter_vnf++)
	{
		(iter_vnf->getHostServerPoint())->releaseResource(*iter_vnf);

		//remove the physical node from the used_phyNode_point_map if the number of its residual resources is equal to its capacity
		if ((iter_vnf->getHostServerPoint())->getResidual() == (iter_vnf->getHostServerPoint())->getCapacity())
		{
			fiter_used_phynode = _topo->used_phyNode_point_map.find((iter_vnf->getHostServerID()));
			if (fiter_used_phynode == _topo->used_phyNode_point_map.end())
			{
				cout<<"Error!!! Can not locate the physical node in the used_phyNode_point_map"<<endl;
				exit(0);
			}
			_topo->used_phyNode_point_map.erase(fiter_used_phynode);
		}

	}

	list<cAppChain>::iterator iter_app_chain = _request->app_chain.begin();
	for (;iter_app_chain != _request->app_chain.end();iter_app_chain++)
	{
		vector<cPhyLink*>::iterator iter_host_phy_link = iter_app_chain->p_host_link_vec.begin();
		for (;iter_host_phy_link != iter_app_chain->p_host_link_vec.end();iter_host_phy_link++)
		{
			(*iter_host_phy_link)->releaseResource(*iter_app_chain);

			//remove the link from the used_phyLink_point_map if the number of its residual resources is equal to its capacity
			if ((*iter_host_phy_link)->getResidual() == (*iter_host_phy_link)->getCapacity())
			{
				pair<ID,ID> node_id;
				linktonode(_topo,*iter_host_phy_link,node_id);
				fiter_used_phylink = _topo->used_phyLink_point_map.find(node_id);
				if (fiter_used_phylink == _topo->used_phyLink_point_map.end())
				{
					cout<<"Error!!! Can not locate the physical link in the used_phyLink_point_map"<<endl;
					exit(0);
				}
				
				_topo->used_phyLink_point_map.erase(fiter_used_phylink);
			}
		}
	}
	return 0;
}

int outputResults(list<cRequest>* _p_requests_list,cTopology* _topo,double _last_time)
{
	ofstream output_file;
	
	res_unit total_node_capcity = 0;
	res_unit total_link_capacity = 0;
	res_unit total_node_resource_used = 0;
	res_unit total_link_resource_used = 0;

	double node_utilization = 0;
	double link_utilization = 0;
	
	list<cRequest>::iterator iter_request = _p_requests_list->begin();
	for (; iter_request != _p_requests_list->end(); iter_request++)
	{
		
		if (iter_request->getIsService())
		{
			accepted_rate++;
			list<cVirtFuncApp>::iterator iter_vnf = iter_request->vir_func_app.begin();
			for (; iter_vnf != iter_request->vir_func_app.end(); iter_vnf++)
			{
				total_node_resource_used += iter_vnf->getResRequired();
			}

			list<cAppChain>::iterator iter_app_chain = iter_request->app_chain.begin();
			for (; iter_app_chain != iter_request->app_chain.end(); iter_app_chain++)
			{
				total_link_resource_used += iter_app_chain->getResRequired();
			}
		}
	}
	accepted_rate /= _p_requests_list->size();

	map<ID, cPhyNode>::iterator iter_phyNode_map = _topo->p_phyNode_map->begin();
	for (;iter_phyNode_map != _topo->p_phyNode_map->end();iter_phyNode_map++)
	{
		total_node_capcity += iter_phyNode_map->second.getCapacity();
	}

	map<pair<ID, ID>, cPhyLink>::iterator iter_phyLink_map = _topo->p_phyLink_map->begin();
	for (; iter_phyLink_map != _topo->p_phyLink_map->end();iter_phyLink_map++)
	{
		total_link_capacity += iter_phyLink_map->second.getCapacity();
	}

	node_utilization = (double)total_node_resource_used / total_node_capcity;
	link_utilization = (double)total_link_resource_used / total_link_capacity;

	output_file.open("output.txt", ios_base::app);
	if (!output_file)
	{
		cerr << "unable to open the output file: output.txt";
		exit(-1);
	}

	output_file << arrival_rate << " " << departure_rate << " " << accepted_rate << " " << node_utilization /_last_time << " " << link_utilization / _last_time << endl;
	
	output_file.close();


	return 0;

}

void updateTopologyInfo(cTopology* _topo)
{
	map<ID, cPhyNode*>::iterator iter_phyNode_point_map = _topo->p_phyNode_point_map.begin();
	map<pair<ID, ID>, cPhyLink*>::iterator iter_phyLink_point_map = _topo->p_phyLink_point_map.begin();

	_topo->used_phyNode_point_map.clear();
	_topo->used_phyLink_point_map.clear();

	_topo->unused_phyNode_point_map.clear();
	_topo->unused_phyLink_point_map.clear();

	for (; iter_phyNode_point_map != _topo->p_phyNode_point_map.end(); iter_phyNode_point_map++)
	{
		if (iter_phyNode_point_map->second->getResidual() < iter_phyNode_point_map->second->getCapacity())
		{
			_topo->used_phyNode_point_map.insert(*iter_phyNode_point_map);
		}
		else
		{
			_topo->unused_phyNode_point_map.insert(*iter_phyNode_point_map);
		}
	}

	for (; iter_phyLink_point_map != _topo->p_phyLink_point_map.end();iter_phyLink_point_map++)
	{
		if (iter_phyLink_point_map->second->getResidual() < iter_phyLink_point_map->second->getCapacity())
		{
			_topo->used_phyLink_point_map.insert(*iter_phyLink_point_map);
		}
		else
		{
			_topo->unused_phyLink_point_map.insert(*iter_phyLink_point_map);
		}
	}

	return;
}

//int requestMappingwithSplitting(cRequest* _request,cTopology* _topo)
//{
//
//	//check whether the physical network has sufficient resources to host the currently arrived request
//	if (!nodeMapping(_request,_topo))
//	{
//		//node mapping succeeds
//		if (!linkMapping(_request,_topo))
//		{
//			//link mapping succeeds
//			return 0;
//		}
//	}
//
//	//fail to map the currently arrived request onto the substrate network
//	return 1;
//}
//
//int requestAllocatingwithSplitting(cTopology* _topo,cRequest* _request)
//{
//
//	//allocate resources to the currently arrived request
//	//if success, retur 0. Otherwise return 1 due to the lack of sufficient number of residual resources;
//	if (!requestMappingwithSplitting(_request,_topo))
//	{
//		//succeeds to allocate resources.
//		_request->setIsService(true);
//		allocateResource(_request);
//		return 0;
//	}
//
//	return 1;
//}