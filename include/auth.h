#ifndef WLHIOS_AUTH_H
#define WLHIOS_AUTH_H
#include "types.h"
#include "permission.h"

void auth_init(void);
int auth_login(perm_level_t* out_level);
void auth_register(void);

#endif
