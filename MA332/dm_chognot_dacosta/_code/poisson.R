# ls() liste toutes les variables du workspace pour les supprimer
rm(list = ls())

#Definition du lambda
l <- 1 / 3

#Fonction qui génère les instants d'arrivée et les intervalles d'arrivée des voitures # nolint
distribution <- function(tmax, res) {
  #Ecoulement du temps
  t <- 0
  #Initialisation du tableau contenant les instants d'arrivée
  tab <- list()
  #Initialisation du tableau contenant la durée entre les arrivées
  intervalle <- list()
  #Initialisation de l'indice pour stocker les intervalles et les instants d'arriée # nolint
  i <- 1
  #Génération d'une valeur aléatoire à partir d'une distribution exponentielle de paramètre l # nolint
  duree <- rexp(1, l)
  #On augmente le temps total écoulé de la valeur de la durée générée
  t <- t + duree
  #Tant qu'on n'a pas dépassé la durée maximale
  while (t < tmax) {
    #On ajoute la valeur de la durée au tableau
    tab[i] <- t
    #On ajoute la valeur de l'intervalle au tableau
    intervalle[i] <- duree
    #On incrémente l'indice
    i <- i + 1
    #On génère une nouvelle valeur aléatoire
    duree <- rexp(1, l)
    #On augmente la duree de la valeur de l'intervalle
    t <- t + duree
  }
  #Si l'utilisateur a demandé une liste des intervalles entre les arrivées, on la retourne # nolint
  if (res == "intervalle") {
    return(intervalle)
  #Sinon si l'utilisateur a demandé une liste des instants d'arrivée, on la retourne  # nolint
  } else if (res == "instant") {
    return(tab)
  # Sinon l'utilisateur s'est trompé
  } else {
    return("Erreur")
  }
}

#Fonction qui trace N(t), le nombre de véhicules arrivés entre les instants t0 et t0+t, pour 0<=t<=T # nolint
plot_n <- function(t) {
  #On récupère la liste des instants d'arrivée
  distrib <- distribution(t, "instant")
  #On initialise le tableau contenant le nombre de voitures arrivées
  list <- seq_along(distrib)
  #On spécifie qu'on trace à partir d'un string
  type <- "s"
  #On ouvre un fichier png pour tracer la courbe
  png("_figs/n_t.png", width = 800, height = 600, res = 100)
  #On trace la courbe de la distribution des instants en fonction du nombre de voitures arrivées # nolint
  plot(distrib, list, type, xlab = "t (s)", ylab = "Nombre de voitures")
  #On ferme le fichier png
  dev.off()
  #On imprime la valeur de l dans la console
  cat("Tracé de N(t) écrit dans n_t.png\n")
  cat("Lambda_pratique = ", length(distrib) / t, "\n\n")
}

#Fonction qui trace la distribution des intervalles entre les arrivées
plot_distribution <- function(t) {
  #On récupère la liste des intervalles entre les arrivées pour une durée de t secondes # nolint
  intervalles <- unlist(distribution(t, "intervalle"))
  #On ouvre un fichier png pour tracer l'histogramme
  png("_figs/distrib.png", width = 800, height = 600, res = 100)
  #On trace l'histogramme des intervalles entre les arrivées
  hist(intervalles, breaks = max(intervalles), plot = TRUE, probability = TRUE)
  #On ferme le fichier png
  curve(dexp(x, l), add = TRUE, col = "red")
  legend("topright", legend = c("Exponentielle théorique", "Resultats simulation"), col = c("red", "grey"), lty = 1) # nolintS
  dev.off()
  cat("Tracé de la distribution des intervalles entre les arrivées écrit dans distrib.png\n\n") # nolint
}

#Tracé de N(t) si l'utilisateur le souhaite
choice <- readline(prompt = "Tracer N(t) ? (y/n) : ")
if (choice == "y") {
  t <- as.numeric(readline(prompt = "Durée de la simulation (en secondes) : "))
  plot_n(t)
}

