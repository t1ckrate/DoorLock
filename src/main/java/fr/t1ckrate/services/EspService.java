package fr.t1ckrate.services;

import fr.t1ckrate.DoorLock;
import fr.t1ckrate.beans.UserBean;
import fr.t1ckrate.beans.UserType;
import io.vertx.ext.web.RoutingContext;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;

public class EspService {
    public void handlePasswordRequest(RoutingContext routingContext) {
        UserBean userBean = DoorLock.passcodeService.checkPassword(routingContext);
        if (userBean != null) {
            if (userBean.getUserType().equals(UserType.EMERGENCY))
                DoorLock.arduinoService.toggleEmergency();
            else
                DoorLock.arduinoService.unlockDoor();
        }

    }

    public void displayAccessGranted() {
        try {
            System.out.println("Trying to connect");
            // Create a neat value object to hold the URL
            URL url = new URL("http://" + System.getenv("ESP_IP") + "/displayAccessGranted");
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestProperty("Authorization", "Basic " + System.getenv("ESP_CREDENTIALS"));
            connection.getInputStream();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
