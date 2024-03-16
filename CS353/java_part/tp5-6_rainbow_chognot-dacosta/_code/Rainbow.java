import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Random;

public class Rainbow {
    public static void main(String[] args) {
        // Déclaration du tableau de Noeuds
        Noeud[] table = new Noeud[1000003];
        // Initialisation du générateur de nombres aléatoires
        Random r = new Random(0);
        // Initialisation d'un ArrayList contenant les p999 déjà trouvés pour chercher plus facilement que dans un tableau de Noeuds
        ArrayList<Integer> PReduc= new ArrayList<Integer>();
        // Initialisation du FileWriter
        FileWriter fw = null;
        
        // Start time
        long time = System.currentTimeMillis();
        // Création de la table
        for (int i = 0; i <= 999999; i++) {
            // Genere un premier nombre entre 0 et 99_999_999
            int px = r.nextInt(100000000); 
            try {
                //On ouvre le fichier si ce n'est pas déjà fait
                if (fw==null) {
                    fw = new FileWriter("table.txt");
                }
                // Calcule le p999 de px
                int p999 = Reduction.calculChaine(px);
                // Insérer seulement si p999 n'est pas déjà dans la table
                if(!PReduc.contains(p999)){
                    // Allocation du Noeud et initialisation de ses attributs
                    table[i] = new Noeud(px,p999);
                    // Ajout de p999 à la liste des p999 déjà trouvés
                    PReduc.add(p999);
                    //Ecriture
                    fw.write(px+","+p999+"\n");
                }
                // Imprimer la progression dans le terminal tous les 10000 Noeuds (environ 1%)
                if(i%10000==0) System.out.println(i/10000+"%. Noeud actuel : "+px+","+p999);
            } catch (Exception e) {
                System.out.println(e);
            }
        }

        // End time
        time = System.currentTimeMillis() - time;
        // Convert to seconds
        time = time/1000;
        // Print
        System.out.println("Temps de génération : "+time+"s, environ " + time/60 + "min");

        // On initialise les 3 derniers à NULL
        table[999999+1]=null;
        table[999999+2]=null;
        table[999999+3]=null;
    }
}
