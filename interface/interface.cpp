#include "interface.h"

/**
 * \brief Document qui comporte la définition des fonctions des classes relatives à l'aspect graphique : Widget_demarrage, Fenetre_principal, Widget_global, Widget_demarrage, Accueil, Bilan, Resultat, Gestion_Compte, Rapprochement, Releve, Nouvelle_Transaction, Modification_Transaction, Gestion_Transaction
 */

//------------------
//Widget Demarrage
//-----------------


/**
 * \brief Widget_demarrage::Widget_demarrage Constructeur du Widget_demarrage
 * \param parent : le widget parent
 */
Widget_demarrage::Widget_demarrage(QWidget *parent) : QWidget(parent){
    setFixedSize(400,400);
    load = new QPushButton("Charger un fichier", nullptr);
    nouveau = new QPushButton("Nouvelle association", nullptr);
    h = new QVBoxLayout(this);
    h->addWidget(load);
    h->addWidget(nouveau);
    connect(load, SIGNAL(clicked()), this, SLOT(chargement_fichier()));
    connect(nouveau, SIGNAL(clicked()), this, SLOT(nouveau_fichier()));
    connect(this, SIGNAL(fichier_load(QStringList)), parent, SLOT(lancer_app(QStringList)));
    connect(this, SIGNAL(demarrage_de_0(QStringList)), parent, SLOT(lancer_par_defaut(QStringList)));
}

/**
 * \brief slot appellé si l'utilisateur choisis de ne pas charger d'anciens fichiers et qui permet la création d'un fichier pour sauvegarder les comptes et d'un fichier pour sauvegarder les transactions à la reception d'un signal associé
 */
void Widget_demarrage::nouveau_fichier(){
    QStringList a;
    QString cpt = QFileDialog::getSaveFileName();
    QString transaction = QFileDialog::getSaveFileName();
    a.append(cpt);
    a.append(transaction);
    emit demarrage_de_0(a);
}

/**
 * \brief slot appellé si l'utilisateur choisis de charger des comptes et des transactions déjà enregistrés à la reception d'un signal associé
 */
void Widget_demarrage::chargement_fichier(){
    QString cpt = QFileDialog::getOpenFileName();
    QString transaction = QFileDialog::getOpenFileName();
    QStringList a;
    a.append(cpt);
    a.append(transaction);
    emit fichier_load(a);
}

//---------------
//Accueil
//---------------

/**
 * \brief Accueil::Accueil Constructeur du Widget Accueil
 * \param parent : le widget parent
 */
Accueil::Accueil(QWidget* parent) : QWidget(parent){
    arborescence = new QTextEdit();
    arborescence->setFixedSize(600,400);
    gestion = new QPushButton("Gestion Compte", nullptr);
    bilan = new QPushButton("Bilan", nullptr);
    resultat = new QPushButton("Resultat", nullptr);
    releve = new QPushButton("Releve", nullptr);
    gestionTransaction = new QPushButton("Gérer Transaction",nullptr);
    quit = new QPushButton("Quitter",nullptr);
    connect(quit,SIGNAL(clicked()),this,SLOT(quitter()));
    connect(bilan,SIGNAL(clicked()),parent,SLOT(afficherBilan()));
    connect(resultat, SIGNAL(clicked()),parent, SLOT(afficherResultat()));
    connect(releve, SIGNAL(clicked()),parent, SLOT(afficherReleve()));
    connect(gestion, SIGNAL(clicked()),parent, SLOT(afficherGestionCompte()));
    connect(gestionTransaction, SIGNAL(clicked()),parent, SLOT(afficherTransaction()));

    v = new QVBoxLayout;
    v->addWidget(gestion);
    v->addWidget(bilan);
    v->addWidget(resultat);
    v->addWidget(releve);
    v->addWidget(gestionTransaction);
    v->addWidget(quit);
    h = new QHBoxLayout(this);
    h->addWidget(arborescence);
    h->addLayout(v);
}

/**
 * \brief fonction qui permet d'afficher l'arborescence des comptes à la reception d'un signal associé
 */
void Accueil::chargerArbo(){
    Singleton& s = Singleton::donneInstance();
    arborescence->setText(QString::fromStdString(s.getArborescence(0)));
}

/**
 * \brief slot qui permet de quitter le widget Accueil à la reception d'un signal associé
 */
void Accueil::quitter(){
    Singleton& s = Singleton::donneInstance();
    s.libereInstance();
    TransactionManager& tm=TransactionManager::getManager();
    tm.libererManager();
    exit(0);
}

//-------------
//Bilan
//-------------

/**
 * \brief Bilan::Bilan Constructeur du Widget Bilan
 * \param parent : le widget parent
 */
Bilan::Bilan(QWidget* parent) : QWidget(parent){
    date=new QDateEdit();
    calculer = new QPushButton("Calculer le bilan",nullptr);
    retour=new QPushButton("Retour Accueil",nullptr);
    connect(calculer,SIGNAL(clicked()),this,SLOT(calculerBilan()));
    connect(retour,SIGNAL(clicked()),parent,SLOT(retourBilan()));
    connect(retour,SIGNAL(clicked()),this,SLOT(cacher()));
    texte1=new QLabel("Bilan :",this);
    texte2=new QLineEdit();
    texte3 = new QTextEdit();
    texte3->setFixedSize(600,400);
    v=new QVBoxLayout(this);
    h1=new QHBoxLayout();
    h2=new QHBoxLayout();
    h1->addWidget(date);
    h1->addWidget(calculer);
    h2->addWidget(texte1);
    h2->addWidget(texte2);
    v->addLayout(h1);
    v->addLayout(h2);
    v->addWidget(texte3);
    v->addWidget(retour);
}

/**
 * \brief slot qui permet de calculer le Bilan des comptes à la reception d'un signal associé
 */
void Bilan::calculerBilan(){
    try {
        Singleton& s = Singleton::donneInstance();
        QString d=(date->date()).toString("dd.MM.yyyy");
        Date dat=Date((d.mid(0,2)).toInt(),(d.mid(3,2)).toInt(),(d.mid(6,4)).toInt());
        s.getBilan(dat);
        QString bilan=QString::number(s.getSoldeBilan());
        texte2->setText(bilan);
        QString affichage="Répartition :\n" + QString::fromStdString(s.getArborescenceBilan(0));
        affichage+="\nSomme des actifs :" + QString::number(s.getSommeActif());
        affichage+="\nSomme des passifs :" + QString::number(s.getSommePassif());
        texte3->setText(affichage);
        //sauvegarde
        QString str=QString::number(dat.getJour())+QString::number(dat.getMois())+QString::number(dat.getAnnee());
        ofstream f(str.toStdString()+"_Bilan.dat");
        f<<"Le bilan au "+d.toStdString()+" est de :";
        f<<s.getSoldeBilan()<<endl;
        f<<"La répartition dans les sous-comptes est la suivante : \n";
        f<<s.getArborescenceBilan(0);
    } catch (TresorerieException e) {
        texte2->setText(QString::fromStdString(e.getInfo()));
    }
}

/**
 * \brief slot qui permet de cacher le Widget Bilan à la reception d'un signal associé
 */
void Bilan::cacher(){
    texte3->setText(NULL);
    texte2->setText(NULL);
    date->setDate(QDate(2000,01,01));
}

//---------------
//Resultat
//----------------


/**
 * \brief Resultat::Resultat Constructeur du Widget Resultat
 * \param parent : le widget parent
 */
Resultat::Resultat(QWidget* parent) : QWidget(parent){
    v=new QVBoxLayout(this);
    h = new QHBoxLayout();
    h1 = new QHBoxLayout();
    retour=new QPushButton("Retour Accueil",nullptr);
    cloture=new QPushButton("Faire une cloture du compte",nullptr);
    connect(retour,SIGNAL(clicked()),parent,SLOT(retourResultat()));
    connect(retour,SIGNAL(clicked()),this,SLOT(cacher()));
    connect(cloture,SIGNAL(clicked()),this,SLOT(calcul_cloture()));
    resultat = new QLabel("Resultat :",this);
    somme = new QLineEdit(this);
    titre= new QLineEdit("Entrez le titre",nullptr);
    somme->setReadOnly(true);
    Singleton& s = Singleton::donneInstance();
    int so=s.resultat();
    somme->setText(QString::number(so));
    date=new QDateEdit();
    h->addWidget(resultat);
    h->addWidget(somme);
    h1->addWidget(titre);
    h1->addWidget(date);
    h1->addWidget(cloture);
    v->addLayout(h);
    v->addLayout(h1);
    v->addWidget(retour);
}

