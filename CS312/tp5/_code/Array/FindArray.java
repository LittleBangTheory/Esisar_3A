package tp5._code.Array;

import java.util.NoSuchElementException;

public class FindArray {
    public static void main(String[] args) {
       
       /* Object tab[] = new Object[3];
        //création de 3 objets
        String a = "aa";
        int b = 4;
        char c = 'd';
        int d = 5;
        //Assignation
        tab[0]=a;
        tab[1]=b;
        tab[2]=c;   */
        Object[] tab = {"aa", 4, 'd', 5};
        //Test
        try {
            int i = find_element(tab, "aa");
            System.out.println("Elément trouvé à l'indice : "+i);
        } catch (NoSuchElementException e) {
            System.out.println("/!\\"+e+"/!\\");
        }

        String[] liste = {"aaa", "nzjkqs", "e458", "sedf", "d789"};

        try{
            int i = int_in_array(liste);
            System.out.println("Premier string castable en int : "+i);
        } catch (NoSuchElementException NE){
            System.out.println("/!\\"+NE+"/!\\");
        }
        
    }

    /* 
        Methode qui recherche un élément dans un tableau. 
        Si l'objet est trouvé, la fonction retourne la première position où se trouve l'objet
        Sinon elle lance une exception (on cherchera une exception existante adéquate)
    */
    public static int find_element(Object[] tab, Object obj) throws NoSuchElementException {
        for (int i = 0; i < tab.length; i++) {
            if (tab[i].equals(obj)) {
                return i;
            }
        }
        throw new NoSuchElementException ("Element " + obj + " not found");
    }

    /* Méthode qui cherche dans un tableau de chaînes la première chaîne qui peut être lue comme un entier à l'aide de la méthode `ParseInt`. 
        Lance une exception adéquate si il n'y a pas de telle chaîne
     */
    public static int int_in_array(String[] tab) throws NoSuchElementException{
        int result;
        for (int i = 0; i < tab.length; i++){
            try{
                result = Integer.parseInt(tab[i]);
                return(result);
            }
            catch (NumberFormatException NF){
            }
        }
        throw new NoSuchElementException ("No string castable to int");
    }

    
}
