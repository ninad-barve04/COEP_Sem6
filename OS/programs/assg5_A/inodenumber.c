#define _LARGEFILE64_SOURCE     /* See feature_test_macros(7) */

#include <stdio.h>
#include <stdlib.h>
#include <ext2fs/ext2_fs.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/types.h>
#include <unistd.h>


void print_inode_data(int fd, int blksiz, struct ext2_inode *inode);
void print_blocks(int fd, int blksiz, struct ext2_inode *inode);
void single_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset);
void double_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset);
void triple_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset);

int main(int argc, char const *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("inodenumber:");
        exit(errno);
    }
    int inodeno = atoi(argv[2]);
    int siz, ngroups;
    int inode_size;
    int block_size;

    int grpno, inode_index;
    /*Struct declerations needed here*/
    struct ext2_super_block supblk;
    struct ext2_group_desc grpdesc;
    struct ext2_inode inode;

    /*Shifting by the Boot block offset*/
    lseek64(fd, 1024, SEEK_SET);
    siz = read(fd, &supblk, sizeof(struct ext2_super_block));

    inode_size = supblk.s_inode_size;
    printf("Inode size: %d\t:%d\n", inode_size, sizeof(struct ext2_inode));
    printf("Magic number: %X\n", supblk.s_magic);
    block_size = 1024 << supblk.s_log_block_size;
    printf("Block size: %d\n", block_size);
    grpno = (inodeno - 1) / supblk.s_inodes_per_group;
    printf("Group number: %d\n", grpno);
    inode_index = (inodeno - 1) % supblk.s_inodes_per_group;
    printf("Inode index: %d\n", inode_index);

    lseek64(fd, block_size + (grpno*sizeof(struct ext2_group_desc)), SEEK_SET);
    siz = read(fd, &grpdesc, sizeof(struct ext2_group_desc));
    printf("Grpdesd size read: %d\n", siz);
    printf("BG Inode table: %d\n", grpdesc.bg_inode_table);
    printf("offset: %d\n", (grpdesc.bg_inode_table * block_size) + (inode_index * sizeof(struct ext2_inode)));
    // lseek64(fd, (grpdesc.bg_inode_table * block_size) + (inode_index * sizeof(struct ext2_inode)), SEEK_SET);
    lseek64(fd, (grpdesc.bg_inode_table * block_size) + (inode_index * inode_size), SEEK_SET);
    siz = read(fd, &inode, sizeof(struct ext2_inode));

    /*printing the inode*/
    print_inode_data(fd, block_size, &inode);

    return 0; 
}



void print_inode_data(int fd, int blksiz, struct ext2_inode *inode) {
    printf("Mode        : %ld\n", inode->i_mode);
    printf("Uid         : %ld\n", inode->i_uid);
    printf("Size        : %ld\n", inode->i_size);
    printf("atime       : %ld\n", inode->i_atime);
    printf("ctime       : %ld\n", inode->i_ctime);
    printf("mtime       : %ld\n", inode->i_mtime);
    printf("dtime       : %ld\n", inode->i_dtime);
    printf("gid         : %ld\n", inode->i_gid);
    printf("links count : %ld\n", inode->i_links_count);
    printf("blocks      : %ld\n", 512 * inode->i_blocks / blksiz);
    printf("flags       : %ld\n", inode->i_flags);
    // printf("%d\n", inode->i_osd1);
    print_blocks(fd, blksiz, inode);
    printf("generation  : %ld\n", inode->i_generation);
    printf("file acl    : %ld\n", inode->i_file_acl);
    printf("faddr       : %ld\n", inode->i_faddr);
    // printf("%d\n", inode->i_osd2);

}


