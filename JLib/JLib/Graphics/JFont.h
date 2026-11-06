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

/** Fuente para mostrar texto en pantalla.
 * @file    JFont.h
 * @author  Juan Carlos Seijo Pérez
 * @date    27/03/2004
 * @version 0.0.1 - 27/03/2004 - Primera versión.
 */

#ifndef _JFONT_INCLUDED
#define _JFONT_INCLUDED

#include <JLib/Graphics/JImage.h>
#include <SDL_ttf.h>
#include <stdarg.h>

/** Alineación del texto a renderizar.
 */
typedef enum JFontAlign
{
	JFONTALIGN_LEFT = 0,                  /**< Alineado a la izquierda. */
	JFONTALIGN_RIGHT,                     /**< Alineado a la derecha. */
	JFONTALIGN_CENTER,                    /**< Centrado. */
};

/** Clase wrapper de la clase SDL_Font de la librería SDL_ttf.
 * Permite mostrar texto en pantalla.
 */
class JFont
{
	/** Tipo de renderizado a usar
	 */
	typedef enum JFontRenderType
	{
		JFONTRENDERTYPE_SOLID = 0,          /**< Sólido con colorkey. */
		JFONTRENDERTYPE_SHADED,             /**< Con antialiasing en fondo sólido. */
		JFONTRENDERTYPE_BLENDED,            /**< Con antializasing en fondo transparente. */
	};

	TTF_Font *font;
	
	/** Función de renderizado de las PrintfXXX.
   * @param  type Tipo de render a emplear.
	 * @param  align Alineación del texto.
	 * @param  fg Color de fuente.
	 * @param  bg Color de fondo (sólo para JFONTRENDERTYPE_SHADED).
	 * @param  str Texto a renderizar.
	 */
	JImage * Printf(JFontRenderType type, JFontAlign align, SDL_Color &fg, SDL_Color &bg, char *str);

 public:
	/** Crea una fuente vacía.
	 */
	JFont() : font(0)
	{}

  /** Recupera la versión de ejecución de SDL_ttf. El objeto devuelto tiene
   * tres campos: major, minor y patch, que identifican la versión.
   * @return La versión de ejecución SDL_ttf.
   */
  const SDL_version * GetLinkedVersion() {return TTF_Linked_Version();}

	/** Inicializa el subsistema de fuentes. Sólo es necesaria la primera vez.
	 * @return <b>true</b> si todo va bien, <b>false</b> en caso contrario.
	 */
	static bool Init()
	{
		if (TTF_WasInit() == 0)
		{
			return (-1 != TTF_Init());
		}

		return true;
	}

	/** Devuelve la cadena de error del último error.
	 * @return Cadena de error del último error.
	 */
	static const char * GetError()
	{
		return TTF_GetError();
	}

	/** Abre la fuente del fichero indicado.
	 * @param  filename Nombre del fichero .ttf o .fon.
	 * @param  size Tamaño en pixels (aproximado).
	 * @return <b>true</b> si se pudo abrir, <b>false</b> si no.
	 */
	bool Open(const char * filename, s32 size)
	{
		Destroy();
		return (0 != (font = TTF_OpenFont(filename, size)));
	}

	/** Establece el estilo.
	 * @param  style Nuevo estilo. Puede ser una combinación de
	 * TTF_STYLE_NORMAL, TTF_STYLE_BOLD, TTF_STYLE_ITALIC o TTF_STYLE_UNDERLINE.
	 */
	void Style(s32 style)
	{
		TTF_SetFontStyle(font, style);
	}

	/** Recupera el estilo.
	 * @return  Estilo actual como combinación de TTF_STYLE_NORMAL, 
	 * TTF_STYLE_BOLD, TTF_STYLE_ITALIC y TTF_STYLE_UNDERLINE.
	 */
	s32 Style()
	{
		return TTF_GetFontStyle(font);
	}

	/** Devuelve la altura máxima de la fuente.
	 * @return Altura máxima de la fuente.
	 */
	s32 Height()
	{
		return TTF_FontHeight(font);
	}

	/** Devuelve el ascenso de la fuente.
	 * @return Ascenso de la fuente.
	 */
	s32 Ascent()
	{
		return TTF_FontAscent(font);
	}

	/** Devuelve el descenso de la fuente.
	 * @return Descenso de la fuente.
	 */
	s32 Descent()
	{
		return TTF_FontDescent(font);
	}

	/** Devuelve la separación recomendada de línea.
	 * @return Separación recomendada de línea.
	 */
	s32 LineDistance()
	{
		return TTF_FontLineSkip(font);
	}

	/** Renderiza el texto dado de forma rápida.
	 * @param  text Texto a renderizar.
	 * @param  fg Color de fuente.
	 */
	JImage * RenderTextSolid(const char * text, SDL_Color &fg)
	{
		SDL_Surface *s = TTF_RenderUTF8_Solid(font, text, fg);
		return s != 0 ? new JImage(s) : 0;
	}

