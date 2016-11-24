#!/bin/sh

./release/img2label -s train/connectedcomponents/word/AosugiruHaru.xml -l 1 images/connectedcomponents/AosugiruHaru/word/*
./release/img2label -s train/connectedcomponents/word/Arisa.xml -l 1 images/connectedcomponents/Arisa/word/*
./release/img2label -s train/connectedcomponents/word/BakuretsuKungFuGirl.xml -l 1 images/connectedcomponents/BakuretsuKungFuGirl/word/*
./release/img2label -s train/connectedcomponents/word/DollGun.xml -l 1 images/connectedcomponents/DollGun/word/*
./release/img2label -s train/connectedcomponents/word/EvaLady.xml -l 1 images/connectedcomponents/EvaLady/word/*
./release/img2label -s train/connectedcomponents/word/LoveHina.xml -l 1 images/connectedcomponents/LoveHina/word/*

./release/img2label -s train/connectedcomponents/notword/AosugiruHaru.xml -l -1 images/connectedcomponents/AosugiruHaru/notword/*
./release/img2label -s train/connectedcomponents/notword/Arisa.xml -l -1 images/connectedcomponents/Arisa/notword/*
./release/img2label -s train/connectedcomponents/notword/BakuretsuKungFuGirl.xml -l -1 images/connectedcomponents/BakuretsuKungFuGirl/notword/*
./release/img2label -s train/connectedcomponents/notword/DollGun.xml -l -1 images/connectedcomponents/DollGun/notword/*
./release/img2label -s train/connectedcomponents/notword/EvaLady.xml -l -1 images/connectedcomponents/EvaLady/notword/*
./release/img2label -s train/connectedcomponents/notword/LoveHina.xml -l -1 images/connectedcomponents/LoveHina/notword/*

./release/img2label -s train/connectedcomponentsregions/word/Belmondo.xml -l 1 images/connectedcomponentsregions/Belmondo/word/*
./release/img2label -s train/connectedcomponentsregions/word/Donburakokko.xml -l 1 images/connectedcomponentsregions/Donburakokko/word/*
./release/img2label -s train/connectedcomponentsregions/word/GarakutayaManta.xml -l 1 images/connectedcomponentsregions/GarakutayaManta/word/*
./release/img2label -s train/connectedcomponentsregions/word/Jyovolley.xml -l 1 images/connectedcomponentsregions/Jyovolley/word/*
./release/img2label -s train/connectedcomponentsregions/word/MagicStarGakuin.xml -l 1 images/connectedcomponentsregions/MagicStarGakuin/word/*
./release/img2label -s train/connectedcomponentsregions/word/MariaSamaNihaNaisyo.xml -l 1 images/connectedcomponentsregions/MariaSamaNihaNaisyo/word/*

./release/img2label -s train/connectedcomponentsregions/notword/Belmondo.xml -l -1 images/connectedcomponentsregions/Belmondo/notword/*
./release/img2label -s train/connectedcomponentsregions/notword/Donburakokko.xml -l -1 images/connectedcomponentsregions/Donburakokko/notword/*
./release/img2label -s train/connectedcomponentsregions/notword/GarakutayaManta.xml -l -1 images/connectedcomponentsregions/GarakutayaManta/notword/*
./release/img2label -s train/connectedcomponentsregions/notword/Jyovolley.xml -l -1 images/connectedcomponentsregions/Jyovolley/notword/*
./release/img2label -s train/connectedcomponentsregions/notword/MagicStarGakuin.xml -l -1 images/connectedcomponentsregions/MagicStarGakuin/notword/*
./release/img2label -s train/connectedcomponentsregions/notword/MariaSamaNihaNaisyo.xml -l -1 images/connectedcomponentsregions/MariaSamaNihaNaisyo/notword/*