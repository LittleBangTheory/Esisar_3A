package Restaurant;

import java.util.ArrayList;
import java.util.Iterator;
import java.io.*;

public class Carte {
    private ArrayList<Consommable> entrees;
    private ArrayList<Consommable> platsPrincipaux;
    private ArrayList<Consommable> desserts;
    private ArrayList<Consommable> boissons;
	
    private ArrayList<Menu> menus;

    public Carte() {
	    entrees = new ArrayList<Consommable>();
	    platsPrincipaux = new ArrayList<Consommable>();
	    desserts = new ArrayList<Consommable>();
	    boissons = new ArrayList<Consommable>();	
	    menus = new ArrayList<Menu>();
    }
    public Carte(String fileName) { // Deuxième constructeur a appeler avec un nom de fichier contenant une carte.
	    entrees = new ArrayList<Consommable>();
	    platsPrincipaux = new ArrayList<Consommable>();
	    desserts = new ArrayList<Consommable>();
	    boissons = new ArrayList<Consommable>();	
	    menus = new ArrayList<Menu>();
        try {
            System.out.println("########## CHARGEMENT DE LA CARTE DEPUIS LE FICHIER : "+fileName+" ##########");
            FileReader FILE = new FileReader(fileName);
            BufferedReader buff = new BufferedReader(FILE);
            String ligne = buff.readLine();
            while (ligne!=null) { // Tant que toute les lignes n'ont pas été lues
                String elem[] = ligne.split(" "); // Les éléments sont séparés par un espace, on les recupère tous et par ligne
                switch (elem[0]) {
                    case "Entree": // Switch sur le premier qui nous informe sur la nature de l'objet. Si l'objet n'est pas reconnu, on va dans default et on ne fait rien.
                        this.addEntree(new Entree(elem[1], Integer.parseInt(elem[2]))); // Integer.parseInt permet de transformer un String en int, indispensable pour le constructeur.
                        break;
                    case "Plat":
                        this.addPlatPrincipal(new PlatPrincipal(elem[1], Integer.parseInt(elem[2])));
                        break;
                    case "Boisson":
                        this.addBoisson(new Boisson(elem[1], Integer.parseInt(elem[2]), Integer.parseInt(elem[3])));
                        break;
                    case "Dessert":
                        this.addDessert(new Dessert(elem[1], Integer.parseInt(elem[2])));
                        break;
                    default:
                        break;
                }

                ligne = buff.readLine();
            }
            FILE.close();
            System.out.println("##### FIN DU CHARGEMENT DES CONSOMMABLES DEPUIS LE FICHIER CARTE #####");
        } catch (Exception e) { // Erreur ouverture. 
            System.out.println(e);
            System.out.println("#################");
        }
    }
	
    public void addEntree(Entree e){
	if (verifCarte(e)) this.entrees.add(e);
    }
	
    public void addPlatPrincipal(PlatPrincipal p){
	if (verifCarte(p)) this.platsPrincipaux.add(p);
    }
	
    public void addDessert(Dessert d){
	if (verifCarte(d)) this.desserts.add(d);
    }
	
    public void addBoisson(Boisson b){
	if (verifCarte(b)) this.boissons.add(b);
    }
	
    public void addMenu(Menu m){
	if (verifMenu(m)) {
	    this.menus.add(m);
        System.out.println("Menu added !");
        }
    }
	
    public ArrayList<Consommable> getEntrees(){
	return this.entrees;
    }
	
    public ArrayList<Consommable> getPlatsPrincipaux(){
	return this.platsPrincipaux;
    }
	
    public ArrayList<Consommable> getDesserts(){
	return this.desserts;
    }
	
    public ArrayList<Consommable> getBoissons(){
	return this.boissons;
    }
	
