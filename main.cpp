#include <iostream>
#include<string.h>
#include<bits/stdc++.h>


using namespace std;


class Graf
{
private:
    int nr_noduri;
public:
    //constructori
    Graf();
    Graf(int nr_noduri);
    //operatorii de copiere
    Graf(const Graf& g);
    Graf& operator=(const Graf& g);
    //functiile de cititre si afisare
    friend ifstream& operator>>(ifstream& in,Graf& g);
    //destructorul
    ~Graf();
    //functie virtuala pura
    virtual void scrie_tip_graf()=0;
    virtual int begin_dfs()=0;
    virtual void bfs(int nod_start)=0;
    //setteri si getteri
    int get_nr_noduri();

    //citire si afisare virtuala
    virtual ifstream& citire_graf_virtuala_fisier(ifstream& in);




};

int Graf::get_nr_noduri()
{
    return this->nr_noduri;
}
Graf::Graf()
{
    this -> nr_noduri = 0;
}

Graf::Graf(int nr_noduri)
{
    this -> nr_noduri = nr_noduri;
}

Graf::Graf(const Graf& g)
{
    this -> nr_noduri = g.nr_noduri;
}

Graf& Graf::operator=(const Graf& g)
{
    if(this != &g)
    {
        this -> nr_noduri = g.nr_noduri;
    }
    return *this;
}
ifstream& Graf::citire_graf_virtuala_fisier(ifstream& in)
{
    in>>this -> nr_noduri;
    return in;
}
ifstream& operator>>(ifstream& in,Graf& g)
{
    return g.citire_graf_virtuala_fisier(in);
}
Graf::~Graf(){};


class Graf_Neorientat:public Graf
{
private:
    int numar_muchii;
    unordered_map<int,vector<int>>lista_adiacenta;
public:
    //constructori
    Graf_Neorientat();
    Graf_Neorientat(int nr_noduri,int numar_muchii,unordered_map <int,vector<int>> lista_adiacenta);
    //operatorii de copiere
    Graf_Neorientat(const Graf_Neorientat& g);
    Graf_Neorientat& operator=(const Graf_Neorientat& g);
    //functiile de cititre si afisare
    friend ifstream& operator>>(ifstream& in,Graf_Neorientat& g);
    //destructorul
    ~Graf_Neorientat();
    //functii
    static void havel_hakimi(vector<int> grade);
    int begin_biconex();
    virtual void scrie_tip_graf();
    virtual int begin_dfs();
    void dfs(int nod, int viz[]);
    void dfs_biconex(int nod_start,int viz[],int lista_intoarcere[],int lista_nivel[],int nivel,int mat[],stack<pair<int,int>> &stiva_muchii,unordered_map<int,vector<int>> &lista_fii);
    void dfs_biconex_stiva(int nod_start,int viz[],int lista_intoarcere[],int lista_nivel[],stack<pair<int,int>> &stiva_muchii,set<int,less<int>> &componenta_biconexa,int lista_noduri_critice[]);
    virtual void bfs(int nod_start);
    static void dfs_muchie_critica(int n,int nod,int viz[],int lista_intoarcere[],int lista_nivel[],int nivel,unordered_map<int,vector<int>> lista_adiacenta1,int mat[],int lista_tati[]);
    static vector<vector<int>> muchii_critice(int n,vector<vector<int>> muchii);
    //setteri si getteriofstream fout("biconex.out");
    int get_nr_muchii();
    unordered_map<int,vector<int>> get_lista_adiacenta();


