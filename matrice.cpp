#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include "matrice.h"
#include "liste.h"

///Question 1 : Implémentation des méthodes de la classe Matrice:
std::vector<std::vector<float>> tableau_vide {};

Matrice::Matrice():mat(tableau_vide){}

Matrice::Matrice(Sommets sommets): mat(tableau_vide),sommets(sommets){
    std::vector<float> a(sommets.dict.size(), NAN);
    std::vector<std::vector<float>> res (sommets.dict.size(),a);
    mat = res;
}

Matrice::Matrice(Sommets sommets, std::vector<std::vector<float>> mat):sommets(sommets), mat(mat){}

Matrice::Matrice(const std::vector<Arete>& v,Sommets sommets):mat(tableau_vide), sommets(sommets){
    Matrice res {sommets};
    for (auto m: v){
        (res.mat).at(sommets.dict[m.pere]).at(sommets.dict[m.fils]) = m.poids;
    }
    mat = res.mat;
}

void Matrice::print(){
    for(auto i: mat){
        for (auto j: i){
            ///pour aligner les nan avec les entiers
            if (std::isnan(j)){
                std::cout << j << " ";
            }
            else{
                std::cout << j << "   ";
            }
            
        }
        std::cout<< std::endl;
    }
    std::cout << std::endl;
}

Matrice Liste::Liste_to_Matrice(){
    Matrice b {sommets};
    for (auto p: liste){
        for(auto k: p.second){
            b.mat.at(sommets.dict[p.first]).at(sommets.dict[k.first]) = k.second;
        }
    }
    return b;
}

Liste Matrice::Matrice_to_Liste(){
    std::vector<Arete> a;
    for (int i = 0; i<mat.size(); i++){
        for(int j = 0; j<mat.at(0).size(); j++){
            if(!(std::isnan(mat.at(i).at(j)))){
                Arete b {sommets.dictbis[i], sommets.dictbis[j], mat.at(i).at(j)};
                a.push_back(b);
            }
        }
    }
    Liste c {a, sommets};
    return c;
}

std::vector<Arete> Matrice::voisins(int i){
    std::vector<Arete> res;
    for (int j = 0; j < mat.at(0).size(); j++){
        if (!(std::isnan(mat.at(i).at(j)))){
            Arete a {sommets.dictbis[i], sommets.dictbis[j],mat.at(i).at(j)};
            res.push_back(a);
        }
    }
    return res;
}

///Question 3: Le vecteur de booléen Vu permet de marquer que le parcours est déjà passé par le sommet.
///Cette fonction ne suffit pas à elle seule car il faut lui fournir le vecteur Vu initialisé qu'avec des false.
void Matrice::parcours_prof_rec_matrice(int i, std::vector<bool>& Vu){
    Vu.at(i) = true;
    std::cout << "sommet parcouru : " << sommets.dictbis[i] << std::endl;
    std::vector<bool> b (sommets.dict.size(), true);
    std::vector<Arete> y;
    Arete z;
    if(Vu == b){
        return;
    }
    else{
        y = voisins(i);
        while(!(y.empty())){
            z = y.back();
            y.pop_back();
            if(!(Vu.at(sommets.dict[z.fils]))){
                z.print();
                parcours_prof_rec_matrice(sommets.dict[z.fils], Vu);
            }
        }
    }
}

void Matrice::parcours_prof_rec_matrice_final(int i){
    std::vector<bool> b (sommets.dict.size(),false);
    parcours_prof_rec_matrice(i, b);
}

///Question 4 : De même que pour les listes, on effectue d'abord le parcours d'une composante connexe.
std::vector<bool> Matrice::parcours_prof_iter_matrice(int i, std::vector<bool>& Vu){
    Arete a;
    std::vector<std::pair<int,Arete>> aVoir {std::make_pair(i, a)};
    Vu.at(i) = true;
    int x;
    std::vector<Arete> y;
    Arete z;
    while(!(aVoir.empty())){
        x = aVoir.back().first;
        aVoir.back().second.print();
        std::cout << "sommet parcouru : " <<sommets.dictbis[x] << std::endl;
        aVoir.pop_back();
        y = voisins(x);
        while(!(y.empty())){
            z = y.back();
            y.pop_back();
            if(!(Vu.at(sommets.dict[z.fils]))){
                aVoir.push_back(std::make_pair(sommets.dict[z.fils],z));
                Vu.at(sommets.dict[z.fils]) = true;
            }
        }
    }
    return Vu;
}

