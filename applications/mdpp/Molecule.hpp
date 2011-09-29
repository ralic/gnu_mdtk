#ifndef mdpp_Molecule_hpp
#define mdpp_Molecule_hpp

#include <iostream>
#include <mdtk/SimLoop.hpp>
#include <algorithm>
#include <vector>
#include "AtomGroup.hpp"

namespace mdepp
{
  using namespace mdtk;

class Molecule : public AtomGroup
{
  enum {ECOUNT = 4};
  enum {C = 0};
  enum {H = 1};
  enum {Ar = 2};
  enum {Cu = 3};
  Float Rc_[ECOUNT][ECOUNT];
  size_t e2i(const mdtk::Atom &atom) const
  {
    using namespace mdtk;
    switch (atom.ID)
    {
      case H_EL : return H; break;
      case C_EL : return C; break;
      case Ar_EL : return Ar; break;
      case Cu_EL : return Cu; break;
      default : throw Exception("ClassicMolecule::e2i() : unknown element");
    };  
  }  
public:
  void initParams()
  {
    using namespace mdtk;
    handledElements.insert(H_EL);
    handledElements.insert(C_EL);
    handledElements.insert(Ar_EL);
    handledElements.insert(Cu_EL);

    for(size_t e1 = 0; e1 < ECOUNT; e1++)
      for(size_t e2 = 0; e2 < ECOUNT; e2++)
        Rc_[e1][e2] = 0.001*Ao;

    Rc_[C][C] = 2.0*Ao;
    Rc_[H][H] = 1.7*Ao;
    Rc_[C][H] = 1.8*Ao;
      Rc_[H][C] = Rc_[C][H];

    Rc_[Cu][Cu] = 3.0*Ao;


    Rc_[Cu][H] = 2.75*Ao;
      Rc_[H][Cu] = Rc_[Cu][H];

    Rc_[Cu][C] = 2.75*Ao;
      Rc_[C][Cu] = Rc_[Cu][C];
      
//    Rc_[Ar][Ar] = 0.001*Ao;
//      Rc_[Ar][ H] = Rc_[ H][Ar] = Rc_[ C][Ar] = Rc_[Ar][ C] = Rc_[Ar][Ar];
  }

  Float Rc(const mdtk::Atom &atom1,const mdtk::Atom &atom2) const
  {
    return Rc_[e2i(atom1)][e2i(atom2)];
  }  
  std::set<mdtk::ElementID> handledElements;
  bool
  isHandled(const mdtk::Atom& atom) const
  {
    return (handledElements.find(atom.ID) != handledElements.end());
  }

  Molecule();
  ~Molecule();

  Molecule(const Molecule &c);
  Molecule& operator =(const Molecule &c);

  virtual void get(std::istream& is);
  virtual void put(std::ostream& os) const;

  void buildFromAtom(const mdtk::Atom& a, const AtomGroup& ag);
};

}

#endif