package de.grimmer.markus;

public class Leser extends Thread
{
	Figur f;
	public Leser(Figur f)
	{
		this.f = f;
	}

	public void run()
	{
		for (int i = 1; i <= 30; i++)
		{
			System.out.print("\n" + f.getPosition() + " ");
		}
	}
}
