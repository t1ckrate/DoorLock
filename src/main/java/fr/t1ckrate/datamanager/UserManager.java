package fr.t1ckrate.datamanager;

import fr.t1ckrate.DoorLock;
import fr.t1ckrate.beans.UserBean;
import fr.t1ckrate.beans.UserType;
import fr.t1ckrate.database.DatabaseManager;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class UserManager {

    // Defines
    private Connection connection = null;
    private PreparedStatement statement = null;
    private ResultSet resultset = null;

    public UserBean getUserByPassword(String hashedPassword) {
        try {
            connection = DatabaseManager.SERVER.getDatabaseAccess().getConnection();
            statement = connection.prepareStatement("SELECT * FROM users");

            resultset = statement.executeQuery();
            while (resultset.next()) {
                if (DoorLock.passwordAuthentication.authenticate(hashedPassword.toCharArray(), resultset.getString("hashedPassword"))) {
                    UserBean userBean = new UserBean();
                    userBean.setUserType(UserType.valueOf(resultset.getString("userType")));
                    userBean.setName(resultset.getString("name"));
                    return userBean;
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            try {
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return null;
    }

}
