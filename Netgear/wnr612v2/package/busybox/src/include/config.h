/*
 * Automatically generated header file: don't edit
 */

/* Version Number */
#define BB_VER "1.01"
#define BB_BT "2011.09.26-03:12+0000"

#define HAVE_DOT_CONFIG 1

/*
 * General Configuration
 */
#define CONFIG_FEATURE_BUFFERS_USE_MALLOC 1
#undef CONFIG_FEATURE_BUFFERS_GO_ON_STACK
#undef CONFIG_FEATURE_BUFFERS_GO_IN_BSS
#undef CONFIG_FEATURE_VERBOSE_USAGE
#undef CONFIG_FEATURE_INSTALLER
#undef CONFIG_LOCALE_SUPPORT
#undef CONFIG_FEATURE_DEVFS
#define CONFIG_FEATURE_DEVPTS 1
#undef CONFIG_FEATURE_CLEAN_UP
#define CONFIG_FEATURE_SUID 1
#undef CONFIG_FEATURE_SUID_CONFIG
#undef CONFIG_SELINUX

/*
 * Build Options
 */
#undef CONFIG_STATIC
#define CONFIG_LFS 1
#undef USING_CROSS_COMPILER
#define EXTRA_CFLAGS_OPTIONS ""

/*
 * Installation Options
 */
#undef CONFIG_INSTALL_NO_USR
#define PREFIX "./_install"

/*
 * Archival Utilities
 */
#undef CONFIG_AR
#undef CONFIG_BUNZIP2
#undef CONFIG_CPIO
#undef CONFIG_DPKG
#undef CONFIG_DPKG_DEB
#undef CONFIG_GUNZIP
#undef CONFIG_GZIP
#undef CONFIG_RPM2CPIO
#undef CONFIG_RPM
#define CONFIG_TAR 1
#define CONFIG_FEATURE_TAR_CREATE 1
#undef CONFIG_FEATURE_TAR_BZIP2
#undef CONFIG_FEATURE_TAR_FROM
#define CONFIG_FEATURE_TAR_GZIP 1
#undef CONFIG_FEATURE_TAR_COMPRESS
#undef CONFIG_FEATURE_TAR_OLDGNU_COMPATABILITY
#define CONFIG_FEATURE_TAR_GNU_EXTENSIONS 1
#undef CONFIG_FEATURE_TAR_LONG_OPTIONS
#undef CONFIG_UNCOMPRESS
#undef CONFIG_UNZIP

/*
 * Common options for cpio and tar
 */
#undef CONFIG_FEATURE_UNARCHIVE_TAPE

/*
 * Coreutils
 */
#undef CONFIG_BASENAME
#undef CONFIG_CAL
#define CONFIG_CAT 1
#undef CONFIG_CHGRP
#define CONFIG_CHMOD 1
#undef CONFIG_CHOWN
#undef CONFIG_CHROOT
#undef CONFIG_CMP
#define CONFIG_CP 1
#define CONFIG_CUT 1
#define CONFIG_DATE 1
#define CONFIG_FEATURE_DATE_ISOFMT 1
#define CONFIG_DD 1
#undef CONFIG_DF
#undef CONFIG_DIRNAME
#undef CONFIG_DOS2UNIX
#undef CONFIG_DU
#define CONFIG_ECHO 1
#define CONFIG_FEATURE_FANCY_ECHO 1
#define CONFIG_ENV 1
#undef CONFIG_EXPR
#undef CONFIG_FALSE
#undef CONFIG_FOLD
#undef CONFIG_HEAD
#undef CONFIG_HOSTID
#undef CONFIG_ID
#undef CONFIG_INSTALL
#undef CONFIG_LENGTH
#define CONFIG_LN 1
#undef CONFIG_LOGNAME
#define CONFIG_LS 1
#define CONFIG_FEATURE_LS_FILETYPES 1
#define CONFIG_FEATURE_LS_FOLLOWLINKS 1
#define CONFIG_FEATURE_LS_RECURSIVE 1
#define CONFIG_FEATURE_LS_SORTFILES 1
#define CONFIG_FEATURE_LS_TIMESTAMPS 1
#define CONFIG_FEATURE_LS_USERNAME 1
#define CONFIG_FEATURE_LS_COLOR 1
#define CONFIG_MD5SUM 1
#define CONFIG_MKDIR 1
#undef CONFIG_MKFIFO
#define CONFIG_MKNOD 1
#define CONFIG_MV 1
#undef CONFIG_OD
#define CONFIG_PRINTF 1
#undef CONFIG_PWD
#undef CONFIG_REALPATH
#define CONFIG_RM 1
#undef CONFIG_RMDIR
#undef CONFIG_SEQ
#undef CONFIG_SHA1SUM
#define CONFIG_SLEEP 1
#define CONFIG_FEATURE_FANCY_SLEEP 1
#define CONFIG_SORT 1
#undef CONFIG_STTY
#undef CONFIG_SYNC
#undef CONFIG_TAIL
#undef CONFIG_TEE
#define CONFIG_TEST 1

