import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ServerAgent extends Remote
{
    Agent execute(Agent agent) throws RemoteException;
}