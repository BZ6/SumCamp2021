/* FILE NAME  : raytracer.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 04.08.2021
 * PURPOSE    : Main work ray tracing definition module.
 */

#ifndef __raytracer_h_
#define __raytracer_h_

#include "../win/win.h"
#include "fb/frame.h"
#include "shapes/shapes.h"

namespace zart
{
  class raytracer : public win
  {
  public:
    camera Cam;
    frame Frm;
    scene Scene;
    std::string ImageName;

    raytracer( VOID )
    {
      Frm.Resize(102, 47);
    }

    VOID Init( VOID )
    {
      Cam.Resize(Frm.W, Frm.H);

      Render();

      Frm.SaveTGA(ImageName);

      InvalidateRect(win::hWnd, nullptr, FALSE);
    }

    VOID Render(VOID)
    {
      zart::vec3 L(2, 2, 2);
      for (INT y = 1; y < Frm.H - 1; y++)
        for (INT x = 1; x < Frm.W - 1; x++)
        {
          ray R = Cam.FrameRay(x + 0.5, y + 0.5);
          intr I;
          if (Scene.Intersection(R, &I))
          {
            DBL nl = I.N & L.Normalizing();
            if (nl < 0.1)
              nl = 0.1;
            zart::vec3 color(0.8, 0.47, 0.27);
            color *= nl;
            if (Scene.Intersection(ray(R(I.T) + L * Trashhold, L), &I))
              color *= 0.30;
            Frm.PutPixel(x, y, (BYTE)(color.X < 0 ? 0 : color.X > 255 ? 255 : color.X * 255) << 16 |
                         (BYTE)(color.Y < 0 ? 0 : color.Y > 255 ? 255 : color.Y * 255) << 8 | (BYTE)(color.Z < 0 ? 0 : color.Z > 255 ? 255 : color.Z * 255));
          }
          else
            Frm.PutPixel(x, y, 0);
        }

      for (INT i = 0; i < Frm.W; i++)
        Frm.PutPixel(i, 0, 0x00FF0000);
      for (INT i = 0; i < Frm.W; i++)
        Frm.PutPixel(i, Frm.H - 1, 0x00FF0000);
      for (INT i = 0; i < Frm.H; i++)
        Frm.PutPixel(0, i, 0x00FF0000);
      for (INT i = 0; i < Frm.H; i++)
        Frm.PutPixel(Frm.W - 1, i, 0x00FF0000);
    }

    VOID Paint(HDC hDC) override
    {
      Frm.Draw(hDC, 0, 0);
    }

    VOID Erase(HDC hDC) override
    {
      SelectObject(hDC, GetStockObject(NULL_PEN));
      Rectangle(hDC, 0, 0, W, H);
    }

