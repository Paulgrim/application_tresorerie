#ifndef COMPTE_H
#define COMPTE_H

/**
 * \brief Document qui comporte la définitions des classes TresorerieException, Date, Compte_Abstrait, Compte_Concret, Compte_Virtuel, Singleton, CompteFactory, Triplet, TransactionManager
 */

#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <iterator>
#include <typeinfo>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <QString>
#include <QDate>
#include <iomanip>
#include <sstream>
using namespace std;


/**
 * \brief Les differents types de compte
 */
enum Type{Actif, Passif, Recette, Depense, Racine};


/**
 * \brief The TresorerieException class
 */
class TresorerieException{
public:
    TresorerieException(const string& message):info(message){}
    string getInfo() const { return info; }
private:
    string info;
};

class Date;

//========================
//Gestion des COMPTES
//========================

/**
 * \brief The Compte_Abstrait class
 */
class Compte_Abstrait
{
    protected:
        string nom;
        unsigned int id;
        Type type;
        static unsigned int numero;
        QString filename = "Comptes.dat";

    public:
        
        /**
        * \brief Méthode qui récupère le nom du compte Abstrait
        * \return Nom du compte 
        */
        string getNom() const{return nom;};
        virtual float getSolde() const=0;
        virtual float getSoldeDernierRapprochement() const=0;
        /**
        * \brief Méthode qui récupère le numéro du compte Abstrait
        * \return Numéro du compte (int)
        */
        int getNumero() const{return numero;};
        /**
        * \brief Méthode qui récupère l'id du compte Abstrait
        * \return Id du compte 
        */
        int getId() const {return id;}
        /**
        * \brief Méthode qui affiche le type du compte Abstrait
        * \return Type du compte sous forme d'une chaine de caractère
        */
        string afficherType() const{
            if(type == Type::Actif)
            {
                return "Actif";
            }
            if(type == Type::Passif)
            {
                return "Passif";
            }
            if(type == Type::Depense)
            {
                return "Depense";
            }
            if(type == Type::Recette)
            {
                return "Recette";
            }
            if(type == Type::Racine)
            {
                return "Racine";
            }
            return "type inconnu";
        };
        /**
        * \brief Méthode qui affiche le type du compte représenté par l'initial du type
        * \return Initial du type du compte sous forme d'une chaine de caractère
        */
        string afficherTypeDiscret() const{
            if(type == Type::Actif)
            {
                return "A";
            }
            if(type == Type::Passif)
            {
                return "P";
            }
            if(type == Type::Depense)
            {
                return "D";
            }
            if(type == Type::Recette)
            {
                return "R";
            }
            return "";
        };
        /**
        * \brief Méthode qui récupère le type du compte
        * \return Type du compte
        */
        Type getType() const {return type;}
        virtual string getArborescence(int h) const =0;
        virtual string getArborescenceBilan(int h) const=0;
        virtual Compte_Abstrait* getCompte(const string& nom)=0;
        virtual Compte_Abstrait* getCompte(int)=0;
        /**
        * \brief Méthode qui retourne le nom du fichier associé aux comptes
        * \return Nom du fichier 
        */
        QString getFilename() const{return filename;}
        virtual bool isVirtuel() const=0;
        virtual float getSoldeBilan()const=0;
        virtual void setSoldeBilan() = 0;
        virtual void save(ofstream& fout,int idPere)const=0;

        virtual ~Compte_Abstrait()=0;
        Compte_Abstrait(const string& n, Type t);
        /**
        * \brief Compte_Abstrait::Compte_Abstrait
        * \param n : nom du compte à creer
        * \param i : id du compte à créer
        * \param t : type de compte à creer
        */
        Compte_Abstrait(const string& n,int i, Type t):nom(n),type(t){
            id=i;
            if (id>=numero){
                numero = id+1;
            }
        }
};

/**
 * \brief The Compte_Concret class
 */
