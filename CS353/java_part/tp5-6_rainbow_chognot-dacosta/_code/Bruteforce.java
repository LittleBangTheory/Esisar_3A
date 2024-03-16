import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;

/**
 * Exo1
 */
public class Bruteforce {
    public static void main(String[] args) throws NoSuchAlgorithmException
    {
        // Création d'un objet MessageDigest
        MessageDigest digest = MessageDigest.getInstance("MD5");

        // Déclaration des variables
        String str;
        byte[] hash;
        StringBuilder sb = new StringBuilder();

        //Déclaration des hash
        String Clara = "38251B4C8C210841C60CDE0B7E4C7A87";
        String Alice = "8FC92036B2963C604DC38B2DDB305148";
        String Bob = "367F3AC1129CC92DCBB8C4B9EA4EE55A";
        
        Boolean Alice_Found = false;
        Boolean Bob_Found = false;
        Boolean Clara_Found = false;

        String Alice_Pass = "";
        String Bob_Pass = "";
        String Clara_Pass = "";

        // Déclaration d'un tableau pour stocker le temps de calcul pour chaque hash
        long[] times = new long[3];

        long time = System.currentTimeMillis();

        //Bruteforce
        int pass = 0;
        while(pass<100000000) {
            str = String.format("%08d", pass); //Pour transformer
            System.out.println("Trying with : "+str);
            hash = digest.digest(str.getBytes());
            sb = new StringBuilder();
            for (byte b : hash)
            {
                sb.append(String.format("%02X", b));
            }
            //sb.toString montre le hash de pass
            if (!Clara_Found && sb.toString().equals(Clara)) {
                times[0] = System.currentTimeMillis() - time;
                System.out.println("Clara : "+pass);
                Clara_Pass = str;
                Clara_Found = true;
            }
            if (!Alice_Found && sb.toString().equals(Alice)) {
                times[1] = System.currentTimeMillis() - time;
                System.out.println("Alice : "+pass);
                Alice_Pass = str;
                Alice_Found = true;
            }
            if (!Bob_Found && sb.toString().equals(Bob)) {
                times[2] = System.currentTimeMillis() - time;
                System.out.println("Bob : "+pass);
                Bob_Pass = str;
                Bob_Found = true;
            }
            if (Alice_Found && Bob_Found && Clara_Found) {
                System.out.println("All found : \n- Clara : "+Clara_Pass+"\n- Alice : "+Alice_Pass+"\n- Bob : "+Bob_Pass);
                break;
            }
            pass=pass+1;
        }

        time = System.currentTimeMillis() - time;
        // Convert to seconds
        time = time/1000;

        System.out.println("Temps de recherche :\n- Clara : "+times[0]/1000+"s\n- Alice : "+times[1]/1000+"s\n- Bob : "+times[2]/1000+"s");
    }
}