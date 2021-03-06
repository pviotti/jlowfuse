/*
 * Copyright (c) 2011 Marcel Lauhoff.
 *
 * This file is part of jlowfuse.
 *
 * jlowfuse is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * jlowfuse is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with jlowfuse.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * jlowfuse.c - java FUSE lowlevel api wapper
 *
 * NOTE: this only works with a _single_ java thread.
 * No mapping of native to java threads
 *
 */


#define FUSE_USE_VERSION 26

#include "jlowfuse.h"
#include "jlowfuse_java_LowlevelOpsProxy.h"
#include "jlowfuse_java_BufferManager.h"

#include <jni.h>

#include <fuse_lowlevel.h>
#include <fuse_opt.h>

#include <sys/statvfs.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>

#include <err.h>

JavaVM *jvm;
jobject thread_group;  /* attached java threads */

/* cached class, object, methodids */
struct class_lowlevel_ops *cl_low_ops;
struct class_buffermanager *cl_bufman;

/* attach native thread to java vm */
JNIEnv *attach_native_thread()
{
	JNIEnv *env;
	JavaVMAttachArgs args;
        int res;

	args.version = JNI_VERSION_1_6;
	args.name = NULL;
	args.group = thread_group;

	res = (*jvm)->GetEnv(jvm, (void**) &env, args.version);

        if (res == JNI_EDETACHED) {
                (*jvm)->AttachCurrentThreadAsDaemon(jvm,
                                                    (void**) &env,
                                                    (void*) &args);
        } else if (res < JNI_OK) { /* should not happen */
                errx(16, "should not happen.. GetEnv result: %i", res);
        }

        return env;
}

/* detach native thread from java vm */
void detach_native_thread()
{
        int res = JNI_OK;
        res = (*jvm)->DetachCurrentThread(jvm);

        if (res != JNI_OK) {
//	        errx(16, "Failed to detach current thread: %i", res);
	}
}

void exception_check(JNIEnv *env)
{
        if ((*env)->ExceptionCheck(env)) {
                (*env)->ExceptionDescribe(env);
                (*env)->ExceptionClear(env);
        }
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *ljvm, void *reserved)
{
        JNIEnv *env;
        int ret;

        jvm = ljvm;

        ret = (*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_2);
        assert(ret != JNI_EVERSION);

        assert(sizeof(void *) <= sizeof(jlong));

        return JNI_VERSION_1_6;
}


struct fuse_lowlevel_ops jlowfuse_ops = {
          .init        = jlowfuse_init,
          .destroy     = jlowfuse_destroy,
          .lookup      = jlowfuse_lookup,
          .forget      = jlowfuse_forget,
          .getattr     = jlowfuse_getattr,
          .setattr     = jlowfuse_setattr,
          .readlink    = jlowfuse_readlink,
          .mknod       = jlowfuse_mknod,
          .mkdir       = jlowfuse_mkdir,
          .unlink      = jlowfuse_unlink,
          .rmdir       = jlowfuse_rmdir,
          .symlink     = jlowfuse_symlink,
          .rename      = jlowfuse_rename,
          .link        = jlowfuse_link,
          .open        = jlowfuse_open,
          .read        = jlowfuse_read,
          .write       = jlowfuse_write,
          .flush       = jlowfuse_flush,
          .release     = jlowfuse_release,
          .fsync       = jlowfuse_fsync,
          .opendir     = jlowfuse_opendir,
          .readdir     = jlowfuse_readdir,
          .releasedir  = jlowfuse_releasedir,
          .fsyncdir    = jlowfuse_fsyncdir,
          .statfs      = jlowfuse_statfs,
          .setxattr    = jlowfuse_setxattr,
          .getxattr    = jlowfuse_getxattr,
          .listxattr   = jlowfuse_listxattr,
          .removexattr = jlowfuse_removexattr,
          .access      = jlowfuse_access,
          .create      = jlowfuse_create,
          .getlk       = NULL,
          .setlk       = NULL,
          .bmap        = jlowfuse_bmap
          /* .ioctl       = NULL,*/
          /* .poll        = NULL, */
};

/* register JLowFuseProxy methods */
JNIEXPORT jlong JNICALL Java_jlowfuse_JLowFuse_setOps
(JNIEnv *env, jclass cls, jobject ops_obj, jobject tgroup)
{
        cl_low_ops = alloc_class_lowlevel_ops(env);
        populate_class_lowlevel_ops(env, cl_low_ops, ops_obj);

        thread_group = tgroup;

        return (jlong)&jlowfuse_ops;
}

/* parse fuse commandline from java string array */
JNIEXPORT jlong JNICALL Java_jlowfuse_JLowFuseArgs_makeFuseArgs
(JNIEnv *env, jclass cls, jobjectArray jstrarr)
{
        jsize len;
        jstring jstr;

        int i;
        struct fuse_args *args;
        const char *str;
        char *mount_point;
        int multi_threaded = -1;
        int foreground = -1;

        args = calloc(1, sizeof(struct fuse_args));

        len = (*env)->GetArrayLength(env, jstrarr);

        for(i=0; i<len; i++) {
                jstr = (*env)->GetObjectArrayElement(env, jstrarr, i);
                str = (*env)->GetStringUTFChars(env, jstr, NULL);
                assert(str != NULL);

                fuse_opt_add_arg(args, str);

                (*env)->ReleaseStringUTFChars(env, jstr, str);
                (*env)->DeleteLocalRef(env, jstr);

        }

        if (fuse_parse_cmdline(args, &mount_point,
                               &multi_threaded, &foreground) != 0) {
                errx(2, "Commandline error");
        }

        return (long)args;
}

/* register JLowFuseProxy methods */
JNIEXPORT void JNICALL Java_jlowfuse_JLowFuse_setBufferManager
(JNIEnv *env, jclass cls, jobject buffermanager_obj, jboolean for_read, jboolean for_write)
{
	cl_bufman = alloc_class_buffermanager(env);
        populate_class_buffermanager(env, cl_bufman, buffermanager_obj);

        cl_bufman->use_for_read = for_read;
        cl_bufman->use_for_write = for_write;
}

#define min(x, y) ((x) < (y) ? (x) : (y))

JNIEXPORT jint JNICALL Java_jlowfuse_Reply_jniReplyByteBuffer
(JNIEnv *env, jclass cls, jlong jreq, jobject jbuf, jlong joff, jlong jmaxsize)
{
        void *buf;
        jlong bufsize;
        fuse_req_t req;
        unsigned long off = joff;
        unsigned long maxsize = jmaxsize;
        jint ret;

        assert(off >= 0);

        bufsize = (*env)->GetDirectBufferCapacity(env, jbuf);
        if (bufsize <= 0)
                errx(16, "GetDirectbufferCapacity failed");

        buf = (*env)->GetDirectBufferAddress(env, jbuf);
        if (buf == NULL)
                errx(16, "GetDirectBufferAddress failed");

        req = (fuse_req_t)jreq;

        if (off < bufsize)
		ret = fuse_reply_buf(req, buf + off,
				      min(bufsize - off, maxsize));
	else
                ret = fuse_reply_buf(req, NULL, 0);

        if (use_buffermanager_for_read(cl_bufman))
	        buffermanager_free(env, cl_bufman, jbuf);

        return ret;
}
