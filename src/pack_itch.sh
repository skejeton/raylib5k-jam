#!/bin/sh

rm -r itchout
mkdir -p itchout
cp raylib_game.html itchout/index.html
cp raylib_game* itchout
zip itch.zip itchout/* 