class Compte_Concret:public Compte_Abstrait{
private:
    float soldeReel;
    float soldeRapprochement;
    float soldeBilan;
public:
    /**
    * \brief Méthode qui récupère un pointeur sur un Compte_Abstrait (recherche par nom)
    * \param nom : nom du compte à récupérer
    * \return Pointeur sur Compte_Abstrait si compte trouvé et nullptr sinon
    */
    Compte_Abstrait* getCompte(const string& nom){
            if((this->getNom().compare(nom))==0)
                return (this);
            else
                return nullptr;
    };
    /**
    * \brief Méthode qui récupère un pointeur sur Compte_Abstrait (recherche par id)
    * \param id : id du compte à récupérer
    * \return Pointeur sur le Compte_Abstrait si compte trouvé et nullptr sinon
    */
    Compte_Abstrait* getCompte(int id){
        if(this->getId() == id)
                return (this);
            else
                return nullptr;
    };
    string getArborescence(int h) const ;
    string getArborescenceBilan(int h) const;
    /**
    * \brief Méthode qui récupère le solde du compte
    * \return Solde du compte
    */
    float getSolde()const {return soldeReel;}
    /**
    * \brief Méthode qui modifie le solde du compte
    * \param n : valeur à ajouter au compte
    */
    void modifSolde(float n){soldeReel+=n;}
    /**
    * \brief Méthode qui retourne le solde au moment du dernier rapprochement
    * \return Solde au dernier rapprochement
    */
    float getSoldeDernierRapprochement() const {return soldeRapprochement;}
    /**
    * \brief Méthode qui affecte le solde réel en tant que solde du dernier rapprochement
    */
    void modifSoldeDernierRapprochement(){soldeRapprochement=soldeReel;}
    /**
    * \brief Méthode qui retourne le solde du bilan
    * \return valeur du solde du bilan
    */
    float getSoldeBilan()const{return soldeBilan;}
    /**
    * \brief Méthode qui permet de modifier le solde du bilan
    * \param n : valeur à ajouter au solde du bilan
    */
    void modifSoldeBilan(float n){soldeBilan+=n;}
    /**
    * \brief Méthode qui affecte 0 au solde du bilan
    */
    void setSoldeBilan(){soldeBilan = 0;}
    /**
    * \brief Méthode qui retourne un booleen vrai si le compte est virtuel et faux sinon
    * \return Faux 
    */
    bool isVirtuel() const{return false;}
    void save(ofstream& fout,int idPere) const;

    /**
    * \brief Compte_Concret()
    * \param n : nom du compte concret à créer
    * \param t : type du compte concret à créer
    * \param s : solde du compte concret à créer
    */
    Compte_Concret(const string& n, Type t, float s=0):Compte_Abstrait(n,t),soldeReel(s),soldeRapprochement(s),soldeBilan(s){};
    /**
    * \brief Compte_Concret()
    * \param n : nom du compte concret à créer
    * \param i : id du compte concret à créer
    * \param t : type du compte concret à créer
    * \param s : solde du compte concret à créer
    */
    Compte_Concret(const string& n, int i, Type t, float s=0):Compte_Abstrait(n,i,t),soldeReel(s),soldeRapprochement(s),soldeBilan(s){};
    ~Compte_Concret()=default;
};


/**
 * \brief The Compte_Virtuel class
 */
