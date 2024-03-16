rm(list = ls())
#Fonction qui génère une distribution d'une loi exponentielle selon param = lambda ou mu # nolint
distrib_arrivees <- function(tmax, param, res) {
  #Ecoulement du temps
  temps <- 0
  #Initialisation du tableau contenant les instants d'arrivée
  tab <- c()
  #Initialisation du tableau contenant la durée entre les arrivées
  intervalle <- c()
  #Initialisation de l'indice pour stocker les intervalles et les instants d'arriée # nolint
  i <- 1
  #Génération d'une valeur aléatoire à partir d'une distribution exponentielle de paramètre param # nolint
  duree <- rexp(1, param)
  #On augmente le temps total écoulé de la valeur de la durée générée
  temps <- temps + duree
  #Tant qu'on n'a pas dépassé la durée maximale
  while (temps < tmax) {
    #On ajoute la valeur de la durée au tableau
    tab[i] <- temps
    #On ajoute la valeur de l'intervalle au tableau
    intervalle[i] <- duree
    #On incrémente l'indice
    i <- i + 1
    #On génère une nouvelle valeur aléatoire
    duree <- rexp(1, param)
    #On augmente la duree de la valeur de l'intervalle
    temps <- temps + duree
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


distrib_depart <- function(temps, instants_arrivees, instants_services, res) {
  instant_courant <- 0
  client <- 1
  # Liste des instants de départ
  instant_depart <- c()
  # Liste des intervalles entre les départs
  intervalle_depart <- c()
  # Initialisation de l'intervalle entre les départs
  intervalle_courant <- instants_arrivees[client] + instants_services[client]
  # Tant que le temps écoulé est inférieur au temps max et qu'on a pas traité tous les clients # nolint
  while ((instant_courant < temps) && client < length(instants_arrivees)) { # nolint
      # On incremente le temps écoulé de l'intervalle entre les départs
      instant_courant <- instant_courant + intervalle_courant
      # On ajoute l'instant de départ au tableau
      instant_depart[client] <- instant_courant
      # On ajoute l'intervalle entre les départs au tableau
      intervalle_depart[client] <- intervalle_courant
      # On incrémente le client
      client <- client + 1
      # On calcule le nouvel intervalle entre les départs. # nolint
      if (instants_arrivees[client] < instant_depart[client - 1]) {
          # Si le prochain client arrive avant le départ du client précédent, il sera traité dès le départ du client précédent # nolint
          # l'intervalle entre les départs est donc la durée du service du client précédent # nolint
          intervalle_courant <- instants_services[client]
      }else {
          # Sinon l'intervalle entre les départs est l'instant d'arrivée du client + la durée de son service - l'instant de départ du client précédent # nolint
          intervalle_courant <- (instants_arrivees[client] - instant_depart[client-1]) + instants_services[client] # nolint
      }
  }
  if (res == "instant") {
      return(instant_depart)
  } else if (res == "intervalle") {
      return(intervalle_depart)
  } else {
      return("Erreur")
  }
}

N_t <- function(t, liste_arrivees, liste_departs) {
    nbre_clients <- c()
    temps <- c()
    arrivee <- 1
    nbre_clients[1] <- 0
    depart <- 1
    i <- 1
    temps[1] <- 0
    while ((depart <= length(liste_departs) && arrivee <= length(liste_arrivees)) && (temps[i] < t)) { # nolint
      if(liste_arrivees[arrivee] < liste_departs[depart]) {
          temps[i+1] <- liste_arrivees[arrivee]
          nbre_clients[i + 1] <- nbre_clients[i] + 1
          arrivee <- arrivee + 1
      }else{
          temps[i + 1] = liste_departs[depart]
          nbre_clients[i + 1] <- nbre_clients[i] - 1
          depart <- depart + 1
      }
      i <- i + 1
    }
    res <- list("x" = temps, "y" = nbre_clients)
    return(res)
}

generate_mm1 <- function(temps) {
  lambda <- 2
  mu <- 3

  liste_arrivees <- distrib_arrivees(temps, lambda, "instant")
  services <- distrib_arrivees(temps, mu, "intervalle")
  depart <- distrib_depart(temps, liste_arrivees, services, "instant")

  list_arr <- seq_along(liste_arrivees)
  list_dep <- seq_along(depart)

  nbr_clients <- N_t(temps, liste_arrivees, depart)

  #On ouvre un fichier png pour tracer le gamma
  png("_figs/filemm1.png", width = 600, height = 800, res = 100)

  layout(matrix(c(1, 2, 3, 3), 2, 2, byrow = TRUE))

  plot(list_arr, liste_arrivees, type = "s", xlab = "Temps (minutes)", ylab = "Arrivées", main = paste("Evolution nombre d'arrivées en\n", temps, "minutes (approx 166.6 h)"), col = "red") # nolint
  plot(list_dep, depart, type = "s", xlab = "Temps (minutes)", ylab = "Départs", main = paste("Evolution nombre de départs en\n", temps, "minutes (approx 166.6 h)"), col = "blue") # nolint
  plot(nbr_clients$x, nbr_clients$y, type = "s", xlab = "Temps (minutes)", ylab = "Clients", main = paste("Evolution nombre de clients sur", temps, "minutes (approx 166.6 h)")) # nolint

  dev.off()
}

choice <- readline(prompt = "Tracer la simulation de la file pour temps = 10000 ? (y/n) : ") # nolint
if (choice == "y"){
  generate_mm1(10000) # nolint
  cat("La simulation a été tracée dans le fichier _figs/filemm1.png\n") # nolint
}