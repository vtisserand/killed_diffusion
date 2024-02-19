# killed_diffusion

- [x] Pricing du call / barrière / double no touch (put/call) par Monte Carlo
- [x] Simulation file -> storing the results
- [x] Revert to using functional programming (bouuuu les classes)
- [x] Plotting Jupyter notebook
- [x] Debugger la fonction barrier call v2 (il y a une erreur quelque part) put + call (bug r -> r*x, sigma -> sigma*x dans le modèle de black scholes)
- [x] Sanity check les autres fonctions existantes (cf. plotting notebook)
- [x] Dev les closed formula price (quand ils existent)

# TO-DOs

- [ ] Vérifier si p(z1, z2, T/N) ou 1-p(z1, z2, T/N) pour les options barrières
- [ ] Illustrer la convergence de nos prix Monte Carlo vers ces formules fermées (quand la closed formula existe) (faible + forte ?)
- [ ] Dev une fonction générique pour un drift / diffusion plus complexe que Black Scholes mais toujours similaire à l'article (cf. la dynamique générale posée par Gobet en introduction)
- [ ] Pour aller + loin : Dev du pricing Monte Carlo pour des barrières en dimension supérieure à 1 -> tester des barrières plus complexes (cf. l'article de Gobet et ses conditions H1, whatever...) (approfondissement ?)
