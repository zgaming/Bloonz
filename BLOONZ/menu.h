/*
 * menus.h
 * 
 * Title screen and menu logic.
 * 
 * www.zgames.com
 */
 
#ifndef MENU_BITMAPS_H
#define MENU_BITMAPS_H

#include <Arduino.h>
#include "globals.h"

#define FONT_TINY                 0
#define FONT_SMALL                1
#define FONT_BIG                  2

#define DATA_TIMER                0
#define DATA_SCORE                1
#define DATA_LEVEL                2

#define MENU_X                    78
#define MENU_Y                    22

byte blinkingFrames = 0;
byte sparkleFrames = 0;
byte cont = 0;

extern void drawNumbers(byte numbersX, byte numbersY, byte fontType, byte data);

// Title screen
void drawTitleScreen()
{
  if (arduboy.everyXFrames(7))blinkingFrames = (++blinkingFrames) % 32;

  // Draw the title screen background
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, titleScreen, i);
  // Draw game title and sparkles
  sprites.drawOverwrite(10, 6, badgeBloonz, 0);
  sprites.drawSelfMasked(5, 5, stars, sparkleFrames);
  // Blink the eyes of the scary guys on the left and right
  sprites.drawSelfMasked(0, 30, leftGuyLeftEye, pgm_read_byte(&blinkingEyesLeftGuy[blinkingFrames]));
  sprites.drawSelfMasked(6, 34, leftGuyRightEye, pgm_read_byte(&blinkingEyesLeftGuy[blinkingFrames]));
  sprites.drawSelfMasked(109, 34, rightGuyEyes, pgm_read_byte(&blinkingEyesRightGuy[blinkingFrames]));
}

// Splash on power up
void stateMenuIntro()
{
  byte y = (64-32)/2;

  // For the first 160 clicks, show the zGames splash
  globalCounter++;
  if (globalCounter < 160)
  {
    // Slide down the splash into position
    if (globalCounter < 32) y = globalCounter >> 1;
    sprites.drawSelfMasked((128-105)/2, y, zgamesSplash, 0);
  }
  else
  {
    // Draw the title screen
    drawTitleScreen();
    // Drop in the main menu after some delay or button press
    if ((globalCounter > 250) || arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
  }
}

// Main menu
void stateMenuMain()
{
  drawTitleScreen();
  // Draw the main menu
  sprites.drawOverwrite(MENU_X, MENU_Y, mainMenu, 0);
  // Menu selection, move immediately
  if (arduboy.justPressed(DOWN_BUTTON) && (menuSelection < 5)) menuSelection++;
  if (arduboy.justPressed(UP_BUTTON) && (menuSelection > 2)) menuSelection--;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = menuSelection;
  // Highlight the selection
  sprites.drawPlusMask(MENU_X-5, MENU_Y + 9 * (menuSelection - 2), selector_plus_mask, 0);
}

// Help menu
void stateMenuHelp()
{
  // Draw QR code
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(32, 32 * i, qrcode, i);
  // Hold it up there until a button press
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

// Info menu
void stateMenuInfo()
{
  // Sparkling game title
  sprites.drawSelfMasked(43, 2, badgeBloonz, 0);
  sprites.drawSelfMasked(38, 0, stars, sparkleFrames);
  sprites.drawSelfMasked(36, 48, madeBy, 0);
  // High score
  EEPROM.get(EEPROM_STORAGE_SPACE_START + EEPROM_BLOONZ_START + OFFSET_HSCORE, scorePlayer);
  // Perfect high score gets extra goodness
  if (EEPROM.read(EEPROM_STORAGE_SPACE_START + EEPROM_BLOONZ_START + OFFSET_COINSHS) == TOTAL_COINS)
  {
    sprites.drawSelfMasked(21, 28, badgeSuper, 0);
  }
  else
  {
    sprites.drawSelfMasked(28, 28, badgeBorder, 0);
  }
  // High Score title
  sprites.drawSelfMasked(30, 28, badgeHighScore, 0);
  drawNumbers(55, 30, FONT_BIG, DATA_SCORE);
  // Hold until a button press
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

// Sound menu
void stateMenuSoundfx()
{
  // Title screen background
  drawTitleScreen();
  // Sound menu goes where main menu was
  sprites.drawOverwrite(MENU_X, MENU_Y, soundMenu, 0);
  if (arduboy.justPressed(DOWN_BUTTON)) arduboy.audio.on();
  if (arduboy.justPressed(UP_BUTTON)) arduboy.audio.off();
  // Highlight selected option
  sprites.drawPlusMask(MENU_X+3, MENU_Y+9 + 9 * arduboy.audio.enabled(), selector_plus_mask, 0);
  // Store on button press
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
}

// Play (Load/New) menu
void stateMenuPlaySelect()
{
  // Title screen background
  drawTitleScreen();
  // Play menu positions at "Play" option main menu position
  sprites.drawOverwrite(MENU_X, MENU_Y+9, continueMenu, 0);
  if (arduboy.justPressed(DOWN_BUTTON)) cont = 1;
  if (arduboy.justPressed(UP_BUTTON)) cont = 0;
  // Highlight selected option
  sprites.drawPlusMask(MENU_X-5, MENU_Y+9 + 9 * cont, selector_plus_mask, 0);
  // Go back on B
  if (arduboy.justPressed(B_BUTTON))
  {
    gameState = STATE_GAME_PLAYCONTNEW + cont;
    cont = 0;
  }
  // Play!
  if (arduboy.justPressed(A_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
  }
}


#endif
