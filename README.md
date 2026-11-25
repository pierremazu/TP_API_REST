# TP Web Service REST — kit de démarrage (C++20, Boost.Beast, Boost.JSON)

## Dépendances

Ubuntu / Debian (Boost ≥ 1.81) :

    sudo apt install build-essential cmake curl libboost-dev libboost-json-dev

## Compilation et exécution

    cmake -S . -B build
    cmake --build build
    ./build/server            # écoute sur http://127.0.0.1:8080
    ./build/server 9000       # autre port

Dans un second terminal :

    curl -i http://localhost:8080/version
    ./test.sh                 # relancer le serveur avant chaque passage

## Contenu

- `src/` : sources du kit ; les fichiers à compléter contiennent des `TODO`
  (`ServicesManager.cpp`, `VersionService.cpp`, `UserService.cpp`, `main.cpp`).
- `dia/classes.dia` : diagramme UML des classes (éditable avec Dia).
- `test.sh` : tests de validation (curl), étiquetés par question.
