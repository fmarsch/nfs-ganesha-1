// ----------------------------------------------------------------------------
// Copyright IBM Corp. 2010, 2011
// All Rights Reserved
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Filename:    pt_ganesha.h
// Description: Define common macros and data types used by PT FSAL layer
// Author:      FSI IPC team
// ----------------------------------------------------------------------------

#ifndef __PT_GANESHA_H__
#define __PT_GANESHA_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "fsi_ipc_ccl.h"
#include "fsal.h"
#include "fsal_internal.h"
#include "fsal_convert.h"

// Linux includes
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/param.h>


// FSI new define
#define FSI_HANDLE_SIZE            32
#define fsi_dirent                 dirent
#define FSI_MAX_HANDLE_CACHE_ENTRY 2500

extern int       debug_flag;
extern struct    fsi_handle_cache_t  g_fsi_name_handle_cache;

void fsi_get_whole_path(const char * parentPath,
                        const char * name,
                        char       * path);
int  fsi_cache_name_and_handle(fsal_op_context_t * p_context,
                               char              * handle,
                               char              * name);
int  fsi_get_name_from_handle(fsal_op_context_t * p_context,
                              char              * handle,
                              char              * name);
int  fsi_update_cache_name(char * oldname,
                           char * newname);

void fsi_remove_cache_by_fullpath(char * path);

void fsi_remove_cache_by_handle(char * handle);

void ccl_log(int    debugLevel,
             char * debugString);

struct fsi_handle_cache_entry_t {
  char m_handle[FSI_HANDLE_SIZE];
  char m_name[PATH_MAX];
};

struct fsi_handle_cache_t {
  /* we need to set a constant to manage this and add LRU cleanup */
  struct fsi_handle_cache_entry_t m_entry[FSI_MAX_HANDLE_CACHE_ENTRY];
  // mutexes go here
  int m_count;
};

int ptfsal_stat_by_handle(fsal_handle_t     * p_filehandle,
                          fsal_op_context_t * p_context,
                          fsi_stat_struct   * p_stat);

int ptfsal_stat_by_name(fsal_op_context_t * p_context,
                        fsal_path_t       * p_fsalpath,
                        fsi_stat_struct   * p_stat);

int ptfsal_stat_by_parent_name(fsal_op_context_t * p_context,
                               fsal_handle_t     * p_parentdir_handle,
                               char              * p_filename,
                               fsi_stat_struct   * p_stat);

void ptfsal_convert_fsi_name(fsi_handle_struct   * handler,
                             const char          * filename,
                             char                * sv_filename,
                             enum e_fsi_name_enum  fsi_name_type);

int ptfsal_opendir(fsal_op_context_t * p_context,
                   const char        * filename,
                   const char        * mask,
                   uint32              attr);

int ptfsal_readdir(fsal_dir_t      * dir_desc,
                   fsi_stat_struct * sbuf,
                   char            * fsi_dname);

int ptfsal_closedir(fsal_dir_t * dir_desc);

int ptfsal_fsync (fsal_file_t * p_file_desciptor);

int ptfsal_open_by_handle(fsal_op_context_t * p_context,
                          fsal_handle_t     * p_object_handle,
                          int oflags, mode_t  mode);

int ptfsal_open(fsal_handle_t     * p_parent_directory_handle,
                fsal_name_t       * p_filename,
                fsal_op_context_t * p_context,
                mode_t              mode,
                fsal_handle_t     * p_object_handle);

int ptfsal_close(fsal_file_t * p_file_descriptor);

int ptfsal_close_mount_root(fsal_export_context_t * p_export_context);

int ptfsal_ftruncate(fsal_op_context_t * p_context,
                     int                 handle_index,
                     uint64_t            offset);

int ptfsal_unlink(fsal_op_context_t * p_context,
                  fsal_handle_t * p_parent_directory_handle,
                  char * p_filename);

int ptfsal_rename(fsal_op_context_t * p_context,
                  fsal_handle_t * p_old_parentdir_handle,
                  char * p_old_name,
                  fsal_handle_t * p_new_parentdir_handle,
                  char * p_new_name);

int fsi_check_handle_index(int handle_index);

int ptfsal_chown(fsal_op_context_t * p_context,
                 const char        * path,
                 uid_t               uid,
                 gid_t               gid);

int ptfsal_chmod(fsal_op_context_t * p_context,
                 const char        * path,
                 mode_t              mode);

int ptfsal_ntimes(fsal_op_context_t * p_context,
                  const char        * filename,
                  uint64_t            atime,
                  uint64_t            mtime);

int ptfsal_mkdir(fsal_handle_t     * p_parent_directory_handle,
                 fsal_name_t       * p_dirname,
                 fsal_op_context_t * p_context,
                 mode_t              mode,
                 fsal_handle_t     * p_object_handle);

int ptfsal_rmdir(fsal_op_context_t * p_context,
                 fsal_handle_t * p_parent_directory_handle,
                 char  * p_object_name);

int ptfsal_dynamic_fsinfo(fsal_handle_t        * p_filehandle,
                          fsal_op_context_t    * p_context,
                          fsal_dynamicfsinfo_t * p_dynamicinfo);

int ptfsal_readlink(fsal_handle_t     * p_linkhandle,
                    fsal_op_context_t * p_context,
                    char              * p_buf);

int ptfsal_symlink(fsal_handle_t     * p_parent_directory_handle,
                   fsal_name_t       * p_linkname,
                   fsal_path_t       * p_linkcontent,
                   fsal_op_context_t * p_context,
                   fsal_accessmode_t   accessmode,
                   fsal_handle_t     * p_link_handle);

int ptfsal_SetDefault_FS_specific_parameter(/* param TBD fsal_parameter_t * out_parameter */);

int ptfsal_name_to_handle(fsal_op_context_t * p_context,
                          fsal_path_t       * p_fsalpath,
                          fsal_handle_t     * p_handle);

int ptfsal_handle_to_name(fsal_handle_t     * p_filehandle,
                          fsal_op_context_t * p_context,
                          char              * path);

uint64_t ptfsal_read(fsal_file_t * p_file_descriptor,
                     char        * buf,
                     size_t        size,
                     off_t         offset,
                     int           in_handle);

uint64_t ptfsal_write(fsal_file_t * p_file_descriptor,
                      const char  * buf,
                      size_t        size,
                      off_t         offset,
                      int           in_handle);

void ptfsal_print_handle(char * handle);

#endif // ifndef __PT_GANESHA_H__