### Espace de travail et packages ----

setwd(dirname(rstudioapi::getActiveDocumentContext()$path))
library(tidyverse)
library(readr)

csv <- read_csv("tuiles.csv")

# Les villes sont considérées comme des blasons
# Les blasons seront traités autrement
csv_probas <- csv |> 
  mutate(across(everything(), ~ recode(.,
                                    "blason" = "ville")))

# Gestion des côtés
cotes <- csv_probas |>
  pivot_longer(cols = starts_with("cote"), names_to = "cote", values_to = "valeurs") |> 
  group_by(valeurs) |> 
  summarise(nb = n()) |>
  mutate(pourcentage = nb / sum(nb) * 100)

graph_cote <- ggplot(data = cotes) +
              aes(x = valeurs, 
                  y = pourcentage, 
                  fill = valeurs) +
              geom_col() +
              labs(x = NULL, 
                   subtitle = "Taux d'apparition des zones sur les côtés (en %)")

# width et height en inches
ggsave("graphique_cotes.png", graph_cote, scale = 1, width = 6, height = 4)

# Gestion du milieu
milieu <- csv_probas |> 
  pivot_longer(cols = "milieu", names_to = "milieu", values_to = "valeurs") |>
  group_by(valeurs) |> 
  summarise(nb = n()) |> 
  mutate(pourcentage = nb / sum(nb) * 100)

graph_milieu <- ggplot(data = milieu) +
  aes(x = valeurs, 
      y = pourcentage, 
      fill = valeurs) +
  geom_col() +
  labs(x = NULL, 
       subtitle = "Taux d'apparition des zones au milieu (en %)")

# width et height en inches
ggsave("graphique_milieu.png", graph_milieu, scale = 1, width = 6, height = 4)

blason <- csv |>
  pivot_longer(cols = cote1:milieu, names_to = "pp", values_to = "valeurs") |> 
  group_by(valeurs) |> 
  summarise(nb = n()) |>
  mutate(pourcentage = nb / sum(nb) * 100) |> 
  filter(!row_number() %in% c(1, 3, 4, 5))

print(cotes)
print(milieu)