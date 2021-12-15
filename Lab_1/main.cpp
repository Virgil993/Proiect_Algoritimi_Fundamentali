#include <iostream>
#include<string.h>
#include<bits/stdc++.h>

#define NMAX 1024

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
    virtual string tip_graf()=0;
    //setteri si getteri
    int get_nr_noduri();
    void set_nr_noduri(int n);

    //citire si afisare virtuala
    virtual ifstream& citire_graf_virtuala_fisier(ifstream& in);




};
void Graf::set_nr_noduri(int n)
{
    this->nr_noduri=n;
}
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


    //functie ajutatoare pentru a afla muchiile critice dintr-un graf
    static void dfs_muchie_critica(int n,int nod,int nivel,unordered_map<int,vector<int>> lista_adiacenta1,vector<int> &viz,vector<int> &lista_intoarcere,vector<int> &lista_nivel,vector<int> &mat,vector<int> &lista_tati);

    //functii ajutatoare pentru a afla componentele biconexe ale unui graf
    void dfs_biconex(int nod_start,int nivel,vector<int> &viz,vector<int> &lista_intoarcere,vector<int> &lista_nivel,vector<int> &mat,unordered_map<int,vector<int>> &lista_fii);
    void dfs_biconex_stiva(int nod_start,vector<int> &viz,vector<int> lista_intoarcere,vector<int> lista_nivel,vector<int> lista_noduri_critice,vector<set<int,less<int>>> &solutie,stack<pair<int,int>> &stiva_muchii);

    //functie ajutatoare pentru a afla cate componente conexe are un graf
    void dfs(int nod, vector<int> &viz);

    //functie ajutatoare pentru a afla diametru unui arbore
    int DFS_DARB(int start,int contor,vector<int> &viz);
    static int DFS_DARB_INFO_ARENA(int start,int contor,vector<int> &viz,vector<vector<int>> &lista_ad);
public:
    //constructori
    Graf_Neorientat();
    Graf_Neorientat(int nr_noduri,int numar_muchii,unordered_map <int,vector<int>> lista_adiacenta);
    //operatorii de copiere
    Graf_Neorientat(const Graf_Neorientat& g);
    Graf_Neorientat& operator=(const Graf_Neorientat& g);
    //functiile de cititre si afisare
    friend ifstream& operator>>(ifstream& in,Graf_Neorientat& g);
    static void REZOLVA_DARB(string fisier_in,string fisier_out);
    //destructorul
    ~Graf_Neorientat();
    //functii
    static bool havel_hakimi(vector<int> grade);
    vector<set<int,less<int>>> begin_biconex();
    string tip_graf();
    int begin_dfs();
    int BFS_DFS_DARB(int start);
    static int BFS_DFS_DARB_INFO_ARENA(int start,vector<vector<int>> &lista_ad);
    static vector<vector<int>> muchii_critice(int n,vector<vector<int>> muchii);
    //getteri
    int get_nr_muchii();
    unordered_map<int,vector<int>> get_lista_adiacenta();


    //citire si afisare virtuala
    virtual ifstream& citire_graf_virtuala_fisier(ifstream& in);


};
int Graf_Neorientat::DFS_DARB(int start,int contor,vector<int> &viz)
{
    int d_max=contor;
    viz[start]=1;
    if(lista_adiacenta.find(start)!=lista_adiacenta.end())
        for(int i=0;i<lista_adiacenta[start].size();i++)
            if(viz[lista_adiacenta[start][i]]==0)
    {
        d_max=max(d_max,DFS_DARB(lista_adiacenta[start][i],contor+1,viz));
    }
    return d_max;
}
int Graf_Neorientat::BFS_DFS_DARB(int start)
{
    vector<int> viz(Graf::get_nr_noduri()+1,0);
    viz[start]=1;
    queue<int> coada;
    coada.push(start);
    int nod_start;
    while(!coada.empty())
    {
        int nod_curent=coada.front();
        coada.pop();
        if(lista_adiacenta.find(nod_curent)!=lista_adiacenta.end())
            for(int i=0;i<lista_adiacenta[nod_curent].size();i++)
                if(viz[lista_adiacenta[nod_curent][i]]==0)
                {
                    viz[lista_adiacenta[nod_curent][i]]=1;
                    coada.push(lista_adiacenta[nod_curent][i]);
                }
        if(coada.empty())
            nod_start=nod_curent;
    }
    for(int i=0;i<viz.size();i++)
        viz[i]=0;
    int diametru=0;
    diametru=max(diametru,DFS_DARB(nod_start,1,viz));
    return diametru;
}

int Graf_Neorientat::BFS_DFS_DARB_INFO_ARENA(int start,vector<vector<int>> &lista_ad)
{
    vector<int> viz(lista_ad.size()+1,0);
    viz[start]=1;
    queue<int> coada;
    coada.push(start);
    int nod_start;
    while(!coada.empty())
    {
        int nod_curent=coada.front();
        coada.pop();
            for(int i=0;i<lista_ad[nod_curent].size();i++)
                if(viz[lista_ad[nod_curent][i]]==0)
                {
                    viz[lista_ad[nod_curent][i]]=1;
                    coada.push(lista_ad[nod_curent][i]);
                }
        if(coada.empty())
            nod_start=nod_curent;
    }
    for(int i=0;i<viz.size();i++)
        viz[i]=0;
    int diametru=0;
    diametru=max(diametru,DFS_DARB_INFO_ARENA(nod_start,1,viz,lista_ad));
    return diametru;
}

int Graf_Neorientat::DFS_DARB_INFO_ARENA(int start,int contor,vector<int> &viz,vector<vector<int>> &lista_ad)
{
    int d_max=contor;
    viz[start]=1;
    for(int i=0;i<lista_ad[start].size();i++)
        if(viz[lista_ad[start][i]]==0)
            d_max=max(d_max,DFS_DARB_INFO_ARENA(lista_ad[start][i],contor+1,viz,lista_ad));
    return d_max;
}
void Graf_Neorientat::REZOLVA_DARB(string fisier_in,string fisier_out)
{
    ifstream fin(fisier_in);
    ofstream fout(fisier_out);
    int n;
    fin>>n;
    vector<vector<int>> lista_ad(n+1);
    int radacina;
    for(int i=0;i<n-1;i++)
    {
        int first,second;
        fin>>first>>second;
        lista_ad[first].push_back(second);
        lista_ad[second].push_back(first);
        if(i==0)
            radacina=first;
    }
    fout<<BFS_DFS_DARB_INFO_ARENA(radacina,lista_ad);

}

void Graf_Neorientat::dfs_muchie_critica(int n,int nod,int nivel,unordered_map<int,vector<int>> lista_adiacenta1,vector<int> &viz,vector<int> &lista_intoarcere,vector<int> &lista_nivel,vector<int> &mat,vector<int> &lista_tati)
{
    viz[nod]=1;//marcam nodul ca fiind vizitat
    lista_nivel[nod]=nivel;//initializam nivelul nodului
    lista_intoarcere[nod]=nivel;//initializam nivelul de intoarcere al nodului cu nivelul curent
    if(lista_adiacenta1.find(nod)!=lista_adiacenta1.end())//daca nodul curent are vecini ii parcurgem
    {
        for(int i=0; i<lista_adiacenta1[nod].size(); i++)
            if(viz[lista_adiacenta1[nod][i]]==0)//gasit un vecin nevizitat
            {
                mat[lista_adiacenta1[nod][i]*n + nod] = 1;//notam muchia in matricea mat ca fiind vizitata pentru a retine daca face pare din arborele dfs
                mat[nod*n + lista_adiacenta1[nod][i]] =1;
                lista_tati[lista_adiacenta1[nod][i]]=nod;//notam tatal nodului pe care urmeaza sa il vizitam
                dfs_muchie_critica(n,lista_adiacenta1[nod][i],nivel+1,lista_adiacenta1,viz,lista_intoarcere,lista_nivel,mat,lista_tati);//apelam dfs pentru vecin
                lista_intoarcere[nod] = min(lista_intoarcere[nod],lista_intoarcere[lista_adiacenta1[nod][i]]);//la final actualizam nivelul de intoarecere al nodului curent
            }
            else//daca vecinul este vizitat
            {
                if(mat[nod*n + lista_adiacenta1[nod][i]]==0)//daca muchia catre vecin nu face parete din arborele dfs inseamna ca muchia aceasta este de intoarcere
                    lista_intoarcere[nod] = min(lista_intoarcere[nod],lista_intoarcere[lista_adiacenta1[nod][i]]);//actualizam nivelul de intoarcere
            }
    }

}

