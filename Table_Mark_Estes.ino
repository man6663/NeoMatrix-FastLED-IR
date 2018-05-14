#include "Table_Mark_Estes.h"

#define zeds ledmatrix

// -------------------------------------------------------------------------------
// Common functoions used by the animations

void lfado(byte bbc)
{
  for ( byte hhh = 0; hhh < MATRIX_WIDTH ; hhh++)
    for (byte jjj = 0; jjj < MATRIX_HEIGHT ; jjj++)
      zeds(hhh, jjj).fadeToBlackBy(bbc);//% = bbc/255
}

void redfado(byte bbc) {
  for ( byte hhh = 0; hhh < MATRIX_WIDTH ; hhh++)
    for (byte jjj = 0; jjj < MATRIX_HEIGHT ; jjj++)
      if (flip2)
        zeds(hhh, jjj) -= CRGB(random(bbc / 2), random(bbc), random(bbc));//leave more red
      else
        zeds(hhh, jjj) -= CRGB(random(bbc / 2), random(bbc / 2), random(bbc));// leave more yellow
}

void greenfado(byte bbc) {
  for ( byte hhh = 0; hhh < MATRIX_WIDTH ; hhh++)
    for (byte jjj = 0; jjj < MATRIX_HEIGHT ; jjj++)
      if (!flip3)
        zeds(hhh, jjj) -= CRGB(random(bbc ), random(bbc / 2), random(bbc));
      else
        zeds(hhh, jjj) -= CRGB(random(bbc ), random(bbc / 2), random(bbc / 2));// leave more teal
}

void bluefado(byte bbc) {
  for ( byte hhh = 0; hhh < MATRIX_WIDTH ; hhh++)
    for (byte jjj = 0; jjj < MATRIX_HEIGHT ; jjj++)
      if (flip2)
        zeds(hhh, jjj) -= CRGB(random(bbc ), random(bbc), random(bbc / 2));
      else
        zeds(hhh, jjj) -= CRGB(random(bbc / 2 ), random(bbc), random(bbc / 2)); //leave more purple

}
void fakenoise()
{
  faudio[0] = random(1, MIDLY);
  for (byte i = 1; i < MATRIX_WIDTH; i++) {
    faudio[i] = faudio[i - 1] + random(10) - 7;
    faudio[i] = constrain(faudio[i ], 1, MATRIX_HEIGHT - 2);
  }
  if (counter % 600 == 0) {
    faudio[random(MATRIX_WIDTH)] = constrain( (MATRIX_HEIGHT / - random(MIDLY >> 2, MIDLY)), 2, MATRIX_HEIGHT - 1);
  }
}

void audioprocess()
{
  fakenoise();
}

void adjuster() {  // applies the screen wide effect
  switch (adjunct) {
    case 0://no effect
      break;
    case 1://no effect
      break;
    case 2:
      zeds.HorizontalMirror();
      zeds.VerticalMirror();
      break;
    case 3:
      zeds.QuadrantBottomTriangleMirror();
      break;
    case 4:
      zeds.HorizontalMirror();
      break;
    case 5:
      zeds.VerticalMirror();
      break;
    case 6:
      zeds.QuadrantRotateMirror();
      break;
    case 7:
      zeds.TriangleTopMirror();
      break;
    case 8:
      zeds.QuadrantMirror();
      break;

    default:// no effect
      break;
  }
}



// -------------------------------------------------------------------------------
// Called from the main code 
// run this before each demo