class Compte_Virtuel: public Compte_Abstrait{
protected:
    vector<Compte_Abstrait*> liste_compte;
    float soldeBilan;
public:
    string getArborescence(int h) const;
    string getArborescenceBilan(int h) const;
    float getSolde() const;
    float getSoldeDernierRapprochement() const;
    float getSoldeBilan()const;
    Compte_Abstrait* getCompte(const string& nom);
    Compte_Abstrait* getCompte(int id);
    Compte_Virtuel& Add(Compte_Virtuel* compte);
    void Add(Compte_Concret* compte);
    bool supprimerElement(int idElement);
    int getIdPere(int idCompte);
    /**
    * \brief Méthode qui retourne un booleen Vrai si le compte est virtuel et Faux sinon
    * \return Vrai
    */
    bool isVirtuel()const{return true;}
    /**
    * \brief Méthode qui retourne un booleen Vrai si le compte est vide et Faux sinon
    * \return Vrai si compte vide et Faux sinon
    */
    bool isEmpty() const{return liste_compte.empty();}
    /**
    * \brief Méthode qui retourne un itérateur sur le premier compte de la liste contenue dans le compte virtuel 
    * \return itérateur du début de la liste de compte
    */
    vector<Compte_Abstrait*>::const_iterator getFirstCompte() const {return liste_compte.begin();};
    /**
    * \brief Méthode qui retourne un itérateur sur le dernier compte de la liste contenue dans le compte virtuel
    * \return itérateur sur la fin de la liste de compte
    */
    vector<Compte_Abstrait*>::const_iterator getLastCompte() const {return liste_compte.end();};
    /**
    * \brief Méthode qui retourne la liste des comptes contenus dans le compte virtuel
    * \return Vecteur de Compte_Abstrait* contenant les différents compte contenus dans le compte virtuel
    */
    vector<Compte_Abstrait*> getListeCompte() const{return liste_compte;};
    void save(ofstream& fout,int idPere) const;
    float getBilan(Date d);
    void setSoldeBilan();

    /**
    * \brief Compte_Virtuel()
    * \param n : nom du compte virtuel à créer
    * \param t : type du compte virtuel à créer
    */
    Compte_Virtuel(const string& n, Type t):Compte_Abstrait(n,t){};
    /**
    * \brief Compte_Virtuel()
    * \param n : nom du compte virtuel à créer
    * \param i : id du compte virtuel à créer
    * \param t : type du compte virtuel à créer
    */
    Compte_Virtuel(const string& n, int i,Type t):Compte_Abstrait(n,i,t){};
    ~Compte_Virtuel();
};

//=========================

/**
 * \brief The Singleton class
 */
class Singleton : public Compte_Virtuel{
private:
    static Singleton* compteRacine;
    /**
    * \brief Singleton()
    * \param n : nom du compte
    */
    Singleton(const string& n):Compte_Virtuel(n,Type::Racine){};
    Singleton(const Singleton&);
    virtual ~Singleton();
    Singleton(Singleton& s)=delete;
    Singleton& operator=(Singleton& s)=delete;

public:
    static Singleton& donneInstance();
    static void libereInstance();
    bool deplacer(int idCompte, int idAncien, int idPere);
    bool suppression(int id_pere, int id_Compte);
    float getSolde() const;
    float getSoldeBilan() const;
    /**
    * \brief Méthode qui permet d'affecter un nom au fichier contenant les comptes
    * \param f : Nom à donner au fichier contenant les comptes
    */
    void setFilename(const QString& f) {filename = f;}
    float resultat();
    void load(const QString& f);
    void save(ofstream& fout,int idPere) const;
    float getSommeActif() const;
    float getSommePassif() const;
};

//=====================

/**
 * \brief The CompteFactory class
 */
class CompteFactory{
public:
    void Fabrication(Type type, string nom, int idPere);
    void Fabrication(Type type, string nom, int idPere, float solde);
    void FabricationLoad(Type type, string nom, int idPere,int id);
    void FabricationLoad(Type type, string nom, int idPere,int id, float solde);
};

//=====================
//Gestion des TRANSACTIONS
//=====================

/**
 * \brief The Date class
 */
class Date{
private:
    int jour;
    int mois;
    int annee;

public:
    Date(int j, int m, int a);
    Date()=default;
    ~Date()=default;
    /**
    * \brief Méthode retournant le jour 
    * \return jour 
    */
    int getJour()const {return jour;};
    /**
    * \brief Méthode retournant le mois
    * \return mois
    */
    int getMois()const {return mois;};
    /**
    * \brief Méthode retournant l'année
    * \return année 
    */
    int getAnnee()const {return annee;};
    bool operator==(const Date& date) const;
    bool operator!=(const Date& date) const;
    bool operator<(const Date& date) const;
    bool operator>(const Date& date) const;
    bool operator>=(const Date& date) const;
    bool operator<=(const Date& date) const;
};
ostream& operator<<(ostream& f, const Date& d);


