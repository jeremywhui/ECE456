import java.io.*;
import java.net.*;
 
/**
 * This program demonstrates a simple TCP/IP socket client.
 */
public class HelloClient {
    static String clientMsg= "Hello from Java client";
    public static void main(String[] args) {
        int port = 9000;
	byte[] buffer = new byte[1024];
        String hostname = "localhost";

        if (args.length > 0) {
            hostname = args[0];
        }
        if (args.length > 1) {
            port = Integer.parseInt(args[1]);
        }
 
        try (Socket socket = new Socket(hostname, port)) {
 
            // write client msg
            OutputStream output = socket.getOutputStream();
            output.write(clientMsg.getBytes());

            // read server msg
            InputStream input = socket.getInputStream();
            int len = input.read(buffer);
            System.out.println(new String(buffer, 0, len));

            socket.close();

        } catch (UnknownHostException ex) {
            System.out.println("Server not found: " + ex.getMessage());
        } catch (IOException ex) {
            System.out.println("I/O exception: " + ex.getMessage());
            ex.printStackTrace();
        }
    }
}