void print_blocks(int fd, int blksiz, struct ext2_inode *inode) {
    int i = 0, j = 0, remaining_blocks, double_entries, max_table_entries, offset;
    int nblocks = 512 * inode->i_blocks / blksiz;
    int *first_indirect, *double_indirect;
    printf("BLOCKS:\n");
    if (nblocks <= 12) {
        // for (i = 0; i < nblocks; i++) {
        //     printf("%d\n", inode->i_block[i]);
        // }
        no_indirection(fd, blksiz, inode, nblocks);
    } else {
        // for (i = 0; i < 12; i++) {
        //     printf("%d\t%d\n", i, inode->i_block[i]);
        // }
        no_indirection(fd, blksiz, inode, 12);
        remaining_blocks = nblocks - 12;
        max_table_entries = (blksiz / sizeof(int));
        printf("remblks = %d\n", remaining_blocks);

        if (remaining_blocks <= max_table_entries) {
            /*Single indirection*/
            single_indirection(fd, blksiz, inode, remaining_blocks, inode->i_block[12]);
        } else {
            single_indirection(fd, blksiz, inode, max_table_entries, inode->i_block[12]);
            remaining_blocks = remaining_blocks - max_table_entries;
            printf("remblks = %d\n", remaining_blocks);
            
            if (remaining_blocks <= max_table_entries*max_table_entries) {
                /*Double indirection to do*/
                double_indirection(fd, blksiz, inode, remaining_blocks, inode->i_block[13]);
            } else {
                /*Triple indirection to do*/
                double_indirection(fd, blksiz, inode, max_table_entries * max_table_entries, inode->i_block[13]);
                remaining_blocks = remaining_blocks - (max_table_entries * max_table_entries);
                triple_indirection(fd, blksiz, inode, max_table_entries, inode->i_block[14]);
            }

        }
    }
    return;
}

void no_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks) {
    int i = 0;
    printf("(");
    for (i = 0; i < remaining_blocks; i++) {
        printf("%d, ", inode->i_block[i]);
    }
    printf(")\n");
    return;
}

void single_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset) {
    int j = 0;
    // int *indirect = (int *)malloc(sizeof(int) * remaining_blocks);
    int indirect[remaining_blocks];
    printf("1 IND: %d\n", offset);
    lseek64(fd, offset*blksiz, SEEK_SET);
    read(fd, indirect, remaining_blocks*sizeof(int));
    printf("(");
    for (j = 0; j < remaining_blocks; j++) {
        // printf("(%d, %d), ", 12+j, indirect[j]);
        printf("%d, ", indirect[j]);
    }
    printf(")\n");
    return;
}

void double_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset) {
    int j = 0, double_entries = 0;
    int max_table_entries = (blksiz / sizeof(int));
    double_entries = remaining_blocks / max_table_entries;

    int *dindirect = (int *)malloc(sizeof(int) * double_entries);
    printf("2 IND: %d\n", offset);
    
    lseek64(fd, offset*blksiz, SEEK_SET);
    read(fd, dindirect, remaining_blocks*sizeof(int));
    for (j = 0; j < double_entries; j++) {
        // printf("\t%d\t%d\n", 12+j, indirect[j]);
        if (j == double_entries - 1) {
            single_indirection(fd, blksiz, inode, remaining_blocks, dindirect[j]);
        } else {
            single_indirection(fd, blksiz, inode, max_table_entries, dindirect[j]);
            remaining_blocks = remaining_blocks - max_table_entries;
        }
    }
    return;
}

void triple_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset) {
    int j = 0, double_entries = 0;
    int max_table_entries = (blksiz / sizeof(int));
    double_entries = remaining_blocks / max_table_entries;

    int *dindirect = (int *)malloc(sizeof(int) * double_entries);
    printf("2 IND: %d\n", offset);
    
    lseek64(fd, offset*blksiz, SEEK_SET);
    read(fd, dindirect, remaining_blocks*sizeof(int));
    for (j = 0; j < double_entries; j++) {
        // printf("\t%d\t%d\n", 12+j, indirect[j]);
        if (j == double_entries - 1) {
            double_indirection(fd, blksiz, inode, remaining_blocks, dindirect[j]);
        } else {
            double_indirection(fd, blksiz, inode, max_table_entries, dindirect[j]);
            remaining_blocks = remaining_blocks - max_table_entries;
        }
    }
    return;
}