/**
 * \brief The Triplet class
 */
class Triplet{
private:
    Compte_Concret* compte;
    float debit;
    float credit;
public:
    /**
    * \brief Méthode retournant le type du compte impliqué dans le Triplet
    * \return type du compte
    */
    Type getType() const {return (*compte).getType();}
    /**
    * \brief Méthode retournant le nom du compte impliqué dans le Triplet 
    * \return nom du compte
    */
    string getNom() const {return (*compte).getNom();}
    /**
    * \brief Méthode retournant la valeur du débit du Triplet
    * \return valeur du debit
    */
    float getDebit() const {return debit;}
    /**
    * \brief Méthode retournant la valeur du crédit du Triplet
    * \return valeur credit
    */
    float getCredit() const {return credit;}
    /**
    * \brief Méthode retournant le compte concret impliqué dans le Triplet
    * \return compte concret du Triplet
    */
    Compte_Concret* getCompte(){return compte;}
    Triplet(Compte_Concret* co,float d,float c);
    ~Triplet()=default;
};
ostream& operator<<(ostream& f,const Triplet& t);

class TransactionManager;

/**
 * \brief The Transaction class
 */
class Transaction{
private:
    friend TransactionManager;
    Date date;
    string refe;
    string titre;
    vector<Triplet*> listeTriplet;
    bool rapproche;
    Transaction(Date d,string r,string t,vector<Triplet*> l);
    ~Transaction();

public :
    /**
    * \brief Méthode retournant la date de la Transactions
    * \return Date transaction
    */
    Date getDate() const{return date;}
    /**
    * \brief Méthode retournant le titre de la Transaction
    * \return Titre transaction
    */
    string getTitre() const{return titre;}
    /**
    * \brief Méthode retournant la référence de la transaction
    * \return Reference transaction
    */
    string getRef() const{return refe;}
    /**
    * \brief Méthode retournant un itérateur sur le début de la liste des triplets de Transaction
    * \return itérateur sur le début de la liste de triplets
    */
    vector<Triplet*>::const_iterator getTriplet() const {return listeTriplet.begin();};
    /**
    * \brief Méthode retournant un itérateur sur la fin de la liste des triplets de Transaction
    * \return itérateur sur la fin de la liste de triplets
    */
    vector<Triplet*>::const_iterator getEndTriplet() const {return listeTriplet.end();};
    /**
    * \brief Méthode permettant de définir une transaction comme rapproché en utilisant un booleen qui devient true pour une transaction rapproché, le booleen est false par défaut
    */
    void setRapproche(){rapproche = true;}
    /**
    * \brief Méthode retournant vrai si la transaction est rapproché et faux sinon
    * \return booleen vrai si la transaction est rapproché, faux sinon
    */
    bool getRapproche()const{return rapproche;}
    bool isInside(const Compte_Concret& compte);
};
ostream& operator<<(ostream& f, const Transaction& t);

/**
 * \brief The TransactionManager class
 */
class TransactionManager {
private :
    //pour pouvoir manipuler le vecteur
        friend Compte_Concret;
        vector<Transaction*> transactions;
        QString filename = "Transaction.dat";
        struct Handler {
                TransactionManager* instance = nullptr;
                ~Handler() {
                        delete instance;
                        instance = nullptr;
                }
                Handler() = default;
        };
        static Handler handler;
        TransactionManager() = default;
        ~TransactionManager();
        // constructeur de recopie
        TransactionManager(TransactionManager& m)=delete;
        // oprateur d'affectation
        TransactionManager& operator=(TransactionManager& m)=delete;

public:
    static TransactionManager& getManager();
    static void libererManager();
    /**
    * \brief Méthode qui retourne le nom de fichier contenant les transactions
    * \return nom du fichier
    */
    QString getFilename(){return filename;}
    /**
    * \brief Méthode permettant de définir le nom du fichier contenant les transactions
    * \param f : nom à donner au fichier
    */
    void setFilename(const QString& f) {filename = f;}
    Transaction* getTransaction(const string& refe);
    void ajouterTransaction(Transaction& t);
    bool creerTransaction(Date d, const string& r, const string& t, vector<Triplet*> l);
    bool supprimerTransaction(const string& refe);
    bool modifierTransaction(const string& r, Date newD, const string& new_titre, vector<Triplet*> new_vec);

