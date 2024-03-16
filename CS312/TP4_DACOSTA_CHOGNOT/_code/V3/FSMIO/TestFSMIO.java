package FSMIO;
import java.util.*;
import java.lang.*;

public class TestFSMIO {

    public static void main(String argv[]){
	State s1, s2, s3, s4;
	s1 = new State("s1");
	s2 = new State("s2");
	s3 = new State("s3");
	s4 = new State("s4");
	
	ArrayList<State> states = new ArrayList<State>();
	states.add(s1);
	states.add(s2);
	states.add(s3);
	states.add(s4);
	

	FSMIO<String, Integer> fsm = new FSMIO<String, Integer>(states, s1);

	// remarquez que l'ordre des arguments n'est pas le même que la V1. Pourquoi ?
	fsm.addTransition(s1, "a", 0, s2);
	fsm.addTransition(s1, "b", 0, s3);
	fsm.addTransition(s2, "a", 0, s3);
	fsm.addTransition(s2, "b", 1, s2);
	fsm.addTransition(s3, "a", 0, s4);
	fsm.addTransition(s3, "b", 0, s3);
	fsm.addTransition(s4, "a", 1, s2);
	fsm.addTransition(s4, "b", 1, s3);

	System.out.println("\n##### Affichage des transition de l'automate n°1 : #####\n"+fsm+"#################");

	
	// TODOK : Play with this automata, and add another one.
	System.out.println("##### Exécution avec la chaine : \"aabbaaba\" #####");
	String to_read="aabbaaba";
	String out="";
	for (int i = 0; i < to_read.length(); i++) {
		out+= fsm.getCurrentState().getName()+"--("+to_read.charAt(i)+"/"+fsm.doTransition(String.valueOf(to_read.charAt(i)))+")--";
	}
	out += fsm.getCurrentState()+"--END";
	System.out.println(out+"\n#################");

	// 2eme automate :

	FSMIO<Integer,Integer> fsm2 = new FSMIO<Integer,Integer>(states,s1);
	fsm2.addTransition(s1, 0, 0, s4);
	fsm2.addTransition(s1, 1, 1, s3);
	fsm2.addTransition(s1, 2, 5, s1);
	fsm2.addTransition(s2, 0, 5, s2);
	fsm2.addTransition(s2, 1, 2, s3);
	fsm2.addTransition(s2, 2, 2, s4);
	fsm2.addTransition(s3, 0, 1, s2);
	fsm2.addTransition(s3, 1, 2, s3);
	fsm2.addTransition(s3, 2, 3, s2);
	fsm2.addTransition(s4, 0, 1, s2);
	fsm2.addTransition(s4, 1, 3, s3);
	fsm2.addTransition(s4, 2, 4, s1);
	System.out.println("\n##### Affichage des transition de l'automate n°2 : #####\n"+fsm2+"#################");
	System.out.println("##### Exécution avec l'entrée : \"0120121120211\" #####");
	to_read="0120121120211";
	out="";
	for (int i = 0; i < to_read.length(); i++) {
		out+= fsm2.getCurrentState().getName()+"--("+to_read.charAt(i)+"/"+fsm2.doTransition(to_read.charAt(i)-48)+")--";
	}
	out += fsm2.getCurrentState()+"--END";
	System.out.println(out+"\n#################");

	// BONUS
	State s0, e0, e1, e2, e3, e4; // s1 s2 s3 s4 déja définis plus haut
	//Définition des états
	s0 = new State("s0");
	e0 = new State("e0");
	e1 = new State("e1");
	e2 = new State("e2");
	e3 = new State("e3");
	e4 = new State("e4");
	// Ajout
	states.add(s0);
	states.add(e0);
	states.add(e1);
	states.add(e2);
	states.add(e3);
	states.add(e4);
	// Création automate + transistions
	FSMIO<Integer,Integer> fsm3 = new FSMIO<Integer,Integer>(states,s0); // s0 état initial
	fsm3.addTransition(s0, 0, 0, s1);
	fsm3.addTransition(s0, 1, 0, e1);

	fsm3.addTransition(s1, 0, 0, s2);
	fsm3.addTransition(s1, 1, 0, e1);

	fsm3.addTransition(s2, 0, 0, s3);
	fsm3.addTransition(s2, 1, 0, e1);

	fsm3.addTransition(s3, 0, 1, s4);
	fsm3.addTransition(s3, 1, 0, e1);

	fsm3.addTransition(s4, 0, 1, s4);
	fsm3.addTransition(s4, 1, 0, e1);

	fsm3.addTransition(e1, 0, 0, s1);
	fsm3.addTransition(e1, 1, 0, e2);

	fsm3.addTransition(e2, 0, 0, s1);
	fsm3.addTransition(e2, 1, 0, e3);

	fsm3.addTransition(e3, 0, 0, s1);
	fsm3.addTransition(e3, 1, 1, e4);

	fsm3.addTransition(e4, 0, 0, s1);
	fsm3.addTransition(e4, 1, 1, e4);
	//EXEC
	System.out.println("\n##### Affichage des transition de l'automate n°3 : #####\n"+fsm3+"#################");
	System.out.println("##### Exécution avec l'entrée : \"001000011111000\" #####");
	to_read="001000011111000";
	out="";
	for (int i = 0; i < to_read.length(); i++) {
		out+= fsm3.getCurrentState().getName()+"--("+to_read.charAt(i)+"/"+fsm3.doTransition(to_read.charAt(i)-48)+")--";
	}
	out += fsm3.getCurrentState()+"--END";
	System.out.println(out+"\n#################");
	//V3 PART
	FSMIO<String,String> fsm4 = new FSMIOString("1.fsm").getFSM();
	System.out.println("\n##### Affichage des transition de l'automate n°4 (BONUS) : #####\n"+fsm4+"#################");
    }
}
