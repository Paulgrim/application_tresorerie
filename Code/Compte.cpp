#include "Compte.h"

/**
 * \brief Document qui comporte les fonctions des classes Compte_Concret, Abstrait, Virtuel, Singleton
 */



//================
//COMPTE ABSTRAIT
//================

unsigned int Compte_Abstrait::numero=4;//initialisation des numéros des comptes

/**
 * \brief Compte_Abstrait::Compte_Abstrait
 * \param n : nom du compte à creer
 * \param t : type de compte à creer
 */
Compte_Abstrait::Compte_Abstrait(const string& n, Type t):nom(n),type(t){
    id=numero;
    numero++;
}

/**
 * \brief Destructeur de la classe compte abstrait
 */
Compte_Abstrait::~Compte_Abstrait(){}

//==================
//COMPTE VIRTUEL
//==================

/**
 * \brief Fonction qui calcule solde des comptes contenu dans un compte virtuel
 * \return solde
 */
float Compte_Virtuel::getSolde() const
{
        //on fait la somme du solde de chaque compte
        float somme=0;

        vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
        const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();

        for(;itb!=ite;itb++){
            if((*itb)->getId()!=3)
                somme+=(*itb)->getSolde();
        }
        return somme;
}


/**
 * \brief Fonction qui calcule le solde au moment du dernier rapprochement
 * \return solde du dernier rapprochement
 */
float Compte_Virtuel::getSoldeDernierRapprochement() const
{
        //on fait la somme du solde de chaque compte
        float somme=0;
        vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
        const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
        for(;itb!=ite;itb++)
            somme+=(*itb)->getSoldeDernierRapprochement();
        return somme;
}

/**
 * \brief Fonction qui retourne le solde au moment du bilan
 * \return solde du bilan
 */
float Compte_Virtuel::getSoldeBilan() const
{
        //on fait la somme du solde de chaque compte
        float somme=0;
        vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
        const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
        for(;itb!=ite;itb++)
            somme+=(*itb)->getSoldeBilan();
        return somme;
}

/**
 * \brief Destructeur de Compte Virtuel
 */
Compte_Virtuel::~Compte_Virtuel()
{
        //on detruit chaque element
        vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
        const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();

        for(;itb!=ite;itb++)
        {
            delete (*itb);
        };
}

/**
 * \brief Fonction qui recupre adresse du compte recherche via son nom
 * \param  nom : nom du compte à récupérer
 * \return adresse d'un compte abstrait si trouvé et nullptr sinon
 */
Compte_Abstrait* Compte_Virtuel::getCompte(const string& nom)
{
    if ((this->getNom().compare(nom))==0){
        return (this);
    }
    vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
    const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();

    for(;itb!=ite;itb++)
    {
        if((*itb)->getCompte(nom)!=nullptr){
            return (*itb)->getCompte(nom);
        };
    };
    return nullptr;
}

/**
 * \brief Fonction qui recupre adresse du compte recherche via son id
 * \param id : id du compte a recuperer
 * \return adresse d'un compte abstrait si trouvé sinon nullptr
 */
Compte_Abstrait* Compte_Virtuel::getCompte(int id)
{
    if (this->getId()==id){
        return (this);
    }
    vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
    const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
    Compte_Abstrait* temp;
    for(;itb!=ite;itb++)
    {
        temp = (*itb)->getCompte(id);
        if(temp!=nullptr){
            return temp;
        };
    };
    return nullptr;
}


/**
 * \brief Compte_Virtuel::getArborescence
 * \param h : profondeur
 * \return arborescence
 */
string Compte_Virtuel::getArborescence(int h) const{
    string arbo = "";
    vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
    const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
    for(int i = 0; i < h; i++){
        arbo += "   ";
    }
    arbo += "|- " + afficherTypeDiscret() +" ";
    arbo += to_string(getId()) + " ";
    arbo+= getNom() + " ";

    float nombre = getSolde();
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << nombre;
    std::string s = stream.str();
    arbo+= s +"\n";
    h++;
    for(;itb!=ite;itb++)
    {
        arbo += (*itb)->getArborescence(h);
    };
    h--;
    return arbo;
}

