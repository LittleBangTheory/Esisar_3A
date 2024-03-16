import java.nio.ByteBuffer;
import java.nio.channels.SeekableByteChannel;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

public class Bloom {
    
    // Fonction de hachage de la chaine de caractères en fonction d'un entier
    private static int hash(String value , int numFonction )
    {
        int m = 200000000;
        int h1 = h1(value);
        int h2 = h2(value);
        int h = ( (h1+numFonction*h2) % m);
        h = (h+m) % m;
        return h;
    }

    private static int h1(String value) {
        char val[] = value.toCharArray();
        int h=0;
        for (int i = 0; i < val.length; i++){
            h = 31 * h + val[i];
        }
        return h;
    }

    private static int h2(String value) {
        char val[] = value.toCharArray();
        int h=0;
        for (int i = 0; i < val.length; i++) {
            h = 57 * (h <<2) + val[i];
        }
        return h;
    }

    // Génération du filtre de bloom
    private static boolean[] generate_bloom(int k) throws Exception{
        System.out.println("Generating bloom with k="+k+"...");
        int m = 200000000;
        // Création d'un tableau de 20000000 de bits
        boolean[] bloomfilter = new boolean[m];
        for (int index = 0; index < bloomfilter.length; index++) {
            bloomfilter[index]=false;
        }

        long start = System.currentTimeMillis();
        SeekableByteChannel sbc = Files.newByteChannel(Paths.get("urls/infected-urls.txt"),StandardOpenOption.READ);
        int count =0;
        // Lecture des 50 000 premiers octets du fichier
        ByteBuffer buf = ByteBuffer.allocate(50000);
        int ret = sbc.read(buf);
        while(ret>0) {
            // Conversion des 50_0000 octets en 1000 chaine de caractères
            for (int i = 0; i < 1000; i++) {
                String str = new String(buf.array(),i*50,50);
                // Suppression des espaces en fin de chaine
                str = str.trim();
                // Affichage de la chaine de caractères sur la console
                //System.out.println(str);

                // Pour chacun des k haches
                for (int j = 0; j < k; j++) {
                    // On met à true la case correspondante dans le filtre de bloom
                    bloomfilter[hash(str, j)] = true;
                }

                count++;
            }
            // Lecture des 50000 octets suivants
            buf.clear();
            ret = sbc.read(buf);
        }
        System.out.println("Nombre d'URL dans le fichier ="+count);
        System.out.println("Temps de génération ="+(System.currentTimeMillis()-start)/1000+"s");
        // Fermeture du fichier
        sbc.close();
        System.out.println("Generation terminated");
        return bloomfilter;
    }

    // Vérification de la présence ou non des URLs dans le filtre de bloom
    private static int bloom(int k, boolean[] bloomfilter,String file) throws Exception{
        System.err.println("Checking valids URLs...");
        long start = System.currentTimeMillis();
        SeekableByteChannel sbc = Files.newByteChannel(Paths.get(file),StandardOpenOption.READ);
        int count =0;
        // Lecture des 50 000 premiers octets du fichier
        ByteBuffer buf = ByteBuffer.allocate(50000);
        int ret = sbc.read(buf);
        while(ret>0) {
            // Conversion des 50_0000 octets en 1000 chaine de caractères
            for (int i = 0; i < 1000; i++) {
                String str = new String(buf.array(),i*50,50);
                // Suppression des espaces en fin de chaine
                str = str.trim();
                // Affichage de la chaine de caractères sur la console
                //System.out.println(str);

                for (int j = 0; j < k; j++) {
                    if(bloomfilter[hash(str, j)] == false){
                        // Si on a une seule case à false, c'est que l'url n'est pas dans le filtre
                        break;
                    } else if(j == k-1){
                        // Si on n'a trouvé aucune case à false, on incrémente le compteur d'url malveillantes
                        count++;
                    }
                }
            }
            // Lecture des 50000 octets suivants
            buf.clear();
            ret = sbc.read(buf);
        }
        System.out.println("Nombre d'URL malveillantes dans le fichier ="+count);
        System.out.println("Temps de verification ="+(System.currentTimeMillis()-start)/1000+"s");
        // Fermeture du fichier
        sbc.close();
        return count;
    }


    public static void main(String[] args) {
        try{
            // Nombre d'essais
            int k_max = 20;

            // Declaration du tableau de resultats
            int[] result = new int[k_max];

            // Pour chaque essai
            for (int k = 1; k <= k_max; k++){
                boolean[] bloomTable=generate_bloom(k);
                result[k-1] = bloom(k,bloomTable,args[0]);

                // Impression de l'avancée
                System.out.println("k="+k+" done");
            }
            System.out.println("Resultats : \n" + result.toString());
            
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
