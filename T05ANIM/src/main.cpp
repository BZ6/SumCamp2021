/* FILE NAME  : main.cpp
 * PROGRAMMER : BZ6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Entry point.
 */

#include "anim/anim.h"

 /* My unit class */
class unit_control : public zagl::unit
{
public:
  VOID Response(zagl::anim *Ani) override
  {
    if (Ani->KeysClick['P'])
      Ani->IsPause = !Ani->IsPause;
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

      Ani->Camera.Move((Ani->Camera.Right * sx + Ani->Camera.Up * sy) * 5);
    }

    //SetCursorPos(Ani->W / 2, Ani->H / 2);
    if (Ani->Keys[VK_LBUTTON] || Ani->Keys[VK_LEFT] || Ani->Keys[VK_RIGHT])
      Ani->Camera.Rotate(mth::vec3<FLT>(0, 1, 0), (20 * -Ani->Keys[VK_LBUTTON] * Ani->MouseDX +
                                                   100 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])) * Ani->GlobalDeltaTime);

    if (Ani->Keys[VK_LBUTTON] || Ani->Keys[VK_UP] || Ani->Keys[VK_DOWN])
      Ani->Camera.Rotate(Ani->Camera.Right, (20 * -Ani->Keys[VK_LBUTTON] * Ani->MouseDY +
                                             100 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime);
  }
}; /* End of 'unit_control' class */

/* BTR unit class */
class unit_btr : public zagl::unit
{
public:
  zagl::primitives *Prs;
  BOOL IsFirst;

  unit_btr( VOID ) : IsFirst(TRUE)
  {
    Prs = new zagl::primitives;
  }

  VOID Response( zagl::anim *Ani ) override
  {
    if (IsFirst)
    {
      Ani->render::shader_manager::Add(const_cast<CHAR *>("DEFAULT"));
      Ani->render::primitives_manager::PrimsLoad(Prs, const_cast<CHAR *>("BIN/MODELS/btr.g3dm"));
      IsFirst = FALSE;
    }
  }

  VOID Render( zagl::anim *Ani ) override
  {
    Prs->PrimsDraw(mth::matr<FLT>::Scale(mth::vec3<FLT>(1)));
  }

  ~unit_btr(VOID)
  {
    Prs->Free();
    delete Prs;
  }
}; /* End of 'unit_btr' class */

/* Torpedo unit class */
class unit_torpedo : public zagl::unit
{
public:
  zagl::primitives *Pr;
  zagl::material mat;
  mth::vec3<FLT> Dir, Pos;
  FLT a;

  BOOL IsFirst, IsDelete;

  unit_torpedo(VOID) : IsFirst(TRUE), IsDelete(FALSE), Dir(0, 0, -1), Pos(0, -5, 0), a(0)
  {
    Pr = new zagl::primitives;
  }

  VOID Response(zagl::anim *Ani) override
  {
    if (IsFirst)
    {
      Ani->render::shader_manager::Add(const_cast<CHAR *>("DEFAULT"));
      Ani->render::primitives_manager::PrimsLoad(Pr, const_cast<CHAR *>("BIN/MODELS/torpedo.g3dm"));
      IsFirst = FALSE;
      Pr->Prims[0].Mtl = &mat;
      Dir.X = Ani->Camera.Dir.X;
      Dir.Z = Ani->Camera.Dir.Z;
      a = atan(Dir.X / Dir.Z) * 180 / PI;
      a = Dir.Z > 0 ? a + 180 : a;
    }
    Pos += Dir * Ani->DeltaTime * 50;
    if (Pos.X > 100 || Pos.X < -100 || Pos.Z > 100 || Pos.Z < -100)
      IsDelete = TRUE;
    /*else if (unit_target::Position == Pos)
      ;*/

  }

  VOID Render(zagl::anim *Ani) override
  {
    if (!IsDelete)
      Pr->PrimsDraw(mth::matr<FLT>::Scale(mth::vec3<FLT>(0.01)) * mth::matr<FLT>::Translate(mth::vec3<FLT>(0, 0, -0.3)) * mth::matr<FLT>::RotateX(Ani->Time * 500) * mth::matr<FLT>::RotateY(-90 + a) * mth::matr<FLT>::Translate(Pos));
  }

  ~unit_torpedo(VOID)
  {
    Pr->Free();
    delete Pr;
  }
}; /* End of 'unit_torpedo' class */

/* target unit class */
class unit_target : public zagl::unit
{
public:
  zagl::primitives *Prs;
  zagl::prim *Sphere;
  zagl::material mat;
  zagl::prim *Base;
  mth::vec3<FLT> Position;
  FLT x, a;
  INT SplitW, SplitH;
  BOOL IsFirst;

  friend class unit_torpedo;

