/* FILE NAME  : anim.h
 * PROGRAMMER : BZ66
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Animation class declaration.
 */

#ifndef __anim_h_
#define __anim_h_

#include "../win/win.h"
#include "../utils/stock.h"
#include "input.h"
#include "rnd/render.h"

namespace zagl
{
  class anim;

  /* Unit class */
  class unit
  {
  public:
    virtual ~unit( VOID )
    {
    }

    virtual VOID Response( anim *Ani )
    {
    }

    virtual VOID Render( anim *Ani )
    {
    }
  }; /* End of 'unit' class */

  /* Scene class */
  class scene : public unit
  {
  private:
    stock<unit *> Units;
  public:
    scene( VOID )
    {
    }

    /* Scene destructor */
    ~scene( VOID )
    {
      Units.Walk([]( unit *Uni ){ delete Uni; });
    }

    stock<unit *> & Add( unit *Uni )
    {
      return Units.Add(Uni);
    }

    scene & operator<<( unit *Uni )
    {
      Units.Add(Uni);
      return *this;
    }

    //scene & operator<<( const std::string &Name );

    VOID Render( anim *Ani )
    {
      Units.Walk([Ani]( unit *Uni ){ Uni->Render(Ani); });
    }

    VOID Response( anim *Ani )
    {
      Units.Walk([Ani]( unit *Uni ){ Uni->Response(Ani); });
    }
  }; /* End of 'unit' class */

  class anim : public win, public input, public timer, public scene, public render
  {
  private:

    anim( VOID ) : win(), input(win::hWnd, win::MouseWheel),
                   timer(), render(win::hWnd, win::W, win::H)
    {
    }

    VOID Init( VOID ) override;
    VOID Close( VOID ) override;
    VOID Resize( VOID ) override;
    VOID Activate( VOID ) override;
    VOID Timer( VOID ) override;
    VOID Idle( VOID ) override;
    VOID Erase( HDC hDC ) override;
    VOID Paint( HDC hDC ) override;

    static anim Instance;
  public:
    /*std::map<std::string, unit *(*)( VOID )> UnitNames;
    template<class UnitType>
      class unit_register
      {
      public:
        static unit * Create( VOID )
        {
          return new UnitType(&Instance);
        }
        unit_register( const std::string &Name )
        {
          Instance.UnitNames[Name] = Create();
        }
      };*/

    scene Scene; // scene stock

    /* Get animation instance function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (anim &) animation instance.
     */
    static anim & Get( VOID )
    {
      return Instance;
    } /* End of 'Get' function */

    /* Get animation instance pointer function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (anim *) animation instance.
     */
    static anim * GetPtr( VOID )
    {
      return &Instance;
    } /* End of 'GetPtr' function */

    /* Render function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( VOID )
    {
      timer::Response();
      input::Response();

      render::Start();

#if _DEBUG
      static FLT ReloadTime = 30.47;
      ReloadTime += timer::GlobalDeltaTime;
      if (ReloadTime > 1)
      {
        shader_manager::Update();
        ReloadTime = 0;
      }
#endif /* _DEBUG */
      Scene.Response(&Instance);
      Scene.Render(&Instance);

      render::End();
    } /* End of 'Render' function */
  }; /* End of 'anim' class */
} /* end of 'zagl' namespace */

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */
