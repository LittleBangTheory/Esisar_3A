package Restaurant;

import java.util.ArrayList;

public class Menu {
    ArrayList<Consommable> items;
    int prix; // en cents
    int totalItems = 0;


    /** Constructeur de la classe Menu
     * @param prix prix du menu en centimes
     * @param e entree du menu
     * @param p plat principal du menu
     * @param d dessert du menu
     * @param b boisson du menu
     * @throws Exception si le prix du menu est superieur au prix total des items
     */
    public Menu(int prix, Entree e, PlatPrincipal p, Dessert d, Boisson b) throws Exception {
        this.prix = prix;
        this.items = new ArrayList<Consommable>();
        this.items.add(e);
        this.items.add(p);
        this.items.add(d);
        this.items.add(b);
        totalItems=e.getPrix()+p.getPrix()+d.getPrix()+b.getPrix();
        // Verifier que le prix du menu est inferieur ou égal au prix total des items
        if (!verifPrixMenu()){
            System.out.println("Prix items séparés : " + totalItems + " centimes, prix menu : " + this.prix + " centimes");
            throw new Exception("Prix menu excessif");
        }
    }
	
    // Getters
    public ArrayList<Consommable> getItems(){
	    return this.items;
    }
	
    public int getPrix(){
	    return this.prix;
    }

    // Impression du menu
    public String toString(){
	    String message = "Menu compose de ";

        // Ajouter les elements du menu a la variable contenu
        String contenu = "";
        for (int i = 0; i < items.size(); i++) {
            contenu += items.get(i).getNom() + ", ";
        }

        // Retourner le string a print
	    return message + contenu + "pour un prix de " + this.prix + " centimes";
    }

    // Verifier que le prix du menu est inferieur ou égal au prix total des items
    private boolean verifPrixMenu() {
        return prix<totalItems;
    }
}
