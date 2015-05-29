#ifndef EVENEMENT_H
#define EVENEMENT_H
#include<string>
#include<iostream>
#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>
#include <QTextStream>
#include "timing.h"
using namespace std;

namespace TIME{
    //EVENEMENTS
    class Evt
    {
        Date date;
        QString sujet;
    public:
        Evt(const Date& d, const QString& s):
            date(d),sujet(s){}
        const Date& getDate() const { return date; }
        const QString& getDescription() const { return sujet; }
        virtual void afficher(std::ostream f=std::cout)=0;
        virtual ~Evt();
    };

    class EvtPj:public Evt
    {
        Date dateFin;
    public:
        EvtPj(const Date& d, const QString& s, const Date& f):
            Evt(d,s),dateFin(f){}
        const Date& getFin() const{return fin;}
        virtual void afficher(std::ostream& f=std::cout) const;
        virtual ~EvtPj();
    };

    class Evt1j:public Evt
    {
    public:
        Evt1j(const Date& d, const QString& s):Evt(d,s){}
        virtual void afficher(std::ostream& f= std::cout) const;
        virtual ~Evt1j();
    };

    class Evt1jDur:public Evt1j
    {
        Horaire debut;
        Duree duree;
    public:
        Evt1jDur(const Date& d, const QString& s, const Horaire& h, const Duree& dur):
            Evt1j(d,s), horaire(h), duree(dur){}
        const Horaire& getHoraire() {return horaire;}
        const Duree& getDuree() {return duree;}
        virtual void afficher(std::ostream& f= std::cout) const;
        virtual ~Evt1jDur();
    };

    class Rdv: public Evt1jDur
    {
        QString lieu;
        QString personnes;
     public:
        Rdv(const Date& d, const QString& s, const Horaire& h, const Duree& dur, const QString& l, const QString& p):
            Evt1jDur(d,s, h,dur), lieu(l),personnes(p){}
        Rdv(const Rdv& rdv):
            Evt1jDur(rdv), lieu(l),personnes(p){}
        const std::string& getLieu(){return lieu;}
        const std::string& getPersonnes(){return personnes;}
        virtual void afficher(std::ostream& f= std::cout) const;
        virtual ~Rdv();
    };

    QTextStream& operator<<(QTextStream& f, const Evt1j& e);

    //AGENDA
    class Agenda
    {
        Evt1j** evenements;
        unsigned int nb;
        unsigned int nbMax;
        Agenda(const Agenda& a);
        Agenda& operator=(const Agenda &a);
        void ajouterEvent(const Evt1j& e);
    public:
        Agenda():evenements(0),nb(0),nbMax(0){}
        class Iterator
        {
            Evt1j** current;
            Iterator(Evt** init):current(init){};
            friend class Agenda;
        public:
            Iterator operator++()
            {
                ++current;
                return *this;
            }
            Iterator operator--()
            {
                --current;
                return *this;
            }
            bool operator==(const Iterator& i) const
            {
                return this->current==i.current;
            }
            bool operator!=(const Iterator& i) const
            {
                return this->current!=i.current;
            }
            Evt& operator*()
            {
                return **current;
            }
        };
        Iterator begin() const{return Iterator(evenements);}
        Iterator end() const{return Iterator(evenements+nb);}
        Agenda& operator<<(Evt1j& e);
        void afficher(std::ostream& out, const Evt1j& e)const;
        ~Agenda();
    };
}
#endif // EVENEMENT_H
