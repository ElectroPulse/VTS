package de.grimmer.markus;

import java.io.IOException;

public class MyThread extends Thread
{
	private final UniqueId uid;

	public MyThread(UniqueId uid)
	{
		this.uid = uid;
	}

	@Override
	public void run()
	{
		for (int n = 0; n < 10; n++) // get 10 ids
		{
			try
			{
				int num = uid.getNext(); // get next id
				System.out.println("Thread " + Thread.currentThread().getName() + " " + num); // print thread and id
			}
			catch (IOException e)
			{
				System.err.println(e.getClass().getSimpleName() + ": " + e.getMessage());
			}
		}
	}
}