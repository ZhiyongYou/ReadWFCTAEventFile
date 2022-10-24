#!/bin/bash

exedir=/home/lhaaso/youzhiyong/WFCTA/WFCTADecode/ReadDecode/software

infiledir=/eos/lhaaso/decode/wfcta/2022/1022
outfiledir=/home/lhaaso/youzhiyong/WFCTA/WFCTADecode/ReadDecode/software/data
infilename=ES.147524.FULL.WFCTA12.es-512.20221022042322.010.dat.event.root
outfilename=${infilename%.event.root}.read.root

cd $exedir
./main $infiledir/$infilename $outfiledir/$outfilename
