import java.net.*;
import java.io.*;

public class MulticastReceiver {
    public static void main(String[] args) throws Exception {
        InetAddress group = InetAddress.getByName("225.0.0.37");
        int port = 10000;
        MulticastSocket socket = new MulticastSocket(port);
        socket.joinGroup(group);

        byte[] buffer = new byte[256];
        DatagramPacket packet = new DatagramPacket(buffer, buffer.length);

        System.out.println("The multicast client is on.");

        while (true) {
            socket.receive(packet);
            String message = new String(packet.getData(), 0, packet.getLength());
            System.out.println(message);
        }
    }
}

