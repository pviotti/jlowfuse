
package jlowfuse.async.tasks;

import fuse.FileInfo;
import jlowfuse.FuseReq;

public interface Release {
	public void setReleaseAttributes(FuseReq req, long ino, FileInfo fi);
}
