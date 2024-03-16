package IO.src;

import java.io.*;

/**
 * Main class for IO simple methods
 * @author O. Carton
 * @version 1.2 modified by LG
 */

// TODO : implement public class "Files": a static method wc and a main.

public class Files {
    public static void main(String[] args) {
        System.out.println("Hello");
        System.out.println(Files.wc("rfc1.txt"));
    }
    public static int wc(String name) {
        try{
        File file = new File(name);
        FileReader reader = new FileReader(file);
        BufferedReader bufreader = new BufferedReader(reader);
        int i = 0;
        while (bufreader.read()!=-1) {
            i++;
        }
        reader.close();
        bufreader.close();
        return i;
        }
        catch (Exception e){
            System.out.println(e);
            return 0;
        }
    }
    
}
