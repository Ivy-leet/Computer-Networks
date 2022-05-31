/*
* 20575085, Koma T (Tlholo)
* 20456078, Dzimati BM (Malcolm)
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

    public Client() {
        
    }
    
    public synchronized void connect(String host, int port, String user, String pass) throws IOException {
        if (socket != null) {
            throw new IOException("Socket already in use");
        }
        socket = new Socket(host, port);

        reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));

        String response = readLine();
        if (!response.startsWith("220 "))
            throw new IOException(response);

        sendLine("USER " + user);

        response = readLine();
        if (!response.startsWith("331 "))
            throw new IOException(response);

        sendLine("PASS " + pass);

        response = readLine();
        if (!response.startsWith("230 "))
            throw new IOException(response);
    }

    public synchronized void disconnect() throws IOException {
        try {
            sendLine("QUIT");
        } finally {
            socket = null;
        }
    }

    public synchronized boolean sendFile(File file) throws IOException {
        if (file.isDirectory())
            throw new IOException("File does not exist");

        String filename = file.getName();

        return stor(new FileInputStream(file), filename);
    }

    public synchronized boolean stor(InputStream inputStream, String filename) throws IOException {

        BufferedInputStream inputStream2 = new BufferedInputStream(inputStream);

        sendLine("PASV");
        String response = readLine();

        if (!response.startsWith("227 "))
            throw new IOException(response);

        String ip = null;
        int port = -1;
        int openingParen = response.indexOf('(');
        int closingParen = response.indexOf(')', openingParen + 1);
        if (closingParen > 0) {
            String dataLink = response.substring(openingParen + 1, closingParen);
            StringTokenizer tokenizer = new StringTokenizer(dataLink, ",");

            try {
                ip = tokenizer.nextToken() + "." + tokenizer.nextToken() + "." + tokenizer.nextToken() + "." + tokenizer.nextToken();
                port = Integer.parseInt(tokenizer.nextToken()) * 256 + Integer.parseInt(tokenizer.nextToken());
            } catch (Exception e) {
                throw new IOException(response);
            }
        }

        Socket dataSocket = new Socket(ip, port);
        sendLine("STOR " + filename);

        response = readLine();
        if (!response.startsWith ("150 ")) 
            throw new IOException(response);

        
        BufferedOutputStream outputStream = new BufferedOutputStream(dataSocket.getOutputStream());
        byte[] buffer = new byte[4096];
        int bytesRead = 0;

        while ((bytesRead = inputStream2.read(buffer)) != -1) 
            outputStream.write(buffer, 0, bytesRead);

        outputStream.flush();
        outputStream.close();
        inputStream2.close();

        response = readLine();
        return response.startsWith("226 ");

    }
    
    private void sendLine(String line) throws IOException {
        if (socket == null)
            throw new IOException("Not Connected to FTP.");
    
        try {
            writer.write(line + "\r\n");
            writer.flush();
        } catch (IOException e) {
            socket = null;
            throw e;
        }
    }

    private String readLine() throws IOException {
        String line = reader.readLine();

        // System.out.println("< " + line);

        return line;
    }

    
}
