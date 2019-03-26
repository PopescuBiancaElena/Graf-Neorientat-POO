#include<iostream>
#include<fstream>

using namespace std;

#define SIZE 20

ifstream f("test.in");
ofstream g("test.out");


class Graph
{
    int muchii;
    int noduri;
    int conexe;
    int matrice[SIZE][SIZE];
    int copie[SIZE][SIZE];
    int vizitat[SIZE];
public:
    Graph();
    friend istream& operator>>(istream&, Graph&);
    friend ostream& operator<<(ostream&, const Graph&);
    friend Graph operator+(const Graph&, const Graph&);
    friend bool operator==(const Graph&, const Graph&);
    void Parcurgere_Latime();
    void Parcurgere_Adancime();
    void Matrice_Drumuri();
    void Componente_Conexe();
    void Graf_Conex();
};

Graph::Graph()
{
    noduri = 0;
    muchii = 0;
}

istream& operator>>(istream& in, Graph& nr)
{
    int nod_x, nod_y, i, j;

    in>>nr.noduri;
    for(i = 0; i < nr.noduri; i++)
        for(j = 0; j < nr.noduri; j++)
        {
            nr.matrice[i][j] = 0;
            nr.copie[i][j] = 0;
        }
    in>>nr.muchii;
    for(i = 0; i< nr.muchii; i++)
    {
        in>>nod_x>>nod_y;
        nr.matrice[nod_x][nod_y] = nr.matrice[nod_y][nod_x]= 1;
        nr.copie[nod_x][nod_y] = nr.copie[nod_y][nod_x]= 1;
    }
    return in;
}

ostream& operator<<(ostream& out, const Graph& ma)
{
    int i, j;
    for(i = 0; i < ma.noduri; i++)
    {
        for(j = 0; j < ma.noduri; j++)
            out<<ma.matrice[i][j]<<" ";
        out<<endl;
    }
    return out;
}

void Graph::Parcurgere_Adancime()
{
    int vizitat[SIZE], Stiva[SIZE];
    int i, j, dim = 1, cate_vizitate = 1;

    for(i = 0; i < noduri; i++)
    {
        vizitat[i] = 0;
        Stiva[i] = 0;
    }
    int s = -2;
    while(s != -1)
    {
        for(i = 0; i < noduri; i++)
            if(vizitat[i] == 0)
            {
                conexe++;
                s = i;
                vizitat[s] = 1;
                Stiva[dim] = s;
                dim++;
                g<<s<<" ";
                break;
            }
            else
                s = -1;
        while(cate_vizitate < noduri && dim != 1)
        {
            int primul = Stiva[dim - 1];
            int vecin_vizitat = 0;
            for(j = 0; j < noduri; j++)
                if(matrice[primul][j] == 1)
                {
                    int vecin = j;
                    if(vizitat[vecin] == 0)
                    {
                        cate_vizitate++;
                        vizitat[vecin] = 1;
                        Stiva[dim] = vecin;
                        dim++;
                        vecin_vizitat = 1;
                        g<<vecin<<" ";
                        break;
                    }
                }
            if(vecin_vizitat == 0)
            {
                Stiva[dim-1] = 0;
                dim--;
            }
        }
    }
}

void Graph::Parcurgere_Latime()
{
    int i, j, conexe = 0, Coada[SIZE];;
    int dim = 1, cate_vizitate = 1;

    for(i=0; i< noduri; i++)
    {
        vizitat[i] = 0;
        Coada[i] = 1;
    }
    int s = -2;
    while(s != -1)
    {
        for(i = 0; i < noduri; i++)
            if(vizitat[i] == 0)
            {
                conexe++;
                s = i;
                Coada[dim] = s;
                dim++;
                vizitat[s] = conexe;
                g<<s<<" ";
                break;
            }
            else
                s = -1;
        while(cate_vizitate < noduri && dim != 1)
        {
            int primul = Coada[dim - 1];
            for(j = 0; j < noduri; j++)
                if(matrice[primul][j] == 1)
                {
                    int vecin = j;
                    if(vizitat[vecin] == 0)
                    {
                        cate_vizitate++;
                        vizitat[vecin]=conexe;
                        g<<vecin<<" ";
                        Coada[dim] = vecin;
                        dim++;
                    }
                }
            for(i = 0; i < noduri; i++)
                Coada[i] = Coada[i+1];
            dim--;
        }
    }
}

