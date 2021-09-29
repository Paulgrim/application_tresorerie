#include "Compte.h"

/**
 * \brief Document qui comporte les fonctions des classes Date, Triplet, Transaction et TransactionManager
 */

//==================
//DATE
//==================

/**
 * \brief Date::Date
 * \param j : jour
 * \param m : mois
 * \param a : annee
 */
Date::Date(int j, int m, int a){
    if(a>=0 && a<=3000) annee=a; else throw TresorerieException("erreur: annee invalide");
        if (m>=1 && m<=12) mois=m; else throw TresorerieException("erreur: mois invalide");
        switch(m){
            case 1: case 3: case 5: case 7: case 8: case 10: case 12: if (j>=1 && j<=31) jour=j; else throw TresorerieException("erreur: jour invalide"); break;
            case 4: case 6: case 9: case 11: if (j>=1 && j<=30) jour=j; else throw TresorerieException("erreur: jour invalide"); break;
            case 2: if (j>=1 && (j<=29 || (j==30 && a%4==0))) jour=j; else throw TresorerieException("erreur: jour invalide"); break;
        }
}

//==================
//Surcharge des opérateurs de date
//==================


/**
 * \brief Surcharge de l'operateur == pour comparer 2 dates
 * \param date : date à comparer
 * \return booleen vrai si les 2 dates sont égales, false sinon
 */
bool Date::operator==(const Date& date) const{
    if(((*this).getAnnee()==date.getAnnee()) && ((*this).getMois()==date.getMois()) && ((*this).getJour()==date.getJour()))
        return 1;
    else
        return 0;
}

/**
 * \brief Surcharge de l'operateur != pour comparer 2 dates
 * \param date : date à comparer
 * \return booleen vrai si les 2 dates sont differentes, false sinon
 */
bool Date::operator!=(const Date& date) const{
    if(((*this).getAnnee()!=date.getAnnee()) || ((*this).getMois()!=date.getMois()) || ((*this).getJour()!=date.getJour()))
        return 1;
    else
        return 0;
}

/**
 * \brief Surcharge de l'operateur < pour comparer 2 dates
 * \param date : date à comparer
 * \return booleen vrai si la date à gauche de l'opérateur est inférieur à la date à droite, false sinon
 */
bool Date::operator<(const Date& date) const{
    if((*this).getAnnee()<date.getAnnee())
        return 1;
    if((*this).getAnnee()>date.getAnnee())
        return 0;
    else{
        if((*this).getMois()<date.getMois())
            return 1;
        if((*this).getMois()>date.getMois())
            return 0;
        else{
            if((*this).getJour()<date.getJour())
                return 1;
            else
                return 0;
        }
    }
}

/**
 * \brief Surcharge de l'operateur > pour comparer 2 dates
 * \param date : date à comparer
 * \return booleen vrai si la date à gauche de l'opérateur est supérieure à la date à droite, false sinon
 */
bool Date::operator>(const Date& date) const{
    if((*this).getAnnee()>date.getAnnee())
        return 1;
    if((*this).getAnnee()<date.getAnnee())
        return 0;
    else{
        if((*this).getMois()>date.getMois())
            return 1;
        if((*this).getMois()<date.getMois())
            return 0;
        else{
            if((*this).getJour()>date.getJour())
                return 1;
            else
                return 0;
        }
    }
}

/**
 * \brief Surcharge de l'operateur >= pour comparer 2 dates
 * \param date : date à comparer
 * \return booleen vrai si la date à gauche de l'opérateur est superieure ou égale à la date à droite, false sinon
 */
bool Date::operator>=(const Date& date) const{
    if((*this).getAnnee()>date.getAnnee())
        return 1;
    if((*this).getAnnee()<date.getAnnee())
        return 0;
    else{
        if((*this).getMois()>date.getMois())
            return 1;
        if((*this).getMois()<date.getMois())
            return 0;
        else{
            if((*this).getJour()>=date.getJour())
                return 1;
            else
                return 0;
        }
    }
}

