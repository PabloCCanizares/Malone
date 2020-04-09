### Output
#set terminal pdf enhanced font 'Times-Roman,5'
set terminal postscript enhanced color font 'Times-Roman,15'
set output outputfile

### Labels and Tics
set xlabel "Test ID" font 'Times-Roman,15'
set ylabel "Mutant ID" font 'Times-Roman,15'  

### Surface type
set pm3d map
set pm3d interpolate 0,0

### Palette
set palette maxcolors 5
set palette defined (-1 "black", 1 "yellow", 2 "green", 3 "blue", 5 "red")
#set palette defined (0 0 0 0.5, 1 0 0 1, 2 0 0.5 1, 3 0 1 1, 4 0.5 1 0.5, 5 1 1 0, 6 1 0.5 0, 7 1 0 0, 8 0.5 0 0, 9 0 0 0, 10 0 0 0)

### Ranges and cbtics

set cblabel "Exec time (in sec.)"  font 'Times-Roman,15'

### Plot!!!
splot datafile matrix w image notitle


