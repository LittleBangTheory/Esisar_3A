import java.io.BufferedReader;
import java.io.FileReader;
import java.security.MessageDigest;
import java.util.Scanner;

public class Cracking {

    public static byte[] StringToByteArray(String s) {
        // Conversion de s en un tableau de byte, le premier byte est 0x40 , le deuxième 0x41 , et ainsi de suite 
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) 
        {
        data[i/2] = (byte) ((Character.digit(s.charAt(i), 16) << 4) + Character.digit(s.charAt(i+1), 16));
        }
        return data;
    }

    public static void main(String[] args) {
        try {
            //Lecture table
            String fileName = "../_data/table.txt";
            System.out.println("########## CHARGEMENT DE LA TABLE DEPUIS LE FICHIER : "+fileName+" ##########");
            FileReader FILE = new FileReader(fileName);
            BufferedReader buff = new BufferedReader(FILE);
            String ligne = buff.readLine();
            // Déclaration du tableau de Noeuds
            Noeud[] table = new Noeud[165901];
            int i = 0;
            while (ligne!=null) {
                String[] elem = ligne.split(",");
                table[i] = new Noeud(Integer.parseInt(elem[0]),Integer.parseInt(elem[1]));
                
                i=i+1;
                ligne = buff.readLine();
            }
            // Fermeture des flux
            FILE.close();
            buff.close();

            //Input user
            String save;
            Scanner input = new Scanner(System.in);
            System.out.println("Entrer un hash MD5 : "); //Exemple : 25d55ad283aa400af464c76d713c07ad
            save = input.nextLine().toUpperCase();
            input.close();

            //Def des variables
            int looking=0; // Détermine la profondeur des fonctions de réduction
            int px = 0;
            MessageDigest digest = MessageDigest.getInstance("MD5"); // Hashage md5
            byte[] tmp;
            boolean cracked = false; // Le password a-t-il été trouvé ?
            boolean hashFound; // Y'a-t-il une occurence ? Faux positif inclus
            String hash;
            int red;
            StringBuilder sb;
            int py=0;

            // Mesure du temps
            long startTime = System.currentTimeMillis();

            while(!cracked){
                hashFound = false;
                hash="";
                red = 0;
                while(looking!=1000 && !hashFound){
                    hash=save;
                    //Calcul réduction successive, looking le degré max
                    for (int nbre=0 ; nbre<=looking ; nbre++) {
                        sb = new StringBuilder();
                        red = Reduction.reduction(StringToByteArray(hash), 999-looking+nbre); //Réduction hash
                        tmp = digest.digest(String.format("%08d",red).getBytes()); //Hash du potentiel password
                        //System.out.println("R"+(999-looking+nbre)+" appliqué à "+hash+" -> "+red);
                        for (byte b : tmp)
                        {
                            sb.append(String.format("%02X", b)); //Mise en place du prochain hash
                        }
                        hash = sb.toString();
                    }

                    looking++;
                    //Recherche dans la table
                    for (i = 0; i < 165901; i++) {
                        if (table[i].p999 == red) {
                            //System.out.println("FOUND "+"R"+(999-looking+1));
                            hashFound = true;
                            break;
                        }
                    }
                }

                // Si on est sortis du while à cause d'une profondeur trop grande
                if (looking==1000) {
                    throw new Exception("PASS NOT FOUND");
                }

                //i contient l'indice de la table
                px = table[i].px;
                py=0;
                String chaine;
                byte[] passBytes;
                // Parcours dans la table depuis px pour aller chercher une réduction correspondante
                for (int j = 0; j <= 999-looking; j++) {
                    chaine = String.format("%08d", px);
                    passBytes = digest.digest(chaine.getBytes());
                    py = Reduction.reduction(passBytes, j);
                    px=py;
                }
                // Potentiel mot de passe
                System.out.println("Pass is : "+py+" ?");
                StringBuilder sb2 = new StringBuilder();

                // Formation du hash
                for (byte b : digest.digest(String.format("%08d", py).getBytes()))
                {
                    sb2.append(String.format("%02X", b));
                }

                // Comparaison des hashs pour confirmation
                if(sb2.toString().equals(save)){
                    long endTime = System.currentTimeMillis();

                    System.out.println("Pass cracked : "+py);
                    System.out.println("Inital hash : "+save);

                    System.out.println("Temps d'exécution : "+(endTime-startTime)/1000 +"s");
                    cracked=true;
                }

            }
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
