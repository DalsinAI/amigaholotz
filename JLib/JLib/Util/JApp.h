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
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 *  Juan Carlos Seijo Pérez
 *  jacob@mainreactor.net
 */

/** Base application class. Allows to init SDL and the other subsystems as sound.
 * @file    JApp.h
 * @author  Juan Carlos Seijo Pérez
 * @date    01/04/2003
 * @version 0.0.1 - First version - 01/04/2003
 */

#ifndef _JAPP_INCLUDED
#define _JAPP_INCLUDED

#include <JLib/Util/JTypes.h>
#include <JLib/Util/JString.h>
#include <JLib/Util/JTimer.h>
#include <JLib/Graphics/JFont.h>
#include <JLib/Sound/JMixer.h>
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#else
#include <unistd.h>
#define _sleep sleep
#endif

/** Application base class. You should inherit from this class and override 
 * the Update and Draw methods to create an application. The following example
 * creates a 400x300 window and fills the window with a diferent color each frame:
 * You can try to execute it with the '-h' option to see what standard parameters a JApp
 * accepts by default.
 * <pre>
 * 
 * #include <JLib/Util/JApp.h>
 * #include <SDL_keysym.h>
 * #include <stdio.h>
 * 
 * class MyApp : public JApp
 * {
 *   int myData;
 * 
 * public:
 *   MyApp() :   JApp("App window title", 400, 300, false, 16), myData(0)
 *   {}
 * 
 *   bool Update() 
 *   {
 *     UpdateEvents();
 * 
 *     ++myData;
 *     if (Keys()[SDLK_ESCAPE] != 0)
 *     {
 *       Exit();
 *     }
 *   }
 * 
 *   bool Draw() {SDL_FillRect(screen, 0, myData); Flip();}
 * };
 * 
 * int main(int argc, char **argv)
 * {
 *   MyApp app;
 *   
 *   // Optionally, parses the command line
 *   app.ParseArgs(argc, argv);
 * 
 *   // Initializes SDL.
 *   if (!app.Init())
 *   {
 *     printf("Error initializing app.\n");
 *     return -1;
 *   }
 * 
 *   // The aplication stays here until Exit() is called.
 *   return app.MainLoop();
 * }
 *
 * </pre>
 */
class JApp
{
protected:
  bool active;                    /**< Is the application active (first plane)? */
  bool paused;                    /**< Is paused the application? */
  bool endLoop;                   /**< Must exit the app? */
  JString title;                  /**< Window title */
  s32 width;                      /**< Window width */
  s32 height;                     /**< Window height */
  s32 depth;                      /**< Color depth */
  bool fullScreen;                /**< Fullscreen flag */
  bool doInput;                   /**< Must capture events (keyboard, mouse, WM)? */
  u32 flags;                      /**< SDL additional flags */
  u32 appPauseTime;               /**< Time at which the application is paused */
	u8 *keys;                       /**< Keyboard state */
	SDLMod keyMods;                 /**< Key modifiers state (Shift, Ctrl, etc.) */
	s32 numKeys;                    /**< Number of keyboard keys */
	s32 mouseX;                     /**< Mouse X position */
	s32 mouseY;                     /**< Mouse Y position */
	u8 mouseBt;                     /**< Mouse button mask */
	s32 mouseRelX;                  /**< Mouse X relative increment */
	s32 mouseRelY;                  /**< Mouse Y relative increment */
	s16 joyAxisX;                   /**< Joystick X offset */
	s16 joyAxisY;                   /**< Joystick Y offset */
	u8 joyButton;                   /**< Joystick button */
	JTimer timer;                   /**< Application timer */
	s32 fps;                        /**< Frames per second */
	bool dumpVideoInfo;             /**< Must dump video info at init? */

	JMixer mixer;                   /**< Audio mixer */
	bool soundEnabled;              /**< Is sound enabled? */

	char *iconName;                 /**< Application icon filename */
	
	static JApp* thisApp;           /**< Pointer to this application */

