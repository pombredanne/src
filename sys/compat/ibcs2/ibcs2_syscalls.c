/*	$OpenBSD: ibcs2_syscalls.c,v 1.6.4.1 2001/07/04 10:39:20 niklas Exp $	*/

/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	OpenBSD: syscalls.master,v 1.7 1999/06/07 07:17:47 deraadt Exp 
 */

char *ibcs2_syscallnames[] = {
	"syscall",			/* 0 = syscall */
	"exit",			/* 1 = exit */
	"fork",			/* 2 = fork */
	"read",			/* 3 = read */
	"write",			/* 4 = write */
	"open",			/* 5 = open */
	"close",			/* 6 = close */
	"waitsys",			/* 7 = waitsys */
	"creat",			/* 8 = creat */
	"link",			/* 9 = link */
	"unlink",			/* 10 = unlink */
	"execv",			/* 11 = execv */
	"chdir",			/* 12 = chdir */
	"time",			/* 13 = time */
	"mknod",			/* 14 = mknod */
	"chmod",			/* 15 = chmod */
	"chown",			/* 16 = chown */
	"obreak",			/* 17 = obreak */
	"stat",			/* 18 = stat */
	"lseek",			/* 19 = lseek */
	"getpid",			/* 20 = getpid */
	"mount",			/* 21 = mount */
	"umount",			/* 22 = umount */
	"setuid",			/* 23 = setuid */
	"getuid",			/* 24 = getuid */
	"stime",			/* 25 = stime */
	"#26 (unimplemented ibcs2_ptrace)",		/* 26 = unimplemented ibcs2_ptrace */
	"alarm",			/* 27 = alarm */
	"fstat",			/* 28 = fstat */
	"pause",			/* 29 = pause */
	"utime",			/* 30 = utime */
	"#31 (unimplemented was stty)",		/* 31 = unimplemented was stty */
	"#32 (unimplemented was gtty)",		/* 32 = unimplemented was gtty */
	"access",			/* 33 = access */
	"nice",			/* 34 = nice */
	"statfs",			/* 35 = statfs */
	"sync",			/* 36 = sync */
	"kill",			/* 37 = kill */
	"fstatfs",			/* 38 = fstatfs */
	"pgrpsys",			/* 39 = pgrpsys */
	"#40 (unimplemented ibcs2_xenix)",		/* 40 = unimplemented ibcs2_xenix */
	"dup",			/* 41 = dup */
	"opipe",			/* 42 = opipe */
	"times",			/* 43 = times */
	"#44 (unimplemented profil)",		/* 44 = unimplemented profil */
	"plock",			/* 45 = plock */
	"setgid",			/* 46 = setgid */
	"getgid",			/* 47 = getgid */
	"sigsys",			/* 48 = sigsys */
#ifdef SYSVMSG
	"msgsys",			/* 49 = msgsys */
#else
	"#49 (unimplemented msgsys)",		/* 49 = unimplemented msgsys */
#endif
	"sysi86",			/* 50 = sysi86 */
	"#51 (unimplemented ibcs2_acct)",		/* 51 = unimplemented ibcs2_acct */
#ifdef SYSVSHM
	"shmsys",			/* 52 = shmsys */
#else
	"#52 (unimplemented shmsys)",		/* 52 = unimplemented shmsys */
#endif
#ifdef SYSVSEM
	"semsys",			/* 53 = semsys */
#else
	"#53 (unimplemented semsys)",		/* 53 = unimplemented semsys */
#endif
	"ioctl",			/* 54 = ioctl */
	"uadmin",			/* 55 = uadmin */
	"#56 (unimplemented)",		/* 56 = unimplemented */
	"utssys",			/* 57 = utssys */
	"#58 (unimplemented)",		/* 58 = unimplemented */
	"execve",			/* 59 = execve */
	"umask",			/* 60 = umask */
	"chroot",			/* 61 = chroot */
	"fcntl",			/* 62 = fcntl */
	"ulimit",			/* 63 = ulimit */
	"#64 (unimplemented reserved for unix/pc)",		/* 64 = unimplemented reserved for unix/pc */
	"#65 (unimplemented reserved for unix/pc)",		/* 65 = unimplemented reserved for unix/pc */
	"#66 (unimplemented reserved for unix/pc)",		/* 66 = unimplemented reserved for unix/pc */
	"#67 (unimplemented reserved for unix/pc)",		/* 67 = unimplemented reserved for unix/pc */
	"#68 (unimplemented reserved for unix/pc)",		/* 68 = unimplemented reserved for unix/pc */
	"#69 (unimplemented reserved for unix/pc)",		/* 69 = unimplemented reserved for unix/pc */
	"#70 (obsolete rfs_advfs)",		/* 70 = obsolete rfs_advfs */
	"#71 (obsolete rfs_unadvfs)",		/* 71 = obsolete rfs_unadvfs */
	"#72 (obsolete rfs_rmount)",		/* 72 = obsolete rfs_rmount */
	"#73 (obsolete rfs_rumount)",		/* 73 = obsolete rfs_rumount */
	"#74 (obsolete rfs_rfstart)",		/* 74 = obsolete rfs_rfstart */
	"#75 (obsolete rfs_sigret)",		/* 75 = obsolete rfs_sigret */
	"#76 (obsolete rfs_rdebug)",		/* 76 = obsolete rfs_rdebug */
	"#77 (obsolete rfs_rfstop)",		/* 77 = obsolete rfs_rfstop */
	"#78 (unimplemented rfs_rfsys)",		/* 78 = unimplemented rfs_rfsys */
	"rmdir",			/* 79 = rmdir */
	"mkdir",			/* 80 = mkdir */
	"getdents",			/* 81 = getdents */
	"#82 (unimplemented)",		/* 82 = unimplemented */
	"#83 (unimplemented)",		/* 83 = unimplemented */
	"sysfs",			/* 84 = sysfs */
	"getmsg",			/* 85 = getmsg */
	"putmsg",			/* 86 = putmsg */
	"poll",			/* 87 = poll */
	"#88 (unimplemented)",		/* 88 = unimplemented */
	"#89 (unimplemented)",		/* 89 = unimplemented */
	"symlink",			/* 90 = symlink */
	"lstat",			/* 91 = lstat */
	"readlink",			/* 92 = readlink */
	"#93 (unimplemented)",		/* 93 = unimplemented */
	"#94 (unimplemented)",		/* 94 = unimplemented */
	"#95 (unimplemented)",		/* 95 = unimplemented */
	"#96 (unimplemented)",		/* 96 = unimplemented */
	"#97 (unimplemented)",		/* 97 = unimplemented */
	"#98 (unimplemented)",		/* 98 = unimplemented */
	"#99 (unimplemented)",		/* 99 = unimplemented */
	"#100 (unimplemented)",		/* 100 = unimplemented */
	"#101 (unimplemented)",		/* 101 = unimplemented */
	"#102 (unimplemented)",		/* 102 = unimplemented */
	"sigreturn",			/* 103 = sigreturn */
	"#104 (unimplemented)",		/* 104 = unimplemented */
	"#105 (unimplemented)",		/* 105 = unimplemented */
	"#106 (unimplemented)",		/* 106 = unimplemented */
	"#107 (unimplemented)",		/* 107 = unimplemented */
	"#108 (unimplemented)",		/* 108 = unimplemented */
	"#109 (unimplemented)",		/* 109 = unimplemented */
	"#110 (unimplemented)",		/* 110 = unimplemented */
	"#111 (unimplemented)",		/* 111 = unimplemented */
	"#112 (unimplemented)",		/* 112 = unimplemented */
	"#113 (unimplemented)",		/* 113 = unimplemented */
	"#114 (unimplemented)",		/* 114 = unimplemented */
	"#115 (unimplemented)",		/* 115 = unimplemented */
	"#116 (unimplemented)",		/* 116 = unimplemented */
	"#117 (unimplemented)",		/* 117 = unimplemented */
	"#118 (unimplemented)",		/* 118 = unimplemented */
	"#119 (unimplemented)",		/* 119 = unimplemented */
	"#120 (unimplemented)",		/* 120 = unimplemented */
	"#121 (unimplemented)",		/* 121 = unimplemented */
	"#122 (unimplemented)",		/* 122 = unimplemented */
	"#123 (unimplemented)",		/* 123 = unimplemented */
	"#124 (unimplemented)",		/* 124 = unimplemented */
	"#125 (unimplemented)",		/* 125 = unimplemented */
	"#126 (unimplemented)",		/* 126 = unimplemented */
	"#127 (unimplemented)",		/* 127 = unimplemented */
	"#128 (unimplemented)",		/* 128 = unimplemented */
	"#129 (unimplemented xenix_xlocking)",		/* 129 = unimplemented xenix_xlocking */
	"#130 (unimplemented xenix_creatsem)",		/* 130 = unimplemented xenix_creatsem */
	"#131 (unimplemented xenix_opensem)",		/* 131 = unimplemented xenix_opensem */
	"#132 (unimplemented xenix_sigsem)",		/* 132 = unimplemented xenix_sigsem */
	"#133 (unimplemented xenix_waitsem)",		/* 133 = unimplemented xenix_waitsem */
	"#134 (unimplemented xenix_nbwaitsem)",		/* 134 = unimplemented xenix_nbwaitsem */
	"rdchk",			/* 135 = rdchk */
	"#136 (unimplemented)",		/* 136 = unimplemented */
	"#137 (unimplemented)",		/* 137 = unimplemented */
	"chsize",			/* 138 = chsize */
	"ftime",			/* 139 = ftime */
	"nap",			/* 140 = nap */
	"#141 (unimplemented xenix_sdget)",		/* 141 = unimplemented xenix_sdget */
	"#142 (unimplemented xenix_sdfree)",		/* 142 = unimplemented xenix_sdfree */
	"#143 (unimplemented xenix_sdenter)",		/* 143 = unimplemented xenix_sdenter */
	"#144 (unimplemented xenix_sdleave)",		/* 144 = unimplemented xenix_sdleave */
	"#145 (unimplemented xenix_sdgetv)",		/* 145 = unimplemented xenix_sdgetv */
	"#146 (unimplemented xenix_sdwaitv)",		/* 146 = unimplemented xenix_sdwaitv */
	"#147 (unimplemented)",		/* 147 = unimplemented */
	"#148 (unimplemented)",		/* 148 = unimplemented */
	"#149 (unimplemented)",		/* 149 = unimplemented */
	"#150 (unimplemented)",		/* 150 = unimplemented */
	"#151 (unimplemented)",		/* 151 = unimplemented */
	"#152 (unimplemented)",		/* 152 = unimplemented */
	"#153 (unimplemented)",		/* 153 = unimplemented */
	"#154 (unimplemented)",		/* 154 = unimplemented */
	"#155 (unimplemented)",		/* 155 = unimplemented */
	"#156 (unimplemented)",		/* 156 = unimplemented */
	"#157 (unimplemented)",		/* 157 = unimplemented */
	"#158 (unimplemented)",		/* 158 = unimplemented */
	"#159 (unimplemented)",		/* 159 = unimplemented */
	"#160 (unimplemented xenix_proctl)",		/* 160 = unimplemented xenix_proctl */
	"#161 (unimplemented xenix_execseg)",		/* 161 = unimplemented xenix_execseg */
	"#162 (unimplemented xenix_unexecseg)",		/* 162 = unimplemented xenix_unexecseg */
	"#163 (unimplemented)",		/* 163 = unimplemented */
	"select",			/* 164 = select */
	"eaccess",			/* 165 = eaccess */
	"#166 (unimplemented xenix_paccess)",		/* 166 = unimplemented xenix_paccess */
	"sigaction",			/* 167 = sigaction */
	"sigprocmask",			/* 168 = sigprocmask */
	"sigpending",			/* 169 = sigpending */
	"sigsuspend",			/* 170 = sigsuspend */
	"getgroups",			/* 171 = getgroups */
	"setgroups",			/* 172 = setgroups */
	"sysconf",			/* 173 = sysconf */
	"pathconf",			/* 174 = pathconf */
	"fpathconf",			/* 175 = fpathconf */
	"rename",			/* 176 = rename */
};
