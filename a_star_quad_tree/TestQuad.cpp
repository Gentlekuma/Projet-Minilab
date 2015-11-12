#include <stdlib.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "node.hpp"
#include "TestQuad.hpp"
#include <vector>
#include "Map.hpp"

using namespace cv;

Mat src;
int level;


std::vector<Node *>leaf; //Contient toutes les feuilles libres de l'arbre

int main( int, char** argv )
{
	int i = 0;
	int j = 0;
	int l = 0;
	int id = 0;
	
	Node* root = new Node();
	
	
	//int reso = 32;
	
	
	//Traitement d'image 
	src = imread(argv[1]);
	
	std::cout << " Taille :"<<src.rows<< std::endl;
	std::cout << " Taille :"<<src.cols<< std::endl;
	
	
	cvtColor(src, src, CV_BGR2GRAY);
	
	//Initialisation du node root 
	
	root->x = 0;
	root->y = 0;
	root->resolution = src.rows;
	root->type = MIXED_NODE;
	
	Decide(root);
	
	
	//Tests accès pixels image
	/*
	for(j = root->x ; j<root->resolution ; j++){
		for(l = root->y ; l<root->resolution ; l++){
			if (src.at<uchar>(j,l) != 0 && src.at<uchar>(j,l) != 255){
				std::cout << "Valeur map "<<src.at<uchar>(i,j)<< std::endl;
			}
			//std::cout << "Valeur map "<<src.at<int>(i,j)<< std::endl;
			
		}
	}
	*/
	
	
	//Réalisation de l'arbre de manière recursive
	cvtColor(src, src, CV_GRAY2BGR);
	
	
	//Verification emplacement des feuilles libres
	int taille = leaf.size();
	for( l = 0; l<taille ; l++){
		//std::cout << "Leaf "<< l << " x "<<leaf[l]->x<< std::endl;
		//std::cout << "Leaf "<< l << " y "<<leaf[l]->y<< std::endl;
		Vec3b color = src.at<Vec3b>(leaf[l]->x,leaf[l]->y);
		color[0] = 0;
        color[1] = 0;
        color[2] = 255;
        src.at<Vec3b>(leaf[l]->x,leaf[l]->y) = color;
	}
	
	int k = 0;
	//Recherche des voisins et choix du goal 
	for( k = 0; k<taille ; k++){
		if ( leaf[k]->x == 160 && leaf[k]->y == 192 ) {
			leaf[k]->type = GOAL_NODE;
		}
		NeigFill(leaf[k]);
	}
	
	/*
	for ( std::list<int>::iterator it=leaf.begin(); it != leaf.end(); ++it ){
		Vec3b color = src.at<Vec3b>(*it->x,*it->y);
		color[0] = 0;
        color[1] = 0;
        color[2] = 255;
        src.at<Vec3b>(*it->x,*it->y) = color;
	}
	*/
	
	//NeigAssign();


	imshow("pad", src);
	
	
	std::cout << "Fin test "<<level<< std::endl;
	waitKey(0);
	return 0;
}

void Decide(Node* node)
{
	std::cout << " Resolution Node "<<node->resolution<< std::endl;
	int i = 0;
	int j = 0;
	int test_w = 0;
	int test_b = 0;
	
	//Parcours du carré correspondant au node
	for (i=node->x ; i<node->x+node->resolution; i++)
	{
		//std::cout << "Valeur i "<<i<< std::endl;
		for(j=node->y ; j<node->y+node->resolution; j++)
		{
			
			//std::cout << "Valeur j "<<j<< std::endl;
			if ( src.at<uchar>(i,j) == 255)
			{
				//std::cout << "Valeur de Test BLANC"<<test_w<< std::endl;
				test_w = test_w + 1;
			}
			if ( src.at<uchar>(i,j) == 0)
			{
				//std::cout << "Pos "<<test_b<< std::endl;
				test_b = test_b + 1;
			}
		}
	}
	
	std::cout << "Valeur de Test BLANC "<<test_w<< std::endl;
	//Choix du type du node en fonction du nombre de pixels obstacles dans le node
	
	//Cas où le node ne contient que des pixels obstacles
	if ( test_w == 0) 
	{

		std::cout << "//// BLOCKED NODE "<<node->x<<" "<<node->y<< std::endl;
		node->type = BLOCKED_NODE;
	}
	
	//Cas où la division est necessaire 
	if ( test_b != 0 && test_w != 0)
	{

		std::cout << "//// DIVISION "<<node->x<<" "<<node->y<< std::endl;
		node->type = MIXED_NODE;
		Divise(node);
	}
	
	//Cas où le node est libre
	if( test_b == 0)
	{
		
		std::cout << "/////// Ajout d'une feuille "<<node->x<<" "<<node->y<< std::endl;
		node->type = FREE_NODE;
		if ( src.at<uchar>(node->x,node->y) == 255){
			leaf.push_back(node);
		}
	}
	
	
}