/*
 * test (forced enabled for use with shell)
 */
#undef CONFIG_FEATURE_TEST_64
#define CONFIG_TOUCH 1
#undef CONFIG_TR
#undef CONFIG_TRUE
#undef CONFIG_TTY
#undef CONFIG_UNAME
#undef CONFIG_UNIQ
#define CONFIG_USLEEP 1
#undef CONFIG_UUDECODE
#undef CONFIG_UUENCODE
#undef CONFIG_WATCH
#define CONFIG_WC 1
#undef CONFIG_WHO
#undef CONFIG_WHOAMI
#undef CONFIG_YES

/*
 * Common options for cp and mv
 */
#define CONFIG_FEATURE_PRESERVE_HARDLINKS 1

/*
 * Common options for ls and more
 */
#define CONFIG_FEATURE_AUTOWIDTH 1

/*
 * Common options for df, du, ls
 */
#define CONFIG_FEATURE_HUMAN_READABLE 1

/*
 * Common options for md5sum, sha1sum
 */
#undef CONFIG_FEATURE_MD5_SHA1_SUM_CHECK

/*
 * Console Utilities
 */
#undef CONFIG_CHVT
#undef CONFIG_CLEAR
#undef CONFIG_DEALLOCVT
#undef CONFIG_DUMPKMAP
#undef CONFIG_LOADFONT
#undef CONFIG_LOADKMAP
#undef CONFIG_OPENVT
#undef CONFIG_RESET
#undef CONFIG_SETKEYCODES

/*
 * Debian Utilities
 */
#define CONFIG_MKTEMP 1
#undef CONFIG_PIPE_PROGRESS
#undef CONFIG_READLINK
#undef CONFIG_RUN_PARTS
#undef CONFIG_START_STOP_DAEMON
#undef CONFIG_WHICH

/*
 * Editors
 */
#define CONFIG_AWK 1
#define CONFIG_FEATURE_AWK_MATH 1
#undef CONFIG_PATCH
#define CONFIG_SED 1
#undef CONFIG_VI

/*
 * Finding Utilities
 */
#define CONFIG_FIND 1
#define CONFIG_FEATURE_FIND_MTIME 1
#define CONFIG_FEATURE_FIND_PERM 1
#define CONFIG_FEATURE_FIND_TYPE 1
#define CONFIG_FEATURE_FIND_XDEV 1
#define CONFIG_FEATURE_FIND_NEWER 1
#define CONFIG_FEATURE_FIND_INUM 1
#define CONFIG_GREP 1
#define CONFIG_FEATURE_GREP_EGREP_ALIAS 1
#define CONFIG_FEATURE_GREP_FGREP_ALIAS 1
#define CONFIG_FEATURE_GREP_CONTEXT 1
#undef CONFIG_XARGS

/*
 * Init Utilities
 */
#define CONFIG_INIT 1
#define CONFIG_FEATURE_USE_INITTAB 1
#undef CONFIG_FEATURE_INITRD
#undef CONFIG_FEATURE_INIT_COREDUMPS
#define CONFIG_FEATURE_INIT_SWAPON 1
#define CONFIG_FEATURE_EXTRA_QUIET 1
#undef CONFIG_HALT
#undef CONFIG_POWEROFF
#define CONFIG_REBOOT 1
#undef CONFIG_MESG

