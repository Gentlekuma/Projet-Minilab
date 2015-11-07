#include <stdlib.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "Node.hpp"
#include "TestQuad.hpp"
#include <vector>
#include "Map.hpp"

using namespace cv;

Mat src;
int level;

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;

std::list<Node *>leaf; //Contient toutes les feuilles libres de l'arbre

int main( int, char** argv )
{
	int i = 0;
	int j = 0;
	int l = 0;
	
	Node *root = new Node();
	
	
	//int reso = 32;
	
	
	//Traitement d'image 
	src = imread(argv[1]);
	
	std::cout << " Taille :"<<src.rows<< std::endl;
	std::cout << " Taille :"<<src.cols<< std::endl;
	
	
	cvtColor(src, src, CV_BGR2GRAY);
	/*
	blur(src, src, Size(3, 3));  // Applique une matrice de convolution de 3 x 3 pour le blur
	threshold(src, src, 210, 255, CV_THRESH_BINARY);
	
	for (i=0;i<8;i++)
	{
		erosion_size = i;
		dilation_size = i;
		Ouverture(0,0);
		
	}
	
	imshow("src", src);
	
	//Remplissage de la map pour avoir des dimensions paires 
	int top = (int) reso-src.cols%reso;
	int right = (int) (src.cols-src.rows);
	copyMakeBorder( src,src,top,0,0,right,BORDER_CONSTANT,0);
	*/
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
	int k =0;
	for( k = 0; l<taille ; l++){
		//std::cout << "Leaf "<< l << " x "<<leaf[l]->x<< std::endl;
		//std::cout << "Leaf "<< l << " y "<<leaf[l]->y<< std::endl;
		Vec3b color = src.at<Vec3b>(leaf[l]->x,leaf[l]->y);
		color[0] = 0;
        color[1] = 0;
        color[2] = 255;
        src.at<Vec3b>(leaf[l]->x,leaf[l]->y) = color;
	}
	
	
	NeigAssign();
	/*
	std::cout << "Leaf x  "<<leaf[3]->x<< std::endl;
	std::cout << "Leaf y "<<leaf[3]->y<< std::endl;
	
	std::cout << "Voisin x "<<leaf[3]->neighborhood.front()->x<< std::endl;
	std::cout << "Voisin y "<<leaf[3]->neighborhood.front()->y<< std::endl;
	*/
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

/*
void FindNeig(){
	
	int taille = leaf.size();
	int i;
	int j;
	int A[2];
	int B[2];
	int C[2];
	int D[2];
	
	for (i = 0; i<taille ; i++)
	{
		
		A[1] = leaf[i]->x;
		A[2] = leaf[i]->y-1;
		B[1] = leaf[i]->x;
		B[2] = leaf[i]->y + leaf[i]->resolution -1;
		C[1] = leaf[i]->x;
		C[2] = leaf[i]->y-1;
		D[1] = leaf[i]->x;
		D[2] = leaf[i]->y-1;
		
		for ( j = 0; j<taille ; j++){
			
		
		}
	}	
}
*/

void Erosion( int, void* )
{
  int erosion_type = 0;
  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( erosion_type,
                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                       Point( erosion_size, erosion_size ) );
  erode( src, src, element );
  //imshow( "Erosion Demo", erosion_dst );
}


void Dilation( int, void*)
{
  int dilation_type = 0;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type,
                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                       Point( dilation_size, dilation_size ) );
  dilate( src, src, element );
  //imshow( "Dilation Demo", dilation_dst );
}

void Fermeture(int, void*)
{
	Dilation( 0, 0);
	Erosion( 0, 0);
}	

void Ouverture(int, void*)
{
	Erosion( 0, 0);
	Dilation( 0, 0);
}


	
	
	
