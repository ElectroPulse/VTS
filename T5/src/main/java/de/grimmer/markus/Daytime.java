package de.grimmer.markus;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Daytime extends Remote
{
    String getDaytime() throws RemoteException;
}
