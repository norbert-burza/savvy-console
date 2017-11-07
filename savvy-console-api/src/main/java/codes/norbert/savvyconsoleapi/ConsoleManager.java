package codes.norbert.savvyconsoleapi;

import org.springframework.stereotype.Component;

import java.io.*;
import java.net.Socket;

@Component
public class ConsoleManager {

    private PrintWriter pw;

    private BufferedReader in;

    public ConsoleManager() {
//        try {
//            Socket socket = new Socket("192.168.43.158", 8080);
//            pw = new PrintWriter(socket.getOutputStream());
//            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
//
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
    }

    public String execute(String input) {
//        if (pw != null) {
//            pw.write(input);
//            try {
//                return in.readLine();
//            } catch (IOException e) {
//                e.printStackTrace();
//            }
//        }
        return "Error";
    }
}