/**
 * \brief retourne l'arborescence du bilan
 * \param h : prodonfeur
 * \return l'arborescence du bilan a un niveau h
 */
string Compte_Virtuel::getArborescenceBilan(int h) const{
    string arbo = "";
    if(type==Type::Actif||type==Type::Passif||type==Type::Racine){
        vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
        const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
        for(int i = 0; i < h; i++){
            arbo += "   ";
        }
        arbo += "|- " + getNom() + " ";
        float nombre = getSoldeBilan();
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << nombre;
        std::string s = stream.str();
        arbo+= s +"\n";
        h++;
        for(;itb!=ite;itb++)
        {
            arbo += (*itb)->getArborescenceBilan(h);
        };
        h--;
   }
    arbo+="\n";
   return arbo;
}

/**
 * \brief Fonction permettant de mettre les solde bilans de chaque comptes concret d'un compte virtuel à 0
 */
void Compte_Virtuel::setSoldeBilan(){
    vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
    const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
    for(;itb!=ite;itb++)
    {
        if (type == Type::Actif || type == Type::Passif || type == Type::Racine)
            (*itb)->setSoldeBilan();
    }
}

/**
 * \brief Fonction permettant le calcul et l'obtention du bilan d'un compte
 * \param d : date du bilan
 * \return bilan : bilan du compte à la date donnée
 */
float Compte_Virtuel::getBilan(Date d){
    setSoldeBilan();
    float bilan =0;
    TransactionManager& t=TransactionManager::getManager();
    vector<Transaction*>::const_iterator itb= t.getIteratorTransaction();
    const vector<Transaction*>::const_iterator ite=t.getEndIteratorTransaction();
    for (;itb!=ite;itb++) {
            if ((*itb)->getDate()<=d){
                vector<Triplet*>::const_iterator itb1=(*itb)->getTriplet();
                const vector<Triplet*>::const_iterator ite1=(*itb)->getEndTriplet();
                for(;itb1!=ite1;itb1++){
                    if ((*itb1)->getCredit() != 0 && (*itb1)->getCompte()->getType() == Type::Actif){

                        (*itb1)->getCompte()->modifSoldeBilan(-((*itb1)->getCredit()));
                        bilan -= (*itb1)->getCredit();
                    }
                    else if ((*itb1)->getDebit() != 0 && (*itb1)->getCompte()->getType() == Type::Passif){
                            (*itb1)->getCompte()->modifSoldeBilan(-((*itb1)->getDebit()));
                             bilan -= (*itb1)->getDebit();
                        }
                        else if ((*itb1)->getDebit() != 0 && (*itb1)->getCompte()->getType() == Type::Actif){
                            (*itb1)->getCompte()->modifSoldeBilan(((*itb1)->getDebit()));
                             bilan += (*itb1)->getDebit();
                            }
                            else if ((*itb1)->getCredit() != 0 && (*itb1)->getCompte()->getType() == Type::Passif){
                                (*itb1)->getCompte()->modifSoldeBilan(((*itb1)->getCredit()));
                                 bilan += (*itb1)->getCredit();
                            }
                    }
            }
    }
    return bilan;
}

/**
  * \brief Fonction pour ajouter un compte virtuel dans un compte virtuel
  * \param compte : adresse d'un compte virtuel
  * \return
  */
Compte_Virtuel& Compte_Virtuel::Add(Compte_Virtuel* compte){
    liste_compte.push_back(compte);
    return (*compte);
}

/**
  * \brief Fonction pour ajouter un compte concret dans un compte virtuel
  * \param compte : adresse d'un compte concret
  * \return
  */
