// Gmsh - Copyright (C) 1997-2008 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to <gmsh@geuz.org>.

#ifndef _GMSH_LINEAR_SYSTEM_H_
#define _GMSH_LINEAR_SYSTEM_H_

// A class that encapsulates a linear system solver interface :
// building a sparse matrix, solving a linear system

class gmshLinearSystem {
public :
  gmshLinearSystem (){}
  virtual bool isAllocated () const = 0;
  virtual void allocate (int nbRows) = 0;
  virtual ~gmshLinearSystem () {}
  virtual void  addToMatrix    (int _row, int _col, double val) = 0;
  virtual double getFromMatrix (int _row, int _col) const = 0;
  virtual void  addToRightHandSide    (int _row, double val) = 0;
  virtual double getFromRightHandSide (int _row) const = 0;
  virtual double getFromSolution (int _row) const = 0;
  virtual void zeroMatrix () = 0;
  virtual void zeroRightHandSide () = 0;
  virtual int systemSolve () = 0;
};
#endif