  void (*__OnActive)(bool active, s32 state);                         /**< Active event callback */
  void (*__OnKeyUp )(SDL_keysym key);                                 /**< Key up event callback */
  void (*__OnKeyDown)(SDL_keysym key);                                /**< Key down event callback */
  void (*__OnMouseMove)(s32 x, s32 y, s32 bt, s32 xRel, s32 yRel);    /**< Mouse move event callback */
  void (*__OnMouseDown)(s32 bt, s32 x, s32 y);                        /**< Mouse button down event callback */
  void (*__OnMouseUp)(s32 bt, s32 x, s32 y);                          /**< Mouse button up event callback */
  void (*__OnJoyMove)(s32 joyNum, s32 axis, s32 value);               /**< Joystick move event callback */
  void (*__OnJoyBall)(s32 joyNum, s32 ballIndex, s32 xRel, s32 yRel); /**< Joystick ball event callback */
  void (*__OnJoyHat)(s32 joyNum, s32 hatIndex, s32 value);            /**< Joystick hat event callback */
  void (*__OnJoyUp)(s32 joyNum, s32 btIndex);                         /**< Joystick button up event callback */
  void (*__OnJoyDown)(s32 joyNum, s32 btIndex);                       /**< Joystick button down event callback */
  void (*__OnQuit)();                                                 /**< Quit event callback */
  void (*__OnSysWM)();                                                /**< Window manager event callback */
  void (*__OnResize)(s32 w, s32 h);                                   /**< Resize event callback */
  void (*__OnExpose)();                                               /**< Window exposure (unhide) event callback */
  void (*__OnUser)(s32 code, void *data1, void *data2);               /**< User event callback */

  SDL_Surface *screen;            /**< Primary surface as of SDL_GetVideoSurface() */
  SDL_Joystick *joystick;         /**< Joystick device */

public:
  /** Creates the application. The audio mixer must be initialized in the child class.
   * @param  strTitle Window title.
   * @param  w Window width.
   * @param  h Window height.
   * @param  fullScr Fullscreen flag.
   * @param  _depth Bits per pixel (color depth, 8, 16, 24, 32, etc.)
   * @param  otherFlags SDL Additional flags
   */
  JApp(const JString& strTitle, s32 w = 1024, s32 h = 768, bool fullScr = true, s32 _depth = 16, u32 otherFlags = 0);

  /** Checks for events to process. In case the associated callback is registered, calls it.
   * @return <b>true</b> if event polling succeded, <b>false</b> if not.
   */
  virtual bool UpdateEvents();

  /** Initializes the app. Creates the window with the paraameters given at construction time.
   * Normally the child application class overrides this method to initialize its objects and calls the parent (this class)
   * to do the SDL initialization stuff.
   * @return <b>true</b> if the creation succeded <b>false</b> if not.
   */
  virtual bool Init(s32 argc = 0, char **argv = 0);

  /** Updates the application objects (graphics, sounds, A.I., etc.). Must be implemented in the child class.
   * JApp will call this method before Draw in the child class (by means of MainLoop().
   * @return <b>true</b> if success, <b>false</b> else.
   */
  virtual bool Update() = 0;

  /** Draws the application. As Update(), must be implemented in the child class.
   * @return <b>true</b> if ok, <b>false</b> else.
   */
  virtual bool Draw() = 0;

	/** Main loop. The default implementation calls Update() and if it succeded then calls Draw. The application remains
   * here until Exit() is called or Update() or Draw() fail.
   * @return Application exit code.
   */
	virtual s32 MainLoop();

  /** Returns the window width.
   * @return Window width.
   */
  s32 Width() {return width;}

  /** Returns the window height.
   * @return Window Height.
   */
  s32 Height() {return height;}

  /** Changes the window size.
   * @param  w New width.
   * @param  h New height.
   * @param  _fullScreen <b>true</b> if the app must be fullscreen, <b>false</b> if windowed.
   */
  void Resize(s32 w, s32 h, bool _fullScreen);

