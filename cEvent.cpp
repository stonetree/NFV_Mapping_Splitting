#include "StdAfx.h"
#include "cEvent.h"


cEvent::cEvent(void)
{
}


cEvent::~cEvent(void)
{
}

cEvent& cEvent::operator=(const cEvent& _event)
{
	if (this != &_event)
	{
		type = _event.type;
		p_request = _event.p_request;
	}

	return *this;
}