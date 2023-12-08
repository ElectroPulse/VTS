package de.grimmer.markus;

import java.io.*;
import java.net.*;

public class HTTPServer extends Thread {
  private int port;
  private String classpath;

  public HTTPServer(int port, String classpath) {
    this.port = port;
    this.classpath = classpath;
  }

  public void startServer() {
    try {
      ServerSocket server = new ServerSocket(port);

      InetAddress addr = InetAddress.getLocalHost();
      System.out.println("HTTPServer auf " +
        addr.getHostName() + "/" + addr.getHostAddress() +
        ":" + port + " gestartet ...");

      while (true) {
        Socket client = server.accept();
        new HTTPThread(client, classpath).start();
      }
    }
    catch (IOException e) {
      System.err.println(e);
    }
  }

  private class HTTPThread extends Thread {
    private Socket client;
    private String classpath;
    private BufferedReader in;
    private BufferedOutputStream out;

    public HTTPThread(Socket client, String classpath) {
      this.client = client;
      this.classpath = classpath;
    }

    public void run() {
      try {
        in = new BufferedReader(new InputStreamReader(client.getInputStream()));
        out = new BufferedOutputStream(client.getOutputStream());

        String path = getPath();
        sendFile(classpath + "/" + path);
      }
      catch (IOException e) {
        try {
          String hdr = "HTTP/1.1 400 " + e.getMessage() + "\r\n";
          hdr += "Content-Type: text/html\r\n\r\n";
          out.write(hdr.getBytes());
          out.flush();

          System.err.println(e);
        }
        catch (IOException ex) {
          System.err.println(ex);
        }
      }
      finally {
        try {
          client.close();
          if (in != null)
            in.close();
          if (out != null)
            out.close();
        }
        catch (IOException e) { }
      }
    }

    private String getPath() throws IOException {
      String line = in.readLine();
      System.out.println(client.getInetAddress().getHostAddress() +
        " " + line);
      String path = "";

      if (line != null && line.startsWith("GET /")) {
        line = line.substring(5);
        int idx = line.indexOf(" ");
        if (idx != -1)
          path = line.substring(0, idx);
      }

      if (path.length() > 0)
        return path;
      else
        throw new IOException("Ungueltiger HTTP-Request");
    }

    private void sendFile(String path) throws IOException {
      File f = new File(path);
      long length = f.length();
      FileInputStream fis = new FileInputStream(f);

      String hdr = "HTTP/1.1 200 OK\r\n";
      hdr += "Content-Type: application/octet-stream\r\n";
      hdr += "Content-Length: " + length + "\r\n\r\n";
      out.write(hdr.getBytes());
      out.flush();

      byte data[] = new byte[1024];
      int cnt = 0;
      while ((cnt = fis.read(data)) != -1) {
        out.write(data, 0, cnt);
      }
      
      out.flush();
      fis.close();
    }
  }

  public static void main(String[] args) {
    if (args.length != 2) {
      System.err.println("java HTTPServer <port> <classpath>");
      System.exit(1);
    }

    int port = Integer.parseInt(args[0]);
    String classpath = args[1];
    new HTTPServer(port, classpath).startServer();
  }
}