/**
 * \brief slot qui permet la mise à jour du Widget Resultat au déclenchement d'un signal associé
 */
void Resultat::mise_a_jour(){
    Singleton& s = Singleton::donneInstance();
    float so=s.resultat();
    somme->setText(QString::number(so));
}

/**
 * \brief slot qui permet le calcul de la cloture au déclenchement d'un signal associé
 */
void Resultat::calcul_cloture(){
    try {
        //Création et ouverture du fichier de sauvegarde
        QString d=(date->date()).toString("dd.MM.yyyy");
        Date dat=Date((d.mid(0,2)).toInt(),(d.mid(3,2)).toInt(),(d.mid(6,4)).toInt());
        QString str=QString::number(dat.getJour())+QString::number(dat.getMois())+QString::number(dat.getAnnee());
        ofstream f(str.toStdString()+"_Resultat.dat");

        TransactionManager& tm= TransactionManager::getManager();
        Singleton& s=Singleton::donneInstance();
        QString tit=titre->text();
        f<<"Le résultat au "+d.toStdString()+" est de :";
        f<<somme->text().toStdString()<<endl;
        f<<"La répartition dans les sous-comptes est la suivante : \n";
        f<<s.getArborescence(0);
        tm.cloture(tit.toStdString(),dat); //réalisation de la cloture
        f<<"Après cloture, l'état des comptes est le suivant :\n";
        f<<s.getArborescence(0);
        this->mise_a_jour();
    } catch (TresorerieException e) {
        somme->setText(QString::fromStdString(e.getInfo()));
    }
}

/**
 * \brief slot qui permet de cacher le widget Resultat au déclenchement d'un signal associé
 */
void Resultat::cacher(){
    titre->setText("Entrez un titre");
    date->setDate(QDate(2000,01,01));
}

//----------
//Relevé des dépenses et recettes
//------------

/**
 * \brief Releve::Releve Constructeur du Widget Releve
 * \param parent : le widget parent
 */
Releve::Releve(QWidget* parent) : QWidget(parent){
    dateDebut=new QDateEdit();
    dateFin=new QDateEdit();
    afficher = new QPushButton("Afficher le releve",nullptr);
    retour=new QPushButton("Retour Accueil",nullptr);
    connect(afficher,SIGNAL(clicked()),this,SLOT(afficherReleve()));
    connect(retour,SIGNAL(clicked()),parent,SLOT(retourReleve()));

    texte1=new QLabel("Releve :",this);

    vueTransferts = new QTableWidget(0,6);
    connect(retour,SIGNAL(clicked()),vueTransferts,SLOT(clearContents()));
    vueTransferts->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vueTransferts->verticalHeader()->setVisible(false);
    QStringList HHeaderList;
    HHeaderList<<"Date"<<"Reference"<<"Titre"<<"Comptes"<<"Debit"<<"Credit";
    vueTransferts->setHorizontalHeaderLabels(HHeaderList);
    vueTransferts->setFixedWidth(600);
    vueTransferts->setFixedHeight(600);

    v=new QVBoxLayout(this);
    h1=new QHBoxLayout();
    h2=new QHBoxLayout();
    h1->addWidget(dateDebut);
    h1->addWidget(dateFin);
    h1->addWidget(afficher);
    h2->addWidget(texte1);
    v->addLayout(h1);
    v->addLayout(h2);
    v->addWidget(vueTransferts);
    v->addWidget(retour);
}

/**
 * \brief fonction qui permet l'affichage de l'accueil du widget Releve
 */
void Releve::afficherAccueilReleve(){
    texte1->hide();
}

/**
 * \brief slot qui permet d'afficher le widget Releve à réception du signal associé
 */
void Releve::afficherReleve(){
    afficher->hide();
    texte1->show();
    afficher->show();
    QString d1=(dateDebut->date()).toString("dd.MM.yyyy");
    Date dat1=Date((d1.mid(0,2)).toInt(),(d1.mid(3,2)).toInt(),(d1.mid(6,4)).toInt());

    QString d2=(dateFin->date()).toString("dd.MM.yyyy");
    Date dat2=Date((d2.mid(0,2)).toInt(),(d2.mid(3,2)).toInt(),(d2.mid(6,4)).toInt());

    TransactionManager& tm=TransactionManager::getManager();
    vector<Transaction*>::const_iterator itb=tm.getIteratorTransaction();
    const vector<Transaction*>::const_iterator ite=tm.getEndIteratorTransaction();
    int i = 0;
    for(;itb!=ite;itb++){
        if(((*itb)->getDate()<=dat2) && ((*itb)->getDate()>=dat1)){
            vector<Triplet*>::const_iterator itb1=(*itb)->getTriplet();
            const vector<Triplet*>::const_iterator ite1=(*itb)->getEndTriplet();
            QString s;
            bool flag = false;
            unsigned int j = 0;
            for(;itb1!=ite1;itb1++){
                if((*itb1)->getType()==Type::Depense || (*itb1)->getType()==Type::Recette){
                    flag = true;
                    break;
                }
            }
            if (flag == true){
                itb1 = (*itb)->getTriplet();
                for(;itb1!=ite1;itb1++){
                        vueTransferts->insertRow(i+j);
                        if (j == 0){
                            QString date = QString::number((*itb)->getDate().getJour()) + "/" + QString::number((*itb)->getDate().getMois()) + "/" + QString::number((*itb)->getDate().getAnnee());
                            vueTransferts->setItem(i+j, 0, new QTableWidgetItem(date));
                            vueTransferts->setItem(i+j, 1, new QTableWidgetItem(QString::fromStdString((*itb)->getRef())));
                            vueTransferts->setItem(i+j, 2, new QTableWidgetItem(QString::fromStdString((*itb)->getTitre())));

                         }
                         vueTransferts->setItem(i+j, 3, new QTableWidgetItem(QString::fromStdString((*itb1)->getCompte()->getNom())));
                         if ((*itb1)->getDebit() != 0)
                             vueTransferts->setItem(i+j, 4, new QTableWidgetItem(QString::number((*itb1)->getDebit())));
                         else
                             vueTransferts->setItem(i+j, 5, new QTableWidgetItem(QString::number((*itb1)->getCredit())));
                        j++;
                        vueTransferts->resizeRowsToContents();
                }
            }
            i += j;
        }
    }
    for(; i<vueTransferts->rowCount(); i++) vueTransferts->hideRow(i);
    Date dt=Date(QDate::currentDate().day(),QDate::currentDate().month(),QDate::currentDate().year());
    QString str=QString::number(dt.getJour())+QString::number(dt.getMois())+QString::number(dt.getAnnee());
    ofstream f(str.toStdString()+"_Releve.dat");
    f<<"Relevé du "+d1.toStdString()+" au "+d2.toStdString()+" :\n";
    f<<tm.releve(dat1,dat2)<<endl;
}

//----------
//Gestion des Comptes
//------------

/**
 * \brief Gestion_Compte Constructeur du Widget Gestion_Compte
 * \param parent : le widget parent
 */
