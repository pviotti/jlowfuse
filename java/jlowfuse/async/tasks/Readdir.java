
package jlowfuse.async.tasks;

import fuse.FileInfo;
import jlowfuse.FuseReq;

public interface Readdir {
	public void attributes(FuseReq req, long ino, long size, long off, FileInfo fi);
}