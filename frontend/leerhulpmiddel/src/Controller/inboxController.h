#ifndef INBOXCONTROLLER_H
#define INBOXCONTROLLER_H

#include "../model/manager/NetworkManager.h"

class inboxController
{
public:
	inline inboxController() : networkManager{ new NetworkManager } {};

private:
	NetworkManager* networkManager;
};

#endif