/**
 * \brief Surcharge de l'operateur <= pour comparer 2 dates
 * \param date : date à comparer
 * \return booleen vrai si la date à gauche de l'opérateur est inférieur ou égale à la date à droite, false sinon
 */
bool Date::operator<=(const Date& date) const{
    if((*this).getAnnee()<date.getAnnee())
        return 1;
    if((*this).getAnnee()>date.getAnnee())
        return 0;
    else{
        if((*this).getMois()<date.getMois())
            return 1;
        if((*this).getMois()>date.getMois())
            return 0;
        else{
            if((*this).getJour()<=date.getJour())
                return 1;
            else
                return 0;
        }
    }
}

/**
 * \brief Surcharge de l'operateur << pour afficher une date
 * \param f : flux ostream
 * \param d : Date à passer sur le flux
 * \return la date sur un flux ostream
 */
ostream& operator<<(ostream& f, const Date& d){
    f << d.getJour() << "/" << d.getMois() << "/" << d.getAnnee();
    return f;
}

//===============
//TRIPLET
//================


/**
 * \brief Triplet::Triplet Constructeur de la classe triplet (gestion des équilibrages)
 * \param co : compte concret
 * \param d : debit
 * \param c : credit
 */
Triplet::Triplet(Compte_Concret* co,float d,float c):compte(co){
    if (c<0){
        d=d-c;
        c=0;
    }
    if (d<0){
        c=c-d;
        d=0;
    }
    if (c>0 && d>0){
        if (c<d){
            d=d-c;
            c=0;
        }else{
            c=c-d;
            d=0;
        }
    }
    debit=d;
    credit=c;
}

/**
 * \brief Surcharge de l'operateur << pour afficher un triplet
 * \param f : flux ostream
 * \param t : triplet à passer sur le flux
 * \return le triplet sur un flux ostream
 */
ostream& operator<<(ostream& f, const Triplet& t){
    f << t.getNom() << " | " << t.getDebit() << " | " << t.getCredit();
    return f;
}

//================
//TRANSACTION
//================

/**
 * \brief Transaction::Transaction Constructeur de la classe Transaction
 * \param d : date
 * \param r : reference
 * \param t : titre
 * \param l : liste de triplets
 */
Transaction::Transaction(Date d,string r,string t,vector<Triplet*> l){
    if (l.size()<2){
        throw TresorerieException("Erreur de taille de transaction");
    }else{
        float debit=0,credit=0;
        vector<Triplet*>::const_iterator itb=l.begin();
        const vector<Triplet*>::const_iterator ite=l.end();
        for(;itb!=ite;itb++){
            debit+=(*itb)->getDebit();
            credit+=(*itb)->getCredit();
        }
        if (debit!=credit){
            throw TresorerieException("Erreur d'équilibrage de la transaction");
        }else{
            listeTriplet=l;
            date=Date(d.getJour(),d.getMois(),d.getAnnee());
            refe=r;
            titre=t;
            rapproche = false;
        }
    }
}


/**
 * \brief Destructeur de la classe Transaction
 */
Transaction::~Transaction(){
    vector<Triplet*>::const_iterator itb=listeTriplet.begin();
    const vector<Triplet*>::const_iterator ite=listeTriplet.end();
    for(;itb!=ite;itb++){
        delete (*itb);
    }
}


/**
 * \brief Fonction qui vérifie si un compte concret fait partie d'une transaction
 * \param compte : compte concret
 * \return booleen vrai si le compte est dans la transaction, false sinon
 */
bool Transaction::isInside(const Compte_Concret& compte){
    vector<Triplet*>::const_iterator itb=listeTriplet.begin();
    const vector<Triplet*>::const_iterator ite=listeTriplet.end();
    for(;itb!=ite;itb++){
        if(((*itb)->getCompte())->getId()==compte.getId())
            return true;
    }
    return false;
}