Gestion_Compte::Gestion_Compte(QWidget* parent): QWidget(parent){
    arborescence = new QTextEdit();
    arborescence->setFixedSize(500,400);
    v = new QVBoxLayout(this);
    h = new QHBoxLayout();
    h1 = new QHBoxLayout();
    v1 = new QVBoxLayout();
    v2 = new QVBoxLayout();
    hType = new QHBoxLayout();
    hID = new QHBoxLayout();
    hNom = new QHBoxLayout();
    hSolde = new QHBoxLayout();
    erreur = new QLineEdit();
    erreur->setReadOnly(true);
    erreur->hide();
    ajout=new QPushButton("Ajouter un nouveau compte",nullptr);
    connect(ajout,SIGNAL(clicked()),this,SLOT(ajouterCompte()));
    connect(ajout,SIGNAL(clicked()),this,SLOT(mise_a_jour_combobox()));
    sup=new QPushButton("Supprimer le compte",nullptr);
    sup->hide();
    connect(sup,SIGNAL(clicked()),this,SLOT(supprimerCompte()));
    connect(sup,SIGNAL(clicked()),this,SLOT(mise_a_jour_combobox()));
    deplacer=new QPushButton("Deplacer le compte",nullptr);
    deplacer->hide();
    connect(deplacer,SIGNAL(clicked()),this,SLOT(deplacerCompte()));
    retour=new QPushButton("Retour Accueil",nullptr);
    rappro=new QPushButton("Rapprocher ce compte",nullptr);
    connect(rappro,SIGNAL(clicked()),this,SLOT(envoyerNom()));
    connect(rappro,SIGNAL(clicked()),this,SLOT(cacher()));

    rappro->hide();
//Widget pour ajouter
    connect(retour,SIGNAL(clicked()),parent,SLOT(retourGestionCompte()));
    connect(retour,SIGNAL(clicked()),this,SLOT(mise_a_jour_combobox()));
    virtuel = new QRadioButton("Compte Virtuel");
    type = new QLabel("Type :");
    listeType = new QComboBox;
    listeType->setFixedWidth(200);
    listeType->addItem("- Choisir le type du compte -");
    listeType->addItems({"Actif","Passif","Recette","Dépense"});
    hType->addWidget(type);
    hType->addWidget(listeType);
    nom = new QLabel("Nom :");
    nom1 = new QLineEdit();
    hNom->addWidget(nom);
    hNom->addWidget(nom1);
    idPere = new QLabel("ID :");
    listeIdPere = new QComboBox;
    listeIdPere->setFixedWidth(200);
    listeIdPere->addItem("- Choisir l'ID du père -");
    hID->addWidget(idPere);
    hID->addWidget(listeIdPere);
    solde = new QLabel("Solde :");
    solde1 = new QDoubleSpinBox();
    solde1->setMaximum(100000);
    hSolde->addWidget(solde);
    hSolde->addWidget(solde1);
    listeComptes=new QComboBox;
    listeComptes->setFixedWidth(200);
    listeComptes->addItem("- Choisir un compte -");
    connect(this,SIGNAL(sigRapprochement(QString)),parent,SLOT(afficherRapprochement(QString)));

//Widgets pour déplacer
    hID2 = new QHBoxLayout();
    idPere2 = new QLabel("ID :");
    idPere2->hide();
    listeIdPere2 = new QComboBox;
    listeIdPere2->hide();
    listeIdPere2->setFixedWidth(200);
    listeIdPere2->addItem("- Choisir l'ID du père -");
    hID2->addWidget(idPere2);
    hID2->addWidget(listeIdPere2);

    Singleton& s=Singleton::donneInstance();
    chargerComboBox(s.getCompte(s.getId()));
    connect(listeComptes,SIGNAL(currentIndexChanged(int)),this,SLOT(refresh()));

    h->addWidget(arborescence);
    v1->addWidget(listeComptes);
    v2->addWidget(rappro);
    v2->addWidget(sup);
    v2->addLayout(hID2);
    v2->addWidget(deplacer);
    v1->addLayout(v2);
    v1->addWidget(virtuel);
    v1->addLayout(hType);
    v1->addLayout(hNom);
    v1->addLayout(hID);
    v1->addLayout(hSolde);
    v1->addWidget(ajout);
    v1->addWidget(retour);
    h1->addWidget(erreur);
    h->addLayout(v1);
    v->addLayout(h);
    v->addLayout(h1);
}

/**
 * \brief fonction qui permet l'affichage de l'arborescence des comptes
 */
void Gestion_Compte::chargerArbo(){
    Singleton& s = Singleton::donneInstance();
    arborescence->setText(QString::fromStdString(s.getArborescence(0)));
}

/**
 * \brief fonction récursive qui permet le chargement des differents comptes dans la comboBox
 * \param c : le compte abstrait à parcourir et afficher
 */
void Gestion_Compte::chargerComboBox(Compte_Abstrait* c){
    if(c->isVirtuel()){
        Compte_Virtuel* cmpt=dynamic_cast<Compte_Virtuel*>(c);
        QString line=QString::number(cmpt->getId());
        line+=";  " + QString::fromStdString(cmpt->getNom());
        listeComptes->addItem(line);
        listeIdPere->addItem(QString::number(c->getId()));
        listeIdPere2->addItem(QString::number(c->getId()));
        vector<Compte_Abstrait*> vec=cmpt->getListeCompte();
        QVector<Compte_Abstrait*> qvec = QVector<Compte_Abstrait*>(vec.begin(), vec.end());
        for(int i = 0; i < qvec.size(); i++){
            chargerComboBox(qvec[i]);
        }
    }
    else{
        QString line=QString::number(c->getId());
        line+="; " + QString::fromStdString(c->getNom());
        listeComptes->addItem(line);
    }
}

/**
 * \brief slot qui permet l'ajout d'un compte à réception d'un signal associé
 */
void Gestion_Compte::ajouterCompte(){
    try {
        CompteFactory compteManager;
        if(virtuel->isChecked()){
            if(listeIdPere->currentIndex()!=0 && listeType->currentIndex()!=0){
                if(listeType->currentText()=="Actif")
                    compteManager.Fabrication(Type::Actif,(nom1->text()).toStdString(),listeIdPere->currentText().toInt());
                if(listeType->currentText()=="Passif")
                    compteManager.Fabrication(Type::Passif,(nom1->text()).toStdString(),listeIdPere->currentText().toInt());
                if(listeType->currentText()=="Recette")
                    compteManager.Fabrication(Type::Recette,(nom1->text()).toStdString(),listeIdPere->currentText().toInt());
                if(listeType->currentText()=="Dépense")
                    compteManager.Fabrication(Type::Depense,(nom1->text()).toStdString(),listeIdPere->currentText().toInt());
            }
        }
        else{
            if(listeIdPere->currentIndex()!=0 && listeType->currentIndex()!=0){
                if(listeType->currentText()=="Actif")
                    compteManager.Fabrication(Type::Actif,(nom1->text()).toStdString(),listeIdPere->currentText().toInt(),solde1->value());
                if(listeType->currentText()=="Passif")
                    compteManager.Fabrication(Type::Passif,(nom1->text()).toStdString(),listeIdPere->currentText().toInt(),solde1->value());
                if(listeType->currentText()=="Recette")
                    compteManager.Fabrication(Type::Recette,(nom1->text()).toStdString(),listeIdPere->currentText().toInt(),solde1->value());
                if(listeType->currentText()=="Dépense")
                    compteManager.Fabrication(Type::Depense,(nom1->text()).toStdString(),listeIdPere->currentText().toInt(),solde1->value());
            }
        }
    } catch (TresorerieException e) {
        erreur->show();
        erreur->setText(QString::fromStdString(e.getInfo()));
    }
    erreur->show();
    erreur->setText("Ajout réussi");
}

/**
 * \brief slot qui permet la suppression d'un compte à reception d'un signal associé
 */
void Gestion_Compte::supprimerCompte(){
    try {
        Singleton& root=Singleton::donneInstance();
        QString s=listeComptes->currentText();
        int i=0;
        while(s[i]!=';')
            i++;
        int id=(s.mid(0,i)).toInt();
        root.suppression(root.getIdPere(id),id);
        chargerArbo();
        listeComptes->removeItem(listeComptes->findText(s));
        listeComptes->setCurrentIndex(0);
    } catch (TresorerieException e) {
        erreur->show();
        erreur->setText(QString::fromStdString(e.getInfo()));
    }
}


/**
 * \brief slot qui permet de déplacer dans l'arborescence un certain compte à reception d'un signal associé
 */
void Gestion_Compte::deplacerCompte(){
    try {
        if(listeIdPere2->currentIndex()!=0){
            Singleton& root=Singleton::donneInstance();
            int newPere=(listeIdPere2->currentText()).toInt();
            QString s=listeComptes->currentText();
            int i=0;
            while(s[i]!=';')
                i++;
            int id=(s.mid(0,i)).toInt();
            root.deplacer(id,root.getIdPere(id),newPere);
            chargerArbo();
            listeComptes->setCurrentIndex(0);
            listeIdPere2->setCurrentIndex(0);
        }
    } catch (TresorerieException e) {
        erreur->show();
        erreur->setText(QString::fromStdString(e.getInfo()));
    }
}

/**
 * \brief slot qui permet de refresh le widget Gestion_Compte à reception d'un signal associé
 */
