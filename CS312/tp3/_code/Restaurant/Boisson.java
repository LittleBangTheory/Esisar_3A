package Restaurant;


public class Boisson implements Consommable{
    private String nom;
    private int prix; // en centimes d'euros
    private int volume; // en centilitres
    
    /** Constructeur de la classe Boisson
     * @param nom nom de la boisson
     * @param prix prix de la boisson en centimes
     * @param volume volume de la boisson en centilitres
     */
    public Boisson(String nom, int prix, int volume){
	    this.nom = nom;
    	this.prix = prix;
	    this.volume = volume;
    }
	
    /** Constructeur de la classe Boisson
     * @param nom nom de la boisson
     * @param volume volume de la boisson en centilitres
     */
    public Boisson(String nom, int volume){
	    this(nom, 0, volume);
    }

    // Getters
    public int getVolume(){
	    return volume;
    }

    public String getNom(){
    	return this.nom;
    }
	
    public int getPrix(){
    	return this.prix;
    }

    // Impression de la boisson
    public String toString(){
    	return this.nom + " (" + this.prix + " centimes)";
    }
}