void Divise(Node* node)
{
		if ( node->resolution >= 8 ) {
		level = level + 1;
		std::cout << "Level "<<level<< std::endl;
		std::cout << "Nombre feuilles "<<leaf.size()<< std::endl;
			
			node->sons[0] = new Node( node->x, node->y , (node->resolution)/2, node , FREE_NODE , node->sons , node->neighborhood );
			Decide(node->sons[0]);
			//TypeAssign(node->sons[0]);
			
			node->sons[1] = new Node( node->x+(node->resolution)/2 , node->y , (node->resolution)/2, node , FREE_NODE , node->sons , node->neighborhood );
			Decide(node->sons[1]);
			//TypeAssign(node->sons[1]);
			
			node->sons[2] = new Node( node->x, node->y+(node->resolution)/2, (node->resolution)/2, node , FREE_NODE , node->sons , node->neighborhood );
			Decide(node->sons[2]);
			//TypeAssign(node->sons[2]);
			
			node->sons[3] = new Node( node->x+(node->resolution)/2 , node->y+(node->resolution)/2 , (node->resolution)/2, node , FREE_NODE , node->sons , node->neighborhood );
			Decide(node->sons[3]);
			//TypeAssign(node->sons[3]);
		}
}

/*
void NeigAssign(){
	
	int taille = leaf.size();
	int i;
	int n;
	int w;
	int s;
	int e;
	for (i = 0; i<taille ; i++)
	{
		std::cout << "Recherche de voisins sur le node x = "<< leaf[i]->x<<" y = "<< leaf[i]->y << std::endl;
		std::cout << "  "<< std::endl;
		//Points de test North,West,South,East
		int N[2];
		int W[2];
		int S[2];
		int E[2];
		
		
		N[1] = leaf[i]->x;
		N[2] = leaf[i]->y-2;
		for (n = 0; n<taille ; n++)
		{
			if ((N[1] == leaf[n]->x) && (N[2] == ((leaf[n]->y)+(leaf[n]->resolution)-2)))
			{
				std::cout << "NNNN Recherche Nord sur le node x = "<< leaf[n]->x<<" y = "<< leaf[n]->y << std::endl;
				std::cout << "Voisin Nord trouvé"<< std::endl;
				leaf[i]->neighborhood.push_back(leaf[n]);
			}
		} 
		
		W[1] = leaf[i]->x-2;
		W[2] = leaf[i]->y;
		
		for (w = 0; w<taille ; w++)
		{
			if (W[2] == leaf[w]->y && W[1] > leaf[w]->y && W[1] < (leaf[w]->y + leaf[w]->resolution))
			{
				std::cout << "WWWW Recherche Ouest sur le node x = "<< leaf[w]->x<<" y = "<< leaf[w]->y << std::endl;
				std::cout << "Voisin Ouest trouvé"<< std::endl;
				leaf[i]->neighborhood.push_back(leaf[w]);
			}
		} 
		
		S[1] = leaf[i]->x;
		S[2] = leaf[i]->y + leaf[i]->resolution + 2;
		for (s = 0; s<taille ; s++)
		{
			if (S[1] == leaf[s]->x && S[2] > leaf[s]->x && S[2] < leaf[s]->x + leaf[s]->resolution)
			{
				std::cout << "SSSS Recherche Sud sur le node x = "<< leaf[s]->x<<" y = "<< leaf[s]->y << std::endl;
				std::cout << "Voisin Sud trouvé"<< std::endl;
				leaf[i]->neighborhood.push_back(leaf[s]);
			}
		} 
		
		E[1] = leaf[i]->x + leaf[i]->resolution + 2;
		E[2] = leaf[i]->y;
		for (e = 0; e<taille ; e++)
		{
			if (E[2] == leaf[e]->y && E[1] > leaf[e]->y && E[1] < (leaf[e]->y + leaf[e]->resolution))
			{
				std::cout << "EEEE Recherche Est sur le node x = "<< leaf[e]->x<<" y = "<< leaf[e]->y << std::endl;
				std::cout << "Voisin Est trouvé"<< std::endl;
				leaf[i]->neighborhood.push_back(leaf[e]);
			}
		}
	}
}
*/