void Compte_Virtuel::Add(Compte_Concret* compte){
        liste_compte.push_back(compte);
}


/**
 * \brief Fonction pour supprimer un compte dans la liste d'un compte virtuel
 * \param idElement : id du compte a supprimer
 * \return true si compte supprimé, false si compte pas trouvé, donc non supprimé.
 */
bool Compte_Virtuel::supprimerElement(int idElement){
    Singleton& root = Singleton::donneInstance();
    Compte_Abstrait* compte=dynamic_cast<Compte_Abstrait*>(root.getCompte(idElement));
    vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
    const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
    for(;itb!=ite;itb++)
    {
        if((*itb)->getId()==compte->getId()){
            liste_compte.erase(itb);
            return true;
        };
    };
    return false;
}

/**
 * \brief Renvoie l'id du pere
 * \param idCompte : l'id du compte dont on veut connaitre le pere
 * \return id_pere int
 */
int Compte_Virtuel::getIdPere(int idCompte){
    int somme=0;
    if(this->isVirtuel()){
           auto itb=getFirstCompte();
           const auto ite=getLastCompte();
           for(;itb!=ite;itb++){
               if((*itb)->getId()==idCompte)
                   somme+=getId();
           }
           itb=getFirstCompte();
           for(;itb!=ite;itb++){
               if((*itb)->isVirtuel())
                   somme+=(dynamic_cast<Compte_Virtuel*>(*itb)->getIdPere(idCompte));
           }
    }
    return somme;
}


/**
  * \brief Sauvegarder le compte dans un fichier
 */
void Compte_Virtuel::save(ofstream& fout,int idPere)const{
    vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
    const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
    fout<<"virtuel;"<<getNom()<<";"<<afficherType()<<";"<<idPere<<";"<<getId()<<"\n";
    for(;itb!=ite;itb++){
        (*itb)->save(fout,getId());
    };
}

//=========================
//COMPTE CONCRET
//=======================

/**
  * \brief Fonction pour afficher proprement l'arborescence h permet de gerer proprement les espaces
  * \param h : profondeur
  * \return arborescence à la profondeur h
  */
string Compte_Concret::getArborescence(int h) const{
    string arbo = "";
    for(int i = 0; i < h; i++){
        arbo += "   ";
    }
    arbo += "|- " + afficherTypeDiscret() +" ";
    arbo += to_string(getId()) +" ";
    arbo+= getNom() + " ";
    float nombre = getSolde();
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << nombre;
    std::string s = stream.str();
    arbo+= s +"\n";
    return arbo;
}

/**
  * \brief Fonction pour afficher proprement l'arborescence h par rapport à un bilan
  * \param h : profondeur
  * \return arborescence à la profondeur h
  */
string Compte_Concret::getArborescenceBilan(int h) const{
    string arbo = "";
    if(type==Type::Actif ||type==Type::Passif){
        for(int i = 0; i < h; i++){
            arbo += "   ";
        }
        arbo += "|- " + getNom();
        float nombre = getSoldeBilan();
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << nombre;
        std::string s = stream.str();
        arbo+= " " + s;
    }
    arbo+="\n";
    return arbo;
}

/**
  * \brief Sauvegarder le compte dans un fichier
  * \param fout : flux ofstream
  * \param idPere : id du compte pere
  */
void Compte_Concret::save(ofstream& fout,int idPere)const{
    fout<<"Concret;"<<getNom()<<";"<<afficherType()<<";"<<idPere<<";"<<getId()<<"\n";
}

//==========================
//SINGLETON
//===========================

/**
 * \brief Singleton::compteRacine
 */
Singleton* Singleton::compteRacine=nullptr; //pas de compte racine au lancement de l'application


/**
 * \brief Donner l'instance de l'unique compte racine
 * \return instance&
 */
Singleton& Singleton::donneInstance() {
    if (compteRacine==nullptr) compteRacine=new Singleton("Compte Racine");
    return *compteRacine;
}

