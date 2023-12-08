package de.grimmer.markus;

public class FigurenThreads
{
	public void startThreads()
	{
		Figur f = new MyFigur();
		Schreiber s = new Schreiber(f);
		Leser l = new Leser(f);

		s.setDaemon(true);
		l.setDaemon(true);

		s.start();
		l.start();
	}
}
