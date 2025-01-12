#ifndef __SHAPE_H__
#define __SHAPE_H__
#include "light.h"

/**
ym = {{1,0,0},{0,ycos,ysin},{0,-ysin,ycos}}(*RotationMatrix[pitch,{-1,0,0}]*)
{{1,0,0},{0,ycos,ysin},{0,-ysin,ycos}}
pm = {{xcos,0,xsin},{0,1,0},{-xsin,0,xcos}}(*RotationMatrix[yaw,{0,1,0}]*)
{{xcos,0,xsin},{0,1,0},{-xsin,0,xcos}}
rm = {{zcos,zsin,0},{-zsin,zcos,0},{0,0,1}}(*RotationMatrix[roll,{0,0,-1}]*)
{{zcos,zsin,0},{-zsin,zcos,0},{0,0,1}}
rm.pm.ym.{0, 0, 1}//MatrixForm
( {
  {xsin ycos zcos+ysin zsin},
  {ysin zcos-xsin ycos zsin},
  {xcos ycos}
 } )ym = {{1,0,0},{0,ycos,ysin},{0,-ysin,ycos}}(*RotationMatrix[pitch,{-1,0,0}]*)
{{1,0,0},{0,ycos,ysin},{0,-ysin,ycos}}
pm = {{xcos,0,xsin},{0,1,0},{-xsin,0,xcos}}(*RotationMatrix[yaw,{0,1,0}]*)
{{xcos,0,xsin},{0,1,0},{-xsin,0,xcos}}
rm = {{zcos,zsin,0},{-zsin,zcos,0},{0,0,1}}(*RotationMatrix[roll,{0,0,-1}]*)
{{zcos,zsin,0},{-zsin,zcos,0},{0,0,1}}
rm.pm.ym.{0, 0, 1}//MatrixForm
( {
  {xsin ycos zcos+ysin zsin},
  {ysin zcos-xsin ycos zsin},
  {xcos ycos}
 } )
 **/
void calcColor(unsigned char* toFill, Autonoma*, Ray ray, unsigned int depth);

#endif
