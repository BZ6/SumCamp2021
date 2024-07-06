/* FILE NAME  : main.cpp
 * PROGRAMMER : BZ6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Unit BTR.
 */

#include "../anim/anim.h"

/* BTR unit class */
class unit_btr : public zagl::unit
{
public:
  zagl::primitives *Prs;

  VOID Response( zagl::anim *Ani ) override
  {
    Prs = new zagl::primitives;
    Ani->render::shader_manager::Add(const_cast<CHAR *>("DEFAULT"));
    Ani->render::primitives_manager::PrimsLoad(Prs, const_cast<CHAR *>("BIN/MODELS/btr.g3dm"));
  }

  VOID Render( zagl::anim *Ani ) override
  {
    Prs->PrimsDraw(mth::matr<FLT>::Scale(mth::vec3<FLT>(0.1)));
  }
}; /* End of 'unit_btr' class */

//static zagl::anim::unit_register<unit_btr> _("BTR");

/* END OF 'unit_btr.cpp' FILE */