/**
 * \brief Surcharge de l'operateur << pour afficher un triplet
 * \param f : flux ostream
 * \param t : transaction à passer sur le flux
 * \return les transactions sur un flux ostream
 */
ostream& operator<<(ostream& f, const Transaction& t){
    f << t.getDate() <<" | "<<t.getRef()<<" | "<<t.getTitre()<<" | ";
    vector<Triplet*>::const_iterator itb=t.getTriplet();
    const vector<Triplet*>::const_iterator ite=t.getEndTriplet();
    for(;itb!=ite;itb++){
        f<<(*itb)->getNom()<<" | "<<(*itb)->getDebit()<<" | "<<(*itb)->getCredit()<<"\n";
    }
    return f;
}

//===============
//TRANSACTION MANAGER
//=================

/**
 * \brief creation du Handler
 * \return le handler
 */
TransactionManager::Handler TransactionManager::handler = TransactionManager::Handler();

/**
 * \brief Fonction qui retourne une référence sur le TransactionManager s'il existe et le créé sinon
 * \return référence sur l'unique TransactionManager
 */
TransactionManager& TransactionManager::getManager() {
        if (!handler.instance) handler.instance = new TransactionManager;
        return *handler.instance;
}

/**
 * \brief Fonction qui libère l'unique TransactionManager
 */
void TransactionManager::libererManager() {
    handler.instance->save();
    delete handler.instance;
    handler.instance = nullptr;
}

/**
 * \brief Destructeur de TransactionManager
 */
TransactionManager::~TransactionManager(){
    vector<Transaction*>::const_iterator itb=transactions.begin();
    const vector<Transaction*>::const_iterator ite=transactions.end();
    for(;itb!=ite;itb++){
        delete (*itb);
    }
}

/**
 * \brief Fonction permettant d'obtenir l'adresse d'une transaction à partir de sa référence
 * \param refe : reference de la transaction recherchée
 * \return l'adresse de la transaction recherchée si trouvée et nullptr sinon
 */
Transaction* TransactionManager::getTransaction(const string& refe){
    vector<Transaction*>::const_iterator itb=getIteratorTransaction();
    const vector<Transaction*>::const_iterator ite=getEndIteratorTransaction();
    for (;itb!=ite;itb++) {
        if((*itb)->getRef()==refe)
            return (*itb);
    }
    return nullptr;
}

/**
 * \brief Fonction permettant d'ajouter une nouvelle transaction
 * \param t : référence sur la transaction à ajouter
 */
void TransactionManager::ajouterTransaction(Transaction& t){
        transactions.push_back(&t);
        vector<Triplet*>::const_iterator itb=t.getTriplet();
        const vector<Triplet*>::const_iterator ite=t.getEndTriplet();
        for(;itb!=ite;itb++){
            Compte_Concret* compte = (*itb)->getCompte();
            if (compte->getType() == Type::Actif || compte->getType() == Type::Depense){
                if ((*itb)->getCredit() != 0)
                    (*itb)->getCompte()->modifSolde(-(*itb)->getCredit());
                else
                    (*itb)->getCompte()->modifSolde((*itb)->getDebit());
            }else { //compte de type passif ou revenus
                if ((*itb)->getCredit() != 0)
                    (*itb)->getCompte()->modifSolde((*itb)->getCredit());
                else
                    (*itb)->getCompte()->modifSolde(-(*itb)->getDebit());
            }
        }
}

/**
 * \brief Fonction permettant la création d'une nouvelle transaction
 * \param d : date nouvelle transaction
 * \param r : reference nouvelle transaction
 * \param t : titre de la transaction
 * \param l : triplet de la transaction
 * \return bool true si la transaction est crée et false si elle existe déjà
 */
bool TransactionManager::creerTransaction(Date d, const string& r, const string& t,vector<Triplet*> l){
    if(getTransaction(r)!=nullptr)
        return false;
    else{
        Transaction *tr = new Transaction(d,r,t,l);
        ajouterTransaction(*tr);
        return true;
    }
}