// Methode de recherche des voisins fonctionnelle

Node* FindNorthNeig(Node* node){
	
	if ( node->isRoot() == true ){
		std::cout << " Est Racine" << std::endl;
		return NULL;
	}
	
	if ( node == node->parent_node->sons[2] ) {
		std::cout << " Voisin nord trouvé " << std::endl;
		return node->parent_node->sons[0];
		
	}
	
	if ( node == node->parent_node->sons[3] ) {
		std::cout << " Voisin nord trouvé " << std::endl;
		return node->parent_node->sons[1];
		
	}
	
	std::cout << " Recherche de voisins dans le niveau au dessus " << std::endl;
	Node* Inter = FindNorthNeig(node->parent_node);

	
	if ( (Inter == NULL)||(Inter->isLeaf() == true) ){
		std::cout << " Est Racine" << std::endl;
		return NULL;
	}
	else
	{
		if ( node == node->parent_node->sons[0] ) { 
			std::cout << " Voisin nord trouvé " << std::endl;
			return Inter->sons[2];
			
		}
		else{
			std::cout << " Voisin nord trouvé " << std::endl;
			return Inter->sons[3];
		}
	}

}

Node* FindEastNeig(Node* node){
	
	if ( node->isRoot() == true ){
		return NULL;
	}
	
	if ( node == node->parent_node->sons[3] ) {
		std::cout << " Voisin est trouvé " << std::endl;
		return node->parent_node->sons[2];
		
	}
	
	if ( node == node->parent_node->sons[1] ) {
		std::cout << " Voisin est trouvé " << std::endl;
		return node->parent_node->sons[0];
		
	}

	Node* Inter = FindEastNeig(node->parent_node);

	
	if ( (Inter == NULL)||(Inter->isLeaf() == true) )
		return NULL;
	else
	{
		if ( node == node->parent_node->sons[2] ) { 
			std::cout << " Voisin est trouvé " << std::endl;
			return Inter->sons[3];
			
		}
		else{
			std::cout << " Voisin est trouvé " << std::endl;
			return Inter->sons[1];
			
		}
	}

}

Node* FindSouthNeig(Node* node){
	
	if ( node->isRoot() == true ){
		return NULL;
	}
	
	if ( node == node->parent_node->sons[1] ) {
		std::cout << " Voisin sud trouvé " << std::endl;
		return node->parent_node->sons[3];
		
	}
	
	if ( node == node->parent_node->sons[0] ) {
		std::cout << " Voisin sud trouvé " << std::endl;
		return node->parent_node->sons[2];
		
	}

	Node* Inter = FindSouthNeig(node->parent_node);

	
	if ( (Inter == NULL)||(Inter->isLeaf() == true) )
		return NULL;
	else
	{
		if ( node == node->parent_node->sons[3] ) { 
			std::cout << " Voisin sud trouvé " << std::endl;
			return Inter->sons[1];
			
		}
		else{
			std::cout << " Voisin sud trouvé " << std::endl;
			return Inter->sons[0];
			
		}
	}

}

Node* FindWestNeig(Node* node){
	
	if ( node->isRoot() == true ){
		return NULL;
	}
	
	if ( node == node->parent_node->sons[0] ) {
		std::cout << " Voisin ouest trouvé " << std::endl;
		return node->parent_node->sons[1];
		
	}
	
	if ( node == node->parent_node->sons[2] ) {
		std::cout << " Voisin ouest trouvé " << std::endl;
		return node->parent_node->sons[3];
		
	}

	Node* Inter = FindWestNeig(node->parent_node);

	
	if ( (Inter == NULL)||(Inter->isLeaf() == true) )
		return NULL;
	else
	{
		if ( node == node->parent_node->sons[1] ) { 
			std::cout << " Voisin ouest trouvé " << std::endl;
			return Inter->sons[0];
			
		}
		else{
			std::cout << " Voisin ouest trouvé " << std::endl;
			return Inter->sons[2];
		}	
	}

}

void NeigFill(Node* node){
	
	std::cout << " Debut recherche voisins " << std::endl;
	node->neighborhood.push_back(FindNorthNeig(node));
	node->neighborhood.push_back(FindEastNeig(node));
	node->neighborhood.push_back(FindSouthNeig(node));
	node->neighborhood.push_back(FindWestNeig(node));
	
	
}




	
	
	
