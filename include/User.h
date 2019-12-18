#ifndef _USER_H_
#define _USER_H_

class User
{
public:
	int GetClientFd() { return clientfd;}

private:
	int clientfd;

};

typedef std::shared_ptr< User > UserPtr;


#endif