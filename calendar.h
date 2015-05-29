#ifndef CALENDAR_h
#define CALENDAR_h
#include<string>
#include<iostream>
#include "timing.h"
using namespace std;
using namespace TIME;


class CalendarException{
public:
	CalendarException(const string& message):info(message){}
	string getInfo() const { return info; }
private:
	string info;
};

class Tache {
    friend class TacheManager;
private:
	string identificateur;
	string titre;
	Duree  duree;
	Date   disponibilite;
	Date   echeance;
	//on interdit la création de tache à l'utilisateur mais il faut mettre en ami tache manager
	Tache(const string& id, const string& t, const Duree& dur, const Date& dispo, const Date& deadline):
            identificateur(id),titre(t),duree(dur),disponibilite(dispo),echeance(deadline){}

    Tache(const Tache& t){
        identificateur=t.identificateur;
        titre=         t.titre;
        duree=         t.duree;
        disponibilite= t.disponibilite;
        echeance=      t.echeance;
    }
    Tache & operator=(const Tache&);

public:
	string getId() const { return identificateur; }
	string getTitre() const { return titre; }
	Duree getDuree() const { return duree; }
	Date getDateDisponibilite() const {  return disponibilite; }
	Date getDateEcheance() const {  return echeance; }
};

class TacheManager {
private:
	Tache** taches;
	unsigned int nb;
	unsigned int nbMax;
	void addItem(Tache* t);
	Tache* trouverTache(const string& id) const;
	string file;
	static TacheManager* instance; //pour le singleton
	TacheManager();
	TacheManager(const TacheManager& um);
	TacheManager& operator=(const TacheManager& um);
	~TacheManager();
public:
	Tache& ajouterTache(const string& id, const string& t, const Duree& dur, const Date& dispo, const Date& deadline);
	Tache& getTache(const string& id);
	const Tache& getTache(const string& code) const;
	void load(const string& f);
	void save(const string& f);
	static TacheManager& getInstance();
	static void LibererInstance();

	class DisponibiliteFilterIterator{
        friend class TacheManager;
    private:
        Tache** currentTache;
        unsigned int nbRemain;
        Date dispo;
        DisponibiliteFilterIterator(Tache** taches,unsigned int i, const Date& d):dispo(d),nbRemain(i),currentTache(taches){
            while((nbRemain > 0) && (dispo < (*currentTache)->getDateDisponibilite())){
                  nbRemain--;currentTache++;
            }
        }
    public:
            DisponibiliteFilterIterator():nbRemain(0),currentTache(0),dispo(0){}
            bool isDone() const {return nbRemain == 0;}
            void next(){
                if(isDone())
                    throw CalendarException("erreur, iterator is done !\n");
                do{
                    nbRemain--;
                    currentTache++;
                }while((nbRemain > 0) && (dispo < (*currentTache)->getDateDisponibilite()));
            }
            Tache & current() const{
                if(isDone())
                    throw CalendarException("erreur, iterator is done !\n");
                return ** currentTache;}
	};

	DisponibiliteFilterIterator getDisponibiliteFilterIterator(const Date& d){
        return DisponibiliteFilterIterator(taches,nb,d);
	}
};

class Programmation {
	const Tache* tache;
	Date date;
	Horaire horaire;
public:
	Programmation(const Tache& t, const Date& d, const Horaire& h):tache(&t), date(d), horaire(h){}
	const Tache& getTache() const { return *tache; }
	Date getDate() const { return date; }
	Horaire getHoraire() const { return horaire; }
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

ostream& operator<<(ostream& f, const Tache& t);
ostream& operator<<(ostream& f, const Programmation& p);

#endif

