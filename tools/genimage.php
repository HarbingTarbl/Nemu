<?php
function TestCard($x, $y) // Generate a testcard resembling Philips PM5544 testcard but with NES colors.
{
  $aspect = 8/7;
  $xcenter = 128; $width =256; $black=0x1D; $xgrid = (($x+8)>>4)-1; $xgridsub = ($x+8)&15;
  $ycenter = 120; $height=240; $white=0x30; $ygrid = (($y+8)>>4)-1; $ygridsub = ($y+8)&15;

  $c = ($xgridsub == 0 || $ygridsub == 0) ? $white // white grid
      : (($ygrid < 0 || $ygrid >= 14)
         ? ($xgrid & 1) ? $white : $black // white or black checkerboard at top & bottom
         : 0x00); // gray otherwise

  if($y > 24 && $ygrid < 13)
  {
    $lobe = Array(0x15/*90*/,0x1A/*250*/, 0x17/*326*/,0x1C/*146*/, 0x14/*0*/,0x18/*180*/);
    $nook = ($ygrid < 3 || ($ygrid >= 11 && ($ygrid>11 || $ygridsub>0)));
    if($xgrid >= 0 && $xgrid <= 1
    && ($xgrid == 0 ? $xgridsub > 0 : $nook)
    ) $c = $lobe[($y < $ycenter) + 2*($xgrid==1)];

    if($xgrid >= 13 && $xgrid <= 14
    && ($xgrid == 14 ? $xgridsub > 0 || $nook : ($nook && $xgridsub > 0))
    ) $c = $lobe[($y < $ycenter) + 4 - 2*($xgrid==13)];
  }

  $xdist = abs($x-$xcenter);
  $ydist = abs($y-$ycenter);
  $xdista = $xdist*$aspect;
  if(sqrt($xdista*$xdista + $ydist*$ydist) >= 6.7*16) return $c;
  $c = $white; // white for circle
  if(($xdist < 8 && $ydist < 24 && $y < 7*16) || $ydist <= 8)
  {
    if($xgridsub != 0 && $ygridsub != 0) $c = $black;
    if($xdist == 0) $c = $white; // white vertical line for center
    return $c;
  }
  switch($ygrid)
  {
    case 1: if( $xdist < $width*0.12 )
      $c = 0x0E + 0x10 * (($x>>3) & 3); // Use four different blacks for the station ID background
      break;
    case 2: if( $xdist > $width*0.18 || $x == (int)($width*0.35)) $c = $black; break;
    case 11: if( $xdist <= $width*0.18 && $x != (int)($width*0.35))
      $c = 0x0F + 0x10 * (($x>>3) & 3); // Use four different blacks for the station ID background
      break;
    case 3: $c = ($x+5)%30 < 15 ? 0x10 : $black; break; // gray or black
    case 4: case 5: case 6:
      $freq = 3 - (int)($x / 64);
      $sub = ($y - 8 - 4*16);
      $c = ((($x >> $freq) ^ (int)($sub / 20)) & 1) ? $white : $black;
      $c |= 0x40 * (int)($sub / 6); // Add vertically changing color de-emphasis
      break;
    case 7: case 8: case 9:
      $sub = (($y - 8*16) >> 3);
      $firstx = $xcenter - 96*$aspect;
      $lastx  = $xcenter + 96*$aspect;
      $xpos = ($x-$firstx) / ($lastx-$firstx);
      if($sub < 4) $c = ((int)(14 * $xpos)&15) + 16 * ($sub&3);
      break;
    case 10:
      $gradient = Array(0x0D,0x1D,0x2D,0x00,0x10,0x3D,0x20,0x30);
      $firstx = $xcenter - 67*$aspect;
      $lastx  = $xcenter + 67*$aspect;
      $xpos = ($x-$firstx) / ($lastx-$firstx);
      $c = @$gradient[(int)(0.5 + (count($gradient)-1) * $xpos)];
      break;
    case 12: case 13: $c = $xdist < 8 ? 0x16 : 0x28; break;
  }
  return $c & 0x1FF;
}

$f = fopen("image.clr", "wb");
for($x = 0; $x < 240; $x++)
{
	for($y = 0; $y < 256; $y++)
	{
		fwrite($f, pack("S1", TestCard($y, $x)), 2);
	}
}
fclose($f);