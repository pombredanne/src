/*	$OpenBSD: netbsd_syscallargs.h,v 1.10.4.3 2001/10/31 03:11:46 nate Exp $	*/

/*
 * System call argument lists.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from;	OpenBSD: syscalls.master,v 1.15 2001/09/21 15:31:59 art Exp 
 */

#ifdef	syscallarg
#undef	syscallarg
#endif

#define	syscallarg(x)							\
	union {								\
		register_t pad;						\
		struct { x datum; } le;					\
		struct {						\
			int8_t pad[ (sizeof (register_t) < sizeof (x))	\
				? 0					\
				: sizeof (register_t) - sizeof (x)];	\
			x datum;					\
		} be;							\
	}

struct netbsd_sys_open_args {
	syscallarg(char *) path;
	syscallarg(int) flags;
	syscallarg(int) mode;
};

struct compat_43_netbsd_sys_creat_args {
	syscallarg(char *) path;
	syscallarg(int) mode;
};

struct netbsd_sys_link_args {
	syscallarg(char *) path;
	syscallarg(char *) link;
};

struct netbsd_sys_unlink_args {
	syscallarg(char *) path;
};

struct netbsd_sys_chdir_args {
	syscallarg(char *) path;
};

struct netbsd_sys_mknod_args {
	syscallarg(char *) path;
	syscallarg(int) mode;
	syscallarg(dev_t) dev;
};

struct netbsd_sys_chmod_args {
	syscallarg(char *) path;
	syscallarg(int) mode;
};

struct netbsd_sys_chown_args {
	syscallarg(char *) path;
	syscallarg(uid_t) uid;
	syscallarg(gid_t) gid;
};

struct netbsd_sys_unmount_args {
	syscallarg(char *) path;
	syscallarg(int) flags;
};

struct netbsd_sys_access_args {
	syscallarg(char *) path;
	syscallarg(int) flags;
};

struct netbsd_sys_chflags_args {
	syscallarg(char *) path;
	syscallarg(u_int) flags;
};

struct compat_43_netbsd_sys_stat_args {
	syscallarg(char *) path;
	syscallarg(struct ostat *) ub;
};

struct compat_43_netbsd_sys_lstat_args {
	syscallarg(char *) path;
	syscallarg(struct ostat *) ub;
};

struct netbsd_sys_revoke_args {
	syscallarg(char *) path;
};

struct netbsd_sys_symlink_args {
	syscallarg(char *) path;
	syscallarg(char *) link;
};

struct netbsd_sys_readlink_args {
	syscallarg(char *) path;
	syscallarg(char *) buf;
	syscallarg(size_t) count;
};

struct netbsd_sys_execve_args {
	syscallarg(char *) path;
	syscallarg(char **) argp;
	syscallarg(char **) envp;
};

struct netbsd_sys_chroot_args {
	syscallarg(char *) path;
};

struct netbsd_sys_rename_args {
	syscallarg(char *) from;
	syscallarg(char *) to;
};

struct compat_43_netbsd_sys_truncate_args {
	syscallarg(char *) path;
	syscallarg(long) length;
};

struct netbsd_sys_mkfifo_args {
	syscallarg(char *) path;
	syscallarg(int) mode;
};

struct netbsd_sys_mkdir_args {
	syscallarg(char *) path;
	syscallarg(int) mode;
};

struct netbsd_sys_rmdir_args {
	syscallarg(char *) path;
};

struct netbsd_sys_stat_args {
	syscallarg(char *) path;
	syscallarg(struct stat *) ub;
};

struct netbsd_sys_lstat_args {
	syscallarg(char *) path;
	syscallarg(struct stat *) ub;
};

struct netbsd_sys_pathconf_args {
	syscallarg(char *) path;
	syscallarg(int) name;
};

struct netbsd_sys_truncate_args {
	syscallarg(char *) path;
	syscallarg(int) pad;
	syscallarg(off_t) length;
};

struct netbsd_sys_fdatasync_args {
	syscallarg(int) fd;
};

struct netbsd_sys_getdents_args {
	syscallarg(int) fd;
	syscallarg(char *) buf;
	syscallarg(size_t) count;
};

struct netbsd_sys_lchmod_args {
	syscallarg(const char *) path;
	syscallarg(netbsd_mode_t) mode;
};

struct netbsd_sys_lutimes_args {
	syscallarg(const char *) path;
	syscallarg(const struct timeval *) tptr;
};

struct netbsd_sys___stat13_args {
	syscallarg(char *) path;
	syscallarg(struct netbsd_stat *) ub;
};

