#ifndef INTERFACE_H
#define INTERFACE_H

/**
 * \brief Document qui comporte la définition des classes relatives à l'aspect graphique : Widget_demarrage, Fenetre_principal, Widget_global, Widget_demarrage, Accueil, Bilan, Resultat, Gestion_Compte, Rapprochement, Releve, Nouvelle_Transaction, Modification_Transaction, Gestion_Transaction
 *
 * Chaque classe correspond à une nouvelle fenêtre
*/


#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTextEdit>
#include <QHeaderView>
#include <QPushButton>
#include <QApplication>
#include <QString>
#include <QDebug>
#include <QLineEdit>
#include <QDateEdit>
#include <QDate>
#include <QFileDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QVector>
#include "Compte.h"

class Widget_demarrage;
class Fenetre_principal;


/**
 * \brief la class Widget_global qui est le parent de tous les autres Widgets
 */
class  Widget_global : public QWidget{
    Q_OBJECT
public :
    explicit Widget_global(QWidget* parent = nullptr);
public slots:
    void lancer_app(QStringList);
    void lancer_par_defaut(QStringList);
private :
    Widget_demarrage *depart;
    Fenetre_principal* fen;
    Singleton* sing;
    TransactionManager* tran;
};

/**
 * \brief la class Widget_demarrage, le widget sur lequel on arrive au lancement de l'application
 */
class Widget_demarrage : public QWidget{
    Q_OBJECT
public :
    explicit Widget_demarrage(QWidget * parent = nullptr);
signals :
    void fichier_load(QStringList);
    void demarrage_de_0(QStringList);
public slots :
    void chargement_fichier();
    void nouveau_fichier();
private :
    QPushButton* load;
    QPushButton* nouveau;
    QVBoxLayout* h;
};


/**
 * \brief la class Accueil
 */
class Accueil : public QWidget{
    Q_OBJECT
public :
    explicit Accueil(QWidget * parent = nullptr);
signals :
public slots :
    void chargerArbo();
    void quitter();
private :
    QTextEdit* arborescence;
    QPushButton* gestion;
    QPushButton* bilan;
    QPushButton* resultat;
    QPushButton* releve;
    QPushButton* gestionTransaction;
    QPushButton* quit;
    QVBoxLayout* v;
    QHBoxLayout* h;

};


/**
 * \brief la class Bilan
 */
class Bilan : public QWidget{
    Q_OBJECT
public:
    explicit Bilan(QWidget* parent=nullptr);
signals:
public slots:
    void calculerBilan();
    void cacher();
private:
    QDateEdit* date;
    QPushButton* calculer;
    QPushButton* retour;
    QLabel* texte1;
    QLineEdit* texte2;
    QTextEdit* texte3;
    QVBoxLayout* v;
    QHBoxLayout* h1;
    QHBoxLayout* h2;
};


/**
 * \brief la class Resultat
 */
class Resultat : public QWidget{
    Q_OBJECT
public :
    explicit Resultat(QWidget * parent = nullptr);
signals :
public slots :
    void mise_a_jour();
    void calcul_cloture();
    void cacher();
private:
    QHBoxLayout* h;
    QHBoxLayout* h1;
    QVBoxLayout* v;
    QLabel* resultat;
    QLineEdit* somme;
    QLineEdit* titre;
    QDateEdit* date;
    QPushButton* cloture;
    QPushButton* retour;
};


/**
 * \brief la class Gestion_Compte
 */
class Gestion_Compte : public QWidget{
    Q_OBJECT
    friend Fenetre_principal;
public :
    explicit Gestion_Compte(QWidget* parent=nullptr);
    void chargerArbo();
    void chargerComboBox(Compte_Abstrait* c);
signals :
    void sigRapprochement(QString);

public slots :
    void ajouterCompte();
    void supprimerCompte();
    void deplacerCompte();
    void refresh();
    void cacher();
    void envoyerNom();
    void mise_a_jour_combobox();
protected:
    QHBoxLayout* h;
    QHBoxLayout* h1;
    QVBoxLayout* v;
    QVBoxLayout* v1;
    QVBoxLayout* v2;
    QTextEdit* arborescence;
    QLineEdit* erreur;
    QPushButton* retour;
    QComboBox* listeComptes;
    QPushButton* ajout;
    QPushButton* sup;
    QPushButton* deplacer;
//Package pour l'ajout
    QHBoxLayout* hType;
    QHBoxLayout* hID;
    QHBoxLayout* hNom;
    QHBoxLayout* hSolde;
    QRadioButton* virtuel;
    QLabel* type;
    QComboBox* listeType;
    QLabel* nom;
    QLineEdit* nom1;
    QComboBox* listeIdPere;
    QLabel* idPere;
    QLabel* solde;
    QDoubleSpinBox* solde1;
//Package pour déplacer
    QHBoxLayout* hID2;
    QComboBox* listeIdPere2;
    QLabel* idPere2;
    //Rapprochement
    QPushButton* rappro;

};


/**
 * \brief la class Rapprochement
 */
class Rapprochement: public QWidget{
    Q_OBJECT
public:
    explicit Rapprochement(QWidget* parent=nullptr);
    void setS(QString i){s = i;}
signals:
public slots:
    void validerRapprochement();
    void voirRapprochement();
    void cacher();
private:
    QString s;
    QHBoxLayout* h;
    QVBoxLayout* v;
    QPushButton* rappro;
    QDateEdit* date;
    QPushButton* voir;
    QTextEdit* affichage;
    QPushButton* valider;
    QPushButton* retour;
};

/**
 * \brief la class Releve
 */
