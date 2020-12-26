package fr.t1ckrate.database;

public class DatabaseCredentials {
    private String host;
    private String database;
    private String user;
    private String pass;
    private int port;


    public DatabaseCredentials(String host, String database, String user, String pass, int port) {
        this.host = host;
        this.database = database;
        this.user = user;
        this.pass = pass;
        this.port = port;
    }

    public String toURI() {
        final StringBuilder sb = new StringBuilder();

        sb.append("jdbc:mysql://")
                .append(host)
                .append(":")
                .append(port)
                .append("/")
                .append(database)
                .append("?allowPublicKeyRetrieval=true&useSSL=false");

        return sb.toString();

    }

    public String getHost() {
        return host;
    }

    public String getDatabase() {
        return database;
    }

    public String getUser() {
        return user;
    }

    public String getPass() {
        return pass;
    }

    public int getPort() {
        return port;
    }
}
