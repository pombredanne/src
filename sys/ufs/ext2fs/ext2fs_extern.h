/*	$OpenBSD: ext2fs_extern.h,v 1.12.2.1 2002/06/11 03:32:49 art Exp $	*/
/*	$NetBSD: ext2fs_extern.h,v 1.9 2000/11/27 08:39:53 chs Exp $	*/

/*-
 * Copyright (c) 1997 Manuel Bouyer.
 * Copyright (c) 1991, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)ffs_extern.h	8.3 (Berkeley) 4/16/94
 * Modified for ext2fs by Manuel Bouyer.
 */

struct buf;
struct fid;
struct m_ext2fs;
struct inode;
struct mount;
struct nameidata;
struct proc;
struct statfs;
struct timeval;
struct ucred;
struct ufsmount;
struct uio;
struct vnode;
struct vfsconf;
struct mbuf;
struct componentname;

extern struct pool ext2fs_inode_pool;		/* memory pool for inodes */

__BEGIN_DECLS

/* ext2fs_alloc.c */
int ext2fs_alloc(struct inode *, ufs_daddr_t, ufs_daddr_t , struct ucred *,
		   ufs_daddr_t *);
int ext2fs_realloccg(struct inode *, ufs_daddr_t, ufs_daddr_t, int, int,
			  struct ucred *, struct buf **);
int ext2fs_reallocblks(void *);
int ext2fs_inode_alloc(struct inode *pip, int mode, struct ucred *, 
    struct vnode **);
daddr_t ext2fs_blkpref(struct inode *, ufs_daddr_t, int, ufs_daddr_t *);
void ext2fs_blkfree(struct inode *, ufs_daddr_t);
int ext2fs_inode_free(struct inode *pip, ino_t ino, int mode);

/* ext2fs_balloc.c */
int ext2fs_buf_alloc(struct inode *, daddr_t, int, struct ucred *,
			struct buf **, int);
int ext2fs_gop_alloc __P((struct vnode *, off_t, off_t, int, struct ucred *));
int ext2fs_balloc_range __P((struct vnode *, off_t, off_t, struct ucred *,
			     int));

/* ext2fs_bmap.c */
int ext2fs_bmap(void *);

/* ext2fs_inode.c */
int ext2fs_init(struct vfsconf *);
int ext2fs_update(struct inode *ip, struct timespec *atime,
    struct timespec *mtime, int waitfor);
int ext2fs_truncate(struct inode *, off_t, int, struct ucred *);
int ext2fs_inactive(void *);

/* ext2fs_lookup.c */
int ext2fs_readdir(void *);
int ext2fs_lookup(void *);
int ext2fs_direnter(struct inode *, struct vnode *,
									struct componentname *);
int ext2fs_dirremove(struct vnode *, struct componentname *);
int ext2fs_dirrewrite(struct inode *, struct inode *,
								struct componentname *);
int ext2fs_dirempty(struct inode *, ino_t, struct ucred *);
int ext2fs_checkpath(struct inode *, struct inode *, struct ucred *);

/* ext2fs_subr.c */
int ext2fs_bufatoff(struct inode *, off_t, char **, struct buf **);
void ext2fs_fragacct(struct m_ext2fs *, int, int32_t[], int);
#ifdef DIAGNOSTIC
void	ext2fs_checkoverlap(struct buf *, struct inode *);
#endif

/* ext2fs_vfsops.c */
int ext2fs_mountroot(void);
int ext2fs_mount(struct mount *, const char *, void *,
		   struct nameidata *, struct proc *);
int ext2fs_reload(struct mount *, struct ucred *, struct proc *);
int ext2fs_mountfs(struct vnode *, struct mount *, struct proc *);
int ext2fs_unmount(struct mount *, int, struct proc *);
int ext2fs_flushfiles(struct mount *, int, struct proc *);
int ext2fs_statfs(struct mount *, struct statfs *, struct proc *);
int ext2fs_sync(struct mount *, int, struct ucred *, struct proc *);
int ext2fs_vget(struct mount *, ino_t, struct vnode **);
int ext2fs_fhtovp(struct mount *, struct fid *, struct vnode **);
int ext2fs_vptofh(struct vnode *, struct fid *);
int ext2fs_sbupdate(struct ufsmount *, int);
int ext2fs_cgupdate(struct ufsmount *, int);
int ext2fs_sysctl(int *, u_int, void *, size_t *, void *, size_t,
		       struct proc *);

/* ext2fs_readwrite.c */
int ext2fs_read(void *);
int ext2fs_write(void *);

/* ext2fs_vnops.c */
int ext2fs_create(void *);
int ext2fs_mknod(void *);
int ext2fs_open(void *);
int ext2fs_access(void *);
int ext2fs_getattr(void *);
int ext2fs_setattr(void *);
int ext2fs_remove(void *);
int ext2fs_link(void *);
int ext2fs_rename(void *);
int ext2fs_mkdir(void *);
int ext2fs_rmdir(void *);
int ext2fs_symlink(void *);
int ext2fs_readlink(void *);
int ext2fs_advlock(void *);
int ext2fs_vinit(struct mount *, int (**specops)(void *),
                 int (**fifoops)(void *), struct vnode **);
int ext2fs_makeinode(int, struct vnode *, struct vnode **,
                     struct componentname *cnp);
int ext2fs_fsync(void *);
int ext2fs_reclaim(void *);

__END_DECLS

#define IS_EXT2_VNODE(vp)   (vp->v_tag == VT_EXT2FS)

extern int (**ext2fs_vnodeop_p)(void *);
extern int (**ext2fs_specop_p)(void *);
#ifdef FIFO
extern int (**ext2fs_fifoop_p)(void *);
#define EXT2FS_FIFOOPS ext2fs_fifoop_p
#else
#define EXT2FS_FIFOOPS NULL
#endif