vector<vector<int>> Graf_Neorientat::muchii_critice(int n,vector<vector<int>> connections)
{
    unordered_map<int,vector<int>>lista_adiacenta1;//lista de vecini pentru fiecare nod
    for(int i=0; i<connections.size(); i++)
    {
        lista_adiacenta1[connections[i][0]].push_back(connections[i][1]);
        lista_adiacenta1[connections[i][1]].push_back(connections[i][0]);

    }
    vector<int> viz(n,0);//vectorul ce retine daca un nod a fost vizitat sau nu
    vector<int> lista_nivel(n,0);//vectorul ce retine nivelul din arborele dfs al fiecarui nod
    vector<int> lista_intoarcere(n,0);//vectorul ce retine nivelul de intoarecere al fiecarui nod
    vector<int> mat(n*n,0);//matricea ce retine daca o muchie face parte sau nu din arborele dfs
    vector<int> lista_tati(n,-1);//vectorul ce retine tatal fiecarui nod din arborele dfs
    dfs_muchie_critica(n,0,0,lista_adiacenta1,viz,lista_intoarcere,lista_nivel,mat,lista_tati);//apelam dfs_muchie_critica pentru a actualiza valorile vectorilor de mai sus
    vector<vector<int>> solutie;//vectorul in care vom retine solutia
    for(int i=0; i<connections.size(); i++)//parcurgem vectorul de muchii
    {
        if(lista_tati[connections[i][1]]==connections[i][0])//daca primul nod este tatal
        {
            if(lista_intoarcere[connections[i][1]]>lista_nivel[connections[i][0]])//daca nivelul de intoarcere al fiului este mai mare decat nivelul tatalui
            {
                //muchia este crtica si o introducem in solutie
                vector<int> muchie_critica;
                muchie_critica = connections[i];
                solutie.push_back(muchie_critica);
            }
        }
        else if(lista_tati[connections[i][0]]==connections[i][1])//daca al doilea nod este tatal
        {
            if(lista_intoarcere[connections[i][0]]>lista_nivel[connections[i][1]])//daca nivelul de intoarcere al fiului este mai mare decat nivelul tatalui
            {
                //muchia este critica si o introducem in solutie
                vector<int> muchie_critica;
                muchie_critica = connections[i];
                solutie.push_back(muchie_critica);
            }
        }
    }
    return solutie;//returnam solutia
}

bool Graf_Neorientat::havel_hakimi(vector<int> grade)
{
    if(grade[0]>=grade.size())//daca primul grad este mai mare decat numarul de noduri-1 atunci este clar ca nu se poate scrie un graf cu aceste grade
        return false;
    else if(grade[0]==0)//daca primul grad = 0 verificam daca toate celelalte grade sunt 0
    {
        int ok=1;
        for(int i=0;i<grade.size();i++)
            if(grade[i]!=0)
                ok=0;
        if(ok==1)
            return true;//daca toate celelalte grade sunt 0 => exista un graf cu aceste grade
        else
            return false;//daca exista un grad care nu este 0 inseamna ca este un nod care are grad < 0 deci returnam false
    }
    else//altfel eliminam primul element si scadem 1 din primele grade[0] noduri  si repetam algoritmul
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
        return havel_hakimi(grade);

    }
}

void Graf_Neorientat::dfs_biconex_stiva(int nod,vector<int> &viz,vector<int> lista_intoarcere,vector<int> lista_nivel,vector<int> lista_noduri_critice,vector<set<int,less<int>>> &solutie,stack<pair<int,int>> &stiva_muchii)
{
    viz[nod] = 1;//marcam nodul ca fiind vizitat
    if(lista_adiacenta.find(nod)!=lista_adiacenta.end())
    {
        for(int i=0;i<lista_adiacenta[nod].size();i++)//parcurgem inca o data arborele dfs
            if(viz[lista_adiacenta[nod][i]] == 0)//daca gasim un vecin nevizitat
        {

            pair<int,int> muchie_curenta;
            muchie_curenta.first = nod;
            muchie_curenta.second = lista_adiacenta[nod][i];
            stiva_muchii.push(muchie_curenta);//adaugam muchia in stiva
            //apelam din nou dfs_biconex_stiva pentru a aduga si urmatoarele muchii in stiva
            dfs_biconex_stiva(lista_adiacenta[nod][i],viz,lista_intoarcere,lista_nivel,lista_noduri_critice,solutie,stiva_muchii);
            //daca nodul curent este critic si fiul sau are nivelul de intoarcere >= decat nivelul tatalui sau atunci inseamna ca toate muchile din ce urmeaza in stiva
            //dupa aceasta muchie(inclusiv) alcatuiesc o componenta biconexa
            if(lista_noduri_critice[muchie_curenta.first]==1 && lista_intoarcere[muchie_curenta.second]>=lista_nivel[muchie_curenta.first])
               {
                set<int,less<int>> componenta_biconexa;//cream un set in care sa retinem nodurile ce alcatuiesc componenta biconexa
                //cat timp in varful stivei nu este muchia curenta adaugam nodurile in set
                while(stiva_muchii.top().first!=muchie_curenta.first||stiva_muchii.top().second!=muchie_curenta.second)
                {
                    componenta_biconexa.insert(stiva_muchii.top().second);
                    componenta_biconexa.insert(stiva_muchii.top().first);
                    stiva_muchii.pop();

                }
                //adaugam si muchia curenta
                componenta_biconexa.insert(stiva_muchii.top().second);
                componenta_biconexa.insert(stiva_muchii.top().first);
                stiva_muchii.pop();
                solutie.push_back(componenta_biconexa);//adaugam setul la vectorul solutie
            }
        }
    }
}

void Graf_Neorientat::dfs_biconex(int nod,int nivel,vector<int> &viz,vector<int> &lista_intoarcere,vector<int> &lista_nivel,vector<int> &mat,unordered_map<int,vector<int>> &lista_fii)
{
        viz[nod] = 1;//marcam nodul curent ca fiind vizitat
        lista_nivel[nod] = nivel;//actualizam nivelul nodului din arborele dfs
        lista_intoarcere[nod] = nivel;//initializam nivelul de intoarcere cu nivelul curent al nodului

        if(lista_adiacenta.find(nod)!=lista_adiacenta.end())
        {
            int nr_noduri2 = Graf_Neorientat::get_nr_noduri()+1;//retinem numarul de noduri pentru a nu apela functia get_nr_noduri de prea multe ori
            for(int i=0;i<lista_adiacenta[nod].size();i++)//parcurgem lista de veicini a nodului
                if(viz[lista_adiacenta[nod][i]]==0)//daca gasim un vecin nevizitat
                {
                    //cout<<nod<<" "<<lista_adiacenta[nod][i-1]<<"\n";
                    lista_fii[nod].push_back(lista_adiacenta[nod][i]);//il adaugam in lista de fii a nodului curent
                    mat[lista_adiacenta[nod][i]*nr_noduri2 + nod] = 1;//actualizam matricea ce retine muchiile ce fac parte din arborele dfs
                    mat[nod*nr_noduri2 + lista_adiacenta[nod][i]] =1;
                    dfs_biconex(lista_adiacenta[nod][i],nivel+1,viz,lista_intoarcere,lista_nivel,mat,lista_fii);//apelam dfs_biconex pentru vecin
                    lista_intoarcere[nod] = min(lista_intoarcere[nod],lista_intoarcere[lista_adiacenta[nod][i]]);//actualizam nivelul de intoarcere al nodului curent
                }
                else//daca gasim un vecin vizitat
                {
                    if(mat[nod* nr_noduri2 + lista_adiacenta[nod][i]]==0)//daca muchia nu face parte din arborele dfs => muchia este de intoarcere
                    lista_intoarcere[nod] = min(lista_intoarcere[nod],lista_intoarcere[lista_adiacenta[nod][i]]);//actualizam nivelul de intoarcere al nodului curent
                }
        }
}

vector<set<int,less<int>>> Graf_Neorientat::begin_biconex()
{
    int nr_noduri2 = Graf_Neorientat::get_nr_noduri();//retinem numarul de noduri ca sa nu apelam functia get de prea multe ori
    vector<int> lista_nivel(nr_noduri2+1,0);//vectorul ce retine nivelul fiecarui nod
    vector<int> lista_intoarcere(nr_noduri2+1,0);//vectorul ce retine nivelul de intoarcere al fiecarui nod
    vector<int> viz(nr_noduri2+1,0);//vectorul ce retine daca un nod este sau nu vizitat
    vector<int> matrice_muchii_dfs((nr_noduri2+1)*(nr_noduri2+1),0);//matricea ce retine muchiile ce fac parte din arborele dfs
    unordered_map<int,vector<int>> lista_fii;//hash-ul ce retine pentru fiecare nod lista de fii din arborele dfs
    //functia dfs biconex actualizeaza vectorii definiti mai sus pentru a putea face operatii rapide cu ei
    dfs_biconex(1,0,viz,lista_intoarcere,lista_nivel,matrice_muchii_dfs,lista_fii);
    vector<int> lista_noduri_critice(nr_noduri2+1);//vectorul ce retine ce noduri sunt critice
    for(int i=1;i<=nr_noduri2+1;i++)//parcurgem toate nodurile
    {
        if(lista_fii.find(i)!=lista_fii.end())
            for(int j=0;j<lista_fii[i].size();j++)//parcugem lista de fii
                if(lista_intoarcere[lista_fii[i][j]]>=lista_nivel[i])//daca pentru nodul i exista cel putin un fiu care are nivelul de intoarcere >= nivelul lui i atunci i este nod critic
                {
                    lista_noduri_critice[i]=1;
                    break; //ne oprim din parcurgerea fiilor
                }
    }
    stack<pair<int,int>> stiva_muchii_dfs_curenta;//stiva ce ne va ajuta sa retinem muchiile pe masura ce le parcurgem in arborele dfs
    vector<int> viz2(nr_noduri2+1,0);//vectorul ce retine daca un nod este sau nu vizitat
    vector<set<int,less<int>>> solutie;//vectorul in care vom retine solutia
    //in functia dfs_biconex_stiva vom construii vecotorul solutie care este transmis prin referinta
    dfs_biconex_stiva(1,viz2,lista_intoarcere,lista_nivel,lista_noduri_critice,solutie,stiva_muchii_dfs_curenta);
    //returnam vectorul solutie
    return solutie;
    /*cout<<"Lista nivel ";
    for(int i=1;i<=Graf_Neorientat::get_nr_noduri();i++)
        cout<<i<<":"<<lista_nivel[i]<<" ";
    cout<<"\n\nLista intoarcere ";
    for(int i=1;i<=Graf_Neorientat::get_nr_noduri();i++)
        cout<<lista_intoarcere[i]<<" ";
    cout<<"\n\nLista noduri critice ";

    for(int i=1;i<=Graf_Neorientat::get_nr_noduri();i++)
        if(lista_noduri_critice[i]==1)
            cout<<i<<" ";
    cout<<"\n\n";*/
}

