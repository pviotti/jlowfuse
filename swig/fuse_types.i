/* Help SWIG with types */
typedef unsigned long int uint64_t;
typedef unsigned long fuse_ino_t;

typedef struct fuse_req * fuse_req_t;

// typedef unsigned long long int uint64_t;
// %apply unsigned long long int { uint64_t }
// %apply unsigned long long int { __fsblkcnt64_t }
// %apply unsigned long long int { __fsfilcnt64_t }
// %apply unsigned long long int { __off64_t }
// %apply unsigned long long int { __dev_t }
%apply unsigned long int { uint64_t }
%apply unsigned long int { __fsblkcnt64_t }
%apply unsigned long int { __fsblkcnt_t }
%apply unsigned long int { __fsfilcnt64_t }
%apply unsigned long { __off64_t }
%apply unsigned long { __off_t }
%apply unsigned long int { __dev_t }
%apply long int { __syscall_slong_t }

%apply unsigned int { __mode_t}
%apply unsigned int { mode_t }
%apply unsigned int { __uid_t }
%apply unsigned int { uid_t }
%apply unsigned int { __gid_t }
%apply unsigned int { gid_t }
%apply int { pid_t }
%apply unsigned long int { __nlink_t }
%apply long int { __blksize_t }
%apply long long int { __blkcnt64_t }

%apply unsigned long int { __ino_t }
%apply unsigned long int { __ino64_t }
%apply unsigned long { fuse_ino_t }
%apply unsigned long { off_t}

%apply long int { __time_t }



