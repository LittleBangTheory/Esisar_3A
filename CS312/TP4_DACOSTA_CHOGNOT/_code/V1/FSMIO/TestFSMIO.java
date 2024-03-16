package FSMIO;
import java.util.*;
import java.lang.*;

public class TestFSMIO {

	public static void main(String argv[]){

	    String stateNames[] = new String[3];
	    char inputs[] = new char[2];
	    stateNames[0] = "s1";
	    stateNames[1] = "s2";
	    stateNames[2] = "s3";
	    inputs[0] = 'a';
	    inputs[1] = 'b';
		

	    SimpleFSMIO sfsm = new SimpleFSMIO(stateNames, "s1", inputs);
	    sfsm.addTransition("s1", 'a', "s1", 0);
	    sfsm.addTransition("s1", 'b', "s3", 0);
	    sfsm.addTransition("s2", 'a', "s1", 0);
	    sfsm.addTransition("s2", 'b', "s2", 1);
	    sfsm.addTransition ("s3", 'a', "s2", 1);
	    sfsm.addTransition("s3", 'b', "s3", 1);

		sfsm.reset();
		String actual=sfsm.getCurrentState();
		// play string "abbaabbaaa" (test getOutput and getNextState)
		String to_read="abbaabbaaa";
		String res="";
		for (int i = 0; i < to_read.length(); i++) {
			try{
			res+= actual+"--("+to_read.charAt(i)+"/"+sfsm.getOutput(actual,to_read.charAt(i))+")--";}
			catch(Exception e){
				System.out.println("Erreur, lettre non reconnue, fin de l'éxéction");
				break;
			}

			try{
			actual=sfsm.getNextState(actual, to_read.charAt(i));}
			catch(Exception e){
				System.out.println("Erreur, lettre non reconnue, fin de l'éxécution");
				break;
			}
		}
		res+=actual+"--END";
		System.out.println("##### Exécution avec l'entrée : \"abbaabbaaa\" #####");
		System.out.println("Sortie du programme :");
		System.out.println(res);
		System.out.println("#################");
		System.out.println("Sortie théorique : ");
		System.out.println("s1--(a/0)--s1--(b/0)--s3--(b/1)--s3--(a/1)--s2--(a/0)--s1--(b/0)--s3--(b/1)--s3--(a/1)--s2--(a/0)--s1--(a/0)--s1--END");
		System.out.println("#################");


	    sfsm.reset();
	    
	}
}
