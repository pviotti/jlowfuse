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

package jlowfuse.async.tasks;

import jlowfuse.FuseReq;
import jlowfuse.Reply;
import jlowfuse.async.Context;
import fuse.Errno;

public class FilesystemOperation<CTX extends Context> extends JLowFuseTask<CTX> {
	protected FuseReq req;
	
	public FilesystemOperation(FuseReq req) {
		this.req = req;
	}
	public void run() {
        Reply.err(req, Errno.ENOSYS);
	}
	public String toString() {
		return new StringBuilder(super.toString())
			.append(" [")
			.append(req.toString())
			.append("]")
			.toString();
	}
}
