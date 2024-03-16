package Restaurant;


public class Plat implements Consommable{
    private String nom;
    private int prix; // en centimes d'euros

    /** Constructeur de la classe Plat
     * @param nom nom du plat
     * @param prix prix du plat en centimes
     */
    public Plat(String nom, int prix){
	    this.nom = nom;
    	this.prix = prix;
    }

    // Getters
    public String getNom(){
    	return this.nom;
    }
	
    public int getPrix(){
    	return this.prix;
    }

    // Impression du plat
    public String toString(){
    	return this.nom + " (" + this.prix + " centimes)";
    }


}
