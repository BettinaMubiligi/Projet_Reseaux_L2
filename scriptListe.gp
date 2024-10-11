set title "Temps d'exécution :Liste Chainee"
set xlabel "Nombre de chaines"
set ylabel "Temps (secondes)"
set grid
set output 'performanceListe.png'


plot "tempsListe.txt" using 1:2 with lines lw 2 title 'Liste chaînée'
