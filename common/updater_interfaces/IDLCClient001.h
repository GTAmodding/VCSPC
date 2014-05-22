#ifndef __DLCCLIENT001
#define __DLCCLIENT001

class IDLCClient001
{
public:
	virtual					~IDLCClient001() {};
	
	virtual void			SendSerialCodeRequest(const std::string* request)=0;
	virtual void			RegisterOnFinishedRequestCallback(SerialCodeRequestCallback callback)=0;
};

#endif