void Gestion_Compte::refresh(){
    rappro->show();
    sup->show();
    idPere2->show();
    listeIdPere2->show();
    deplacer->show();
}

/**
 * \brief slot qui permet d'emettre le nom du compte selectionné dans la comboBox à reception d'un signal associé
 */
void Gestion_Compte::envoyerNom()
{
    emit sigRapprochement(listeComptes->currentText());
}

/**
 * \brief slot qui permet de cacher le widget Gestion_Compte à recpetion d'un signal associé
 */
void Gestion_Compte::cacher(){
    sup->hide();
    erreur->hide();
    idPere2->hide();
    listeIdPere2->hide();
    deplacer->hide();
    rappro->hide();
    listeComptes->setCurrentIndex(0);
    listeIdPere->setCurrentIndex(0);
    listeIdPere2->setCurrentIndex(0);
}


/**
 * \brief slot qui permet de mettre à jour la comboBox affichant les comtes à reception d'un signal associé
 */
void Gestion_Compte::mise_a_jour_combobox()
{
    chargerArbo();
    Singleton& s=Singleton::donneInstance();
    listeIdPere->clear();
    listeIdPere->addItem("- Choisir l'ID du père -");
    listeIdPere2->clear();
    listeIdPere2->addItem("- Choisir l'ID du père -");
    listeComptes->clear();
    listeComptes->addItem("- Choisir un compte -");
    chargerComboBox(s.getCompte(s.getId()));
    sup->hide();
    erreur->hide();
    idPere2->hide();
    listeIdPere2->hide();
    deplacer->hide();
    rappro->hide();
    nom1->setText("");
    solde1->setValue(0);
    listeIdPere->setCurrentIndex(0);
    listeType->setCurrentIndex(0);
    listeComptes->setCurrentIndex(0);
}

//------------
//Rapprochement
//--------------

/**
 * \brief Rapprochement::Rapprochement Constructeur du Widget Rapprochement
 * \param parent : le widget parent
 */
Rapprochement::Rapprochement(QWidget* parent): QWidget(parent){
    h = new QHBoxLayout();
    v=new QVBoxLayout(this);
    date=new QDateEdit();
    voir = new QPushButton("Voir l'évolution du compte",nullptr);
    connect(voir,SIGNAL(clicked()),this,SLOT(voirRapprochement()));
    retour = new QPushButton("Retour gestion des comptes",nullptr);
    connect(retour,SIGNAL(clicked()),parent,SLOT(retourRapprochement()));
    connect(retour,SIGNAL(clicked()),this,SLOT(cacher()));
    valider = new QPushButton("Valider le rapprochement",nullptr);
    valider->hide();
    connect(valider,SIGNAL(clicked()),this,SLOT(validerRapprochement()));
    affichage = new QTextEdit();

    h->addWidget(date);
    h->addWidget(voir);
    v->addLayout(h);
    v->addWidget(affichage);
    v->addWidget(valider);
    v->addWidget(retour);

}

/**
 * \brief slot qui permet l'affichage du rapprochement par rapport à une date à reception d'un certain signal associé
 */
void Rapprochement::voirRapprochement(){
    try {
        TransactionManager& tm=TransactionManager::getManager();
        QString d=(date->date()).toString("dd.MM.yyyy");
        Date dat=Date((d.mid(0,2)).toInt(),(d.mid(3,2)).toInt(),(d.mid(6,4)).toInt());
        int i=0;

        if(s != "- Choisir un compte -")
        {
        while(s[i]!=';')
            i++;
        int id=(s.mid(0,i)).toInt();
        tm.voirRapprochement(dat,id);
        affichage->setText(QString::fromStdString(tm.voirRapprochement(dat,id)));
        valider->show();
        } else
        {
            affichage->setText("Vous n'avez pas sélectionné de compte");
        }

    } catch (TresorerieException e) {
        affichage->setText(QString::fromStdString(e.getInfo()));
    }

}

/**
 * \brief slot permettant de valider un rapprochement à reception d'un signal associé
 */
void Rapprochement::validerRapprochement(){
    try {
        TransactionManager& tm=TransactionManager::getManager();
        QString d=(date->date()).toString("dd.MM.yyyy");
        Date dat=Date((d.mid(0,2)).toInt(),(d.mid(3,2)).toInt(),(d.mid(6,4)).toInt());
        int i=0;
        while(s[i]!=';')
            i++;
        int id=(s.mid(0,i)).toInt();
        tm.validerRapprochement(dat,id);
        affichage->setText("Rapprochement effectué");
    } catch (TresorerieException e) {
        affichage->setText(QString::fromStdString(e.getInfo()));
    }
}

/**
 * \brief slot permettant de cacher le widget Rapprochement à réception d'un signal associé
 */
void Rapprochement::cacher(){
    affichage->setText(NULL);
    valider->hide();
    date->setDate(QDate(2000,01,01));
}


//---------------------
// Gestion Transaction
//---------------------

/**
 * \brief Gestion_Transaction::Gestion_Transaction Constructeur du Widget Gestion_Transaction
 * \param parent : le widget parent
 */
Gestion_Transaction::Gestion_Transaction(QWidget* parent) : QWidget(parent)
{
    n = new Nouvelle_Transaction();
    connect(n,SIGNAL(retour(int)),this,SLOT(fin_ajouter(int)));
    n->hide();
    m=new Modification_Transaction();
    connect(m,SIGNAL(retour(int)),this,SLOT(fin_modifier(int)));
    m->hide();
    titre = new QLabel("Mes Transactions", nullptr);
    vueTransac = new QTableWidget(0,7,nullptr);
    vueTransac->setEditTriggers(0);
    vueTransac->setFixedSize(800,600);
    QStringList header("Date");
    header.append("Reference");
    header.append("Titre");
    header.append("Compte");
    header.append("Debit");
    header.append("Credit");
    header.append("Rapproché");
    vueTransac->setHorizontalHeaderLabels(header);

    listeComptes = new QComboBox(nullptr);
    listeComptes->addItem(" -Tous les comptes- ");
    connect(listeComptes,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshBox()));

    ajouter = new QPushButton("Nouvelle Transaction",nullptr);
    connect(ajouter,SIGNAL(clicked()),this,SLOT(nouvelle_Transaction()));
    modifier = new QPushButton("Modifier Transaction", nullptr);
    connect(modifier,SIGNAL(clicked()),this,SLOT(modifier_Transaction()));
    supprimer = new QPushButton("Supprimer Transaction", nullptr);
    connect(supprimer,SIGNAL(clicked()),this,SLOT(supprimer_Transaction()));
    retour = new QPushButton("Retour Accueil", nullptr);
    connect(retour,SIGNAL(clicked()),parent,SLOT(retourTransaction()));
    connect(ajouter,SIGNAL(clicked()),n,SLOT(mise_a_jour_combobox()));
    v1 = new QVBoxLayout;
    v2 = new QVBoxLayout;
    h1 = new QHBoxLayout(this);

    v1->addWidget(titre);
    v1->addWidget(vueTransac);

    v2->addWidget(listeComptes);
    v2->addWidget(ajouter);
    v2->addWidget(modifier);
    v2->addWidget(supprimer);
    v2->addWidget(retour);

    h1->addLayout(v1);
    h1->addLayout(v2);
    h1->addWidget(n);
    h1->addWidget(m);
    refresh();

}

/**
 * \brief slot permettant l'affichage du widget Nouvelle_Transaction et de cacher le widget Gestion_Transaction à réception d'un signal associé
 * \param parent : le widget parent
 */
void Gestion_Transaction::nouvelle_Transaction()
{
    n->show();
    titre->hide();
    vueTransac->hide();
    modifier->hide();
    supprimer->hide();
    retour->hide();
    ajouter->hide();
    listeComptes->hide();
}

/**
 * \brief slot permettant de cacher le widget Ajouter_Transaction et l'affichage du widget Gestion_Transaction à réception d'un signal asscoié
 * \param a : code d'erreur
 */
void Gestion_Transaction::fin_ajouter(int a)
{
    if(a == 1)
    {
        titre->setText("ATTENTION Transaction a échoué, pbm Taille ");
    }
    else if(a == 2)
    {
            titre->setText("ATTENTION Transaction a échoué, pbm d'équilibrage");
    }else if(a == 3)
    {
            titre->setText("ATTENTION La transaction a échoué, la référence existe déjà");
    } else{
        titre->setText("Mes Transactions");
    }
    n->hide();
    m->hide();
    titre->show();
    vueTransac->show();
    modifier->show();
    supprimer->show();
    retour->show();
    ajouter->show();
    listeComptes->show();
    setListeCompteIndex(0);
    refresh();
}

