#ifndef WLHIOS_VFS_H
#define WLHIOS_VFS_H
#include "types.h"

#define VFS_FILE 0
#define VFS_DIR  1

typedef struct fs_node {
    char name[32];
    uint32_t flags; // VFS_FILE or VFS_DIR
    uint32_t size;  // 文件大小
    void* data;     // 指向文件内容的指针 (RAMFS 专用)
    
    struct fs_node* parent;
    struct fs_node* first_child;
    struct fs_node* next_sibling;
} fs_node_t;

void vfs_init(void);
fs_node_t* vfs_get_root(void);
fs_node_t* vfs_find(const char* name);
fs_node_t* vfs_create(const char* name, uint32_t flags);
void vfs_list(void);

#endif