    //citire si afisare virtuala
    virtual ifstream& citire_graf_virtuala_fisier(ifstream& in);


};
void Graf_Neorientat::dfs_muchie_critica(int n,int nod,int viz[],int lista_intoarcere[],int lista_nivel[],int nivel,unordered_map<int,vector<int>> lista_adiacenta1,int mat[],int lista_tati[])
{
    viz[nod]=1;
    lista_nivel[nod]=nivel;
    lista_intoarcere[nod]=nivel;
    if(lista_adiacenta1.find(nod)!=lista_adiacenta1.end())
    {
        for(int i=0; i<lista_adiacenta1[nod].size(); i++)
            if(viz[lista_adiacenta1[nod][i]]==0)
            {
                mat[lista_adiacenta1[nod][i]*n + nod] = 1;
                mat[nod*n + lista_adiacenta1[nod][i]] =1;
                lista_tati[lista_adiacenta1[nod][i]]=nod;
                dfs_muchie_critica(n,lista_adiacenta1[nod][i],viz,lista_intoarcere,lista_nivel,nivel+1,lista_adiacenta1,mat,lista_tati);
                lista_intoarcere[nod] = min(lista_intoarcere[nod],lista_intoarcere[lista_adiacenta1[nod][i]]);
            }
            else
            {
                if(mat[nod*n + lista_adiacenta1[nod][i]]==0)
                    lista_intoarcere[nod] = min(lista_intoarcere[nod],lista_intoarcere[lista_adiacenta1[nod][i]]);
            }
    }

}
vector<vector<int>> Graf_Neorientat::muchii_critice(int n,vector<vector<int>> connections)
{

    int* viz = new int[n];
    int* lista_nivel = new int[n];
    int* lista_intoarcere=new int[n];
    int* mat = new int[n*n];
    int* lista_tati = new int[n];
    for(int i=0; i<n; i++)
    {
        viz[i]=0;
        lista_nivel[i]=0;
        lista_intoarcere[i]=0;
        lista_tati[i]=-1;
    }
    for(int i=0; i<n*n; i++)
        mat[i]=0;


    unordered_map<int,vector<int>>lista_adiacenta1;
    for(int i=0; i<connections.size(); i++)
    {
        lista_adiacenta1[connections[i][0]].push_back(connections[i][1]);
        lista_adiacenta1[connections[i][1]].push_back(connections[i][0]);

    }
    dfs_muchie_critica(n,0,viz,lista_intoarcere,lista_nivel,0,lista_adiacenta1,mat,lista_tati);
    vector<vector<int>> solutie;
    for(int i=0; i<connections.size(); i++)
    {
        if(lista_tati[connections[i][1]]==connections[i][0])
        {
            if(lista_intoarcere[connections[i][1]]>lista_nivel[connections[i][0]])
            {

                vector<int> muchie_critica;
                muchie_critica = connections[i];
                solutie.push_back(muchie_critica);
            }
        }
        else if(lista_tati[connections[i][0]]==connections[i][1])
        {
            if(lista_intoarcere[connections[i][0]]>lista_nivel[connections[i][1]])
            {
                vector<int> muchie_critica;
                muchie_critica = connections[i];
                solutie.push_back(muchie_critica);
            }
        }


    }

    delete[] lista_tati;
    delete[] viz;
    delete[] lista_nivel;
    delete[] lista_intoarcere;
    delete[] mat;
    return solutie;
}
void Graf_Neorientat::havel_hakimi(vector<int> grade)
{
    if(grade[0]>=grade.size())
        cout<<"NU";
    else if(grade[0]<0)
        cout<<"NU";
    else if(grade[0]==0)
    {
        int ok=1;
        for(int i=0;i<grade.size();i++)
            if(grade[i]!=0)
                ok=0;
        if(ok==1)
            cout<<"DA";
        else
            cout<<"NU";
    }
    else
    {
        int grad1 =grade[0];
        for(int i=1;i<grade.size();i++)
        {
            if(i<=grad1)
                grade[i]--;
            grade[i-1]=grade[i];
        }
        grade.pop_back();
        sort(grade.begin(), grade.end(), greater<int>());
        havel_hakimi(grade);

    }
}
ofstream fout3("biconex.out");
void Graf_Neorientat::dfs_biconex_stiva(int nod,int viz[],int lista_intoarcere[],int lista_nivel[],stack<pair<int,int>> &stiva_muchii,set<int,less<int>> &componenta_biconexa,int lista_noduri_critice[])
{
    viz[nod] = 1;
    int nr_noduri2 = Graf_Neorientat::get_nr_noduri();

    if(lista_adiacenta.find(nod)!=lista_adiacenta.end())
    {
        for(int i=1;i<=lista_adiacenta[nod].size();i++)
            if(viz[lista_adiacenta[nod][i-1]] == 0)
        {
            pair<int,int> muchie_curenta;
            muchie_curenta.first = nod;
            muchie_curenta.second = lista_adiacenta[nod][i-1];
            stiva_muchii.push(muchie_curenta);
            dfs_biconex_stiva(lista_adiacenta[nod][i-1],viz,lista_intoarcere,lista_nivel,stiva_muchii,componenta_biconexa,lista_noduri_critice);
            if(lista_noduri_critice[muchie_curenta.first]==1 && lista_intoarcere[muchie_curenta.second]>=lista_nivel[muchie_curenta.first])
               {
                while(stiva_muchii.top().first!=muchie_curenta.first||stiva_muchii.top().second!=muchie_curenta.second)
                {
                    componenta_biconexa.insert(stiva_muchii.top().second);
                    componenta_biconexa.insert(stiva_muchii.top().first);
                    stiva_muchii.pop();

                }
                componenta_biconexa.insert(stiva_muchii.top().second);
                componenta_biconexa.insert(stiva_muchii.top().first);
                stiva_muchii.pop();
                set<int, less<int> >::iterator itr;
                for(itr=componenta_biconexa.begin();itr!=componenta_biconexa.end();itr++)
                    fout3<<*itr<<" ";
                fout3<<"\n";
                componenta_biconexa.clear();
            }
        }
    }
}
void Graf_Neorientat::dfs_biconex(int nod,int viz[],int lista_intoarcere[],int lista_nivel[],int nivel,int mat[],stack<pair<int,int>> &stiva_muchii,unordered_map<int,vector<int>> &lista_fii)
{
        viz[nod] = 1;
        lista_nivel[nod] = nivel;
        lista_intoarcere[nod] = nivel;
        int nr_noduri2 = Graf_Neorientat::get_nr_noduri()+1;
        if(lista_adiacenta.find(nod)!=lista_adiacenta.end())
        {
            for(int i=1;i<=lista_adiacenta[nod].size();i++)
                if(viz[lista_adiacenta[nod][i-1]]==0)
                {
                    //cout<<nod<<" "<<lista_adiacenta[nod][i-1]<<"\n";
                    lista_fii[nod].push_back(lista_adiacenta[nod][i-1]);
                    mat[lista_adiacenta[nod][i-1]*nr_noduri2 + nod] = 1;
                    mat[nod*nr_noduri2 + lista_adiacenta[nod][i-1]] =1;
                    pair<int,int> muchie_curenta;
                    muchie_curenta.first = nod;
                    muchie_curenta.second = lista_adiacenta[nod][i-1];
                    stiva_muchii.push(muchie_curenta);
                    dfs_biconex(lista_adiacenta[nod][i-1],viz,lista_intoarcere,lista_nivel,nivel+1,mat,stiva_muchii,lista_fii);
                    lista_intoarcere[nod] = min(lista_intoarcere[nod],lista_intoarcere[lista_adiacenta[nod][i-1]]);
                }
                else
                {
                    if(mat[nod* nr_noduri2 + lista_adiacenta[nod][i-1]]==0)
                    lista_intoarcere[nod] = min(lista_intoarcere[nod],lista_intoarcere[lista_adiacenta[nod][i-1]]);//might need some remaking
                }
        }
}
int Graf_Neorientat::begin_biconex()
{
    int nr_componente_biconexe =0;
    int lista_nivel[Graf_Neorientat::get_nr_noduri()+1];
    int lista_intoarcere[Graf_Neorientat::get_nr_noduri()+1];
    int lista_noduri_critice[Graf_Neorientat::get_nr_noduri()+1]={0};
    int viz[Graf_Neorientat::get_nr_noduri()+1]={0};
    int matrice_muchii_dfs[(Graf_Neorientat::get_nr_noduri()+1)*(Graf_Neorientat::get_nr_noduri()+1)] ={0};
    stack<pair<int,int>> stiva_muchii_dfs;
    unordered_map<int,vector<int>> lista_fii;
    dfs_biconex(1,viz,lista_intoarcere,lista_nivel,0,matrice_muchii_dfs,stiva_muchii_dfs,lista_fii);
    for(int i=1;i<=Graf_Neorientat::get_nr_noduri()+1;i++)
    {
        if(lista_fii.find(i)!=lista_fii.end())
            for(int j=0;j<lista_fii[i].size();j++)
                if(lista_intoarcere[lista_fii[i][j]]>=lista_nivel[i])
                {
                    lista_noduri_critice[i]=1;
                    break;
                }
    }
    while(!stiva_muchii_dfs.empty())
    {

        if(lista_noduri_critice[stiva_muchii_dfs.top().first]==1 && lista_intoarcere[stiva_muchii_dfs.top().second]>=lista_nivel[stiva_muchii_dfs.top().first])
            nr_componente_biconexe++;
        stiva_muchii_dfs.pop();

    }
    fout3<<nr_componente_biconexe<<"\n";
    set<int,less<int>> componenta_biconexa;
    stack<pair<int,int>> stiva_muchii_dfs_curenta;
    int viz2[Graf_Neorientat::get_nr_noduri()+1] = {0};
    dfs_biconex_stiva(1,viz2,lista_intoarcere,lista_nivel,stiva_muchii_dfs_curenta,componenta_biconexa,lista_noduri_critice);


    cout<<"Lista nivel ";
    for(int i=1;i<=Graf_Neorientat::get_nr_noduri();i++)
        cout<<i<<":"<<lista_nivel[i]<<" ";
    cout<<"\n\nLista intoarcere ";
    for(int i=1;i<=Graf_Neorientat::get_nr_noduri();i++)
        cout<<lista_intoarcere[i]<<" ";
    cout<<"\n\nLista noduri critice ";

    for(int i=1;i<=Graf_Neorientat::get_nr_noduri();i++)
        if(lista_noduri_critice[i]==1)
            cout<<i<<" ";
    cout<<"\n\n";






    return 1;




}
void Graf_Neorientat::bfs(int nod_start)
{
    int viz[Graf::get_nr_noduri()+1]={0};
    ofstream fout("bfs.out");
    int contor = 0;
    int distanta[Graf::get_nr_noduri()+1]={-1};
    queue<int> bfs_queue;
    bfs_queue.push(nod_start);
    viz[nod_start] = 1;
    while(!bfs_queue.empty())
    {
        contor++;
        int front_queue = bfs_queue.front();
        if(lista_adiacenta.find(front_queue)!=lista_adiacenta.end())
        {
            for(int i=1;i<=lista_adiacenta[front_queue].size();i++)
            {
                if(viz[lista_adiacenta[front_queue][i-1]]==0)
                {
                    bfs_queue.push(lista_adiacenta[front_queue][i-1]);
                    distanta[lista_adiacenta[front_queue][i-1]]=contor;
                    viz[lista_adiacenta[front_queue][i-1]]=1;
                }
            }
        }
        bfs_queue.pop();
    }
    distanta[nod_start] = 0;
    for(int i=1;i<=Graf::get_nr_noduri();i++)
        fout<<distanta[i]<<" ";
}
void Graf_Neorientat::dfs(int nod,int viz[])
{
        viz[nod] = 1;
        if(lista_adiacenta.find(nod)!=lista_adiacenta.end())
        {
            for(int i=1;i<=lista_adiacenta[nod].size();i++)
                if(viz[lista_adiacenta[nod][i-1]]==0)
                    dfs(lista_adiacenta[nod][i-1],viz);
        }

}
int Graf_Neorientat::begin_dfs()
{
    int viz[Graf::get_nr_noduri()+1] ={0};
    int contor=0;
    for(int i=1;i<=Graf::get_nr_noduri();i++)
        if(viz[i]==0)
    {
        contor++;
        Graf_Neorientat::dfs(i,viz);
    }
    return contor;

}
void Graf_Neorientat::scrie_tip_graf()
{
    cout<<"\nGraf Neorientat";
}
int Graf_Neorientat::get_nr_muchii()
{
    return this -> numar_muchii;
}
unordered_map<int,vector<int>> Graf_Neorientat::get_lista_adiacenta()
{
    return this -> lista_adiacenta;
}
Graf_Neorientat::Graf_Neorientat():Graf()
{
    this -> numar_muchii = 0;
}
Graf_Neorientat::Graf_Neorientat(int nr_noduri,int numar_muchii,unordered_map <int,vector<int>> lista_adiacenta):Graf(nr_noduri)
{

    this->numar_muchii = numar_muchii;
    this->lista_adiacenta = lista_adiacenta;
}
Graf_Neorientat::Graf_Neorientat(const Graf_Neorientat& g):Graf(g)
{
    this -> numar_muchii = g.numar_muchii;
    this -> lista_adiacenta= g.lista_adiacenta;
}
Graf_Neorientat& Graf_Neorientat::operator=(const Graf_Neorientat& g)
{
    if(this!= &g)
    {
        Graf::operator=(g);
        this->numar_muchii = g.numar_muchii;
        this->lista_adiacenta = g.lista_adiacenta;
    }
    *this;
}

