#include "evenement.h"

QTextStream& operator<<(QTextStream& out, const TIME::Evt& e)
{
    e.afficher(out);
    return out;
}

TIME::Evt::~Evt(){}

void TIME::EvtPj::afficher(QTextStream &f) const
{
    f<<"-------------Evenement plusieurs jours-------------\n"
    <<"Date début: "<<getDate()
    <<"Date fin: "<<getFin()
    <<"Sujet: "<<getDescription()
    <<"\n";
    return f;
}

TIME::EvtPj::~EvtPj(){}

virtual void Evt1j::afficher(std::ostream& f= std::cout) const
{
    f<<"-------------Evenement 1 jour-------------\n"
    <<"Date début: "<<getDate()
    <<"Sujet: "<<getDescription()
    <<"\n";
    return f;
}

virtual Evt1j::~Evt1j(){}

virtual void Evt1jDur::afficher(std::ostream& f= std::cout) const {
    f<<"-------------Evenement 1 jour-------------\n"
    <<"Date début: "<<getDate()
    <<"Durée: "<<getDuree()
    <<"Sujet: "<<getDescription()
    <<"\n";
    return f;
}

virtual Evt1jDur::~Evt1jDur(){}

virtual void afficher(std::ostream& f= std::cout) const;
virtual ~Rdv();