void td_init() {
  // File-wise init
  driftx = random8(4, MATRIX_WIDTH - 4);//set an initial location for the animation center
  drifty = random8(4, MATRIX_HEIGHT - 4);// set an initial location for the animation center
  mstep = byte( 256 / (MATRIX_WIDTH - 1)); //mstep is the step size to distribute 256 over an array the width of the matrix
 steper = random8(2, 8);// steper is used to modify h to generate a color step on each move
  lastmillis = millis();
  lasttest = millis();
  randomSeed(analogRead(0) - analogRead(3) + analogRead(5));
  hue = random8();//get a starting point for the color progressions
  mscale = 2.2;
  fakenoise();
  cangle = (sin8(random(25, 220)) - 128.0) / 128.0;//angle of movement for the center of animation gives a float value between -1 and 1
  sangle = (sin8(random(25, 220)) - 128.0) / 128.0;//angle of movement for the center of animation in the y direction gives a float value between -1 and 1

  // Per pattern init
  targetfps = random(20, 30);
  bfade = random(1, 8);
  dot = random(2, 6);// controls the size of a circle in many animations
  adjunct = (random(3, 11));//controls which screen wide effect is used if any

  dwell = 1000 * (random(20, 40));//set how long the pattern will play
  ringdelay = random(30, 90);//sets how often one of the effects will happen
  bringdelay = random(70, 105);//sets how often one of the effects will happen

  counter = 0;// reset the counter with each new pattern
  steper = random(2, 8);//color tempo
//  shifty = random (3, 12);//how often the drifter moves

  blender = random8();
//  if (random8() > 180)
//  {
//    if (random8() > 120)
//      blackringme = true;
//    else
//      ringme = true;
//  }

  flip = false;
  flip2 = false;
  flip3 = false;
  if (random8() > 127)
    flip = true;
  if (random8() > 127)
    flip2 = true;
  if (random8() > 127)
    flip3 = true;

  hue += random(64);//picks the next color basis
  h = hue;
  // new drift factors for x and y drift
  cangle = (sin8(random(25, 220)) - 128.0) / 128.0;
  sangle = (sin8(random(25, 220)) - 128.0) / 128.0;
}

void td_loop() {
  hue += 1;//increment the color basis
  h = hue;  //set h to the color basis
  if (counter % shifty == 0) driftx =  driftx + cangle;//move the x center every so often
  if (driftx > (MATRIX_WIDTH - MIDLX / 2))//change directin of drift if you get near the right 1/4 of the screen
    cangle = 0 - abs(cangle);
  if (driftx < MIDLX / 2)//change directin of drift if you get near the right 1/4 of the screen
    cangle = abs(cangle);
  if ((counter + shifty / 2) % shifty == 0) drifty =  drifty + sangle;//move the y center every so often
  if (drifty > ( MATRIX_HEIGHT - MIDLY / 2))// if y gets too big, reverse
    sangle = 0 - abs(sangle);
  if (drifty < MIDLY / 2) {// if y gets too small reverse
    sangle = abs(sangle);
  }

  driftx = constrain(driftx, MIDLX - MIDLX / 3, MIDLX + MIDLX / 3);//constrain the center, probably never gets evoked any more but was useful at one time to keep the graphics on the screen....
  drifty = constrain(drifty, MIDLY - MIDLY / 3, MIDLY - MIDLY / 3);
  // test for frame rate,  every 15 frames

  // this calls the pre frame fade,based on a byte "bfade" which is mostly random, but sometimes assignes for a specific effect
  switch (bfade)
  {
    case 0:
      lfado(3);//almost none  3/256 or 1.17%
      break;
    //cases 5,6,and 7 cause the fade to be non symetric, favoring one color over others, this adds a bit of fun to the ghosts
    case 5:
      bluefado(8 + (random8() >> 2));
      break;
    case 6:
      redfado(8 + (random8() >> 2));
      break;
    case 7:
      greenfado(8 + (random8() >> 2));
      break;

    default:
      lfado(bfade << 4);// 1 = 16,  4 = 64, this sets the amont of fade as a fraction over 256  range is 16/256 (6.25%) i.e. not much  to 64/256 (25%)or considerable
      break;
  }
  counter++;//increment the counter which is used for many things
}

// ------------------------------------------------------------------

void corner()//pattern=10
{
  zeds.DrawFilledRectangle(0 , 0,  MATRIX_WIDTH, MATRIX_HEIGHT, CHSV(h, 255, 255));
  h += steper;

  for (byte y = 0; y <= MATRIX_WIDTH / 2 - 1 ; y++)
  {
    zeds.DrawFilledCircle(MIDLX, MIDLY, (MATRIX_WIDTH / 2 + 1 - y) / 3, CHSV(h  + y * steper, 255, 255));
    zeds.DrawFilledCircle(0, 0, MATRIX_WIDTH / 2 - y, CHSV(64 + h + y * steper, 255, 255));
    zeds.DrawFilledCircle(0, MATRIX_HEIGHT - 1, MATRIX_WIDTH / 2 - y, CHSV(h - 64 + y * steper, 255, 255));
    zeds.DrawFilledCircle(MATRIX_WIDTH  - 1, 0, MATRIX_WIDTH / 2 - y, CHSV(h - 64 + y * steper, 255, 255));
    zeds.DrawFilledCircle(MATRIX_WIDTH  - 1, MATRIX_HEIGHT - 1, MATRIX_WIDTH / 2 - y, CHSV(h + 64 + y * steper, 255, 255));
  }
}
void bkringer() {
  if (counter >= bringdelay)
  {
    if (counter - bringdelay <= MATRIX_WIDTH + 13)
    {
      zeds.DrawCircle(driftx, drifty , (MATRIX_WIDTH + 12 - counter + bringdelay), CHSV(h + 60, 255, 255));
      zeds.DrawCircle(driftx, drifty, (MATRIX_WIDTH + 8 - counter + bringdelay), CHSV(h + 70 , 255, 255));
      zeds.DrawCircle(driftx, drifty , (MATRIX_WIDTH + 4 - counter + bringdelay), CHSV(h + 80 , 255, 255));
      zeds.DrawCircle(driftx, drifty , (MATRIX_WIDTH  - counter + bringdelay), CHSV(h + 90 , 255, 255));
    }
    else {
      bringdelay = counter + random(60, 120);

    }
  }
}

