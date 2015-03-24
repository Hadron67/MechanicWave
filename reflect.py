w=wave.getwave()
for i in range(0,400):
	for j in range(0,i):
		w.setv(j,i,0.6)
for i in range(150,350):
	w.addsource(wave.wavesource(i,50,80,2,0))
