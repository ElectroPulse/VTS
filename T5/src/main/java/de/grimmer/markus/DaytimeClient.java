package de.grimmer.markus;

import java.rmi.Naming;

public class DaytimeClient {
    public static void main(String args[]) throws Exception {
        String host = args[0];
        String text = args[1];
        Daytime remote = (Daytime) Naming.lookup("//" + host + "/echo");
        String received = remote.getDaytime(text);
        System.out.println(received);
    }
}
