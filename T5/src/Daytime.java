import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Daytime extends Remote
{
    String getDayTime() throws RemoteException;
}