struct netbsd_sys___fstat13_args {
	syscallarg(int) fd;
	syscallarg(struct netbsd_stat *) ub;
};

struct netbsd_sys___lstat13_args {
	syscallarg(char *) path;
	syscallarg(struct netbsd_stat *) ub;
};

struct netbsd_sys___sigaltstack14_args {
	syscallarg(const struct netbsd_sigaltstack *) nss;
	syscallarg(struct netbsd_sigaltstack *) oss;
};

struct netbsd_sys___sigaction14_args {
	syscallarg(int) signum;
	syscallarg(const struct netbsd_sigaction *) nsa;
	syscallarg(struct netbsd_sigaction *) osa;
};

struct netbsd_sys___sigpending14_args {
	syscallarg(netbsd_sigset_t *) set;
};

struct netbsd_sys___sigprocmask14_args {
	syscallarg(int) how;
	syscallarg(const netbsd_sigset_t *) set;
	syscallarg(netbsd_sigset_t *) oset;
};

struct netbsd_sys___sigsuspend14_args {
	syscallarg(const netbsd_sigset_t *) set;
};

struct netbsd_sys___sigreturn14_args {
	syscallarg(struct netbsd_sigcontext *) sigcntxp;
};

struct netbsd_sys___getcwd_args {
	syscallarg(char *) bufp;
	syscallarg(size_t) length;
};

/*
 * System call prototypes.
 */

