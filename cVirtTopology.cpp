#include "StdAfx.h"
#include "cVirtTopology.h"


cVirtTopology::cVirtTopology(void)
{
	p_virtNode_map = new map<ID,cVirtNode>;
	p_virtLink_map = new map<ID,cVirtLink>;
}

cVirtTopology::~cVirtTopology(void)
{
	free(p_virtNode_map);
	free(p_virtLink_map);
}


cVirtTopology::cVirtTopology(const cVirtTopology& _topo)
{
	operator=(_topo);
}

cVirtTopology& cVirtTopology::operator=(const cVirtTopology& _topo)
{
	if (this!=&_topo)
	{
		//this->pnode = _topo.pnode;
		//this->plink = _topo.plink;

		p_virtNode_map = new map<ID,cVirtNode>;
		p_virtLink_map = new map<ID,cVirtLink>;

		this->p_virtNode_map->erase(p_virtNode_map->begin(),p_virtNode_map->end());
		this->p_virtNode_map->insert(_topo.p_virtNode_map->begin(),_topo.p_virtNode_map->end());

		this->p_virtLink_map->erase(p_virtLink_map->begin(),p_virtLink_map->end());
		this->p_virtLink_map->insert(_topo.p_virtLink_map->begin(),_topo.p_virtLink_map->end());
	}

	return *this;
}


cVirtLink* isDuplicate(vector<cVirtLink>& _virtLink_vec,int _endSrcPhyNodeID,int _endDesNodeID)
{
	vector<cVirtLink>::iterator iter_virtLink_vec = _virtLink_vec.begin();

	for (;iter_virtLink_vec!=_virtLink_vec.end();iter_virtLink_vec++)
	{
		if (iter_virtLink_vec->getEndSrcNodeID() == _endSrcPhyNodeID && iter_virtLink_vec->getEndDesNodeID() == _endDesNodeID)
			return &(*iter_virtLink_vec);
	}
	return NULL;
}


int cVirtTopology::markAdjacentNode(cVirtNode* _startPoint)
{
	list<cVirtNode*>::iterator iter_node;
	//cVirtLink *plink;
	map<pair<ID,ID>,cVirtLink*>::iterator iter_link_map;

	//ID startPointID = _startPoint->getId();
	//ID endPointID;
	//ID linkStartPointID, linkEndPointID;
	cVirtNode* p_end_point = NULL;
	cVirtNode* p_start_point = _startPoint;
	cVirtNode* p_link_start_node = NULL;
	cVirtNode* p_link_end_node = NULL;

	for (iter_link_map = _startPoint->adjacent_link_map.begin();iter_link_map != _startPoint->adjacent_link_map.end();iter_link_map++)
	{
		p_link_start_node = (cVirtNode*)(iter_link_map->second->getEndSrcNode());
		p_link_end_node = (cVirtNode*)(iter_link_map->second->getEndDesNode());

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
			p_end_point->setNodeDistance(_startPoint->getNodeDistance() + iter_link_map->second->getLinkWeight());
			p_end_point->setPreNodeID(_startPoint->getId());
		}
	}

	return 0;
}

int cVirtTopology::outputShortestPath(cVirtNode* _pnode,vector<cVirtNode*>& _path)
{
	vector<cVirtNode*> tem_path;
	vector<cVirtNode*>::reverse_iterator riter_tem_revr_pah;

	tem_path.erase(tem_path.begin(),tem_path.end());

	while(_pnode->getPreNodeID() != 0) 
	{
		tem_path.push_back(_pnode);
		_pnode = _pnode->getPreNodePoint();
	} 

	tem_path.push_back(_pnode);

	for (riter_tem_revr_pah = tem_path.rbegin();riter_tem_revr_pah!=tem_path.rend();riter_tem_revr_pah++)
	{
		_path.push_back(*riter_tem_revr_pah);
	}

	return 0;
}


int cVirtTopology::findShortestPath(cVirtNode* _p_src_node, cVirtNode* _p_des_node,vector<cVirtNode*>& _path)
{
	map<ID,cVirtNode*> been_found;
	map<ID,cVirtNode*> not_been_found;
	map<ID,cVirtNode*>::iterator iter_find;

	map<ID,cVirtNode*>::iterator iter_node_algo;

	cVirtNode* p_virt_node = NULL;

	map<ID,cVirtNode>::iterator iter_node;

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
	for (iter_node = p_virtNode_map->begin();iter_node != p_virtNode_map->end();iter_node++)
	{
		//Put all nodes in the un-found set first
		not_been_found.insert(make_pair(iter_node->second.getId(),(cVirtNode*)(&(iter_node->second))));

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
		p_virt_node = NULL;

		//Find the node with minimum distance in un-found set
		for (iter_node_algo = not_been_found.begin();iter_node_algo != not_been_found.end();iter_node_algo++)
		{
			if (iter_node_algo->second->getNodeDistance() < maxDistAlgo)
			{
				p_virt_node = iter_node_algo->second;
				maxDistAlgo = iter_node_algo->second->getNodeDistance();
			}
		}


		if (p_virt_node == NULL)
		{
			//can't find a path from source node to destination node;
			return 1;
		}

		//determine if it has already arrived the destination node
		if (p_virt_node == _p_des_node)
		{
			//successfully obtain the shortest path between the source node and destination node
			//The algorithm will stop when having arrived the destination node
			outputShortestPath(p_virt_node,_path);
			return 0;
		}

		iter_find = not_been_found.find(p_virt_node->getId());
		if (iter_find == not_been_found.end())
		{
			//abnormal exit
			//did not find the node that should be exist
			return 2;

		}

		//delete the node that should be as a starting node from the non-found set
		//_path.push_back(pvirtNode->getId());
		not_been_found.erase(iter_find);

		//update the distance for the node pointed by p_virt_node
		markAdjacentNode(p_virt_node);
	}

	//succeed to find a shortest path between specified two nodes.
	return 0;
}