cat("----------------------------------\n\n")

#Tracé de la distribution des intervalles entre les arrivées si l'utilisateur le souhaite # nolint
choice <- readline(prompt = "Tracer la distribution des intervalles entre les arrivées ? (y/n) : ") # nolint
if (choice == "y") {
  t <- as.numeric(readline(prompt = "Durée de la simulation (en secondes) : "))
  plot_distribution(t)
}

cat("----------------------------------\n\n")

#Fonction qui calcule le nombre moyen d'arrivée dans l'intervalle [0; t]
#Cette fonction réalise une moyenne d'ensemble sur n simulations
nbr_moyen_arrive <- function(t, n) {
  #On initialise le nombre d'arrivées à 0
  nbr_arrivees <- 0
  #On réalise n simulations
  for (i in (1:n)) {
    #On ajoute le nombre d'arrivées de la simulation à la variable arrive
    nbr_arrivees <- nbr_arrivees + length(distribution(t, "instant"))
  }
  #On retourne la moyenne
  res <- nbr_arrivees / n
  return(res)
}

#Fonction qui calcule la différence entre la moyenne théorique et la moyenne calculée pour une durée de t secondes et n simulations # nolint
diff_moyenne <- function(t, n) {
  cat("Moyenne pour t =", t, "et n =", n, "\n")
  #On calcule la moyenne
  moyenne_pratique <-  nbr_moyen_arrive(t, n)
  #On calcule la moyenne théorique
  moyenne_theorique <- l * t
  #On calcule la différence entre les deux
  difference <- abs((moyenne_pratique - moyenne_theorique) * 100 / moyenne_pratique) # nolint
  #On imprime les résultats dans la console
  cat("Moyenne =", moyenne_pratique, ", moyenne théorique =", moyenne_theorique, ", difference =", difference, "\n\n" ) # nolint
  return(diff)
}

#Calcul de la différence entre la moyenne théorique et la moyenne calculée si l'utilisateur le souhaite # nolint
choice <- readline(prompt = "Tracer la distribution des intervalles entre les arrivées ? (y/n) : ") # nolint
if (choice == "y") {
  diff_moyenne(3600, 2000)
  diff_moyenne(3600, 2000)
  diff_moyenne(3600, 10)
  diff_moyenne(720000, 10)

  cat("\nOn effectue deux fois 1000 simulations de 3600 secondes, pour une marge d'erreur de moins de 0.001%.\nPuis deux fois 10 simulations de 3600 et 720 000 secondes (respectivement). La première a une marge d'erreur plus élevée de 0.08%, du fait du faible nombre de simulations, et la deuxième de 0.0009%, proche des deux premières car plus longue (3600 x 2000 = 720 000 x 10)\n") # nolint
}

cat("----------------------------------\n\n")

