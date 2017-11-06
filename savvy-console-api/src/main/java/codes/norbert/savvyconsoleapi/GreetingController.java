package codes.norbert.savvyconsoleapi;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.messaging.simp.SimpMessageHeaderAccessor;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.scheduling.annotation.EnableScheduling;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Controller;
import org.springframework.web.socket.server.support.HttpSessionHandshakeInterceptor;

import java.security.Principal;

@EnableScheduling
@Controller
public class GreetingController {

    @Autowired
    SimpMessagingTemplate s;


    @MessageMapping("/hello")
    @SendTo("/topic/greetings")
    public Greeting greeting(HelloMessage message) throws Exception {
        System.out.println(SimpMessageHeaderAccessor.create().toMap().values());

        return new Greeting("Hello, ");
    }

    @Scheduled(fixedRate=2000)
    public void sendMessages() {
System.out.println("dzialam");
            s.convertAndSendToUser("a", "/queue/horray", "Horray, " + "a" + "!");
    }
}