package fr.t1ckrate.services;

import com.google.gson.Gson;
import fr.t1ckrate.DoorLock;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Promise;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;

public class VerticleService extends AbstractVerticle {

    private static final Gson GSON = new Gson();

    @Override
    public void start(Promise<Void> startPromise) {
        try {
            Router router = Router.router(vertx);

            router.route("/api/*").handler(BodyHandler.create());
            router.get("/api/hello").handler(this::getServerStatus);

            router.post("/api/esp/requestPassword").handler(DoorLock.espService::handlePasswordRequest);

            router.post("/api/arduino/handleAccessGranted").handler(DoorLock.arduinoService::handleAccessGranted);

            vertx.createHttpServer()
                    .requestHandler(router)
                    .listen(config().getInteger("http.port", Integer.parseInt(System.getenv("PORT"))),
                            result -> {
                                if (result.succeeded())
                                    startPromise.complete();
                                else
                                    startPromise.fail(result.cause());
                            }
                    );
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void getServerStatus(RoutingContext routingContext) {
        routingContext.response()
                .setStatusCode(200)
                .putHeader("Access-Control-Allow-Origin", "*")
                .putHeader("content-type", "application/json; charset=utf-8")
                .end();
    }

}
