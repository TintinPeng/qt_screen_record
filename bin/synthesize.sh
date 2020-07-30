#!/bin/zsh
../bin/ffmpeg -i $1.mp4 -i $1.mp3 -t 25 -y $1屏幕录制.mp4
rm -rf $1.mp4 $1.mp3
