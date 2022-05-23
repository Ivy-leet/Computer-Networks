public static void main(String[] args){
    String host;
    int port;
    String user;
    String password;
    String filename=;
    File file = new File(filename);
    Client client = new Client();

    client.connect(host, port, user, password);
    client.sendFile(file);
    client.disconnect();

    return 0;
}
