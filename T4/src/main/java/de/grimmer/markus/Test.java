package de.grimmer.markus;

import java.io.IOException;

public class Test {
    public static void main(String[] args) {
        UniqueId uid = new UniqueId("id.dat");
        try {
            uid.init(10000); // init with 10000
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        for (int i = 0; i < 5; i++) { // start 5 threads
            Thread t = new MyThread(uid); // pass uid to thread
            t.start();
        }
    }
}
