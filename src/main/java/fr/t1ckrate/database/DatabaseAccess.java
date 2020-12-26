package fr.t1ckrate.database;

import com.zaxxer.hikari.HikariConfig;
import com.zaxxer.hikari.HikariDataSource;

import java.sql.Connection;
import java.sql.SQLException;

public class DatabaseAccess {
    private DatabaseCredentials credentials;
    private HikariDataSource hikariDataSource;

    public DatabaseAccess(DatabaseCredentials credentials) {
        this.credentials = credentials;
    }

    private void setupHikariCP() {
        final HikariConfig hikariConfig = new HikariConfig();

        hikariConfig.setMaximumPoolSize(10);
        hikariConfig.setJdbcUrl(credentials.toURI());
        hikariConfig.setUsername(credentials.getUser());
        hikariConfig.setPassword(credentials.getPass());
        hikariConfig.setMaxLifetime(31000);
        hikariConfig.setConnectionTimeout(30000);
        hikariConfig.setLeakDetectionThreshold(30000);
        hikariConfig.setMinimumIdle(0);

        this.hikariDataSource = new HikariDataSource(hikariConfig);
    }

    public void initPool() {
        setupHikariCP();
    }

    public void closePool() {
        this.hikariDataSource.close();
    }

    public Connection getConnection() throws SQLException {
        if (this.hikariDataSource == null) {
            System.out.println("Not connected");
            setupHikariCP();
        }
        return this.hikariDataSource.getConnection();

    }
}

