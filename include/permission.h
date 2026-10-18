#ifndef WLHIOS_PERMISSION_H
#define WLHIOS_PERMISSION_H
#include "types.h"
typedef enum {
    PERM_RESTRICTED = 0,
    PERM_USER = 1,
    PERM_ADMIN = 2,
    PERM_ROOT = 3,
    PERM_SUPERROOT = 4
} perm_level_t;
typedef enum {
    CAP_CAMERA = 1u << 0,
    CAP_MIC = 1u << 1,
    CAP_LOCATION = 1u << 2,
    CAP_FILESYSTEM = 1u << 3,
    CAP_NETWORK = 1u << 4,
    CAP_USB = 1u << 5,
    CAP_KERNEL_HOTSWAP = 1u << 31
} perm_capability_t;
const char* perm_level_name(perm_level_t level);
int perm_level_has_gui_entry(perm_level_t level);
int perm_level_can_attempt_kernel_hotswap(perm_level_t level);
int perm_command_is_hidden_from_help(const char* cmd);
int perm_command_allowed(perm_level_t current, const char* cmd);
uint32_t perm_default_capabilities(perm_level_t level);
const char* perm_hotswap_policy_text(void);
#endif
