package de.grimmer.markus;

public class MyFigur extends Figur
{

	@Override
	synchronized public void setPosition(char x, int y)
	{
		this.x = x;
		MachMal.eineZehntelSekundeLangGarNichts();
		this.y = y;
	}

	@Override
	synchronized public String getPosition()
	{
		try
		{
			Thread.sleep(1000);
		}
		catch (InterruptedException e)
		{
			System.err.println("Failed to sleep!");
			throw new RuntimeException(e);
		}
		return "" + x + y;
	}
}