    /**
    * \brief Méthode retournant un itérateur sur la première transaction 
    * \return itérateur sur la première transaction
    */
    vector<Transaction*>::const_iterator getIteratorTransaction() const {return transactions.begin();}
    /**
    * \brief Méthode retournant un itérateur sur la dernière transaction
    * \return itérateur sur la dernière transaction
    */
    vector<Transaction*>::const_iterator getEndIteratorTransaction() const {return transactions.end();}

    string voirRapprochement(Date d, int idCompte);
    void validerRapprochement(Date d, int idCompte);
    void cloture(string titre,Date d);
    string releve(Date debut, Date fin);
    //void setFilename(const Qstring& f) { filename = f; }
    //void load(const Qstring& f);
    void save() const;
    void load(const QString& f);


    /**
     * \brief la classe FilterIterator permet de parcourir les transactions associées d'un compte
     */
    class FilterIterator {
        friend TransactionManager;
        vector<Transaction*>::const_iterator first=getManager().getIteratorTransaction();
        const vector<Transaction*>::const_iterator last=getManager().getEndIteratorTransaction();
        const Compte_Concret& compte;
        void goToTheNextTranfert(){
            while (first!=last && !(*first)->Transaction::isInside(compte))
                first++;
        }
        FilterIterator(const Compte_Concret& c) :compte(c) {goToTheNextTranfert();}

    public:
        /**
        * \brief Méthode retournant vrai si toutes les transactions du filter iterator sont parcourues et faux sinon
        * \return vrai si on a parcouru toutes les transactions incluant un compte donné, faux sinon
        */
        bool isDone() const {return first == last;}
        /**
        * \brief Méthode permettant de passer à la transaction suivante au sein du filter iterator
        */
        void next() {
            if (isDone()) throw TresorerieException("erreur, next sur un iterateur en fin de sequence diteration");
            first++;
            goToTheNextTranfert();
        }
        /**
        * \brief Méthode retournant une référence sur la transaction "pointée" de l'itérateur 
        * \return Référence sur la transaction "pointée" par l'itérateur
        */
        Transaction& currentItem() const {
            if (isDone())
                throw TresorerieException("erreur, indirection sur un iterateur en fin de sequence diteration");
            return **first;
        }
    };
    /**
    * \brief Méthode permettant d'obtenir une instance du FilterIterator
    * \param c : Compte étant necessairement contenu dans les transactions choisies par le filter iterator
    * \return FilterIterator
    */
    FilterIterator getFilterIterator(const Compte_Concret& c) const { return FilterIterator(c);}


    /**
     * \brief la classe FilterIteratorRapproche permet de parcourir les transactions non rapprochées d'un compte
     */
    class FilterIteratorRapproche:public FilterIterator{
        friend TransactionManager;
        /**
        * \brief Méthode permettant d'itérer sur la prochaine transaction non rapprochée
        */
        void goToTheNextTranfert(){
            while (first!=last && !(*first)->Transaction::isInside(compte) && !((*first)->getRapproche()))
                first++;
        }
        /**
        * \brief FilterIteratorRapproche()
        * \param c : compte nécessairement présent dans les transactions sur lesquelles on va itérer
        */
        FilterIteratorRapproche(const Compte_Concret& c) :FilterIterator(c) {goToTheNextTranfert();}
    };
    /**
    * \brief Méthode permettant d'obtenir une instance du FilterIteratorRapproche
    * \param c : Compte étant necessairement contenu dans les transactions choisies par le filter iterator rapproche
    * \return FilterIteratorRapproche
    */
    FilterIteratorRapproche getFilterIteratorRapproche(Compte_Concret& c) const {return FilterIteratorRapproche(c);}
};
#endif