ifstream& Graf_Neorientat::citire_graf_virtuala_fisier(ifstream& in)
{
    Graf::citire_graf_virtuala_fisier(in);
    in>>this->numar_muchii;
    for(int i=1;i<=this->numar_muchii;i++)
    {
        int first,second;
        in>>first>>second;
        lista_adiacenta[first].push_back(second);
        lista_adiacenta[second].push_back(first);
    }
    return in;
}
ifstream& operator>>(ifstream& in,Graf_Neorientat& g)
{
    return g.citire_graf_virtuala_fisier(in);
}
Graf_Neorientat::~Graf_Neorientat(){}

class Graf_Orientat:public Graf
{
private:
    int numar_muchii;
    unordered_map<int,vector<int>>lista_adiacenta;
public:
    //constructori
    Graf_Orientat();
    Graf_Orientat(int nr_noduri,int numar_muchii,unordered_map <int,vector<int>> lista_adiacenta);
    //operatorii de copiere
    Graf_Orientat(const Graf_Orientat& g);
    Graf_Orientat& operator=(const Graf_Orientat& g);
    //functiile de cititre si afisare
    friend ifstream& operator>>(ifstream& in,Graf_Orientat& g);
    //destructorul
    ~Graf_Orientat();
    //functie virtuala pura
    virtual void scrie_tip_graf();
    virtual int begin_dfs();
    void dfs(int nod, int viz[]);
    virtual void bfs(int nod_start);
    void dfs_stiva(int nod,int viz[],stack<int> &noduri);
    void dfs_tare_conex(int nod,int viz[],set<int,less<int>> &componenta_tare_conexa);
    void begin_componente_tare_conexe();
    void begin_sortaret();
    //setteri si getteri
    int get_nr_muchii();
    unordered_map<int,vector<int>> get_lista_adiacenta();


