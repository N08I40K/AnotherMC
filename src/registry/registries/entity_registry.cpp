//
// Created by n08i40k on 13.08.2024.
//

#include "registry/registry.h"
#include "world/entity/entity.h"
#include "world/entity/player.h"

registry<entity>::registry() { add("player", std::make_unique<entities::player>()); }