  /** Returns the fullscreen flag.
   * @return <b>true</b> if fullscreen, <b>false</b> if windowed.
   */
  bool IsFullscreen() {return fullScreen;}
  
  /** Returns the color depth.
   * @return Color depth in bits (normally 8, 16, 24 or 32).
   */
  s32 Depth() {return depth;}

  /** Ends the application, exiting from the main loop.
   */
  void Exit() {endLoop = true;}

  /** Returns the active flag.
   * @return <b>true</b> if the application is active (foreground) <b>false</b> if not.
   */
  bool IsActive() {return active;}
  
  /** Returns the pause state of the application.
   * @return <b>true</b> if paused <b>false</b> otherwise.
   */
  bool IsPaused() {return paused;}
  
  /** Pauses or resumes the application.
   * @param  doPause <b>true</b> pauses the application, <b>false</b> resumes.
   * @return if pausing (doPause is <b>true</b>), returns the number
   * of milliseconds from the start of the application. If resuming, 
   * (doPause <b>false</b>), returns the number of milliseconds
   * during pause mode.
   */
  u32 Pause(bool doPause);

  /** Returns the window title.
   * @return Window title.
   */
  const JString &Title() {return title;}

  /** Sets the window title.
   * @param  newTitle New window title..
   */
  void Title(JString &newTitle) {title = newTitle; SDL_WM_SetCaption(title, 0);}

  /** Returns the execution time in milliseconds.
   * @return Execution time in milliseconds.
   */
  u32 AppTime() {return SDL_GetTicks();}

  /** Returns the main SDL surface. The surface is created after Init() 
   * @return SDL main surface.
   */
  SDL_Surface * Screen() {return screen;}

  /** Establishes if events must be captured.
   * @param  b <b>true</b> if they must be <b>false</b> otherwise.
   */
  void DoInput(bool b) {doInput = b;}

  /** Determines if events must be captured.
   * @return <b>true</b> if so <b>false</b> if not.
   */
  bool DoInput() {return doInput;}

  /** Sets the active event callback.
	 * @param _OnActive Callback.
	 */
  void SetOnActive    (void (*_OnActive)(bool active, s32 state)) 
	{__OnActive = _OnActive;}

  /** Sets the kay up event callback.
	 * @param  _OnKeyUp Callback.
	 */
  void SetOnKeyUp     (void (*_OnKeyUp )(SDL_keysym key)) 
	{__OnKeyUp  = _OnKeyUp ;}

  /** Sets the key down event callback.
	 * @param  _OnKeyDown Callback.
	 */
  void SetOnKeyDown   (void (*_OnKeyDown)(SDL_keysym key)) 
	{__OnKeyDown = _OnKeyDown;}

  /** Sets the mouse move event callback.
	 * @param  _OnMouseMove Callback.
	 */
  void SetOnMouseMove (void (*_OnMouseMove)(s32 x, s32 y, s32 bt, s32 xRel, s32 yRel)) 
	{__OnMouseMove = _OnMouseMove;}

  /** Sets the mouse button down event callback.
	 * @param  _OnMouseDown Callback.
	 */
  void SetOnMouseDown (void (*_OnMouseDown)(s32 bt, s32 x, s32 y)) 
	{__OnMouseDown = _OnMouseDown;}

  /** Sets the mouse button up event callback.
	 * @param  _OnMouseUp Callback.
	 */
  void SetOnMouseUp   (void (*_OnMouseUp)(s32 bt, s32 x, s32 y)) 
	{__OnMouseUp = _OnMouseUp;}

  /** Sets the joystick move event callback.
	 * @param  _OnJoyMove Callback.
	 */
  void SetOnJoyMove   (void (*_OnJoyMove)(s32 joyNum, s32 axis, s32 value)) 
	{__OnJoyMove = _OnJoyMove;}

