#!/bin/sh

PNGCRUSH="pngcrush -l 9"

convert -background transparent app-icon.svg -resize 16x16 temp.png
$PNGCRUSH temp.png app-icon-16x16.png

convert -background transparent app-icon.svg -resize 32x32 temp.png
$PNGCRUSH temp.png app-icon-32x32.png

convert -background transparent app-icon.svg -resize 48x48 temp.png
$PNGCRUSH temp.png app-icon-48x48.png

convert -background transparent app-icon.svg -resize 64x64 temp.png
$PNGCRUSH temp.png app-icon-64x64.png

rm temp.png
