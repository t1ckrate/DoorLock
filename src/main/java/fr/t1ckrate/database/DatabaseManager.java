package fr.t1ckrate.database;

public enum DatabaseManager {
    SERVER(new DatabaseCredentials(System.getenv("MYSQL_HOST"), System.getenv("MYSQL_DATABASE"), System.getenv("MYSQL_USER"), System.getenv("MYSQL_PASSWORD"), Integer.parseInt(System.getenv("MYSQL_PORT"))));

    private DatabaseAccess databaseAccess;

    DatabaseManager(DatabaseCredentials credentials) {
        this.databaseAccess = new DatabaseAccess(credentials);
    }

    public DatabaseAccess getDatabaseAccess() {
        return databaseAccess;
    }

    public static void initAllDatabaseConnections() {
        for (DatabaseManager databaseManager : values()) {
            databaseManager.databaseAccess.initPool();
        }
    }

    public static void closeAllDatabaseConnections() {
        for (DatabaseManager databaseManager : values()) {
            databaseManager.databaseAccess.closePool();
        }
    }
}