void Graf_Neorientat::dfs(int nod,vector<int> &viz)
{
        viz[nod] = 1;//notam nodul curent ca fiind vizitat
        if(lista_adiacenta.find(nod)!=lista_adiacenta.end())
        {
            for(int i=0;i<lista_adiacenta[nod].size();i++)//parcurgem toti vecinii nodului curent
                if(viz[lista_adiacenta[nod][i]]==0)//daca gasim un vecin nevizitat apelam dfs de acel veci
                    dfs(lista_adiacenta[nod][i],viz);
        }

}

int Graf_Neorientat::begin_dfs()
{
    int nr_noduri2 = Graf_Neorientat::get_nr_noduri();//retinem numarul de noduri pentru a nu apela functia get de prea multe ori
    vector<int> viz(nr_noduri2+1,0);//initializam vectorul ce retine daca un nod este vizitat sau nu
    int contor=0;//contor va retine numarul de componente conexe
    for(int i=1;i<=nr_noduri2;i++)//parcugem toate nodurile si apelam dfs pentru cele nevizitate
        if(viz[i]==0)
        {
            contor++;//atunci cand gasim un nod nevizitat marim contorul
            Graf_Neorientat::dfs(i,viz);
        }
    return contor;

}

string Graf_Neorientat::tip_graf()
{
    return "\nGraf Neorientat";
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

class Graf_Neorientat_Cost:public Graf
{
private:
    int numar_muchii;
    unordered_map<int,vector<pair<int,int>>> lista_adiacenta;

public:
    //constructori
    Graf_Neorientat_Cost();
    Graf_Neorientat_Cost(int nr_noduri,int numar_muchii,unordered_map<int,vector<pair<int,int>>> lista_adiacenta);
    //operatori de copiere
    Graf_Neorientat_Cost(const Graf_Neorientat_Cost& g);
    Graf_Neorientat_Cost& operator=(const Graf_Neorientat_Cost& g);
    //functiile de citire
    friend ifstream& operator>>(ifstream& in,Graf_Neorientat_Cost& g);
    string tip_graf();
    virtual ifstream& citire_graf_virtuala_fisier(ifstream& in);
    //getteri
    unordered_map<int,vector<pair<int,int>>> get_lista_adiacenta();
    int get_numar_muchii();
    static void disjoint(string fisier_intrare,string fisier_iesire);
    //functii
    vector<pair<int,int>> begin_APM(int &suma_arborelui);
    //destructor
    ~Graf_Neorientat_Cost(){}
};
//aceasta este o clasa ajutatoare in care putem pastra o muchie si costul ei. Aceasta clasa este necesara deoarece trebuie sa suprascriem operatorul< pentru a putea folosi
//un heap ce contine elemente din aceasta clasa
class muchie_cost
{
private:
    int cost,nod_1,nod_2;
public:
    muchie_cost(int cost1,int nod_1,int nod_2)
    {
        this->cost=cost1;
        this->nod_1=nod_1;
        this->nod_2=nod_2;
    }
    friend bool operator<(const muchie_cost& m1,const muchie_cost& m2)
    {
        return m1.cost <= m2.cost;
    }
    int get_cost() const {return cost;}
    int get_nod_1() const {return nod_1;}
    int get_nod_2() const {return nod_2;}
};

vector<pair<int,int>> Graf_Neorientat_Cost::begin_APM(int &suma_arborelui)
{
    //aceasta clasa va returna prin parametru suma costurilor din arborele partial de cost minim. De asemenea vom returna un vector de perechi ce reprezinta muchiile
    //ce alcatuiesc arborele partial de cost minim
    suma_arborelui=0;//initilizam suma cu 0
    int numar_noduri = Graf::get_nr_noduri();//retine numarul de noduri pentru a nu apela getterul de prea multe ori
    set<muchie_cost,less<muchie_cost>> heap;//in acest min heap vom retine muchiile in ordine crescatoare in functie de cost
    for(int i=0;i<lista_adiacenta[1].size();i++)//initiizam primele valori din heap cu muchiile legate de nodul 1
    {
        muchie_cost m_curenta(lista_adiacenta[1][i].second,1,lista_adiacenta[1][i].first);
        heap.insert(m_curenta);
    }
    vector<int> viz(numar_noduri+1,0);//vectorul ce retine daca un nod a fost vizitat sau nu
    viz[1]=1;//1 a fost deja evaluat deci il notam ca fiind vizitat
    vector<pair<int,int>> solutie;//acesta este vectorul ce va retine muchiile din APM
    while(!heap.empty())//cat timp heap-ul nu este gol
    {
        set<muchie_cost,less<muchie_cost>>::iterator itr;
        itr=heap.begin();//luam primul element din heap
        if(viz[(*itr).get_nod_2()]==0)//verificam daca nodul al doilea este vizitat
        {
            //daca nodul nu este vizitat inseamna ca aceasta este o muchie de cost minim valida asa ca o adaugam la solutie
            viz[(*itr).get_nod_2()]=1;//urmatorul nod devine vizitat
            suma_arborelui=suma_arborelui+(*itr).get_cost();//incrementam suma arborelui
            pair<int,int> pereche;
            pereche.first=(*itr).get_nod_1();
            pereche.second=(*itr).get_nod_2();
            solutie.push_back(pereche);//adaugam muchia la solutie
            int nod_urmator=(*itr).get_nod_2();
            for(int i=0;i<lista_adiacenta[nod_urmator].size();i++)//adaugam toti veciinii urmatorului nod in heap
                if(viz[lista_adiacenta[nod_urmator][i].first]==0)
                {
                    muchie_cost m_curenta(lista_adiacenta[nod_urmator][i].second,nod_urmator,lista_adiacenta[nod_urmator][i].first);
                    heap.insert(m_curenta);

                }
            heap.erase(itr);//eliminam primul element din heap si reluam algoritmul
        }
        else
            heap.erase(itr);//daca nodul a fost vizitat inseamna ca muchia nu este valida deci doar eliminam primul element din heap si reluam algoritmul
    }

    return solutie;
}

string Graf_Neorientat_Cost::tip_graf()
{
    return "Graf neorientat cu costuri";
}

unordered_map<int,vector<pair<int,int>>> Graf_Neorientat_Cost::get_lista_adiacenta()
{
    return this->lista_adiacenta;
}

void Graf_Neorientat_Cost::disjoint(string fisier_intrare,string fisier_iesire)
{
    ifstream fin(fisier_intrare);
    ofstream fout(fisier_iesire);

    int n,m;//n este numarul de multimi de la inceput unde fiecare multime i contine un singur element i,m este numarul de comenzi
    fin>>n>>m;//citim n si m din fisier
    struct nod//structura nod va retine valoarea nodului curent, tatal nodului curent(0 in cazul in care nu are tata) si inaltimea subarborelui sau
    {
        int valoare;
        int tata;
        int inaltime;
    };
    vector<nod> noduri(n+1);//vectorul in care vom retine fiecare nod
    for(int i=1;i<=n;i++)
    {
        noduri[i].valoare=i;//initializam valorile
        noduri[i].tata=0;//la inceput toate nodurile sunt radacini deoarece sunt singure in multime deci toate nodurile au tataul 0
        noduri[i].inaltime=1;//inaltimea fiecaruia este 1 deoarece este doar radacina
    }
    for(int i=0;i<m;i++)
    {
        int comanda,nod_1,nod_2;//citim comanda si cele doua noduri
        fin>>comanda>>nod_1>>nod_2;
        if(comanda==1)//daca comanda e 1 inseamna ca trebuie sa face reuniune de multimi
        {
            int nod_3=nod_1,nod_4=nod_2;//nod_3 si nod_4 au rolul de trece din nou prin arbore in sus dupa prima parcurgere pentru a lega toate nodurile de radacina arborelui
            while(noduri[nod_1].tata!=0)//parcurgem arborele primului nod
                nod_1=noduri[nod_1].tata;
            while(noduri[nod_2].tata!=0)//parcurgem arborele celui de-al doilea nod
                nod_2=noduri[nod_2].tata;
            while(noduri[nod_3].tata!=0)//legam toate nodurile de radacina lor respectiva
            {
                int nod_copy=nod_3;
                nod_3=noduri[nod_3].tata;
                noduri[nod_copy].tata=nod_1;
            }
            while(noduri[nod_4].tata!=0)
            {
                int nod_copy=nod_4;
                nod_4=noduri[nod_4].tata;
                noduri[nod_copy].tata=nod_2;
            }
            if(noduri[nod_1].inaltime<noduri[nod_2].inaltime)//daca inaltimea primului arbore este mai mica, atunci legam primul arbore de ce de-al doilea
            {
                noduri[nod_1].tata=nod_2;
            }
            else if(noduri[nod_1].inaltime==noduri[nod_2].inaltime)//daca inaltimiile sunt egale inseamna ca putem sa legam oricare din acesti arbori la celalalt cu conditia sa incrementam inaltimea arborelui parine
            {
                noduri[nod_1].tata=nod_2;
                noduri[nod_2].inaltime++;
            }
            else if(noduri[nod_1].inaltime>noduri[nod_2].inaltime)//daca inaltimea celui de-al doilea arbore este mai mica atunci legam al doilea arbore de primul
            {
                noduri[nod_2].tata=nod_1;
            }

        }
        else//daca comanda e 2 inseamna ca trebuie doar sa parcurgem ambii arbori si sa vedem daca au aceeasi radacina
        {
            int nod_3=nod_1,nod_4=nod_2;//asemenator cu comanda 1 parcurgem prima data pentru a afla radacina si apoi mai parcurgem o data pentru a lega toate nodurile de radacina
            while(noduri[nod_1].tata!=0)
                nod_1=noduri[nod_1].tata;
            while(noduri[nod_2].tata!=0)
                nod_2=noduri[nod_2].tata;
            if(nod_1==nod_2)//daca radaciniile sunt egale inseamna ca ambele noduri fac parte din acelasi arbore
                fout<<"DA"<<"\n";
            else//altfel => ca cele doua noduri fac parte din arbori diferiti
                fout<<"NU"<<"\n";
            while(noduri[nod_3].tata!=0)
            {
                int nod_copy=nod_3;
                nod_3=noduri[nod_3].tata;
                noduri[nod_copy].tata=nod_1;
            }
            while(noduri[nod_4].tata!=0)
            {
                int nod_copy=nod_4;
                nod_4=noduri[nod_4].tata;
                noduri[nod_copy].tata=nod_2;
            }
        }
    }


}

int Graf_Neorientat_Cost::get_numar_muchii()
{
    return this->numar_muchii;
}

Graf_Neorientat_Cost::Graf_Neorientat_Cost():Graf()
{
    this->numar_muchii=0;
}

Graf_Neorientat_Cost::Graf_Neorientat_Cost(int nr_noduri,int numar_muchii,unordered_map<int,vector<pair<int,int>>> lista_adiacenta):Graf(nr_noduri)
{
    this->numar_muchii=numar_muchii;
    this->lista_adiacenta=lista_adiacenta;
}

Graf_Neorientat_Cost::Graf_Neorientat_Cost(const Graf_Neorientat_Cost& g):Graf(g)
{
    this->numar_muchii=g.numar_muchii;
    this->lista_adiacenta=g.lista_adiacenta;
}

Graf_Neorientat_Cost& Graf_Neorientat_Cost::operator=(const Graf_Neorientat_Cost& g)
{
    if(this!=&g)
    {
        Graf::operator=(g);
        this->numar_muchii=g.numar_muchii;
        this->lista_adiacenta=g.lista_adiacenta;
    }
    *this;
}

ifstream& Graf_Neorientat_Cost::citire_graf_virtuala_fisier(ifstream& in)
{
    Graf::citire_graf_virtuala_fisier(in);
    in>>this->numar_muchii;
    for(int i=1;i<=this->numar_muchii;i++)
    {
        int fst,snd,cost;
        in>>fst>>snd>>cost;
        pair<int,int> pereche;
        pereche.first = snd;
        pereche.second = cost;
        lista_adiacenta[fst].push_back(pereche);
        pereche.first = fst;
        pereche.second = cost;
        lista_adiacenta[snd].push_back(pereche);

    }
    return in;
}

ifstream& operator>>(ifstream& in,Graf_Neorientat_Cost& g)
{
    return g.citire_graf_virtuala_fisier(in);
}

class Graf_Orientat:public Graf
{
private:
    int numar_muchii;
    unordered_map<int,vector<int>>lista_adiacenta;

    //functii ajutatoare pentru a afla componentele tare conexe
    void dfs_stiva(int nod,vector<int> &viz,stack<int> &noduri);
    void dfs_tare_conex(int nod,vector<int> &viz,set<int,less<int>> &componenta_tare_conexa);
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
    //functii
    string tip_graf();
    vector<int> bfs(int nod_start);
    vector<set<int,less<int>>> begin_componente_tare_conexe();
    vector<int> begin_sortaret();
    //setteri si getteri
    int get_nr_muchii();
    unordered_map<int,vector<int>> get_lista_adiacenta();


    //citire si afisare virtuala
    virtual ifstream& citire_graf_virtuala_fisier(ifstream& in);


};

string Graf_Orientat::tip_graf()
{
    return "\nGraf Orientat";
}

vector<int> Graf_Orientat::begin_sortaret()
{
    int numar_noduri = Graf_Orientat::get_nr_noduri();//retinem numarul de noduri pentru a nu folosi getterul de prea multe ori
    vector<int> grad_interior(numar_noduri+1,0);//vector ce retine pentru fiecare nod gradul interior
    for(int i=1;i<=numar_noduri;i++)//parcurgem toate nodurile si initializam gradul interior
    {
        if(lista_adiacenta.find(i)!=lista_adiacenta.end())
            for(int j=0;j<lista_adiacenta[i].size();j++)
                grad_interior[lista_adiacenta[i][j]]++;
    }
    queue<int> coada_noduri;//coada in care vom retine nodurile pe masura ce parcurgem graful
    vector<int> viz(numar_noduri+1,0);//vector ce retine pentru fiecare nod daca a fost vizitat sau nu
    for(int i=1;i<=numar_noduri;i++)//adaugam in coada toate nodurile care au gradul 0 si le marcam ca fiind vizitate
        if(grad_interior[i]==0)
        {
            coada_noduri.push(i);
            viz[i] = 1;
        }
    vector<int> solutie;//vectorul in care vom pastra solutia
    while(!coada_noduri.empty())//cat timp coada nu este goala
    {
        solutie.push_back(coada_noduri.front());//adaugam primul nod din coada in solutie
        if(lista_adiacenta.find(coada_noduri.front())!=lista_adiacenta.end())
            for(int i=0;i<lista_adiacenta[coada_noduri.front()].size();i++)//trecem prin toti vecinii nodului curent si scadem gradul interior cu 1
            {
                grad_interior[lista_adiacenta[coada_noduri.front()][i]]--;
                if(grad_interior[lista_adiacenta[coada_noduri.front()][i]]==0)//daca gradul interior devine 0 il adaugam in coada
                {
                    viz[lista_adiacenta[coada_noduri.front()][i]]=1;
                    coada_noduri.push(lista_adiacenta[coada_noduri.front()][i]);
                }

            }
        coada_noduri.pop();//eliminam nodul de la inceput si reluam algoritmul
    }
    return solutie;
}

void Graf_Orientat::dfs_stiva(int nod,vector<int> &viz,stack<int> &noduri)
{
    viz[nod] = 1;//notam nodul curent ca fiind vizitat
    if(lista_adiacenta.find(nod)!=lista_adiacenta.end())
        for(int i=0;i<lista_adiacenta[nod].size();i++)//parcurgem toti vecinii nodului
        {
            if(viz[lista_adiacenta[nod][i]]==0)//cand gasim un vecin nevizitat apelam dfs_stiva pe acel vecin
                dfs_stiva(lista_adiacenta[nod][i],viz,noduri);
        }
    noduri.push(nod);//la final introducem in stiva nodul pentru a avea ordinea inversa parcurgerii in stiva
}

void Graf_Orientat::dfs_tare_conex(int nod,vector<int> &viz,set<int,less<int>> &componenta_tare_conexa)
{
    viz[nod]=1;//notam nodul curent ca fiind vizitat
    componenta_tare_conexa.insert(nod);//inseram nodul in setul ce retine componenta tare conexa
    if(lista_adiacenta.find(nod)!=lista_adiacenta.end())
        for(int i=0;i<lista_adiacenta[nod].size();i++)//parcurgem toti vecinii
        {
            if(viz[lista_adiacenta[nod][i]]==0)//daca gasim un vecin nevizitat apelam dfs_tare_conex din nou
                dfs_tare_conex(lista_adiacenta[nod][i],viz,componenta_tare_conexa);
        }
    //la final functia va returna prin parametrul componenta_tare_conexa nodurile ce alcatuiesc aceasta componenta tare conexa
}

vector<set<int,less<int>>> Graf_Orientat::begin_componente_tare_conexe()
{
    int numar_noduri = Graf_Orientat::get_nr_noduri();//retinem numarul de noduri pentru a nu apela functia get de prea multe ori
    vector<int> viz(numar_noduri+1,0);//vectorul ce retine daca un nod a fost vizitat sau nu
    vector<set<int,less<int>>> componente_conexe;//vectorul de componente conexe ce va fi returnat la final
    for(int k=1; k<=numar_noduri; k++)//parcurgem toate nodurile
    {
        if(viz[k]==0)//daca gasim un nod nevizitat
        {
            stack<int> noduri;//stiva in care vom retine parcurgerea inversa a nodurilor in arborele dfs
            dfs_stiva(k,viz,noduri);//functia ce ne returneaza prin parametru parcurgerea inversa
            int numar_muchii2 = Graf_Orientat::get_nr_muchii();//retinem numarul de muchii pentru a nu apela functia get de prea multe ori
            unordered_map<int,vector<int>> lista_ad;//lista_ad reprezinta listele de adiacenta pentru fiecare nod in graful transpus
            for(int i=1; i<=numar_noduri; i++)//initializam lista_ad
            {
                if(viz[i]==1)
                    if(lista_adiacenta.find(i)!=lista_adiacenta.end())
                        for(int j=0; j<lista_adiacenta[i].size(); j++)
                            lista_ad[lista_adiacenta[i][j]].push_back(i);
            }
            Graf_Orientat g2(numar_noduri,numar_muchii2,lista_ad);//cream graful transpus
            vector<int> viz2(numar_noduri+1,0);//vector ce retine daca un nod este sau nu vizitat
            while(!noduri.empty())//cat timp stiva nu e goala
            {
                set<int,less<int>> componenta_tare_conexa;//in acest set vom retine componenta tare conexa curenta
                if(viz2[noduri.top()]==0)//daca nodul din varf nu este vizitat
                {
                    g2.dfs_tare_conex(noduri.top(),viz2,componenta_tare_conexa);//apelam dfs_tare_conex pe graful transpus incepand cu nodul din varf
                    if(componenta_tare_conexa.size()!=0)//daca componenta nu e goala, inseamna ca avem o solutie valida
                    {
                        componente_conexe.push_back(componenta_tare_conexa);//adaugam componenta tare conexa in solutie
                        componenta_tare_conexa.clear();//eliberam memoria din componenta_tare_conexa pentru a o refolosi
                    }
                }
                noduri.pop();//eliminam un nod din varf si reluam while-ul

            }

        }
    }
    return componente_conexe;
}

vector<int> Graf_Orientat::bfs(int nod_start)
{
    int nr_noduri2 = Graf_Orientat::get_nr_noduri();//retinem numarul de noduri pentru a nu apela de fiecare data functia get
    queue<int> bfs_queue;//coada in care vom adauga nodurile pe masura ce parcurgem graful
    bfs_queue.push(nod_start);//adaugam in coada primul nod
    vector<int> viz(nr_noduri2+1,0);//vectorul ce retine daca un nod a fost sau nu vizitat
    viz[nod_start] = 1;//nod de start devine vizitat
    vector<int> distanta(nr_noduri2+1,-1);//vectorul in care vom retine distanta de la nod_start la fiecare nod (este indexat de la 1)
    distanta[nod_start]=0;//initalizam distanta de la nod_start la el insusi
    while(!bfs_queue.empty())//cat timp coada nu e goala
    {
        int front_queue = bfs_queue.front();//retinem primul element din coada pentru a nu apela front de fiecare data
        if(lista_adiacenta.find(front_queue)!=lista_adiacenta.end())
        {
            for(int i=0;i<lista_adiacenta[front_queue].size();i++)//parcurgem vecinii primului nod si verificam daca sunt vizitati
            {
                if(viz[lista_adiacenta[front_queue][i]]==0)//daca gasim un vecin nevizitat , il punem in coada, actualizam distanta lui cu distanta parintelui+1 si il notam ca vizitat
                {
                    bfs_queue.push(lista_adiacenta[front_queue][i]);
                    distanta[lista_adiacenta[front_queue][i]]=distanta[front_queue]+1;
                    viz[lista_adiacenta[front_queue][i]]=1;
                    //cout<<"\n"<<front_queue<<" "<<lista_adiacenta[front_queue][i-1]<<" "<<contor;

                }
            }
        }
        bfs_queue.pop();//la final eliminam un nod din coada si se reia while-ul pana nu mai avem noduri
    }
    return distanta;

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

class Graf_Orientat_Cost:public Graf
{
private:
    int numar_muchii;
    unordered_map<int,vector<pair<int,int>>> lista_adiacenta;

    //functii ajutatoare pentru a crea un ciclu hamiltonian de lungime minima
    static int valoare_ciclu_minim_hamiltonian(int nod_inceput,int noduri_atasate_in_biti,int nod_final,vector<vector<int>> lista_ad_inversa,vector<vector<int>> costuri,vector<vector<int>> &matrice);
public:
    //constructori
    Graf_Orientat_Cost();
    Graf_Orientat_Cost(int nr_noduri,int numar_muchii,unordered_map<int,vector<pair<int,int>>> lista_adiacenta);
    //operatori de copiere
    Graf_Orientat_Cost(const Graf_Orientat_Cost& g);
    Graf_Orientat_Cost& operator=(const Graf_Orientat_Cost& g);
    //functiile de citire
    friend ifstream& operator>>(ifstream& in,Graf_Orientat_Cost& g);
    string tip_graf();
    virtual ifstream& citire_graf_virtuala_fisier(ifstream& in);
    void citire_graf_roy_floyd(string nume_fisier);
    static void Rezolva_ROY_FLOYD(string fisier_out,string fisier_in);
    static void Rezolva_Hamilton(string fisier_in,string fisier_out);
    //getteri
    unordered_map<int,vector<pair<int,int>>> get_lista_adiacenta();
    int get_numar_muchii();
    //functii
    vector<long long> Dijkstra(int n);
    vector<long long> Bellman_Ford();

    //destructor
    ~Graf_Orientat_Cost(){}
};

int Graf_Orientat_Cost::valoare_ciclu_minim_hamiltonian(int nod_inceput,int noduri_atasate_in_biti,int nod_final,vector<vector<int>> lista_ad_inversa,vector<vector<int>> costuri,vector<vector<int>> &matrice)
{
    if(matrice[noduri_atasate_in_biti][nod_final]==-1)
    {
        matrice[noduri_atasate_in_biti][nod_final]=100000000;
        for(int i=0;i<lista_ad_inversa[nod_final].size();i++)
        {
            //daca nodul urmator se gaseste in nodurile atasate in lant atunci continuam calculul,in cazul in care nodul nu s-ar gasi in lant inseamna ca a fost deja vizitat si nu luam muchia in calcul
            if(noduri_atasate_in_biti & 1<<lista_ad_inversa[nod_final][i])
            {
                //daca nodul urmator care are o muchie spre nodul final este nodul de inceput atunci verificam daca configuratia nodurilor atunci cand eliminam nodul final este egala cu configurata nodurilor cand avem
                //un singur nod in lant si acela este nodul de inceput . Daca nu este egala inseamna ca mai sunt noduri ce trebuie adaugate la calcul in configuratie, asa ca nu parcurgem muchia respectiva
                if(lista_ad_inversa[nod_final][i]==nod_inceput &&(noduri_atasate_in_biti^1<<nod_final)!=(1<<nod_inceput)) continue;
                //calculam valoarea minima a ciclului recursiv prin adunarea costului muchiei curente si valoarea minima lantului urmatorului nod
                matrice[noduri_atasate_in_biti][nod_final] = min(matrice[noduri_atasate_in_biti][nod_final],
                                                                 //valoare minima a urmatorului nod
                                                                 //de asemenea din configuratie eliminam nodul curent final
                                                                 valoare_ciclu_minim_hamiltonian(nod_inceput,(noduri_atasate_in_biti^1<<nod_final),lista_ad_inversa[nod_final][i],lista_ad_inversa,costuri,matrice)
                                                                 //costul muchie curente pe care o parcurgem
                                                                 +costuri[lista_ad_inversa[nod_final][i]][nod_final]);
            }
        }
    }
    return matrice[noduri_atasate_in_biti][nod_final];
}

void Graf_Orientat_Cost::Rezolva_Hamilton(string fisier_in,string fiser_out)
{
    ifstream fin(fisier_in);
    ofstream fout(fiser_out);
    vector<vector<int>> liste_ad_inversa(20);
    vector<vector<int>> costuri(20,vector<int>(20));
    vector<vector<int>> matrice(262150,vector<int>(20,-1));
    int n,m;
    fin>>n>>m;
    for(int i=0;i<m;i++)
    {
        int x,y,cost;
        fin>>x>>y>>cost;
        liste_ad_inversa[y].push_back(x);
        costuri[x][y]=cost;
    }
    int solutie=100000000;
    matrice[1][0]=0;
    for(int i=0;i<liste_ad_inversa[0].size();i++)
        solutie=min(solutie,valoare_ciclu_minim_hamiltonian(0,(1<<n)-1,liste_ad_inversa[0][i],liste_ad_inversa,costuri,matrice)+costuri[liste_ad_inversa[0][i]][0]);
    if(solutie==100000000)
        fout<<"Nu exista solutie";
    else
        fout<<solutie;


}
void Graf_Orientat_Cost::Rezolva_ROY_FLOYD(string fisier_in,string fisier_out)
{
    ifstream fin(fisier_in);
    ofstream fout(fisier_out);
    int INF = 1061109567;
    int n;
    fin>>n;
    int matrice[101][101];
    int dis_minima[101][101];
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
            fin>>matrice[i][j];
            if(i!=j && matrice[i][j]==0)
                dis_minima[i][j]=INF;
            else
                dis_minima[i][j]=matrice[i][j];

        }
    for(int i=0;i<n;i++)//nodurile intermediare 0..i-1
        for(int j=0;j<n;j++)
            for(int k=0;k<n;k++)
    {
        if(dis_minima[j][k]>(dis_minima[j][i]+dis_minima[i][k]) &&(dis_minima[j][i]!=INF&&dis_minima[i][k]!=INF))
            dis_minima[j][k]=dis_minima[j][i]+dis_minima[i][k];

    }
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(dis_minima[i][j]==INF)
                fout<<0<<" ";
            else
                fout<<dis_minima[i][j]<<" ";
        }
        fout<<"\n";
    }
}

