package codes.norbert.savvyconsoleapi;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

@Controller
@SpringBootApplication
public class SavvyConsoleApiApplication {

    @RequestMapping(value = "input", method = RequestMethod.POST)
    @ResponseBody
    @CrossOrigin
    ConsoleOutput input(@RequestBody ConsoleInput input) {
        System.out.println(input.input);
        return new ConsoleOutput(input.input);
    }

	public static void main(String[] args) {
		SpringApplication.run(SavvyConsoleApiApplication.class, args);
	}
}
