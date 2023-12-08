// Datei: UniqueId.java
package de.grimmer.markus;

import java.io.*;

public class UniqueId
{
	private String file;

	public UniqueId(String file)
	{
		this.file = file;
	}

	public void init(int id) throws IOException
	{
		DataOutputStream out = new DataOutputStream(new FileOutputStream(file));
		out.writeInt(id);
		out.close();
	}

	public int getNext() throws IOException
	{
		int oldId;
		int newId;
		synchronized (file)
		{
			DataInputStream in = new DataInputStream(new FileInputStream(file));
			oldId = in.readInt(); // read and save old id
			in.close(); // close file
		}

		synchronized (file)
		{
			DataOutputStream out = new DataOutputStream(new FileOutputStream(file));
			newId = ++oldId; // increment id
			out.writeInt(newId); // write new id
			out.close(); // close file
		}
		return newId;
	}
}