/**
 * \brief Libere l'unique compte racine
 */
void Singleton::libereInstance(){
    if (compteRacine!=nullptr)
        delete compteRacine;
    compteRacine=nullptr;
}

/**
 * \brief Fonction pour deplacer un compte
 * \param idCompte : compte que l'on veut deplacer
 * \param idAncien : compte qui contient le compte a deplacer
 * \param idPere : compte ou l'on doit ranger le nouveau compte
 * \return bool : false si le compte n'a pas pu etre supprimé, true si tout est bon
 */
bool Singleton::deplacer(int idCompte, int idAncien, int idPere){
    Singleton& root = Singleton::donneInstance();
    Compte_Virtuel* base=dynamic_cast<Compte_Virtuel*>(root.getCompte(idAncien));
    Compte_Virtuel* pere=dynamic_cast<Compte_Virtuel*>(root.getCompte(idPere));
    if(pere!=nullptr){ //Le nouveau pere est un compte virtuel ?
        if(Compte_Virtuel* virtuel=dynamic_cast<Compte_Virtuel*>(root.getCompte(idCompte))){ //Le compte à déplacer est un compte virtuel ?
            if(pere->getType()!=virtuel->getType() && pere->getType()!=Racine)
                throw TresorerieException("Type du pere different du type du compte.\n");
            else{
                pere->Add(virtuel);
                if(base->supprimerElement(virtuel->getId()))
                    return true;
                else
                    return false;
            }
        }
        else{ //Le compte à déplacer est un compte concret
            Compte_Concret* concret=dynamic_cast<Compte_Concret*>(root.getCompte(idCompte));
            if(pere->getType()!=concret->getType() && pere->getType()!=Racine)
                throw TresorerieException("Type du pere different du type du compte.\n");
            else{
                pere->Add(concret);
                if(base->supprimerElement(concret->getId()))
                    return true;
                else
                    return false;
            }
        }
    }
    else
        throw TresorerieException("Ajout impossible ici (pere).\n");
}

/**
 * \brief Destructeur singleton
 */
Singleton::~Singleton(){
    Singleton& root=Singleton::donneInstance();
    ofstream fout((root.getFilename()).toStdString());
    root.save(fout, getId());
    fout.close();
}

/**
 * \brief Fonction qui calcule solde des comptes contenu dans un compte virtuel
 * \return solde
 */
float Singleton::getSolde() const
{
        //on fait la somme du solde de chaque compte
        float somme=0;
        vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
        const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
        for(;itb!=ite;itb++){
            if((*itb)->getId()!=3){
                if((*itb)->getType()==Type::Actif || (*itb)->getType()==Type::Recette)
                    somme+=(*itb)->getSolde();
                else
                    somme-=(*itb)->getSolde();
            }
        }
        return somme;
}

/**
 * \brief Fonction pour supprimer un compte dans la liste d'un compte virtuel
 * \param id_pere : l'id du pere du compte à supprimer
 * \param id_Compte : l'id du compte à supprimer
 * \return true si compte supprimé, false si compte pas trouvé, donc non supprimé.
 */
bool Singleton::suppression(int id_pere, int id_Compte)
{
    Compte_Virtuel* pere = dynamic_cast<Compte_Virtuel*>(getCompte(id_pere));
    if(!((*pere).isVirtuel()))
    {
        throw TresorerieException("Le pere doit etre virtuel");
    }
    Compte_Virtuel* base = dynamic_cast<Compte_Virtuel*>(getCompte(id_Compte));
    if(base != nullptr)
    {
        if(base->isEmpty())
        {
            bool suppression = pere->supprimerElement(id_Compte);
            if(suppression)
            {
                delete base;
            }
            return suppression;
        } else {
            throw TresorerieException("Le compte virtuel n'est pas vide, vider d'abord le compte puis recommencer\n");
        }
    }
    else
    {
        Compte_Concret* c = dynamic_cast<Compte_Concret*>(getCompte(id_Compte));
        if(c == nullptr)
        {
            throw TresorerieException("Le compte n'est pas concret\n");
        }
        TransactionManager& t = TransactionManager::getManager();
        vector<Transaction*>::const_iterator itb = t.getIteratorTransaction();
        const vector<Transaction*>::const_iterator ite = t.getEndIteratorTransaction();
        for(;itb!=ite;itb++){
                if((*itb)->isInside(*c))
                {
                    throw TresorerieException("Le compte que vous voulez supprimer est lié à une transaction\n");
                }
        }
        bool suppression = pere->supprimerElement(id_Compte);
        if(suppression)
        {
            delete c;
        }
        return suppression;
    }
}