  unit_target(VOID) : IsFirst(TRUE), a(0), x(-5), Position(0, -4, x), SplitW(15), SplitH(20)
  {
    std::vector<INT> I;
    std::vector<zagl::vertex::std> V;

    FLT tetha = 0, phi = 0;
    for (INT i = 0; i < SplitH; i++, tetha += PI / (SplitH - 1))
      for (INT j = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
        V.push_back(zagl::vertex::std(mth::vec3(sin(tetha) *sin(phi), cos(tetha), sin(tetha) *cos(phi)), mth::vec2<FLT>(0),
                                      mth::vec3<FLT>(sin(tetha) *sin(phi), cos(tetha), sin(tetha) *cos(phi)), mth::vec4<FLT>::Rnd0()));

    for (INT i = 0; i < SplitH - 1; i++)
      for (INT j = 0; j < SplitW - 1; j++)
      {
        I.push_back(i * SplitW + j + 1);
        I.push_back((i + 1) * SplitW + j);
        I.push_back(i * SplitW + j);

        I.push_back(i * SplitW + j + 1);
        I.push_back((i + 1) * SplitW + j);
        I.push_back((i + 1) * SplitW + j + 1);
      }

    Sphere = new zagl::prim;

    memset(Sphere, 0, sizeof(zagl::prim));

    zagl::topology::base<zagl::vertex::std> T1(zagl::prim_type::TRIMESH, V, I);

    Sphere->Create<zagl::vertex::std>(T1);
    Sphere->Mtl = &mat;
    Sphere->MtlNo = 1;
    Sphere->Mtl->ShdName = "DEFAULT";
    Sphere->Transform = mth::matr<FLT>::Scale(mth::vec3<FLT>(1.5)) * mth::matr<FLT>::Translate(mth::vec3<FLT>(Position)) * mth::matr<FLT>::RotateY(a);

    srand(30);

    V.clear();
    I.clear();

    zagl::vertex::std ve(mth::vec3<FLT>(100, 0, 100), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 1, 0), mth::vec4<FLT>(1));
    V.push_back(ve);
    zagl::vertex::std ve1(mth::vec3<FLT>(100, 0, -100), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 1, 0), mth::vec4<FLT>(1));
    V.push_back(ve1);
    zagl::vertex::std ve2(mth::vec3<FLT>(-100, 0, 100), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 1, 0), mth::vec4<FLT>(1));
    V.push_back(ve2);
    zagl::vertex::std ve3(mth::vec3<FLT>(-100, 0, -100), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 1, 0), mth::vec4<FLT>(1));
    V.push_back(ve3);

    I.push_back(0);
    I.push_back(1);
    I.push_back(2);

    I.push_back(2);
    I.push_back(1);
    I.push_back(3);

    zagl::topology::base<zagl::vertex::std> T(zagl::prim_type::TRIMESH, V, I);

    Base = new zagl::prim;

    memset(Base, 0, sizeof(zagl::prim));

    Base->Create<zagl::vertex::std>(T);
    Base->Mtl = &mat;
    Base->MtlNo = 1;
    Base->Mtl->ShdName = "DEFAULT";
  }

  VOID Response(zagl::anim *Ani) override
  {
    if (IsFirst)
    {
      Ani->render::shader_manager::Add(const_cast<CHAR *>("DEFAULT"));
      IsFirst = FALSE;
    }

    Ani->Camera.Rotate(mth::vec3<FLT>(0, 1, 0), (50 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]) * Ani->GlobalDeltaTime));

    if (Ani->KeysClick[VK_SPACE])
    {
      if (atan(1 / sqrtf((Position - Ani->Camera.Loc).Length2() - 1)) <= acos((Position - Ani->Camera.Loc).Normalizing() & Ani->Camera.Dir.Normalizing()))
      {
        x = -rand() % 90 - 8;
        Position = mth::vec3<FLT>(0, -4, x);
        a = rand();
        Sphere->Transform = mth::matr<FLT>::Scale(mth::vec3<FLT>(1.5)) * mth::matr<FLT>::Translate(mth::vec3<FLT>(Position)) * mth::matr<FLT>::RotateY(a);
      }
      Ani->Scene << new unit_torpedo;
    }
  }

  VOID Render(zagl::anim *Ani) override
  {
    Ani->DrawPrim(Base, mth::matr<FLT>::Translate(mth::vec3<FLT>(0, -5.25, 0)));
    Ani->DrawPrim(Sphere, mth::matr<FLT>::Identity());
  }

  ~unit_target(VOID)
  {
    Sphere->Free();
    delete Sphere;

    Base->Free();
    delete Base;
  }
}; /* End of 'unit_target' class */

/* Mobius unit class */
class unit_mobius : public zagl::unit
{
public:
  zagl::prim *Prs, *Sphere;
  zagl::material mat;
  BOOL IsFirst;
  FLT a, b, c, v, R;
  INT N, SplitW, SplitH;

