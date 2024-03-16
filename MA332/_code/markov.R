# Calcul de de l'état de la population
nouvel_etat <- function(etat_initial) {
  # Génération d'une probabilité aléatoire entre 0 et 1
  p <- runif(1, min = 0, max = 1)
  etat_fin <- 0
  # Si l'état initial est 0 (sain et non vacciné)
  if (etat_initial == 0) {
    # Probabilité d'être malade
    if (p <= 0.15) {
      etat_fin <- 2
    # Probabilité d'être vacciné
    } else if (p > 0.15 && p <= 0.35) {
      etat_fin <- 1
    # Probabilité de rester saint et non vacciné
    } else {
      etat_fin <- 0
    }
  # Si l'état initial est 1 (vacciné ou immunisé)
  } else if (etat_initial == 1) {
    etat_fin <- 1
  # Si l'état initial est 2 (malade)
  } else if (etat_initial == 2) {
    # Probabilité de mourir
    if (p <= 0.1) {
      etat_fin <- 3
    # Probabilité de guérir
    } else if (p > 0.1 && p <= 0.4) {
      etat_fin <- 1
    # Probabilité de rester malade
    } else {
      etat_fin <- 2
    }
  # Si l'état initial est 3 (mort)
  } else {
    etat_fin <- 3
  }
  return(etat_fin)
}

# Fait évoluer l'état d'une population
nouvel_etat_population <- function(population) {
  # Applique la fonction nouvel_etat à chaque élément de la population
  for (i in seq_along(population)){
    population[i] <- nouvel_etat(population[i])
  }
  return(population)
}

# Calcul de la proportion de chaque état dans la population
total_population <- function(population) {
  # Declaration d'un tableau de taille 4 contenant les etats
  etats <- rep.int(0, 4)

  # Compte le nombre de chaque état dans la population
  for (i in population){
    if(i==1) etats[1]=etats[1]+1
    else if(i==2) etats[2]=etats[2]+1
    else if(i==3) etats[3]=etats[3]+1
    else etats[4]=etats[4]+1
  }

  # Divise par la taille de la population pour avoir la proportion
  for (i in seq(1:4)) {
    etats[i] <- etats[i] / length(population)
  }

  #cat(sprintf("Nombre de :\n- Personnes saines : %s\n- Personnes vaccinées : %s\n- Personnes malades : %s\n- Personnes mortes: %s \n", etats[1], etats[2], etats[3], etats[4])) # nolint

  return(etats)
}

# Run la simulation pour un nombre de nbr_semaines et une taille de population données # nolint
simulation <- function(nbr_semaines, population) {
  # Création d'une liste de 0 de taille population
  liste_population <- rep.int(0, population)

  # Simulation de 20 simulations
  for (i in seq(1:nbr_semaines)) {
   # Fait évoluer la population
    liste_population <- nouvel_etat_population(liste_population)
  }
  etats_courants <- total_population(liste_population)
  return(etats_courants)
}

# Calcul de la moyenne de chaque état pour un nombre de simulations données
moyenne <- function(nbre_simu){
  res <- c()
  moy_0 <- 0
  moy_1 <- 0
  moy_2 <- 0
  moy_3 <- 0
  valeur_retour <- c()
  #On simule
  for (sim in seq(1:nbre_simu)) {
    res <- c(res, simulation(5, 10000))
  }

  #Etat 0
  j <- 1/4
  while(j <= nbre_simu){
    moy_0 <- moy_0 + res[j*4]
    #cat(sprintf("---%s\n", res[j*4]))
    j <- j + 1
  }
  #Etat 1
  j <- 2/4
  while(j <= nbre_simu){
    moy_1 <- moy_1 + res[j*4]
    #cat(sprintf("---%s\n", res[j*4]))
    j <- j + 1
  }
  #Etat 2
  j <- 3/4
  while(j <= nbre_simu){
    moy_2 <- moy_2 + res[j*4]
    #cat(sprintf("---%s\n", res[j*4]))
    j <- j + 1
  }
  #Etat 3
  j <- 4
  while(j <= nbre_simu){
    moy_3 <- moy_3 + res[j*4]
    #cat(sprintf("---%s\n", res[j*4]))
    j <- j + 1
  }
  valeur_retour <- c(valeur_retour,moy_0/nbre_simu)
  valeur_retour <- c(valeur_retour,moy_1/nbre_simu)
  valeur_retour <- c(valeur_retour,moy_2/nbre_simu)
  valeur_retour <- c(valeur_retour,moy_3/nbre_simu)
  return(valeur_retour)
}

choice <- readline(prompt = "Générer les resultats de 20 simulations pour 5 semaines et 10000 personnes ? (y/n) : ") # nolint

if (choice != "n") {
  cat("Génération des resultats de 20 simulations pour 5 semaines et 10000 personnes\n") # nolint
  etats <- moyenne(20)
  # Affichage des résultats
  cat(sprintf("Nombre de :\n- Personnes saines : %s\n- Personnes vaccinées : %s\n- Personnes malades : %s\n- Personnes mortes: %s \n", etats[1], etats[2], etats[3], etats[4])) # nolint
}


cat("------------------------------------------\n") # nolint

# Simulation du temps nécessaire pour que toutes les personnes d'une population soient mortes ou vaccinées # nolint
tps_mort_vax <- function(population) {
  tps_total <- 0
  for (i in seq(1:population)){
    etat <- 0
    tps_courant <- 0
    while ((etat != 1) && (etat != 3)) {
      etat <- nouvel_etat(etat)
      tps_courant <- tps_courant + 1
    }
    tps_total <- tps_total + tps_courant
  }
  return(tps_total / population)
}

choice <- readline(prompt = "Simuler le temps moyen de mort/vaccination ? (y/n) : ") # nolint
if (choice != "n") {
  tps <- tps_mort_vax(10000)
  cat(sprintf("Temps moyen toutes les personnes d'une population de 10 000 personnes soient mortes ou vaccinées : %s semaines\n", tps)) # nolint
}

cat("------------------------------------------\n") # nolint

proba_mort_robert <- function(nbr_simus) {
  mort <- 0
  for (i in seq(1:nbr_simus)){
    etat <- 0
    # Tant que robert n'est ni mort ni vacciné
    while (etat != 1 && etat != 3) {
      # On génère un nouvel état
      etat <- nouvel_etat(etat)
    }
    # Si robert est mort
    if (etat == 3) {
      mort <- mort + 1
    }
  }
  return((mort / nbr_simus) * 100)
}

choice <- readline(prompt = "Simuler la probabilité que Robert meurt ? (y/n) : ") # nolint
if (choice != "n") {
  proba_mort <- proba_mort_robert(10000)
  cat(sprintf("Probabilité que Robert meurt sur 10 000 simulations : %s %%\n", proba_mort)) # nolint
}