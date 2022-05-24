/*
* 20575085, Koma T(Tlholo)
* 20456078, Dzimati BM(Malcolm)
*/

import java.io.File;
import java.io.IOException;
import java.nio.file.*;

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

            final Path path = FileSystems.getDefault().getPath(System.getProperty("user.home"), "Desktop");
            System.out.println(path);
            try (final WatchService watchService = FileSystems.getDefault().newWatchService()) {
                final WatchKey watchKey = path.register(watchService, StandardWatchEventKinds.ENTRY_MODIFY);
                while (true) {
                    final WatchKey wk = watchService.take();
                    for (WatchEvent<?> event : wk.pollEvents()) {
                        //we only register "ENTRY_MODIFY" so the context is always a Path.
                        final Path changed = (Path) event.context();
                        System.out.println(changed);
                        if (changed.endsWith("index.html")) {
                            System.out.println("My file has changed");
                            client.sendFile(file);
                        }
                    }
                    // reset the key
                    boolean valid = wk.reset();
                    if (!valid) {
                        System.out.println("Key has been unregisterede");
                    }
                }
            }catch(Exception e){

            }

            client.disconnect();
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        
    }
}



