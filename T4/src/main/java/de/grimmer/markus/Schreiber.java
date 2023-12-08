package de.grimmer.markus;

public class Schreiber extends Thread
{
	Figur f;
	public Schreiber(Figur f)
	{
		this.f = f;
	}

	public void run()
	{
		while (true)
		{
			int z = (int)(Math.random() * 8); // 0 .. 7
			char x = (char)('A' + z); // A .. H
			int y = 1 + z; // 1 .. 8
			f.setPosition(x, y);
		}
	}
}