/*
 * Login/Password Management Utilities
 */
#undef CONFIG_USE_BB_PWD_GRP
#undef CONFIG_ADDGROUP
#undef CONFIG_DELGROUP
#undef CONFIG_ADDUSER
#undef CONFIG_DELUSER
#undef CONFIG_GETTY
#undef CONFIG_LOGIN
#undef CONFIG_PASSWD
#undef CONFIG_SU
#undef CONFIG_SULOGIN
#undef CONFIG_VLOCK

/*
 * Miscellaneous Utilities
 */
#undef CONFIG_ADJTIMEX
#define CONFIG_CROND 1
#undef CONFIG_FEATURE_CROND_CALL_SENDMAIL
#undef CONFIG_CRONTAB
#undef CONFIG_DC
#undef CONFIG_DEVFSD
#undef CONFIG_LAST
#undef CONFIG_HDPARM
#undef CONFIG_MAKEDEVS
#undef CONFIG_MT
#undef CONFIG_RX
#undef CONFIG_STRINGS
#undef CONFIG_TIME
#undef CONFIG_WATCHDOG

/*
 * Linux Module Utilities
 */
#define CONFIG_INSMOD 1
#define CONFIG_FEATURE_2_4_MODULES 1
#define CONFIG_FEATURE_2_6_MODULES 1
#undef CONFIG_FEATURE_INSMOD_VERSION_CHECKING
#define CONFIG_FEATURE_INSMOD_KSYMOOPS_SYMBOLS 1
#undef CONFIG_FEATURE_INSMOD_LOADINKMEM
#define CONFIG_FEATURE_INSMOD_LOAD_MAP 1
#define CONFIG_FEATURE_INSMOD_LOAD_MAP_FULL 1
#define CONFIG_LSMOD 1
#undef CONFIG_MODPROBE
#define CONFIG_RMMOD 1
#undef CONFIG_FEATURE_CHECK_TAINTED_MODULE

/*
 * Networking Utilities
 */
#undef CONFIG_FEATURE_IPV6
#define CONFIG_ARPING 1
#undef CONFIG_FTPGET
#undef CONFIG_FTPPUT
#undef CONFIG_HOSTNAME
#undef CONFIG_HTTPD
#define CONFIG_IFCONFIG 1
#define CONFIG_FEATURE_IFCONFIG_STATUS 1
#undef CONFIG_FEATURE_IFCONFIG_SLIP
#undef CONFIG_FEATURE_IFCONFIG_MEMSTART_IOADDR_IRQ
#define CONFIG_FEATURE_IFCONFIG_HW 1
#define CONFIG_FEATURE_IFCONFIG_BROADCAST_PLUS 1
#undef CONFIG_IFUPDOWN
#undef CONFIG_INETD
#undef CONFIG_IP
#undef CONFIG_IPCALC
#undef CONFIG_IPADDR
#undef CONFIG_IPLINK
#undef CONFIG_IPROUTE
#undef CONFIG_IPTUNNEL
#undef CONFIG_NAMEIF
#undef CONFIG_NC
#undef CONFIG_NETSTAT
#undef CONFIG_NSLOOKUP
#define CONFIG_PING 1
#define CONFIG_PINGDET 1
#define CONFIG_FEATURE_FANCY_PING 1
#define CONFIG_ROUTE 1
#undef CONFIG_TELNET
#undef CONFIG_TELNETD
#define CONFIG_TFTP 1
#define CONFIG_FEATURE_TFTP_GET 1
#define CONFIG_FEATURE_TFTP_PUT 1
#undef CONFIG_FEATURE_TFTP_BLOCKSIZE
#undef CONFIG_FEATURE_TFTP_DEBUG
#define CONFIG_TRACEROUTE 1
#undef CONFIG_FEATURE_TRACEROUTE_VERBOSE
#define CONFIG_VCONFIG 1
#undef CONFIG_WGET

/*
 * udhcp Server/Client
 */
