#pragma once

#include "User.h"

#include <map>

/// Simulation d'une petite base de données d'utilisateurs.
/// Dans un cas réel, ce serait l'interface vers une base SQL ou NoSQL.
///
/// Les identifiants sont attribués automatiquement à partir de 1.
class UserDB {
public:
    /// Renvoie un pointeur vers l'utilisateur, ou nullptr s'il n'existe pas.
    const User* getUser(int id) const;

    /// Ajoute un utilisateur et renvoie son identifiant.
    int addUser(User user);

    /// Remplace l'utilisateur existant. Renvoie false si l'id est inconnu.
    bool setUser(int id, User user);

    /// Supprime l'utilisateur. Renvoie false si l'id est inconnu.
    bool removeUser(int id);

    /// Accès en lecture à tous les utilisateurs (triés par id).
    const std::map<int, User>& getUsers() const { return users_; }

private:
    int idseq_ = 1;
    std::map<int, User> users_;
};
