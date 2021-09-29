#include "Compte.h"

int main(void)
{
    try{
        CompteFactory compteManager;
                compteManager.Fabrication(Type::Recette,"Recette 1",4,500);
                compteManager.Fabrication(Type::Depense,"Depense 1",4,100);
                compteManager.Fabrication(Type::Actif,"Actif",4);
                compteManager.Fabrication(Type::Recette,"Recette",4);
                compteManager.Fabrication(Type::Actif,"Actif 1",7,500);
                compteManager.Fabrication(Type::Actif,"Actif 3",7,300);
                compteManager.Fabrication(Type::Actif,"Actif 2",4);
                compteManager.Fabrication(Type::Passif,"Passif",4);
                compteManager.Fabrication(Type::Passif,"Passif 1",12,800);
                compteManager.Fabrication(Type::Passif,"Passif 2",12,250);
        Singleton& root=Singleton::donneInstance();
        cout<<root.getArborescence(0)<<"\n";

       /* //Test save
        ofstream fout(root.getFilename());
        root.save(fout);
        fout.close();
        root.libereInstance();
        Singleton& root=Singleton::donneInstance();
        root.load(root.getFilename());

        cout<<root.getArborescence(0);
        root.libereInstance();*/
    }catch(TresorerieException e){cout<<e.getInfo();}

    try{
         Singleton& root=Singleton::donneInstance();
        Compte_Concret* actif1 = dynamic_cast<Compte_Concret*>(root.getCompte("Actif 1"));
        Compte_Concret* actif3 = dynamic_cast<Compte_Concret*>(root.getCompte("Actif 3"));
        Compte_Concret* passif1 = dynamic_cast<Compte_Concret*>(root.getCompte("Passif 1"));
        Compte_Concret* depense1 = dynamic_cast<Compte_Concret*>(root.getCompte("Passif 2"));


        Triplet t1=Triplet(actif1,250,0);
        Triplet t2=Triplet(actif3,0,250);
        Triplet t4=Triplet(passif1,0,250);
        Triplet t5=Triplet(depense1,250,0);

        vector<Triplet*> listeTriplet;

        listeTriplet.push_back(&t1);
        listeTriplet.push_back(&t4);

        vector<Triplet*> liste2;
        liste2.push_back(&t4);
        liste2.push_back(&t5);

        TransactionManager& t=TransactionManager::getManager();
       // cout<<t.getTransaction("AB")<<endl;
        Date d= Date(12,05,2020);
        cout<<d<<endl;

        // Test suppression
       t.creerTransaction(d,"AB","LS",listeTriplet);
/*        cout<<"Avant suppression :\n"<<compte->getNom()<<" "<<compte->getSolde()<<endl;
        cout<<compte1->getNom()<<" "<<compte1->getSolde()<<endl;
        cout<<compte2->getNom()<<" "<<compte2->getSolde()<<endl;
        t.supprimerTransaction("AB");
        cout<<"Après suppression :\n"<<compte->getNom()<<" "<<compte->getSolde()<<endl;
        cout<<compte1->getNom()<<" "<<compte1->getSolde()<<endl;
        cout<<compte2->getNom()<<" "<<compte2->getSolde()<<endl;
        cout<<t.getTransaction("AB")<<endl;*/

        t.creerTransaction(d,"BB","LT",listeTriplet);
        t.creerTransaction(d,"CB","LU",listeTriplet);
        t.creerTransaction(d,"DB","LJ",liste2);
        t.creerTransaction(d,"EB","LT",liste2);
        t.creerTransaction(d,"FB","LU",liste2);

      /*  TransactionManager::FilterIterator it=t.getFilterIterator(*actif3);
        TransactionManager::FilterIterator it2=t.getFilterIterator(*passif1);
        cout<<"Avant rapprochement, Actif 3 :\n";
        for(;!it.isDone();it.next())
        {
            cout<<it.currentItem().getRef()<<endl;
        };
        cout<<"Avant rapprochement, Actif 1 :\n";
        for(;!it2.isDone();it2.next())
        {
            cout<<it2.currentItem().getRef()<<endl;
        };

       // t.rapprochement(d,0);
 /*       t.cloture("Test","TestDep","TestRef","TestRes",d);
        cout<<root.getArborescence(0)<<"\n";
        cout << root.getCompte(1)->getSolde()<<" Excedent";
        cout << root.getCompte(0)->getSolde()<<" Resultat";*/

        //test bilan
       t.save();


    }catch(TresorerieException e){cout<<e.getInfo()<<endl;}
     Singleton& root=Singleton::donneInstance();
     ofstream fout(root.getFilename());
     root.save(fout,0);
}