vector<long long> Graf_Orientat_Cost::Bellman_Ford()
{
    int numar_noduri=Graf::get_nr_noduri();//retinem numarul de noduri pentru a nu apela getterul de prea multe ori
    vector<muchie_cost> muchii;//in acest vector vom retine muchiile grafului cu costurile lor
    for(int i=1;i<=numar_noduri;i++)//initializam vectorul muchii
        if(lista_adiacenta.find(i)!=lista_adiacenta.end())
            for(int j=0;j<lista_adiacenta[i].size();j++)
            {
                muchie_cost muchie_curenta(lista_adiacenta[i][j].second,i,lista_adiacenta[i][j].first);
                muchii.push_back(muchie_curenta);
            }
    long long infinit = 1061109567;//variabila infint cu care sunt intializate drumurile minime ale tuturor nodurilor
    vector<long long> drumuri_minime(numar_noduri+1,infinit);//vectorul drumuri minime va retine drumul minim de la 1 la fiecare nod
    drumuri_minime[1]=0;//drumul minim pana la 1 este 0
    bool diff=true;//variabila semafor ce se asigura ca s-a intamplat o modifcare a drumurilor minime
    int contor = 0;//contorul ce verifica daca nu am trecut de numarul maxim de pasi
    while(diff)
    {
        contor++;
        if(contor>=numar_noduri)//daca contorul este mai mare sau egal cu n inseamna ca avem un ciclu negativ si ne oprim
            break;
        diff=false;
        for(int i=0;i<muchii.size();i++)//trecem prin toate muchiile si verificam daca distanta nodului in care ajungem este mai mare decat distanta nodului din care veinim +costul muchiei
            if(drumuri_minime[muchii[i].get_nod_2()]>drumuri_minime[muchii[i].get_nod_1()]+muchii[i].get_cost())//in cazul in care se respecta conditia,modifica drumul minim si diff=true pentru a continua while-ul
            {
                drumuri_minime[muchii[i].get_nod_2()]=drumuri_minime[muchii[i].get_nod_1()]+muchii[i].get_cost();
                diff=true;
            }
    }
    if(contor>=numar_noduri)//daca contorul esta >= decat numarul de noduri inseamna ca avem un ciclu negativ
        drumuri_minime[0]=0;//vom folosi prima valoare din vector pentru a vedea daca exista sau nu un ciclu negativ
    else
        drumuri_minime[0]=1;
    return drumuri_minime;

}

