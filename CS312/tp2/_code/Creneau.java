import java.util.*;

public class Creneau {
    private int annee;
    private int mois; // 1 à 12
    private int jour; // 1 à 31
    private int heure; // 0 à 23
    private int minute; // 0 à 59
    private int duree; // en minutes, maximum 210
    
    private Salle salle;
    private Activite activite;
    
    public Creneau(int a, int m, int j, int h, int min, int d, Salle s, Activite ac) {
        
    annee = a; mois = m; jour = j;
    heure = h; minute = min; duree = d;
    salle = s;
    activite = ac;
    
    if(!verifCapacite()){
        System.out.println("Capacit� insuffisante !");
        System.exit(1); // On pourrait utiliser un mécanisme d'exception, aussi. 
    }
    if(!verifDuree()){
        System.exit(1);
    }
    if(!verifSalle()){
        System.exit(1);
    }
    }
    
    // Verifie l'adéquation de la salle : la salle affectée doit être une des salles appropriées de l'activité. Indication: utiliser une methode de liste.
    private boolean verifSalle(){
        return activite.getSalles().contains(salle);
    }
    
    // Verifie que la taille de la salle convient à la promo
    private boolean verifCapacite(){
        ArrayList<Groupe> ensemble=activite.getGroupes();
        Iterator<Groupe> it = ensemble.iterator();
        int nbre=0;
        while(it.hasNext()){
            Groupe elem=it.next();
            nbre+=elem.getEffectif();
        }
        return nbre<=salle.getCapacite();
    }
    
    // Verifie que le début et la fin du créneau sont dans la même journée, entre 8h et 19h
    private boolean verifDuree(){
        return heure > 8 && (heure*60 + duree < 19*60);
    }
    
    /**
     * @return la salle associée au créneau
     */
    public Salle getSalle(){
        return salle;
    }
    
    /**
     * @return l'activité associée au créneau
     */
    public Activite getActivite(){
        return activite;
    }
    
    /**
     * @return la durée du créneau en minutes
     */
    public int getDuree(){
        return duree;
    }
    
    /**
     * @return l'heure de début du créneau
     */
    public int getHeure(){
        return heure;
    }

    /**
     * @return la minute de début du créneau (pour une heure donnée)
     */
    public int getMinute(){
        return minute;
    }

    public String toString(){
        return jour + "/" + mois + "/" + annee + " " + heure + ":" + minute +" (" + duree +") : " + 
        activite + " " + salle;
    }

    
    /**
     * Deux créneaux ont une intersection si leurs tranches horaires se chevauchent et :
        * soit ils occupent la même salle
        * soit ils sont associés à des activités ayant des groupes d'élèves en commun
     * @param c
     * @return true si les créneaux ont une intersection, false sinon
    */
    public boolean intersection(Creneau c){
        //Chercher si les créneaux ont une intersection horaire
        if (this.heure*60 + this.minute + this.duree >= c.getHeure()*60 + c.getMinute() || c.getHeure()*60 + c.getMinute() + c.getDuree() >= this.heure*60 + this.minute){
            // Si les créneaux partagent la même salle
            return (this.salle == c.getSalle() || this.activite == c.getActivite());
        }
        // Si les créneaux n'ont pas d'intersection horaire
        return false;
    }    


}
