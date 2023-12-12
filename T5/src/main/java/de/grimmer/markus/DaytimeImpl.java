package de.grimmer.markus;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.time.LocalDateTime;

public class DaytimeImpl extends UnicastRemoteObject implements Daytime
{
    protected DaytimeImpl() throws RemoteException {}

    public String getDaytime()
    {
        return LocalDateTime.now().toString();
    }
}