vector<long long> Graf_Orientat_Cost::Dijkstra(int n)
{
    int numar_noduri = Graf::get_nr_noduri();//retinem numarul de noduri pentru a nu apela getterul de prea multe ori
    long long infinit=1061109567;//retinem un cost maxim ce nu poate fii atins
    vector<long long> noduri_cost(numar_noduri+1,infinit);//vectori ce retine pentru fiecare nod distanta de la n la el, il initializam pe tot cu infinit
    vector<int> viz(numar_noduri+1,0);//vectorul ce retine daca un nod a fost selectat deja sau nu
    noduri_cost[n]=0;//drumul de la n la n este 0
    viz[n]=1;//nodul n este deja vizitat
    set<pair<int,int>> heap;//in acest heap vom retine perechi de forma (cost,nod) pentru a accesa in O(1) nodul nevizitat cu drumul cel mai scurt
    if(lista_adiacenta.find(1)!=lista_adiacenta.end())//parcurgem toti vecinii lui n
        for(int i=0; i<lista_adiacenta[n].size(); i++)
        {
            noduri_cost[lista_adiacenta[n][i].first]=lista_adiacenta[n][i].second;//actualizam drumurile fiecarui vecin
            pair<int,int> cost_nod;//cream pentru fiecare vecin o pereche de forma (cost,nod) si o introducem in heap
            cost_nod.first=lista_adiacenta[n][i].second;
            cost_nod.second=lista_adiacenta[n][i].first;
            heap.insert(cost_nod);
        }
    while(!heap.empty())//cat timp heap-ul are elemente
    {
        set<pair<int,int>>::iterator itr;
        itr=heap.begin();//luam primul element din heap(acestea fiind ordonate dupa cel mai mic cost)
        int nod_curent=(*itr).second;
        if(viz[nod_curent]==0)//daca nodul curent nu a fost vizitat il selectam
        {
            viz[nod_curent]=1;//nodul devine vizitat
            if(lista_adiacenta.find(nod_curent)!=lista_adiacenta.end())
                for(int i=0; i<lista_adiacenta[nod_curent].size(); i++)//parcugem toti vecinii nevizitat ai nodului curent si verificam daca e nevoie de o modifcare a drumului minim
                {
                    if(viz[lista_adiacenta[nod_curent][i].first]==0)
                    {
                        if(noduri_cost[lista_adiacenta[nod_curent][i].first]>noduri_cost[nod_curent]+lista_adiacenta[nod_curent][i].second)//in cazul in care drumul minim trebuie modificat introducem o noua pereche in heap
                        {
                            noduri_cost[lista_adiacenta[nod_curent][i].first]=noduri_cost[nod_curent]+lista_adiacenta[nod_curent][i].second;
                            pair<int,int> cost_nod;
                            cost_nod.first=noduri_cost[lista_adiacenta[nod_curent][i].first];
                            cost_nod.second=lista_adiacenta[nod_curent][i].first;
                            heap.insert(cost_nod);
                        }
                    }
                }
        }
        else//daca nodul a fost vizitat inseamna ca pereche curenta face parte din modificarile facute anterior si este irelevanta deci o eliminam din heap
            heap.erase(itr);

    }
    for(int i=1;i<=numar_noduri;i++)//la final actualizam drumurile nodurilor la care nu s-a putut ajunge
        if(noduri_cost[i]==infinit)
            noduri_cost[i]=0;
    return noduri_cost;//returnam vectorul de distante minime

}