void ringer() {
  if (counter >= ringdelay)
  {
    if (counter - ringdelay <= MATRIX_WIDTH)
    {
      //if (flip || flip2)
        zeds.DrawCircle(driftx, drifty , counter - ringdelay, CHSV( h + 85, 255, 255));
      //else
      //  zeds.DrawCircle(driftx, drifty , counter - ringdelay, CRGB::White);
    }
    else
      ringdelay = counter + random(30, 80);
  }
}



void whitewarp() {
  if (counter == 0 )
  {
    howmany = random (MIDLX * 3 / 2, MATRIX_WIDTH + 4 );

    for (int i = 0; i < howmany; i++) {
      fcount[i] = random8(); //angle
      fcolor[i] = random8();
      fpeed[i] = random(2, 12);
      xfire[i] = driftx;
      yfire[i] = drifty;

    }
  }

  for (int i = 0; i < howmany; i++)
  {
    xfire[i] = xfire[i] + fpeed[i] / 4.0 * (sin8(fcount[i] + h ) - 128.0) / 128.0;
    yfire[i] = yfire[i] + fpeed[i] / 4.0 * (cos8(fcount[i] + h ) - 128.0) / 128.0;

    //if (!flip2)
    //  zeds(xfire[i], yfire[i]) = CRGB::White;
    //else
      zeds(xfire[i], yfire[i]) = CHSV(fcolor[i] , 255, 255); //one shade of color

    if (xfire[i] < 0 || yfire[i] < 0 || xfire[i] > MATRIX_WIDTH || yfire[i] > MATRIX_HEIGHT) {
      xfire[i] = driftx;
      yfire[i] = drifty;
      fcount[i] = random8(); //angle
      fcolor[i] = random8();;
      fpeed[i] = random8(2, 12);
    }
  }
  //if (!flip2)
    zeds(xfire[howmany - 1], yfire[howmany - 1]) = CHSV(fcolor[howmany - 1] , 255, 255);//many color
  //else
   // zeds(xfire[howmany - 1], yfire[howmany - 1]) = CRGB::White;
}

void Raudio()  // rotating ring,  dots  , freq rotates, colors do not
{
  for (int i = 0; i < MATRIX_WIDTH ; i++) {
    xangle =  (sin8(i * mstep  +  h) - 128.0) / 128.0;
    yangle =  (cos8(i * mstep  +  h) - 128.0) / 128.0;
    zeds(MIDLX + xangle * (MATRIX_WIDTH / 2 - faudio[i] / mscale) , MIDLY + yangle * (MATRIX_HEIGHT / 2 - faudio[i] / mscale)) = CHSV(i * mstep , 255, 255);
  }
  audioprocess();
}

void spire() {

  if (counter == 0)
  {
    radius =  MATRIX_WIDTH / 2 - 3;
    flip = true;
    radius2 =  5;
    flip2 = false;
    dot = dot + 3 + random(5);
  }

  if (h % 16 == 0)
  {
    if (radius < 5)
      flip = 1 - flip;
    if (radius > MATRIX_WIDTH / 2)
      flip = 1 - flip;
    if (flip)
      radius --;
    else
      radius++;

    if (radius2 < 5)
      flip2 = 1 - flip2;
    if (radius2 > MATRIX_WIDTH / 2)
      flip2 = 1 - flip2;
    if (flip2)
      radius2 --;
    else
      radius2++;
  }
}


// vim:sts=2:sw=2