    //citire si afisare virtuala
    virtual ifstream& citire_graf_virtuala_fisier(ifstream& in);


};
void Graf_Orientat::begin_sortaret()
{
    int numar_noduri = Graf_Orientat::get_nr_noduri();
    int grad_interior[numar_noduri+1]={0};
    int viz[numar_noduri+1] = {0};
    for(int i=1;i<=numar_noduri;i++)
    {
        if(lista_adiacenta.find(i)!=lista_adiacenta.end())
            for(int j=0;j<lista_adiacenta[i].size();j++)
                grad_interior[lista_adiacenta[i][j]]++;
    }
    queue<int> coada_noduri;
    for(int i=1;i<=numar_noduri;i++)
        if(grad_interior[i]==0)
        {
            coada_noduri.push(i);
            viz[i] = 1;
        }
    ofstream fout4("sortaret.out");
    while(!coada_noduri.empty())
    {
        fout4<<coada_noduri.front()<<" ";
        if(lista_adiacenta.find(coada_noduri.front())!=lista_adiacenta.end())
            for(int i=0;i<lista_adiacenta[coada_noduri.front()].size();i++)
            {
                grad_interior[lista_adiacenta[coada_noduri.front()][i]]--;
                if(grad_interior[lista_adiacenta[coada_noduri.front()][i]]==0)
                {
                    viz[lista_adiacenta[coada_noduri.front()][i]]=1;
                    coada_noduri.push(lista_adiacenta[coada_noduri.front()][i]);
                }

            }
        coada_noduri.pop();
    }
}
void Graf_Orientat::dfs_stiva(int nod,int viz[],stack<int> &noduri)
{
    viz[nod] = 1;
    int numar_noduri = Graf_Orientat::get_nr_noduri();
    if(lista_adiacenta.find(nod)!=lista_adiacenta.end())
        for(int i=0;i<lista_adiacenta[nod].size();i++)
        {
            if(viz[lista_adiacenta[nod][i]]==0)
                dfs_stiva(lista_adiacenta[nod][i],viz,noduri);
        }
    noduri.push(nod);
}
void Graf_Orientat::dfs_tare_conex(int nod,int viz[],set<int,less<int>> &componenta_tare_conexa)
{
    viz[nod]=1;
    componenta_tare_conexa.insert(nod);
    if(lista_adiacenta.find(nod)!=lista_adiacenta.end())
        for(int i=0;i<lista_adiacenta[nod].size();i++)
        {
            if(viz[lista_adiacenta[nod][i]]==0)
                dfs_tare_conex(lista_adiacenta[nod][i],viz,componenta_tare_conexa);
        }
}
void Graf_Orientat::begin_componente_tare_conexe()
{
    int numar_noduri = Graf_Orientat::get_nr_noduri();
    int viz[numar_noduri+1]= {0};
    int nr_componente_tare_conexe=0;
    ofstream fout2("ctc.out");
    vector<set<int,less<int>>> componente_conexe;
    for(int k=1; k<=numar_noduri; k++)
    {
        if(viz[k]==0)
        {
            stack<int> noduri;
            dfs_stiva(k,viz,noduri);
            int numar_muchii2 = Graf_Orientat::get_nr_muchii();
            unordered_map<int,vector<int>> lista_ad;
            for(int i=1; i<=numar_noduri; i++)
            {
                if(viz[i]==1)
                    if(lista_adiacenta.find(i)!=lista_adiacenta.end())
                        for(int j=0; j<lista_adiacenta[i].size(); j++)
                            lista_ad[lista_adiacenta[i][j]].push_back(i);
            }
            Graf_Orientat g2(numar_noduri,numar_muchii2,lista_ad);
            int viz2[numar_noduri+1]= {0};
            stack<int> noduri_copy=noduri;
            while(!noduri_copy.empty())
            {
                set<int,less<int>> componenta_tare_conexa;
                if(viz2[noduri_copy.top()]==0)
                {
                    g2.dfs_tare_conex(noduri_copy.top(),viz2,componenta_tare_conexa);
                    if(componenta_tare_conexa.size()!=0)
                    {
                        componente_conexe.push_back(componenta_tare_conexa);
                        componenta_tare_conexa.clear();
                    }
                }
                noduri_copy.pop();

            }

        }
    }
    fout2<<componente_conexe.size()<<"\n";
    for(int i=0;i<componente_conexe.size();i++)
    {
        for(auto itr=componente_conexe[i].begin();itr!=componente_conexe[i].end();itr++)
            fout2<<*itr<<" ";
        fout2<<"\n";
    }
}


