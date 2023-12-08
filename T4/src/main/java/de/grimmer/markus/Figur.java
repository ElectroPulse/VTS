package de.grimmer.markus;

public abstract class Figur
{
	protected char x;
	protected int y;
	abstract public void setPosition(char x, int y);

	abstract public String getPosition();
}
