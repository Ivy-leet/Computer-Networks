/*
* 20575085, Koma T (Tlholo)
* 20456078, Dzimati BM (Malcolm)
*/

import java.io.File;
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
        String filename="/index.html";
        
        Client client = new Client();
    
        try {
            client.connect(host, port, user, password);

            final Path path = FileSystems.getDefault().getPath(System.getProperty("user.home"), "Desktop");
            System.out.println(path);

            File file = new File(path.toString()+filename);

            try (final WatchService watchService = FileSystems.getDefault().newWatchService()) {
                final WatchKey watchKey = path.register(watchService, StandardWatchEventKinds.ENTRY_MODIFY);
                while (true) {
                    final WatchKey wk = watchService.take();

                    for (WatchEvent<?> event : wk.pollEvents()) {
                        
                        final Path changed = (Path) event.context();
                        // System.out.println(changed);

                        if (changed.endsWith("index.html")) {
                            System.out.println(changed.toString()+" has changed.");
                            client.sendFile(file);
                        }
                    }
                    
                    boolean valid = wk.reset();
                    if (!valid)
                        System.out.println("Key has been unregisterede");
        
                }
            }catch(Exception e){
                e.printStackTrace();
            }

            client.disconnect();
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        
    }
}



