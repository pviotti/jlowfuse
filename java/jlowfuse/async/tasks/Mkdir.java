package jlowfuse.async.tasks;

import jlowfuse.FuseReq;
import jlowfuse.async.Context;

public class Mkdir<CTX extends Context> extends FilesystemOperation<CTX> {
	protected long parent;
	protected String name;
	protected short mode;
	
	public Mkdir(FuseReq req, long parent, String name, short mode) {
		super(req);
		this.parent = parent;
		this.name = name;
		this.mode = mode;
	}
	
	public String toString() {
		return new StringBuilder(super.toString())
			.append(" parent=")
			.append(parent)
			.append(" name=")
			.append(name)
			.append(" mode=")
			.append(mode)	
			.toString();
	}
}
