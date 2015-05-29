#ifndef CALENDAR_h
#define CALENDAR_h
#include<string>
#include<iostream>
#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>
#include <QTextStream>
#include "timing.h"
using namespace std;
using namespace TIME;


class CalendarException{
public:
    CalendarException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};

//TACHE: doit etre ABSTRAITE!! -> methode à rendre VIRTUAL PURE

class Tache {
protected:
    friend class TacheManager;
    QString identificateur;
    QString titre;
	Date disponibilite;
    Tache(const QString& id, const QString& t, const Date& dispo):
            identificateur(id),titre(t),disponibilite(dispo){}
    Tache(const Tache& t):
        identificateur(t.identificateur),titre(t.titre),disponibilite(t.disponibilite){}
    Tache& operator=(const Tache& t);
    virtual ~Tache(){}
public:
    const QString& getId() const { return identificateur; }
    const QString& getTitre() const { return titre; }
    const Date& getDateDisponibilite() const {  return disponibilite; }
    virtual const Date& getEcheance() const =0;
    void setId(const QString& str);
    void setTitre(const QString& str){titre=str;}
    void setDateDisponibilite(const Date& disp){disponibilite=disp;}
    //const string& getProjet(); à définir? Dans l'id?
};

class TacheUnitaire:public Tache
{
protected:
    Date echeance;
    Duree duree;
    bool preemptive;
    TacheUnitaire(const QString& id, const QString& t, const Date& dispo, const Date& deadline, const Duree& dur):
        Tache(id,t,dispo),echeance(deadline),duree(dur){}
    TacheUnitaire(const TacheUnitaire& t):
        Tache(t.getId(),t.getTitre(),t.getDateDisponibilite()),duree(t.duree),echeance(t.echeance){}
    TacheUnitaire& operator=(const TacheUnitaire& t);
    virtual ~TacheUnitaire(){}
public:
    virtual const Date& getEcheance() const { return echeance;}
    const Duree& getDuree() const { return duree; }
    const bool isPreemptive()const {return preemptive;}
    void setDuree(const Duree& d){duree=d;}
    void setEcheance(const Date& e){echeance=e;}
    void setPreemptive(){preemptive=true;}
    void setNonPreemptive(){preemptive=false;}
};

class TacheComposite:public Tache//HERITAGE à faire avec PROJET!!
{
protected:
    Tache** taches;
    unsigned int nb;
    unsigned int nbMax;
    TacheComposite(const QString& id, const QString& t, const Date& dispo):
                   Tache(id,t,dispo){}
    TacheComposite(const TacheComposite& um);
    TacheComposite& operator=(const TacheComposite& um);
    void addTache(Tache* t);
    Tache* trouverTache(const QString& id) const;
    virtual ~TacheComposite();
public:
    class IteratorSTL
    {
        Tache** current;
        IteratorSTL(Tache** taches): current(taches){}
        friend class TacheComposite;
    public:
        IteratorSTL():current(0){}
        Tache& operator*() const{return **current;}
        bool operator!=(const IteratorSTL& i) const{
            return i.current != this->current;
        }
        IteratorSTL& operator++(int){
            ++current;
            return *this;
        }
    };
    IteratorSTL begin(){
        return IteratorSTL(taches);
    }
    IteratorSTL end(){
        return IteratorSTL (taches+nb);
    }
    virtual const Date& getEcheance()const;//borne sup des taches du tableau
    Tache& ajouterTache(const TacheUnitaire& tu);//on ne peut pas jouter une tache sans l'avoir créée avant-> pb?
    Tache& ajouterTache(const TacheComposite& tc);
    Tache& getTache(const QString& id);
    const Tache& getTache(const QString& code) const;
};

//PROJET

class Projet: public TacheComposite
{
protected:
    Projet(const QString& id, const QString& t, const Date& dispo);
    Projet(const Projet& um);
    Projet& operator=(const Projet& um);
    virtual ~Projet();
};