void Graf_Orientat::bfs(int nod_start)
{
    int viz[Graf::get_nr_noduri()+1]={0};
    ofstream fout("bfs.out");
    int contor = 1;
    int distanta[Graf::get_nr_noduri()+1];
    for(int i=1;i<=Graf::get_nr_noduri();i++)
        distanta[i]=-1;
    queue<int> bfs_queue;
    bfs_queue.push(nod_start);
    viz[nod_start] = 1;
    distanta[nod_start]=0;
    while(!bfs_queue.empty())
    {

        int front_queue = bfs_queue.front();
        if(lista_adiacenta.find(front_queue)!=lista_adiacenta.end())
        {
            for(int i=1;i<=lista_adiacenta[front_queue].size();i++)
            {
                if(viz[lista_adiacenta[front_queue][i-1]]==0)
                {
                    bfs_queue.push(lista_adiacenta[front_queue][i-1]);
                    distanta[lista_adiacenta[front_queue][i-1]]=distanta[front_queue]+1;
                    viz[lista_adiacenta[front_queue][i-1]]=1;
                    cout<<"\n"<<front_queue<<" "<<lista_adiacenta[front_queue][i-1]<<" "<<contor;

                }
            }
        }
        bfs_queue.pop();
    }
    distanta[nod_start] = 0;
    for(int i=1;i<=Graf::get_nr_noduri();i++)
        fout<<distanta[i]<<" ";
}
void Graf_Orientat::dfs(int nod,int viz[])
{
        viz[nod] = 1;
        if(lista_adiacenta.find(nod)!=lista_adiacenta.end())
        {
            for(int i=1;i<=lista_adiacenta[nod].size();i++)
                if(viz[lista_adiacenta[nod][i-1]]==0)
                    dfs(lista_adiacenta[nod][i-1],viz);
        }

}
int Graf_Orientat::begin_dfs()
{
    int viz[Graf::get_nr_noduri()+1] ={0};
    int contor=0;
    for(int i=1;i<=Graf::get_nr_noduri();i++)
        if(viz[i]==0)
    {
        contor++;
        Graf_Orientat::dfs(i,viz);
    }
    return contor;

}
void Graf_Orientat::scrie_tip_graf()
{
    cout<<"\nGraf Orientat";
}
int Graf_Orientat::get_nr_muchii()
{
    return this -> numar_muchii;
}
unordered_map<int,vector<int>> Graf_Orientat::get_lista_adiacenta()
{
    return this -> lista_adiacenta;
}
Graf_Orientat::Graf_Orientat():Graf()
{
    this -> numar_muchii = 0;
}
Graf_Orientat::Graf_Orientat(int nr_noduri,int numar_muchii,unordered_map <int,vector<int>> lista_adiacenta):Graf(nr_noduri)
{
    this->numar_muchii = numar_muchii;
    this->lista_adiacenta = lista_adiacenta;
}
Graf_Orientat::Graf_Orientat(const Graf_Orientat& g):Graf(g)
{
    this -> numar_muchii = g.numar_muchii;
    this -> lista_adiacenta= g.lista_adiacenta;
}
Graf_Orientat& Graf_Orientat::operator=(const Graf_Orientat& g)
{
    if(this!= &g)
    {
        Graf::operator=(g);
        this->numar_muchii = g.numar_muchii;
        this->lista_adiacenta = g.lista_adiacenta;
    }
    *this;
}

