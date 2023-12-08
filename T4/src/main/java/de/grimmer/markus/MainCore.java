package de.grimmer.markus;

public class MainCore
{
	public static void main(String[] args)
	{
		FigurenThreads ft = new FigurenThreads();
		ft.startThreads();

		try
		{
			Thread.sleep(1000 * 60 * 5);
		}
		catch (InterruptedException e)
		{
			throw new RuntimeException(e);
		}
	}
}