/**
 * \brief Fonction permettant de supprimer une transaction existante
 * \param refe : reference de la transaction à supprimer
 * \return true si la transaction est supprimée et false sinon
 */
bool TransactionManager::supprimerTransaction(const string& refe){
    Transaction* transac=getTransaction(refe);
    if(transac==nullptr)
        throw TresorerieException("La transaction n'existe pas.\n");
    else{
        if(transac->getRapproche())
            throw TresorerieException("Transaction rapprochée, impossible de la supprimer.\n");
        else{
            vector<Transaction*>::const_iterator itb= getIteratorTransaction();
            const vector<Transaction*>::const_iterator ite=getEndIteratorTransaction();
            for(;itb!=ite;itb++){
                if((*itb)->getRef()==refe){
                    vector<Triplet*>::const_iterator tripletb=(*itb)->getTriplet();
                    const vector<Triplet*>::const_iterator triplete=(*itb)->getEndTriplet();
                    for(;tripletb!=triplete;tripletb++){
                        Compte_Concret* compte=(*tripletb)->getCompte();
                        if (compte->getType() == Type::Actif || compte->getType() == Type::Depense){
                            if ((*tripletb)->getCredit() != 0)
                                (*tripletb)->getCompte()->modifSolde((*tripletb)->getCredit());
                            else
                                (*tripletb)->getCompte()->modifSolde(-(*tripletb)->getDebit());
                        }else { //compte de type passif ou revenus
                            if ((*tripletb)->getCredit() != 0)
                                (*tripletb)->getCompte()->modifSolde(-(*tripletb)->getCredit());
                            else
                                (*tripletb)->getCompte()->modifSolde((*tripletb)->getDebit());
                        }
                    }
                    delete(*itb);
                    transactions.erase(itb);
                    return true;
                }
            }
            return false;
        }
    }
}

/**
 * \brief Fonction permettant de modifier une transaction existante (tout sauf sa reference)
 * \param r : reference de la transaction à modifier
 * \param newD : nouvelle date de la transaction
 * \param new_titre : nouveau titre de la transaction
 * \param new_vec : nouveau triplet de la transaction
 * \return false si la transaction n'est pas correctement modifiée et vrai sinon
 */
bool TransactionManager::modifierTransaction(const string& r, Date newD, const string& new_titre, vector<Triplet*> new_vec){
    Transaction * transac = getTransaction(r);
    if(transac == nullptr)
    {
        throw TresorerieException("La ref a cette Transaction n'existe pas\n");
    }
    if(transac->getRapproche() == 1)
    {
        throw TresorerieException("Cette transaction n'est plus modifiable\n");
    }
    bool supp = supprimerTransaction(r);
    if(!supp)
    {
        return false;
    }
    return creerTransaction(newD, r, new_titre, new_vec);
 }

/**
 * \brief Fonction permettant de voir les resultats du dernier rapprochement d'un compte par rapport au solde actuel
 * \param d : date
 * \param idCompte : id du compte à rapprocher
 * \return string: chaine de caractere affichant le resultat du dernier rapprochement
 */
string TransactionManager::voirRapprochement(Date d, int idCompte){
    Singleton& root = Singleton::donneInstance();
    std::ostringstream oss;
    Compte_Virtuel* compte=dynamic_cast<Compte_Virtuel*>(root.getCompte(idCompte));
    if(compte==nullptr){
        Compte_Concret* compte2=dynamic_cast<Compte_Concret*>(root.getCompte(idCompte));
        oss<<"Compte : "<<compte2->getId()<<" "<<compte2->getNom()<<endl;
        oss<<"Solde après le dernier rapprochement = "<<compte2->getSoldeDernierRapprochement()<<endl;
        oss<<"Solde actuel = "<<compte2->getSolde()<<endl;
        oss<<"\n";
        return oss.str();
    }
    else{
        oss<<"Compte : "<<compte->getId()<<" "<<compte->getNom()<<endl;
        oss<<"Solde après le dernier rapprochement = "<<compte->getSoldeDernierRapprochement()<<endl;
        oss<<"Solde actuel ="<<compte->getSolde()<<endl;
        oss<<"\n";
        vector<Compte_Abstrait*>::const_iterator itb=compte->getFirstCompte();
        const vector<Compte_Abstrait*>::const_iterator ite=compte->getLastCompte();
        for(;itb!=ite;itb++){
            oss<<voirRapprochement(d,(*itb)->getId());
        };
        return oss.str();
    }
}

