/*
* 20575085, Koma T(Tlholo)
* 20456078, Dzimati BM(Malcolm)
*/

import java.io.File;
import java.io.IOException;

/**
 * Main
 */
public class Main {

    public static void main(String[] args){
        String host="localhost";
        int port=21;
        String user=System.getenv("USER");
        String password=System.getenv("PASSWD");
        String filename="index.html";
        File file = new File(filename);
        Client client = new Client();
    
        try {
            client.connect(host, port, user, password);
            client.sendFile(file);
            client.disconnect();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        
    }
}



