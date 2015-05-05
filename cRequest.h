#pragma once
#include "cServChain.h"

class cRequest:
	public cServChain
{
private:
	ID	id;
	double startingTime;
	double durationTime;
	bool isService;

public:
	void setId(ID _id){id = _id;}
	ID   getId(void) const {return id;}

	void setArrivalTime(double _time){startingTime = _time;}
	double getArrivalTime(void) const {return startingTime;}

	void setDuringTime(double _time){durationTime = _time;}
	double getDuringTime(void) const {return durationTime;}

	void setIsService(bool _isService) {isService = _isService;}
	bool getIsService(void)const {return isService;}

public:
	cRequest(void);
	cRequest(ID _id, double _start_time = 0, double _duration_time = 0,bool _isService = false)\
		:id(_id),startingTime(_start_time),durationTime(_duration_time),isService(_isService){}
	cRequest(const cRequest& _request);
	cRequest& operator=(const cRequest& _request);
	~cRequest(void);
};



