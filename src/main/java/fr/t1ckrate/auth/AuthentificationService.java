package fr.t1ckrate.auth;

import io.vertx.ext.web.RoutingContext;

public class AuthentificationService {

    public boolean handleAuth(RoutingContext routingContext) {
        if (routingContext.queryParams().get("key") == null || !routingContext.queryParams().get("key").equals(AuthentificationToken.AUTHENTIFICATION_TOKEN.getToken())) {
            routingContext.response()
                    .putHeader("Access-Control-Allow-Origin", "*")
                    .putHeader("content-type", "application/json; charset=utf-8")
                    .setStatusCode(401)
                    .end("Authorization required.");
            return false;
        }
        return true;
    }
}
