package jlowfuse;

import fuse.*;

public class FuseReq extends SWIGTYPE_p_fuse_req_t {
    public FuseReq(long ptr) {
        super(ptr, false);
    }

    long getCPtr() { return FuseReq.getCPtr(this); }

    public String toString() {
        return "fuse_req_t: ptr=0x" + Long.toHexString(getCPtr(this));
    }

    public FuseContext getContext() {
        return FuseRequest.getContext(this);
    }
}                            
