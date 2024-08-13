//
// Created by n08i40k on 13.08.2024.
//

#ifndef SERVER_H
#define SERVER_H
#include "world/world.h"

class server {
	world world;

public:
	server();
	~server();

	[[nodiscard]] ::world&
	get_world() { return world; }

	void tick();
};

#endif //SERVER_H
