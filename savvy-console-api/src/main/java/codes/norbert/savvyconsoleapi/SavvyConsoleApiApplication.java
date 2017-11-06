package codes.norbert.savvyconsoleapi;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

@Controller
@SpringBootApplication
public class SavvyConsoleApiApplication {

    @Autowired
    private ConsoleManager consoleManager;

    @RequestMapping(value = "input", method = RequestMethod.POST)
    @ResponseBody
    @CrossOrigin
    ConsoleOutput input(@RequestBody ConsoleInput input) {
        String output = consoleManager.execute(input.input);
        return new ConsoleOutput(output);
    }

	public static void main(String[] args) {
		SpringApplication.run(SavvyConsoleApiApplication.class, args);
	}
}
