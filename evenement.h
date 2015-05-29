#ifndef EVENEMENT_H
#define EVENEMENT_H
#include <iostream>
#include <string>
#include "timing.h"

namespace TIME{
    class Evt1j {
    private:
        Date date;
        std::string sujet;
    public:
        Evt1j(const Date& d, const std::string& s):date(d),sujet(s){std::cout<<"Construction d'un objet de la classe Evt1j\n";}
        virtual ~Evt1j(){std::cout<<"Destruction d'un objet de la classe Evt1j\n";}
        const std::string& getDescription() const { return sujet; }
        const Date& getDate() const { return date; }
        virtual void afficher(std::ostream& f= std::cout) const {
        f<<"***** Evt ********"<<"\n"<<"Date="<<date<<" sujet="<<sujet<<"\n";
    }
    };
    class Evt1jDur : public Evt1j{
    private:
        Horaire debut;
        Duree duree;
    public:
        Evt1jDur(const Date& d, const std::string& s, const Horaire& h, const Duree& dur):Evt1j(d,s),debut(h),duree(dur){std::cout<<"Construction d'un objet de la classe Evt1jDur\n";}
        virtual ~Evt1jDur(){std::cout<<"Destruction d'un objet de la classe Evt1jDur\n";}
        const Horaire& getHoraire()const{return debut;}
        const Duree& getDuree()const{return duree;}
        virtual void afficher(std::ostream& f=std::cout)const{
            Evt1j::afficher(f);
            f<<"horaire="<<debut<<" Duree="<<duree<<"\n";
        }
    };
    class Rdv : public Evt1jDur{
    private:
        std::string lieu;
        std::string personne;
    public:
        Rdv(const Date& d, const std::string& s, const Horaire& h, const Duree& dur, const std::string& l, const std::string& p):Evt1jDur(d,s,h,dur),lieu(l),personne(p){std::cout<<"Construction d'un objet de la classe Rdv\n";}
        virtual ~Rdv(){std::cout<<"Destruction d'un objet de la classe Rdv\n";}
        const std::string& getLieu()const{return lieu;}
        const std::string& getPersonne()const{return personne;}
        virtual void afficher(std::ostream& f=std::cout)const{
            Evt1jDur::afficher(f);
            f<<"lieu="<<lieu<<" personne(s)="<<personne<<"\n";
        }
    };

    std::ostream& operator<<(std::ostream& out, const Evt1j& e);

    class Agenda{
    private:
        Evt1j ** events;
        unsigned int nb, nbmax;

        //interdire la duplication
        Agenda(const Agenda& a);
        Agenda& operator=(const Agenda& a);
        void ajouterEvent(Evt1j *e);
    public:
        Agenda():events(0),nb(0),nbmax(0){}
        void afficher(std::ostream &f=std::cout)const;
        Agenda& operator<<(Evt1j& e);
    };
}



#endif // EVENEMENT

