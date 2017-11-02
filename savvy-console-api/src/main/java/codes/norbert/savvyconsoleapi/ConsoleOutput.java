package codes.norbert.savvyconsoleapi;

import com.fasterxml.jackson.annotation.JsonProperty;

public class ConsoleOutput {

    @JsonProperty("output")
    public String output;

    public ConsoleOutput(String output) {
        this.output = output;
    }
}
