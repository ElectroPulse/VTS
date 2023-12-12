package de.grimmer.markus;

import java.rmi.Naming;

public class DaytimeClient
{
    public static void main(String args[]) throws Exception
    {
        String host = args[0];
        Daytime remote = (Daytime) Naming.lookup("//" + host + "/echo");
        String received = remote.getDaytime();
        System.out.println(received);
    }
}