/**
 * \brief fonction permettant de refresh les transactions affichées
 */
void Gestion_Transaction::refresh()
{
    Singleton& singleton = Singleton::donneInstance();
    if (listeComptes->currentIndex() == 0 || listeComptes->currentIndex() == -1){
        //on met à jour la liste des comptes concrets
        if (listeComptes->count()>0){
            listeComptes->setCurrentIndex(0);
            while (listeComptes->count()!=1)
                listeComptes->removeItem(1);
        }
        chargerComboBox(&singleton);

        int nombre = vueTransac->rowCount();
        for(int q = 0; q < nombre; q++)
            vueTransac->hideRow(q);
        TransactionManager& t = TransactionManager::getManager();
        vector<Transaction*>::const_iterator itb = t.getIteratorTransaction();
        vector<Transaction*>::const_iterator ite = t.getEndIteratorTransaction();
        //pour chaque transfert
        for(;itb!=ite;itb++)
        {
            traitementTransaction((**itb));
        }
    }
    else {
        int nombre = vueTransac->rowCount();
        for(int q = 0; q < nombre; q++)
            vueTransac->hideRow(q);

        QString s=listeComptes->currentText();
        int i=0;
        while(s[i]!=';')
            i++;
        int id=(s.mid(0,i)).toInt();

        Compte_Concret* compte = dynamic_cast<Compte_Concret*>(singleton.getCompte(id));
        TransactionManager& t = TransactionManager::getManager();
        auto itb = t.getFilterIterator(*compte);
        //pour chaque transfert où est impliqué le compte séléctionné
        for (;!(itb.isDone());itb.next())
        {
            traitementTransaction(itb.currentItem());
        }
    }
}

/**
 * \brief fonction permettant d'afficher une certaine transaction dans le tableau
 * \param transaction : la transaction à afficher dans le tableau
 */
void Gestion_Transaction::traitementTransaction(const Transaction& transaction){
    //ajout d'une ligne
    vueTransac->insertRow(0);
    vueTransac->setRowHeight(0,100);
    //ajout de la date
    QString date = QString::number(transaction.getDate().getJour()) +"/";
    QString mois = QString::number(transaction.getDate().getMois());
    if (mois.length() != 2)
    {
        date+="0"+mois;
    } else {
        date+= mois;
    }
    date += "/"+ QString::number(transaction.getDate().getAnnee());
    QTableWidgetItem* dateItem = new QTableWidgetItem(date);
    vueTransac->setItem(0,0,dateItem);
    //ajout de Reference
    QString ref = QString::fromStdString(transaction.getRef());
    vueTransac->setItem(0,1,new QTableWidgetItem(ref));
    //ajout du Titre
    QString titre = QString::fromStdString(transaction.getTitre());
    vueTransac->setItem(0,2,new QTableWidgetItem(titre));
    //ajout des Comptes débit et credit
    auto debut_tri = transaction.getTriplet();
    auto fin_tri = transaction.getEndTriplet();
    QString nom;
    QString debit;
    QString credit;
    for(;debut_tri!=fin_tri;debut_tri++)
    {
        nom += QString::fromStdString((*debut_tri)->getNom()) + "\n";
        debit += QString::number((*debut_tri)->getDebit()) + "\n";
        credit += QString::number((*debut_tri)->getCredit()) + "\n";

    }
    nom.remove(nom.length()-1,nom.length());
    debit.remove(debit.length()-1,debit.length());
    credit.remove(credit.length()-1,credit.length());
    vueTransac->setItem(0,3,new QTableWidgetItem(nom));
    vueTransac->setItem(0,4,new QTableWidgetItem(debit));
    vueTransac->setItem(0,5,new QTableWidgetItem(credit));
    //ajout du rapprochement
    bool rapp = transaction.getRapproche();
    if(rapp)
    {
        vueTransac->setItem(0,6,new QTableWidgetItem("Oui"));
    } else {
        vueTransac->setItem(0,6,new QTableWidgetItem("Non"));
    }

}

/**
 * \brief fonction récursive qui permet le chargement des differents comptes dans la comboBox
 * \param c : le compte abstrait à parcourir et afficher
 */

void Gestion_Transaction::chargerComboBox(Compte_Abstrait* c){
    if(c->isVirtuel()){
        Compte_Virtuel* cmpt=dynamic_cast<Compte_Virtuel*>(c);
        vector<Compte_Abstrait*> vec=cmpt->getListeCompte();
        QVector<Compte_Abstrait*> qvec = QVector<Compte_Abstrait*>(vec.begin(), vec.end());
        for(int i = 0; i < qvec.size(); i++){
            chargerComboBox(qvec[i]);
        }
    }
    else{
        QString line=QString::number(c->getId());
        line+="; " + QString::fromStdString(c->getNom());
        listeComptes->addItem(line);
    }
}

/**
 * \brief slot permettant la suppression d'une transaction séléctionnée à reception d'un signal associé
 */
void Gestion_Transaction::supprimer_Transaction(){
    try{
        TransactionManager& t = TransactionManager::getManager();
        if (vueTransac->currentRow() >=0){
            t.supprimerTransaction(vueTransac->item(vueTransac->currentRow(),1)->text().toStdString());
            refresh();
        }
    }
    catch (TresorerieException e) {titre->setText(QString::fromStdString(e.getInfo()));}
}

/**
 * \brief slot permettant de cacher le widget Modifier_Transaction et l'affichage du widget Gestion_Transaction à réception d'un signal asscoié
 * \param a : code d'erreur
 */
void Gestion_Transaction::fin_modifier(int a)
{
    if(a == 1)
    {
        titre->setText("ATTENTION la modification de transaction a échouée, pbm Taille : la modification a ete abandonnee ");
    }
    else if(a == 2)
    {
            titre->setText("ATTENTION la modification de transaction a échoué, pbm d'équilibrage : la modification a ete abandonnee ");
    } else{
        titre->setText("Mes Transactions");
    }
    n->hide();
    m->hide();
    titre->show();
    vueTransac->show();
    modifier->show();
    supprimer->show();
    retour->show();
    ajouter->show();
    listeComptes->show();
    setListeCompteIndex(0);
}

/**
 * \brief slot permettant l'affichage du widget Modifier_Transaction et de cacher le widget Gestion_Transaction à réception d'un signal associé
 * \param parent : le widget parent
 */
void Gestion_Transaction::modifier_Transaction()
{
    m->chargerComboBoxRef();
    m->show();
    n->hide();
    titre->hide();
    vueTransac->hide();
    modifier->hide();
    supprimer->hide();
    retour->hide();
    ajouter->hide();
    listeComptes->hide();
}

/*/////////////////////
//Nouvelle_Transaction
/////////////////////*/


/**
 * \brief Nouvelle_Transaction::Nouvelle_Transaction Constructeur du widget Nouvelle_Transaction
 * \param parent : le widget parent
 */
