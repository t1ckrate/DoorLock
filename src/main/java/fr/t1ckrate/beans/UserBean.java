package fr.t1ckrate.beans;

public class UserBean {
    public UserType userType;
    public String name;
    public String hashedPasscode;

    public UserType getUserType() {
        return userType;
    }

    public void setUserType(UserType userType) {
        this.userType = userType;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getHashedPasscode() {
        return hashedPasscode;
    }

    public void setHashedPasscode(String hashedPasscode) {
        this.hashedPasscode = hashedPasscode;
    }
}