/**
 * \brief Singleton::resultat
 * \return resultat sur l'ensemble des comptes
 */
float Singleton::resultat(){
    Singleton& root = Singleton::donneInstance();
    int i;
    float depense=0,recette=0;
    for(i=4;i<=root.getNumero();i++)//on a 4 id réservés
    {
        Compte_Abstrait* compte = root.getCompte(i);

        Compte_Concret* compte1=dynamic_cast<Compte_Concret*>(compte);
        if (compte1!=nullptr){
            if (compte1->getType()==Type::Recette){
                recette+=compte1->getSolde();
            }else  if (compte1->getType()==Type::Depense){
                depense+=compte1->getSolde();
            }
        }
    }
    return recette-depense;

}

/**
 * \brief Sauvegarder compte dans un fichier
 * \param fout : flux ofstream
 * \param idPere : id du compte pere
 */
void Singleton::save(ofstream& fout,int idPere)const{
    vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
    const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
    for(;itb!=ite;itb++){
        (*itb)->save(fout, idPere);
    };
}

/**
 * \brief Charger des comptes à partir d'un fichier
 * \param f : nom du fichier
 */
void Singleton::load(const QString& f) {
    CompteFactory compteManager;
    if (filename != f){
        ofstream fout(getFilename().toStdString());
        save(fout,getId());
        fout.close();
    }
    filename = f;
    ifstream fin(filename.toStdString()); // ouverture du fichier
    if (!fin) throw TresorerieException("erreur, probleme d'ouverture du fichier.\n");
    fin.get();
    if (!fin.eof()){
        fin.seekg(ios_base::beg); //On revient au début du fihcier si il n'était pas vide
        while (!fin.eof() && fin.good()) {
            string virtuel;
            getline(fin,virtuel,';');
            string idPere;
            string id;
            string nom;
            string type;
            Type t;
            getline(fin, nom, ';'); // lecture du nom jusqu'au prochain ';'
            if (fin.bad()) throw TresorerieException("erreur de lecture dans le fichier de comptes.\n");
            getline(fin, type, ';');
            if(type.compare("Actif")==0)
                t=Type::Actif;
            else if(type.compare("Passif")==0)
                        t=Type::Passif;
                else if(type.compare("Depense")==0)
                        t=Type::Depense;
                     else t=Type::Recette;
            getline(fin, idPere, ';'); // lecture de l'id du pere jusqu'au prochain ';'
            if(fin.bad()) throw TresorerieException("erreur de lecture dans le fichier de comptes.\n");
            fin >> id; //lecture de l'id du compte
            if(virtuel.compare("virtuel")==0){
                if (fin.bad()) throw TresorerieException("erreur de lecture dans le fichier de comptes.\n");
                compteManager.FabricationLoad(t, nom, stoi(idPere),stoi(id));
            }
            else{
                compteManager.FabricationLoad(t, nom, stoi(idPere),stoi(id),0);
            }
                    while (fin.peek() == '\r') fin.ignore();
                    while (fin.peek() == '\n') fin.ignore();
        }
    }
    fin.close(); // close file
}

