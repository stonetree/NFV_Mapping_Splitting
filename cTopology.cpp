#include "StdAfx.h"
#include "cTopology.h"

cTopology::cTopology(void)
{
	p_phyNode_map = new map<ID,cPhyNode>;
	p_phyLink_map = new map<pair<ID,ID>,cPhyLink>;
}

cTopology::~cTopology(void)
{
	free(p_phyLink_map);
	free(p_phyNode_map);
}


cTopology::cTopology(const cTopology& _topo)
{
	operator=(_topo);
}

cTopology& cTopology::operator=(const cTopology& _topo)
{
	if (this!=&_topo)
	{
		//this->pnode = _topo.pnode;
		//this->plink = _topo.plink;

		p_phyNode_map = new map<ID,cPhyNode>;
		p_phyLink_map = new map<pair<ID,ID>,cPhyLink>;

		p_phyNode_point_map.clear();
		p_phyLink_point_map.clear();

		used_phyNode_point_map.clear();
		used_phyLink_point_map.clear();

		unused_phyNode_point_map.clear();
		unused_phyLink_point_map.clear();
		
		this->p_phyNode_map->erase(p_phyNode_map->begin(),p_phyNode_map->end());
		this->p_phyNode_map->insert(_topo.p_phyNode_map->begin(),_topo.p_phyNode_map->end());

		this->p_phyLink_map->erase(p_phyLink_map->begin(),p_phyLink_map->end());
		this->p_phyLink_map->insert(_topo.p_phyLink_map->begin(),_topo.p_phyLink_map->end());

		map<ID,cPhyNode>::iterator iter_phyNode = p_phyNode_map->begin();
		for (;iter_phyNode != p_phyNode_map->end();iter_phyNode++)
		{
			p_phyNode_point_map.insert(make_pair(iter_phyNode->second.getId(),&(iter_phyNode->second)));
			if (iter_phyNode->second.getResidual() < iter_phyNode->second.getCapacity())
			{
				used_phyNode_point_map.insert(make_pair(iter_phyNode->second.getId(),&(iter_phyNode->second)));
			}
			else
			{
				unused_phyNode_point_map.insert(make_pair(iter_phyNode->second.getId(),&(iter_phyNode->second)));
			}
		}

		map<pair<ID,ID>,cPhyLink>::iterator iter_phyLink = p_phyLink_map->begin();
		for (;iter_phyLink != p_phyLink_map->end();iter_phyLink++)
		{
			p_phyLink_point_map.insert(make_pair(make_pair(iter_phyLink->second.getEndSrcNodeID(),iter_phyLink->second.getEndDesNodeID()),&(iter_phyLink->second)));
			if (iter_phyLink->second.getResidual() < iter_phyLink->second.getCapacity())
			{
				used_phyLink_point_map.insert(make_pair(make_pair(iter_phyLink->second.getEndSrcNodeID(),iter_phyLink->second.getEndDesNodeID()),&(iter_phyLink->second)));
			}
			else
			{
				unused_phyLink_point_map.insert(make_pair(make_pair(iter_phyLink->second.getEndSrcNodeID(),iter_phyLink->second.getEndDesNodeID()),&(iter_phyLink->second)));
			}
		}

		
	}

	return *this;
}

int cTopology::markAdjacentNode(cPhyNode* _startPoint)
{
	list<cPhyNode*>::iterator iter_node;
	//cVirtLink *plink;
	map<pair<ID,ID>,cVirtLink*>::iterator iter_link_map;

	//ID startPointID = _startPoint->getId();
	//ID endPointID;
	//ID linkStartPointID, linkEndPointID;
	cPhyNode* p_end_point = NULL;
	cPhyNode* p_start_point = _startPoint;
	cPhyNode* p_link_start_node = NULL;
	cPhyNode* p_link_end_node = NULL;

	for (iter_link_map = _startPoint->adjacent_link_map.begin();iter_link_map != _startPoint->adjacent_link_map.end();iter_link_map++)
	{
		if (!iter_link_map->second->getAvailable())
		{
			//the link is unavailable
			continue;
		}

		p_link_start_node = (cPhyNode*)(iter_link_map->second->getEndSrcNode());
		p_link_end_node = (cPhyNode*)(iter_link_map->second->getEndDesNode());

		if (p_start_point == p_link_start_node)
		{
			p_end_point = p_link_end_node;
		}
		else
		{
			p_end_point = p_link_start_node;
		}

		if (p_start_point->getNodeDistance() + iter_link_map->second->getLinkWeight()<p_end_point->getNodeDistance())
		{
			//find a node with shorter distance
			//update it's distance info
			p_end_point->setNodeDistance(p_start_point->getNodeDistance() + iter_link_map->second->getLinkWeight());
			p_end_point->setPreNodeID(p_start_point->getId());
			p_end_point->setPreNodePoint((cVirtNode*)p_start_point);
		}
	}

	return 0;
}

