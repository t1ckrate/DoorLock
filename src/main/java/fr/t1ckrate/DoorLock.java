package fr.t1ckrate;

import fr.t1ckrate.auth.AuthentificationService;
import fr.t1ckrate.auth.PasswordAuthentication;
import fr.t1ckrate.database.DatabaseManager;
import fr.t1ckrate.datamanager.UserManager;
import fr.t1ckrate.services.ArduinoService;
import fr.t1ckrate.services.EspService;
import fr.t1ckrate.services.PasswordService;
import fr.t1ckrate.services.VerticleService;
import io.vertx.core.Vertx;

public class DoorLock {

    private static VerticleService verticleService;

    public static AuthentificationService authentificationService;
    public static PasswordAuthentication passwordAuthentication;

    public static UserManager userManager;

    public static PasswordService passcodeService;
    public static ArduinoService arduinoService;
    public static EspService espService;

    public static void main(String[] args) {
        System.out.println("--- Server initialisation ---");

        DatabaseManager.initAllDatabaseConnections();
        userManager = new UserManager();

        System.out.println("==============================");
        System.out.println("Database & DataManager initialization completed");

        passwordAuthentication = new PasswordAuthentication();
        System.out.println("==============================");
        System.out.println("Password service initialization completed");

        authentificationService = new AuthentificationService();
        System.out.println("==============================");
        System.out.println("Authentification service initialization completed");

        passcodeService = new PasswordService();
        System.out.println("==============================");
        System.out.println("Passcode service initialization completed");

        arduinoService = new ArduinoService();
        System.out.println("==============================");
        System.out.println("Arduino service initialization completed");

        espService = new EspService();
        System.out.println("==============================");
        System.out.println("ESP8266 service initialization completed");

        verticleService = new VerticleService();
        try {
            Vertx vertx = Vertx.vertx();
            vertx.deployVerticle(verticleService);
        } catch (Exception e) {
            e.printStackTrace();
        }

        System.out.println("==============================");
        System.out.println("Verticle initialization completed");

        System.out.println("======= Initialization completed =======");

        System.out.println(passwordAuthentication.hash("175839".toCharArray()));
    }
}
