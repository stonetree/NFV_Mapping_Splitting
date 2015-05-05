#include "StdAfx.h"
#include "cTopology.h"

cTopology::cTopology(void)
{
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
		
		this->p_phyNode_vec.erase(p_phyNode_vec.begin(),p_phyNode_vec.end());
		this->p_phyNode_vec.assign(_topo.p_phyNode_vec.begin(),_topo.p_phyNode_vec.end());

		this->p_phyLink_vec.erase(p_phyLink_vec.begin(),p_phyLink_vec.end());
		this->p_phyLink_vec.assign(_topo.p_phyLink_vec.begin(),_topo.p_phyLink_vec.end());
	}

	return *this;
}

cTopology::~cTopology(void)
{
}