/**
 * \brief Fonction permettant de valider un rapprochement en mettant le "setRapproche" des comptes impliqués à true
 * \param d : date
 * \param idCompte : id compte à valider
 */
void TransactionManager::validerRapprochement(Date d, int idCompte){
    Singleton& root = Singleton::donneInstance();
        Compte_Virtuel* compte=dynamic_cast<Compte_Virtuel*>(root.getCompte(idCompte));
        if(compte==nullptr){
            Compte_Concret* compte2=dynamic_cast<Compte_Concret*>(root.getCompte(idCompte));
            FilterIteratorRapproche it=getFilterIteratorRapproche(*compte2);
            for(;!it.isDone();it.next())
            {
                if(it.currentItem().getDate()<d || it.currentItem().getDate()==d)
                    it.currentItem().setRapproche();
            };
            compte2->modifSoldeDernierRapprochement();
        }
        else{
            vector<Compte_Abstrait*>::const_iterator itb=compte->getFirstCompte();
            const vector<Compte_Abstrait*>::const_iterator ite=compte->getLastCompte();
            for(;itb!=ite;itb++){
                validerRapprochement(d,(*itb)->getId());
            };
        }
}

/**
 * \brief Fonction permettant de sauvegarder les données de TransactionManager dans un fichier
 */
void TransactionManager::save() const {
        ofstream fout(filename.toStdString());
        vector<Transaction*>::const_iterator itb= getIteratorTransaction();
        const vector<Transaction*>::const_iterator ite=getEndIteratorTransaction();
        for (;itb!=ite;itb++) {
                fout << (*itb)->getDate().getJour() << ";" << (*itb)->getDate().getMois() << ";" << (*itb)->getDate().getAnnee() << ";" << (*itb)->getRef() << ";"<< (*itb)->getTitre() << ";";
                vector<Triplet*>::const_iterator itb1=(*itb)->getTriplet();
                const vector<Triplet*>::const_iterator ite1=(*itb)->getEndTriplet();
                for(;itb1!=ite1;itb1++){
                    fout << (*itb1)->getCompte()->getId() << ";"
                    << (*itb1)->getDebit() << ";"
                    << (*itb1)->getCredit() << ";";
                }
                //fin de la transaction
                fout << "\n";
        }
        fout.close();
}

/**
 * \brief Fonction permettant de récupérer les informations de TransactionManager depuis un fichier
 * \param f : nom du fichier d'où charger les transactions
 */
