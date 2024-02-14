# killed_diffusion

- [x] Pricing du call / barrière / double no touch (put/call) par Monte Carlo
- [x] Simulation file -> storing the results
- [x] Revert to using functional programming (bouuuu les classes)
- [x] Plotting Jupyter notebook

# TO-DOs

- [ ] Debugger la fonction barrier call v2 (il y a une erreur quelque part) put + call
- [ ] Sanity check les autres fonctions existantes (cf. plotting notebook)
- [ ] Dev les closed formula price (quand ils existent)
- [ ] Illustrer la convergence de nos prix Monte Carlo vers ces formules fermées (quand la closed formula existe) (faible + forte ?)
- [ ] Dev une fonction générique pour un drift / diffusion plus complexe que Black Scholes mais toujours similaire à l'article (cf. la dynamique générale posée par Gobet en introduction)
- [ ] Pour aller + loin : Dev du pricing Monte Carlo pour des barrières en dimension supérieure à 1 -> tester des barrières plus complexes (cf. l'article de Gobet et ses conditions H1, whatever...) (approfondissement ?)
