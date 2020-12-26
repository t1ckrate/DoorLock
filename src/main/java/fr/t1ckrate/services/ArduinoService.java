package fr.t1ckrate.services;

import fr.t1ckrate.DoorLock;
import io.vertx.ext.web.RoutingContext;

import java.io.IOException;
import java.net.URL;

public class ArduinoService {
    public void handleAccessGranted(RoutingContext routingContext) {
        if (!DoorLock.authentificationService.handleAuth(routingContext))
            return;

        routingContext.response()
                .setStatusCode(200)
                .end();

        DoorLock.espService.displayAccessGranted();
    }

    public void unlockDoor() {
        try {
            URL url = new URL("http://" + System.getenv("ARDUINO_IP") + "/?key=" + System.getenv("ARDUINO_TOKEN_UNLOCK") + "#");
            url.openConnection().getInputStream();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void toggleEmergency() {
        try {
            URL url = new URL("http://" + System.getenv("ARDUINO_IP") + "/?key=" + System.getenv("ARDUINO_TOKEN_EMERGENCY") + "#");
            url.openConnection().getInputStream();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


}
