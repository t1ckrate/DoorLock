package fr.t1ckrate.services;

import com.google.gson.Gson;
import fr.t1ckrate.DoorLock;
import fr.t1ckrate.beans.UserBean;
import io.vertx.ext.web.RoutingContext;

public class PasswordService {
    public UserBean checkPassword(RoutingContext routingContext) {
        try {
            if (!DoorLock.authentificationService.handleAuth(routingContext))
                return null;


            if (routingContext.queryParams().get("password") == null) {
                routingContext.response()
                        .putHeader("Access-Control-Allow-Origin", "*")
                        .putHeader("content-type", "application/json; charset=utf-8")
                        .setStatusCode(400)
                        .end("Missing passcode parameter");
                return null;
            }


            UserBean userBean = DoorLock.userManager.getUserByPassword(routingContext.queryParams().get("password"));

            if (userBean == null) {
                routingContext.response()
                        .putHeader("Access-Control-Allow-Origin", "*")
                        .putHeader("content-type", "application/json; charset=utf-8")
                        .setStatusCode(401)
                        .end("Password not allowed");
                return null;
            }

            routingContext.response()
                    .putHeader("Access-Control-Allow-Origin", "*")
                    .putHeader("content-type", "application/json; charset=utf-8")
                    .setStatusCode(200)
                    .end(new Gson().toJson(userBean));

            return userBean;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
}
