//a Node.js add-on to wrap a GPU to look like a parallel port

////////////////////////////////////////////////////////////////////////////////
////
/// GpuPort
//

//This is a Node.js add-on to display a rectangular grid of pixels (a texture) on screen using SDL2 and hardware acceleration (via GPU).
//In essence, the RPi GPU provides a 24-bit high-speed parallel port with precision timing.
//Optionally, OpenGL and GLSL shaders can be used for generating effects.
//In dev mode, an SDL window is used.  In live mode, full screen is used (must be configured for desired resolution).
//Screen columns generate data signal for each bit of the 24-bit parallel port.
//Without external mux, there can be 24 "universes" of external LEDs to be controlled.  Screen height defines max universe length.

//Copyright (c) 2015, 2016, 2017, 2018 Don Julien, djulien@thejuliens.net


////////////////////////////////////////////////////////////////////////////////
////
/// Setup
//

//1. Dependencies:
//1a. install nvm
//1b. install node v10.12.0  #or later; older versions might work; ymmv
//1c. install git client
//2. Create parent project
//2a. create a folder + cd into it
//2b. npm init
//2c. npm install --save gpuport
//OR
//2a. git clone this repo
//2b. cd this folder
//2c. npm install
//2d. npm test  #dev mode test, optional

//1d. upgrade node-gyp >= 3.6.2
//     [sudo] npm explore npm -g -- npm install node-gyp@latest
//3. config:
//3a. edit /boot/config.txt
//3b. give RPi GPU 256 MB RAM (optional)


//WS281X notes:
//30 usec = 33.3 KHz node rate
//1.25 usec = 800 KHz bit rate; x3 = 2.4 MHz data rate => .417 usec
//AC SSRs:
//120 Hz = 8.3 msec; x256 ~= 32.5 usec (close enough to 30 usec); OR x200 = .0417 usec == 10x WS281X data rate
//~ 1 phase angle dimming time slot per WS281X node
//invert output
//2.7 Mbps serial date rate = SPBRG 2+1
//8+1+1 bits = 3x WS281X data rate; 3 bytes/WS281X node
//10 serial bits compressed into 8 WS281X data bits => 2/3 reliable, 1/3 unreliable bits
//5 serial bits => SPBRG 5+1, 1.35 Mbps; okay since need to encode anyway?


////////////////////////////////////////////////////////////////////////////////
////
/// Headers, general macros, inline functions
//

#include "sdl-helpers.h"

//C++11 implements a lot of SDL functionality in a more C++-friendly way, so let's use it! :)
#if __cplusplus < 201103L
 #pragma message("CAUTION: this file probably needs c++11 to compile correctly")
#endif

#define LIMIT_BRIGHTNESS  (3*212) //limit R+G+B value; helps reduce power usage; 212/255 ~= 83% gives 50 mA per node instead of 60 mA; safely allows 300 LEDs per 20A at "full" (83%) white

//#define rdiv(n, d)  int(((n) + ((d) >> 1)) / (d))
//#define divup(n, d)  int(((n) + (d) - 1) / (d))

//#define SIZE(thing)  int(sizeof(thing) / sizeof((thing)[0]))

//#define return_void(expr) { expr; return; } //kludge: avoid warnings about returning void value

//inline int toint(void* val) { return (int)(long)val; }
//#define toint(ptr)  reinterpret_cast<int>(ptr) //gives "loses precision" warning/error
//#define toint(expr)  (int)(long)(expr)

//#define CONST  //should be const but function signatures aren't defined that way

//typedef enum {No = false, Yes = true, Maybe} tristate;
//enum class tristate: int {No = false, Yes = true, Maybe, Error = Maybe};

//#define uint24_t  uint32_t //kludge: use pre-defined type and just ignore first byte

//kludge: need nested macros to stringize correctly:
//https://stackoverflow.com/questions/2849832/c-c-line-number
//#define TOSTR(str)  TOSTR_NESTED(str)
//#define TOSTR_NESTED(str)  #str

//#define CONCAT(first, second)  CONCAT_NESTED(first, second)
//#define CONCAT_NESTED(first, second)  first ## second


////////////////////////////////////////////////////////////////////////////////
////
/// unit test:
//

#ifdef WANT_UNIT_TEST
#undef WANT_UNIT_TEST //prevent recursion

//#include <iostream> //std::cout
#include "sdl-helpers.h"
#include "msgcolors.h"
#include "srcline.h"


//int main(int argc, const char* argv[])
void unit_test()
{
//    SDL_Lib sdllib(SDL_INIT_VIDEO, SRCLINE);
    SDL_AutoSurface surf(xx);
    debug(BLUE_MSG << "finish" << ENDCOLOR);
//    return 0;
}

#endif //def WANT_UNIT_TEST
//eof