Nouvelle_Transaction::Nouvelle_Transaction(QWidget * parent): QWidget(parent){
    listeComptes=new QComboBox;
    listeComptes->setFixedWidth(200);
    listeComptes->addItem("- Choisir un compte -");
    Singleton& s=Singleton::donneInstance();
    chargerComboBox(s.getCompte(s.getId()));
    debit_label = new QLabel("Debit");
    credit_label = new QLabel("Credit");
    debit = new QDoubleSpinBox();
    credit = new QDoubleSpinBox();
    debit->setMaximum(100000);
    debit->setMinimum(-100000);
    credit->setMaximum(100000);
    credit->setMinimum(-100000);
    recap = new QTextEdit("");
    recap->setFixedSize(200,200);
    recap->setReadOnly(false);
    titre_label = new QLabel("Titre");
    ref_label = new QLabel("Référence");
    ref = new QLineEdit("Reference");
    titre = new QLineEdit("Titre");
    date_Transac = new QDateEdit();
    annuler = new QPushButton("Annuler");
    ajouter_triplet = new QPushButton("Ajouter Triplet");
    ajouter_Transa = new QPushButton("Valider Transaction");

    triplet = new QHBoxLayout;
    reca_ajoutTriplet = new QHBoxLayout;
    titre_ref_date = new QHBoxLayout;
    valider_annuler = new QHBoxLayout;
    v1 =new QVBoxLayout(this);

    triplet->addWidget(listeComptes);
    triplet->addWidget(debit_label);
    triplet->addWidget(debit);
    triplet->addWidget(credit_label);
    triplet->addWidget(credit);
    reca_ajoutTriplet->addWidget(recap);
    reca_ajoutTriplet->addWidget(ajouter_triplet);
    titre_ref_date->addWidget(titre_label);
    titre_ref_date->addWidget(titre);
    titre_ref_date->addWidget(ref_label);
    titre_ref_date->addWidget(ref);
    titre_ref_date->addWidget(date_Transac);
    valider_annuler->addWidget(ajouter_Transa);
    valider_annuler->addWidget(annuler);
    v1->addLayout(triplet);
    v1->addLayout(reca_ajoutTriplet);
    v1->addLayout(titre_ref_date);
    v1->addLayout(valider_annuler);

    connect(ajouter_triplet, SIGNAL(clicked()), this, SLOT(ajouter_Triplet()));
    connect(ajouter_Transa, SIGNAL(clicked()), this, SLOT(ajouter_Transaction()));
    connect(annuler, SIGNAL(clicked()), this, SLOT(annulation()));
}


/**
 * \brief fonction recursive qui permet le chargement des comptes concrets dans la comboBox
 * \param c : compte abstrait à parcourir (si Compte Virtuel, on itere sur chaque compte ; si Compte Concret, on affiche)
 */
void Nouvelle_Transaction::chargerComboBox(Compte_Abstrait* c){
    if(c->isVirtuel()){
        Compte_Virtuel* cmpt=dynamic_cast<Compte_Virtuel*>(c);
        vector<Compte_Abstrait*> vec=cmpt->getListeCompte();
        QVector<Compte_Abstrait*> qvec = QVector<Compte_Abstrait*>(vec.begin(), vec.end());
        for(int i = 0; i < qvec.size(); i++){
            chargerComboBox(qvec[i]);
        }
    }
    else{
        QString line=QString::number(c->getId());
        line+="; " + QString::fromStdString(c->getNom());
        listeComptes->addItem(line);
    }
}

/**
 * \brief slot qui permet l'ajout de triplet à la liste des triplet associés la nouvelle transaction à réception d'un signal associé
 */
void Nouvelle_Transaction::ajouter_Triplet(){
    try {
        Singleton& root=Singleton::donneInstance();
        if(listeComptes->currentIndex() != 0)
        {
            QString s=listeComptes->currentText();
            int i=0;
            while(s[i]!=';')
                i++;
            int id=(s.mid(0,i)).toInt();
            Compte_Concret* cpt = dynamic_cast<Compte_Concret*>(root.getCompte(id));
            float cred = credit->value();
            float deb = debit->value();
            Triplet* triplet = new Triplet(cpt, deb, cred);
            t.push_back(triplet);
            listeComptes->removeItem(listeComptes->findText(s));
            listeComptes->setCurrentIndex(0);
            QString text = QString::fromStdString(cpt ->getNom()) +
                    " Débit " + QString::number(deb) +
                    " Crédit " + QString::number(cred) + "\n";
            recap->insertPlainText(text);
            credit->setValue(0);
            debit->setValue(0);
        }
    } catch (TresorerieException e) {
        recap->setText(QString::fromStdString(e.getInfo()));
    }
}


/**
 * \brief slot qui permet l'ajout d'une transaction à la réception d'un signal associé
 */
void Nouvelle_Transaction::ajouter_Transaction()
{
    int reussite = 0;
    try{
        QString titre_transac = titre->text();
        QString ref_transac = ref->text();
        QString d=(date_Transac->date()).toString("dd.MM.yyyy");
        Date date=Date((d.mid(0,2)).toInt(),(d.mid(3,2)).toInt(),(d.mid(6,4)).toInt());
        TransactionManager& tm = TransactionManager::getManager();
        tm.creerTransaction(date, ref_transac.toStdString(), titre_transac.toStdString(), t);
    }catch(TresorerieException e){
        string info = e.getInfo();
        if(info == "Erreur de taille de transaction")
        {
            reussite = 1;
        }
        if(info == "Erreur d'équilibrage de la transaction")
        {
            reussite = 2;
        }
        if(info == "La référence existe déjà")
        {
            reussite = 3;
        }
    }
    //remettre tout à 0
    //vider triplet
    while(t.size() != 0)
    {
        t.pop_back();
    }
    //reinitialiser combo box
    emit retour(reussite);
}


/**
 * \brief slot qui permet l'annulation de l'ajout de la transaction en cours à réception d'un signal associé
 */
void Nouvelle_Transaction::annulation()
{
    while(t.size() != 0)
    {
        t.pop_back();
    }
    //reinitialiser combo box
    emit retour(0);
}

/**
 * \brief slot qui permet la mise à jour des comptes de la comboBox à réception d'un signal associé
 */
void Nouvelle_Transaction::mise_a_jour_combobox()
{
    listeComptes->clear();
    listeComptes->addItem("- Choisir un compte -");
    Singleton& s=Singleton::donneInstance();
    chargerComboBox(s.getCompte(s.getId()));
    credit->setValue(0);
    debit->setValue(0);
    titre->setText("");
    ref->setText("");
    recap->clear();
    date_Transac->setDate(QDate(2000,01,01));
}

//-----------
//Modification transaction
//--------------


/**
 * \brief Modification_Transaction::Modification_Transaction Constructeur du widget Modification_Transaction
 * \param parent : le widget parent
 */
Modification_Transaction::Modification_Transaction(QWidget * parent): QWidget(parent){
    listeReference=new QComboBox;
    listeReference->setFixedWidth(200);
    ref = new QLineEdit("");
    ref->setReadOnly(true);

    listeComptes=new QComboBox;
    listeComptes->setFixedWidth(200);
    listeComptes->addItem("- Choisir un compte -");

    debit_label = new QLabel("Debit");
    credit_label = new QLabel("Credit");
    debit = new QDoubleSpinBox();
    credit = new QDoubleSpinBox();
    debit->setMaximum(100000);
    debit->setMinimum(-100000);
    credit->setMaximum(100000);
    credit->setMinimum(-100000);

    recap = new QTextEdit("");
    recap->setFixedSize(200,200);
    recap->setReadOnly(true);
    titre_label = new QLabel("Titre");
    ref_label = new QLabel("Référence");
    titre = new QLineEdit("Titre");
    date_Transac = new QDateEdit();
    annuler = new QPushButton("Annuler");
    ajouter_triplet_amont = new QPushButton("Ajouter Triplet");
    modifier_triplet_amont = new QPushButton("Modifier Triplet");
    ajouter_triplet_aval = new QPushButton("Ajouter Triplet");
    modifier_triplet_aval = new QPushButton("Modifier Triplet");
    modifier_Transa = new QPushButton("Valider Modification");
    choisir_ref = new QPushButton("Choisir");

    triplet = new QHBoxLayout;
    reca_ajoutTriplet = new QHBoxLayout;
    titre_ref_date = new QHBoxLayout;
    valider_annuler = new QHBoxLayout;
    v1 =new QVBoxLayout(this);

    triplet->addWidget(ref_label);
    triplet->addWidget(listeReference);
    triplet->addWidget(ref);
    triplet->addWidget(listeComptes);
    triplet->addWidget(debit_label);
    triplet->addWidget(debit);
    triplet->addWidget(credit_label);
    triplet->addWidget(credit);
    reca_ajoutTriplet->addWidget(recap);
    reca_ajoutTriplet->addWidget(modifier_triplet_amont);
    reca_ajoutTriplet->addWidget(modifier_triplet_aval);
    reca_ajoutTriplet->addWidget(ajouter_triplet_amont);
    reca_ajoutTriplet->addWidget(ajouter_triplet_aval);
    titre_ref_date->addWidget(titre_label);
    titre_ref_date->addWidget(titre);
    titre_ref_date->addWidget(date_Transac);
    valider_annuler->addWidget(modifier_Transa);
    valider_annuler->addWidget(choisir_ref);
    valider_annuler->addWidget(annuler);
    v1->addLayout(triplet);
    v1->addLayout(reca_ajoutTriplet);
    v1->addLayout(titre_ref_date);
    v1->addLayout(valider_annuler);

    listeComptes->hide();
    debit->hide();
    credit->hide();
    debit_label->hide();
    credit_label->hide();
    ajouter_triplet_amont->hide();
    modifier_triplet_amont->hide();
    ajouter_triplet_aval->hide();
    modifier_triplet_aval->hide();
    recap->hide();
    titre->hide();
    date_Transac->hide();
    titre_label->hide();
    ref->hide();
    modifier_Transa->hide();


    connect(modifier_triplet_amont, SIGNAL(clicked()), this, SLOT(chargerComboBoxComptesRef()));
    connect(ajouter_triplet_amont, SIGNAL(clicked()), this, SLOT(chargerComboBoxComptesNonRef()));
    connect(ajouter_triplet_aval, SIGNAL(clicked()), this, SLOT(ajouter_Triplet()));
    connect(modifier_triplet_aval, SIGNAL(clicked()), this, SLOT(modifier_Triplet()));
    connect(modifier_Transa, SIGNAL(clicked()), this, SLOT(modifier_Transaction()));
    connect(annuler, SIGNAL(clicked()), this, SLOT(annulation()));
    connect(choisir_ref,SIGNAL(clicked()),this,SLOT(afficher_Transaction()));

}

