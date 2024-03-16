package FSMIO;
import java.util.Arrays;

public class SimpleFSMIO {
    private SimpleTransitionFunction t;
    private String initialState;
    private String currentState;
    private String states[];
    private char inputs[];

    // Yes, these methods would need a bit more documentation.
    public SimpleFSMIO(String states[], String initialState, char inputs[]) {
		this.t = new SimpleTransitionFunction(states, inputs);
		this.states = states;
		this.inputs = inputs;
		this.currentState = this.initialState = initialState;
    }
    
    public void addTransition(String fromState, char inputVal, String destState, int output){

		boolean fromStateValid = false;
		boolean destStateValid = false;
		boolean inputValid = false;
		
		for(int ie = 0; ie < this.states.length && (!fromStateValid || !destStateValid); ie++){
			if(fromState.equals(this.states[ie])) fromStateValid = true;
			if(destState.equals(this.states[ie])) destStateValid = true;
		}
			
		for(int i = 0; i < this.inputs.length && !inputValid; i++){
			inputValid = (inputVal == this.inputs[i]);
		}
			
		if (fromStateValid && destStateValid && inputValid){
			try{
			t.addTransition(fromState, inputVal, destState, output);}
			catch(Exception e){
				System.out.println("/!\\" + e + " /!\\");
			}
		}
    }
	
	/** 
	 * Gère une transition de l'automate à partir de l'état courant et d'une valeur d'input
	 * @param val La valeur d'input
	 * @return L'output de la transition
	 */
    public int doTransition(char val) throws Exception{
		int output = -1;
		try{
			checkInput(val);
			output = getOutput(this.currentState, val);
			this.currentState = getNextState(this.currentState, val);
		}
		catch (Exception e){
			System.out.println("/!\\" + e + " /!\\");
			throw e;			
		}
		return output;
    }

    
	/**
	 * Réinitialise l'automate à son état initial
	*/
    public void reset(){
		this.currentState = this.initialState;
    }
	
	/**
	 * @return Retourne l'état courant de l'automate
	 */
    public String getCurrentState(){
		return this.currentState;
    }
	
	/**
	 * @return Le orochain état de l'automate 
	 * @param s L'état courant
	 * @param e La valeur d'input
	 */
    public String getNextState(String s, char i) throws Exception{
		String nextState = null;
		try{
			checkState(s);
			checkInput(i);
			nextState = this.t.getNextState(s, i);
		}
		catch (Exception e){
			System.out.println("/!\\" + e + " /!\\");
			throw e;
		}
		return(nextState);
    }

	/**
	 * @return L'output de la transition (-1 si la transition n'existe pas)
	 */
    public int getOutput(String s, char i) throws Exception{
		int output = -1;
		try{
			checkState(s);
			checkInput(i);
			output = this.t.getOutput(s, i);
		}
		catch (Exception e){
			System.out.println("/!\\" + e + " /!\\");
			throw e;
		}
		return(output);
	
    }

	/**
	 * Vérifie si une valeur d'input est valide
	 * @param val La valeur d'input
	 */
	private void checkInput(char val) throws Exception{
		boolean inArray = false;
		for(int i = 0; i < this.inputs.length; i++){
			if(val == this.inputs[i]){
				inArray = true;
			}
		}
		if(!inArray) throw new Exception("Invalid input : " + val);
	}

	/**
	 * Vérifie si un état est valide
	 * @param state L'état
	 */
	private void checkState(String state) throws Exception{
		boolean inArray = false;
		for(int i = 0; i < this.states.length; i++){
			if(state == this.states[i]){
				inArray = true;
			}
		}
		if(!inArray) throw new Exception("Invalid state : " + state);
	}
}
