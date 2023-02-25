#include <ext2fs/ext2_fs.h>

int main(int argc, char const *argv[])
{
    struct ext2_inode inode;
    printf("%d\t%d\n", sizeof(struct ext2_inode), sizeof(inode));
    return 0;
}
