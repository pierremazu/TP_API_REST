#!/usr/bin/env bash
# Tests du TP REST.
# Usage : lancer le serveur dans un autre terminal (./build/server), puis
#         ./test.sh [url_de_base]          (défaut : http://localhost:8080)
# Les tests modifient la base : relancez le serveur avant chaque passage.

BASE="${1:-http://localhost:8080}"
pass=0; fail=0

# check <question> <méthode> <chemin> <statut attendu> [corps envoyé] [extrait attendu dans la réponse]
check() {
    local q="$1" method="$2" path="$3" expected="$4" data="$5" needle="$6"
    local args=(-s -o /tmp/tp_rest_body.$$ -w '%{http_code}' -X "$method")
    [[ -n "$data" ]] && args+=(-H 'Content-Type: application/json' --data "$data")
    local status body
    status=$(curl "${args[@]}" "$BASE$path")
    body=$(tr -d ' \n' < /tmp/tp_rest_body.$$)
    if [[ "$status" == "$expected" && ( -z "$needle" || "$body" == *"$needle"* ) ]]; then
        printf '  \e[32mOK\e[0m    %-4s %-7s %-16s -> %s\n' "$q" "$method" "$path" "$status"
        ((pass++))
    else
        printf '  \e[31mÉCHEC\e[0m %-4s %-7s %-16s -> %s (attendu %s%s)  %s\n' \
               "$q" "$method" "$path" "$status" "$expected" "${needle:+, contenant $needle}" "$body"
        ((fail++))
    fi
}

if ! curl -s -o /dev/null "$BASE/"; then
    echo "Serveur injoignable sur $BASE : lancez-le d'abord." >&2
    exit 2
fi

echo "== Service version (Q1-Q3)"
check Q1  GET    /version     200 "" '{"major":1,"minor":0}'
check Q3  GET    /version/23  200 "" '"major":1'
check Q3  GET    /versions    404
check Q3  GET    /nothing     404 "" '"error"'

echo "== Lecture d'un utilisateur (Q4-Q6)"
check Q4  GET    /user/abc    400
check Q4  GET    /user/1x     400
check Q5  PUT    /version     501
check Q11 DELETE /version     405
check Q6  GET    /user/1      200 "" '"name":"Paul","age":23'
check Q6  GET    /user/42     404

echo "== Création (Q7-Q8)"
check Q7  POST   /user        400 '{"name":'
check Q7  POST   /user        400 ''
check Q8  POST   /user        201 '{"name":"Jean","age":44}' '{"id":2}'
check Q8  GET    /user/2      200 "" '"name":"Jean","age":44'
check Q8  POST   /user        400 '{"name":"Zoé"}'
check Q8  POST   /user        400 '{"name":"Zoé","age":"vingt"}'
check Q8  POST   /user        400 '[1,2,3]'
check Q7  POST   /user/3      405 '{"name":"Zoé","age":20}'

echo "== Modification (Q9-Q10)"
check Q10 PATCH  /user/1      204 '{"age":24}'
check Q10 GET    /user/1      200 "" '"name":"Paul","age":24'
check Q10 PATCH  /user/1      204 '{"name":"Paulo"}'
check Q10 GET    /user/1      200 "" '"name":"Paulo","age":24'
check Q10 PATCH  /user/99     404 '{"age":24}'
check Q10 PATCH  /user/1      400 '{"taille":180}'
check Q10 PATCH  /user/1      400 '{"age":-3}'
check Q10 PATCH  /user        405 '{"age":24}'

echo "== Suppression (Q11-Q12)"
check Q12 DELETE /user/2      204
check Q12 GET    /user/2      404
check Q12 DELETE /user/2      404
check Q12 DELETE /user        405

echo "== Bonus (Q13)"
check Q13 GET    /user        200 "" '[{"name":"Paulo","age":24,"id":1}]'

rm -f /tmp/tp_rest_body.$$
echo
echo "Réussis : $pass   Échoués : $fail"
[[ $fail -eq 0 ]]
