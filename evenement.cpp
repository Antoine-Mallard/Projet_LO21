#include "evenement.h"

std::ostream& TIME::operator<<(std::ostream& out, const Evt1j& e){
    e.afficher(out);
    return(out);
}

void TIME::Agenda::ajouterEvent(TIME::Evt1j *e){
    if(nb == nbmax){
        Evt1j ** newtab = new Evt1j*[10+nbmax];
        Evt1j ** oldtab = events;
        for(unsigned int i=0;i<nb;++i)
            newtab[i]=oldtab[i];
        nbmax = nbmax+10;
        events = newtab;
        delete[] oldtab;
    }
    events[nb++] = e;
}

 TIME::Agenda& TIME::Agenda::operator<<(TIME::Evt1j& e){
     this->ajouterEvent(&e);
     return *this;
 }

 void TIME::Agenda::afficher(std::ostream &f)const{
     for(unsigned int i=0; i<nb; ++i){
         f<<"event " << i << " :\n";
         f<<*events[i];
     }
 }