  /** Sets the joystick ball event callback.
	 * @param  _OnJoyBall Callback.
	 */
  void SetOnJoyBall   (void (*_OnJoyBall)(s32 joyNum, s32 ballIndex, s32 xRel, s32 yRel)) 
	{__OnJoyBall = _OnJoyBall;}

  /** Sets the joystick hat event callback.
	 * @param  _OnJoyHat Callback.
	 */
  void SetOnJoyHat    (void (*_OnJoyHat)(s32 joyNum, s32 hatIndex, s32 value)) 
	{__OnJoyHat = _OnJoyHat;}

  /** Sets the joystick button up event callback.
	 * @param  _OnJoyUp Callback.
	 */
  void SetOnJoyUp     (void (*_OnJoyUp)(s32 joyNum, s32 btIndex)) 
	{__OnJoyUp = _OnJoyUp;}

  /** Sets the joystick button down event callback.
	 * @param  _OnJoyDown Callback.
	 */
  void SetOnJoyDown   (void (*_OnJoyDown)(s32 joyNum, s32 btIndex)) 
	{__OnJoyDown = _OnJoyDown;}

  /** Sets the quit event callback.
	 * @param  _OnQuit Callback.
	 */
  void SetOnQuit      (void (*_OnQuit)()) 
	{__OnQuit = _OnQuit;}

  /** Sets the window manager event callback.
	 * @param  _OnSysWM Callback.
	 */
  void SetOnSysWM     (void (*_OnSysWM)()) 
	{__OnSysWM = _OnSysWM;}

  /** Sets the resize event callback.
	 * @param  _OnResize Callback.
	 */
  void SetOnResize    (void (*_OnResize)(s32 w, s32 h)) 
	{__OnResize = _OnResize;}

  /** Sets the video expose event callback.
	 * @param  _OnExpose Callback.
	 */
  void SetOnExpose    (void (*_OnExpose)()) 
	{__OnExpose = _OnExpose;}

  /** Sets the user event callback.
	 * @param  _OnUser Callback.
	 */
  void SetOnUser(void (*_OnUser)(s32 code, void *data1, void *data2)) 
	{__OnUser = _OnUser;}

  /** Returns mouse X position.
	 * @return Mouse X position.
	 */
	s32 MouseX() {return mouseX;}

  /** Returns mouse Y position.
	 * @return Mouse Y position.
	 */
	s32 MouseY() {return mouseY;}

	/** Returns the relative X mouse position (since the last move).
	 * @return Relative X mouse position.
	 */
	s32 MouseRelX() {return mouseRelX;}

	/** Returns the relative Y mouse position (since the last move).
	 * @return Relative Y mouse position.
	 */
	s32 MouseRelY() {return mouseRelY;}

	/** Returns the mouse button press mask.
	 * @return Mouse button state mask.
	 */
	u8 MouseBt() {return mouseBt;}

	/** Returns the keyboard key state. The state for a certain key is determined
	 * with Keys()[SDLK_XXX], where SDLK_XXX is the key to query.
	 * If the value is 0, the key is not pressed, else, it is pressed.
	 * @return Key state array.
	 */
	u8 * Keys() {return keys;}

	/** Returns the key modifier mask. The bits of this mask are those for SDL 
	 * KMOD_XXX, with XXX=LCTRL, LSHIFT, etc.
	 * @return Key modifiers bit mask.
	 */
	SDLMod KeyMods() {return keyMods;}

	/** Returns the joystick X axis
	 * @return joystick X axis
	 */
	s16 JoystickX() {return joyAxisX;}

	/** Returns the joystick Y axis
	 * @return joystick Y axis
	 */
	s16 JoystickY() {return joyAxisY;}

	/** Returns the joystick buttonstate. Note: It does not differentiate between buttons.
	 * @return joystick button
	 */
	u8 JoystickButton() {return joyButton;}

