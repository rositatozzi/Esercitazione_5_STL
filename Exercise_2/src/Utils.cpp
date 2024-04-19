#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include <sstream>
#include <fstream>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;


namespace MeshLibrary {
bool importMesh(const string& path, PolygonalMesh& mesh,double tol1D,double tol2D){
    if(!importCell0Ds(path + "./Cell0Ds.csv",mesh)){
        return false;
    }
    else{ //TEST INSERIMENTO MARKER
        for(auto& el : mesh.VerticesMarker){// per ogni elemento (chiave,valore) della mappa dei Marker dei vertici
            cout<<"key:" <<el.first<< endl; // .first() restituisce la chiave
            for(auto& id : el.second ){     // . second() restituisve il valore che in questo caso è una lista
                cout<<id<<" ";              // sul quale posso iterare
            }
            cout<<endl;
        }
    }

    if(!importCell1Ds(path + "./Cell1Ds.csv",mesh, tol1D)){
        return false;
    }
    else{//TEST INSERIMENTO MARKER
        for(auto& el : mesh.EdgesMarker){// per ogni elemento (chiave,valore) della mappa dei Marker dei vertici
            cout<<"key:" <<el.first<< endl; // .first() restituisce la chiave
            for(auto& id : el.second ){     // . second() restituisve il valore che in questo caso è una lista
                cout<<id<<" ";              // sul quale posso iterare
            }
            cout<<endl;
        }
    }


    if(!importCell2Ds(path + "./Cell2Ds.csv",mesh, tol2D)){
        return false;
    }
    else{
        for(unsigned int i = 0; i< mesh.NumberofCell2Ds;i++){
            cout <<"Id: "<< mesh.IdCell2Ds[i] <<" Numero vertici e lati: "<< (mesh.VerticesCell2Ds[i]).size() <<" "<< (mesh.EdgesCell2Ds[i]).size()<<endl;
        }
    }
    return true;
}


bool importCell0Ds(const string& fileName, PolygonalMesh& mesh){
    ifstream file(fileName);
    if(file.fail()){
        return false;
    }

    string header; // Prima riga
    getline(file,header);
    list<string> lines;
    string line;
    while(getline(file,line)){ // si ferma quando ha letto tutto il file e non riesce a fare getline
        lines.push_back(line); // inserisco con push_back alla fine della lista
            // posso dimenticarmi del file e lavorare sulla lista
    }
    file.close(); // Ho finito di leggere posso chiudere

    mesh.NumberofCell0Ds = lines.size(); // Il num di righe del file Cell0Ds corrisponde al numero di vertici che ho perchè su ogni riga ho un vertice
    mesh.CoordinatesCell0Ds.reserve(mesh.NumberofCell0Ds);
    mesh.IdCell0Ds.reserve(mesh.NumberofCell0Ds);
    mesh.MarkerCell0Ds.reserve(mesh.NumberofCell0Ds); //Il vettore ha ancora dimensione nulla ma non la capacita
        // Viene riservato sufficiente spazio per poter inserire gli elementi
        // Non usare resize
    for(const string& line : lines){//per ogni linea nella lista delle linee
        istringstream convert(line); // converte la linea
        char c;
        unsigned int id;
        unsigned int marker;
        Vector2d coordinates;
        convert >> id >>c>> marker >>c>> coordinates(0) >>c>> coordinates(1);

        // Qui ho dei vettori quindi aggiungo in coda i nuovi elementi che leggo
        mesh.IdCell0Ds.push_back(id);
        mesh.MarkerCell0Ds.push_back(marker); // così ho in posizione i il marker del nodo con id in posizione i
        mesh.CoordinatesCell0Ds.push_back(coordinates);

        // Ora aggiorno la mappa (marker,lista_dei_vertici_con_quel_marker)
        // Devo controllare se la chiave esiste già
        // Se esiste, aggiungo un elemento alla lista, altrimenti deve creare una chiave
        if(marker != 0){//Non ho interesse a memorizzare i vertici interni
            auto ret = mesh.VerticesMarker.insert({marker,{id}}); // {id} crea una lista che contiene id
            // restituisce una coppia iteratore,booleano
            // dove iteratore punta alla chiave e booleano mi dice se l'ha trovata o meno
            // se ritorna true, vuol dire che la chiave non esisteva ed è stata inserita correttamente
            // se ritorna false vuol dire che non l'ha inserito perchè già esisteva
            if(!ret.second){// se non l'ha inserito lo devo inserire io ad uno esistente
                mesh.VerticesMarker[marker].push_back(id);
                // accede all'elemento della mappa con chiave [marker] e aggiunge alla lista corrispondente un elemento
            }
        }
    }
    return true;
}

bool importCell1Ds(const string& fileName, PolygonalMesh& mesh,double tol1D){
    ifstream file(fileName);
    if(file.fail()){
        return false;
    }

    string header; // Prima riga
    getline(file,header);
    list<string> lines;
    string line;
    while(getline(file,line)){ // si ferma quando ha letto tutto il file e non riesce a fare getline
        lines.push_back(line); // inserisco con push_back alla fine della lista
            // posso dimenticarmi del file e lavorare sulla lista
    }
    file.close();
    mesh.NumberofCell1Ds = lines.size(); // Il num di righe del file Cell1Ds corrisponde al numero di vertici che ho perchè su ogni riga ho un vertice
    mesh.IdCell1Ds.reserve(mesh.NumberofCell1Ds);
    mesh.VerticesCell1Ds.reserve(mesh.NumberofCell1Ds);
    mesh.MarkerCell1Ds.reserve(mesh.NumberofCell1Ds);

    for(const string& line : lines){//per ogni linea nella lista delle linee
        istringstream convert(line);
        char c;
        unsigned int id;
        unsigned int marker;
        array<unsigned int,2> vertices ;
        convert >> id >>c>> marker >> c>>vertices[0] >>c>> vertices[1];

        // Qui ho dei vettori quindi aggiungo in coda i nuovi elementi che leggo
        mesh.IdCell1Ds.push_back(id);
        mesh.MarkerCell1Ds.push_back(marker); // così ho in posizione i il marker del nodo con id in posizione i
        mesh.VerticesCell1Ds.push_back(vertices);
        // Ora aggiorno la mappa (marker,lista_dei_vertici_con_quel_marker)
        // Devo controllare se la chiave esiste già
        // Se esiste, aggiungo un elemento alla lista, altrimenti deve creare una chiave
        if(marker != 0){//Non ho interesse a memorizzare i vertici interni
            auto ret = mesh.EdgesMarker.insert({marker,{id}}); // {id} crea una lista che contiene id
            // restituisce una coppia iteratore,booleano
            // dove iteratore punta alla chiave e booleano mi dice se l'ha trovata o meno
            // se ritorna true, vuol dire che la chiave non esisteva ed è stata inserita correttamente
            // se ritorna false vuol dire che non l'ha inserito perchè già esisteva
            if(!ret.second){// se non l'ha inserito lo devo inserire io ad uno esistente
                mesh.EdgesMarker[marker].push_back(id);
                // accede all'elemento della mappa con chiave [marker] e aggiunge alla lista corrispondente un elemento
            }
        }
    }

    return true;
}

bool importCell2Ds(const string& fileName, PolygonalMesh& mesh,double tol2D){
    ifstream file(fileName);
    if(file.fail()){
        return false;
    }

    string header; // Prima riga
    getline(file,header);
    list<string> lines;
    string line;
    while(getline(file,line)){ // si ferma quando ha letto tutto il file e non riesce a fare getline
        lines.push_back(line); // inserisco con push_back alla fine della lista
            // posso dimenticarmi del file e lavorare sulla lista
    }
    file.close();
    mesh.NumberofCell2Ds = lines.size(); // Il num di righe del file Cell1Ds corrisponde al numero di vertici che ho perchè su ogni riga ho un vertice
    mesh.IdCell2Ds.reserve(mesh.NumberofCell2Ds);
    mesh.VerticesCell2Ds.reserve(mesh.NumberofCell2Ds);
    mesh.EdgesCell2Ds.reserve(mesh.NumberofCell2Ds);

    for(const string& line : lines){//per ogni linea nella lista delle linee
        istringstream convert(line);
        char c;
        unsigned int id;
        unsigned int marker;
        unsigned int numVertices;
        unsigned int numEdges;
        vector<unsigned int> vertices ;
        vector<unsigned int> edges;

        convert >> id >>c>> marker >>c>> numVertices >> c;

        mesh.IdCell2Ds.push_back(id);
        vertices.reserve(numVertices);
        for(unsigned int i = 0; i < numVertices; i ++){
            unsigned int idVertice;
            convert >> idVertice >> c;
            vertices.push_back(idVertice);
        }

        convert >> numEdges;
        edges.reserve(numEdges);
        for(unsigned int i = 0; i < numVertices; i ++){
            unsigned idEdge;
            convert >>c>> idEdge ;
            edges.push_back(idEdge);
        }


        mesh.VerticesCell2Ds.push_back(vertices);
        mesh.EdgesCell2Ds.push_back(edges);

    }
    return true;
}


void testLengthEdges(PolygonalMesh& mesh, double tol1D){
    for(auto& id : mesh.IdCell1Ds){//sto accedendo direttamente all'oggetto
        unsigned int idBegin = mesh.VerticesCell1Ds[id][0];
        unsigned int idEnd = mesh.VerticesCell1Ds[id][1];
        if((mesh.CoordinatesCell0Ds[idBegin]-mesh.CoordinatesCell0Ds[idEnd]).norm() < tol1D){
            cout << "ERROR : the edge " << id << " has zero length"<<endl;

        }
    }


}



void testAreaPolygonal(PolygonalMesh& mesh, const double tol2D){
    for(auto& id : mesh.IdCell2Ds){
        vector<unsigned int> idVertices = mesh.VerticesCell2Ds[id];
        // CALCOLO AREA
        double Area = crossProduct(mesh.CoordinatesCell0Ds[idVertices.front()], mesh.CoordinatesCell0Ds[idVertices.back()]);
        //.front() e .back() restituiscono primo e ultimo elemento di un vettore
        for(unsigned int i = 0; i<(idVertices.size()-1);i++){
            Area = Area + crossProduct(mesh.CoordinatesCell0Ds[idVertices[i]],mesh.CoordinatesCell0Ds[idVertices[i+1]]);
        }
        Area = Area /2;
        if(Area < tol2D){
            cout << "ERROE : the polygonal " << id << " has zero area"<<endl;

        }
    }

}

}


double crossProduct(Vector2d& v1, Vector2d& v2 ){
    return abs(v1[0]*v2[1] - v1[1]*v2[0]);
}
double setTol1D(){
    double tolDefault = 10 *  numeric_limits<double>::epsilon();
    double tolInput;
    cout << "Inserire tolleranza 1D";
    cin >> tolInput;
    return max(tolDefault,tolInput);
}

double setTol2D(const double tol1D){
    double tolDefault = 10 *  numeric_limits<double>::epsilon();
    double tolInput;
    cout << "Inserire tolleranza 2D";
    cin >> tolInput;
    double tol2D = pow(tol1D,2)*sqrt(3)/4;
    double tol = max(tol2D,tolDefault);
    return max(tol,tolInput);
}


