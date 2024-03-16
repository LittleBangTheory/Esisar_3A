package Restaurant;

public class TestRestaurant {

    public TestRestaurant() {
    }
    
    /**
     * @param args the command line arguments
     * @throws Exception the exception thrown by Menu
     */
    public static void main(String args[]) throws Exception{
        // Declaration
        Boisson b1 = new Boisson("Coca", 200, 200);
        Boisson b2 = new Boisson("Ice Tea", 180, 200);
        Boisson b3 = new Boisson("Fanta", 150, 200);
        PlatPrincipal p1 = new PlatPrincipal("Poulet", 1000);
        PlatPrincipal p2 = new PlatPrincipal("Steak", 1500);
        PlatPrincipal p3 = new PlatPrincipal("Poisson", 1000);
        Entree e1 = new Entree("Salade", 500);
        Entree e2 = new Entree("Soupe", 400);
        Entree e3 = new Entree("Houmous", 300);
        Dessert d1 = new Dessert("Tarte", 700);
        Dessert d2 = new Dessert("Glace", 600);
        Dessert d3 = new Dessert("Creme Brulee", 800);
        
        /*
        System.out.println("########## TEST CLASSES ##########");
        System.out.println(b1);
        System.out.println(p1);
        System.out.println(e1);
        System.out.println(d1);
        System.out.println("########## END ##########\n");
        */

        /* Verification du prix du menu 
        * try{} permet de tester si Menu déclenche (throws) une exception (de type Exception)
        * catch{} permet de capturer l'exception et de l'afficher
        * finally{} contient du code à éxécuter dans tous les cas
        * On pourrait déclarer une classe d'exception spécifique pour le prix du menu
        */
        System.out.println("########## TEST MENU ##########");
        System.out.println("Menu avec prix excessif :");
        try{
            // Test Menu
            Menu m1 = new Menu(3000, e1, p1, d1, b1);
            System.out.println(m1);
        }
        catch(Exception e){
            System.out.println("/!\\ "+e + "\nOn commande séparement !");
        }

        System.out.println("\nMenu avec prix correct :");
        try{
            // Test Menu
            Menu m2 = new Menu(2000, e1, p1, d1, b1);
            System.out.println(m2);
        }
        catch(Exception e){
            System.out.println("/!\\ "+e + "\nOn commande séparement !");
        }
        System.out.println("########## END ##########\n");

        // Test Carte
        System.out.println("########## TEST CARTE ##########");
        Carte c = new Carte("Restaurant/CARTE"); // On appelle Carte avec des elements du fichier CARTE. Question bonus.
        Menu m1 = new Menu(1800, e1, p1, d1, b1);
        Menu m2 = new Menu(2000, e2, p2, d2, b2);
        Menu m3 = new Menu(2200, e3, p3, d3, b3);

        // Ajout des items à la carte
        c.addBoisson(b1);
        c.addBoisson(b2);
        c.addBoisson(b3);

        c.addEntree(e1);
        c.addEntree(e2);
        c.addEntree(e3);

        c.addDessert(d1);
        c.addDessert(d2);
        c.addDessert(d3);

        c.addBoisson(new Boisson("Coca", 10)); //N'est pas ajouté car Coca existe déjà
        c.addBoisson(b2);
        c.addBoisson(b3);

        c.addPlatPrincipal(p1);
        c.addPlatPrincipal(p2);
        c.addPlatPrincipal(p3);

        // Ajout des menus à la carte
        c.addMenu(m1);
        c.addMenu(m3);
        c.addMenu(m2);
        System.out.println(c);
        System.out.println("########## END ##########\n");

        // Test Commande
        System.out.println("########## TEST COMMANDE ##########");
        Commande commande = new Commande();
        commande.addItem(d1);
        commande.addItem(e1);
        commande.addItem(b1);
        commande.addItem(p1);
        commande.addItem(new Boisson("Fanta", 100, 50));
        System.out.println(commande);
        System.out.println("pour un prix de " + c.calculerPrixCommande(commande)/100 + " €");
        System.out.println("########## END ##########\n");
    }
    
}