string Graf_Orientat_Cost::tip_graf()
{
    return "Graf orientat cu costuri";
}

unordered_map<int,vector<pair<int,int>>> Graf_Orientat_Cost::get_lista_adiacenta()
{
    return this->lista_adiacenta;
}

int Graf_Orientat_Cost::get_numar_muchii()
{
    return this->numar_muchii;
}

Graf_Orientat_Cost::Graf_Orientat_Cost():Graf()
{
    this->numar_muchii=0;
}

Graf_Orientat_Cost::Graf_Orientat_Cost(int nr_noduri,int numar_muchii,unordered_map<int,vector<pair<int,int>>> lista_adiacenta):Graf(nr_noduri)
{
    this->numar_muchii=numar_muchii;
    this->lista_adiacenta=lista_adiacenta;
}

Graf_Orientat_Cost::Graf_Orientat_Cost(const Graf_Orientat_Cost& g):Graf(g)
{
    this->numar_muchii=g.numar_muchii;
    this->lista_adiacenta=g.lista_adiacenta;
}

Graf_Orientat_Cost& Graf_Orientat_Cost::operator=(const Graf_Orientat_Cost& g)
{
    if(this!=&g)
    {
        Graf::operator=(g);
        this->numar_muchii=g.numar_muchii;
        this->lista_adiacenta=g.lista_adiacenta;
    }
    *this;
}