class Releve : public QWidget{
    Q_OBJECT
public:
    explicit Releve(QWidget* parent=nullptr);
signals:
public slots:
    void afficherReleve();
private:
    QDateEdit* dateDebut;
    QDateEdit* dateFin;
    QPushButton* afficher;
    QPushButton* retour;
    QLabel* texte1;
    QTableWidget* vueTransferts;
    QVBoxLayout* v;
    QHBoxLayout* h1;
    QHBoxLayout* h2;
public:
    void afficherAccueilReleve();
};


/**
 * \brief la class Nouvelle_Transaction
 */
class Nouvelle_Transaction : public QWidget{
    Q_OBJECT
public :
    explicit Nouvelle_Transaction(QWidget * parent = nullptr);
signals :
    void retour(int); //1 pbm taille 2 pbm equi
public slots :
    void ajouter_Triplet(); //get compte, getcredit et getdebit
    void ajouter_Transaction(); //get les triplets, get le titre, get la ref, get Date
    void annulation();
    void mise_a_jour_combobox();
private:
    vector<Triplet*> t;
    QLabel* debit_label;
    QLabel* credit_label;
    QLabel* titre_label;
    QLabel* ref_label;
    QDoubleSpinBox* debit;
    QDoubleSpinBox* credit;
    QLineEdit* ref;
    QLineEdit* titre;
    QTextEdit* recap;
    QDateEdit* date_Transac;
    QComboBox* listeComptes;
    QPushButton* ajouter_triplet;
    QPushButton* annuler;
    QPushButton* ajouter_Transa;
    QHBoxLayout* triplet;
    QHBoxLayout* reca_ajoutTriplet;
    QHBoxLayout* titre_ref_date;
    QHBoxLayout* valider_annuler;
    QVBoxLayout* v1;
public :
    void chargerComboBox(Compte_Abstrait* c);
};


/**
 * \brief la class Modification_Transaction
 */
class Modification_Transaction : public QWidget{
    Q_OBJECT
public :
    explicit Modification_Transaction(QWidget * parent = nullptr);
signals :
    void retour(int); //1 pbm taille 2 pbm equi
public slots :
    void ajouter_Triplet(); //get compte, getcredit et getdebit
    void modifier_Transaction(); //get les triplets, get le titre, get la ref, get Date
    void annulation();
    void chargerComboBoxComptesRef();
    void chargerComboBoxComptesNonRef();
    void afficher_Triplet();
    void afficher_Transaction();
    void modifier_Triplet();
private:
    vector<Triplet*> triple;
    QLabel* debit_label;
    QLabel* credit_label;
    QLabel* titre_label;
    QLabel* ref_label;
    QDoubleSpinBox* debit;
    QDoubleSpinBox* credit;
    QLineEdit* ref;
    QLineEdit* titre;
    QTextEdit* recap;
    QDateEdit* date_Transac;
    QComboBox* listeReference;
    QComboBox* listeComptes;
    QPushButton* ajouter_triplet_amont;
    QPushButton* modifier_triplet_amont;
    QPushButton* ajouter_triplet_aval;
    QPushButton* modifier_triplet_aval;
    QPushButton* annuler;
    QPushButton* modifier_Transa;
    QPushButton* choisir_ref;
    QHBoxLayout* triplet;
    QHBoxLayout* reca_ajoutTriplet;
    QHBoxLayout* titre_ref_date;
    QHBoxLayout* valider_annuler;
    QVBoxLayout* v1;
public :
    void routine_triplet();
    void routine_fin();
    void chargerComboBoxRef();
    void ajouter_RecursivementComptesNonRef(Compte_Abstrait* c,Transaction* t);
};

/**
 * \brief la class Gestion_Transaction
 */
class Gestion_Transaction : public QWidget{
    Q_OBJECT
public :
    explicit Gestion_Transaction(QWidget * parent = nullptr);
    void chargerComboBox(Compte_Abstrait*);
    void refresh();
    void traitementTransaction(const Transaction& transactionCourante);

    /**
     * \brief fonction permettant de changer l'indice de la comboBox contenant les comptes
     * \param i : le nouvel index
     */
    void setListeCompteIndex(int i ) { listeComptes->setCurrentIndex(i);}
signals :

public slots :
    void nouvelle_Transaction(); //ajouter une transaction
    void fin_ajouter(int);
    void supprimer_Transaction();
    void modifier_Transaction(); //pouvoir modifier que les transactions non rapprochées
    void fin_modifier(int);

    /**
     * \brief slot permettant d'appeler la fonction qui refresh les transactions
     */
    void refreshBox(){refresh();};
private:
    QLabel* titre;
    QTableWidget * vueTransac;
    Nouvelle_Transaction *n;
    Modification_Transaction *m;
    QComboBox* listeComptes;
    QPushButton* ajouter;
    QPushButton* modifier;
    QPushButton* supprimer;
    QPushButton* retour;
    QHBoxLayout* h1;
    QVBoxLayout* v1;
    QVBoxLayout* v2;
};

/**
 * \brief la class Fenetre_principal
 */
class Fenetre_principal : public QWidget{
    Q_OBJECT
public :
    explicit Fenetre_principal(QWidget * parent = nullptr);
signals :
    void cacherBouton();
public slots :
        void afficherBilan();
        void retourBilan();
        void afficherResultat();
        void retourResultat();
        void afficherGestionCompte();
        void retourGestionCompte();
        void afficherTransaction();
        void retourTransaction();
        void afficherReleve();
        void retourReleve();
        void afficherRapprochement(QString);
        void retourRapprochement();
private :
    Widget_demarrage* depart;
    Accueil* a;
    Bilan* b;
    Resultat* resu;
    Gestion_Compte* g;
    Releve *rele;
    Gestion_Transaction* t;
    Rapprochement* rappro;
};
#endif // INTERFACE_H
