import java.security.MessageDigest;

/**
 * Reduction
 */
public class Reduction {
    public static void main(String[] args) {
        try{
            int num = 1;
            System.out.println("Chaine réduite obtenue à partir de la chaine \"" + num + "\" : " + String.format("%08d",calculChaine(num)));
        }
        catch(Exception e){System.out.println(e);}
    }

    /** 
     * @param bs Tableau d'octets contenant le hash  
     * @param num Nombre de réductions
     * @return Le résultat de la réduction
     */
    public static int reduction(byte[] bs,int num){
        int res = num;
        int mult = 1;
        for (int i = 0; i <8 ; i++)
        {
            res = res+mult*( (bs[i]+256) % 10);
            mult = mult*10;
        }
        return res % 100_000_000;
    }

    /**
     * @param px Nombre à réduire
     * @return Le résultat des réductions
     * @throws Exception
     */
    public static int calculChaine(int px) throws Exception {
        // Vérification de la validité de px
        if(1>px || px>99999999){
            throw new Exception("px out of bounds !");
        }

        // Initialisation de la chaine et du digest
        String chaine = String.format("%08d", px);
        MessageDigest digest = MessageDigest.getInstance("MD5");

        // Conversion de la chaine en tableau d'octets
        byte[] hash = digest.digest(chaine.getBytes());

        // Première réduction
        int P = reduction(hash, 0);
        int i=1;

        // Réductions suivantes
        while (i!=999+1){
            // Formatage de la réduction en chaine de caractères
            chaine = String.format("%08d", P);
            // Calcul du nouveau hash
            hash = digest.digest(chaine.getBytes());
            // Calcul de la nouvelle réduction
            P = reduction(hash, i);
            i=i+1;
        }
        return P;
    }
}