/**
 * \brief fonction qui permet de charger dans la comboBox associée aux references la liste des réferences des transactions non rapprochées
 */
void Modification_Transaction::chargerComboBoxRef(){
    listeReference->clear();
    listeReference->addItem(" -Choisir une reference- ");

    TransactionManager& t = TransactionManager::getManager();
    vector<Transaction*>::const_iterator itb= t.getIteratorTransaction();
    const vector<Transaction*>::const_iterator ite=t.getEndIteratorTransaction();
    for (;itb!=ite;itb++) {
        if ((*itb)->getRapproche() == 0){
           QString line = QString::fromStdString((*itb)->getRef());
           listeReference->addItem(line);
        }
    }
}

/**
 * \brief slot qui permet de charger dans la comboBox, associée aux comptes, les comptes ayant participé à la transaction à réception d'un signal associé
 */
void Modification_Transaction::chargerComboBoxComptesRef(){
        QString refe=listeReference->currentText();
        TransactionManager& tm = TransactionManager::getManager();
        Transaction* t = tm.getTransaction(refe.toStdString());

        listeReference->hide();
        ref->show();
        ref->setText(refe);
        credit->show();
        debit->show();
        credit_label->show();
        debit_label->show();
        ajouter_triplet_aval->hide();
        ajouter_triplet_amont->hide();
        modifier_triplet_amont->hide();
        modifier_triplet_aval->show();
        listeComptes->show();
        listeComptes->clear();
        listeComptes->addItem("- Choisir un compte -");

        auto itb= t->getTriplet();
        auto ite=t->getEndTriplet();
        for (;itb!=ite;itb++) {
            QString line=QString::number((*itb)->getCompte()->getId());
            line+="; " + QString::fromStdString((*itb)->getNom());
            listeComptes->addItem(line);
        }
}

/**
 * \brief slot qui permet de charger dans la comboBox, associée aux comptes, les comptes n'ayant pas participé à la transaction, à réception d'un signal associé
 */
void Modification_Transaction::chargerComboBoxComptesNonRef(){
        QString refe=listeReference->currentText();
        TransactionManager& tm = TransactionManager::getManager();
        Transaction* t = tm.getTransaction(refe.toStdString());
        Singleton& root=Singleton::donneInstance();

        listeReference->hide();
        ref->show();
        ref->setText(refe);
        credit->show();
        debit->show();
        credit_label->show();
        debit_label->show();
        ajouter_triplet_amont->hide();
        modifier_triplet_amont->hide();
        modifier_triplet_aval->hide();
        ajouter_triplet_aval->show();
        listeComptes->show();
        listeComptes->clear();
        listeComptes->addItem("- Choisir un compte -");

        ajouter_RecursivementComptesNonRef(&root,t);
}

/**
 * \brief fonction récursive qui permet de charger dans la comboBox, associée aux comptes, les comptes concrets n'ayant pas participé à la transaction
 * \param c : compte abstrait étudié qui : s'il est virtuel est parcouru ; s'il est concret et qu'il n'est pas asscocié à la transaction, est ajouté
 * \param t : transaction en cours de modification
 */
void Modification_Transaction::ajouter_RecursivementComptesNonRef(Compte_Abstrait* c,Transaction* t){
    Compte_Concret* concret =dynamic_cast<Compte_Concret*>(c);
    if (concret != nullptr){
        if (t->isInside(*concret)==false){
            QString line=QString::number(concret->getId());
            line+="; " + QString::fromStdString(concret->getNom());
            listeComptes->addItem(line);
        }

    }
    else {
        Compte_Virtuel* virtuel = dynamic_cast<Compte_Virtuel*>(c);
        auto itb = virtuel->getFirstCompte();
        auto ite = virtuel->getLastCompte();
        for (;itb!=ite;itb++) {
            ajouter_RecursivementComptesNonRef((*itb),t);
        }
    }
}

/**
 * \brief slot qui permet d'afficher les informations de la transaction sélectionnée à réception d'un signal associé
 */
void Modification_Transaction::afficher_Transaction(){
    QString refe=listeReference->currentText();
    TransactionManager& tm = TransactionManager::getManager();
    Transaction* t = tm.getTransaction(refe.toStdString());

    listeReference->hide();
    choisir_ref->hide();
    ref->show();
    ref->setText(refe);
    modifier_Transa->show();
    recap->show();
    recap->clear();
    ajouter_triplet_aval->hide();
    ajouter_triplet_amont->show();
    modifier_triplet_amont->show();
    modifier_triplet_aval->hide();
    titre->show();
    titre->setText(QString::fromStdString(t->getTitre()));
    QDate* d = new QDate(t->getDate().getAnnee(),t->getDate().getMois(),t->getDate().getJour());
    date_Transac->setDate(*d);
    date_Transac->show();
    titre_label->show();

    auto itb= t->getTriplet();
    auto ite=t->getEndTriplet();
    for (;itb!=ite;itb++) {
        Triplet* recop = new Triplet((*itb)->getCompte(),(*itb)->getDebit(),(*itb)->getCredit());
        triple.push_back(recop);
    }
    afficher_Triplet();
}

/**
 * \brief slot qui permet d'afficher les triplets initialement impliqués dans la transaction en cours de modification à réception d'un signal associé
 */
void Modification_Transaction::afficher_Triplet(){
        auto itb= triple.begin();
        auto ite=triple.end();
        recap->clear();
        for (;itb!=ite;itb++) {
            QString text = QString::fromStdString((*itb)->getCompte()->getNom()) +
                    " Débit " + QString::number((*itb)->getDebit()) +
                    " Crédit " + QString::number((*itb)->getCredit()) + "\n";
            recap->insertPlainText(text);
        }
}

/**
 * \brief slot qui permet la modification d'un triplet initialement présent dans la transaction en cours de modification à réception d'un signal associé
 */
void Modification_Transaction::modifier_Triplet(){
    Singleton& root=Singleton::donneInstance();
    if(listeComptes->currentIndex() != 0)
    {
        QString s=listeComptes->currentText();
        int i=0;
        while(s[i]!=';')
            i++;
        int id=(s.mid(0,i)).toInt();
        Compte_Concret* cpt = dynamic_cast<Compte_Concret*>(root.getCompte(id));
        float cred = credit->value();
        float deb = debit->value();
        Triplet* triplet = new Triplet(cpt, deb, cred);
        auto itb = triple.begin();
        auto ite = triple.end();
        for (;itb!=ite;itb++){
            if ((*itb)->getCompte()->getId() == id )
                triple.erase(itb);
        }
        triple.push_back(triplet);
        routine_triplet();
    }
}

/**
 * \brief slot qui permet l'ajout d'un compte non impliqué initialement dans la transaction en cours de modification, associé à un certain débit et crédit, à réception d'un signal associé
 */
