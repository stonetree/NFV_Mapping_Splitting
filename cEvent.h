#pragma once
#include "common.h"
#include "stdafx.h"
#include "cRequest.h"

class cEvent
{
private:
	event_type type;
	cRequest* p_request;

public:
	event_type getEventType(void){return type;}
	cRequest* getRequestPoint(void){return p_request;}
public:
	cEvent(void);
	cEvent(event_type _type,cRequest* _p_request):type(_type),p_request(_p_request){}
	cEvent(const cEvent& _event){operator=(_event);}
	cEvent& operator=(const cEvent& _event);
	~cEvent(void);
};