void TransactionManager::load(const QString& f) {
        if (filename != f) save();
        filename = f;
        ifstream fin(filename.toStdString()); // ouverture du fichier
        if (!fin) throw TresorerieException("erreur, probleme d'ouverture du fichier");
        Singleton& root=Singleton::donneInstance();
        Compte_Concret* compte;
        fin.get();
        if (!fin.eof()){
            fin.seekg(ios_base::beg); //On revient au début du fichier si il n'était pas vide
            while (!fin.eof() && fin.good()) {
                    string jour,mois,annee;
                    string refe;
                    string titre;
                    string Id;
                    string Debit;
                    string Credit;
                    vector<Triplet*> listeTriplet;
                    getline(fin, jour, ';');
                    if (fin.bad()) throw TresorerieException("erreur de lecture dans le fichier de transferts");
                    getline(fin, mois, ';');
                    if (fin.bad()) throw TresorerieException("erreur de lecture dans le fichier de transferts");
                    getline(fin, annee, ';');
                    if (fin.bad()) throw TresorerieException("erreur de lecture dans le fichier de transferts");
                    getline(fin, refe, ';');
                    if (fin.bad()) throw TresorerieException("erreur de lecture dans le fichier de transferts");
                    getline(fin, titre, ';');
                    if (fin.bad()) throw TresorerieException("erreur de lecture dans le fichier de transferts");
                    while (fin.peek()!='\n'){
                        getline(fin, Id, ';');
                        if (fin.bad()) throw TresorerieException("erreur de lecture dans le fichier de transferts");
                        getline(fin, Debit, ';');
                        if (fin.bad()) throw TresorerieException("erreur de lecture dans le fichier de transferts");
                        getline(fin, Credit, ';');
                        if (fin.bad()) throw TresorerieException("erreur de lecture dans le fichier de transferts");
                        compte=dynamic_cast<Compte_Concret*>(root.getCompte(stoi(Id)));
                        if (compte==nullptr)
                            throw TresorerieException("Le compte demandé n'existe pas");
                        Triplet* t1=new Triplet(compte,stoi(Debit),stoi(Credit));
                        listeTriplet.push_back(t1);
                    }
                    Date d(stoi(jour),stoi(mois),stoi(annee));
                    creerTransaction(d,refe,titre,listeTriplet);
                    while (fin.peek() == '\r') fin.ignore();
                    while (fin.peek() == '\n') fin.ignore();
            }
        }
        fin.close(); // close file
}

/**
 * \brief Fonction permettant d'effectuer la cloture du livret de compte
 * \param titre
 * \param d : date
 */