	/** Renderiza el texto UNICODE dado de forma rápida.
	 * @param  text Texto a renderizar.
	 * @param  fg Color de fuente.
	 */
	JImage * RenderUNICODESolid(const u16 *text, SDL_Color &fg)
	{
		SDL_Surface *s = TTF_RenderUNICODE_Solid(font, text, fg);
		return s != 0 ? new JImage(s) : 0;
	}

	/** Renderiza el caracter dado de forma rápida.
	 * @param  c Caracter a renderizar.
	 * @param  fg Color de fuente.
	 */
	JImage * RenderGlyphSolid(char c, SDL_Color &fg)
	{
		SDL_Surface *s = TTF_RenderGlyph_Solid(font, c, fg);
		return s != 0 ? new JImage(s) : 0;
	}

	/** Renderiza el texto dado suavizado sobre fondo sólido.
	 * @param  text Texto a renderizar.
	 * @param  fg Color de fuente.
	 * @param  bg Color de fondo.
	 */
	JImage * RenderTextShaded(const char * text, SDL_Color &fg, SDL_Color &bg)
	{
		SDL_Surface *s = TTF_RenderUTF8_Shaded(font, text, fg, bg);
		return s != 0 ? new JImage(s) : 0;
	}

	/** Renderiza el texto UNICODE dado suavizado sobre fondo sólido.
	 * @param  text Texto a renderizar.
	 * @param  fg Color de fuente.
	 * @param  bg Color de fondo.
	 */
	JImage * RenderUNICODEShaded(const u16 *text, SDL_Color &fg, SDL_Color &bg)
	{
		SDL_Surface *s = TTF_RenderUNICODE_Shaded(font, text, fg, bg);
		return s != 0 ? new JImage(s) : 0;
	}

	/** Renderiza el caracter dado suavizado sobre fondo sólido.
	 * @param  c Caracter a renderizar.
	 * @param  fg Color de fuente.
	 * @param  bg Color de fondo.
	 */
	JImage * RenderGlyphShaded(char c, SDL_Color &fg, SDL_Color &bg)
	{
		SDL_Surface *s = TTF_RenderGlyph_Shaded(font, c, fg, bg);
		return s != 0 ? new JImage(s) : 0;
	}

	/** Renderiza el texto dado suavizado sobre fondo transparente.
	 * @param  text Texto a renderizar.
	 * @param  fg Color de fuente.
	 */
	JImage * RenderTextBlended(const char * text, SDL_Color &fg)
	{
		SDL_Surface *s = TTF_RenderUTF8_Blended(font, text, fg);
		return s != 0 ? new JImage(s) : 0;
	}

	/** Renderiza el texto dado suavizado sobre fondo transparente.
	 * @param  text Texto a renderizar.
	 * @param  fg Color de fuente.
	 */
	JImage * RenderUNICODEBlended(const u16 * text, SDL_Color &fg)
	{
		SDL_Surface *s = TTF_RenderUNICODE_Blended(font, text, fg);
		return s != 0 ? new JImage(s) : 0;
	}

	/** Renderiza el caracter dado suavizado sobre fondo transparente.
	 * @param  c Caracter a renderizar.
	 * @param  fg Color de fuente.
	 */
	JImage * RenderGlyphBlended(char c, SDL_Color &fg)
	{
		SDL_Surface *s = TTF_RenderGlyph_Blended(font, c, fg);
		return s != 0 ? new JImage(s) : 0;
	}

	/** Destruye el objeto, liberando la memoria asociada.
	 */
	void Destroy()
	{
		if (font != 0)
		{
			TTF_CloseFont(font);
			font = 0;
		}
	}

	/** Renderiza el texto con formato dado de forma rápida.
	 * @param  align Alineación del texto.
	 * @param  fg Color de fuente.
	 * @param  strFormat Texto a renderizar.
	 * @param  ... Argumentos adicionales tipo printf().
	 */
	JImage * PrintfSolid(JFontAlign align, SDL_Color &fg, const char *strFormat, ...);

	/** Renderiza el texto con formato suavizado sobre fondo sólido.
	 * @param  align Alineación del texto.
	 * @param  fg Color de fuente.
	 * @param  bg Color de fondo.
	 * @param  strFormat Texto a renderizar.
	 * @param  ... Argumentos adicionales tipo printf().
	 */
	JImage * PrintfShaded(JFontAlign align, SDL_Color &fg, SDL_Color &bg, const char *strFormat, ...);

	/** Renderiza el texto con formato suavizado sobre fondo transparente.
	 * @param  align Alineación del texto.
	 * @param  fg Color de fuente.
	 * @param  strFormat Texto a renderizar.
	 * @param  ... Argumentos adicionales tipo printf().
	 */
	JImage * PrintfBlended(JFontAlign align, SDL_Color &fg, const char *strFormat, ...);

	/** Destruye el objeto, liberando la memoria asociada.
	 */
	~JFont()
	{
		Destroy();
	}
};

#endif // _JFONT_INCLUDED
