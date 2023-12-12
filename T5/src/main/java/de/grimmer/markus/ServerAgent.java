package de.grimmer.markus;

import java.rmi.*;

public interface ServerAgent extends Remote
{
    Agent execute(Agent agent) throws RemoteException;
}