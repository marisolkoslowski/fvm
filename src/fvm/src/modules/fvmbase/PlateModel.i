%{
#include "PlateModel.h"
#include "AMG.h"
%}


using namespace std;
using namespace boost;


%include "FloatVarDict.i"

template <class T>
struct PlateBC : public FloatVarDict<T>
{
  string bcType;
}; 

template <class T>
struct PlateVC : public FloatVarDict<T>
{
}; 

template <class T>
struct PlateModelOptions : public FloatVarDict<T>
{
  bool printNormalizedResiduals;
  bool transient;
  bool incompressible;
  double deformationTolerance;
  int timeDiscretizationOrder;
  double scf;
  LinearSolver *deformationLinearSolver;
}; 

//%template(Vector3) Vector<ATYPE_STR,3>;

%template(PlateBCA) PlateBC< ATYPE_STR >;
%template(PlateBCList) std::vector<PlateBC< ATYPE_STR >* >;
%template(PlateBCsMap) std::map<int,PlateBC< ATYPE_STR >* >;

%template(PlateVCA) PlateVC< ATYPE_STR >;
%template(PlateVCList) std::vector<PlateVC< ATYPE_STR >* >;
%template(PlateVCsMap) std::map<int,PlateVC< ATYPE_STR >* >;

%template(PlateModelOptionsA) PlateModelOptions< ATYPE_STR >;

%include "PlateModel.h"


%template(PlateModelA) PlateModel< ATYPE_STR >;