int	sys_exit	__P((struct proc *, void *, register_t *));
int	sys_fork	__P((struct proc *, void *, register_t *));
int	sys_read	__P((struct proc *, void *, register_t *));
int	sys_write	__P((struct proc *, void *, register_t *));
int	netbsd_sys_open	__P((struct proc *, void *, register_t *));
int	sys_close	__P((struct proc *, void *, register_t *));
int	sys_wait4	__P((struct proc *, void *, register_t *));
int	compat_43_netbsd_sys_creat	__P((struct proc *, void *, register_t *));
int	netbsd_sys_link	__P((struct proc *, void *, register_t *));
int	netbsd_sys_unlink	__P((struct proc *, void *, register_t *));
int	netbsd_sys_chdir	__P((struct proc *, void *, register_t *));
int	sys_fchdir	__P((struct proc *, void *, register_t *));
int	netbsd_sys_mknod	__P((struct proc *, void *, register_t *));
int	netbsd_sys_chmod	__P((struct proc *, void *, register_t *));
int	netbsd_sys_chown	__P((struct proc *, void *, register_t *));
int	sys_obreak	__P((struct proc *, void *, register_t *));
int	compat_25_sys_getfsstat	__P((struct proc *, void *, register_t *));
int	sys_lseek	__P((struct proc *, void *, register_t *));
int	sys_getpid	__P((struct proc *, void *, register_t *));
int	sys_mount	__P((struct proc *, void *, register_t *));
int	netbsd_sys_unmount	__P((struct proc *, void *, register_t *));
int	sys_setuid	__P((struct proc *, void *, register_t *));
int	sys_getuid	__P((struct proc *, void *, register_t *));
int	sys_geteuid	__P((struct proc *, void *, register_t *));
int	sys_ptrace	__P((struct proc *, void *, register_t *));
int	sys_recvmsg	__P((struct proc *, void *, register_t *));
int	sys_sendmsg	__P((struct proc *, void *, register_t *));
int	sys_recvfrom	__P((struct proc *, void *, register_t *));
int	sys_accept	__P((struct proc *, void *, register_t *));
int	sys_getpeername	__P((struct proc *, void *, register_t *));
int	sys_getsockname	__P((struct proc *, void *, register_t *));
int	netbsd_sys_access	__P((struct proc *, void *, register_t *));
int	netbsd_sys_chflags	__P((struct proc *, void *, register_t *));
int	sys_fchflags	__P((struct proc *, void *, register_t *));
int	sys_sync	__P((struct proc *, void *, register_t *));
int	sys_kill	__P((struct proc *, void *, register_t *));
int	compat_43_netbsd_sys_stat	__P((struct proc *, void *, register_t *));
int	sys_getppid	__P((struct proc *, void *, register_t *));
int	compat_43_netbsd_sys_lstat	__P((struct proc *, void *, register_t *));
int	sys_dup	__P((struct proc *, void *, register_t *));
int	sys_opipe	__P((struct proc *, void *, register_t *));
int	sys_getegid	__P((struct proc *, void *, register_t *));
int	sys_profil	__P((struct proc *, void *, register_t *));
#ifdef KTRACE
int	sys_ktrace	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_sigaction	__P((struct proc *, void *, register_t *));
int	sys_getgid	__P((struct proc *, void *, register_t *));
int	sys_sigprocmask	__P((struct proc *, void *, register_t *));
int	sys_getlogin	__P((struct proc *, void *, register_t *));
int	sys_setlogin	__P((struct proc *, void *, register_t *));
int	sys_acct	__P((struct proc *, void *, register_t *));
int	sys_sigpending	__P((struct proc *, void *, register_t *));
int	sys_sigaltstack	__P((struct proc *, void *, register_t *));
int	sys_ioctl	__P((struct proc *, void *, register_t *));
int	sys_reboot	__P((struct proc *, void *, register_t *));
int	netbsd_sys_revoke	__P((struct proc *, void *, register_t *));
int	netbsd_sys_symlink	__P((struct proc *, void *, register_t *));
int	netbsd_sys_readlink	__P((struct proc *, void *, register_t *));
int	netbsd_sys_execve	__P((struct proc *, void *, register_t *));
int	sys_umask	__P((struct proc *, void *, register_t *));
int	netbsd_sys_chroot	__P((struct proc *, void *, register_t *));
int	sys_fstat	__P((struct proc *, void *, register_t *));
int	compat_43_sys_getkerninfo	__P((struct proc *, void *, register_t *));
int	compat_43_sys_getpagesize	__P((struct proc *, void *, register_t *));
int	sys_omsync	__P((struct proc *, void *, register_t *));
int	sys_vfork	__P((struct proc *, void *, register_t *));
int	sys_sbrk	__P((struct proc *, void *, register_t *));
int	sys_sstk	__P((struct proc *, void *, register_t *));
int	sys_mmap	__P((struct proc *, void *, register_t *));
int	sys_ovadvise	__P((struct proc *, void *, register_t *));
int	sys_munmap	__P((struct proc *, void *, register_t *));
int	sys_mprotect	__P((struct proc *, void *, register_t *));
int	sys_madvise	__P((struct proc *, void *, register_t *));
int	sys_mincore	__P((struct proc *, void *, register_t *));
int	sys_getgroups	__P((struct proc *, void *, register_t *));
int	sys_setgroups	__P((struct proc *, void *, register_t *));
int	sys_getpgrp	__P((struct proc *, void *, register_t *));
int	sys_setpgid	__P((struct proc *, void *, register_t *));
int	sys_setitimer	__P((struct proc *, void *, register_t *));
int	compat_43_sys_wait	__P((struct proc *, void *, register_t *));
int	sys_swapon	__P((struct proc *, void *, register_t *));
int	sys_getitimer	__P((struct proc *, void *, register_t *));
int	compat_43_sys_gethostname	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sethostname	__P((struct proc *, void *, register_t *));
int	compat_43_sys_getdtablesize	__P((struct proc *, void *, register_t *));
int	sys_dup2	__P((struct proc *, void *, register_t *));
int	sys_fcntl	__P((struct proc *, void *, register_t *));
int	sys_select	__P((struct proc *, void *, register_t *));
int	sys_fsync	__P((struct proc *, void *, register_t *));
int	sys_setpriority	__P((struct proc *, void *, register_t *));
int	sys_socket	__P((struct proc *, void *, register_t *));
int	sys_connect	__P((struct proc *, void *, register_t *));
int	sys_accept	__P((struct proc *, void *, register_t *));
int	sys_getpriority	__P((struct proc *, void *, register_t *));
int	compat_43_sys_send	__P((struct proc *, void *, register_t *));
int	compat_43_sys_recv	__P((struct proc *, void *, register_t *));
int	sys_sigreturn	__P((struct proc *, void *, register_t *));
int	sys_bind	__P((struct proc *, void *, register_t *));
int	sys_setsockopt	__P((struct proc *, void *, register_t *));
int	sys_listen	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sigvec	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sigblock	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sigsetmask	__P((struct proc *, void *, register_t *));
int	sys_sigsuspend	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sigstack	__P((struct proc *, void *, register_t *));
#ifdef MSG_COMPAT
int	compat_43_sys_recvmsg	__P((struct proc *, void *, register_t *));
#else
#endif
#ifdef MSG_COMPAT
int	compat_43_sys_sendmsg	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_gettimeofday	__P((struct proc *, void *, register_t *));
int	sys_getrusage	__P((struct proc *, void *, register_t *));
int	sys_getsockopt	__P((struct proc *, void *, register_t *));
int	sys_readv	__P((struct proc *, void *, register_t *));
int	sys_writev	__P((struct proc *, void *, register_t *));
int	sys_settimeofday	__P((struct proc *, void *, register_t *));
int	sys_fchown	__P((struct proc *, void *, register_t *));
int	sys_fchmod	__P((struct proc *, void *, register_t *));
#ifdef MSG_COMPAT
int	compat_43_sys_recvfrom	__P((struct proc *, void *, register_t *));
#else
#endif
int	compat_43_sys_setreuid	__P((struct proc *, void *, register_t *));
int	compat_43_sys_setregid	__P((struct proc *, void *, register_t *));
int	netbsd_sys_rename	__P((struct proc *, void *, register_t *));
int	compat_43_netbsd_sys_truncate	__P((struct proc *, void *, register_t *));
int	compat_43_sys_ftruncate	__P((struct proc *, void *, register_t *));
int	sys_flock	__P((struct proc *, void *, register_t *));
int	netbsd_sys_mkfifo	__P((struct proc *, void *, register_t *));
int	sys_sendto	__P((struct proc *, void *, register_t *));
int	sys_shutdown	__P((struct proc *, void *, register_t *));
int	sys_socketpair	__P((struct proc *, void *, register_t *));
int	netbsd_sys_mkdir	__P((struct proc *, void *, register_t *));
int	netbsd_sys_rmdir	__P((struct proc *, void *, register_t *));
int	sys_utimes	__P((struct proc *, void *, register_t *));
int	sys_adjtime	__P((struct proc *, void *, register_t *));
int	compat_43_sys_getpeername	__P((struct proc *, void *, register_t *));
int	compat_43_sys_gethostid	__P((struct proc *, void *, register_t *));
int	compat_43_sys_sethostid	__P((struct proc *, void *, register_t *));
int	compat_43_sys_getrlimit	__P((struct proc *, void *, register_t *));
int	compat_43_sys_setrlimit	__P((struct proc *, void *, register_t *));
int	compat_43_sys_killpg	__P((struct proc *, void *, register_t *));
int	sys_setsid	__P((struct proc *, void *, register_t *));
int	sys_quotactl	__P((struct proc *, void *, register_t *));
int	compat_43_sys_quota	__P((struct proc *, void *, register_t *));
int	compat_43_sys_getsockname	__P((struct proc *, void *, register_t *));
#if defined(NFSCLIENT) || defined(NFSSERVER)
int	sys_nfssvc	__P((struct proc *, void *, register_t *));
#else
#endif
int	compat_43_sys_getdirentries	__P((struct proc *, void *, register_t *));
int	compat_25_sys_statfs	__P((struct proc *, void *, register_t *));
int	compat_25_sys_fstatfs	__P((struct proc *, void *, register_t *));
#if defined(NFSCLIENT) || defined(NFSSERVER)
int	sys_getfh	__P((struct proc *, void *, register_t *));
#else
#endif
int	compat_09_sys_getdomainname	__P((struct proc *, void *, register_t *));
int	compat_09_sys_setdomainname	__P((struct proc *, void *, register_t *));
int	compat_09_sys_uname	__P((struct proc *, void *, register_t *));
int	sys_sysarch	__P((struct proc *, void *, register_t *));
#if defined(SYSVSEM) && !defined(alpha)
int	compat_10_sys_semsys	__P((struct proc *, void *, register_t *));
#else
#endif
#if defined(SYSVMSG) && !defined(alpha)
int	compat_10_sys_msgsys	__P((struct proc *, void *, register_t *));
#else
#endif
#if defined(SYSVSHM) && !defined(alpha)
int	compat_10_sys_shmsys	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_pread	__P((struct proc *, void *, register_t *));
int	sys_pwrite	__P((struct proc *, void *, register_t *));
#ifdef NTP
int	sys_ntp_gettime	__P((struct proc *, void *, register_t *));
int	sys_ntp_adjtime	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_setgid	__P((struct proc *, void *, register_t *));
int	sys_setegid	__P((struct proc *, void *, register_t *));
int	sys_seteuid	__P((struct proc *, void *, register_t *));
#ifdef LFS
int	lfs_bmapv	__P((struct proc *, void *, register_t *));
int	lfs_markv	__P((struct proc *, void *, register_t *));
int	lfs_segclean	__P((struct proc *, void *, register_t *));
int	lfs_segwait	__P((struct proc *, void *, register_t *));
#else
#endif
int	netbsd_sys_stat	__P((struct proc *, void *, register_t *));
int	sys_fstat	__P((struct proc *, void *, register_t *));
int	netbsd_sys_lstat	__P((struct proc *, void *, register_t *));
int	netbsd_sys_pathconf	__P((struct proc *, void *, register_t *));
int	sys_fpathconf	__P((struct proc *, void *, register_t *));
int	sys_swapctl	__P((struct proc *, void *, register_t *));
int	sys_getrlimit	__P((struct proc *, void *, register_t *));
int	sys_setrlimit	__P((struct proc *, void *, register_t *));
int	sys_getdirentries	__P((struct proc *, void *, register_t *));
int	sys_mmap	__P((struct proc *, void *, register_t *));
int	sys_lseek	__P((struct proc *, void *, register_t *));
int	netbsd_sys_truncate	__P((struct proc *, void *, register_t *));
int	sys_ftruncate	__P((struct proc *, void *, register_t *));
int	sys___sysctl	__P((struct proc *, void *, register_t *));
int	sys_mlock	__P((struct proc *, void *, register_t *));
int	sys_munlock	__P((struct proc *, void *, register_t *));
int	sys_undelete	__P((struct proc *, void *, register_t *));
int	sys_futimes	__P((struct proc *, void *, register_t *));
int	sys_getpgid	__P((struct proc *, void *, register_t *));
int	sys_xfspioctl	__P((struct proc *, void *, register_t *));
int	sys_poll	__P((struct proc *, void *, register_t *));
#ifdef LKM
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
int	sys_lkmnosys	__P((struct proc *, void *, register_t *));
#else	/* !LKM */
#endif	/* !LKM */
#ifdef SYSVSEM
int	compat_23_sys___semctl	__P((struct proc *, void *, register_t *));
int	sys_semget	__P((struct proc *, void *, register_t *));
int	sys_semop	__P((struct proc *, void *, register_t *));
#else
#endif
#ifdef SYSVMSG
int	compat_23_sys_msgctl	__P((struct proc *, void *, register_t *));
int	sys_msgget	__P((struct proc *, void *, register_t *));
int	sys_msgsnd	__P((struct proc *, void *, register_t *));
int	sys_msgrcv	__P((struct proc *, void *, register_t *));
#else
#endif
#ifdef SYSVSHM
int	sys_shmat	__P((struct proc *, void *, register_t *));
int	compat_23_sys_shmctl	__P((struct proc *, void *, register_t *));
int	sys_shmdt	__P((struct proc *, void *, register_t *));
int	sys_shmget	__P((struct proc *, void *, register_t *));
#else
#endif
int	sys_clock_gettime	__P((struct proc *, void *, register_t *));
int	sys_clock_settime	__P((struct proc *, void *, register_t *));
int	sys_clock_getres	__P((struct proc *, void *, register_t *));
int	sys_nanosleep	__P((struct proc *, void *, register_t *));
int	netbsd_sys_fdatasync	__P((struct proc *, void *, register_t *));
int	netbsd_sys_getdents	__P((struct proc *, void *, register_t *));
int	sys_minherit	__P((struct proc *, void *, register_t *));
int	netbsd_sys_lchmod	__P((struct proc *, void *, register_t *));
int	sys_lchown	__P((struct proc *, void *, register_t *));
int	netbsd_sys_lutimes	__P((struct proc *, void *, register_t *));
int	sys_msync	__P((struct proc *, void *, register_t *));
int	netbsd_sys___stat13	__P((struct proc *, void *, register_t *));
int	netbsd_sys___fstat13	__P((struct proc *, void *, register_t *));
int	netbsd_sys___lstat13	__P((struct proc *, void *, register_t *));
int	netbsd_sys___sigaltstack14	__P((struct proc *, void *, register_t *));
int	netbsd_sys___vfork14	__P((struct proc *, void *, register_t *));
int	sys_getsid	__P((struct proc *, void *, register_t *));
#ifdef KTRACE
#else
#endif
int	sys_preadv	__P((struct proc *, void *, register_t *));
int	sys_pwritev	__P((struct proc *, void *, register_t *));
int	netbsd_sys___sigaction14	__P((struct proc *, void *, register_t *));
int	netbsd_sys___sigpending14	__P((struct proc *, void *, register_t *));
int	netbsd_sys___sigprocmask14	__P((struct proc *, void *, register_t *));
int	netbsd_sys___sigsuspend14	__P((struct proc *, void *, register_t *));
int	netbsd_sys___sigreturn14	__P((struct proc *, void *, register_t *));
int	netbsd_sys___getcwd	__P((struct proc *, void *, register_t *));
int	sys_issetugid	__P((struct proc *, void *, register_t *));