	/** Updates the given region of the primary surface. If all parameters are 0, 
	 * the whole window is updated. This is simply a wrapper for SDL_UpdateRect(...).
	 * @param  x X position of the rectangle to update.
	 * @param  y Y position of the rectangle to update.
	 * @param  w Width of the rectangle to update.
	 * @param  h Height of the rectangle to update..
	 */
	void UpdateRect(s32 x, s32 y, s32 w, s32 h) {SDL_UpdateRect(screen, x, y, w, h);}

	/** Update the given rects. As UpdateRect() is only a wrapper for SDL_UpdateRects().
	 * @param  num Number of rects to update.
	 * @param  rc Array with the rects to update.
	 */
	void UpdateRects(s32 num, SDL_Rect *rc) {SDL_UpdateRects(screen, num, rc);}

	/** Swaps front and back buffer (if SDL_DOUBLEBUF is used as flag during SDL initializacion)
	 * or updates the whole screen. This is a wrapper for SDL_Flip().
	 * @return <b>true</b> if ok, <b>false</b> if an error ocurred..
	 */
	bool Flip() {return -1 != SDL_Flip(screen);}

	/** Returns the audio mixer. Must be initialized in the child class.
	 * @return Audio mixer.
	 */
	JMixer & Mixer() {return mixer;}

	/** Returns this application instance.
	 * @return Pointer to this application.
	 */
	static JApp * App() {return thisApp;}

	/** Sets the number of frames per second (FPS). Its the maximum number of times Draw() is called per second.
	 * @param  newFPS New FPS value.
	 */
	void FPS(s32 newFPS);

	/** Returns the FPS (frames per second). Its the maximum number of times Draw() is called per second.
	 * @return FPS value.
	 */
	s32 FPS() {return fps;}
	
	/** Returns a pointer to the current video mode video info. Is Exactly a wrapper for SDL_GetVideoInfo().
	 * If called before Init() or Resize() it contains the 'best' intended video mode.
	 * @return Information about the current video mode or about the video system in general.
	 */
	const SDL_VideoInfo *VideoInfo() {return SDL_GetVideoInfo();}

	/** Show the video info in the standard output.
	 */
	void DumpVideoInfo();

	/** Parses the application argument.
	 * @param  args Command line arguments.
	 * @param  argc Arguments left.
	 * @return Number of parameters used.
	 */
	virtual int ParseArg(char *args[], int argc);
	
	/** Shows the usage string. It contains the default arguments a JApp can be passed.
   * @param  program Program's name.
	 */
	virtual void PrintUsage(char *program);

	/** Parses the application arguments.
	 * @param  argc Command line argument count.
	 * @param  argv Command line arguments.
	 */
	void ParseArgs(s32 argc, char **argv);
	
	/** Enables or disables sound playback.
   * @param  enable <b>true</b> if it must be enabled, <b>false</b> if not.
	 */
	void SoundEnabled(bool enable) {soundEnabled = (enable && mixer.Valid());}

	/** Shows or hides the mouse cursor.
	 * @param b <b>true</b> to show, <b>false</b> to hide.
	 */
	void MouseCursor(bool b) {SDL_ShowCursor(b ? SDL_ENABLE : SDL_DISABLE);}
	
	/** Returns the viewing state of the mouse cursor.
	 * @param b <b>true</b> if showing, <b>false</b> if not.
	 */
	bool MouseCursor() {return SDL_ENABLE == SDL_ShowCursor(SDL_QUERY);}
 
	/** Sets the file name of the icon to be used for the window. Must be called before init.
	 * In Windows must have exactly 32x32 pixels.
	 * @param  filename Filename of the icon, 0 to use the default icon.
	 */
	void Icon(const char *filename);
	
	/** Returns the sound mixer state. 
	 * @return <b>true</b> if active, <b>false</b> if not or the mixer couldn't be initialized properly.
	 * @see JMixer::Valid(), Mixer().
	 */
	bool SoundEnabled() {return soundEnabled;}

  /** Destroys this application, frees resources and quits the SDL subsystems initialized.
   */
  void Destroy();

  /** Destroys the object.
   */
  virtual ~JApp();
};

#endif  // JAPP_INCLUDED
