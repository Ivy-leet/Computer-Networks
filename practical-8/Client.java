/*
* 20575085, Koma T(Tlholo)
* 20456078, Dzimati BM(Malcolm)
*/

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.util.StringTokenizer;

public class Client{
    private Socket socket = null;

    private BufferedReader reader = null;

    private BufferedWriter writer = null;

    private static boolean DEBUG = false;

    public Client() {

    }

    /**
     * Connects to an FTP server and logs in with the supplied username and
     * password.
     */
    public synchronized void connect(String host, int port, String user, String pass) throws IOException {
        if (socket != null) {
            throw new IOException("SimpleFTP is already connected. Disconnect first.");
        }
        socket = new Socket(host, port);
        reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));

        String response = readLine();
        if (!response.startsWith("220 ")) {
            throw new IOException("SimpleFTP received an unknown response when connecting to the FTP server: "+ response);
        }

        sendLine("USER " + user);

        response = readLine();
        if (!response.startsWith("331 ")) {
            throw new IOException("SimpleFTP received an unknown response after sending the user: " + response);
        }

        sendLine("PASS " + pass);

        response = readLine();
        if (!response.startsWith("230 ")) {
            throw new IOException("SimpleFTP was unable to log in with the supplied password: "+ response);
        }
    }

    /**
     * Disconnects from the FTP server.
     */
    public synchronized void disconnect() throws IOException {
        try {
            sendLine("QUIT");
        } finally {
            socket = null;
        }
    }

    /**
     * Sends a file to be stored on the FTP server. Returns true if the file
     * transfer was successful. The file is sent in passive mode to avoid NAT or
     * firewall problems at the client end.
     */
    public synchronized boolean sendFile(File file) throws IOException {
        if (file.isDirectory()) {
            throw new IOException("SimpleFTP cannot upload a directory.");
        }

        String filename = file.getName();

        return stor(new FileInputStream(file), filename);
    }

    /**
     * Sends a file to be stored on the FTP server. Returns true if the file
     * transfer was successful. The file is sent in passive mode to avoid NAT or
     * firewall problems at the client end.
     */
    public synchronized boolean stor(InputStream inputStream, String filename)
        throws IOException {

        BufferedInputStream input = new BufferedInputStream(inputStream);

        sendLine("PASV");
        String response = readLine();
        if (!response.startsWith("227 ")) {
            throw new IOException("SimpleFTP could not request passive mode: "+ response);
        }

        String ip = null;
        int port = -1;
        int opening = response.indexOf('(');
        int closing = response.indexOf(')', opening + 1);
        if (closing > 0) {
            String dataLink = response.substring(opening + 1, closing);
            StringTokenizer tokenizer = new StringTokenizer(dataLink, ",");
            try {
                ip = tokenizer.nextToken() + "." + tokenizer.nextToken() + "." + tokenizer.nextToken() + "." + tokenizer.nextToken();
                port = Integer.parseInt(tokenizer.nextToken()) * 256 + Integer.parseInt(tokenizer.nextToken());
            } catch (Exception e) {
                throw new IOException("SimpleFTP received bad data link information: "+ response);
            }
        }

        Socket dataSocket = new Socket(ip, port);
        sendLine("STOR " + filename);

        response = readLine();
        if (!response.startsWith ("150 ")) {
            //if (!response.startsWith("150 ")) {
            throw new IOException("SimpleFTP was not allowed to send the file: "+ response);
        }

        
        BufferedOutputStream output = new BufferedOutputStream(dataSocket.getOutputStream());
        byte[] buffer = new byte[4096];
        int bytesRead = 0;
        while ((bytesRead = input.read(buffer)) != -1) {
            output.write(buffer, 0, bytesRead);
        }
        output.flush();
        output.close();
        input.close();

        response = readLine();
        return response.startsWith("226 ");

        
    }
    

    /**
     * Sends a raw command to the FTP server.
     */
    private void sendLine(String line) throws IOException {
        if (socket == null) {
            throw new IOException("SimpleFTP is not connected.");
        }
        try {
            writer.write(line + "\r\n");
            writer.flush();
            if (DEBUG) {
                System.out.println("> " + line);
            }
        } catch (IOException e) {
            socket = null;
            throw e;
        }
    }

    private String readLine() throws IOException {
        String line = reader.readLine();
        if (DEBUG) {
            System.out.println("< " + line);
        }
        return line;
    }

    
}
