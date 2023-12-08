package de.grimmer.markus;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class DaytimeImpl extends UnicastRemoteObject implements Daytime {
    public DaytimeImpl() throws RemoteException {
    }
    public String getDaytime(String s) {
        return s+"/erzeugt das Echo/"+s;
    }
}