void Modification_Transaction::ajouter_Triplet(){
    if(listeComptes->currentIndex() != 0)
    {
        Singleton& root=Singleton::donneInstance();
            QString s=listeComptes->currentText();
            int i=0;
            while(s[i]!=';')
                i++;
            int id=(s.mid(0,i)).toInt();
            Compte_Concret* cpt = dynamic_cast<Compte_Concret*>(root.getCompte(id));
            float cred = credit->value();
            float deb = debit->value();
            Triplet* triplet = new Triplet(cpt, deb, cred);
            triple.push_back(triplet);
            routine_triplet();
    }
}

/**
 * \brief fonction qui permet la factorisation d'instructions quant à l'ajout ou la modification de triplet
 */
void Modification_Transaction::routine_triplet(){

    credit->setValue(0);
    debit->setValue(0);
    credit->hide();

    debit->hide();
    credit_label->hide();
    debit_label->hide();
    ajouter_triplet_aval->hide();
    modifier_triplet_aval->hide();
    listeComptes->hide();
    ajouter_triplet_amont->show();
    modifier_triplet_amont->show();

    afficher_Triplet();
}

/**
 * \brief slot qui permet la modification d'une transaction donnée à réception d'un signal associé
 */
void Modification_Transaction::modifier_Transaction()
{
    int reussite = 0;
    try{
        QString ref_transac=listeReference->currentText();
        QString titre_transac = titre->text();
        QString d=(date_Transac->date()).toString("dd.MM.yyyy");
        Date date=Date((d.mid(0,2)).toInt(),(d.mid(3,2)).toInt(),(d.mid(6,4)).toInt());
        TransactionManager& tm = TransactionManager::getManager();
        bool test = tm.modifierTransaction(ref_transac.toStdString(),date, titre_transac.toStdString(), triple);
        if (test == true) cout<<"Modif. réussie"<<endl;
    }catch(TresorerieException e){
        string info = e.getInfo();
        if(info == "Erreur de taille de transaction")
        {
            reussite = 1;
        }
        if(info == "Erreur d'équilibrage de la transaction")
        {
            reussite = 2;
        }
        if (info == "La transaction n'a pas pu etre supprimee\n"){
             reussite = 3;
        }
      }
    emit retour(reussite);
}

/**
 * \brief slot qui permet l'annulation de la modification d'une transaction
 */
void Modification_Transaction::annulation()
{
    routine_fin();
    emit retour(0);
}

/**
 * \brief fonction qui permet la factorisation d'instructions quant à l'annulation ou la validation de la modification de la transaction
 */
void Modification_Transaction::routine_fin(){

    //remettre tout à 0
    //vider triplet
    while(triple.size() != 0)
    {
        triple.pop_back();
    }
    //reinitialiser combo box
    listeComptes->clear();
    listeComptes->addItem("- Choisir un compte -");

    credit->setValue(0);
    debit->setValue(0);
    titre->setText("");
    ref->setText("");
    date_Transac->setDate(QDate(2000,01,01));
    recap->clear();

    listeComptes->hide();
    debit->hide();
    credit->hide();
    debit_label->hide();
    credit_label->hide();
    ajouter_triplet_amont->hide();
    modifier_triplet_amont->hide();
    ajouter_triplet_aval->hide();
    modifier_triplet_aval->hide();
    recap->hide();
    titre->hide();
    date_Transac->hide();
    titre_label->hide();
    modifier_Transa->hide();
    ref->hide();

    choisir_ref->show();
    listeReference->show();


}

/*\\\\\\\\\\\\\\\\\\\
//FENETRE PRINCIPAL
///////////////////*/


/**
 * \brief Fenetre_principal::Fenetre_principal Constructeur du widget Fenetre_principal
 * \param parent : le widget parent
 */
Fenetre_principal::Fenetre_principal(QWidget *parent) : QWidget(parent){
    setFixedSize(1000,1000);
    setWindowTitle("Ma Trésorerie");
    a = new Accueil(this);
    b = new Bilan(this);
    resu = new Resultat(this);
    g=new Gestion_Compte(this);
    t=new Gestion_Transaction(this);
    rele = new Releve(this);
    rappro = new Rapprochement(this);
    a->show();
    a->chargerArbo();
    b->hide();
    resu->hide();
    g->hide();
    t->hide();
    rele->hide();
    rappro->hide();
    connect(this,SIGNAL(cacherBouton()),g,SLOT(cacher()));
}


/**
 * \brief slot qui permet d'afficher le widget Bilan et de cacher le widget Fenetre_principal
 */
void Fenetre_principal::afficherBilan(){
    a->hide();
    b->show();
}

/**
 * \brief slot qui permet d'afficher le widget Fenetre_principal et de cacher le widget Bilan
 */
void Fenetre_principal::retourBilan(){
    b->hide();
    a->chargerArbo();
    a->show();
}

/**
 * \brief slot qui permet d'afficher le widget Resultat et de cacher le widget Fenetre_principal
 */
void Fenetre_principal::afficherResultat(){
    a->hide();
    resu->mise_a_jour();
    resu->show();
}

/**
 * \brief slot qui permet d'afficher le widget Fenetre_principal et de cacher le widget Resultat
 */
void Fenetre_principal::retourResultat(){
    resu->hide();
    a->chargerArbo();
    a->show();
}

/**
 * \brief slot qui permet d'afficher le widget Gestion_Compte et de cacher le widget Fenetre_principal
 */
void Fenetre_principal::afficherGestionCompte(){
    a->hide();
    g->chargerArbo();
    g->show();

}

/**
 * \brief slot qui permet d'afficher le widget Fenetre_principal et de cacher le widget Gestion_Compte
 */
void Fenetre_principal::retourGestionCompte(){
    g->hide();
    a->show();
    a->chargerArbo();
    emit cacherBouton();
}

/**
 * \brief slot qui permet d'afficher le widget Gestion_Transaction et de cacher le widget Fenetre_principal
 */
void Fenetre_principal::afficherTransaction()
{
    a->hide();
    t->refresh();
    t->show();
}

/**
 * \brief slot qui permet d'afficher le widget Fenetre_principal et de cacher le widget Gestion_Transaction
 */
void Fenetre_principal::retourTransaction(){
    t->hide();
    t->setListeCompteIndex(0);
    a->chargerArbo();
    a->show();
}

/**
 * \brief slot qui permet d'afficher le widget Releve et de cacher le widget Fenetre_principal
 */
void Fenetre_principal::afficherReleve(){
    a->hide();
    rele->show();
}

/**
 * \brief slot qui permet d'afficher le widget Fenetre_principal et de cacher le widget Releve
 */
void Fenetre_principal::retourReleve(){
    rele->hide();
    a->chargerArbo();
    a->show();
}

/**
 * \brief slot qui permet d'afficher le widget Rapprochement et de cacher le widget Gestion_Compte
 */
void Fenetre_principal::afficherRapprochement(QString i){
    g->hide();
    g->cacher();
    rappro->setS(i);
    rappro->show();
}

/**
 * \brief slot qui permet d'afficher le widget Gestion_Compte et de cacher le widget Rapprochement
 */
void Fenetre_principal::retourRapprochement(){
    rappro->hide();
    g->show();
}

////////////////
//Widget Global
////////////////


/**
 * \brief Widget_global::Widget_global Constructeur du widget Widget_global
 * \param parent : le widget parent
 */
Widget_global::Widget_global(QWidget* parent): QWidget(parent)
{
    setWindowTitle("Démarrage");
    depart = new Widget_demarrage(this);
    depart->show();
    fen = nullptr;
    Singleton& s =Singleton::donneInstance();
    TransactionManager& tm=TransactionManager::getManager();
    sing = &s;
    tran = &tm;
};

/**
 * \brief slot qui permet de lancer l'application avec des fichiers contenant les comptes et les transactions qui existent deja
 * \param a : contient les fichiers choisis
 */
void Widget_global::lancer_app(QStringList a)
{
    sing->load(a[0]);
    tran->load(a[1]);
    fen = new Fenetre_principal();
    fen->show();
    depart->hide();
    this->hide();
}


/**
 * \brief slot qui permet de lancer l'application en créant de nouveaux fichiers
 * \param a : contient les noms et l'emplacement des nouveaux fichiers choisis
 */
void Widget_global::lancer_par_defaut(QStringList a)
{
    sing->setFilename(a[0]);
    tran->setFilename(a[1]);
    fen = new Fenetre_principal();
    fen->show();
    depart->hide();
    this->hide();
}