/**
 * \brief Fonction qui calcule la somme des soldes des comptes d'Actifs au moment du bilan
 * \return somme de leur solde
 */
float Singleton::getSommeActif() const
{
        //on fait la somme du solde de chaque compte
        float somme=0;
        vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
        const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
        for(;itb!=ite;itb++){
            if((*itb)->getId()!=3){
                if((*itb)->getType()==Type::Actif)
                    somme+=(*itb)->getSoldeBilan();
            }
        }
        return somme;
}

/**
 * \brief Fonction qui calcule la somme des soldes des comptes de Passifs au moment du bilan
 * \return somme de leur solde
 */
float Singleton::getSommePassif() const
{
        //on fait la somme du solde de chaque compte
        float somme=0;
        vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
        const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
        for(;itb!=ite;itb++){
            if((*itb)->getId()!=3){
                if((*itb)->getType()==Type::Passif)
                    somme+=(*itb)->getSoldeBilan();
            }
        }
        return somme;
}

/**
 * \brief Fonction qui retourne la somme des soldes au moment du bilan
 * \return solde du bilan
 */
float Singleton::getSoldeBilan() const
{
        //on fait la somme du solde de chaque compte
        float somme=0;
        vector<Compte_Abstrait*>::const_iterator itb=liste_compte.begin();
        const vector<Compte_Abstrait*>::const_iterator ite=liste_compte.end();
        for(;itb!=ite;itb++){
            if((*itb)->getId()!=3){
                if((*itb)->getType()==Type::Actif)
                    somme+=(*itb)->getSoldeBilan();
                if((*itb)->getType()==Type::Passif)
                    somme-=(*itb)->getSoldeBilan();
            }
        }
        return somme;
}

//=====================
//FACTORY
//====================


/**
 * \brief Fabrication d'un compte de type virtuel et placement dans un compte virtuel
 * \param type : type du compte a fabriquer
 * \param nom : nom du nouveau compte
 * \param idPere pere : compte ou l'on doit ranger le nouveau compte
 */
void CompteFactory::Fabrication(Type type, string nom, int idPere){
    Singleton& root = Singleton::donneInstance();
    Compte_Virtuel* base=dynamic_cast<Compte_Virtuel*>(root.getCompte(idPere));
    if(type==Type::Racine)
        throw TresorerieException("Type invalide.\n");
    else{
        if((base->getNom()).compare("Compte Racine")==0)
            root.Add(new Compte_Virtuel(nom,type));
        else{
            if((base->getType()!=type) && (base->getType()!=Type::Racine))
                throw TresorerieException("Type du pere different du type precise.\n");
            else
                base->Add(new Compte_Virtuel(nom,type));
        }
    }
}


/**
 * \brief Fabrication d'un compte de type concret et placement dans un compte virtuel
 * \param  nom : nom du nouveau compte
 * \param pere : compte ou l'on doit ranger le nouveau compte
 * \param idPere : id du père du compte à creer
 * \param solde : solde du nouveau compte
 */