#define CONFIG_UDHCPD 1
#undef CONFIG_DHCPRELAY
#define CONFIG_UDHCPC 1
#define CONFIG_DUMPLEASES 1
#undef CONFIG_FEATURE_UDHCP_SYSLOG
#undef CONFIG_FEATURE_UDHCP_DEBUG

/*
 * Process Utilities
 */
#define CONFIG_FREE 1
#define CONFIG_KILL 1
#define CONFIG_KILLALL 1
#undef CONFIG_PIDOF
#define CONFIG_PS 1
#undef CONFIG_RENICE
#undef CONFIG_TOP
#undef CONFIG_UPTIME
#define CONFIG_SYSCTL 1

/*
 * Another Bourne-like Shell
 */
#define CONFIG_FEATURE_SH_IS_ASH 1
#undef CONFIG_FEATURE_SH_IS_HUSH
#undef CONFIG_FEATURE_SH_IS_LASH
#undef CONFIG_FEATURE_SH_IS_MSH
#undef CONFIG_FEATURE_SH_IS_NONE
#define CONFIG_ASH 1

/*
 * Ash Shell Options
 */
#define CONFIG_ASH_JOB_CONTROL 1
#define CONFIG_ASH_ALIAS 1
#define CONFIG_ASH_MATH_SUPPORT 1
#undef CONFIG_ASH_MATH_SUPPORT_64
#define CONFIG_ASH_GETOPTS 1
#define CONFIG_ASH_CMDCMD 1
#undef CONFIG_ASH_MAIL
#define CONFIG_ASH_OPTIMIZE_FOR_SIZE 1
#undef CONFIG_ASH_RANDOM_SUPPORT
#undef CONFIG_HUSH
#undef CONFIG_LASH
#undef CONFIG_MSH

/*
 * Bourne Shell Options
 */
#undef CONFIG_FEATURE_SH_EXTRA_QUIET
#undef CONFIG_FEATURE_SH_STANDALONE_SHELL
#define CONFIG_FEATURE_COMMAND_EDITING 1
#define CONFIG_FEATURE_COMMAND_HISTORY 15
#undef CONFIG_FEATURE_COMMAND_SAVEHISTORY
#define CONFIG_FEATURE_COMMAND_TAB_COMPLETION 1
#undef CONFIG_FEATURE_COMMAND_USERNAME_COMPLETION
#define CONFIG_FEATURE_SH_FANCY_PROMPT 1

/*
 * System Logging Utilities
 */
#define CONFIG_SYSLOGD 1
#define CONFIG_FEATURE_ROTATE_LOGFILE 1
#define CONFIG_FEATURE_REMOTE_LOG 1
#define CONFIG_FEATURE_IPC_SYSLOG 1
#define CONFIG_FEATURE_IPC_SYSLOG_BUFFER_SIZE 16
#undef CONFIG_LOGREAD
#define CONFIG_KLOGD 1
#define CONFIG_LOGGER 1

/*
 * Linux System Utilities
 */
#undef CONFIG_DMESG
#undef CONFIG_FBSET
#undef CONFIG_FDFLUSH
#undef CONFIG_FDFORMAT
#undef CONFIG_FDISK
#define FDISK_SUPPORT_LARGE_DISKS 1
#undef CONFIG_FREERAMDISK
#undef CONFIG_FSCK_MINIX
#undef CONFIG_MKFS_MINIX
#undef CONFIG_GETOPT
#undef CONFIG_HEXDUMP
#undef CONFIG_HWCLOCK
#undef CONFIG_LOSETUP
#undef CONFIG_MKSWAP
#undef CONFIG_MORE
#undef CONFIG_PIVOT_ROOT
#undef CONFIG_RDATE
#undef CONFIG_SWAPONOFF
#define CONFIG_MOUNT 1
#undef CONFIG_NFSMOUNT
#undef CONFIG_UMOUNT

/*
 * Common options for mount/umount
 */
#undef CONFIG_FEATURE_MOUNT_LOOP
#undef CONFIG_FEATURE_MTAB_SUPPORT

/*
 * User Utilities
 */
#undef CONFIG_DETCABLE
#undef CONFIG_MY_PRINT
#undef CONFIG_WANWD

/*
 * Debugging Options
 */
#undef CONFIG_DEBUG
