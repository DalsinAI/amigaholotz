/*
 *  JLib - Jacob's Library.
 *  Copyright (C) 2003, 2004  Juan Carlos Seijo Pérez
 * 
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 * 
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 * 
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 *  Juan Carlos Seijo Pérez
 *  jacob@mainreactor.net
 */

///////////////////////////////////////////////////////////////////////////////
// @author: Juan Carlos Seijo Pérez
// @date: 24/05/2003
// @description: Funciones de manejo de texto de windows
///////////////////////////////////////////////////////////////////////////////

#include <JLib/Util/JTextUtil.h>

// Crea una superficie en formato RGBA de 32 bits con un mapa de caracteres en
// 16 filas y 16 columnas para el formato de fuente especificado.
// NOTA: Es responsabilidad del llamador liberar la memoria del buffer devuelto
// por esta función.
//u32* GetFontSurface(JFontFormat format)
//{
//  HDC hDCSurf, hDCScr;
//  hDCScr = GetDC(0);
//  hDCSurf = CreateCompatibleDC(hDCScr);
//
//  // Crea la fuente a partir de las de windows
//  HFONT font = CreateFont(-format.fontSize,
//                          0,
//                          0,
//                          0,
//                          format.fontWeight,
//                          format.cursive,
//                          format.underscore,
//                          0,
//                          ANSI_CHARSET,
//                          OUT_DEFAULT_PRECIS,
//                          CLIP_DEFAULT_PRECIS,
//                          ANTIALIASED_QUALITY,
//                          DEFAULT_PITCH | FF_SWISS,
//                          format.fontName);
//
//  // Dibuja el texto en memoria
//  HBITMAP hBMP, hBMPOld;
//  BITMAPINFO bi;
//  
//  RECT rc;
//  s8 str[272];    // 255 caracteres + 15 saltos de línea \n\r + 1 '/0'
//  for (s32 i = 0, cnt = 0; i < 271; ++i)
//  {
//    if (i > 0 && i < 271 && i % 16 == 0)
//    {
//      // cr + lf
//      str[i] = '\n';
//      str[++i] = '\r';
//      ++i;
//    }
//
//    str[i] = cnt++;
//  }
//  
//  str[271] = 0;
//
//  
//  // Calcula el tamaño del rectángulo con la fuente escogida
//  SIZE sz;
//  HFONT oldFont = (HFONT)SelectObject(hDCSurf, font);
//  GetTextExtentPoint32(hDCScr, str, (s32)strlen(str), &sz);
//  rc.top = 0;
//  rc.left = 0;
//  rc.right = sz.cx;
//  rc.bottom = sz.cy;
//  
//  // Pinta sobre la superficie en memoria
//  bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//  bi.bmiHeader.biBitCount = 32;
//  bi.bmiHeader.biCompression = BI_RGB;
//  bi.bmiHeader.biPlanes = 1;
//  bi.bmiHeader.biWidth = rc.right;
//  bi.bmiHeader.biHeight = rc.bottom;
//  bi.bmiHeader.biSizeImage = 0;
//
//  void *bits, *surf;
//  hBMP = CreateDIBSection(hDCSurf, &bi, DIB_RGB_COLORS, &bits, 0, 0x0);
//  hBMPOld = (HBITMAP)SelectObject(hDCSurf, hBMP);
//  
//  // Establece el color de fuente
//  SetBkColor(hDCSurf, RGB(0, 0, 0));
//  SetTextColor(hDCSurf, RGB(format.b, format.g, format.r));
//  TextOut(hDCSurf, 0, 0, str, (s32)strlen(str));
//
//  // Optimizar para MMX
//  surf = (void *)new u32[rc.right * rc.bottom];
//  CopyMemory(surf, bits, rc.right * rc.bottom * 4);
//
//  // Asigna valores alfa para la superficie en función de la
//  // proximidad de los pixels al texto
//  u32 val;
//  u32 color = RGB(format.b, format.g, format.r);
//  u8 alpha;
//
//  for (u32 i = 0; i < (u32)bi.bmiHeader.biWidth * bi.bmiHeader.biHeight; ++i)
//  {
//    val = *(((u32 *)surf) + i);
//
//    if (val > 0)
//      val = val;
//
//    // Asignamos un valor al canal alfa inversamente proporcional al valor del color
//    alpha = (u8)(255 * ((double)val/(double)color));
//
//    if (alpha > 0)
//      alpha = alpha;
//
//    *(((u32*)surf) + i) |= ((u32)alpha) << 24;
//  }
//
//  SelectObject(hDCSurf, hBMPOld);
//  SelectObject(hDCSurf, oldFont);
//  DeleteDC(hDCSurf);
//  ReleaseDC(0, hDCScr);
//
//  return (u32 *)surf;
//}
