#include <stdlib.h>
#include <stdio.h>
#include "Node.hpp"

using namespace cv;

void Decide(Node* node); // Decide si l'on doit partager selon si un obstacle et une zone libre se trouvent dans la meme region
void Divise(Node* node); // Création des enfants lorsque le node est mixed
void NeigAssign(); // Recherche de voisins
void FindNeig(); // Recherche de voisins type 2
