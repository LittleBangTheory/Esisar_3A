package src;
import org.apache.commons.csv.CSVParser;
import org.apache.commons.csv.CSVRecord;
import org.apache.commons.csv.CSVFormat;
import java.io.Reader;
import java.nio.file.Files;
import java.nio.file.Paths;

public class Test {
    static public void main(){
        try{
        Reader reader = Files.newBufferedReader(Paths.get("./conso.csv"));
        CSVParser parser = new CSVParser(reader, CSVFormat.DEFAULT);
        for (CSVRecord record : parser) {
            String columnOne = record.get(0);
            String columnTwo = record.get(1);
        }
    } catch (Exception e){
        System.out.println(e);
    }
    }
}
