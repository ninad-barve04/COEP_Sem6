#define _LARGEFILE64_SOURCE     /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ext2fs/ext2_fs.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/types.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
    struct ext2_dir_entry_2 dirent;

    int fd = open("/dev/sda9", O_RDONLY);

    int inode_data_offset= 751 * 4096;
    lseek64(fd, inode_data_offset, SEEK_SET);
    read(fd, &dirent, sizeof(struct ext2_dir_entry_2));

    while (dirent.inode != 0) {
        printf("%s\n", dirent.name);
        inode_data_offset += dirent.rec_len;
        lseek64(fd, dirent.rec_len - sizeof(dirent), SEEK_CUR);
        read(fd, &dirent, sizeof(struct ext2_dir_entry_2));
    }
    return 0;
}
