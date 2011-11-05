package jlowfuse.async.tasks;

import jlowfuse.FuseReq;
import jlowfuse.async.Context;

public class Removexattr<CTX extends Context> extends FilesystemOperation<CTX> {
	protected long ino;
	protected String name;
	
	public Removexattr(FuseReq req, long ino, String name) {
		super(req);
		this.ino = ino;
		this.name = name;
	}
	
	public String toString() {
		return new StringBuilder(super.toString())
			.append(" ino=")
			.append(ino)
			.append(" name=")
			.append(name)
			.toString();
	}
}