void CompteFactory::Fabrication(Type type, string nom, int idPere, float solde){
    Singleton& root = Singleton::donneInstance();
    if(type==Type::Racine)
        throw TresorerieException("Type invalide.\n");
    else{
        if(root.getId()==idPere){
             root.Add(new Compte_Concret(nom,type,0));
        }
        else{
            if(Compte_Virtuel* base=dynamic_cast<Compte_Virtuel*>(root.getCompte(idPere))){
                if((base->getType()!=type) && (base->getType()!=Type::Racine))
                    throw TresorerieException("Type du pere different du type precise.\n");
                else{
                    base->Add(new Compte_Concret(nom,type,0));
                }
            }
            else throw TresorerieException("Le pere est un compte terminal.\n");
        }
    }
    Compte_Concret* compte=dynamic_cast<Compte_Concret*>(root.getCompte(root.getNumero()-1));
    if(type==Type::Actif){
        Compte_Concret* capitaux=dynamic_cast<Compte_Concret*>(root.getCompte(3));//id de compte réservé au compte de capitaux propres)
        if (capitaux==nullptr){
            capitaux=new Compte_Concret("Capitaux Propres",3,Type::Passif,0);
            root.Add(capitaux); //on créée le compte resultat sous la racine
        }
        Triplet* capit=new Triplet(capitaux,0,solde);
        Triplet* compt =new Triplet(compte,solde,0);
        vector<Triplet*> liste;
        liste.push_back(capit);
        liste.push_back(compt);
        TransactionManager& t=TransactionManager::getManager();
        Date d=Date(QDate::currentDate().day(),QDate::currentDate().month(),QDate::currentDate().year());
        string refe="A"+std::to_string(compte->getId())+"1";
        t.creerTransaction(d,refe,"Solde initial",liste);
    }
    if(type==Type::Passif){
        Compte_Concret* capitaux=dynamic_cast<Compte_Concret*>(root.getCompte(3));//id de compte réservé au compte de résultat(les comptes créés commentcent à partir de id =3)
        if (capitaux==nullptr){
            capitaux=new Compte_Concret("Capitaux Propres",3,Type::Passif,0);
            root.Add(capitaux); //on créée le compte resultat sous la racine
        }
        Triplet* capit=new Triplet(capitaux,solde,0);
        Triplet* compt =new Triplet(compte,0,solde);
        vector<Triplet*> liste;
        liste.push_back(capit);
        liste.push_back(compt);
        TransactionManager& t=TransactionManager::getManager();
        Date d=Date(QDate::currentDate().day(),QDate::currentDate().month(),QDate::currentDate().year());
        string refe="P"+std::to_string(compte->getId())+"1";
        t.creerTransaction(d,refe,"Solde initial",liste);
    }
}

/**
 * \brief Fabrication d'un compte de type virtuel dans le load
 * \param type : type du compte a fabriquer
 * \param nom : nom du nouveau compte
 * \param idPere : id du compte ou l'on doit ranger le nouveau compte
 * \param id : id du compte à creer
 */
void CompteFactory::FabricationLoad(Type type, string nom, int idPere, int id){
    Singleton& root = Singleton::donneInstance();
    Compte_Virtuel* base=dynamic_cast<Compte_Virtuel*>(root.getCompte(idPere));
    if(type==Type::Racine)
        throw TresorerieException("Type invalide.\n");
    else{
        if((base->getNom()).compare("Compte Racine")==0)
            root.Add(new Compte_Virtuel(nom,id,type));
        else{
            if((base->getType()!=type) && (base->getType()!=Type::Racine))
                throw TresorerieException("Type du pere different du type precise.\n");
            else
                base->Add(new Compte_Virtuel(nom,id,type));
        }
    }
}

/**
 * \brief Fabrication d'un compte de type concret dans le load
 * \param type : type du compte a fabriquer
 * \param idPere : id du compte ou l'on doit ranger le nouveau compte
 * \param nom : nom du nouveau compte
 * \param id : id du compte à creer
 * \param solde : solde du nouveau compte
 */
void CompteFactory::FabricationLoad(Type type, string nom, int idPere, int id,float solde){
    Singleton& root = Singleton::donneInstance();
    if(type==Type::Racine)
        throw TresorerieException("Type invalide.\n");
    else{
        if(root.getId()==idPere)
            root.Add(new Compte_Concret(nom,id,type,solde));
        else{
            if(Compte_Virtuel* base=dynamic_cast<Compte_Virtuel*>(root.getCompte(idPere))){
                if((base->getType()!=type) && (base->getType()!=Type::Racine))
                    throw TresorerieException("Type du pere different du type precise.\n");
                else
                    base->Add(new Compte_Concret(nom,id,type,solde));
            }
            else throw TresorerieException("Le pere est un compte terminal.\n");
        }
    }
}
