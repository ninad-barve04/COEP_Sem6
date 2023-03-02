/**
 * @file ext2.c
 * @author Ninad Barve
 * @brief Write a program which given the complete path name, will print the
 *        (a) inode of the file/directory specified, from an ext2 file system.
 *        (b) contents of the file or directory
 * 
 * @copyright Copyright (c) 2023
 * 
 */


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

/*Utility functions*/
int get_blocksize(int fd);
unsigned long get_inode_offset(int fd, int inodeno);
int get_inode(int fd, char *filepath, struct ext2_inode **s_node);

/*Printing of inodes*/
void print_inode(int fd, int blksiz, struct ext2_inode *inode);
void print_blocks(int fd, int blksiz, struct ext2_inode *inode);
void no_indirection_print(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks);
void single_indirection_print(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset);
void double_indirection_print(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset);
void triple_indirection_print(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset);

/*Printing of data*/
void print_data(int fd, int blksiz, struct ext2_inode *inode);
void no_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks);
void single_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset);
void double_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset);
void triple_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset);


int main(int argc, char const *argv[])
{
    if (argc != 4) {
        printf("No arguments passed\nformat: ./a.out  device-file-name  path-on-partition  inode/data\n");
        exit(errno);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("ext2.c:");
        exit(errno);
    }
    //TODO: add filepath errorcheck here
    int siz, inode_no;

    int blksize = get_blocksize(fd);

    if (strcmp(argv[3], "inode") == 0) {
        struct ext2_inode *inode = NULL;
        inode = (struct ext2_inode *)malloc(sizeof(struct ext2_inode));
        inode_no = get_inode(fd, argv[2], &inode);
        if (inode != NULL) {
            print_inode(fd, blksize, inode);
        }
        
    }
    else if (strcmp(argv[3], "data") == 0) {
        struct ext2_inode *inode = NULL;
        inode = (struct ext2_inode *)malloc(sizeof(struct ext2_inode));
        inode_no = get_inode(fd, argv[2], &inode);
        if (inode != NULL) {
            printf("Data of file %s:\n", argv[2]);
            print_data(fd, blksize, inode);
        }
    } else {
        exit(errno);
    }



    return 0;
}

/**
 * @brief Get the blocksize of filesystem
 * 
 * @param fd    : file descriptor number
 * @return int  : blocksize
 */
int get_blocksize(int fd) {
    int siz;
    struct ext2_super_block supblk;
    lseek64(fd, 1024, SEEK_SET);
    siz = read(fd, &supblk, sizeof(struct ext2_super_block));
    return 1024 << supblk.s_log_block_size;
}

/**
 * @brief Get the inode offset from SEEK_SET
 * 
 * @param fd        : file descriptor
 * @param inodeno   : inode number
 * @return unsigned : long offset in "bytes"
 */
unsigned long get_inode_offset(int fd, int inodeno) {
    if (inodeno < 2) {
        printf("Invalid inode\n");
        return -1;
    }
    unsigned long siz, block_size, grpno;
    struct ext2_super_block supblk;
    struct ext2_group_desc grpdesc;

    lseek64(fd, 1024, SEEK_SET);
    siz = read(fd, &supblk, sizeof(struct ext2_super_block));

    block_size = 1024 << supblk.s_log_block_size;
    grpno = (inodeno - 1) / supblk.s_inodes_per_group;

    lseek64(fd, block_size + (grpno*sizeof(struct ext2_group_desc)), SEEK_SET);
    siz = read(fd, &grpdesc, sizeof(struct ext2_group_desc));
    
    unsigned long inode_index = (inodeno - 1) % supblk.s_inodes_per_group;
    unsigned long offset = (grpdesc.bg_inode_table * block_size) + (inode_index * supblk.s_inode_size);

    return offset;
}

/**
 * @brief Get the inode struct from filepath
 * 
 * @param fd        : file descriptor
 * @param filepath  : path from root
 * @param s_node    : pointer to struct pointer
 * @return int      : inode number
 */
