import java.net.*;
import java.io.*;

public class MulticastSender {

    public static void main(String[] args) throws Exception {

        InetAddress group = InetAddress.getByName("225.0.0.37");
        int port = 10000;
        MulticastSocket socket = new MulticastSocket();
        //socket.joinGroup(group);

        String msg = "This is a multicast message.\n";
        byte[] buf = msg.getBytes();

        System.out.println("The multicast sender is on.");

        while (true) {
            System.out.println("The multicast sender sends: " + msg);
            DatagramPacket packet = new DatagramPacket(buf, buf.length, group, port);
            socket.send(packet);
            Thread.sleep(5000);
        }

    }
}

