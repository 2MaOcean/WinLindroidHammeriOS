#include "vfs.h"
#include "heap.h"
#include "console.h"

static fs_node_t* root = 0;
static fs_node_t* current_dir = 0;

// 简易字符串拷贝与比较
static void k_strcpy(char* dest, const char* src, int max_len) {
    int i = 0;
    while (src[i] && i < max_len - 1) { dest[i] = src[i]; i++; }
    dest[i] = '\0';
}
static int k_streq(const char* a, const char* b) {
    while (*a && *b) { if (*a != *b) return 0; a++; b++; }
    return *a == *b;
}

void vfs_init(void) {
    // 创建根目录 "/"
    root = (fs_node_t*)kmalloc(sizeof(fs_node_t));
    k_strcpy(root->name, "/", 32);
    root->flags = VFS_DIR;
    root->parent = 0;
    root->first_child = 0;
    root->next_sibling = 0;
    current_dir = root;
    console_puts("[VFS] RAMFS mounted at /\n");
}

fs_node_t* vfs_get_root(void) { return root; }

fs_node_t* vfs_find(const char* name) {
    fs_node_t* child = current_dir->first_child;
    while (child) {
        if (k_streq(child->name, name)) return child;
        child = child->next_sibling;
    }
    return 0;
}

fs_node_t* vfs_create(const char* name, uint32_t flags) {
    if (vfs_find(name)) return 0; // 已存在

    fs_node_t* new_node = (fs_node_t*)kmalloc(sizeof(fs_node_t));
    k_strcpy(new_node->name, name, 32);
    new_node->flags = flags;
    new_node->size = 0;
    new_node->data = 0;
    new_node->parent = current_dir;
    new_node->first_child = 0;
    new_node->next_sibling = 0;

    // 插入到当前目录的子节点链表
    if (!current_dir->first_child) {
        current_dir->first_child = new_node;
    } else {
        fs_node_t* temp = current_dir->first_child;
        while (temp->next_sibling) temp = temp->next_sibling;
        temp->next_sibling = new_node;
    }
    return new_node;
}

void vfs_list(void) {
    fs_node_t* child = current_dir->first_child;
    if (!child) {
        console_puts("(empty directory)\n");
        return;
    }
    while (child) {
        if (child->flags == VFS_DIR) console_puts("[DIR]  ");
        else console_puts("[FILE] ");
        console_puts(child->name);
        console_puts("\n");
        child = child->next_sibling;
    }
}

int vfs_write(fs_node_t* node, const char* buffer, uint32_t size) {
    if (!node || node->flags != VFS_FILE) return -1;
    
    // 如果之前有数据，先释放
    if (node->data) kfree(node->data);
    
    // 分配新内存并复制数据
    node->data = kmalloc(size + 1);
    if (!node->data) return -1;
    
    for (uint32_t i = 0; i < size; i++) {
        ((char*)node->data)[i] = buffer[i];
    }
    ((char*)node->data)[size] = '\0'; // 确保字符串结束
    node->size = size;
    return 0;
}

int vfs_read(fs_node_t* node, char* buffer, uint32_t max_size) {
    if (!node || node->flags != VFS_FILE || !node->data) return -1;
    
    uint32_t read_size = node->size < max_size - 1 ? node->size : max_size - 1;
    for (uint32_t i = 0; i < read_size; i++) {
        buffer[i] = ((char*)node->data)[i];
    }
    buffer[read_size] = '\0';
    return read_size;
}