void Graf_Orientat_Cost::citire_graf_roy_floyd(string nume_fisier)
{
    ifstream fin(nume_fisier);
    int n;
    fin>>n;
    int contor=0;
    unordered_map<int,vector<pair<int,int>>> lista_auxiliara;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
    {
        int x;
        fin>>x;
        if(x!=0)
        {
            contor++;
            pair<int,int> pereche;
            pereche.first=j;
            pereche.second=x;
            lista_auxiliara[i].push_back(pereche);
        }

    }
    this->lista_adiacenta=lista_auxiliara;
    this->numar_muchii=contor;
    Graf::set_nr_noduri(n);


}

ifstream& Graf_Orientat_Cost::citire_graf_virtuala_fisier(ifstream& in)
{
    Graf::citire_graf_virtuala_fisier(in);
    in>>this->numar_muchii;
    for(int i=1;i<=this->numar_muchii;i++)
    {
        int fst,snd,cost;
        in>>fst>>snd>>cost;
        pair<int,int> pereche;
        pereche.first = snd;
        pereche.second = cost;
        lista_adiacenta[fst].push_back(pereche);

    }
    return in;
}

ifstream& operator>>(ifstream& in,Graf_Orientat_Cost& g)
{
    return g.citire_graf_virtuala_fisier(in);
}


class Graf_Orientat_Flux:public Graf
{
private:
    int numar_muchii;
    int** capacitate;
    vector<int> muchii[1200];

    bool lant_nesat_BF(int tata[],int** flux);
    void revizuieste_flux_lant(int tata[],int** flux);
public:
    Graf_Orientat_Flux();
    friend ifstream& operator>>(ifstream& in,Graf_Orientat_Flux& g);
    virtual ifstream& citire_graf_virtuala_fisier(ifstream& in);
    int Max_flow();
    virtual string tip_graf();
    ~Graf_Orientat_Flux();
};

Graf_Orientat_Flux::Graf_Orientat_Flux():Graf()
{
    this->numar_muchii=0;
    capacitate = new int*[1001];
    for(int i=0;i<1000;i++)
    {
        capacitate[i]=new int[1001];
        for(int j=0; j<1000; j++)
        {
            this->capacitate[i][j]=0;
        }
    }
}
ifstream& Graf_Orientat_Flux::citire_graf_virtuala_fisier(ifstream& in)
{
    Graf::citire_graf_virtuala_fisier(in);
    in>>this->numar_muchii;
    for(int i=0;i<this->numar_muchii;i++)
    {
        int first,second,cap;
        in>>first>>second>>cap;
        capacitate[first][second]=cap;
        muchii[first].push_back(second);
        muchii[second].push_back(first);
    }
    return in;
}
ifstream& operator>>(ifstream& in,Graf_Orientat_Flux& g)
{
    return g.citire_graf_virtuala_fisier(in);
}
bool Graf_Orientat_Flux::lant_nesat_BF(int tata[],int **flux)
{
    for(int i=1;i<=Graf::get_nr_noduri();i++)
        tata[i]=0;
    int viz[1001]={0};
    queue<int> coada_noduri;
    viz[1]=1;
    coada_noduri.push(1);
    while(!coada_noduri.empty())
    {
        int nod = coada_noduri.front();
        coada_noduri.pop();
        for(int i=0;i<muchii[nod].size();i++)
        {
            int nod2 = muchii[nod][i];
            if(viz[nod2]==0)
            {
                if(capacitate[nod][nod2]>flux[nod][nod2])
                {
                    coada_noduri.push(nod2);
                    viz[nod2]=1;
                    tata[nod2]=nod;
                    if(nod2==Graf::get_nr_noduri())
                        return true;
                }
                if(capacitate[nod][nod2]==0 && flux[nod2][nod]>0)
                {
                    coada_noduri.push(nod2);
                    viz[nod2]=1;
                    tata[nod2]=-nod;
                    if(nod2==Graf::get_nr_noduri())
                        return true;
                }
            }
        }
    }
    return false;
}
void Graf_Orientat_Flux::revizuieste_flux_lant(int tata[],int** flux)
{
    int minim=110001;
    int n=Graf::get_nr_noduri();
    while(tata[n]!=0)
    {
        if(tata[n]>0)
        {
            minim=min(minim,capacitate[tata[n]][n]-flux[tata[n]][n]);
            n=tata[n];
        }
        else
        {
            minim=min(minim,flux[n][abs(tata[n])]);
            n=abs(tata[n]);
        }

    }
    n=Graf::get_nr_noduri();
    while(tata[n]!=0)
    {
        if(tata[n]>0)
        {
            flux[tata[n]][n]+=minim;
            n=tata[n];
        }
        else
        {
            flux[n][abs(tata[n])]-=minim;
            n=abs(tata[n]);
        }

    }
}
int Graf_Orientat_Flux::Max_flow()
{
    int tata[1001];
    int** flux =new int*[1001];
    for(int i=0;i<1000;i++)
    {
        flux[i]=new int[1001];
        for(int j=0;j<1000;j++)
            flux[i][j]=0;
    }
    while(lant_nesat_BF(tata,flux))
        revizuieste_flux_lant(tata,flux);
    int suma=0;
    for(int i=0;i<muchii[1].size();i++)
    {
        int nod=muchii[1][i];
        if(capacitate[1][nod]>0)
            suma+=flux[1][nod];
    }
    for(int i=0;i<1000;i++)
    {
        delete[] flux[i];
    }
    delete[] flux;
    return suma;

}
string Graf_Orientat_Flux::tip_graf()
{

    return "Graf Orientat cu Flux";
}

Graf_Orientat_Flux::~Graf_Orientat_Flux()
{
    if(this->capacitate!=NULL)
    {
        for(int i=0; i<1000; i++)
        {
            delete[] capacitate[i];
        }
    }
    delete[] capacitate;
}

class Multigraf:public Graf
{
private:
    vector<int> liste_ad[85000];
    vector<int> liste_nod_muchii[85000];

    //functie ajutatoare DFS pentru a afla ciclul eulerian
    void DFS_Euler(int nod,vector<int> &viz,vector<int> &solutie);
public:
    //constructor
    Multigraf(){};
    //functii de citire
    friend ifstream& operator>>(ifstream& in,Multigraf& g);
    virtual ifstream& citire_graf_virtuala_fisier(ifstream& in);
    //functii si metode
    virtual string tip_graf();
    vector<int> Ciclu_Euler();
    ~Multigraf(){};
};

ifstream& Multigraf::citire_graf_virtuala_fisier(ifstream& in)
{
    Graf::citire_graf_virtuala_fisier(in);
    int muchii,nr_noduri2=Graf::get_nr_noduri();
    in>>muchii;
    for(int i=1;i<=muchii;i++)
    {
        int fst,snd;
        in>>fst>>snd;
        this->liste_ad[fst].push_back(snd);
        this->liste_ad[snd].push_back(fst);
        this->liste_nod_muchii[fst].push_back(i);
        this->liste_nod_muchii[snd].push_back(i);
    }
    return in;
}
ifstream& operator>>(ifstream& in,Multigraf& g)
{
    return g.citire_graf_virtuala_fisier(in);
}
string Multigraf::tip_graf()
{
    return "MultiGraf";
}

void Multigraf::DFS_Euler(int nod,vector<int> &viz,vector<int> &solutie)
{
    while(!liste_ad[nod].empty())
    {
        int final_vector=liste_ad[nod].size()-1;
        int nod_urmator=liste_ad[nod][final_vector];
        int muchie_urmatoare=liste_nod_muchii[nod][final_vector];

        liste_ad[nod].pop_back();
        liste_nod_muchii[nod].pop_back();
        if(viz[muchie_urmatoare]==0)
        {
            viz[muchie_urmatoare]=1;
            DFS_Euler(nod_urmator,viz,solutie);
        }
    }
    solutie.push_back(nod);
}
vector<int> Multigraf::Ciclu_Euler()
{
    int nr_noduri2= Graf::get_nr_noduri();
    vector<int> muchii_vizitate(500005,0);
    vector<int> solutie;
    for(int i=1;i<nr_noduri2;i++)
        if(liste_ad[i].size()%2!=0)
    {
        solutie.push_back(-1);
        return solutie;
    }
    DFS_Euler(1,muchii_vizitate,solutie);
    return solutie;

}

class Graf_Bipartit
{
private:
    int nr_noduri_1,nr_noduri_2;
    vector<vector<int>> matrice;

    //functii ajutatoare pentru Cuplajul Maxim
    vector<int> Drum_augmentat(vector<int> &left_match,vector<int> &right_match,vector<vector<int>> mat);
    vector<int> DFS_Cuplaj(int nod,int check,vector<int> &viz_dreapta,vector<int> &viz_stanga,vector<int> &right_match,vector<vector<int>> &mat);
public:
    //constructori
    Graf_Bipartit();

    //functii de citire
    friend ifstream& operator>>(ifstream& in,Graf_Bipartit& g);

    //functii/metode
    vector<vector<int>> Cuplaj_Maxim(int &contor);

    //setteri si getteri
    int get_nr_noduri_2();
    int get_nr_noduri_1();
};

int Graf_Bipartit::get_nr_noduri_1()
{
    return this->nr_noduri_1;
}

