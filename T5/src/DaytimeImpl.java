import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.time.LocalDateTime;

public class DaytimeImpl extends UnicastRemoteObject implements Daytime

{
    protected DaytimeImpl() throws RemoteException
    {
    }

    @Override
    public String getDayTime() throws RemoteException
    {
        return LocalDateTime.now().toString();
    }
}
