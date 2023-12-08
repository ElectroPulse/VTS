package main;// File  : IncDecThreads.java
// Date  : 9.8.2005 23:15:35

// Diese Klasse definiert einen Z�hler,
// der von einem Thread um die Gr��e increment
// weitergeschaltet wird.
// In der Anwendung wird als increment nur +1 bzw. -1 benutzt.
// Das Programm entstand in Kooperation mit
// Herrn Dipl. Inf. (FH) Martin Opel

// Ab JDK 1.5

public class IncDecThreads extends Thread {
  // Maximim f�r die Iteration
  public static final long MAX = 10000000;
  // Um die folgende Gr��e weiterschalten
  private long increment = 0;
  
  // Festlegung des Wertes von increment
  public IncDecThreads (long increment) {
    this.increment = increment;
  }
  
  // Erzeuge die Threads, Ablauf, Warten auf die Threads
  public static void main (String[] args) {
    IncDecThreads thread1 = new IncDecThreads(+1);
    IncDecThreads thread2 = new IncDecThreads(-1);
    long start = System.currentTimeMillis ();
    thread1.start(); 
    thread2.start();
    try {
      thread1.join ();
      thread2.join ();
    } catch (Exception e) {
    }
    System.out.println ("zaehler: "+  zaehler + " nach msec: "+(System.currentTimeMillis ()-start));
  }

  
  // Dies ist der Z�hler, der von diversen Threads
  // bearbeitet wird.
  static long zaehler=0;
  
  

  // Unsynchronisiert geht es schnell, aber....
  public void demoUnSync () {
    for (long i = 0; i < MAX; i++) {
      zaehler = zaehler + increment;
    }
  }

  // Einfach rauf oder runter z�hlen...
  public void demo () {
    for (long i = 0; i < 100; i++) {
      zaehler = zaehler + increment;
      try {
        sleep ((int)(1000*Math.random ()));
      } catch (Exception e) {
        return;
      }
    }
  }
  
  public void run() {
    demoUnSync ();  // Unsynchronisiertes Z�hlen
    //demoSync ();  // Synchronisiertes Z�hlen
  }
}
