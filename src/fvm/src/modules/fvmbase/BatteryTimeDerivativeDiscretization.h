// This file os part of FVM
// Copyright (c) 2012 FVM Authors
// See LICENSE file for terms.


#ifndef _BATTERYTIMEDERIVATIVEDISCRETIZATION_H_
#define _BATTERYTIMEDERIVATIVEDISCRETIZATION_H_

#include "CRMatrix.h"
#include "Field.h"
#include "MultiField.h"
#include "MultiFieldMatrix.h"
#include "Mesh.h"
#include "Discretization.h"
#include "StorageSite.h"

template<class X, class Diag, class OffDiag>
class BatteryTimeDerivativeDiscretization : public Discretization
{
public:
  typedef typename NumTypeTraits<X>::T_Scalar T_Scalar;
  
  typedef CRMatrix<Diag,OffDiag,X> CCMatrix;
  typedef typename CCMatrix::DiagArray DiagArray;
  typedef typename CCMatrix::PairWiseAssembler CCAssembler;

  typedef Array<X> XArray;
  typedef Array<T_Scalar> TArray;
  typedef Array<int> IntArray;

  BatteryTimeDerivativeDiscretization(const MeshList& meshes,
                               const GeomFields& geomFields,
                               Field& varField,
                               Field& varN1Field,
                               Field& varN2Field,
                               const T_Scalar dT) :
      Discretization(meshes),
      _geomFields(geomFields),
      _varField(varField),
      _varN1Field(varN1Field),
      _varN2Field(varN2Field),
      _dT(dT)
      {}
  
  void discretize(const Mesh& mesh, MultiFieldMatrix& mfmatrix,
                  MultiField& xField, MultiField& rField)
  {

    const StorageSite& cells = mesh.getCells();

    const TArray& cellVolume =
      dynamic_cast<const TArray&>(_geomFields.volume[cells]);
    
    const MultiField::ArrayIndex cVarIndex(&_varField,&cells);
    CCMatrix& matrix =
      dynamic_cast<CCMatrix&>(mfmatrix.getMatrix(cVarIndex,cVarIndex));

    DiagArray& diag = matrix.getDiag();

    const XArray& x = dynamic_cast<const XArray&>(_varField[cells]);
    const XArray& xN1 = dynamic_cast<const XArray&>(_varN1Field[cells]);
    
    XArray& rCell = dynamic_cast<XArray&>(rField[cVarIndex]);
   
    const int nCells = cells.getSelfCount();
   
    if (_varN2Field.hasArray(cells))
    {
      // Dont worry about second order for now
      /*
        // second order
        const XArray& xN2 = dynamic_cast<const XArray&>(_varN2Field[cells]);

        T_Scalar onePointFive(1.5);
        T_Scalar two(2.0);
        T_Scalar pointFive(0.5);
        if (_geomFields.volumeN1.hasArray(cells))
	{
	    const TArray& cellVolumeN1 = 
	      dynamic_cast<const TArray&>(_geomFields.volumeN1[cells]);
	    const TArray& cellVolumeN2 = 
              dynamic_cast<const TArray&>(_geomFields.volumeN2[cells]);
            for(int c=0; c<nCells; c++)
	    {
                const T_Scalar rhoVbydT = density[c]*cellVolume[c]/_dT;
                const T_Scalar rhobydT = density[c]/_dT;
		const T_Scalar term1 = onePointFive*cellVolume[c];
                const T_Scalar term2 = two*cellVolumeN1[c];
                const T_Scalar term3 = pointFive*cellVolumeN2[c];
                rCell[c] -= rhobydT*(term1*x[c]- term2*xN1[c]
                                      + term3*xN2[c]);
                diag[c] -= rhoVbydT*onePointFive;
	    }
	}
	else
	{
            for(int c=0; c<nCells; c++)
            {
                const T_Scalar rhoVbydT = density[c]*cellVolume[c]/_dT;
                rCell[c] -= rhoVbydT*(onePointFive*x[c]- two*xN1[c]
                                      + pointFive*xN2[c]);
                diag[c] -= rhoVbydT*onePointFive;
	    }
	    }*/
    }
    else
    {
        if (_geomFields.volumeN1.hasArray(cells))
	{
	  //Moving Mesh
	  /*
	    const TArray& cellVolumeN1 =
	      dynamic_cast<const TArray&>(_geomFields.volumeN1[cells]);
	    for(int c=0; c<nCells; c++)
            {	    
	        const T_Scalar rhoVbydT = density[c]*cellVolume[c]/_dT;
		const T_Scalar rhobydT = density[c]/_dT;
	        rCell[c] -= rhobydT*(cellVolume[c]*x[c] 
                                     - cellVolumeN1[c]*xN1[c]);
		
	        diag[c] -= rhoVbydT;
		
		}*/
	}
        else if (_geomFields.ibTypeN1.hasArray(cells))
	{ 
	  //IBM
	  /*

	    const IntArray& ibTypeN1 =
	      dynamic_cast<const IntArray&>(_geomFields.ibTypeN1[cells]);
	    const IntArray& ibType =
	      dynamic_cast<const IntArray&>(_geomFields.ibType[cells]);
	    for(int c=0; c<nCells; c++)
            {	    
	        const T_Scalar rhoVbydT = density[c]*cellVolume[c]/_dT;

                if (ibTypeN1[c] == Mesh::IBTYPE_FLUID &&
                    (ibType[c] == Mesh::IBTYPE_FLUID))
                  rCell[c] -= rhoVbydT*(x[c]  - xN1[c]);
                else if (ibType[c] == Mesh::IBTYPE_FLUID)
                  rCell[c] -= rhoVbydT*(x[c] );
	        diag[c] -= rhoVbydT;
		}*/
	}
        else
	{
	  // only apply unsteady terms for species equation, not potential
            for(int c=0; c<nCells; c++)
            {
                const T_Scalar rhoVbydT = 1.0*cellVolume[c]/_dT;
                (rCell[c])[1] -= rhoVbydT*((x[c])[1]- (xN1[c])[1]);
		(diag[c])(1,1) -= rhoVbydT;
	
	    }
	   
	}
    }
  
  }

  

private:
  const GeomFields& _geomFields;
  const Field& _varField;
  const Field& _varN1Field;
  const Field& _varN2Field;
  const T_Scalar _dT;
};

#endif
