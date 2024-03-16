package FSMIO;

public class SimpleTransitionFunction {
    private String stateNames[];
    private char inputs[];
    private String nextState[][];
    private int output[][];


	/**
	 * Constructeur de la classe SimpleTransitionFunction
	 * @param stateNames : tableau des noms des états
	 * @param inputs : tableau des entrées
	 * Transitions :
	 	* t(s1, a) = (s1, 0) ; t(s1, b) = (s3, 0)
	 	* t(s2, a) = (s1, 0) ; t(s2, b) = (s2, 1)
	 	* t(s3, a) = (s2, 1) ; t(s3, b) = (s3, 1)
	 * Liste des etats : [s1, s2, s3] -> [0, 1, 2]
	 * Liste des entrées : [a, b]-> [0, 1]
	*/
    public SimpleTransitionFunction(String stateNames[], char inputs[]) {
		this.stateNames = stateNames;
		this.inputs = inputs;
		int stateNameslen = stateNames.length;
		int inputslen = inputs.length;

		nextState = new String[stateNameslen][inputslen];
		output = new int[stateNameslen][inputslen];
		
		//TODOK (V1.Q2) 2- initialize nextState/output with empty string or -1 value (4 lines)
		
		for(int i = 0; i < stateNameslen; i++){
			for(int j = 0; j < inputslen; j++){
				nextState[i][j] = "";
				output[i][j] = -1;
			}
		}
		//To debug, you can uncomment the following line
		System.out.println("transition table of size "+stateNames.length+" x "+inputs.length);
    }

    // Cherche l'index d'un état donné par sa chaîne.
    private int getStateIndex(String etat) throws Exception{
		int i=0;
		while(i < this.stateNames.length
			&& !this.stateNames[i].equals(etat))
			i++;
		if (i == this.stateNames.length)
			throw new Exception("Etat inexistant");
		return i;
    }

    // identiquement...
    private int getInputIndex(char input) throws Exception{
		int i=0;
		while(i < this.inputs.length && this.inputs[i] != input)
			i++;
		if (i == this.inputs.length) throw new Exception("Input inexistante");
		return i;
    }
	
    public void addTransition(String fromStateName, char input, String nomEtatDest, int output) throws Exception{
		try{
			// On recupère les indices de l'état et de l'entrée dans les tableaux stateNames et inputs
			int i = getStateIndex(fromStateName);
			int j = getInputIndex(input);

			// On teste la validé de l'état nomEtatDest
			getStateIndex(nomEtatDest);

			// On teste la validé de l'output
			if (output != 0 && output != 1)	throw new Exception("Output invalide");

			this.nextState[i][j] = nomEtatDest;
			this.output[i][j] = output;
		}
		catch(Exception e) {
			System.out.println("/!\\" + e + " /!\\");
		}
    }

    public String getNextState(String fromStateName, char input){
		int i,j;
		if(fromStateName.equals("s1"))i=0; // Etat s1
		else if(fromStateName.equals("s2"))i=1; // s2
		else i=2; // ou s3
		
		if (input=='a') j=0; // On a un a
		else j=1; // ou un b
		return nextState[i][j];
    }


    public int getOutput(String fromStateName, char input){
		int i,j;
		if(fromStateName.equals("s1"))i=0; // Etat s1
		else if(fromStateName.equals("s2"))i=1; // s2
		else i=2; // ou s3
		
		if (input=='a') j=0; // On a un a
		else j=1; // ou un b
		return output[i][j];
	}
}