#Fonction qui calcule la densité de la distribution des intervalles entre les arrivées # nolint
loi_gamma <- function(nbr_simus) {
  # Création de listes vides
  a5 <- c()
  a20 <- c()
  a100 <- c()
  a200 <- c()
  a600 <- c()

  # On effectue 1000 simulations de 3600 secondes
  for (i in 1:nbr_simus) {
    distribs <- distribution(3600, "instant")
    a5[i] <- distribs[5]
    a20[i] <- distribs[20]
    a100[i] <- distribs[100]
    a200[i] <- distribs[200]
    a600[i] <- distribs[600]
  }

  #On ouvre un fichier png pour tracer le gamma
  png("_figs/gamma.png", width = 800, height = 600, res = 100)

  # Création d'un layout de 2 lignes et 3 colonnes pour tracer les 5 graphiques
  par(mfrow = c(2, 3))

  # On calcule la moyenne de chaque liste
  a5_densite <- density(unlist(a5))
  plot(a5_densite, xlab = "t (s)", ylab = "Densité de a5", col = "green")
  # Génération des points d'abscisses
  x <- seq(0, 20)
  # Tracé de la courbe théorique
  lines(x, dgamma(x, 5, l), col = "red")

  legend("topright", legend = c("Resultats théoriques", "Resultats pratiques"), col = c("red", "green"), lty = 1) # nolint

  a20_densite <- density(unlist(a20))
  plot(a20_densite, xlab = "t (s)", ylab = "Densité de a20", col = "green")
  x <- seq(0, 70)
  lines(x, dgamma(x, 20, l), col = "red")

  a100_densite <- density(unlist(a100))
  plot(a100_densite, xlab = "t (s)", ylab = "Densité de a100", col = "green")
  x <- seq(200, 500)
  lines(x, dgamma(x, 100, l), col = "red")

  a200_densite <- density(unlist(a200))
  plot(a200_densite, xlab = "t (s)", ylab = "Densité de a200", col = "green")
  x <- seq(250, 700)
  lines(x, dgamma(x, 200, l), col = "red")

  a600_densite <- density(unlist(a600))
  plot(a600_densite, xlab = "t (s)", ylab = "Densité de a600", col = "green")
  x <- seq(1200, 1800)
  lines(x, dgamma(x, 600, l), col = "red")

  #On ferme le fichier png
  dev.off()
  cat("Tracé des gamma des intervalles entre les arrivées écrit dans gamma.png\n\n") # nolint
}

#Calcul de la différence entre la moyenne théorique et la moyenne calculée si l'utilisateur le souhaite # nolint
choice <- readline(prompt = "Verifier la distribution selon une loi Gamma des instants d'arrivée ? (y/n) : ") # nolint
if (choice != "n") {
  cat("Verification de la distribution selon une loi Gamma des instants d'arrivée lorsque le nombre total d'arrivées est connu (1000).\n") # nolint
  loi_gamma(10000)
  cat("Le resultat est visualisable dans le fichier gamma.png\n\n") # nolint
  cat("On voit que nos courbes pratiques, et les distributions théoriques sont très proches, les instants d'arrivée sont donc bien distribués selon une loi Gamma.\n\n") # nolint
}

cat("----------------------------------\n\n")

#Fonction qui calcule la densité de la distribution des intervalles entre les arrivées # nolint
loi_uniforme <- function(n) {
  # Création d'une liste vide
  liste_arrivee <- c()
  for (i in 1:n) {
    # n simulations de 60 secondes
    distribs <- unlist(distribution(60, "instant"))
    # On ajoute les simulations de 20 instants d'arrivée à la liste
    if (length(distribs) == 20) {
      liste_arrivee <- c(liste_arrivee, distribs)
    }
  }
  densite_arrivee <- density(liste_arrivee)

  #On ouvre un fichier png pour tracer la densité des instants d'arrivée
  png("_figs/uniforme.png", width = 800, height = 600, res = 100)

  plot(densite_arrivee, xlab = "t (s)", ylab = "Densite", col = "green")
  x <- seq(-5, 65)
  lines(x, dunif(x, min = 0, max = 60), col = "red")

  legend("topright", legend = c("Resultats théoriques", "Resultats pratiques"), col = c("red", "green"), lty = 1) # nolint

  #On ferme le fichier png
  dev.off()
  cat("Tracé de la densité des instants d'arrivée écrit dans uniforme.png\n\n") # nolint
}

choice <- readline(prompt = "Verifier la distribution uniforme des instants d'arrivée ? (y/n) : ") # nolint
if(choice != "n") {
  cat("Verification de la distribution uniforme des instants d'arrivée lorsque le nombre total d'arrivées est connu (10000).\n") # nolint
  loi_uniforme(10000)
  cat("On voit que nos courbes pratiques, et les distributions théoriques sont très proches, les instants d'arrivée sont donc bien distribués selon une loi uniforme.\n\n") # nolint
}