ifstream& Graf_Orientat::citire_graf_virtuala_fisier(ifstream& in)
{
    Graf::citire_graf_virtuala_fisier(in);
    in>>this->numar_muchii;
    for(int i=1;i<=this->numar_muchii;i++)
    {
        int first,second;
        in>>first>>second;
        int ok =1;
        if(lista_adiacenta.find(first)!=lista_adiacenta.end())
            for(int i=0;i<lista_adiacenta[first].size();i++)
                if(second==lista_adiacenta[first][i])
            {
                ok=0;
                break;
            }
        if(ok==1)
            lista_adiacenta[first].push_back(second);
    }
    return in;
}
ifstream& operator>>(ifstream& in,Graf_Orientat& g)
{
    return g.citire_graf_virtuala_fisier(in);
}
Graf_Orientat::~Graf_Orientat(){}


int main()
{
    //exercitiul DFS
       /* ifstream fin("dfs.in");
        ofstream fout("dfs.out");
        Graf_Neorientat g;
        fin>>g;
        fout<<g.begin_dfs();*/

    //exercitiul BFS
       /* ifstream fin("bfs.in");
        ofstream fout("bfs.out");
        int nr_noduri,nr_muchii,nod_start;
        unordered_map<int,vector<int>> lista_adiacenta;
        fin>>nr_noduri>>nr_muchii>>nod_start;
        for(int i=0;i<nr_muchii;i++)
        {
            int first,second;
            fin>>first>>second;
            lista_adiacenta[first].push_back(second);
        }
        Graf_Orientat g(nr_noduri,nr_muchii,lista_adiacenta);
        g.bfs(nod_start);*/

    //exerctiul Biconexe
       /*ifstream fin("biconex.in");
        Graf_Neorientat g;
        fin>>g;
        g.begin_biconex();*/

    //exercitiul CTC
        /*ifstream fin("ctc.in");
        Graf_Orientat g;
        fin>>g;
        g.begin_componente_tare_conexe();*/
    //exercitiul Sortaret
        /*ifstream fin("sortaret.in");
        Graf_Orientat g;
        fin>>g;
        g.begin_sortaret();*/
    //exercitiul havel hakimi
     /*   int n;
        cout<<"Numarul de noduri =";
        cin>>n;
        vector<int> grade;
        for(int i=0;i<n;i++)
        {
            int grad;
            cout<<"Grad "<<i+1<<" =";
            cin>>grad;
            grade.push_back(grad);
        }
        sort(grade.begin(), grade.end(), greater<int>());
        Graf_Neorientat::havel_hakimi(grade);*/
    //exercitiul muchie critica
       /* int n,m;
        cout<<"Numarul de noduri = ";
        cin>>n;
        cout<<"Numarul de muchii = ";
        cin>>m;
        vector<vector<int>> muchii;
        for(int i=0;i<m;i++)
        {
            int prim,sec;
            cout<<"Muchia "<<i+1<<" :";
            cin>>prim>>sec;
            vector<int> muchie_curenta;
            muchie_curenta.push_back(prim);
            muchie_curenta.push_back(sec);
            muchii.push_back(muchie_curenta);

        }
        vector<vector<int>> sol=Graf_Neorientat::muchii_critice(n,muchii);
        for(int i=0;i<sol.size();i++)
        {
            cout<<sol[i][0]<<" "<<sol[i][1]<<"\n";
        }*/






    return 0;
}
