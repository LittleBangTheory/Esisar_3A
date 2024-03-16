import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;

/**
 * Reformat
 */
public class Reformat {

    public static void main(String[] args) {
        try{
            FileReader fr = new FileReader("table.txt");
            BufferedReader buff = new BufferedReader(fr);
            FileWriter fw = new FileWriter("table2.txt");
            String[] elem;
            String line = buff.readLine();
            String to_write1;
            String to_write2;
            while (line!=null) {
                elem = line.split(",");
                to_write1 = String.format("%08d",Integer.parseInt(elem[0]));
                to_write2 = String.format("%08d",Integer.parseInt(elem[1]));
                fw.write(to_write1+","+to_write2+"\n");
                line = buff.readLine();
            }
            fr.close();
            buff.close();
            fw.close();
        }//END
        catch (Exception e){
            System.out.println(e);
            return;
        }
    }
}