  unit_mobius( VOID ) : IsFirst(TRUE), a(5), b(1), c(3), N(50), SplitW(15), SplitH(20), v(8), R(120)
  {
    std::vector<zagl::vertex::std> V;
    zagl::vertex::std ve(mth::vec3(a, b, c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 1, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve);
    zagl::vertex::std ve1(mth::vec3(a, b, -c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 1, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve1);
    zagl::vertex::std ve2(mth::vec3(-a, b, c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 1, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve2);
    zagl::vertex::std ve3(mth::vec3(-a, b, -c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 1, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve3);

    zagl::vertex::std ve4(mth::vec3(a, -b, c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, -1, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve4);
    zagl::vertex::std ve5(mth::vec3(-a, -b, c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, -1, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve5);
    zagl::vertex::std ve22(mth::vec3(a, -b, -c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, -1, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve22);
    zagl::vertex::std ve23(mth::vec3(-a, -b, -c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, -1, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve23);


    zagl::vertex::std ve6(mth::vec3(a, b, c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 0, 1), mth::vec4<FLT>::Rnd0());
    V.push_back(ve6);
    zagl::vertex::std ve7(mth::vec3(-a, b, c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 0, 1), mth::vec4<FLT>::Rnd0());
    V.push_back(ve7);
    zagl::vertex::std ve8(mth::vec3(a, -b, c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 0, 1), mth::vec4<FLT>::Rnd0());
    V.push_back(ve8);
    zagl::vertex::std ve9(mth::vec3(-a, -b, c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 0, 1), mth::vec4<FLT>::Rnd0());
    V.push_back(ve9);

    zagl::vertex::std ve10(mth::vec3(a, b, -c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 0, -1), mth::vec4<FLT>::Rnd0());
    V.push_back(ve10);
    zagl::vertex::std ve11(mth::vec3(a, -b, -c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 0, -1), mth::vec4<FLT>::Rnd0());
    V.push_back(ve11);
    zagl::vertex::std ve12(mth::vec3(-a, b, -c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 0, -1), mth::vec4<FLT>::Rnd0());
    V.push_back(ve12);
    zagl::vertex::std ve13(mth::vec3(-a, -b, -c), mth::vec2<FLT>(0), mth::vec3<FLT>(0, 0, -1), mth::vec4<FLT>::Rnd0());
    V.push_back(ve13);


    zagl::vertex::std ve14(mth::vec3(a, b, c), mth::vec2<FLT>(0), mth::vec3<FLT>(1, 0, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve14);
    zagl::vertex::std ve15(mth::vec3(a, -b, c), mth::vec2<FLT>(0), mth::vec3<FLT>(1, 0, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve15);
    zagl::vertex::std ve16(mth::vec3(a, b, -c), mth::vec2<FLT>(0), mth::vec3<FLT>(1, 0, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve16);
    zagl::vertex::std ve17(mth::vec3(a, -b, -c), mth::vec2<FLT>(0), mth::vec3<FLT>(1, 0, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve17);

    zagl::vertex::std ve18(mth::vec3(-a, b, c), mth::vec2<FLT>(0), mth::vec3<FLT>(-1, 0, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve18);
    zagl::vertex::std ve19(mth::vec3(-a, -b, c), mth::vec2<FLT>(0), mth::vec3<FLT>(-1, 0, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve19);
    zagl::vertex::std ve20(mth::vec3(-a, b, -c), mth::vec2<FLT>(0), mth::vec3<FLT>(-1, 0, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve20);
    zagl::vertex::std ve21(mth::vec3(-a, -b, -c), mth::vec2<FLT>(0), mth::vec3<FLT>(-1, 0, 0), mth::vec4<FLT>::Rnd0());
    V.push_back(ve21);

    std::vector<INT> I;

    I.push_back(0);
    I.push_back(1);
    I.push_back(2);

    I.push_back(2);
    I.push_back(1);
    I.push_back(3);


    I.push_back(4);
    I.push_back(5);
    I.push_back(6);

    I.push_back(6);
    I.push_back(5);
    I.push_back(7);


    I.push_back(8);
    I.push_back(9);
    I.push_back(10);

    I.push_back(10);
    I.push_back(9);
    I.push_back(11);

    I.push_back(12);
    I.push_back(13);
    I.push_back(14);

    I.push_back(14);
    I.push_back(13);
    I.push_back(15);


    I.push_back(16);
    I.push_back(17);
    I.push_back(18);

    I.push_back(18);
    I.push_back(17);
    I.push_back(19);


    I.push_back(20);
    I.push_back(21);
    I.push_back(22);

    I.push_back(22);
    I.push_back(21);
    I.push_back(23);

    zagl::topology::base<zagl::vertex::std> T(zagl::prim_type::TRIMESH, V, I);

    Prs = new zagl::prim[N];

    memset(Prs, 0, sizeof(zagl::prim) * N);

    for (INT i = 0; i < N; i++)
    {
      Prs[i].Create<zagl::vertex::std>(T);
      Prs[i].Mtl = &mat;
      Prs[i].MtlNo = 1;
      Prs[i].Mtl->ShdName = "DEFAULT";
    }

    Sphere = new zagl::prim[2 * N];

    V.clear();
    I.clear();

    srand(30);

    FLT tetha = 0, phi = 0;
    for (INT i = 0; i < SplitH; i++, tetha += PI / (SplitH - 1))
      for (INT j = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
        V.push_back(zagl::vertex::std(mth::vec3(sin(tetha) * sin(phi), cos(tetha), sin(tetha) * cos(phi)), mth::vec2<FLT>(0),
                                      mth::vec3<FLT>(sin(tetha) * sin(phi), cos(tetha), sin(tetha) * cos(phi)), mth::vec4<FLT>::Rnd0()));


    for (INT i = 0; i < SplitH - 1; i++)
      for (INT j = 0; j < SplitW - 1; j++)
      {
        I.push_back(i * SplitW + j + 1);
        I.push_back((i + 1) * SplitW + j);
        I.push_back(i * SplitW + j);

        I.push_back(i * SplitW + j + 1);
        I.push_back((i + 1) * SplitW + j);
        I.push_back((i + 1) * SplitW + j + 1);
      }

    zagl::topology::base<zagl::vertex::std> T1(zagl::prim_type::TRIMESH, V, I);

    for (INT i = 0; i < 2 * N; i++)
    {
      Sphere[i].Create<zagl::vertex::std>(T1);
      Sphere[i].Mtl = &mat;
      Sphere[i].MtlNo = 1;
      Sphere[i].Mtl->ShdName = "DEFAULT";
    }

  }

  VOID Response( zagl::anim *Ani ) override
  {
    if (IsFirst)
    {
      Ani->render::shader_manager::Add(const_cast<CHAR *>("DEFAULT"));
      IsFirst = FALSE;
    }
    DBL an = 0;
    for (INT i = 0; i < N; i++, an += (FLT)360 / N)
    {
      Prs[i].Transform = mth::matr<FLT>::RotateZ(Ani->Time * v / 2 + an / 2) * mth::matr<FLT>::Translate(mth::vec3<FLT>(R, 0, 0)) * mth::matr<FLT>::RotateY(Ani->Time * v + an);
      Sphere[i].Transform = mth::matr<FLT>::Translate(mth::vec3<FLT>(0, 1.4 + 2 * fabs(sin(Ani->Time * N * PI * v / 180)), 0)) * mth::matr<FLT>::RotateZ(-Ani->Time * v / 2 - an / 2) *
                          mth::matr<FLT>::Translate(mth::vec3<FLT>(R * 0.4, 0, 0)) * mth::matr<FLT>::RotateY(-Ani->Time * v - an);
    }

    an = 0;
    for (INT i = N; i < 2 * N; i++, an += (FLT)360 / N)
      Sphere[i].Transform = mth::matr<FLT>::Translate(mth::vec3<FLT>(0, -1.4 - 2 * fabs(sin(Ani->Time * N * PI * v / 180)), 0)) * mth::matr<FLT>::RotateZ(-Ani->Time * v / 2 - an / 2) *
                            mth::matr<FLT>::Translate(mth::vec3<FLT>(R * 0.4, 0, 0)) * mth::matr<FLT>::RotateY(-Ani->Time * v - an);
  }

  VOID Render( zagl::anim *Ani ) override
  {
    for (INT i = 0; i < N; i++)
      Ani->DrawPrim(&Prs[i], mth::matr<FLT>::Scale(mth::vec3<FLT>(0.2)));

    for (INT i = 0; i < 2 * N; i++)
      Ani->DrawPrim(&Sphere[i], mth::matr<FLT>::Scale(mth::vec3<FLT>(0.5)));
  }

  ~unit_mobius( VOID )
  {
    for (INT i = 0; i < N; i++)
      Prs[i].Free();
    delete[] Prs;

    for (INT i = 0; i < 2 * N; i++)
      Sphere[i].Free();
    delete[] Sphere;
  }
}; /* End of 'unit_mobius' class */

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  zagl::anim &AnimInstance = zagl::anim::Get();

  AnimInstance.Scene << new unit_target;
  //AnimInstance.Scene << new unit_control << new unit_mobius;

  AnimInstance.Run();
} /* End of 'WinMain' function */

/* END OF 'main.cpp' FILE */