int cTopology::outputShortestPath(cPhyNode* _pnode,vector<cPhyNode*>& _path)
{
	vector<cPhyNode*> tem_path;
	vector<cPhyNode*>::reverse_iterator riter_tem_revr_pah;

	tem_path.erase(tem_path.begin(),tem_path.end());

	while(_pnode->getPreNodeID() != 0) 
	{
		tem_path.push_back(_pnode);
		_pnode = (cPhyNode*)(_pnode->getPreNodePoint());
	} 

	tem_path.push_back(_pnode);

	for (riter_tem_revr_pah = tem_path.rbegin();riter_tem_revr_pah!=tem_path.rend();riter_tem_revr_pah++)
	{
		_path.push_back(*riter_tem_revr_pah);
	}

	return 0;
}


int cTopology::findShortestPath(cPhyNode* _p_src_node, cPhyNode* _p_des_node,vector<cPhyNode*>& _path)
{
	map<ID,cPhyNode*> been_found;
	map<ID,cPhyNode*> not_been_found;
	map<ID,cPhyNode*>::iterator iter_find;

	map<ID,cPhyNode*>::iterator iter_node_algo;

	cPhyNode* p_phy_node = NULL;

	map<ID,cPhyNode>::iterator iter_node;

	int maxDistAlgo;

	//clear all data already existing in every containers
	_path.erase(_path.begin(),_path.end());
	been_found.erase(been_found.begin(),been_found.end());
	not_been_found.erase(not_been_found.begin(),not_been_found.end());


	if (_p_src_node->adjacent_link_map.empty() || _p_des_node->adjacent_link_map.empty())
	{
		//There is no path existing between the specified source node and destination node.
		//Since we will try to find up to K paths between these specified two nodes, therefore after updating the virtual topology,
		//it may have no adjacent link(s) with the source or destination node before K paths have been found.
		//In this case, the ShortestPath algorithm should be stopped.

		return 1;
	}


	//Reset the parameters of the topology
	for (iter_node = p_phyNode_map->begin();iter_node != p_phyNode_map->end();iter_node++)
	{
		//Put all nodes in the un-found set first
		not_been_found.insert(make_pair(iter_node->second.getId(),(cPhyNode*)(&(iter_node->second))));

		//Initial the distance and pre-node info for each node in the virtual topology
		if (iter_node->second.getId() == _p_src_node->getId())
		{
			iter_node->second.setNodeDistance(0);
		}
		else
		{
			iter_node->second.setNodeDistance(max_distance);
		}
		iter_node->second.setPreNodeID(0);
		iter_node->second.setPreNodePoint(NULL);
	}

	//The main body of the Shortest Path algorithm
	while (!not_been_found.empty())
	{

		maxDistAlgo = max_distance;
		p_phy_node = NULL;

		//Find the node with minimum distance in un-found set
		for (iter_node_algo = not_been_found.begin();iter_node_algo != not_been_found.end();iter_node_algo++)
		{
			if (iter_node_algo->second->getNodeDistance() < maxDistAlgo)
			{
				p_phy_node = iter_node_algo->second;
				maxDistAlgo = iter_node_algo->second->getNodeDistance();
			}
		}


		if (p_phy_node == NULL)
		{
			//can't find a path from source node to destination node as no connected path exists
			return 1;
		}

		//determine if it has already arrived the destination node
		if (p_phy_node == _p_des_node)
		{
			//successfully obtain the shortest path between the source node and destination node
			//The algorithm will stop when having arrived the destination node
			outputShortestPath(p_phy_node,_path);
			return 0;
		}

		iter_find = not_been_found.find(p_phy_node->getId());
		if (iter_find == not_been_found.end())
		{
			//abnormal exit
			//did not find the node that should be exist
			return 2;

		}

		//delete the node that should be as a starting node from the non-found set
		//_path.push_back(pvirtNode->getId());
		not_been_found.erase(iter_find);

		//update the distance for the node pointed by p_phy_node
		markAdjacentNode(p_phy_node);
	}

	//succeed to find a shortest path between specified two nodes.
	return 0;
}

int cTopology::updateAvailableLink(cAppChain& _app_chain)
{
	map<pair<ID,ID>,cPhyLink>::iterator iter_phy_link_map = p_phyLink_map->begin();
	for (;iter_phy_link_map != p_phyLink_map->end();iter_phy_link_map++)
	{
		if (iter_phy_link_map->second.havingSufficientRes(_app_chain.getResRequired()))
		{
			iter_phy_link_map->second.setAvailable(true);
		}
		else
		{
			iter_phy_link_map->second.setAvailable(false);
		}
	}
	
	
	return 0;
}