import java.io.*;
import java.net.*;
import java.util.Date;
 
/**
 * This program demonstrates a simple TCP/IP socket server.
 */
public class HelloServer {
    static String serverMsg= "Hello from Java server";
    public static void main(String[] args) {
        int port = 9000;
	    byte[] buffer = new byte[1024];

        if (args.length > 0) 
            port = Integer.parseInt(args[0]);
 
        // open listening socket 
        try (ServerSocket serverSocket = new ServerSocket(port)) {
 
            System.out.println("Server is listening on port " + port);
 
            while (true) {
                Socket socket = serverSocket.accept();

                InputStream input = socket.getInputStream();
                int len = input.read(buffer);
                System.out.println(new String(buffer, 0, len));

                OutputStream output = socket.getOutputStream();
		output.write(serverMsg.getBytes());

                socket.close();
            }
        } catch (IOException ex) {
            System.out.println("Server exception: " + ex.getMessage());
            ex.printStackTrace();
        }
    }
}
