package fr.t1ckrate.auth;

public enum AuthentificationToken {

    AUTHENTIFICATION_TOKEN(System.getenv("TOKEN")),
    ;

    private String token;

    AuthentificationToken(String token) {
        this.token = token;
    }

    public String getToken() {
        return token;
    }
}