void TransactionManager::cloture(string titre,Date d){
    Singleton& root = Singleton::donneInstance();//on récupère le singleton pour la gestion des comptes
    TransactionManager& t=getManager();//on récupère le Transaction Manager unique
    Compte_Concret* resultat1=dynamic_cast<Compte_Concret*>(root.getCompte(0));//id de compte réservé au compte de résultat(les comptes créés commentcent à partir de id =3)
    if (resultat1==nullptr){//si resultat1=0, le cast s'est mal déroulé ou le compte n'existe pas
        resultat1=new Compte_Concret("Resultat",0,Type::Passif,0);
        root.Add(resultat1); //on créée le compte resultat sous la racine avec un id spécifié (0)
    }
    int i;
    float depense=0,recette=0;//on va stocker la somme des recettes et des dépenses qui sera utile dans la transaction avec Resultat
    vector<Triplet*> listedepense;
    vector<Triplet*> listerecette;//on créé des listes de triplet pour créer les transactions pour mettre les comptes à 0
    for(i=4;i<=root.getNumero();i++)//on a 4 id réservés et on balaye le reste des comptes
    {
        Compte_Abstrait* compte = root.getCompte(i); //on récupère le ième compte créé
        //cout << compte->getNom()<<"\n";
        Compte_Concret* compte1=dynamic_cast<Compte_Concret*>(compte);//on tente de le transformer en Compte concret
        if (compte1!=nullptr){//si ce n'est pas un compte Concret, dynamic cast renvoie nullptr
            if (compte1->getType()==Type::Recette){//on vérifie que c'est un compte de recette et on le stocke dans la liste de triplet avec son solde en débit pour le mettre à 0
                Triplet* t = new Triplet(compte1,compte1->getSolde(),0);
                listerecette.push_back(t);
                recette+=compte1->getSolde();//on calcule la somme des recette
            }else  if (compte1->getType()==Type::Depense){//pareil que pour les recettes mais avec un crédit à la place d'un débit
                Triplet* t =new Triplet(compte1,0,compte1->getSolde());
                listedepense.push_back(t);
                depense+=compte1->getSolde();
            }
        }
    }
    //Compte_Concret* resultat1=dynamic_cast<Compte_Concret*>(root.getCompte(0));
    string refeRec= std::to_string(d.getJour())+std::to_string(d.getMois())+std::to_string(d.getAnnee())+std::to_string(recette);
    if (listerecette.size()>0){//on vérifie qu'on a bien un compte dans la liste pour faire la transaction
        Triplet* tripletrecette = new Triplet(resultat1,0,recette);
        listerecette.push_back(tripletrecette);
        t.creerTransaction(d,refeRec,titre,listerecette);//si c'est le cas on ajoute une transaction avec la liste des recettes
    }
    string refeDep= std::to_string(d.getJour())+std::to_string(d.getMois())+std::to_string(d.getAnnee())+std::to_string(depense);
    if (listedepense.size()>0){//pareil que pour les recettes
        Triplet* tripletdepense= new Triplet(resultat1,depense,0);
        listedepense.push_back(tripletdepense);
        t.creerTransaction(d,refeDep,titre,listedepense);
    }
//fin création compte Resultat
    string refeRes= std::to_string(d.getJour())+std::to_string(d.getMois())+std::to_string(d.getAnnee())+std::to_string(resultat1->getSolde());
    if (resultat1->getSolde()>0){// si le solde est supérieur à 0, c'est un excédent, on crée donc le compte s'il n'existe pas et on lui trnasfere le solde du resultat
        float solde = resultat1->getSolde();
        Compte_Concret* excedent=dynamic_cast<Compte_Concret*>(root.getCompte(1));//id de compte réservé au compte de résultat(les comptes créés commentcent à partir de id =3)
        if (excedent==nullptr){
            excedent=new Compte_Concret("Excedent",1,Type::Passif,0);
            root.Add(excedent); //on créée le compte resultat sous la racine
        }
        //Compte_Concret* excedent1=dynamic_cast<Compte_Concret*>(root.getCompte(1));
        Triplet* result=new Triplet(resultat1,solde,0);
        Triplet* exced =new Triplet(excedent,0,solde);
        vector<Triplet*> liste;
        liste.push_back(result);
        liste.push_back(exced);
        t.creerTransaction(d,refeRes,titre,liste);

    }else{//sinon, on fait la même chose avec un compte de deficit
        Compte_Concret* deficit=dynamic_cast<Compte_Concret*>(root.getCompte(2));//id de compte réservé au compte de résultat(les comptes créés commentcent à partir de id =3)
        if (deficit==nullptr){
            deficit=new Compte_Concret("Deficit",2,Type::Passif,0);
            root.Add(deficit); //on créée le compte resultat sous la racine
        }
        //Compte_Concret* deficit1=dynamic_cast<Compte_Concret*>(root.getCompte(2));
        Triplet* result=new Triplet(resultat1,resultat1->getSolde(),0);
        Triplet* defi =new Triplet(deficit,0,resultat1->getSolde());
        vector<Triplet*> liste;
        liste.push_back(result);
        liste.push_back(defi);
        t.creerTransaction(d,refeRes,titre,liste);
    }
}

/**
 * \brief Fonction qui affiche le releve du livre de compte
 * \param debut : date du debut du releve
 * \param fin : date de fin du releve
 * \return chaine de caractere representant le releve de compte demandé
 */
string TransactionManager::releve(Date debut, Date fin){
    std::ostringstream oss;
    TransactionManager& t=getManager();
    vector<Transaction*>::const_iterator itb=transactions.begin();
    const vector<Transaction*>::const_iterator ite=transactions.end();
    for(;itb!=ite;itb++){
        if(((*itb)->getDate()<=fin) && ((*itb)->getDate()>=debut)){
            auto itb1=(*itb)->getTriplet();
            auto ite1=(*itb)->getEndTriplet();
            for(;itb1!=ite1;itb1++){
                if((*itb1)->getCompte()->getType()==Type::Depense || (*itb1)->getCompte()->getType()==Type::Recette){
                    oss<<(*t.getTransaction((*itb)->getRef()));
                }
            }
        }
    }
    return oss.str();
}