int get_inode(int fd, char *filepath, struct ext2_inode **s_node) {
    struct ext2_inode *inode = *s_node;

    int pathlen = strlen(filepath);
    int count = 0;
    for (int i = 0; i < pathlen; i++) {
        if (filepath[i] == '/') {
            count++;
        }
    }
    char **names = (char *)malloc(sizeof(char *) * count);
    names[0] = strtok(filepath, "/");
    for (int i = 1; i < count; i++) {
        names[i] = strtok(NULL, "/");
    }

    unsigned long inode_offset, inode_data_offset;
    inode_offset = get_inode_offset(fd, 2);

    unsigned long blocksize = get_blocksize(fd);

    lseek64(fd, inode_offset, SEEK_SET);
    read(fd, inode, sizeof(struct ext2_inode));
    if (pathlen == 1) {
        /*root dir asked*/
        return;   
    }

    inode_data_offset = inode->i_block[0];
    inode_data_offset = inode_data_offset * blocksize;

    struct ext2_dir_entry_2 dirent;
    int found_flag = 0;
    for (int i = 0; i < count; i++) {
        found_flag = 0;
        lseek64(fd, inode_data_offset, SEEK_SET);
        read(fd, &dirent, sizeof(struct ext2_dir_entry_2));

        while (dirent.inode != 0) {
            if (strcmp(dirent.name, names[i]) == 0) {
                found_flag = 1;
                break;
            } 
            inode_data_offset += dirent.rec_len;
            lseek64(fd, dirent.rec_len - sizeof(dirent), SEEK_CUR);
            read(fd, &dirent, sizeof(struct ext2_dir_entry_2));
        }
        if (found_flag == 1) {

            inode_offset = get_inode_offset(fd, dirent.inode);

            lseek64(fd, inode_offset, SEEK_SET);
            read(fd, inode, sizeof(struct ext2_inode));

            inode_data_offset = inode->i_block[0];
            inode_data_offset = inode_data_offset * blocksize;
            
            
        } else {
            // if (dirent.file_type )
            printf("Directory or file %s not found\n", names[i]);
            break;
        }
        
    }

    if (found_flag == 0) {
        printf("Inode not found\n");
        inode = NULL;
        *s_node = inode;
        return -1;
    }

    inode_offset = get_inode_offset(fd, dirent.inode);
    lseek64(fd, inode_offset, SEEK_SET);
    read(fd, inode, sizeof(struct ext2_inode));
    
    *s_node = inode;
    return dirent.inode;

}


/*************Printing the data inside respective inode*******************/

/**
 * @brief Print the data inside data blocks of given inode
 * 
 * @param fd        : file descriptor
 * @param blksiz    : blocksize of the filesystem
 * @param inode     : inode struct
 */