///Fonction finale pour effectuer le parcours en profondeur
///On effectue un premier parcours sur la composante connexe du sommet d'origine.
///Ensuite on effectue autant de parcours que nécessaire jusqu'à ce que tous les sommets soient explorés.
void Matrice::parcours_prof_iter_matrice_final(int i){
    std::vector<bool> Vu (sommets.dict.size(),false);
    std::vector<bool> a  (sommets.dict.size(),true);
    int k {0};
    Vu = parcours_prof_iter_matrice(i, Vu);
    while(Vu != a){
        while(Vu[k]){
            k = k + 1;
        }
        Vu = parcours_prof_iter_matrice(k, Vu);
    }
}

///Question 5 : On reprend le principe du parcours en profondeur en utilisant une structure de file plutôt qu'une structure de pile.
void Matrice::parcours_large_iter_matrice(int i, int k){
    Arete a;
    std::queue<std::pair<int,Arete>> aVoir ;
    aVoir.push(std::make_pair(i, a));
    std::vector<bool> Vu (mat.size(),false);
    Vu.at(i) = true;
    int x;
    std::vector<Arete> y;
    Arete z;
    if (k!=0){
        k = k+1;
    }
    else{
        k = -1;
    }
    while(!(aVoir.empty())&&(k!=0)){
        if(x != sommets.dict[aVoir.front().second.pere]){
            k = k-1;
        }
        x = aVoir.front().first;
        aVoir.front().second.print();
        std::cout << "sommet parcouru : " << sommets.dictbis[x] << std::endl;
        aVoir.pop();
        y = voisins(x);
        while(!(y.empty())){
            z = y.back();
            y.pop_back();
            if(!(Vu.at(sommets.dict[z.fils]))){
                aVoir.push(std::make_pair(sommets.dict[z.fils],z));
                Vu.at(sommets.dict[z.fils]) = true;
            }
        }
    }
}

///Question 7 : Algorithme de Floyd Warshall. Celui-ci va retourner la matrice des poids des plus courts chemins
/// et une matrice contenant un vecteur d'arête qui permet de reconstruire le plus court chemin.
std::pair<Matrice,std::vector<std::vector<std::vector<Arete>>>> Matrice::Floyd_Warshall(){
    std::vector<std::vector<float>> res = mat;
    int n = mat.size();
    Arete a;
    std::vector<Arete> b (n, a);
    std::vector<std::vector<Arete>> c (n, b);
    std::vector<std::vector<std::vector<Arete>>> chemins (n, c); ///construction de la matrice avec les vecteurs d'arêtes
    for(int i = 0; i<n; i++){ /// On initialise la matrice des poids et la matrice des vecteurs d'arêtes 
        for(int j = 0; j<n; j++){
            if(!(std::isnan(res.at(i).at(j)))){
                a.pere = sommets.dictbis[i];
                a.fils = sommets.dictbis[j];
                a.poids = res.at(i).at(j);
                chemins.at(i).at(j).push_back(a);
            }
            else if(i!=j){
                res.at(i).at(j) = INFINITY; /// On utilise la valeur INFINITY plutôt que NAN pour les comparaisons
            }
        }
    }
    for(int k = 0; k<n; k++){/// On lance enfin l'algorithme de Floyd Warshall
        for(int i = 0; i<n; i++){
            for(int j = 0; j<n; j++){
                if(res.at(i).at(k)+res.at(k).at(j)<res.at(i).at(j)){ /// La comparaison sert uniquement à savoir si on modifie le chemin.
                    chemins.at(i).at(j) = chemins.at(i).at(k);
                    for(auto p: chemins.at(k).at(j)){
                        chemins.at(i).at(j).push_back(p);
                    }
                }
                res.at(i).at(j) = std::min(res.at(i).at(j), res.at(i).at(k)+res.at(k).at(j)); /// Actualisation des poids à chaque itération
            }
        }
    }
    Matrice e {sommets, res};
    return std::make_pair(e,chemins);
}