//PROGRAMMATION:
/*
class Programmation {
    const Tache* tache;
    Date date;
    Horaire horaire;
public:
    Programmation(const Tache& t, const Date& d, const Horaire& h):tache(&t), date(d), horaire(h){}
    const Tache& getTache() const { return *tache; }
    Date getDate() const { return date; }
    Horaire getHoraire() const { return horaire; }
}; A VOIR TD08+TD09*

//EVENEMENTS: cf evenements.h

//MANAGERS:

class Manager
{

};

class TacheManager {
private:
	Tache** taches;
	unsigned int nb;
	unsigned int nbMax;
    string file;
    static class HandlerTacheManager
    {
        TacheManager* instance;
       public:
        HandlerTacheManager();
        ~HandlerTacheManager();
    };
    TacheManager();
    TacheManager(const TacheManager& um);
    TacheManager& operator=(const TacheManager& um);
    void addItem(Tache* t);
    Tache* trouverTache(const string& id) const;
    ~TacheManager();
public:
    class Iterator
    {
        Tache ** currentTache;
        unsigned int nbRemain;
        Iterator(Tache** taches, unsigned int nb): currentTache(taches), nbRemaint(nb){}
        friend class TacheManager;
     public:
        Iterator():nbRemain(0),currentTache(0){}
        bool isDone() const {return nbRemain==0;}
        virtual void next(){
            if(isDone()){
                throw CalendarException("Erreur, iterator is Done");
            }
            nbRemain--;
            currentTache++;
        }
        Tache& current() const{
            if(isDone()){
                throw CalendarException("Erreur, iterator is Done");
            }
            return ** currentTache;
        }
    };
    Iterator getIterator(){
        return Iterator(taches,nb);
    }
    class IteratorSTL
    {
        Tache** current;
        IteratorSTL(Tache** taches): currentTache(taches){}
        friend class TacheManager;
    public:
        IteratorSTL():currentTache(0){}
        Tache& operator*() const{return **current;}
        bool operator!=(const IteratorSTL& i) const{
            return i.current != this->current;
        }
        IteratorSTL& operator++(int){
            ++current;
            return *this;
        }
    };
    IteratorSTL begin(){
        return iterator(taches);
    }
    IteratorSTL end(){
        return iterator (taches+nb);
    }
    class DisponibiliteFilterIterator:public Iterator{
        Date dispo;
        DisponibilitefilterIterator(Tache** taches, unsigned int nb, Date d): currentTache(taches), nbRemain(nb), dispo(d){
            while(nbRemain>0 && dispo<(*currentTache)->getDateDisponibilite()){
                nbRemain --;
                currentTache++;
            }
        }
        friend class TacheManager;//à vérifier
     public:
        DisponibiliteFilterIterator():nbRemain(0),currentTache(0), dispo(d){}
        virtual void next(){
            if(isDone()){
                throw CalendarException("Erreur, iterator is Done");
            }
            do{
                nbRemain--;
                currentTache++;
            }while(nbRemain>0 && dispo<(*currentTache)->getDateDisponibilite());
        }
    };
    DisponibiliteFilterIterator getDisponibiliteFilterIterator(const Date& d){
        return DisponibiliteFilterIterator(taches,nb,d);
    }
    Tache& ajouterTache(const QString& id, const string& t, const Duree& dur, const Date& dispo, const Date& deadline);
    Tache& getTache(const QString& id);
    const Tache& getTache(const QString& code) const;
    //bool isTacheExistante(const QString& id);
    void load(const QString& f);
    void save(const QString& f);
    static TacheManager& getInstance();
    static void LibererInstance ();
};

class ProgrammationManager {
private:
    Programmation** programmations;
    unsigned int nb;
    unsigned int nbMax;
    void addItem(Programmation* t);
    Programmation* trouverProgrammation(const Tache& t) const;
public:
    ProgrammationManager();
    ~ProgrammationManager();
    ProgrammationManager(const ProgrammationManager& um);
    ProgrammationManager& operator=(const ProgrammationManager& um);
    void ajouterProgrammation(const Tache& t, const Date& d, const Horaire& h);
};



*/



QTextStream& operator<<(QTextStream& f, const Tache& t);
//QTextStream& operator<<(QTextStream& f, const Programmation& p);

#endif
