#include "permission.h"
static int streq(const char* a, const char* b) {
    if (!a || !b) return 0;
    while (*a && *b) { if (*a != *b) return 0; a++; b++; }
    return *a == *b;
}
const char* perm_level_name(perm_level_t l) {
    switch (l) {
    case PERM_RESTRICTED: return "Restricted";
    case PERM_USER: return "User";
    case PERM_ADMIN: return "Admin";
    case PERM_ROOT: return "Root";
    case PERM_SUPERROOT: return "SuperRoot";
    default: return "Unknown";
    }
}
int perm_level_has_gui_entry(perm_level_t l) { return l == PERM_USER || l == PERM_ADMIN || l == PERM_ROOT; }
int perm_level_can_attempt_kernel_hotswap(perm_level_t l) { return l == PERM_SUPERROOT; }
int perm_command_is_hidden_from_help(const char* c) { return streq(c, "unlock-superroot") || streq(c, "enter-restricted"); }
int perm_command_allowed(perm_level_t cur, const char* cmd) {
    if (streq(cmd, "help")) return 1;
    if (streq(cmd, "unlock-superroot")) return cur == PERM_ADMIN || cur == PERM_ROOT;
    if (streq(cmd, "enter-restricted")) return cur >= PERM_USER;
    return 0;
}
uint32_t perm_default_capabilities(perm_level_t l) {
    switch (l) {
    case PERM_RESTRICTED: return 0;
    case PERM_USER: return CAP_NETWORK;
    case PERM_ADMIN:
    case PERM_ROOT: return CAP_CAMERA | CAP_MIC | CAP_LOCATION | CAP_FILESYSTEM | CAP_NETWORK | CAP_USB;
    case PERM_SUPERROOT: return CAP_CAMERA | CAP_MIC | CAP_LOCATION | CAP_FILESYSTEM | CAP_NETWORK | CAP_USB | CAP_KERNEL_HOTSWAP;
    default: return 0;
    }
}
const char* perm_hotswap_policy_text(void) {
    return "Only SuperRoot may attempt kernel hotswap after offline key + physical confirmation + forced snapshot; all others forbidden.";
}