void Graph::Matrice_Drumuri() //Roy_Warshall
{
    int i, j, k;
    for(k = 0; k < noduri; k++)
        for(i = 0; i < noduri; i++)
            for(j = 0; j < noduri; j++)
                if(i != j)
                    if(matrice[i][j] == 0)
                        matrice[i][j] = matrice[i][k]*matrice[k][j];
    g<<"Matricea drumurilor este: "<<endl;
    int n, m;
    for(m = 0; m < noduri; m++)
    {
        for(n = 0; n < noduri; n++)
            g<<matrice[m][n]<<" ";
        g<<endl;
    }
}

void Graph::Componente_Conexe()
{
    int i, j;

    for(i = 1; i <= conexe; i++)
    {
        g<<"Componenta conexa:"<<i<<" este formata din nodurile: ";
        for(j = 0; j < noduri; j++)
        {
            if(vizitat[j] == i)
                g<<j<<" ";
        }
    }

}

void Graph::Graf_Conex()
{
    int nr = 0, i, j;

    for(i = 1; i <= conexe; i++)
    {
        for(j = 0; j < noduri; j++)
        {
            if(vizitat[j] == i)
            {
                nr++;
                break;
            }
        }
    }
    if(nr != 1)
    {
        g<<"Graf nu e conex";
        g<<endl;
    }
    else
    {
        g<<"Graful este conex";
        g<<endl;

    }
}

Graph operator+(const Graph& matrice1, const Graph& matrice2)
{
    int i, j;
    Graph matrice3;
    matrice3.noduri = matrice1.noduri;
    matrice3.muchii = matrice2.muchii + matrice1.muchii;
    if(matrice1.noduri != matrice2.noduri)
        g<<"Nu au acelasi numar de noduri";
    else
    {
        //cele 2 matrice au nr de noduri egal
        for(i = 0; i < matrice3.noduri; i++)
            for(j = 0; j < matrice3.noduri; j++)
                if(matrice1.copie[i][j] + matrice2.copie[i][j] == 2) //daca amandoua au 1 in matrice atunci si matricea rezultata va avea 1
                {
                    matrice3.copie[i][j] = 1;
                    matrice3.muchii = matrice2.muchii + matrice1.muchii;
                }
                else
                {
                    matrice3.copie[i][j] = matrice1.copie[i][j] + matrice2.copie[i][j];
                    matrice3.muchii = matrice2.muchii + matrice1.muchii;
                }
        //afisarea matricei rezultata
        for(i = 0; i < matrice3.noduri; i++)
        {
            for(j = 0; j < matrice3.noduri; j++)

                g<<matrice3.copie[i][j]<<" ";
            g<<endl;
        }
    }
    return matrice3;

}

bool operator==(const Graph& matrice1, const Graph& matrice2)
{
    int i, j;

    if(matrice1.noduri != matrice2.noduri)
        return false;
    if(matrice1.muchii != matrice2.muchii)
        return false;
    for(i = 0; i < matrice1.noduri; i++)
        for(j = 0; j < matrice1.muchii; j++)
            if(matrice1.copie[i][j] != matrice2.copie[i][j])
                return false;

    return true;
}

int main()
{
    int n, i;
    f>>n;
    Graph graph[n];//creeaza n obiectul graph
    for(i = 0; i <n; i++)
    {
        g<<"GRAPH: "<<i<<endl;
        g<<endl;
        g<<"Matricea de adiacenta este:"<<endl;
        f>>graph[i];
        g<<graph[i];
        g<<endl;
        g<<"Parcurgerea in Adancime este: ";
        graph[i].Parcurgere_Adancime();
        g<<endl;
        g<<"Parcurgerea in Latime este: ";
        graph[i].Parcurgere_Latime();
        g<<endl;
        graph[i].Matrice_Drumuri();
        graph[i].Componente_Conexe();
        g<<endl;
        graph[i].Graf_Conex();
        g<<endl;
    }
    int k, l;
    g<<"Reuniunea celor 2 grafuri: ";
    g<<endl;
    for(k = 0; k < n; k++)
        for(l = k+1; l < n; l++)
        {
            Graph graf3;
            graf3 = graph[k] + graph[l];
        }
    g<<endl;
    g<<"Egalitatea a doua matrici: ";
    g<<endl;
    for(k = 0; k < n; k++)
    {
        for(l = k+1; l < n; l++)
            if(graph[k] == graph[l])
                g<<"Cele doua sunt egale!";
            else
                g<<"Cele doua nu sunt egale";
    }

    f.close();
    g.close();
    return 0;
}
