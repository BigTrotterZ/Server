#ifndef _USER_H_
#define _USER_H_

#include <memory>

class User
{
public:
	int GetClientFd() const { return clientfd;}

private:
	int clientfd;

};

typedef std::shared_ptr< User > UserPtr;


#endif