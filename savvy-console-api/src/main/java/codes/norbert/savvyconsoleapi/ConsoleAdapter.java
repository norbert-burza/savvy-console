package codes.norbert.savvyconsoleapi;

import java.security.Principal;

public interface ConsoleAdapter {

    /**
     * Opens a console assigned to the specified user.
     * @param user session user for whom to open the console
     * @return console manager
     */
    ConsoleManager openConsole(Principal user);


    /**
     * Allows to send key strokes and key combinations to the underlying process.
     * @param key a single key or key combination
     */
    void sendKey(String key);


}
