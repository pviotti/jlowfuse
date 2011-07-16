package jlowfuse;

import fuse.FuseReply;
import fuse.FuseExtra;
import fuse.Dirbuf;
import java.nio.ByteBuffer;

public class Reply extends FuseReply {
    private static native int jniReplyByteBuffer(long req, ByteBuffer buf,
                                                 long off, long maxsize);

    public static int byteBuffer(FuseReq req, ByteBuffer buf,
                                 long off, long maxsize) {
	    return jniReplyByteBuffer(req.getCPtr(), buf, off, maxsize);
    }

	public static int dirBufLimited(FuseReq req, Dirbuf dir, long off, long maxsize) {
		return FuseExtra.replyBufLimited(req, dir.getP(), dir.getSize(),
		                                 off, maxsize);
	}
	
}   