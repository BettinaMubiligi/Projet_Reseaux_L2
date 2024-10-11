set title "Temps d'execution : Table de Hachage vs ArbreQuat"
set xlabel "Nombre de chaines"
set ylabel "Temps (secondes)"
set grid 

set output 'performanceHachageArbre.png'

plot "tempsHachageArbre.txt" using 1:2 with lines lw 2 title 'Table hachage taille 10', \
     "tempsHachageArbre.txt" using 1:3 with lines lw 2 title 'Table hachage taille 500', \
     "tempsHachageArbre.txt" using 1:4 with lines lw 2 title 'Table hachage taille 1000', \
     "tempsHachageArbre.txt" using 1:5 with lines lw 2 title 'ArbreQuat'