    // Vérifie qu'il n'y a pas d'homonymes dans la carte
    // utiliser equals et des iterateurs
    // Comme une carte est composée d'entrees, plats, boissons et desserts,
    // l'idée est de confronter le consommable à ajouter à tous ces élements.
    // On s'arrête si l'objet est déjà dans la carte.
    private boolean verifCarte(Consommable c){
        Iterator<Consommable> it1 = entrees.iterator();
        while (it1.hasNext()) {
            if (c.getNom().equals(it1.next().getNom())) return false;
        }
        Iterator<Consommable> it2 = platsPrincipaux.iterator();
        while (it2.hasNext()) {
            if (c.getNom().equals(it2.next().getNom())) return false;
        }
        Iterator<Consommable> it3 = desserts.iterator();
        while (it3.hasNext()) {
            if (c.getNom().equals(it3.next().getNom())) return false;
        }
        Iterator<Consommable> it4 = boissons.iterator();
        while (it4.hasNext()) {
            if (c.getNom().equals(it4.next().getNom())) return false;
        }
        System.out.println(c.getNom()+" added !");
	    return true;
    }

    // Vérifie que les plats et boissons du menu sont bien dans la carte
    // verifCarte renvoie true si l'objet n'est pas dans la carte.
    // Ici, si l'objet n'est pas dans la carte, on ne verifie pas le menu.
    private boolean verifMenu(Menu m){
        ArrayList<Consommable> item_menu = m.getItems();
        Iterator<Consommable> it = item_menu.iterator();
        while(it.hasNext()){
            if(verifCarte(it.next()))return false; // On utilise la recherche de verifCarte pour savoir si les items sont présents.
        }
        return true;
    }

    
    /* Calcule le prix de la commande. A priori, ce prix est la somme des prix des items 
     * SAUF si une partie de ces items constituent un menu; dans ce cas, le tarif menu s'applique pour ces items.
     */
    // L'idée ici est de prendre chaque menu est de voir si les elements du menu sont dans la commande.
    // SI oui : On retire les elemtents du menu de la commande, on les remplace par le prix et on calcule ensuite le prix du reste.
    // SI non : On calcule le prix de chaque elements et on fait la somme.
    public int calculerPrixCommande(Commande c){
        Commande tmp = c;
        int res=0;
        ArrayList<Consommable> consommables_menu = tmp.getItemsCommandes();
        for (Menu menu : menus) {
            if (consommables_menu.containsAll(menu.getItems())) {
                consommables_menu.removeAll(menu.getItems());
                res=res+menu.getPrix();
                break;
            }
        }
        for (Consommable elem : consommables_menu) {
            res = res+elem.getPrix();
        }
        return res;
    }
	
    public String toString(){
        String message = "\n--CARTE--\n";

        // Ajouter les elements de l'entree a la variable entre_string
        String entree_string = "\nEntrées : \n";
        for (int i = 0; i < entrees.size(); i++) {
            entree_string += entrees.get(i).getNom() + "\t" + entrees.get(i).getPrix()/100 + "€\n" ;
        }

        // Ajouter les elements des plats principaux a la variable plats_string
        String plats_string = "\nPlats principaux : \n";
        for (int i = 0; i < platsPrincipaux.size(); i++) {
            plats_string += platsPrincipaux.get(i).getNom() + "\t" + platsPrincipaux.get(i).getPrix()/100 + "€\n" ;
        }

        // Ajouter les elements des desserts a la variable desserts_string
        String desserts_string = "\nDesserts : \n";
        for (int i = 0; i < desserts.size(); i++) {
            desserts_string += desserts.get(i).getNom() + "\t" + desserts.get(i).getPrix()/100 + "€\n" ;
        }

        // Ajouter les elements des boissons a la variable boissons_string
        String boissons_string = "\nBoissons : \n";
        for (int i = 0; i < boissons.size(); i++) {
            boissons_string += boissons.get(i).getNom() + "\t" + boissons.get(i).getPrix()/100 + "€\n" ;
        }

        // Retourner le string a print
        return message + entree_string + plats_string + desserts_string + boissons_string;
    }
}