int Graf_Bipartit::get_nr_noduri_2()
{
    return this->nr_noduri_2;
}
Graf_Bipartit::Graf_Bipartit()
{
    this->nr_noduri_1=0;
    this->nr_noduri_2=0;
}
ifstream& operator>>(ifstream& in,Graf_Bipartit& g)
{
    in>>g.nr_noduri_1>>g.nr_noduri_2;
    vector<vector<int>> mat_aux(g.nr_noduri_1+1,vector<int>(g.nr_noduri_2+1,0));
    g.matrice=mat_aux;
    int nr_muchii;
    in>>nr_muchii;
    for(int i=0;i<nr_muchii;i++)
    {
        int fst,snd;
        in>>fst>>snd;
        g.matrice[fst][snd]=1;
    }
    return in;
}
vector<int> Graf_Bipartit::DFS_Cuplaj(int nod,int check,vector<int> &viz_dreapta,vector<int> &viz_stanga,vector<int> &right_match,vector<vector<int>> &mat)
{
    vector<int> solutie;
    if(check==0)
    {
        viz_stanga[nod]=1;
        int ok=1;
        for(int i=1;i<=this->nr_noduri_2;i++)
            if(this->matrice[nod][i]==1 && mat[nod][i]==0 && viz_dreapta[i]==0)
        {
            cout<<"\nVecinul lui "<<nod<<" este "<<i<<"\n";
            solutie=DFS_Cuplaj(i,1,viz_dreapta,viz_stanga,right_match,mat);
            if(solutie.size()!=0)
            {
                solutie.push_back(nod);
                return solutie;
            }
        }
        vector<int> vid;
        return vid;
    }
    else
    {
        viz_dreapta[nod]=1;
        int ok=1;
        if(right_match[nod]==0)
        {
            solutie.push_back(nod);
            return solutie;
        }
        else
        {
            for(int i=1;i<=this->nr_noduri_1;i++)
                if(this->matrice[i][nod]==1 && mat[i][nod]==1 && viz_stanga[i]==0)
            {
                cout<<"\nVecinul lui "<<nod<<" este "<<i<<"\n";
                solutie=DFS_Cuplaj(i,0,viz_dreapta,viz_stanga,right_match,mat);
                if(solutie.size()!=0)
                {
                    solutie.push_back(nod);
                    return solutie;
                }
            }
            vector<int> vid;
            return vid;
        }
    }
}
vector<vector<int>> Graf_Bipartit::Cuplaj_Maxim(int &contor)
{
    vector<vector<int>> solutie(this->nr_noduri_1+1,vector<int>(this->nr_noduri_2+1,0));
    vector<int> Right_match(this->nr_noduri_2+1,0);
    vector<int> viz(this->nr_noduri_1+1,0);
    while(true)
    {
        vector<int> viz_right(this->nr_noduri_2+1,0);
        vector<int> viz_left(this->nr_noduri_1+1,0);
        int nod_start=-1;
        for(int i=1;i<=this->nr_noduri_1;i++)
            if( viz[i]==0)
        {
            viz[i]=1;
            nod_start=i;
            break;
        }
        if(nod_start==-1)
            break;
        vector<int> drum=DFS_Cuplaj(nod_start,0,viz_right,viz_left,Right_match,solutie);

        if(drum.size()!=0)
        {
            cout<<"Drumul este ";
            for(int i=drum.size()-1;i>=0;i--)
                cout<<drum[i]<<" ";
            cout<<"\n\n\n\n";
            Right_match[drum[0]]=1;
            int check=0;
            for(int i=drum.size()-1; i>0; i--)
            {
                int nod_1=drum[i],nod_2=drum[i-1];
                if(check==0)
                {
                    solutie[nod_1][nod_2]=1;
                    contor++;
                    check=1;
                    //cout<<"am scos muchia "<<nod_1<<" "<<nod_2<<"\n";
                }
                else
                {
                    solutie[nod_2][nod_1]=0;
                    //cout<<"am adaugat muchia "<<nod_1<<" "<<nod_2<<"\n";
                    contor--;
                    check=0;
                }
            }
        }

    }
    return solutie;


}


int main()
{
    //exercitiul DFS
        /*ifstream fin("dfs.in");
        ofstream fout("dfs.out");
        Graf_Neorientat g;
        fin>>g;
        fout<<g.begin_dfs();*/

    //exercitiul BFS
        /*ifstream fin("bfs.in");
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
        vector<int> solutie;
        solutie=g.bfs(nod_start);
        for(int i=1;i<=nr_noduri;i++)
            fout<<solutie[i]<<" ";*/

    //exerctiul Biconexe
        /*ifstream fin("biconex.in");
        ofstream fout("biconex.out");
        Graf_Neorientat g;
        fin>>g;
        vector<set<int,less<int>>> solutie;
        solutie=g.begin_biconex();
        fout<<solutie.size()<<"\n";
        for(int i=0;i<solutie.size();i++)
        {
            for(auto itr=solutie[i].begin();itr!=solutie[i].end();itr++)
                fout<<*itr<<" ";
            fout<<"\n";
        }*/

    //exercitiul CTC
    /*ifstream fin("ctc.in");
    ofstream fout("ctc.out");
    Graf_Orientat g;
    fin>>g;
    vector<set<int,less<int>>> solutie;
    solutie = g.begin_componente_tare_conexe();
    fout<<solutie.size()<<"\n";
    for(int i=0; i<solutie.size(); i++)
    {
        for(auto itr=solutie[i].begin(); itr!=solutie[i].end(); itr++)
            fout<<*itr<<" ";
        fout<<"\n";
    }*/

    //exercitiul Sortaret
        /*ifstream fin("sortaret.in");
        ofstream fout("sortaret.out");
        Graf_Orientat g;
        fin>>g;
        vector<int> solutie;
        solutie=g.begin_sortaret();
        for(int i=0;i<solutie.size();i++)
            fout<<solutie[i]<<" ";*/

    //exercitiul havel hakimi
        /*int n;
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
        if(Graf_Neorientat::havel_hakimi(grade))
            cout<<"da";
        else
            cout<<"nu";*/

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

    //exercitiul APM
       /* ifstream fin("apm.in");
        ofstream fout("apm.out");
        Graf_Neorientat_Cost g;
        fin>>g;
        vector<pair<int,int>> solutie;
        int suma;
        solutie = g.begin_APM(suma);
        fout<<suma<<"\n";
        fout<<solutie.size()<<"\n";
        for(int i=0;i<solutie.size();i++)
            fout<<solutie[i].first<<" "<<solutie[i].second<<"\n";*/

    //exercitiul disjoint
        /*Graf_Neorientat_Cost::disjoint("disjoint.in","disjoint.out");*/

    //exercitiul Dijkstra
        /*ifstream fin("dijkstra.in");
        ofstream fout("dijkstra.out");
        Graf_Orientat_Cost g;
        fin>>g;
        vector<long long> solutie;
        solutie = g.Dijkstra(1);
        for(int i=2;i<solutie.size();i++)
            fout<<solutie[i]<<" ";*/

    //exercitiul Bellman_Ford
        /*ifstream fin("bellmanford.in");



        ofstream fout("bellmanford.out");
        Graf_Orientat_Cost g;
        fin>>g;
        vector<long long> solutie;
        solutie=g.Bellman_Ford();
        if(solutie[0])
            for(int i=2;i<solutie.size();i++)
                fout<<solutie[i]<<" ";
        else
            fout<<"Ciclu negativ!";
    */
    //exercitiul MaxFlow
    /*ifstream fin("maxflow.in");
    ofstream fout("maxflow.out");
    Graf_Orientat_Flux g;
    fin>>g;
    fout<<g.Max_flow();*/

    //exercitiul Roy Floyd
    //Graf_Orientat_Cost::Rezolva_ROY_FLOYD("royfloyd.in","royfloyd.out");

    //exercitiul DARB
    /*Graf_Neorientat::REZOLVA_DARB("darb.in","darb.out");*/

    //exercitiul Ciclu Eulerian
    /*ifstream fin("ciclueuler.in");
    ofstream fout("ciclueuler.out");
    Multigraf g;
    fin>>g;
    vector<int> solutie;
    solutie=g.Ciclu_Euler();
    if(solutie[0]==-1)
        fout<<-1;
    else
        for(int i=0;i<solutie.size()-1;i++)
            fout<<solutie[i]<<" ";*/

    //exercitiul Ciclu Hamiltonian minim
    /*Graf_Orientat_Cost::Rezolva_Hamilton("hamilton.in","hamilton.out");*/

    //exercitiul Cuplaj Maxim
    Graf_Bipartit g;
    ifstream fin("cuplaj.in");
    ofstream fout("cuplaj.out");
    fin>>g;
    int contor=0;
    vector<vector<int>> solutie;
    solutie=g.Cuplaj_Maxim(contor);
    fout<<contor<<"\n";
    for(int i=1;i<=g.get_nr_noduri_1();i++)
        for(int j=1;j<=g.get_nr_noduri_2();j++)
            if(solutie[i][j]==1)
                fout<<i<<" "<<j<<"\n";

    /*fout.close();
    fin.close();*/


    return 0;
}
