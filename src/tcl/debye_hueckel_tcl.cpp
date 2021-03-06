/*
  Copyright (C) 2010,2011,2012,2013,2014,2015,2016 The ESPResSo project
  Copyright (C) 2002,2003,2004,2005,2006,2007,2008,2009,2010 
    Max-Planck-Institute for Polymer Research, Theory Group
  
  This file is part of ESPResSo.
  
  ESPResSo is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  ESPResSo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/
/** \file debye_hueckel_tcl.cpp
 *
 *  Implementation of \ref debye_hueckel_tcl.hpp
 */
#include "debye_hueckel_tcl.hpp"
#include "debye_hueckel.hpp"

#ifdef ELECTROSTATICS

int tclprint_to_result_dh(Tcl_Interp *interp)
{
  char buffer[TCL_DOUBLE_SPACE];
  Tcl_PrintDouble(interp, dh_params.kappa, buffer);
  Tcl_AppendResult(interp, "dh ", buffer, " ",(char *) NULL);
  Tcl_PrintDouble(interp, dh_params.r_cut, buffer);
  Tcl_AppendResult(interp, buffer, (char *) NULL);
#ifdef COULOMB_DEBYE_HUECKEL
  Tcl_PrintDouble(interp, dh_params.eps_int, buffer);
  Tcl_AppendResult(interp, " ", buffer, " ", (char *) NULL);
  Tcl_PrintDouble(interp, dh_params.eps_ext, buffer);
  Tcl_AppendResult(interp, buffer, " ", (char *) NULL);
  Tcl_PrintDouble(interp, dh_params.r0, buffer);
  Tcl_AppendResult(interp, buffer, " ", (char *) NULL);
  Tcl_PrintDouble(interp, dh_params.r1, buffer);
  Tcl_AppendResult(interp, buffer, " ", (char *) NULL);
  Tcl_PrintDouble(interp, dh_params.alpha, buffer);
  Tcl_AppendResult(interp, buffer, (char *) NULL);
#endif
  return TCL_OK;
}

int tclcommand_inter_coulomb_parse_dh(Tcl_Interp * interp, int argc, char ** argv)
{
  double kappa, r_cut;
  int i;
#ifdef COULOMB_DEBYE_HUECKEL
  double eps_int, eps_ext, r0, r1, alpha;
#endif

#ifdef COULOMB_DEBYE_HUECKEL
  if(argc < 6) {
    Tcl_AppendResult(interp, "Not enough parameters: inter coulomb dh <kappa> <r_cut> <eps_int> <r0> <r1> <alpha>", (char *) NULL);
    return TCL_ERROR;
  }
#else
  if(argc < 2) {
    Tcl_AppendResult(interp, "Not enough parameters: inter coulomb dh <kappa> <r_cut>", (char *) NULL);
    return TCL_ERROR;
  }
#endif
  
  coulomb.method = COULOMB_DH;

  if(! ARG0_IS_D(kappa))
    return TCL_ERROR;
  if(! ARG1_IS_D(r_cut))
    return TCL_ERROR;
#ifdef COULOMB_DEBYE_HUECKEL
  if(!ARG_IS_D(2, eps_int) || !ARG_IS_D(3, eps_ext) || !ARG_IS_D(4,r0) || !ARG_IS_D(5,r1) || !ARG_IS_D(6,alpha))
    return TCL_ERROR;  

  i = dh_set_params_cdh(kappa, r_cut, eps_int, eps_ext, r0, r1, alpha);
#else
  i = dh_set_params(kappa, r_cut);
#endif

  if ( i < 0) {
    switch (i) {
    case -1:
      Tcl_AppendResult(interp, "dh kappa must be positiv.",(char *) NULL);
      break;
    case -2:
      Tcl_AppendResult(interp, "dh r_cut must be positiv.",(char *) NULL);
      break;
    default:
      Tcl_AppendResult(interp, "unspecified error",(char *) NULL);
    }
    
    return TCL_ERROR;
  }

  return TCL_OK;
}

#endif
