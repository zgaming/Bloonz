/*
 * Bloonz: http://www.zgames.com/games/bloonz
 *
 * Created for the Arduboy: http://www.arduboy.com
 * by the zGames team: http://www.zgames.com
 *
 * Adapted from Mystic Balloon v1.5
 * by TEAM a.r.g. : http://www.team-arg.org/more-about.html
 * 2016 - GAVENO - CastPixel -JO3RI
 *
 * Game License: MIT : https://opensource.org/licenses/MIT
 *
 */

// identify the game (used to try and make sure the settings in EEPROM are for this game)
#define  GAME_ID 34

#include "zlib.h"
#include "globals.h"
#include "menu.h"
#include "game.h"
#include "inputs.h"
#include "player.h"
#include "enemies.h"
#include "elements.h"
#include "levels.h"


typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM  mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlaySelect,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGameNextLevel,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
  stateMenuPlayContinue,
  stateMenuPlayNew,
};

void setup()
{
  arduboy.start();
  arduboy.setFrameRate(60);                                 // set the frame rate of the game at 60 fps
  loadSetEEPROM();
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  if (gameState < STATE_GAME_NEXT_LEVEL && arduboy.everyXFrames(10))sparkleFrames = (++sparkleFrames) % 5;
  arduboy.poll();
  arduboy.clearDisplay();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
}