void print_data(int fd, int blksiz, struct ext2_inode *inode) {
    int i = 0, j = 0, remaining_blocks, double_entries, max_table_entries, offset;
    int nblocks = 512 * inode->i_blocks / blksiz;
    int *first_indirect, *double_indirect;

    if (nblocks <= 12) {
        no_indirection(fd, blksiz, inode, nblocks);
    } else {
        no_indirection(fd, blksiz, inode, 12);
        remaining_blocks = nblocks - 12;
        max_table_entries = (blksiz / sizeof(int));

        if (remaining_blocks <= max_table_entries) {
            /*Single indirection*/
            single_indirection(fd, blksiz, inode, remaining_blocks, inode->i_block[12]);
        } else {
            single_indirection(fd, blksiz, inode, max_table_entries, inode->i_block[12]);
            remaining_blocks = remaining_blocks - max_table_entries;
            
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

/**
 * @brief Writing in case of no indirection
 * 
 * @param fd                : file descriptor
 * @param blksiz            : blocksize of file system
 * @param inode             : inode struct
 * @param remaining_blocks  : remaining number of blocks to print
 */
void no_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks) {
    int i = 0;
    char buffer[blksiz];
    for (i = 0; i < remaining_blocks; i++) {
        lseek64(fd, inode->i_block[i] * blksiz, SEEK_SET);
        read(fd, buffer, blksiz);
        write(1, buffer, blksiz);
    }
    return;
}

/**
 * @brief Writing data single indirection
 * 
 * @param fd                : file descriptor
 * @param blksiz            : blocksize of file system
 * @param inode             : inode struct
 * @param remaining_blocks  : remaining number of blocks to print
 * @param offset            : offset blocks
 */
void single_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset) {
    int j = 0;
    // int *indirect = (int *)malloc(sizeof(int) * remaining_blocks);
    int indirect[remaining_blocks];
    char buffer[blksiz];
    lseek64(fd, offset*blksiz, SEEK_SET);
    read(fd, indirect, remaining_blocks*sizeof(int));
    for (j = 0; j < remaining_blocks; j++) {
        lseek64(fd, indirect[j] * blksiz, SEEK_SET);
        read(fd, buffer, blksiz);
        write(1, buffer, blksiz);
        printf("%d, ", indirect[j]);
    }
    return;
}

/**
 * @brief Writing data double indirection
 * 
 * @param fd                : file descriptor
 * @param blksiz            : blocksize of file system
 * @param inode             : inode struct
 * @param remaining_blocks  : remaining number of blocks to print
 * @param offset            : offset blocks
 */
void double_indirection(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset) {
    int j = 0, double_entries = 0;
    int max_table_entries = (blksiz / sizeof(int));
    double_entries = remaining_blocks / max_table_entries;

    int *dindirect = (int *)malloc(sizeof(int) * double_entries);
    
    lseek64(fd, offset*blksiz, SEEK_SET);
    read(fd, dindirect, remaining_blocks*sizeof(int));
    for (j = 0; j < double_entries; j++) {
        if (j == double_entries - 1) {
            single_indirection(fd, blksiz, inode, remaining_blocks, dindirect[j]);
        } else {
            single_indirection(fd, blksiz, inode, max_table_entries, dindirect[j]);
            remaining_blocks = remaining_blocks - max_table_entries;
        }
    }
    return;
}

/**
 * @brief Writing data triple indirection
 * 
 * @param fd                : file descriptor
 * @param blksiz            : blocksize of file system
 * @param inode             : inode struct
 * @param remaining_blocks  : remaining number of blocks to print
 * @param offset            : offset blocks
 */
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


/*******Printing the block numbers and inode information inside respective inode*************/

/**
 * @brief Print inode data
 * 
 * @param fd        : file descriptor
 * @param blksiz    : blocksize of file system
 * @param inode     : inode struct
 */
void print_inode(int fd, int blksiz, struct ext2_inode *inode) {
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
    printf("generation  : %ld\n", inode->i_generation);
    printf("file acl    : %ld\n", inode->i_file_acl);
    printf("faddr       : %ld\n", inode->i_faddr);
    print_blocks(fd, blksiz, inode);
    return;
}

/**
 * @brief Print block numbers
 * 
 * @param fd        : file descriptor
 * @param blksiz    : blocksize of file system
 * @param inode     : inode struct
 */
void print_blocks(int fd, int blksiz, struct ext2_inode *inode) {
    int i = 0, j = 0, remaining_blocks, double_entries, max_table_entries, offset;
    int nblocks = 512 * inode->i_blocks / blksiz;
    int *first_indirect, *double_indirect;
    printf("BLOCKS:\n");
    if (nblocks <= 12) {
        no_indirection_print(fd, blksiz, inode, nblocks);
    } else {
        no_indirection_print(fd, blksiz, inode, 12);
        remaining_blocks = nblocks - 12;
        max_table_entries = (blksiz / sizeof(int));

        if (remaining_blocks <= max_table_entries) {
            /*Single indirection*/
            single_indirection_print(fd, blksiz, inode, remaining_blocks, inode->i_block[12]);
        } else {
            single_indirection_print(fd, blksiz, inode, max_table_entries, inode->i_block[12]);
            remaining_blocks = remaining_blocks - max_table_entries;
            
            if (remaining_blocks <= max_table_entries*max_table_entries) {
                /*Double indirection to do*/
                double_indirection_print(fd, blksiz, inode, remaining_blocks, inode->i_block[13]);
            } else {
                /*Triple indirection to do*/
                double_indirection_print(fd, blksiz, inode, max_table_entries * max_table_entries, inode->i_block[13]);
                remaining_blocks = remaining_blocks - (max_table_entries * max_table_entries);
                triple_indirection_print(fd, blksiz, inode, max_table_entries, inode->i_block[14]);
            }

        }
    }
    return;
}

/**
 * @brief Printing blocks in case of no indirection
 * 
 * @param fd                : file descriptor
 * @param blksiz            : blocksize of file system
 * @param inode             : inode struct
 * @param remaining_blocks  : remaining number of blocks to print
 */
void no_indirection_print(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks) {
    int i = 0;
    for (i = 0; i < remaining_blocks; i++) {
        printf("%d, ", inode->i_block[i]);
    }
    return;
}

/**
 * @brief Print inode data blocks single indirection
 * 
 * @param fd                : file descriptor
 * @param blksiz            : blocksize of file system
 * @param inode             : inode struct
 * @param remaining_blocks  : remaining number of blocks to print
 * @param offset            : offset blocks
 */
void single_indirection_print(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset) {
    int j = 0;
    // int *indirect = (int *)malloc(sizeof(int) * remaining_blocks);
    int indirect[remaining_blocks];
    lseek64(fd, offset*blksiz, SEEK_SET);
    read(fd, indirect, remaining_blocks*sizeof(int));
    for (j = 0; j < remaining_blocks; j++) {
        printf("%d, ", indirect[j]);
    }
    return;
}

/**
 * @brief Print inode data blocks double indirection
 * 
 * @param fd                : file descriptor
 * @param blksiz            : blocksize of file system
 * @param inode             : inode struct
 * @param remaining_blocks  : remaining number of blocks to print
 * @param offset            : offset blocks
 */
void double_indirection_print(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset) {
    int j = 0, double_entries = 0;
    int max_table_entries = (blksiz / sizeof(int));
    double_entries = remaining_blocks / max_table_entries;

    int *dindirect = (int *)malloc(sizeof(int) * double_entries);
    
    lseek64(fd, offset*blksiz, SEEK_SET);
    read(fd, dindirect, remaining_blocks*sizeof(int));
    for (j = 0; j < double_entries; j++) {
        if (j == double_entries - 1) {
            single_indirection_print(fd, blksiz, inode, remaining_blocks, dindirect[j]);
        } else {
            single_indirection_print(fd, blksiz, inode, max_table_entries, dindirect[j]);
            remaining_blocks = remaining_blocks - max_table_entries;
        }
    }
    return;
}

/**
 * @brief Print inode data blocks triple indirection
 * 
 * @param fd                : file descriptor
 * @param blksiz            : blocksize of file system
 * @param inode             : inode struct
 * @param remaining_blocks  : remaining number of blocks to print
 * @param offset            : offset blocks
 */
void triple_indirection_print(int fd, int blksiz, struct ext2_inode *inode, int remaining_blocks, int offset) {
    int j = 0, double_entries = 0;
    int max_table_entries = (blksiz / sizeof(int));
    double_entries = remaining_blocks / max_table_entries;

    int *dindirect = (int *)malloc(sizeof(int) * double_entries);
    
    lseek64(fd, offset*blksiz, SEEK_SET);
    read(fd, dindirect, remaining_blocks*sizeof(int));
    for (j = 0; j < double_entries; j++) {
        if (j == double_entries - 1) {
            double_indirection_print(fd, blksiz, inode, remaining_blocks, dindirect[j]);
        } else {
            double_indirection_print(fd, blksiz, inode, max_table_entries, dindirect[j]);
            remaining_blocks = remaining_blocks - max_table_entries;
        }
    }
    return;
}