    VOID LoadData(const std::string &FileName)
    {
      std::ifstream f("bin/data/" + FileName + ".txt");
      auto asd = f.is_open();
      std::string s((std::istreambuf_iterator<char>(f)),
                    std::istreambuf_iterator<char>());

      if (s.substr(0, 6) == "Size:\n")
      {
        INT tmp = 6, w = 0, h = 0;

        if (s[tmp] == 'W')
        {
          tmp += 3;
          while (s[tmp] != ';')
            w = 10 * w + (s[tmp++] - '0');
          tmp += 2;

          tmp += 3;
          while (s[tmp] != ';')
            h = 10 * h + (s[tmp++] - '0');
          tmp += 2;
        }
        else
        {
          tmp += 3;
          while (s[tmp] != ';')
            h = 10 * h + (s[tmp++] - '0');
          tmp += 2;

          tmp += 3;
          while (s[tmp] != ';')
            w = 10 * w + (s[tmp++] - '0');
          tmp += 2;

        }
        Frm.Resize(w, h);
        s.erase(0, tmp);
      }

      if (s.substr(0, 15) == "Name of image:\n")
      {
        INT tmp = s.find_first_of(";");
        ImageName = s.substr(15, tmp - 15);
        s.erase(0, tmp + 2);
      }

      if (s.substr(0, 10) == "Movement:\n")
      {
        INT tmp = s.find_first_of(":", 10);
        if (s.substr(10, tmp - 10) == "RotateY")
        {
          DBL Angle = 0;
          INT k = 1;

          tmp += 2;
          if (s[tmp] == '-')
            k = -k, tmp++;
          while (s[tmp] != ';' && s[tmp] != '.')
            Angle = 10 * Angle + (s[tmp++] - '0');
          if (s[tmp] == '.')
          {
            tmp++;
            DBL p = 10;
            while (s[tmp] != ';')
              Angle += (s[tmp++] - '0') / p++;
          }
          Angle *= k;
          tmp += 2;

          Cam.Rotate(vec3(0, 1, 0), Angle);
        }

        if (s.substr(tmp, s.find_first_of(":", tmp) - tmp) == "RotateR")
        {
          DBL Angle = 0;
          INT k = 1;
          tmp = s.find_first_of(":", tmp);

          tmp += 2;
          if (s[tmp] == '-')
            k = -k, tmp++;
          while (s[tmp] != ';' && s[tmp] != '.')
            Angle = 10 * Angle + (s[tmp++] - '0');
          if (s[tmp] == '.')
          {
            tmp++;
            DBL p = 10;
            while (s[tmp] != ';')
              Angle += (s[tmp++] - '0') / p++;
          }
          Angle *= k;
          tmp += 2;

          Cam.Rotate(Cam.Right, Angle);
        }

        if (s.substr(tmp, s.find_first_of(":", tmp) - tmp) == "MoveX")
        {
          DBL Dx = 0;
          INT k = 1;
          tmp = s.find_first_of(":", tmp);

          tmp += 2;
          if (s[tmp] == '-')
            k = -k, tmp++;
          while (s[tmp] != ';' && s[tmp] != '.')
            Dx = 10 * Dx + (s[tmp++] - '0');
          if (s[tmp] == '.')
          {
            tmp++;
            DBL p = 10;
            while (s[tmp] != ';')
              Dx += (s[tmp++] - '0') / p++;
          }
          Dx *= k;
          tmp += 2;

          Cam.Move(vec3(Dx, 0, 0));
        }

        if (s.substr(tmp, s.find_first_of(":", tmp) - tmp) == "MoveZ")
        {
          DBL Dz = 0;
          INT k = 1;
          tmp = s.find_first_of(":", tmp);

          tmp += 2;
          if (s[tmp] == '-')
            k = -k, tmp++;
          while (s[tmp] != ';' && s[tmp] != '.')
            Dz = 10 * Dz + (s[tmp++] - '0');
          if (s[tmp] == '.')
          {
            tmp++;
            DBL p = 10;
            while (s[tmp] != ';')
              Dz += (s[tmp++] - '0') / p++;
          }
          Dz *= k;
          tmp += 2;

          Cam.Move(vec3(0, 0, Dz));
        }

        s.erase(0, tmp);
      }

      if (s.substr(0, 8) != "Append:\n")
        return;
      for (INT i = 8; i < s.length(); i++)
      {
        INT tmp1 = s.find_first_of(":", i), tmp2 = s.find_first_of(";", i);
        if (tmp1 < tmp2 && tmp1 != -1)
        {
          DBL a = 0, b = 0, c = 0, d = 0;

          std::string sub = s.substr(i, tmp1 - i);
          if (sub == "sphere")
          {
            INT k = 1;
            tmp1 += 3;
            if (s[tmp1] == '-')
              k = -k, tmp1++;
            while (s[tmp1] != ',' && s[tmp1] != '.')
              a = 10 * a + (s[tmp1++] - '0');
            if (s[tmp1] == '.')
            {
              tmp1++;
              DBL p = 10;
              while (s[tmp1] != ',')
                a += (s[tmp1++] - '0') / p++;
            }
            a *= k;
            tmp1 += 2;

            k = 1;
            if (s[tmp1] == '-')
              k = -k, tmp1++;
            while (s[tmp1] != ',' && s[tmp1] != '.')
              b = 10 * b + (s[tmp1++] - '0');
            if (s[tmp1] == '.')
            {
              tmp1++;
              DBL p = 10;
              while (s[tmp1] != ',')
                b += (s[tmp1++] - '0') / p++;
            }
            b *= k;
            tmp1 += 2;

            k = 1;
            if (s[tmp1] == '-')
              k = -k, tmp1++;
            while (s[tmp1] != ')' && s[tmp1] != '.')
              c = 10 * c + (s[tmp1++] - '0');
            if (s[tmp1] == '.')
            {
              tmp1++;
              DBL p = 10;
              while (s[tmp1] != ')')
                c += (s[tmp1++] - '0') / p++;
            }
            c *= k;
            tmp1 += 2;

            k = 1;
            if (s[tmp1] == '-')
              k = -k, tmp1++;
            while (s[tmp1] != ';' && s[tmp1] != '.')
              d = 10 * d + (s[tmp1++] - '0');
            if (s[tmp1] == '.')
            {
              tmp1++;
              DBL p = 10;
              while (s[tmp1] != ';')
                d += (s[tmp1++] - '0') / p++;
            }
            d *= k;

            Scene << new zart::sphere(zart::vec3(a, b, c), d);
          }
          else
          {
            INT k = 1;
            tmp1 += 3;
            if (s[tmp1] == '-')
              k = -k, tmp1++;
            while (s[tmp1] != ',' && s[tmp1] != '.')
              a = 10 * a + (s[tmp1++] - '0');
            if (s[tmp1] == '.')
            {
              tmp1++;
              DBL p = 10;
              while (s[tmp1] != ',')
                a += (s[tmp1++] - '0') / p++;
            }
            a *= k;
            tmp1 += 2;

            k = 1;
            if (s[tmp1] == '-')
              k = -k, tmp1++;
            while (s[tmp1] != ',' && s[tmp1] != '.')
              b = 10 * b + (s[tmp1++] - '0');
            if (s[tmp1] == '.')
            {
              tmp1++;
              DBL p = 10;
              while (s[tmp1] != ',')
                b += (s[tmp1++] - '0') / p++;
            }
            b *= k;
            tmp1 += 2;

            k = 1;
            if (s[tmp1] == '-')
              k = -k, tmp1++;
            while (s[tmp1] != ')' && s[tmp1] != '.')
              c = 10 * c + (s[tmp1++] - '0');
            if (s[tmp1] == '.')
            {
              tmp1++;
              DBL p = 10;
              while (s[tmp1] != ')')
                c += (s[tmp1++] - '0') / p++;
            }
            c *= k;
            tmp1 += 2;

            k = 1;
            if (s[tmp1] == '-')
              k = -k, tmp1++;
            while (s[tmp1] != ';' && s[tmp1] != '.')
              d = 10 * d + (s[tmp1++] - '0');
            if (s[tmp1] == '.')
            {
              tmp1++;
              DBL p = 10;
              while (s[tmp1] != ';')
                d += (s[tmp1++] - '0') / p++;
            }
            d *= k;

            Scene << new zart::plane(zart::vec3(a, b, c), d);
          }
          i = tmp1 + 1;
        }
        else
        {
          std::string sub = s.substr(i, tmp2);
          if (sub == "sphere;")
            Scene << new zart::sphere;
          else
            Scene << new zart::plane;
          i = tmp1 + 1;
        }
      }
    }

  }; /* End of 'raytracer' class */
}

#endif /* __raytracer_h_ */

/* END OF 'raytracer.h' FILE */