/* FILE NAME  : main.cpp
 * PROGRAMMER : BZ6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Unit control.
 */

#include "../anim/anim.h"

/* My unit class */
class unit_control : public zagl::unit
{
public:
  VOID Response( zagl::anim *Ani ) override
  {
    if (Ani->Keys['W'] || Ani->Keys['A'] || Ani->Keys['S'] || Ani->Keys['D'])
          Ani->Camera.Move(((Ani->Camera.Dir * ((Ani->Keys['W'] - Ani->Keys['S'])) +
                         Ani->Camera.Right * ((Ani->Keys['D'] - Ani->Keys['A']))) * 30 * Ani->GlobalDeltaTime).GetVecInPlaneXZ());
    if (Ani->Keys[VK_SPACE] || Ani->Keys[VK_LSHIFT])
          Ani->Camera.Move(mth::vec3<FLT>(0,
                                       ((Ani->Camera.Up * (Ani->Keys[VK_SPACE] - Ani->Keys[VK_LSHIFT])) * 30 * Ani->GlobalDeltaTime)[1],
                                       0));

    if (Ani->Keys[VK_RBUTTON])
    {
      FLT Hp, Wp = Hp = Ani->Camera.ProjDist;

      if (Ani->Camera.FrameW > Ani->Camera.FrameH)
        Wp *= static_cast<FLT>(Ani->Camera.FrameW) / Ani->Camera.FrameH;
      else
        Hp *= static_cast<FLT>(Ani->Camera.FrameH) / Ani->Camera.FrameW;

      FLT Dist = !(Ani->Camera.At - Ani->Camera.Loc);

      FLT sx = -Ani->MouseDX * Wp / Ani->Camera.FrameW * Dist / Ani->Camera.ProjDist;
      FLT sy = Ani->MouseDY * Hp / Ani->Camera.FrameH * Dist / Ani->Camera.ProjDist;

      Ani->Camera.Move(Ani->Camera.Right * sx + Ani->Camera.Up * sy);
    }

    //SetCursorPos(Ani->W / 2, Ani->H / 2);
    if (Ani->Keys[VK_LBUTTON] || Ani->Keys[VK_LEFT] || Ani->Keys[VK_RIGHT])
      Ani->Camera.Rotate(mth::vec3<FLT>(0, 1, 0), (100 * -Ani->Keys[VK_LBUTTON] * Ani->MouseDX +
                                            100 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])) * Ani->GlobalDeltaTime);

    if (Ani->Keys[VK_LBUTTON] || Ani->Keys[VK_UP] || Ani->Keys[VK_DOWN])
      Ani->Camera.Rotate(Ani->Camera.Right, (100 * -Ani->Keys[VK_LBUTTON] * Ani->MouseDY +
                                              100 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime);
  }
}; /* End of 'unit_control' class */

//static zagl::anim::unit_register<unit_control> _("Control");

/* END OF 'unit_control.cpp' FILE */
